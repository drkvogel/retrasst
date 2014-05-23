use strict;
use warnings;
use File::Copy;
use File::Path;
use Cwd;

sub copyFile {
    my ( $from, $to ) = @_;
    copy( $from, $to ) or die "Failed to copy $from to $to: $!";
    print "Copied $from to $to\n";
}

my $project = 'valc';
my $target  = 'Build';
my $config  = 'Release';
my $releaseDir = 'K:/corex/LIMS/ValC';

if ( -e 'Win64' ) {
    die unless rmtree('Win64');
}

die "Found remnants of an earlier build.\n" if ( -e 'Win64' );

$ENV{BDS} = "C:/Program Files (x86)/Embarcadero/RAD Studio/11.0";
$ENV{FrameworkDir}="C:/Windows/Microsoft.NET/Framework/v3.5";
my @buildArgs = ();
push @buildArgs, "$project.cbproj";
push @buildArgs, "/p:Config=$config";
push @buildArgs, "/t:$target";
push @buildArgs, "/verbosity:diagnostic";
my $builtOK = ( 0 == system("$ENV{FrameworkDir}/MSBuild.exe", @buildArgs) );

print "[Enter] to continue...";
<STDIN>;

if ( $builtOK )
{
    copyFile( "Win64/$config/$project.exe", "$releaseDir/" );
}
 
print "[Enter] to quit...";
<STDIN>;

1;

