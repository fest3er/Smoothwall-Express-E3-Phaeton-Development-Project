#! /usr/bin/perl -w

# traffic.cgi
#
# Copyright 2009, Neal P. Murphy. All rights reserved.

# License is granted to use and distribute this software with the
#   free Smoothwall Express firewall system.
# Contact neal.p.murphy@alum.wpi.edu for more information.


# Get libs
use File::Copy;
use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );
use smoothtype qw(:standard);


# Define global vars
my (%cgiparams, %selected, %checked, $success, $count, $dname);
my ($errormessage, $script_name, $config_name, $plain_name, $cfggrp_name);
my ($dir_name, $dir_cfggrp);
my $tcdir = "${swroot}/mods/trafficControl";
my $settingsfile = "${tcdir}/settings";

# devdir: works-in-progress, can delete; can save to (overwrite);
#   can move to perm; can move to dev (rename)
my $devdir = "${tcdir}/dev";

# permdir: protected configs; cannot delete; can copy, move and save to dev
my $permdir = "${tcdir}/perm";

# stockdir: configs as shipped; cannot move or delete from; cannot save to;
#   can copy and save to dev
my $stockdir = "${tcdir}/stock";

# Config names must be unique among dev, perm and stock.

# Configs must be moved from perm to dev before they can be deleted.
# Configs can be copied from dev to perm to prevent accidental deletion.
# Configs can be copied from stock to dev to be modified.

# 'Save Changes' ALWAYS saves to dev.



# Function files2RadioButtons() lists the files in DIR as radio buttons
#   belonging to GRP.
#   Two arguments: DIR: the name of the directory to read
#                  GRP: the name of the radio button group

sub files2RadioButtons {
  local($DIR, $GRP, $BKT) = @_;

  @dirlist = &getFiles($DIR);
  foreach $i (0 .. $#dirlist)
  {
    my $tmp = $dirlist[$i];
    my $idtmp = $tmp;
    $idtmp =~ s/ //;
    print "      <p style='margin:.3em 0 0 0; vertical-align:middle'>\n";
    print "        <input type='radio' name='$GRP' value='$tmp'\n";
    if ($tmp eq "$cgiparams{'CFGGRP'}")
    {
      print " checked='checked'";
    }
    print "               id='$idtmp'\n";
    # Do this later; it needs a JS function
    # print "               onclick='setButtons(\"$BKT\");'\n";
    print "               style='margin:0 .3em 0 1em; vertical-align:middle' />\n";
    print "        <label for='$idtmp'>\n";
    $tmp =~ s/\.js//;
    print "          $tmp\n";
    print "        </label>\n";
    print "<br />\n";
  }
}


# Function getFiles() returns an array of the files in the dir.
#   One argument: the name of the directory to read.

sub getFiles {
  local($dirname) = @_;

  opendir (DIRHDL, $dirname);
  my @dirlist = grep (/\.js$/, sort (readdir(DIRHDL)));
  closedir (DIRHDL);

  return @dirlist;
}


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
    &addFeedback ("trafficControl (getDirname): config '$name' not found!", "error");
    return "";
  }
}


# Function getConfig() finds the dir containing the config and loads it
#   into cgiparams.
#   One argument: the name of the config to find, sans intentions and extensions.

sub getConfig {
  my $fname = $_;
  my $dname;

  # Find the JS config to send to browser
  $dname = &getDirname($fname);

  if ($dname ne "")
  {
    # It's an open and shut case
    open (HDL, "<$dname/$fname.js");
    $cgiparams{'textconfig'} = <HDL>;
    close (HDL);
  }
}


# Function doShorthand() resets script/config info. It is called upon entry
#   and when a script/config is deleted (requiring the script/config to be
#   reset to the currently selected one
#

