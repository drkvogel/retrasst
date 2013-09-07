use strict;
use warnings;
use File::Copy;
use Cwd;

my $target = 'Make';
my @projects = glob '*.cbproj';
die unless @projects == 1;
my $project = shift @projects;
print "Building $project...\n";

exit 1;

$ENV{BDS} = "C:/Program Files (x86)/Embarcadero/RAD Studio/9.0";
$ENV{FrameworkDir}="C:/Windows/Microsoft.NET/Framework/v3.5";
my @buildArgs = ();
push @buildArgs, $project;
push @buildArgs, "/p:Config=Release";
push @buildArgs, "/t:$target";
push @buildArgs, "/verbosity:diagnostic";
system("$ENV{FrameworkDir}/MSBuild.exe", @buildArgs) == 0 or die "Failed to build: $!";


1;

