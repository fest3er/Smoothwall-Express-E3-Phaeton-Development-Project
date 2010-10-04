#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %cgiparams, %settingsParams, %uiSettingsParams;

$cgiparams{'ACTION'} = '';
$cgiparams{'MENU'} = 'off';
$cgiparams{'LANGUAGE'} = 'en';
$cgiparams{'HOSTNAME'} = '';
$cgiparams{'KEYMAP'} = '';
$cgiparams{'OPENNESS'} = '';

&getcgihash(\%cgiparams);

if ($cgiparams{'ACTION'} eq $tr{'save'})
{
	$settingsParams{'LANGUAGE'} = $cgiparams{'LANGUAGE'};
	$settingsParams{'HOSTNAME'} = $cgiparams{'HOSTNAME'};
	$settingsParams{'KEYMAP'} = $cgiparams{'KEYMAP'};
	$settingsParams{'OPENNESS'} = $cgiparams{'OPENNESS'};

	$uiSettingsParams{'MENU'} = $cgiparams{'MENU'};
	
	&writehash("${swroot}/main/settings", \%settingsParams);
	&writehash("${swroot}/main/uisettings", \%uiSettingsParams);
        &readhash("${swroot}/main/settings", \%settings);
        &readhash("${swroot}/main/uisettings", \%uisettings);
        $language = $settings{'LANGUAGE'};
	if ($ENV{'HTTPS'} eq "on")
	{
	  $method = "https:";
	} else {
	  $method = "http:";
	}
	print "Location: ${method}//$ENV{'HTTP_HOST'}$ENV{'REQUEST_URI'}\n\n";
}

&showhttpheaders();

if ($cgiparams{'ACTION'} eq '')
{
	$cgiparams{'MENU'} = 'on';
}

&readhash("${swroot}/main/uisettings", \%cgiparams);
&readhash("${swroot}/main/settings", \%cgiparams);

my %checked, %selected;

$checked{'MENU'}{$cgiparams{'MENU'}} = " checked";
$selected{'LANGUAGE'}{$cgiparams{'LANGUAGE'}} = " checked";

&openpage( $tr{'preferences'}, 1, '', 'maintenance');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<form method='post'>\n";
print "  <input type='hidden' name='HOSTNAME' value='$cgiparams{'HOSTNAME'}'\n";
print "  <input type='hidden' name='KEYMAP' value='$cgiparams{'KEYMAP'}'\n";
print "  <input type='hidden' name='OPENNESS' value='$cgiparams{'OPENNESS'}'\n";

&openbox($tr{'user interface'});

print "
<table style='width: 100%;'>
  <tr>
    <td class='base' style='width: 25%;'>$tr{'drop down menus'}</td>
    <td style='width: 25%;'><input type='checkbox' name='MENU' $checked{'MENU'}{'on'}></td>
    <td style='width: 25%;'>&nbsp;</td>
    <td style='width: 25%;'>&nbsp;</td>
  </tr>
</table>
";

&closebox();

# Get all langs present

&openbox('Language:');

$langpipe = "cd /usr/lib/smoothwall/langs;";
$langpipe .= "/bin/ls -C1 | /bin/grep -v base | ";
$langpipe .= "/usr/bin/sed -e 's/alertboxes\.//' -e 's/glossary\.//' -e 's/\.pl//' | ";
$langpipe .= "/bin/sort | /usr/bin/uniq |";
if (open (LANGS, $langpipe))
{
  my @langs = <LANGS>;
  chomp @langs;
  close LANGS;

  print "
<table style='width:100%; margin: 8pt 0'>
  <tr>
    <td>
";

  my $rowlim = int (@langs / 5);
  my $rowcnt = 0;
  foreach $lang (@langs)
  {
    print "      <p style='margin:4pt 0 0 18pt'>\n";
    print "        <input type='radio' name='LANGUAGE' value='$lang'";
    print "$selected{'LANGUAGE'}{$lang}>$lang\n";
    print "      </p>\n";
    $rowcnt++;
    if ($rowcnt > $rowlim)
    {
      $rowcnt = 0;
      print "    </td>\n    <td>\n";
    }
  }
}


print "
    </td>
  </tr>
</table>
";

&closebox();

print <<END
<table style='width: 100%;'>
<tr>
	<td style='width: 100%; text-align: center;'>
	<input type='submit' name='ACTION' value='$tr{'save'}'>
	</td>
</tr>
</table>
END
;

print "</form>\n";

&closebigbox();

&closepage();
