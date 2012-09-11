#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use IO::Socket;

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use update qw( :standard );
use smoothnet qw( :standard );
use smoothd qw(message);

&showhttpheaders();

my (%uploadsettings,$errormessage);
&getcgihash(\%uploadsettings);

my $extrahead = qq {
<script>
function toggle( what )
{
  var tog = document.getElementById( what );
  if ( tog.style.display && tog.style.display != 'inline' ){
    tog.style.display = 'inline';
  } else {
    tog.style.display = 'none';
  }
}
</script>
};

&openpage($tr{'updates'}, 1, $extrahead, 'maintenance');

&openbigbox('100%', 'LEFT');

if ($uploadsettings{'ACTION'} eq $tr{'upload'})
{
  my @list;
  my $return = &downloadlist();
  if ($return =~ m/^HTTP\/\d+\.\d+ 200/)
  {
    unless (open(LIST, ">${swroot}/patches/available"))
    {
                  $errormessage = $tr{'could not open available updates file'};
      	 	        goto ERROR;
        	}
    flock LIST, 2;
          my @this = split(/----START LIST----\n/,$return);
    print LIST $this[1];
    close(LIST);
    @list = split(/\n/,$this[1]);
  } 
  else
  {
    unless(open(LIST, "${swroot}/patches/available"))
    {
      $errormessage = $tr{'could not open available updates list'};
      goto ERROR;
    }
    @list = <LIST>;
    close(LIST);
    $errormessage = $tr{'could not download the available updates list'};
  }
  unless (mkdir("/var/patches/$$",0700))
  {
    $errormessage = $tr{'could not create directory'};
    goto ERROR;
  }
  unless (open(FH, ">/var/patches/$$/patch.tar.gz"))
  {
    $errormessage = $tr{'could not open update for writing'};
    goto ERROR;
  }
  flock FH, 2;
  print FH $uploadsettings{'FH'};
  close(FH);
  my $md5sum;
  chomp($md5sum = `/usr/bin/md5sum /var/patches/$$/patch.tar.gz`);
  my $found = 0;
  my ($id,$md5,$title,$description,$date,$url);
  foreach (@list)
  {
    chomp();
    ($id,$md5,$title,$description,$date,$url) = split(/\|/,$_);
    if ($md5sum =~ m/^$md5\s/)
    {
      $found = 1;
      last;
    }
  }
  unless ($found == 1)
  {
    $errormessage = $tr{'this is not an authorised update'};
    goto ERROR;
  }
  unless (system("cd /var/patches/$$ && /usr/bin/tar xvfz patch.tar.gz > /dev/null") == 0)
  {
    $errormessage = $tr{'this is not a valid archive'};
    goto ERROR;
  }
  unless (open(INFO, "/var/patches/$$/information"))
  {
    $errormessage = $tr{'could not open update information file'};
    goto ERROR;
  }
  my $info = <INFO>;
  close(INFO);
  open(INS, "${swroot}/patches/installed") or $errormessage = $tr{'could not open installed updates file'};
  while (<INS>)
  {
    my @temp = split(/\|/,$_);
    if($info =~ m/^$temp[0]/)
    {
      $errormessage = $tr{'this update is already installed'};
      goto ERROR;
    }
  }
  chdir("/var/patches/$$");
  print STDERR "Going for installation attempt\n";
  if (system( '/usr/bin/setuids/installpackage', $$))
  {
    $errormessage = $tr{'package failed to install'};
    goto ERROR;
  }
  unless (open(IS, ">>${swroot}/patches/installed")) {
 		$errormessage = $tr{'update installed but'}; }
  flock IS, 2;
  my @time = gmtime();
  chomp($info);
  $time[4] = $time[4] + 1;
  $time[5] = $time[5] + 1900;
  if ($time[3] < 10) { $time[3] = "0$time[3]"; }
  if ($time[4] < 10) { $time[4] = "0$time[4]"; }
  print IS "$info|$time[5]-$time[4]-$time[3]\n";
  close(IS);
  &log("$tr{'the following update was successfully installedc'} $title"); 
}
elsif ($uploadsettings{'ACTION'} eq $tr{'refresh update list'})
{
  my $return = &downloadlist();
  if ($return =~ m/^HTTP\/\d+\.\d+ 200/)
  {
    unless(open(LIST, ">${swroot}/patches/available"))
    {
      $errormessage = $tr{'could not open available updates file'};
      goto ERROR;
    }
    flock LIST, 2;
    my @this = split(/----START LIST----\n/,$return);
    print LIST $this[1];
    close(LIST);
    
    &log($tr{'successfully refreshed updates list'});
  } 
  else {
    $errormessage = $tr{'could not download the available updates list'}; }
}

