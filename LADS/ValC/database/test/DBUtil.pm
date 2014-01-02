package DBUtil;
our @EXPORT = qw( runSQLOnIngres dumpAll buildAll );
use base qw(Exporter);
use strict;
use warnings;
use DBI;
use Data::Dumper;
use File::Slurp qw(edit_file);
use YAML;

my $verbose = 0;
my $terminalMonitor = "\"C:/Program Files/Ingres/IngresIJ/ingres/bin/Sql.exe\"";

sub runSQLOnIngres {
    my ($cmds, $vnode, $db) = @_;
    
    unshift @{$cmds}, "set autocommit on";

    my $vnodeDB = sprintf( "%s::%s", $vnode, $db );

    open my $dbSession, "|$terminalMonitor $vnodeDB" or die "Failed to connect to $vnodeDB: $!";

    foreach my $cmd ( @{$cmds} ) {
        print $dbSession "$cmd\\g\n";
        print $dbSession "\\r\n";
    }

    print $dbSession "\\q\n";

    close $dbSession;
}


sub logIt {
    my $funcName = shift @_;
    my $func = shift @_;
    print '____________________________________',"\n";
    print "$funcName\n";
    print '____________________________________',"\n";
    print Dump(@_);
    print '____________________________________',"\n";
    $func->(@_);
    print '____________________________________',"\n";
}

sub removeIfExists {
    my ($filename) = @_;

    unlink $filename if ( -e $filename );

    die "$filename already exists." if ( -e $filename );
}

sub cmdOutToFile {
    my ($cmd, $outFilename) = @_;

    removeIfExists( $outFilename );

    open my $outFile, '>', $outFilename or die "Failed to open $outFilename: $!";

    open my $cmdOut, "$cmd|" or die "$!";

    while ( <$cmdOut> ) {
        print $outFile $_;
    }

    close $cmdOut;
    close $outFile;
}

sub dumpFromMySQL {
    my ($params) = @_;
    my $host  = $params->{host};
    my $db    = $params->{db};
    my $table = $params->{table};
    my $user  = $params->{user};
    my $outFilename = $params->{outFilename};
    my $where = '';
    if ( $params->{restriction} ) {
        $where = sprintf("-w\"%s\"", $params->{restriction} );
    }
    # Example restriction: 'buddy_sample_id > 475800';

    logIt 'cmdOutToFile', \&cmdOutToFile, "mysqldump $where --xml -h $host -u $user $db $table", $params->{outFilename};
}


sub toCSV {
    my ($inFile, $outFilename) = @_;
    cmdOutToFile( "xslt.bat -IN $inFile -XSL to_csv.xsl", $outFilename );
}



sub uploadToIngres {
    my ($params) = @_;
    my $vnode           = $params->{vnode};
    my $db              = $params->{db};
    my $table           = $params->{table};
    my $colSpecFunc     = $params->{colSpecFunc};
    my $tableDataFile   = $params->{tableDataFile};
    my $yesTruncate     = defined( $params->{truncateYesNo} ) && ( 'yes' eq $params->{truncateYesNo} );

    # Get rid of any dodgy extra commas in tableDataFile
    edit_file { s/Lp\(a\),\s*Apo/Lp(a) Apo/g } $tableDataFile;
    
    my $colSpec = $colSpecFunc->();#`xslt $tableXMLFile create_copy_cmd_col_list.xsl`;

    my $copyStmt = sprintf( "copy %s ( %s ) FROM '%s' WITH ON_ERROR = TERMINATE, ERROR_COUNT = 1, ROLLBACK = ENABLED ",
        $table, $colSpec, $tableDataFile );

    my @sqlCmds;
    
    if ( $yesTruncate ) {
        push @sqlCmds, "modify $table to truncated";
    }

    push @sqlCmds, $copyStmt;

    logIt 'runSQLOnIngres', \&runSQLOnIngres, \@sqlCmds, $vnode, $db;
}

