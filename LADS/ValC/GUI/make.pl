use strict;
use warnings;
use File::Copy;
use Cwd;

sub copyFile {
    my ( $from, $to ) = @_;
    copy( $from, $to ) or die "Failed to copy $from to $to: $!";
    print "Copied $from to $to\n";
}

my $project = 'valc-gui';
my $target  = 'Make';
my $config  = 'Debug';
my $appDataLocal = 'C:/Users/paulst/AppData/Local/ValC';

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
    copyFile( "../BusinessLayer/config.txt", "$appDataLocal/config-businesslayer.txt" );
    copyFile( "./config-copies/config-gui.txt"          , "$appDataLocal/config-gui.txt" );
    system("Win64/$config/$project.exe");
}
 

1;

