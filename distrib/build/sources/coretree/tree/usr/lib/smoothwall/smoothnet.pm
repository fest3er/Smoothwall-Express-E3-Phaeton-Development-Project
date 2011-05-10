# (c) 2004-2005 SmoothWall Ltd

package smoothnet;
use lib "/usr/lib/smoothwall";
use header qw(:standard);
require Exporter;
use File::Copy;
@ISA = qw(Exporter);

# define the Exportlists.

@EXPORT       = qw();
@EXPORT_OK    = qw( download progress progress_bar update_bar $progress_store $download_store $server cancel clear_download_cache checkstatus checkmd5 );
%EXPORT_TAGS  = (
		standard   => [ @EXPORT_OK ],
		);

our $server        = "roadster.agcl.us/updates";

my $download_store = "/var/patches/downloads/";
my $progress_store = "/var/patches/pending/";

sub checkmd5
{
	my ( $filename , $md5 ) = @_;

	my $destination = "$downloadstore/$filename";
	# calculate the MD5 of the file we just downloaded
        my $md5sum;

	open(PIPE, '-|') || exec('/usr/bin/md5sum', "$destination");
	
	while (<PIPE>) { $md5sum = $md5sum.$_; }
	close (PIPE);
	
	chomp $md5sum;
	$md5sum =~ /^([\da-fA-F]+)/;
	$md5sum = $1;
	if ($md5sum eq $md5) {
		# file is ok, it has the correct md5
		return 1;
	} else {
		# failed;
		warn "Failed $md5sum\n";
		return 0;
	}
}

sub download
{
	my ( $base, $file ) = @_;
	print STDERR "Going for download... ($file)\n";

	my %proxy;
	&readhash("${swroot}/main/proxy", \%proxy);
	my @proxy_opt = ();
	if ($proxy{'SERVER'}) {
		my $server = $proxy{'SERVER'};
		my $port = $proxy{'PORT'} || 80;
		@proxy_opt = ("-e", "http_proxy = http://$server:$port/");
	}

	# invoke wget to download a file and store accordingly.

	my $final = "$progress_store$file";
	my $log   = "$progress_store$file.log";
	my $pid   = "$progress_store$file.pid";

	my @commands = ( "/usr/bin/wget", @proxy_opt, "-o", "$log", "-b", "--progress=bar", "-O", "$final", "$base$file" );
	print STDERR join(" ", @commands), "\n";

	my ( $status, $pid_out );
	open(PIPE, '-|') || exec( @commands );
        while (<PIPE>) { 
		$status .= $_; 
	}
	close(PIPE);

	# determine PID information (which was told to us by wget
	my ( $pid_number ) = ( $status =~ /.*pid\s+(\d+)\..*/m );
	
	print STDERR "wget returned $status\n";

	unless ( open ( $pid_out, ">$pid" ) ){
		print STDERR "Unable to allocate PID $pid\n";
		kill $pid_number;
		return "cannot allocate pid";
	}

	print $pid_out "$pid_number";
	close $pid_out;

	sleep( 3 ); # give things a chance to start

	print STDERR "Got the reply from wget as $status\n";
	print STDERR "Going to $final and $log $pid ($pid_number)\n";

}

sub progress
{
	my ( $file ) = @_;

	# check on the status of the download of $file
	my ( $status_file, $status, $pid_file, $pidnumber );

	my $log   = "$progress_store$file.log";
	my $pid   = "$progress_store$file.pid";

	unless( open ( $pid_file, "<$pid" ) ){
		if ( -e "$download_store$file" ){
			# download is completed ...
			print STDERR "Download is complete...\n";
			return ( "", "100%", "-", "$download_store$file" );
		}

		print STDERR "Cannot open $pid\n";
		return "error";
	}

	unless( open ( $status_file, "<$log" ) ){
		if ( -e "$download_store$file" ){
			# download is completed ...
			return ( "", "100%", "-", "$download_store$file" );
		}

		print STDERR "Cannot open $log\n";
		return "error";
	}


	my ( $down, $percent, $speed, $complete ) = ( 0, "0px", 0, "" );

	while ( $status = <$status_file> ) { 
		my ( $ddown, $dpercent, $dspeed ) = ( $status =~ /(\d+K|M)[^0-9]+(\d+%)\s+(\d*\.*\d*[KMG][ =]+\d*\.*\d*s)/ );
		if ( defined $ddown ){
			$down = $ddown;
			$percent = $dpercent;
			$speed = $dspeed;
		}
                #print STDERR "down/speed/percent: $down/$speed/$percent\n";
		if ( $percent eq "100%" ){
			print STDERR "Checking for completion $status\n";
			if ( $status =~ /`([^`]*)'/ ){
				$complete = $1;
			}
		}	
	};

	# status should now be the last line of the status ...

	if ( $complete eq "$progress_store$file" ){
		# download is complete, move the file to the download cache
		# and then tidy up.
		my $final = "$download_store$file";
		move( "$progress_store$file", $final );
		downloadtidy( $file );
		return ( $down, $percent, $speed, $final );
	}

	# get the PID and check it.
	$pidnumber = <$pid_file>;

	$status = &checkstatus( $pidnumber );

	if ( $status != 1 ){
		print STDERR "unable to get associated Process for $pidnumber $status\n";
		return "error";
	}

	return ( $down, $percent, $speed );
}

