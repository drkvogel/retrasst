use strict;
use warnings;
use File::Copy;
use Cwd;

my ($config,$target) = ('Debug','Make');

my @projects = glob '*.cbproj';
die unless @projects == 1;
my $project = shift @projects;
print "Building $project...\n";

$ENV{BDS} = "C:/Program Files (x86)/Embarcadero/RAD Studio/11.0";
$ENV{FrameworkDir}="C:/Windows/Microsoft.NET/Framework/v3.5";
my @buildArgs = ();
push @buildArgs, $project;
push @buildArgs, "/p:Config=$config";
push @buildArgs, "/t:$target";
push @buildArgs, "/verbosity:diagnostic";
system("$ENV{FrameworkDir}/MSBuild.exe", @buildArgs) == 0 or die "Failed to build: $!";

my $exe = $project;
$exe =~ s/cbproj/exe/;

print "Running $exe...\n";

system("Win64/$config/$exe"); 

1;