ERROR:

my %updates;

open(AV, "${swroot}/patches/available") or $errormessage = $tr{'could not open the available updates file'};
while (<AV>){
  next if $_ =~ m/^#/;
  chomp $_;
  my @temp = split(/\|/,$_);
  my ($summary) = ( $temp[3] =~ /^(.{0,85})/ );
  $updates{ $temp[ 0 ] } = { name => $temp[2], summary => $summary, description => $temp[3], date => $temp[4], info => $temp[5], size => $temp[6], title => $temp[7], md5 => $temp[1] };
}
close(AV);

open (PF, "${swroot}/patches/installed") or $errormessage = $tr{'could not open installed updates file'};
while (<PF>)
{
  my @temp = split(/\|/,$_);
  $updates{$temp[0]}{'installed'} = "---";
}
close PF;

&alertbox($errormessage);

# Display options for adding / installing etc updates

&openbox();

# Get # of updates
my $available_count = 0;
foreach my $update ( sort keys %updates ){
  next if ( defined $updates{$update}{'installed'} );
  $available_count ++;
}

# Get # of installed updates
my $installed_count = 0;
foreach my $update ( sort keys %updates ){
  next if ( not defined $updates{$update}{'installed'} );
  $installed_count ++;
}

my $height = 250;

print "
  <div style='height: ${height}px; overflow: auto;'>
          <p style='margin: 2pt 2pt 4pt 10pt; padding:4pt; text-indent: -10pt'>
";

if ( $available_count > 0 ){
  print "
      <b>$tr{'available updates'}</b><br />
      <span style='color: #808080;'>
              These updates have not been installed:
            </span>
          </p>
  <table class='blank' style='margin:2pt 2pt 2pt 14pt; width:95%'>
";

  foreach my $update ( sort keys %updates )
  {
    next if ( defined $updates{$update}{'installed'} );
    my $maj = substr($updates{$update}{"name"}, 0, 1);
    my $min = substr($updates{$update}{"name"}, 1, 2);
    my $pnt = substr($updates{$update}{"name"}, 3, 3);
    my $fx = substr($updates{$update}{"name"}, 6, 3);
    my $displayName = sprintf ("%d.%d.%d.%d", $maj, $min, $pnt, $fx);
    print <<END
  <tr style='vertical-align:top'>
    <td style='width: 10%;' ><a href='$updates{$update}{"info"}' target='_new'>$displayName</a></td>
    <td onClick="toggle('update-$update');" class='expand' style='vertical-align:top'>
      <div>
        $updates{$update}{'title'}
      </div>
      <div id='update-$update' class='expand' style='display:none; margin:2pt 2em 2pt 2em'>
        $updates{$update}{'description'}
      </div>
    </td>
    <td style='width: 10%; text-align: right;' style='vertical-align:top' >$updates{$update}{'date'}</td>
  </tr>
END
  ;
  }

  print qq{
  </table>
  };
}