sub cancel 
{
	my ( $file ) = @_;
	
	# cancel a download, killing the process (if it's running)

	#retrieve the process ID
	my $pid   = "$progress_store$file.pid";
	my $pid_in;

	unless ( open ( $pid_in, "<$pid" ) ){
		return "cannot get pid";
	}
	
	my $pid_number = <$pid_in>;
	close $pid_in;

	my $status = checkstatus( $pid_number );

	if ( $status == 1 ){
		kill 15, $pid_number;
	}

	downloadtidy( $file );
}


sub checkstatus
{
	my $pid_number = $_[0];
	# check that this PID is a) running and b) belongs to wget...

	if (open(FILE, "/proc/${pid_number}/status"))
	{
		while (<FILE>)
		{
			if (/^Name:\W+(.*)/) {
				$testcmd = $1; }
		}
		close FILE;
		if ($testcmd =~ /wget/)
		{
			print STDERR "W000T found the process for $pid (it's running too)\n";
			return 1;
		} else {
			print STDERR "This is not the process you're looking for\n";
			return 0;
		}
	} else {
		print STDERR "No process to kill, guess it's not there\n";
		return 0;
	}	
}

sub downloadtidy
{
	# tidy up the PID and progress files for the relevant file.

	my ( $file ) = @_;

	my $final = "$progress_store$file";
	my $log   = "$progress_store$file.log";
	my $pid   = "$progress_store$file.pid";
	
	unlink $log;
	unlink $pid;
	unlink $final;

}

sub progress_bar
{
	# create a progress bar to display the download progress
	# and management of the file $file ($_[0])

	my ( $file, $url_override ) = @_ ;

	print <<END
<table style='float: right;' id='container-$file'>
<tr>
	<form method='post'>
	<td>
END
;

	# get the intial status and render controls accordingly.

	my ( $down, $percent, $speed, $complete ) = &progress( $file );

	if ( $percent eq "100%" ){
		# download is complete... hmm
	} elsif( not defined $percent or $percent eq "" ){
		# there was an error somewhere, or we don't have it
		print <<END
	<input type='hidden' name='file' value='$file'>
	<input type='submit' name='download' value='download' id='button-$file'>
END
;
	} else {
		# we are currently downloading the thing, we can issue a cancel if we want
		print <<END
		<table class='progressbar'>
			<tr>
				<td id='progress-$file' style='background-color: blue; width: 1px;'>&nbsp;</td>
				<td class='progressend'>&nbsp;</td>
			</tr>
		</table>
	</td>
	<td rowspan='2' style='vertical-align: top;'>

	<input type='hidden' name='file' value='$file'>
	<input type='submit' name='cancel' value='cancel'>
END
;
	}

print <<END
	</td>
</tr>
<tr>
	<td><span id='status-$file'></span></td>
</tr>
</form>
</table>
END
;
	
}

sub update_bar
{
	my ( $file ) = @_;

	my ( $down, $percent, $speed, $complete ) = &progress( $file );

	print <<END
<script>
	document.getElementById('progress-$file').style.width = '${percent};';
</script>
END
;

	if ( $percent eq "100%" ){
		return 0;
	} elsif( not defined $percent or $percent eq "" ){
		return -1;
	} else {
		return 1;
	}
}

sub clear_download_cache
{
	opendir( my $dir, "/var/patches/downloads" );

	my @list = readdir( $dir );

	closedir( $dir );

	foreach my $file ( @list ){
		next if ( $file =~ /^\..*/ );
		unlink( "/var/patches/downloads/$file" );
	}
}

1;
