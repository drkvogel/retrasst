# Populates the following tables:
#   qc_lot
#   qc_gate
#   qc_rule
#   qc_rule_config
#
use strict;
use warnings;
use DBI;
use Data::Dumper;
use DBUtil;
use File::Slurp;

my $configRule = {
        script_file     => 'rule_config.lua',
    };

my $componentRules = [
    {
        internal_name   => '3:2s',
        script_file     => 'rule_3_2s.lua',
    },];


my ($vnode, $db)        = ( 'cp_brat_64', 'paulst_test3' );
my $dataSource          = 'paulst_test3_on_brat';
my $connectionString    = "dbi:ODBC:DSN=$dataSource";
my $username            = undef;
my $qcProjectID         = -1234;

my $last = undef;

sub connectToDB {
    DBI->connect( $connectionString, $username, undef, {AutoCommit => 1} ) or die $DBI::errstr;
}

sub requiresDifferentGateFromTheRowBefore {
    my ($qcResult) = @_;
    my $oldLast = $last;
    $last = join ":", ( @{$qcResult}[0,1,2] ); # qc_id, machine_id, test_id
    return ( ! defined( $oldLast ) ) || ( $oldLast ne $last );
}

# Entries in qc_lot are derived from entries in buddy_result_float.
sub buildQCLot {
    my $distinctBarcodePatterns = <<EOF;
        declare global temporary table session.qc_barcode_patterns as
        select distinct left( barcode, 8 ) as barcode_pattern
        from
        buddy_result_float brf, buddy_database bd
        where
        bd.buddy_sample_id = brf.buddy_sample_id
        and
        bd.barcode like 'QC%'
        on commit preserve rows with norecovery
EOF

    my $insertIntoQC_LOT = <<EOF;
        insert into qc_lot ( 
                qc_id                 , lot            , lot_descrip    , barcode_pattern, qc_status, level_cid )
        select  valc_next_id.nextval, barcode_pattern, barcode_pattern, barcode_pattern,   2      ,  0
        from
        session.qc_barcode_patterns
EOF

    my @sqlCmds = ();
    push @sqlCmds, "delete from qc_lot";
    push @sqlCmds, $distinctBarcodePatterns;
    push @sqlCmds, $insertIntoQC_LOT;
    runSQLOnIngres( \@sqlCmds, $vnode, $db );
}

# Entries in qc_gate are derived from entries in buddy_result_float.
sub buildQCGate {
    my $QCLotResults = <<EOF;
        select ql.qc_id, bd.machine_cid, brf.test_id, brf.res_value
        from buddy_result_float brf, buddy_database bd , qc_lot ql
        where 
        brf.buddy_sample_id = bd.buddy_sample_id
        and 
        left( bd.barcode, 8 ) = left( ql.barcode_pattern, 8 )
        order by 1, 2, 3
EOF

    my $insertStmt = <<EOF;
        insert into qc_gate 
        ( qc_id, machine_cid, test_cid, mean, stddev, record_cid          , status, source ) 
        values
        ( %d    ,    %d     ,    %d   ,  %f ,  %f   , valc_next_id.nextval, 1     , 1      )
EOF

    my @sqlCmds = ();
    push @sqlCmds, "delete from qc_gate";

    my $dbh = connectToDB;

    my $qcResultQuery = $dbh->prepare( $QCLotResults ) or die $dbh->errstr;

    $qcResultQuery->execute or die $qcResultQuery->errstr;

    my $qcResult = $qcResultQuery->fetchrow_arrayref;

    while( $qcResult ) {
        if ( requiresDifferentGateFromTheRowBefore( $qcResult ) ) {
            my ($qcID, $machineID, $testID, $resultValue) = @{$qcResult};
            push @sqlCmds, sprintf( $insertStmt, $qcID, $machineID, $testID, $resultValue, (abs($resultValue)/10.0) );
        }
        $qcResult = $qcResultQuery->fetchrow_arrayref;
    }

    $dbh->disconnect or warn $dbh->errstr;

    runSQLOnIngres( \@sqlCmds, $vnode, $db );
}

# Inserts into c_object_name and qc_rule
sub insertRule {
    my ( $dbh, $ruleID, $ruleName, $scriptFile, $returnType ) = @_;

    my $insertRule = <<EOF;
        insert into qc_rule
        ( record_cid           , rule_cid, return_type, internal_name, script , status, valid_from, valid_to )
        values
        ( valc_next_id.nextval ,    ?    ,      ?     ,        ?     ,   ?    ,   1   ,date('now'), date('now') + 200 )
EOF

    my $insertObjectName = <<EOF;
        insert into c_object_name
        ( object_cid, process_cid, external_name, external_full, object_type, status, time_stamp )
        values
        (     ?     ,    0       ,      ?       ,      ?       ,     15     ,   1   , date('now') )
EOF

    my $script = read_file( $scriptFile );

    $dbh->do( $insertObjectName, undef, $ruleID, $ruleName, $ruleName            ) or die $dbh->errstr;
    $dbh->do( $insertRule      , undef, $ruleID, $returnType, $ruleName, $script ) or die $dbh->errstr;
}

sub insertRuleConfig {
    my ( $dbh, $ruleID, $testID, $machineID, $projectID ) = @_;

    my $insertSQL = <<EOF;
        insert into qc_rule_config 
        ( record_cid               , rule_cid, test_cid, machine_cid, project_cid , status, valid_from , valid_to          )  values
        (  valc_next_id.nextval    , ?       , ?       , ?          , ?           , 1     , date('now'), date('now') + 200 )
EOF

    $dbh->do( $insertSQL, undef, $ruleID, $testID, $machineID, $projectID ) or die $dbh->errstr;
}


sub deleteExistingRules {

    my $dbh = connectToDB;

    $dbh->do( "delete from c_object_name where object_type = 15" ) or die $dbh->errstr;
    $dbh->do( "delete from qc_rule"                              ) or die $dbh->errstr; 
    $dbh->do( "delete from qc_rule_config"                       ) or die $dbh->errstr; 

    $dbh->disconnect;
}

sub applyQCConfigRuleForAllQCs {
    my ($configRule) = @_;

    my $dbh = connectToDB;

    # Query for list of distinct test/machine values
    my $testMachines = $dbh->selectall_arrayref( 'select distinct machine_cid, test_cid from qc_gate' );

    my $ruleID = 0;

    # for each test/machine
    foreach my $testMachine (@{$testMachines}) {
        my ($machineID, $testID) = @{$testMachine};
        # create a ruleID and ruleName
        my $ruleName = sprintf( "rule%d", ++$ruleID );
        # Insert into c_object_name
        # Insert into qc_rule
        insertRule( $dbh, $ruleID, $ruleName, $configRule->{script_file}, 1 );

        # Insert into qc_rule_config
        insertRuleConfig( $dbh, $ruleID, $testID, $machineID, $qcProjectID );
    }

    $dbh->disconnect or warn $dbh->errstr;
}

sub insertComponentRules {
    my ($rules) = @_;

    my $dbh = connectToDB;

    my $ruleID = 100;

    foreach my $rule (@{$rules}) {
        insertRule( $dbh, ++$ruleID, $rule->{internal_name}, $rule->{script_file}, 0 );
    }

    $dbh->disconnect;
}

buildQCLot;
buildQCGate;
deleteExistingRules;
applyQCConfigRuleForAllQCs($configRule);
insertComponentRules($componentRules);

