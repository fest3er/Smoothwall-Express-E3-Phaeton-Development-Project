#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );
use smoothtype qw( :standard );

use Cwd;

my (%snortsettings, %checked);
my $maxwidth = 20;  # em (EM - width of an M in the current font)

&showhttpheaders();

$snortsettings{'ENABLE_SNORT'} = 'off';
$snortsettings{'DOWNLOAD'} = 'off';
$snortsettings{'ACTION'} = '';
&getcgihash(\%snortsettings);
$formdownload = $snortsettings{'DOWNLOAD'};
$formdebug = $snortsettings{'DEBUG'};
delete $snortsettings{'DOWNLOAD'};
delete $snortsettings{'DEBUG'};
&readhash("${swroot}/snort/settings", \%snortoldsettings);

$errormessage = '';


############################
# Early actions
#
# Verify OINK code length and content
if ($snortsettings{'ACTION'} eq $tr{'save'} and \
    $formdownload eq 'on' and \
    !$errormessage)
{
  if ($snortsettings{'OINK'} !~ /^([\da-f]){40}$/i)
  {
    $errormessage = $tr{'oink code must be 40 hex digits'};
  }
  else
  {
  &writehash("${swroot}/snort/settings", \%snortsettings);
  }
}

# Turn on or off, but only if the option changed and not downloading.
if ($snortsettings{'ACTION'} eq $tr{'save'} and
    $formdownload ne "on" and
    $snortoldsettings{'ENABLE_SNORT'} ne $snortsettings{'ENABLE_SNORT'}) {

  &writehash("${swroot}/snort/settings", \%snortsettings);

  if ($snortsettings{'ENABLE_SNORT'} eq 'on')
  {
    &log($tr{'snort is enabled'});
  }
  else
  {
    &log($tr{'snort is disabled'});
  }

  my $success = message('snortrestart');

  if (not defined $success)
  {
    $errormessage = $tr{'smoothd failure'};
  }
}


############################
# Preparations
&readhash("${swroot}/snort/settings", \%snortsettings);

# Extract on or off
$checked{'ENABLE_SNORT'}{'off'} = '';
$checked{'ENABLE_SNORT'}{'on'} = '';
$checked{'ENABLE_SNORT'}{$snortsettings{'ENABLE_SNORT'}} = 'CHECKED';

# Extract days since last DL
my $mylastdownload = 'N/A';
if (-e "${swroot}/snort/DL-age")
{
  $idslastDLdays = int(-M "${swroot}/snort/DL-age");
  if ($idslastDLdays == 1)
  {
    $idslastdownload = "$idslastDLdays $tr{'ids day ago'}";
  }
  else
  {
    $idslastdownload = "$idslastDLdays $tr{'ids days ago'}";
  }
}
else
{
  $idslastdownload = "($tr{'ids never downloaded'})";
}

# Extract rule age from rule files
my $ruleage = 'N/A';
if (-d "${swroot}/snort/rules")
{
  my $opwd = `pwd`;
  chomp $opwd;
  chdir "${swroot}/snort/rules";
  my $ruledate = `egrep "\$Id:.*,v.*vrtbuild" * | \
                    sed -e 's/.*,v [0-9.]* //' -e's/ .*//' | \
                    sort | tail -1`;
  chomp $ruledate;
  $ruledate = `date -d '$ruledate' "+%s"`;
  $today = `date "+%s"`;
  $diff = ($today-$ruledate)/3600/24;
  $days = int($diff);
  $ruleage = "$days $tr{'ids days'}";
  chdir $opwd;
}
else
{
  $days = -1;
  $ruleage = "($tr{'ids never downloaded'})";
}

# Decide if download should be prechecked
$checked{'DOWNLOAD'}{'off'} = '';
$checked{'DOWNLOAD'}{'on'} = '';
if (($days==-1 || $days > 30) and ($idslastDLdays > 5)) {
  $checked{'DOWNLOAD'}{'on'} = 'checked';
}

# status at page rendering
if ($snortsettings{'ENABLE_SNORT'} eq 'on')
{
  $ids_status = $tr{'ids enabled'};
}
else
{
  $ids_status = $tr{'ids disabled'};
}


############################
# Start page rendering
&openpage($tr{'intrusion detection system'}, 1, '', 'services');

&openbigbox('100%', 'left');

&alertbox($errormessage);

print "<form method='post'>\n";

&openbox($tr{'intrusion detection system2'}.' <i>('.$ids_status.')</i>');

print "
<div style='margin:12pt 0 0 0'>
  <div class='base' style='text-align:right; margin-right:.2em; display:inline-block; width:20%; '>
    $tr{'ids enable'}
  </div>
  <div style='margin-right:.2em; display:inline-block; width:20%; '>
    <input type='checkbox' name='ENABLE_SNORT' style='margin:0' $checked{'ENABLE_SNORT'}{'on'}>
  </div>
  <div class='base' style='text-align:right; margin-right:.2em; display:inline-block; width:20%; '>
    $tr{'rule age'}
  </div>
  <div style='margin-right:.2em; display:inline-block; width:20%; '>
    <b>$ruleage</b>
  </div>
  <br />
  <div class='base' style='text-align:right; margin-right:.2em; display:inline-block; width:20%; '>
    $tr{'ids download label'}
  </div>
  <div style='margin-right:.2em; display:inline-block; width:20%; '>
    <input type='checkbox' name='DOWNLOAD' style='margin:0' $checked{'DOWNLOAD'}{'on'}>
    <span style='margin-left:18pt; margin-right:.5em'>$tr{'ids debug'}</span>
    <span>
      <input type='checkbox' name='DEBUG' style='margin:0'>
    </span>
  </div>
  <div class='base' style='text-align:right; margin-right:.2em; display:inline-block; width:20%; '>
    $tr{'last download'}
  </div>
  <div style='margin-right:.2em; display:inline-block; width:20%; '>
    <b>$idslastdownload</b>
  </div>