sub dumpIngresTable {
    my ($args) = @_;
    my $colSpecFunc = $args->{colSpecFunc};
    my $vnode       = $args->{vnode};
    my $db          = $args->{db};
    my $table       = $args->{table};
    my $outFile     = $args->{outFile};

    removeIfExists( $outFile );

    my $colSpec = $colSpecFunc->();

    my $copyCmd = sprintf( "copy %s ( %s ) INTO '%s' WITH ON_ERROR = TERMINATE, ERROR_COUNT = 1, ROLLBACK = ENABLED ", 
        $table, $colSpec, $outFile );

    logIt 'runSQLOnIngres', \&runSQLOnIngres, [ $copyCmd ], $vnode, $db;
}

sub colSpecFromCreateTableScript {
    my ($tableCreationScript) = @_;
    
    my @cols = ();
    
    open my $file_handle, '<',  $tableCreationScript or die "Failed to open $tableCreationScript: $!";

    while ( <$file_handle> ) {
        if ( /^\s+(\w+)\s+/ ) {
            my $col = $1;
            unless ( /--\s*NEW/ ) { push @cols, "$col = TEXT(0)comma with null('rabbit')" };
        }
    }
    close $file_handle;

    $cols[ -1 ] =~ s/comma/nl/;

    join ",\n", @cols;
}

# -1019429, AU680 1041, -1019430
# -1019349, AU680 1016, -1019430

# The following subroutines are to do with deriving QC worklist entries 
# from QC entries in buddy_database.
sub createQCIterator {
    my ($dataSource,$username) = @_;

    my $connectionString = "dbi:ODBC:DSN=$dataSource";
    my $dbh = DBI->connect( $connectionString, $username, undef, undef ) or die "Oh dear.";#$DBI::err;

    my $SQL = <<EOF;
select 
buddy_sample_id,
barcode,
machine_cid,
date_analysed,
database_name,
alpha_sample_id,
result_type,
note_exists,
delete_status,
update_when,
rack_number,
cup_number,
test_type,
sample_type
from buddy_database bd 
where bd.barcode like 'QC%'
order by buddy_sample_id
EOF
    my $stmt = $dbh->prepare( $SQL ) or die $dbh->errstr;

    $stmt->execute or die $stmt->errstr;

    sub {

        my $row = undef;

        if ( $dbh ) {

            $row = $stmt->fetchrow_hashref;

            unless( $row ) {
                $dbh->disconnect;
                $dbh = undef;
            }
        }

        return $row;
    }
}

sub dateOnly {
    my ($dateTime) = @_;
    ( split ' ', $dateTime ) [0];
}

sub ifNull {
    my ($arg1, $arg2) = @_;

    return $arg1 ? $arg1 : $arg2;
}

# QCIterator:
#   Rows from buddy_database that represent QCs
#
# testInfoSource:
#   Given a row from buddy_database that represents a QC, testInfoSource 
#   returns the tests for which worklist entries should be created.
#
# recNumSequence:
#   For obtaining 'record_no' values that do not clash with existing entries in valc_worklist
sub dumpQCs {
    my ($QCIterator, $testInfoSource, $qcProjectID, $recNumSequence, $outFilename) = @_;
    my $ts_sequence = 100;

    removeIfExists( $outFilename );

    open my $outFile, '>', $outFilename or die "Failed to open $outFilename: $!";

    while( my $qc = $QCIterator->() ) {

        my $tests = $testInfoSource->( $qc );
        
        foreach my $test ( @{$tests} ) {

            unless ( defined( $test->{diluent} ) ) {
                warn( 'Diluent not defined for test ' . $test->{test_id} . ' for QC ' . Dump($qc) );
                $test->{diluent} = 0;
            }

            my $status = 'P';
            my %actFlagToStatus = ( '0' => 'C', '1' => 'F', '2' => 'V', '3' => 'T', );

            next unless ( $test->{action_flag} ) && exists( $actFlagToStatus{ $test->{action_flag} } );

            my @colValues = (
                [ '%d', $recNumSequence->() ], #record_no
                [ '%d', $qc->{machine_cid}  ],
                [ '%s', $qc->{barcode}      ],
                [ '%d', $test->{test_id}    ], 
                [ '%d', 0                   ], #group_id
                [ '%d', 0                   ], #category_id
                [ '%d', 0                   ], #sample_id
                [ '%s', 'N'                 ], #range_set
                [ '%d', $qcProjectID        ], #project_cid
                [ '%d', 0                   ], #profile_id
                [ '%s', '.'                 ], #profile_name
                [ '%s', ifNull( $test->{date_analysed}, $qc->{date_analysed} )  ], #time_stamp
                [ '%d', $ts_sequence++      ],
                [ '%s', dateOnly($qc->{date_analysed}) ], #in_date
                [ '%d', 0                   ], #trigger_id
                [ '%d', 0                   ], #lower_trigger_limit
                [ '%d', 0                   ], #upper_trigger_limit
                [ '%s', 'N'                 ], #private_result
                [ '%s', $actFlagToStatus{ $test->{action_flag} }], #status
                [ '%f', $test->{diluent}    ], #diluent
                [ '%d', exists( $test->{buddy_result_id} ) ? $test->{buddy_result_id} : 0 ] #buddy_result_id

            );

            my @formatList;
            my @formatArgs;

            foreach my $colVal (@colValues) {
                push @formatList, $colVal->[0];
                push @formatArgs, $colVal->[1];
            }

            my $formatString = join ',', @formatList;
            
            printf $outFile "$formatString\n", @formatArgs;
        }
    }

    close $outFile;
}

