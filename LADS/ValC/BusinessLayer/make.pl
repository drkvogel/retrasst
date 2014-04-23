use strict;
use warnings;
use File::Copy;
use Cwd;

my $project = 'business_layer_lib';
my $target = 'Make';
my $config = 'Debug';

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

1;