sub doShorthand {
  # Prepare the file names
  if (defined($cgiparams{'FILENAME'}))
  {
    $script_name = "rc.$cgiparams{'FILENAME'}";
    $config_name = "$cgiparams{'FILENAME'}.js";
    $plain_name = "$cgiparams{'FILENAME'}";
  }
  else
  {
    $script_name = "rc.$settings{'CURRENT_CONFIG'}";
    $config_name = "$settings{'CURRENT_CONFIG'}.js";
    $plain_name = "$settings{'CURRENT_CONFIG'}";
    $cfggrp_name = "$cgiparams{'CFGGRP'}";
    $cfggrp_name =~ s/.js//;
  }
  # Find the script's domicile
  $dir_name = &getDirname($plain_name);
  $dir_cfggrp = &getDirname($cfggrp_name);
}


# Function addFeedback() adds the message to $errormessage using the
#   specified color.
#   Defined severities: error, warning, note. Anything else is charcoal grey.

# Feedback message colors (red, amber, green, charcoal)
%fbColor = ('error', "#ef0000", 'warning', "#bb9900",
             'note', "#008f00", 'anon', "#3f3f3f");

sub addFeedback
{
  local ($msg, $severity);
  # Give the severity the default value if not specified
  if (!defined $_[1])
  {
    $_[1] = "anon";
  }
  ($msg, $severity) = @_;
  # And give it the default value if it's unknown
  if (!defined $fbColor{$severity})
  {
    $severity = "anon";
  }

  # Add the message
  $errormessage .= "<span style='color:$fbColor{$severity}'>$msg</span><br />\n";
}



# Start of main code
#

# Start the HTTP/HTML
&showhttpheaders();

# If the settings file doesn't exist, create it and set the defaults.
if (! -e $settingsfile)
{
  $settings{'DEFAULT_CONFIG'} = "Throttled with Exceptions";
  $settings{'CURRENT_CONFIG'} = $settings{'DEFAULT_CONFIG'};
  $settings{'STATE'} = "off";
  &writehash ($settingsfile, \%settings);
}

# Get all the POST/GET vars and the settings.
&getcgihash(\%cgiparams);
&readhash ($settingsfile, \%settings);

# ensure ACTION is defined
if (!defined($cgiparams{'ACTION'}))
{
  $cgiparams{'ACTION'} = "";
}

# ensure CFGGRP is defined
if (!defined($cgiparams{'CFGGRP'}))
{
  $cgiparams{'CFGGRP'} = "$settings{'CURRENT_CONFIG'}.js";
}

&doShorthand();

######################
## DEBUG: display all vars
if ($debug eq "true")
{
  foreach $key (keys %cgiparams)
  {
    &addFeedback("cgiparams[$key]=>$cgiparams{$key}");
  }
  foreach $key (keys %settings)
  {
    &addFeedback("settings[$key]=>$settings{$key}");
  }
  &addFeedback("swroot=>$swroot<br />tcdir=>$tcdir<br />settingsfile=>$settingsfile");
  &addFeedback("devdir=>$devdir<br />permdir=>$permdir<br />stockdir=>$stockdir");
  &addFeedback("script_name=>$script_name<br />config_name=>$config_name");
  &addFeedback("plain_name=>$plain_name<br />dir_name=>$dir_name");
}
#######################

# And finally take action!
#

if ($cgiparams{'ACTION'} eq "Save Changes")
{
  # Saving config and script

  local ($name, $newName, $dir);

  $name = $cgiparams{'CFGGRP'};
  $name =~ s/\.js$//;
  $dir = &getDirname ($name);
  $shortdir = $dir;
  $newName = $cgiparams{'newName'};
  if ($newName eq "")
  {
    $newName = $name;
  }
  if ($shortdir ne "dev")
  {
    if (-e "$permdir/$newName.js" ||
        -e "$stockdir/$newName.js")
    {
      $name = "$newName-a";
    }
    else
    {
      $name = $newName;
    }
  }
  else
  {
    if (-e "$permdir/$name.js" ||
        -e "$stockdir/$name.js")
    {
      $name = "$name-a";
    }
  }

  &addFeedback ("Save Changes 'dev/$name'", 'note');

  if ($name ne "")
  {
    # Save the script
    if (open(HDL, ">$devdir/rc.$name"))
    {
      print HDL $cgiparams{'textscript'};
      close (HDL);
    }
    else
    {
      &addFeedback ("Traffic couldn't save script!", "error");
    }

    # Save the config
    if (open(HDL, ">$devdir/$name.js"))
    {
      print HDL $cgiparams{'textconfig'};
      close (HDL);
    }
    else
    {
      &addFeedback ("Traffic couldn't save config!", "error");
    }
    $cgiparams{'newName'} = "";
    $cgiparams{'CFGGRP'} = "$name.js";
    &doShorthand();
  }
  else
  {
    # Can't proceed without a filename
    &addFeedback ("Traffic needs a filename to save to!", "error");
  }
}