# Returns a subroutine which, when invoked for a given QC entry (from buddy_database), queries qc_test_machine and
# returns the tests that are configured for that QC on the machine on which it was run.
# Updated (27 June 2013) so tolerant of absence of any configuration for the given QC. Even tolerant of 
# when cannot find its level in qc_level.  Have experienced this for QCRCl00117.
sub createTestInfoSource {
    my ($dataSource) = @_;
    my $dbh = DBI->connect( "dbi:ODBC:DSN=$dataSource", undef, undef, {AutoCommit => 1} ) or die "Oh dear";#$DBI::err;
    my $query_qc_level = 'select record_cid from qc_level where material = ? and qc_level = ? and status != 99';
    my $query_qc_test_machine = 'select test_cid, diluent from qc_test_machine where level_cid = ? and machine_cid = ? and status != 99';

    sub {
        my ($qc) = @_;
        my $tests = [];
        my $level_cid = undef;

        die "Bad barcode: $qc->{barcode}" unless ( $qc->{barcode} =~ /QC(\w{2})(\w)/ );

        my ($material, $level, $machine_cid) = ( $1, $2, $qc->{machine_cid} );

        my $stmt = $dbh->prepare( $query_qc_level ) or die $dbh->errstr;

        $stmt->execute( $material, $level );

        my $row = $stmt->fetchrow_arrayref;

        if ( $row ) {
            $level_cid = $row->[0];
        } 
        else {
            print "Failed to obtain QC level identifier.\n$query_qc_level\nparams: $material, $level\nbarcode: $qc->{barcode}\n";
        }

        if ($level_cid) {

            $row = $stmt->fetchrow_arrayref;

            die "More than one row" if $row;

            print "level_cid: $level_cid; machine_cid: $machine_cid\n" if ($verbose);

            $stmt = $dbh->prepare( $query_qc_test_machine ) or die $dbh->errstr;

            $stmt->execute( $level_cid, $machine_cid );

            while ( $row = $stmt->fetchrow_arrayref ) {
                die unless ( defined( $row->[1] ) ); # insist on a value for diluent
                push @{$tests}, { test_id => $row->[0], diluent => $row->[1] };
            }

            my $numTests = @{$tests};
            print "No tests listed.\n\tmaterial: $material\n\tlevel: $level\n\tmachine: $machine_cid\n" unless ( $numTests > 0 );
        }

        return $tests;
    }
}

# Used to wrap/adapt subroutine instances created by createTestInfoSource, so that the return values from 
# the invocation of such instances can be supplemented with extra detail from buddy_result_float.
# Thus, as well as learning the range of tests configured for a QC entry in buddy_database, it can
# also be learnt which tests (if any) have actually been run for that QC.
sub supplementUsingBuddyResultFloat {
    my ($dataSource,$username, $delegate) = @_;
    my $dbh = DBI->connect( "dbi:ODBC:DSN=$dataSource", $username, undef, undef ) or die "Oh dear.";#$DBI::err;
    my $query = $dbh->prepare('select buddy_result_id, test_id, date_analysed, action_flag from buddy_result_float where buddy_sample_id = ?') or 
        die $dbh->errstr;

    sub {
        my ($qc) = @_;

        my $tests = $delegate->( @_ );

        my %keyedOnID = map { $_->{test_id} => $_ } @{$tests};

        my $numTestsConfigured = scalar @{$tests};

        $query->execute( $qc->{buddy_sample_id} ) or die $query->errstr;

        while ( my $row = $query->fetchrow_hashref ) {
            my $testID = $row->{test_id};
            my $diluent = exists $keyedOnID{$testID} ? $keyedOnID{$testID}->{diluent} : undef;
            $keyedOnID{$testID} = $row; 
            $row->{diluent} = $diluent;
        }

        my @v = values( %keyedOnID );

        my $numTestsAfterIncludingActualResults = scalar @v;

        if ( $numTestsAfterIncludingActualResults > $numTestsConfigured ) {
            warn "Results exceed configuration for QC " . Dump($qc);
        }

        return \@v;
    }
}