if ( $installed_count > 0 ){
    print qq|
          <p style='margin: 12pt 2pt 2pt 14pt; text-indent: -10pt'>
      <b>$tr{'installed updates'}</b><br />
      <span style='color: #808080;'>
    The following updates have already been applied to your Roadster.
            </span>
          </p>
  <table class='blank' style='margin:2pt 2pt 2pt 14pt; width:95%; vertical-align:top'>
|;

foreach my $update ( sort keys %updates ){
  next if ( not defined $updates{$update}{'installed'} );
  print <<END
  <tr style='vertical-align:top'>
    <td style='width: 10%;' ><a style='color: #808080;' href='$updates{$update}{"info"}' target='_new'>$updates{$update}{"name"}</a></td>
    <td onClick="toggle('update-$update');" class='expand' style='color:#8080ff; vertical-align:top'>
      <div>
        $updates{$update}{'title'}
      </div>
      <div id='update-$update' class='expand' style='display:none; color:#805080; margin:2pt 2em 2pt 2em'>
        $updates{$update}{'description'}
      </div>
    </td>
    <td style='width: 10%; text-align: right;' style='color: #805080; vertical-align:top' >$updates{$update}{'date'}</td>
  </tr>
END
;

}

  print qq|</table>|;
}

if ( $available_count+$installed_count == 0 ){
    print <<END
          <p style='margin: 12pt 2pt 4pt 10pt; text-indent: -10pt'>
            <b>No applicable updates found</b><br />
          </p>
END
}

print "</div>";

&closebox();
&openbox();

print <<END
<table class='blank'>
<tr>

  <td id='progressbar'>
<table class='progressbar' style='width: 380px;'>
  <tr>
    <td id='progress' class='progressbar' style='width: 1px;'>&nbsp;</td>
    <td class='progressend'>&nbsp;</td>
  </tr>
</table>
  <span id='status'></span>
  </td>
  <td>&nbsp;</td>
  <form action='/cgi-bin/updates.cgi' method='post'>
  <td style='width: 350px;' style='text-align: right;'>
    <input type='submit' name='ACTION' value='$tr{'refresh update list'}'>
    <input type='submit' name='ACTION' value='$tr{'update'}'>
  </td>
  </form>
  <td style='text-align: right;' id='actionsection'>
  </td>

</tr>
</table>
END
;
&closebox();

print qq{
  <div id='manualinstall'>
};

&openbox( $tr{'install new update'} );

print qq|
$tr{'to install an update'}
<table class='blank'>
  <tr>
  <form method='post' action='/cgi-bin/updates.cgi' enctype='multipart/form-data'>
  <td>
    $tr{'upload update file'}
  </td>
  <td>
    <input type="file" name="FH"> <input type='submit' name='ACTION' value='$tr{'upload'}'>
  </td>
  </form>
  </tr>
</table>|;

&closebox();
print qq{
  </div>
  <script>
    var add = "<input type='button' value='Advanced >>' onClick=\\"toggle('manualinstall');\\">";
    document.getElementById('actionsection').innerHTML += add;
    toggle('manualinstall');
  </script>
};

&closebigbox();


# update downloads etc need to be dealt with at the end of the page (otherwise
# we would find ourselves with a blank page that doesn't seem to be doing a 
# great deal.
# Since the updates are "running" in the background all we "need" to do is 
# periodically test for updates


# firstly, simulate the action of the "closepage()" function, but ommit
# the </html> tags

&closepage( "update" );

