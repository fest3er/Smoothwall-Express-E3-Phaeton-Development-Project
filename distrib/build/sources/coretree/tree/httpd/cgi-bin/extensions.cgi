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
use smoothnet qw( :standard );

my $download_store = "/var/patches/downloads/";
my $progress_store = "/var/patches/pending/";

&showhttpheaders();

my (%uploadsettings,$errormessage);
&getcgihash(\%uploadsettings);

my @active_updates;

# determine the list of installed extensions

my %extensions;

if ( open ( my $line, "</var/smoothwall/extensions/installed" )){
	while ( my $details = <$line> ){
		chomp $details;
		my ( $name, $version, $md5, $icon, $sample, $link, $download, $file, $description ) = ( $details =~ /([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|(.*)/ );

		$extensions{ "$name-$version" } = { sample => $sample, icon => $icon, description => $description, info => $link };

	}
	close $line;
}

# perform actions based on what we were asked to do.
if ($uploadsettings{'ACTION'} eq $tr{'refresh extensions list'}){
	$errormessage = &downloadlist();
}

if (defined $uploadsettings{'download'} and $uploadsettings{'download'} eq "download"){
	# find this patch to get some additional details for it.

	if ( open ( my $line, "</var/smoothwall/extensions/available" )){
		while ( my $details = <$line> ){
			chomp $details;
			my ( $name, $version, $md5, $icon, $sample, $link, $download, $file, $description ) = ( $details =~ /([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|(.*)/ );

			if ( $file eq $uploadsettings{'file'} ){
				download( $download, $file );
			}
		}
	}
}

if (defined $uploadsettings{'cancel'} and $uploadsettings{'cancel'} eq "cancel"){
	cancel( $uploadsettings{'file'} );
}

&openpage($tr{'extensions'}, 1, '', 'extensions');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

#&progress_bar( $fili );
#&update_bar( $fili );

&openbox();

print <<END
<table class='centered'>
<tr>
	<td>$tr{'extensions description'}</td>
	<td style='text-align: center;'><form method='post'><input type='submit' name='ACTION' value='$tr{'refresh extensions list'}'></form></td>
</tr>
</table> 
END
;

&closebox();

# open the list of (available) extensions and detail them.

&openbox();

print "<strong>$tr{'available extensions'}</strong><br/>\n";

if ( open ( my $line, "</var/smoothwall/extensions/available" )){

	# are we connected to the internet ?  (this has a bearing on the icons
	# we display

	my $connected = "false";
	$connected = "true" if ( -e "$swroot/red/active" );

	while ( my $details = <$line> ){
		chomp $details;
		my ( $name, $version, $md5, $icon, $sample, $link, $download, $file, $description ) = ( $details =~ /([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|(.*)/ );

		# is it already installed ?
	
		next if ( defined $extensions{ "$name-$version" } );

		if ( $icon eq "" or ( $connected eq "false" ) ){
			$icon = "/ui/img/extension.gif";
		}

		if ( $sample eq "" or ( $connected eq "false" ) ){
			$sample = "&nbsp;";
		}

		next if ( $file eq "" );

		print <<END
<table class='centered'>
<tr>
	<th rowspan='3' class='extension' style='width: 55px;'><img src='$icon' alt='extension'></th>
	<th colspan='2' class='extension'>$name ($version)</th>
</tr>
<tr class='dark'> 
	<td>$description</td>
	<td style='width: 60px;'>$sample</td>
</tr>
<tr class='dark'>
	<td colspan='2' style='text-align: right;'>
END
;

		&progress_bar( "$file" );
		&update_bar( "$file" );
	
		push @active_updates, $file;

print <<END
<!--<form><input type='submit' value='install'></form>-->
</td>
</tr>
</table>
<br/>
END
;
	}
}

print "<strong>$tr{'installed extensions'}</strong><br/>\n";


foreach my $extension ( keys %extensions ){

	# detail the installed extensions.

	my ( $name, $version ) = ( $extension =~ /(.*)\-(.*)/ );

	print <<END
<table class='centered'>
<tr>
	<th rowspan='3' class='extension' style='width: 55px;'><img src='$extensions{$extension}->{'icon'}' alt='extension'></th>
	<th colspan='2' class='extension'>$name ($version)</th>
</tr>
<tr class='dark'> 
	<td>$extensions{$extension}{'description'}</td>
	<td style='width: 60px;'>$extensions{$extension}{'sample'}</td>
</tr>
</table>
<br/>
END
;
}

&closebox();

&closebigbox();

my $awaiting = 0;

do {
	$awaiting = 0;
	foreach my $file ( @active_updates ){
		my $response = &update_bar( $file );

		if ( $response == 1 ){
			$awaiting++;
		} elsif ( $response == 0 ) {
			# extension has been downloaded, we should install it.
			&apply($file);
		}
		sleep ( 1 );
	}
} while ( $awaiting > 0 );
	

while ( &update_bar( $fili ) == 1 ){ sleep( 1 ); };

&closepage();

# -------------------- Functions and stuff ------------------------------------

sub apply
{
	my ( $file ) = @_;

	# remove the download container.

	print <<END
<script>document.getElementById('container-$file').style.display = 'none';</script>
END
;

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

	use File::Copy;
	move( "$download_store/$file", "/var/patches/$$/patch.tar.gz" );

	my $md5sum;
	chomp($md5sum = `/usr/bin/md5sum /var/patches/$$/patch.tar.gz`);
	my $found = 0;
	my ($id,$date,$url);
	my ( $title, $version, $md5, $icon, $sample, $link, $download, $file, $description );

	unless(open(LIST, "${swroot}/extensions/available"))
	{
		$errormessage = $tr{'could not open available extensions list'};
		print STDERR "returning $errormessage\n";
		tidy();
		return undef;
	}
	@list = <LIST>;
	close(LIST);

	foreach (@list)
	{
		chomp();
		( $title, $version, $md5, $icon, $sample, $link, $download, $file, $description ) = ( $_ =~ /([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|([^\|]*)\|(.*)/ );
		if ($md5sum =~ m/^$md5\s/)
		{
			$found = 1;
			last;
		}
	}
	unless ($found == 1)
	{
		$errormessage = $tr{'this is not an authorised extension'};
		print STDERR "$md5 $errormessage";
		tidy();
		return undef;
	}
	unless (system("/usr/bin/tar", "xfz", "/var/patches/$$/patch.tar.gz", "-C", "/var/patches/$$") == 0)
	{
		$errormessage = $tr{'this is not a valid archive'};
		print STDERR "$errormessage";
		tidy();
		return undef;
	}

	unless (open(INFO, "/var/patches/$$/information"))
	{
		$errormessage = $tr{'could not open update information file'};
		print STDERR $errormessage;
		tidy();
		return undef;
	}
	my $info = <INFO>;
	close(INFO);
	open(INS, "${swroot}/extensions/installed") or $errormessage = $tr{'could not open installed extensions file'};
	while (<INS>)
	{
		my @temp = split(/\|/,$_);
		if($info =~ m/^$temp[0]/)
		{
			$errormessage = $tr{'this extension is already installed'};
			print STDERR $errormessage;
			tidy();
			return undef;
		}
	}

	chdir("/var/patches/$$");
#	unless (system("/usr/local/bin/installpackage $$") == 0)
#	{
#		$errormessage = $tr{'package failed to install'};
#		print STDERR $errormessage;
#		tidy();
#		return undef;
#	}
	unless (open(IS, ">>${swroot}/extensions/installed")) {
 		$errormessage = $tr{'extension installed but'}; }
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
	&log("$tr{'the following extension was successfully installedc'} $title"); 
}


sub downloadlist
{

	print STDERR "Looking for extensions\n";

	my %proxy;
	&readhash("${swroot}/main/proxy", \%proxy);

	my $host; my $port;
        unless ($proxy{'SERVER'})
        {
                $host = 'www.smoothwall.org';
                $port = 80;
        }
        else
        {
                $host = $proxy{'SERVER'};
                $port = $proxy{'PORT'};
        }
	my $sock;
	unless ($sock = new IO::Socket::INET (PeerAddr => $host, PeerPort => $port,
		Proto => 'tcp', Timeout => 5))
	{
		$errormessage = $tr{'could not connect to smoothwall org'};
		print STDERR "unable to connect $errormessage:\n";
		return $errormessage;
	}
	$version = "2.0";
	print $sock "GET http://www.smoothwall.org/extensions/$version HTTP/1.1\r\nHost: www.smoothwall.org\r\nConnection: close\r\n\r\n";
	my $ret = '';
	while (<$sock>) {
		$ret .= $_; }
	close($sock);

	print STDERR "Returned $ret\n";

	if ($ret =~ m/^HTTP\/\d+\.\d+ 200/)
	{
		unless (open(LIST, ">${swroot}/extensions/available"))
		{
	                $errormessage = "$tr{'could not open available updates file'} $!";
print STDERR "nope, that didn't work :(\n";
      	 	        return $errormessage;
        	}
		flock LIST, 2;
	        my @this = split(/----START LIST----\n/,$ret);
		print LIST $this[1];
		close(LIST);
	} else {
		$errormessage = "$tr{'could not open available updates file'} $ret";
      	 	return $errormessage;
	}
		 

	return "";
}

sub tidy
{
	print STDERR "Tidying up\n";

	opendir(CUSTOM, "/var/patches/$$/");
	my @files = readdir (CUSTOM);
	closedir(CUSTOM);

	foreach my $file (@files) {
		print STDERR "Unlinking $file\n";
		next if ( $file =~ /^\..*/ );
		unlink "/var/patches/$$/$file";
	}

	print STDERR "Removing directory $$\n";
	rmdir "/var/patches/$$";
}