# Wraps a subroutine reference so that, if a subroutine is called twice with the same arguments,
# on the second (and subsequent) occasion the wrapped subroutine is not called, but the 
# value obtained the first time is returned.
sub useCaching {
    my ($delegate, $keyGenFunc) = @_;
    my %cache;

    sub {
        my $key = $keyGenFunc->(@_);

        unless ( exists $cache{$key} )
        {
            $cache{$key} = $delegate->( @_ );
        }

        return $cache{$key};
    }
}

sub MaterialLevelMachine {
    my ($qc) = @_;
    my $barcode = $qc->{barcode};
    my $machine = $qc->{machine_cid};
    die "Could not parse barcode: $barcode." unless( $barcode =~ /^QC(\w{2})(\w)/ );
    my ($material,$level) = ($1,$2);
    join ':', $material,$level,$machine;
}

sub createValcWorklistRecNumSequence {
    my ($dataSource) = @_;
    my $dbh = DBI->connect( "dbi:ODBC:DSN=$dataSource", undef, undef, {AutoCommit => 1} ) or die "Oh dear";#die $DBI::err;
    my $row = $dbh->selectrow_arrayref( 'select max(record_no) from valc_worklist' ) or die $dbh->errstr;
    my $sequenceVal = $row->[0];
    die unless ( defined($sequenceVal) );

    sub {
        ++$sequenceVal;
    }
}

sub dumpAll {
    my ($toDir,$clusterHost, $vnode_vlab) = @_;

    die ("Must specify vnode_vlab vnode name.") unless $vnode_vlab;

    foreach my $table ( qw/ buddy_database buddy_result_float buddy_worklist / ) {

        my $params = {
            host => $clusterHost,
            db   => 'ldbc',
            table => $table,
            user  => 'ODBC',
            outFilename => "$toDir/$table.dump",
        #    restriction => 'buddy_sample_id > 475800',
        };

        dumpFromMySQL( $params );
    }

    foreach my $table( qw/ c_project c_cluster_machine / ) {
        dumpIngresTable( {
            vnode       => $vnode_vlab, 
            db          => 'ldbc', 
            table       => $table,
            outFile     => "$toDir/$table.dump",
            colSpecFunc => sub { colSpecFromCreateTableScript("create_table_$table.sql") },
        } 
        );
    }

    dumpIngresTable( {
        vnode       => $vnode_vlab, 
        db          => 'ldbc', 
        table       => 'c_buddy_worklist',
        outFile     => "$toDir/c_buddy_worklist.dump",
        colSpecFunc => sub { colSpecFromCreateTableScript('create_table_valc_worklist.sql') },
    } 
    );
}

sub addWorklistEntriesForQCs {
    my ($dumpDir,$qcProjectID, $targetDB) = @_;

    dumpQCs( 
        createQCIterator( $targetDB->{dsn} ),
        supplementUsingBuddyResultFloat( 
            $targetDB->{dsn}, 
            undef, 
            useCaching( createTestInfoSource( 'dsn_lab_ingres' ), \&MaterialLevelMachine ) ),
        $qcProjectID,
        createValcWorklistRecNumSequence( $targetDB->{dsn} ),
        "$dumpDir/qc_worklist_entries.dump"
    );

    print Dump($targetDB);

    uploadToIngres( {
        vnode           => $targetDB->{vnode},
        db              => $targetDB->{db},
        table           => 'valc_worklist',
        colSpecFunc     => sub { colSpecFromCreateTableScript('create_table_valc_worklist.sql') },
        tableDataFile   => "$dumpDir/qc_worklist_entries.dump",
        truncateYesNo   => 'no',
        }
    );
}