if ($uploadsettings{'ACTION'} eq "$tr{'update'}" )
{
  use lib "/usr/lib/smoothwall/";

  print STDERR "Performing Update\n";

  # determine the list of updates we currently require.

  my %required;
  
  foreach my $update ( sort keys %updates )
  {
    next if ( defined $updates{$update}->{'installed'} );
    $required{ $update } = $updates{$update};
  }

  if ( scalar( keys %required ) == 0 ){
    print <<END
<script>
  document.getElementById('status').innerHTML = "All updates installed";
</script>
END
;		
  } else {
    my $status = "System requires ".scalar( keys %required )." updates";

    print <<END
<script>
  document.getElementById('status').innerHTML = "$status";
  document.getElementById('progress').style.width = '1px';
  document.getElementById('progress').style.background = '#a0a0ff';
</script>
END
;		

    # the progress bar is 600pixels wide
    # hence we need the following bits of information.

    my $width_per_update = ( 400 / (scalar( keys %required )) );
    my $complete = 0;

    sub update
    {
      my ( $percent ) = @_;
      my $distance = ( $complete * $width_per_update ) + int( int( $width_per_update / 100 ) * $percent );
      $distance = 1 if ( $distance <= 0 );

      print <<END
<script>
  document.getElementById('progress').style.width = '${distance}px';
</script>
END
;
    }
  
    my $error;

    foreach my $req ( sort keys %required ){
      print STDERR "going for download of $req ($required{$req}{'name'})\n";
      $status = "Downloading update $required{$req}{'name'}";

      print <<END
<script>
  document.getElementById('status').innerHTML = "$status";
</script>
END
;

      my ( $down, $percent, $speed );
    
      my $uri = "http://roadster.agcl.us/updates/$major/";
      my $filename = "$required{$req}{'name'}.tar.gz";
print "$uri:$filename";
  
      download( $uri, $filename );

      my $stop = 0;

      do { 
        ( $down, $percent, $speed, $required{$req}{'file'} ) = &progress( $filename );

        my $distance = ( $complete * $width_per_update ) + int( ( $width_per_update / 100 ) * $percent );
        $distance = 1 if ( $distance <= 0 );

        print <<END
<script>
  document.getElementById('progress').style.width = '${distance}px';
</script>
END
;

        print STDERR "Returned a rather happy $percent\n";

        if ( $percent eq "100%" ){
          $stop = 1;
        } elsif( not defined $percent or $percent eq "" ){
          $stop = -1;
        } else {
          $stop = 0;
        }
        
        sleep( 1 ); 
      } while ( $stop == 0 );

      ( $down, $percent, $speed, $required{$req}->{'file'} ) = &progress( $filename );

      $complete++;
      my $comp = $width_per_update * $complete; 

      print <<END
<script>
  document.getElementById('progress').style.width = '${comp}px';
</script>
END
;

      print STDERR "Completion progress is $complete for $req - $required{$req}->{'md5'} ($required{$req}->{'size'}) - $required{$req}->{'file'}\n";
    }

    print STDERR "Completed downloading\n";

    if ( $error eq "" ){
      foreach my $req ( sort keys %required ){
        $status = "Installing update $req";
        print <<END
<script>
  document.getElementById('status').innerHTML = "$status";
</script>
END
;
        use Data::Dumper;
        print STDERR Dumper $required{$req};
       	my $worked = apply( $required{$req}->{'file'} );

        if ( not defined $worked ){
          print <<END
<script>
  document.getElementById('status').innerHTML = "$errormessage";
</script>
END
;
          $error = $errormessage;
          last;
        } 
      }
    }

    if ( $error ne "" ){
      print <<END
<script>
  document.getElementById('status').innerHTML = "One or more updates failed to install - upgrade aborted";
  document.getElementById('progress').style.width = '1px';
  document.getElementById('progress').style.background = 'none';
</script>
END
;	
    } else {
      print <<END
<script>
  document.getElementById('status').innerHTML = "Updates Installed";
  document.getElementById('progress').style.background = 'none';
  document.location = "/cgi-bin/updates.cgi";
</script>
END
;	
    }		
  }

}

print <<END
</body>
</html>
END
;