</div>
";

&closebox();

print "
<div style='width:100%; text-align:center; vertical-align:middle'>
  <input type='submit' name='ACTION' value='$tr{'save'}'>
</div>
";


&openbox($tr{'rule retreval'});

print "
<table width='100%'>
  <tr>
    <td class='base' width='20%'>
      $tr{'oink code'}
    </td>
    <td width='75%'>
      <input type='text' name='OINK' size='42' maxlength='40'
             value='$snortsettings{OINK}'
             id='OINK' @{[jsvalidregex('OINK','^([0-9a-fA-F]){40}$')]}>
    </td>
  </tr>
  <tr>
    <td class='base' width='20%'>
      $tr{'ids download progress'}
    </td>
    <td>
      <div style='width:${maxwidth}em; height:100%; border:lightgrey 1px solid'>
        <div id='progress' class='progressbar'
             style='width:0; height:100%; background-color:#000090'>&nbsp;</div>
      </div>
    </td>
    <td class='progressend'>&nbsp;</td>
  </tr>
  <tr>
    <td class='base' width='20%'>
      $tr{'ids download status'}
    </td>
    <td>
      <span id='status'></span>
    </td>
  </tr>
</table>
";

&closebox();

&alertbox('add', 'add');

print "</form>\n";

&closebigbox();

# close except </body> and </html>
&closepage( "update" );
  
if (($snortsettings{'ACTION'} eq $tr{'save'}) &&
    ($formdownload eq 'on') &&
    !$errormessage)
{
  my $origsnortversion = &readvalue('/usr/lib/smoothwall/snortversion');

  my $snortversion = $origsnortversion;
  
  $snortversion =~ s/\.//g;
  
  while (length $snortversion < 4)
  {
    $snortversion = $snortversion.'0';
  }
  
  &runoinkmaster($snortversion);
  
  if ($errormessage)
  {
    $snortversion = $origsnortversion;
    
    $snortversion =~ /^(\d+\.\d+)/;
    $snortversion = $1;
    
    &runoinkmaster($snortversion);
  }
  
  if ($snortsettings{'ENABLE_SNORT'} eq 'on' and !$errormessage)
  {  
    my $success = message('snortrestart');

    if (not defined $success)
    {
      $errormessage = $tr{'smoothd failure'};
    }
  }

  if ($errormessage)
  {
    print "
<script>
  document.getElementById('status').innerHTML = '$errormessage';
</script>
";
  }
  else
  {
    print "
<script>
  document.getElementById('status').innerHTML = 'Installation complete';
  document.getElementById('progress').style.width = '${maxwidth}em';
  //document.location = '/cgi-bin/ids.cgi';
</script>
";
  }

}
print "
</body>
</html>
";

sub runoinkmaster
{
  my $v = $_[0];
  if (defined $_[1]) {my $attempt = $_[1];}
  my $url = "http://www.snort.org/reg-rules/snortrules-snapshot-$v.tar.gz/" . $snortsettings{'OINK'};
#  my $url = "http://downloads/snortrules-snapshot-$v.tar.gz";

  my $curdir = getcwd;
  chdir "${swroot}/snort/";
  
  select STDOUT;
  $| = 1;

  my $pid = open(FD, '-|');
  if (!defined $pid)
  {
    $errormessage = $tr{'unable to fetch rules'};
  }
  elsif ($pid)
  {
    $errormessage = $tr{'rules not available'};


    print "
<script>
  document.getElementById('status').innerHTML = 'Downloading, please wait';
  document.getElementById('progress').style.background = '#a0a0ff';
</script>
";
    while(<FD>)
    {
      if ($formdebug ne "")
      {
        print $_."<br>\n";
      }
      print STDERR $_;
      $errormessage = '';
      if (/(\d{1,3})%/)
      {
        my $percent = $1;
        my $message;
        if ($percent == 100)
        {
          print "
<script>
  document.getElementById('status').innerHTML = 'Installing, please wait';
  document.getElementById('progress').style.width = '${maxwidth}em';
</script>
";
        }
        else
        {
#          $message = "Download $percent% complete";
          my $curwidth = $maxwidth * $percent/100;
          print "
<script>
document.getElementById('progress').style.width = '${curwidth}em';
</script>
";
        }
      }
    }
    close(FD);

    if ($?)
    {
      $errormessage = $tr{'unable to fetch rules'};
      if (not defined $attempt)
      {
        ($v1, $v2, $v3, $v4) = split(//, $v);
        if ($v4 > 0)
        {
          # Try again, but once only.
          $v4--;
          &runoinkmaster (join('', $v1, $v2, $v3, $v4), 1);
        }
      }
    }
    else
    {
      unlink (">${swroot}/snort/DL-age");
      open (FILE, ">${swroot}/snort/DL-age");
      close (FILE);
      #open (FILE, ">${swroot}/snort/ruleage");
      #close (FILE);
    }
  }
  else
  {
    # so we see wget's output
    close(STDERR);
    open(STDERR, ">&STDOUT");

#    exec("wget", "-O", "/tmp/a.tgz", "$url");
    exec('/usr/bin/oinkmaster.pl', '-v', '-C',
    '/usr/lib/smoothwall/oinkmaster.conf', '-o', 'rules', '-u', $url);
  }

  chdir $curdir;
}