elsif ($cgiparams{'ACTION'} eq "Delete Config")
{
  # Delete config and script

  local ($name, $newName, $dir);

  $name = $cgiparams{'CFGGRP'};
  $name =~ s/\.js$//;
  $dir = &getDirname ($name);
  $shortdir = $dir;
  $shortdir =~ s=.*/==;

  &addFeedback ("Delete Config '$shortdir/$name'", 'note');

  # Get script filename
  if ($dir eq "")
  {
    &addFeedback ("Traffic couldn't find the config.", "error");
    $cgiparams{'CFGGRP'} = "$settings{'CURRENT_CONFIG'}.js";
    &doShorthand();
  }
  elsif ($dir eq "/var/smoothwall/mods/trafficControl/dev")
  {
    if ($name ne "")
    {
      my $total = 0;
      # Delete it
      if (-e "$dir/rc.$name")
      {
        my $count = 0;
        $count = unlink ("$dir/rc.$name");
        $total += $count;
        if ($count ne 1)
        {
          &addFeedback ("Traffic couldn't delete RC script", "error");
        }
      }
      if (-e "$dir/$name.js")
      {
        my $count = 0;
        $count = unlink ("$dir/$name.js");
        $total += $count;
        if ($count ne 1)
        {
          &addFeedback ("Traffic couldn't delete JS config.", "error");
        }
      }
      if ($total eq 2)
      {
        $cgiparams{'CFGGRP'} = "$settings{'CURRENT_CONFIG'}.js";
        &doShorthand();
      }
    }
    else
    {
      # Can't delete nothing!?!
      &addFeedback ("Traffic needs a filename in order to delete it.", "error");
    }
  }
  else
  {
    # Can't delete!?!
    $shortdir = "Saved" if $shortdir eq "perm";
    $shortdir = "Stock" if $shortdir eq "stock";
    &addFeedback ("Traffic: Can't delete a $shortdir config.", "error");
  }
}

elsif ($cgiparams{'ACTION'} eq "Move to Dev")
{
  # Move the selected perm config to dev.

  local ($name, $newName, $dir, $fromdir, $tmpMsg, $shortdir);

  # Prep the name(s).
  $name = $cgiparams{'CFGGRP'};
  $name =~ s/\.js$//;
  # Get the current directory
  $dir = &getDirname ($name);
  $shortdir = $dir;
  $shortdir =~ s=.*/==;

  $newName = $cgiparams{'newName'};
  if ($newName eq "")
  {
    $newName = $name;
  }
  if (-e "$devdir/$newName.js" && $shortdir ne "dev")
  {
    $newName .= "-a";
    $tmpMsg .= "Appended '-a' to the new name.";
  }

  &addFeedback ("Move to Dev '$shortdir/$name' 'dev/$newName'", 'note');
  if ($tmpMsg ne "")
  {
    &addFeedback ($tmpMsg, 'warning');
  }

  if ($dir eq "/var/smoothwall/mods/trafficControl/perm" ||
      ($dir eq "/var/smoothwall/mods/trafficControl/dev" &&
       $name ne $newName))
  {
    # Get script filename
    if ($name ne "")
    {
      # Rename the script
      if (-e "$dir/rc.$name")
      {
        if (!move ("$dir/rc.$name", "$devdir/rc.$newName"))
        {
          &addFeedback ("Traffic couldn't move the RC script.", "error");
        }
      }
      # Rename the config
      if (-e "$dir/$name.js")
      {
        if (!move ("$dir/$name.js", "$devdir/$newName.js"))
        {
          &addFeedback ("Traffic couldn't move the JS config.", "error");
        }
      }
      $cgiparams{'newName'} = "";
      $cgiparams{'CFGGRP'} = "$newName.js";
      &doShorthand();
    }
    else
    {
      # Can't move nothing!?!
      &addFeedback ("Traffic needs a filename in order to move it.", "error");
    }
  }
  elsif ($dir eq "/var/smoothwall/mods/trafficControl/dev" &&
         $name eq $newName)
  {
    # Same file
    &addFeedback ("Traffic can't rename; files are the same.", "error");
  }
  else
  {
    # Can't move!?!
    &addFeedback ("Traffic can't move config from Stock to Dev.", "error");
  }
}

