use strict;
use warnings;
use File::Copy;
use Cwd;

my $userArg = shift(@ARGV);
my ($target,$config);
my $project = 'LUATests';

$userArg = "Make" unless defined($userArg);

if ( $userArg eq 'Make' ) {
    $target = 'Make';
    $config = 'Debug';
}
elsif ( $userArg eq 'Release' ) {
    $target = 'Build';
    $config = 'Release';
}
else {
    print STDERR "Target not specified. Valid values: Make, Release.\n";
}

$ENV{BDS} = "C:/Program Files (x86)/Embarcadero/RAD Studio/11.0";
$ENV{FrameworkDir}="C:/Windows/Microsoft.NET/Framework/v3.5";
my @buildArgs = ();
push @buildArgs, "$project.cbproj";
push @buildArgs, "/p:Config=$config";
push @buildArgs, "/t:$target";
push @buildArgs, "/verbosity:diagnostic";
system("$ENV{FrameworkDir}/MSBuild.exe", @buildArgs) == 0 or die "Failed to build $project.cbproj: $!";

system("Win64/Debug/LUATests.exe");

1;

