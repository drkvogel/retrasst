use strict;
use warnings;
use DBI;
use DBUtil;

my %tables = (
    buddy_database      => "create_table_buddy_database.sql",
    buddy_result_float  => "create_table_buddy_result_float.sql",
    buddy_worklist      => "create_table_buddy_worklist.sql",
    c_cluster_machine   => "create_table_c_cluster_machine.sql",
    c_project           =>   "create_table_c_project.sql",
    c_test              => "create_table_c_test.sql",
    c_worklist_relation => "create_table_c_worklist_relation.sql",
    sample_run          => "../create_table_sample_run.sql",
    valc_worklist       => "create_table_valc_worklist.sql",
    qc_rule             => "create_table_qc_rule.sql",
    qc_rule_config      => "create_table_qc_rule_config.sql",
    c_object_name       => "create_table_c_object_name.sql",
    qc_lot              => "create_table_qc_lot.sql",
    qc_gate             => "create_table_qc_gate.sql",
);

my %sequences = (
    valc_next_id => "create_sequence_valc_next_id.sql",
    sample_run_id => "../create_sequence_sample_run_id.sql", );

sub confirm {
    my $msg = shift;
    print $msg;
    print "\nOK? (yes or no)\n";
    my $userResponse = <STDIN>;
    chomp $userResponse;
    die "Script execution cancelled by user.\n" unless ( $userResponse eq 'yes' );
}

sub createDBObjects {

    my ($vnode, $db ) = @_;

    my @sqlCmds;

    foreach my $table (keys %tables) {
        my $sqlCmd;
        my $scriptFile = $tables{$table};
        open my $file, '<', $scriptFile or die "Could not open $scriptFile: %!";
        $sqlCmd = join ' ', <$file> ;
        push @sqlCmds, $sqlCmd;
        close $file;
    }

    foreach my $seq (keys %sequences) {
        my $sqlCmd;
        my $scriptFile = $sequences{$seq};
        open my $file, '<', $scriptFile or die "Could not open $scriptFile: %!";
        $sqlCmd = join ' ', <$file> ;
        push @sqlCmds, $sqlCmd;
        close $file;
    }


    runSQLOnIngres( \@sqlCmds, $vnode, $db );
}

sub dropDBObjects {

    my ($vnode, $db) = @_;

    my @sqlCmds;

    foreach my $table (keys %tables) {
        my $sqlCmd  = "drop table $table";
        push @sqlCmds, $sqlCmd;
    }

    foreach my $seq (keys %sequences) {
        my $sqlCmd = "drop sequence $seq";
        push @sqlCmds, $sqlCmd;
    }

    runSQLOnIngres( \@sqlCmds, $vnode, $db );
}

my ($action, $vnode, $db, $dsn, $vnode_vlab) = @ARGV;
my $workdir = 'workdir';
my $buddyPC = 'au680-1016.ctsu.ox.ac.uk';
my $QC_PROJECT_ID = -1234;
my $USAGE = <<EOF;
Usage:

    build_new_db.pl action vnode database [ODBC_data_source_name vnode_vlab]

    Valid values for 'action': clean, build

    Whether vnode and vnode_vlab are 64bit or 32bit depends on the 
    executable described by 'terminalMonitor' (in DBUtil.pm).

    ODBC_data_source_name should be 32bit.

    ODBC_data_source_name is an ODBC datasource which references 
    the same database as vnode::database.  A value for this argument is 
    only required if the 'action' is 'build'.

    vnode_vlab is the name of the local vnode for vlab. Again, a value for this argument
    is only required if the 'action' is 'build'.

    If the 'action' is build, then this script creates a subdirectory '$workdir'.  
    Refuses to run if a subdirectory with this name already exists.

EOF

die $USAGE unless ( defined($action) && ( $action eq 'clean' || $action eq 'build' ) );

die $USAGE if ( ($action eq 'build') && ! defined( $vnode_vlab ) );

if ( $action eq 'clean' ) {
    confirm( "Dropping tables and sequences from $vnode\::$db" );
    dropDBObjects( $vnode, $db );
}
else {
    confirm( "Building $vnode\::$db from scratch, using $buddyPC" );
    die "Please delete or rename '$workdir'.\n"     if ( -e $workdir );
    die "Failed to create directory '$workdir': $!" unless mkdir $workdir;
    createDBObjects( $vnode, $db );
    dumpAll( $workdir, $buddyPC, $vnode_vlab );
    buildAll( 
        $workdir,
        $QC_PROJECT_ID,
        {
            vnode   => $vnode,
            db      => $db,
            dsn     => $dsn,
        }
        );
}