elsif ($cgiparams{'ACTION'} eq "Copy to Dev")
{
  # Copy the selected perm/stock config to dev.

  local ($name, $newName, $dir, $fromdir, $tmpMsg);

  $name = $cgiparams{'CFGGRP'};
  $name =~ s/\.js$//;
  $newName = $cgiparams{'newName'};
  if ($newName eq "")
  {
    $newName = $name;
  }
  if (-e "$devdir/$newName.js" ||
      -e "$permdir/$newName.js" ||
      -e "$stockdir/$newName.js")
  {
    $newName .= "-a";
    $tmpMsg .= "Appended '-a' to the new name.";
  }
  $dir = &getDirname ($name);
  $fromdir = $dir;
  $fromdir =~ s=.*/==;

  &addFeedback ("Copy to Dev '$fromdir/$name' 'dev/$newName'", 'note');
  if ($tmpMsg ne "")
  {
    &addFeedback ($tmpMsg, 'warning');
  }

  # Can copy from any of the three to dev
  if ($dir eq "/var/smoothwall/mods/trafficControl/dev" ||
      $dir eq "/var/smoothwall/mods/trafficControl/perm" ||
      $dir eq "/var/smoothwall/mods/trafficControl/stock")
  {
    # Get script filename
    if ($name ne "")
    {
      # Rename the script
      if (-e "$dir/rc.$name")
      {
        if (!copy ("$dir/rc.$name", "$devdir/rc.$newName"))
        {
          &addFeedback ("Traffic couldn't copy the RC script.", "error");
        }
      }
      # Rename the config
      if (-e "$dir/$name.js")
      {
        if (!copy ("$dir/$name.js", "$devdir/$newName.js"))
        {
          &addFeedback ("Traffic couldn't copy the JS config.", "error");
        }
      }
      $cgiparams{'newName'} = "";
      $cgiparams{'CFGGRP'} = "$newName.js";
      &doShorthand();
    }
    else
    {
      # Can't move nothing!?!
      &addFeedback ("Traffic needs a filename in order to copy it.", "error");
    }
  }
}

elsif ($cgiparams{'ACTION'} eq "Move to Saved")
{
  # Copy the selected dev config to perm.

  local ($name, $newName, $dir);

  $name = $cgiparams{'CFGGRP'};
  $name =~ s/\.js$//;
  $newName = $cgiparams{'newName'};
  if ($newName eq "")
  {
    $newName = $name;
  }
  $dir = &getDirname ($name);
  $shortdir = $dir;
  $shortdir =~ s=.*/==;

  &addFeedback ("Move to Saved '$shortdir/$name' 'perm/$newName'", 'note');

  if ($dir eq "/var/smoothwall/mods/trafficControl/dev")
  {
    # Get script filename
    if ($name ne "")
    {
      # Move the script
      if (-e "$dir/rc.$name")
      {
        if (!move ("$dir/rc.$name", "$permdir/rc.$newName"))
        {
          &addFeedback ("Traffic couldn't move 'dev/rc.$name' to perm/rc.$newName.", "error");
        }
      }
      # Move the config
      if (-e "$dir/$name.js")
      {
        if (!move ("$dir/$name.js", "$permdir/$newName.js"))
        {
          &addFeedback ("Traffic couldn't move 'dev/$name.js' to perm/$newName.js", "error");
        }
      }
      $cgiparams{'newName'} = "";
      $cgiparams{'CFGGRP'} = "$newName.js";
      &doShorthand();
    }
    else
    {
      # Can't move nothing!?!
      &addFeedback ("Traffic needs a filename in order to move it!", "error");
    }
  }
  else
  {
    # Can't move!?!
    if ($shortdir eq 'perm') {
      &addFeedback ("Traffic can't rename config in Saved.", "error");
    }
    if ($shortdir eq 'stock') {
      &addFeedback ("Traffic can't move config from Stock to Saved.", "error");
    }
  }
}