sub addWorklistEntriesForReruns {
    my ($dumpDir, $targetDB) = @_;

    # Iterate over buddy_worklist (in what sort order?)
    #       Order by ifnull( duplicate, 9,999,999 ) 
    #           so that:
    #               => Q-ed entries sort last
    #               => Reruns appear later than their ancestors
    #               However, note that only-queued-never-run will sort last, even if was rerun
    #               So let's say, up-front, that we're only in the business of identifying reruns of 
    #               entries that were at least programed the first time round.
    #               But what if the 1st was only Q-ed, the 2nd got a result, the 3rd got a result - 
    #               in danger of representing the 2nd as the original, when it was, in fact, a rerun.
    #               This is where matching on valc_worklist has value - because they will be the originals.
    # If a profile_name contains the string 'rerun' (case-insensitive), then
    #       Find its parent
    #           Find the set of buddy_worklist entries that share the same barcode and test_id and, 
    #           from within this set, identifying the single instance with the highest value for 'duplicate' 
    #           (i.e. buddy_result_id) that is less than any buddy_result_id value associated with the Rerun instance. 
    #           OR
    #           Find the most recent (higest record_no) entry in valc_worklist that matches on barcode and test_id
    #       Insert the rerun into valc_worklist
    #           Does the 'duplicate' reference a result? 
    #               If so, give buddy_result_id a value.
    #               Also need to give the parent a value for buddy_result_id, if it doesn't have one, and if one is available.
    #                   This could be done as a second pass. Possibly, might then need to remove the newly inserted reruns, 
    #                   if a buddy_result_id for the original cannot be found.
    #       Insert into worklist_relation, to capture the rerun relationship
}

sub truncateIngresTable {
    my ($table, $vnode, $db ) = @_;
    my @sqlCmds = ();

    push @sqlCmds, "modify $table to truncated";

    logIt 'runSQLOnIngres', \&runSQLOnIngres, \@sqlCmds, $vnode, $db;
}


sub buildAll {
    my ($dumpDir,$qcProjectID, $targetDB) = @_;

    foreach my $table ( qw/ c_worklist_relation sample_run / ) {
        truncateIngresTable( 
            $table, 
            $targetDB->{vnode},
            $targetDB->{db},
            );
    }

    foreach my $table ( qw/ buddy_database buddy_result_float buddy_worklist / ) {

        toCSV( "$dumpDir/$table.dump", "$dumpDir/$table.csv" );

        uploadToIngres( {
            vnode         => $targetDB->{vnode},
            db            => $targetDB->{db},
            table         => $table,
            colSpecFunc   => sub { `xslt.bat -IN "$dumpDir/$table.dump" -XSL create_copy_cmd_col_list.xsl`},
            tableDataFile => "$dumpDir/$table.csv",
            truncateYesNo => 'yes',
            } );
    }

    uploadToIngres( {
        vnode           => $targetDB->{vnode},
        db              => $targetDB->{db},
        table           => 'valc_worklist',
        colSpecFunc     => sub { colSpecFromCreateTableScript('create_table_valc_worklist.sql') },
        tableDataFile   => "$dumpDir/c_buddy_worklist.dump",
        truncateYesNo   => 'yes',
        } );

    uploadToIngres( {
        vnode           => $targetDB->{vnode},
        db              => $targetDB->{db},
        table           => 'c_test',
        colSpecFunc     => sub { colSpecFromCreateTableScript('create_table_c_test.sql') },
        tableDataFile   => "./c_test.dump",
        truncateYesNo   => 'yes',
        } );

    foreach my $table ( qw/ c_project c_cluster_machine / ) {
        uploadToIngres( {
            vnode           => $targetDB->{vnode},
            db              => $targetDB->{db},
            table           => $table,
            colSpecFunc     => sub { colSpecFromCreateTableScript("create_table_$table.sql") },
            tableDataFile   => "$dumpDir/$table.dump",
            truncateYesNo   => 'yes',
            } );
    }

    addWorklistEntriesForQCs( $dumpDir, $qcProjectID, $targetDB );

    addWorklistEntriesForReruns( $dumpDir, $targetDB );
}

1;

