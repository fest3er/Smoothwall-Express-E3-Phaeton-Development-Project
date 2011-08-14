#! /usr/bin/perl

# Get libs
use lib "/usr/lib/smoothwall";
use header qw(:standard);


# Define global vars
my $script = "";
my $tcdir = "${swroot}/mods/trafficControl";
my $settingsfile = "${tcdir}/settings";
# devdir: works-in-progress, can delete and save; can move to perm
my $devdir = "${tcdir}/dev";
# permdir: protected scripts; can copy, move and save to dev
my $permdir = "${tcdir}/perm";
# stockdir: scripts as shipped; cannot move; can copy and save to dev
my $stockdir = "${tcdir}/stock";


# Function getDirname() finds the dir containing the config
#   One argument: the name of the config to find, sans intentions and extensions.

sub getDirname {
  local($name) = @_;

  # Find the JS config to send to browser
  if (-e "$stockdir/$name.js")
  {
    return $stockdir;
  }
  elsif (-e "$permdir/$name.js")
  {
    return $permdir;
  }
  elsif (-e "$devdir/$name.js")
  {
    return $devdir;
  }
  else
  {
    return "";
  }
}


# Function findScript() finds the dir containing the RC script and returns it.
#   One argument: the name of the script to find, sans intentions and extensions.

sub findScript {
  my ($fname) = @_;
  my $dname;

  # Find the JS config to send to browser
  $dname = &getDirname($fname);

  if ($dname ne "")
  {
    return "$dname/rc.$fname";
  }
  else
  {
    return "";
  }
}


# Start of main code
#

# If the settings file exists, try to start/stop TC
if (-e $settingsfile)
{
  # Get the settings.
  &readhash ($settingsfile, \%settings);

  # find the script to run
  $script = &findScript($settings{'CURRENT_CONFIG'});

  # Run it, if it was found
  if ($script ne "")
  {
    if ($settings{'STATE'} eq "off")
    {
      system("/bin/bash", $script, "stop");
    }
    elsif ($settings{'STATE'} eq "on")
    {
      system("/bin/bash", $script, "restart");
    }
  }
}