elsif ($cgiparams{'ACTION'} eq 'Stop')
{
  &addFeedback("Stop", "note");
  # The man say stop, we stop.
  $settings{'STATE'} = "off";
  &writehash($settingsfile, \%settings);
  $success = &message('trafficrestart');
  if (not defined $success)
  {
    &addFeedback ("$tr{'smoothd failure'}", "error");
  }
}

elsif ($cgiparams{'ACTION'} eq 'Start')
{
  &addFeedback("Start", "note");
  # The man say run, we run.
  # But only if the config hasn't strayed.
  if (&getDirname($plain_name) eq "")
  {
    &addFeedback ("trafficControl: couldn't find '$config_name' to activate!", "warning");
  }
  else
  {
    $settings{'STATE'} = "on";
    &writehash($settingsfile, \%settings);
    $success = &message('trafficrestart');
    if (not defined $success)
    {
      &addFeedback ("$tr{'smoothd failure'}", "error");
    }
  }
}

elsif ($cgiparams{'ACTION'} eq 'Use Selected')
{
  &addFeedback("Use Selected '$cgiparams{'CFGGRP'}'", "note");
  # He say use different script. Chop, chop!
  # But only if the config hasn't strayed.
  my $cfg = $cgiparams{'CFGGRP'};
  $cfg =~ s/\.js$//;
  if (&getDirname($cfg) eq "")
  {
    &addFeedback ("trafficControl: couldn't find '$cfg' to change to!", "warning");
  }
  else
  {
    $settings{'CURRENT_CONFIG'} = $cfg;
    $settings{'CURRENT_CONFIG'} =~ s/\.js$//;
    &writehash($settingsfile, \%settings);
    $success = &message('trafficrestart');
    if (not defined $success)
    {
      &addFeedback ("$tr{'smoothd failure'}", "error");
    }
    $cgiparams{'textconfig'} = "";
  }
}

elsif ($cgiparams{'ACTION'} eq 'Cancel/Edit')
{
  &addFeedback ("Cancel/Edit", "note");
  # Toss all changes in the dust bin, reload from disk
  # This is otherwise a no-op.
}

########
# Also need actions for upload and download
########

# Done last before generating HTML
# Fetch the config
  my $cfg = "$dir_cfggrp/$cfggrp_name.js";
  if (open (TMPHDL, "<$cfg"))
  {
    $cfggrp_script = "";
    while (<TMPHDL>)
    {
      $cfggrp_script .= $_;
    }
    close (TMPHDL);
  } else {
    &addFeedback ("trafficControl: config '$cfg' not found!", "error");
    $cfggrp_script = "";
  }

# Now display the page regardless of the action taken

&openpage ($tr{'traffic configuration'}, 1, '', 'networking');
&openbigbox('100%', 'left');

print<<END;
<script type="text/javascript">
// wz_tooltip.js: Copyright (c) 2002-2008 Walter Zorn. All rights reserved. Licensed
//   under GNU LGPL. See wz.tooltip.js for details.
// -
// All other code in this development software: Copyright 2009, Neal P. Murphy.
//   All rights reserved.

// qNics, qRoots, qRules arrays/object are global, read via a .js file
// For testing, define them here

// Define globals
//
var bgBool = 0;
var spaces = '                                                                ';
var nictree = '';
var commands = '';
// Need a global for 'iptables -D' commands
var ipt_stop_txt = '';

