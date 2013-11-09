use strict;
use warnings;
use DBUtil;

my ($vnode, $db) = ( 'cp_brat_64', 'paulst_test2' );

my $idSequence = 1;

my @configs = ( { name => 'a',}, { name => 'b' } );

my @machines = ( -1019429, -1019349 );

my $projectID = -1234; #QC project

my @tests = ( -12669171, -12591095, -1031390, -1031389, -1031388, -1031387, -1031386, -1031385, -1031384 );

my @sqlCmds = ();

sub queueSQL {
    my ($sql) = @_;
    push @sqlCmds, $sql;
}

queueSQL( "delete from c_object_name" );
queueSQL( "delete from qc_rule_config" );
queueSQL( "delete from qc_rule" );

foreach my $config (@configs) {
    $config->{'id'} = ++$idSequence;
    my $sql=  sprintf( 
        "insert into c_object_name (object_cid, external_name, external_full ) values( %d, 'QC_CONFIG_%s', 'QC configuration %s' )",
        $config->{'id'}, $config->{'name'}, $config->{'name'} );
    queueSQL( $sql );
}

# Next: qc_rule_config */
# Assumed defaults: status = 1, valid_from = now, valid_to = 1/1/2020 */

for ( my $i = 0; $i < 2; ++$i ) {
    my $config  = $configs [$i];
    my $machine = $machines[$i];
    foreach my $test (@tests) {
        my $sql = sprintf(
            "insert into qc_rule_config (record_cid, rule_cid, test_cid, machine_cid, project_cid ) values(%d, %d, %d, %d, %d)",
            ++$idSequence, $config->{'id'}, $test, $machine, $projectID);
        queueSQL( $sql );
    }
}

my $simpleScript = <<EOF;
  function onLoad() end 
  function applyRules( qc ) return {}, "ok", 1 end
EOF

# Next: qc_rule
# Assumed defaults: status = 1, valid_from = now, valid_to = 01/01/2020
foreach my $config (@configs) {
    my $sql = sprintf(
        "insert into qc_rule( record_cid, rule_cid, return_type, internal_name, script ) values( %d, %d, 1, 'current', '%s' )",
            ++$idSequence, $config->{'id'}, $simpleScript );
    queueSQL( $sql );
}

runSQLOnIngres( \@sqlCmds, $vnode, $db );

