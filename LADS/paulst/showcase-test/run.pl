use strict;
use warnings;

my $classpath = 'target/showcase-test-1.0-SNAPSHOT.jar';
$classpath .= ';C:/maven_repo/log4j/log4j/1.2.14/log4j-1.2.14.jar';

my $urlFile = 'urls.txt';
my $threads = 35;
my $logFile = 'results.log';

unlink $logFile;

my @args = qw/ java -cp /;
push( @args, $classpath );
push( @args, 'uk.ac.ox.ctsu.paulst.App' );
push( @args, $urlFile );
push( @args, $threads );

system( @args );

open my $fh, '<', $logFile or die "Failed to open file $logFile: $!\n";

while (<$fh>)
{
    print;
}

0;