String.prototype.toHex = function() {
  var cDash = this.indexOf(":");
  if (cDash != -1 ) {
    var major = Number(this.substring(0, cDash));
    var minor = Number(this.substring(cDash+1));
    return major.toString(16)+ ":" +minor.toString(16);
  } else {
    var major = Number(this.valueOf());
    return major.toString(16);
  }
}

var new_counter = 0;
var qNics = [
  "GREEN",
  "PURPLE",
  "ORANGE",
  "RED"
];
var qRoots = {
  "GREEN": {
    "gui_color": "green",
    "view": "nocfg",
    "state": "nocfg",
    "qDisc": "",
    "pending_action": "",
    "nextMajor": "1",
    "NATted": ""
  },
  "PURPLE": {
    "gui_color": "purple",
    "view": "nocfg",
    "state": "nocfg",
    "qDisc": "",
    "pending_action": "",
    "nextMajor": "1",
    "NATted": ""
  },
  "ORANGE": {
    "gui_color": "orange",
    "view": "nocfg",
    "state": "nocfg",
    "qDisc": "",
    "pending_action": "",
    "nextMajor": "1",
    "NATted": ""
  },
  "RED": {
    "gui_color": "red",
    "view": "nocfg",
    "state": "nocfg",
    "qDisc": "",
    "pending_action": "",
    "nextMajor": "1",
    "NATted": "1"
  }
};

var qRules = new Array();

var qColors = {
  "green": "007f00",
  "purple": "6f00bf",
  "orange": "e57a00",
  "red": "df0000",
};
</script>
END

# start the form
print qq|
  <form method="post">
    <input type="hidden" id="configJS" name="textconfig" value="">
    <input type="hidden" id="rcScript" name="textscript" value="">
|;

# Show the feedback box, if needed
if ($errormessage ne "")
{
  &openbox('Feedback');
  print qq|
  <p style="margin:2pt 4pt 4pt 4pt; color:#800000">
    $errormessage
  </p>\n
|;
  &closebox();
}

# make the Control block
&openbox ('Control');

my $action;

# Determine which button works and which doesn't
if ($settings{'STATE'} eq 'on')
{
  @usable = (" disabled='disabled'", "");
  $start = "Running";
  $stop = "Stop";
}
else
{
  @usable = ("", " disabled='disabled'");
  $start = "Start";
  $stop = "Stopped";
}

# Show 'em
print qq|
    <div style="margin:0; padding:0;  vertical-align:middle">
      <p style="width:9em; margin:1em 0 1em 0; text-align:right; display:inline-block">
        <span class='base'>Current config:</span>
      </p>
      <p style="margin:1em 0 1em 1em; display:inline-block; text-align:left">
        <b>$settings{'CURRENT_CONFIG'}</b>
      </p>
    </div>
    <div style="margin:0; padding:0;  vertical-align:bottom">
      <input type='submit' name="ACTION" value="$start"
             style="margin-left: 8em; background-color:#d2e8d2; display:inline-block"$usable[0]
             onclick="uiToggleStart(); return true;">
      <input type='submit' name="ACTION" value="$stop"
             style="margin-left: 4em; background-color:#e6d2d2; display:inline-block"$usable[1]
             onclick="uiToggleStart(); return true;">
      <input type='submit' name="ACTION" value="Use Selected"
             style="margin-left: 4em; display:inline-block">
    </div>
|;

&closebox();

&openbox('Configurations');

print qq|
  <div style='width:30%; vertical-align:top; display:inline-block; margin:.3em; padding:.3em'>
    <p class='box' style='margin:0'>Dev:</p>
|;

&files2RadioButtons ($devdir, "CFGGRP", "moveToDev");

print qq|
  </div>
|;
print qq|
  <div style='width:30%; vertical-align:top; display:inline-block; margin:.3em; padding:.3em'>
    <p class='box' style='margin:0'>Saved:</p>
|;

&files2RadioButtons ($permdir, "CFGGRP", "copyToSav");

print qq|
  </div>
|;
print qq|
  <div style='width:30%; vertical-align:top; display:inline-block; margin:.3em; padding:.3em'>
    <p class='box' style='margin:0'>Stock:</p>
|;