sub apply
{
  my ( $f ) = @_;
  print STDERR "Applying Patch $f\n";
  
  unless (mkdir("/var/patches/$$",0700))
  {
    $errormessage = $tr{'could not create directory'};
    print STDERR "returning $errormessage\n";
    tidy();
    return undef;
  }
  unless (open(FH, ">/var/patches/$$/patch.tar.gz"))
  {
    $errormessage = $tr{'could not open update for writing'};
    print STDERR "returning $errormessage\n";
    tidy();
    return undef;
  }

  print STDERR "Writing /var/patches/$$/patch.tar.gz\n";
  
  if ( defined $f ){
    use File::Copy;
    move( $f, "/var/patches/$$/patch.tar.gz" );
  } else {
    flock fH, 2;
    print FH $uploadsettings{'FH'};
    close(FH);
  }

  my $md5sum;
  chomp($md5sum = `/usr/bin/md5sum /var/patches/$$/patch.tar.gz`);
  my $found = 0;
  my ($id,$md5,$title,$description,$date,$url);
  print STDERR "looking for md5\n";

  unless(open(LIST, "${swroot}/patches/available"))
  {
    $errormessage = $tr{'could not open available updates list'};
    print STDERR "returning $errormessage\n";
    tidy();
    return undef;
  }
  @list = <LIST>;
  close(LIST);

  foreach (@list)
  {
    chomp();
    ($id,$md5,$title,$description,$date,$url) = split(/\|/,$_);
print STDERR "Checking MD5 Sum for $f $md5sum against $md5 ($title)\n";
    if ($md5sum =~ m/^$md5\s/)
    {
      $found = 1;
      last;
    }
  }
print STDERR "Checking Authority\n";
  unless ($found == 1)
  {
    $errormessage = $tr{'this is not an authorised update'};
    print STDERR "$md5 $errormessage";
    tidy();
    return undef;
  }
print STDERR "Attempting Tar Operation\n";
  unless (system("/usr/bin/tar", "xfz", "/var/patches/$$/patch.tar.gz", "-C", "/var/patches/$$") == 0)
  {
    $errormessage = $tr{'this is not a valid archive'};
    print STDERR "$errormessage";
    tidy();
    return undef;
  }
print STDERR "Attempting to extract information file\n";
  unless (open(INFO, "/var/patches/$$/information"))
  {
    $errormessage = $tr{'could not open update information file'};
    print STDERR $errormessage;
    tidy();
    return undef;
  }
  my $info = <INFO>;
  close(INFO);
print STDERR "Checking status of installed updates\n";
  open(INS, "${swroot}/patches/installed") or $errormessage = $tr{'could not open installed updates file'};
  while (<INS>)
  {
    my @temp = split(/\|/,$_);
    if($info =~ m/^$temp[0]/)
    {
      $errormessage = $tr{'this update is already installed'};
      print STDERR $errormessage;
      tidy();
      return undef;
    }
  }
  chdir("/var/patches/$$");
  
  if (system( '/usr/bin/setuids/installpackage', $$))
  {
    $errormessage = $tr{'smoothd failure'};
    tidy();
    return undef;
  }
  
  unless (open(IS, ">>${swroot}/patches/installed")) {
 		$errormessage = $tr{'update installed but'}; }
  flock IS, 2;
  my @time = gmtime();
  chomp($info);
  $time[4] = $time[4] + 1;
  $time[5] = $time[5] + 1900;
  if ($time[3] < 10) { $time[3] = "0$time[3]"; }
  if ($time[4] < 10) { $time[4] = "0$time[4]"; }
  print IS "$info|$time[5]-$time[4]-$time[3]\n";
  close(IS);
  tidy();
  
  &log("$tr{'the following update was successfully installedc'} $title"); 
}

sub tidy
{
return;
  print STDERR "Tidying up\n";

  opendir(CUSTOM, "/var/patches/$$/");
  my @files = readdir (CUSTOM);
  closedir(CUSTOM);

  foreach my $file (@files)
  {
    print STDERR "Unlinking $file\n";
    next if ( $file =~ /^\..*/ );
    unlink "/var/patches/$$/$file";
  }

  print STDERR "Removing directory $$\n";
  rmdir "/var/patches/$$";
}