&files2RadioButtons ($stockdir, "CFGGRP", "copyToDev");

print qq|
  </div>
|;

print qq|
    <div style="margin:1em 1em 0 1em; text-align:center">
      <div style="margin:1em 3em 0 1em; text-align:center; display:inline-block">
        <input type='submit' name="ACTION" value="Move to Dev"
               id="moveToDev"
               style="margin:0 .5em; display:inline-block; font-size:8pt">
        <input type='submit' name="ACTION" value="Move to Saved"
               id="CopyToSav"
               style="margin:0 .5em; display:inline-block; font-size:8pt">
        <input type='submit' name="ACTION" value="Copy to Dev"
               id="copyToDev"
               style="margin:0 .5em; display:inline-block; font-size:8pt">
        <br />
        <br />
        New Name: <input type="text" name="newName" value="$cgiparams{'newName'}" size=30>
      </div>
      <div style="margin:1em 1em 0 3em; text-align:center; display:inline-block">
        <input type='submit' name="ACTION" value="Save Changes"
               id="saveChanges"
               onclick="getScriptAndConfig();"
               style="margin:0 .5em; display:inline-block; font-size:8pt">
        <input type='submit' name="ACTION" value="Cancel/Edit"
               id="Edit"
               style="margin:0 .5em; display:inline-block; font-size:8pt">
        <br />
        <br />
        <input type='submit' name="ACTION" value="Delete Config"
               id="deleteConfig"
               style="margin:0 .5em; display:inline-block; font-size:8pt">
      </div>
    </div>
|;

# Here, if the Edit button was clicked, display all the JS stuff to edit the config
# Put it all in a div.

print<<END;

<div style="margin:1em">
<p class="endnotes" style="text-align:center;display:none">
  <span id="savebutton_div" style="margin:14pt; padding:4pt">
    <button type="button" id="save_button" style="margin:4pt"
            onclick='return changePanel("save_div", "savebutton_div");'>Load/Save</button>
  </span>
  <span id="worksheetbutton_div" style="margin:14pt; padding:4pt">
    <button type="button" id="worksheet_button" style="margin:4pt"
            onclick='return changePanel("tree_div", "worksheetbutton_div");'>Worksheet</button>
  </span>
  <span id="tcgenbutton_div" style="margin:14pt; padding:4pt">
    <button type="button" id="tcgen_button" style="margin:4pt"
            onclick='return changePanel("tcgen_div", "tcgenbutton_div");'>Code Generator</button>
  </span>
  <span id="configbutton_div" style="margin:14pt; padding:4pt">
    <button type="button" id="config_button" style="margin:4pt" disabled="disabled"
            onclick='return changePanel("config_div", "configbutton_div");'>Interface Config</button>
  </span>
</p>
</div>

<hr style="margin:18pt">
<div id="tree_div" style="text-align:left; display:none"></div>

<div id="config_div" style="text-align:center; display:none"></div>

<div id="save_div" style="text-align:left; display:block"></div>

<div id="tcgen_div" style="text-align:left; display:none"></div>

<script type="text/javascript" src="/ui/js/wz_tooltip.js"></script>
<script type="text/javascript" src="/ui/js/qos-actions.js"></script>
<script type="text/javascript" src="/ui/js/qos-commands.js"></script>
<script type="text/javascript" src="/ui/js/qos-display.js"></script>
<script type="text/javascript" src="/ui/js/qos-menu.js"></script>
<script type="text/javascript" src="/ui/js/qos-oenum.js"></script>
<script type="text/javascript" src="/ui/js/qos-tree.js"></script>
END

print<<END;
<script type="text/javascript">
// Main code; this is actually run by the browser when it gets here.
END

  print $cfggrp_script;

# Include the config file name
  print ("\nvar cfg_file = \"$cgiparams{'CFGGRP'}\";\n");

print<<END;
// Expose yourself!
changePanel('tree_div', 'worksheetbutton_div');

</script>
END

&closebox();

&openbox('Upload/Download');
&closebox();

# Close the form
print qq|
  </form>
|;

&closebigbox();
&closepage();
