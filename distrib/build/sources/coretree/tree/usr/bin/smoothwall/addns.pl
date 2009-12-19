#!/usr/bin/perl  

# Addns.pl
# David Hasson    	     
# 07-20-03
# Licensed under the GPL License 
# Version 1.01e                  

use IO::Socket;
use Sys::Hostname;
use integer;
use English;
use strict;

my $creator = 'dph@funtaff.com';	
my $hostname = hostname();
my $progname = $0;
my $progshort = "addns";
my $pid = 0;
my $ver = "1.01e";
my $agent = "Addns.pl/$ver $creator";
my $curr_host = '';		# name of subhash for current config (multiple hosts / interfaces)
my $quiet = 0;			# depreciated, keeps output happy TODO
my $debug = 0;			# debug level
my $force = 0;			# force updates
my $defaultsystem = "dyndns";	# default system when none is specified

my %system_v = (
	'proxy_host' => '',
	'proxy_port' => "80",
	'use_proxy' => 0,
);

my %host_cache;
	# Hash for hostnames and thier last updated IP's

my %default = (
	'update_host' => '',
	'detect_method' => 'iface',
		# manual, iface, webcheck?
	'update_ip' => '0.0.0.0',
	'iface' => 'eth0',
	'username' => '',
	'password' => '',
	'last_ip' => '0.0.0.0',
	'ip_detect_host' => 'checkip.dyndns.org',
	'ip_detect_port' => '80',
	'ip_detect_look' => 'Current IP Address:',
	'ip_detect_url' => '/',
	'server_host' => 'members.dyndns.org',
	'server_port' => 80,
	'system_type' => '',
	'mx_record' => '',
	'back_mx' => 'nochg',
	'offline' => 'no',
	'active' => 1
);

my %host_v;
#%{ $host_v{'default'} } = %default;

my %daemon_v = (
	'interval' => 600,
	'running' => 0,
);

my %files = (
	'pid' => "/var/run/$progshort.pid",
	'log' => "/var/log/$progshort.log",
	'config' => "/etc/$progshort.conf",
	'cache' => "/etc/$progshort.db"
);

# Format: (value, flag, posix_flag, description, bool?);
my %cmd_args = (
	'config_name' => ['', '-x', '--configname', "Configuration Label to Update"],
	'update_host' => ['', '-H', '--host', "Hostname to Update"],
	'detect_method' => ['', '-m', '--method', "Method for determining IP address: manual, iface or webcheck"],
		# manual, iface, webcheck, router?
	'update_ip' => ['', '-l', '--ip', "IP Address for update (manual)"],
	'iface' => ['', '-n', '--interface', "Interface to obtain IP address from"],
	'username' => ['', '-u', '--username', "Dyndns.org Username"],
	'password' => ['', '-p', '--password', "Dyndns.org Password"],
	'ip_detect_host' => ['', '', '--detect_host', "Hostname/IP of the system that will validate IP address"],
	'system_type' => ['', '-s', '--system', "Dyndns.org system type: statdns, dyndns, custom"],
	'mx_record' => ['', '-m', '--mx', "MX record host/ip"],
	'back_mx' => ['', '-M', '--backup_mx', "Is MX record primary or backup?: yes, no, nochg"],
	'offline' => [0, '-o', '--offline', "Sets host offline: yes, no", 1],
	'help' => [0, '-h', '--help', "Displays this help information", 1],
	'verbose' => [0, '-v', '--verbose', "Tell me more of whats going on", 1],
	'morverbose' => [0, '-vv', '--debug', "Show me everything", 1],
	'daemon' => [0, '-d', '--daemon', "Run as a daemon", 1],
	'force' => [0, '-f', '--force', "Force Updates", 1],
	'interval' => ['', '-x', '--interval', "Update Interval for Daemon", 1]
);
	
$| = 1;


# output() subroutine.  call in the form of output("message", write_to_log, write_to_terminal, die_or_not)
# if write_to_log is 1, then it logs it. if write_to_terminal is 1, writes to STDOUT, it it's 2
# write to STDERR.  It dies if die_or_not is 1.

sub output {
	write_log($_[0]) if ($_[1]==1);	
	print STDERR "$_[0]\n" if ($_[2]==2 && $quiet!=1);
	print STDOUT "$_[0]\n" if ($_[2]==3);
	print STDOUT "$_[0]\n" if ($_[2]==1 && $quiet!=1);
	if ($_[3]) {
		exit 1;
	}
}

# time_fmt() - output timestamp in a form suitable for timestamping log
#	entries

sub time_fmt {
	my($sec, $min, $hour, $mday, $mon) = localtime($_[0]);
	my($fmt_mon) = ('Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec')[$mon];
	return(sprintf("%s %d %.2d:%.2d:%.2d", $fmt_mon, $mday, $hour, $min, $sec));
}

sub write_log {
	my($entry) = time_fmt(time)." ($curr_host) $_[0]\n";
	my @log_entry = @_;
	open(LOG, ">>$files{log}");
		print LOG $entry;
	close(LOG);
}

# encode_base64() taken from Base64.pm

sub encode_base64 ($;$)
{
    my $res = "";
    my $eol = $_[1];
    $eol = "\n" unless defined $eol;
    pos($_[0]) = 0;                          # ensure start at the beginning
    while ($_[0] =~ /(.{1,45})/gs) {
        $res .= substr(pack('u', $1), 1);
        chop($res);
    }
    $res =~ tr|` -_|AA-Za-z0-9+/|;               # `# help emacs
    # fix padding at the end
    my $padding = (3 - length($_[0]) % 3) % 3;
    $res =~ s/.{$padding}$/'=' x $padding/e if $padding;
    # break encoded string into lines of no more than 76 characters each 
    if (length $eol) {
        $res =~ s/(.{1,76})/$1$eol/g;
    }   
    $res;
}

sub send_update 
{
	my(	$server_host,		# Hostname of actual http server
		$connect_host,		# Hostname of real tcp host (ie proxy)
		$ip,			# Update ip
		$coded,			# Encoded Username & Password
		$msg,			# Http GET Message
		$port			# tcp port for access
	);

	my($updt_host) = $host_v{$curr_host}{'update_host'};
	if($updt_host eq '') { 
		output("send_update: Malformed Hostname \"$updt_host\"... Use host.domain.suffix",1,2,1);
	}
	if($updt_host !~ m/^[\w-\.+]+$/) 
	{
		output("Malformed Hostname \"$updt_host\"... Use host.domain.suffix",1,2,1);
	}
	
	$ip = $host_v{$curr_host}{'update_ip'};
	if(! $ip =~ /^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/) { 
		output("Malformed IP Address \"$ip\"... use xxx.xxx.xxx.xxx form",1,2,1);
		return(3); 
	}
	
	my $server_host = $host_v{$curr_host}{'server_host'};

	if($system_v{'use_proxy'} eq "yes") {
		print "$curr_host: using proxy \"$system_v{'proxy_host'}:$system_v{'proxy_port'}\"\n" if $debug;
		$connect_host = $system_v{'proxy_host'};
		if($system_v{'proxy_port'}) {
			$port = $system_v{'proxy_port'};
		}
		else {
			$port = 8080;
		}
	}
	else {
		$connect_host = $server_host;
		if($host_v{$curr_host}{'server_port'}) {
			$port = $host_v{$curr_host}{'server_port'};
		}
		else {
			$port = 80;
		}
	}
	print "$curr_host: updating to host $server_host via $connect_host on port $port\n" if $debug;
	
	my $uname = $host_v{$curr_host}{'username'};
	unless($uname) { output("No Username Set (exiting)",1,2,1); }
	
	my $passwd = $host_v{$curr_host}{'password'};
	unless($passwd) { output("No Password Set (exiting)",1,2,1); }

	my($wildcard) = $host_v{$curr_host}{'wildcard'};

	my($mx) = $host_v{$curr_host}{'mx_host'};
	
	my($backmx) = $host_v{$curr_host}{'backup_mx'};

	my($system_type) = $host_v{$curr_host}{'system_type'};
	if (! ($system_type eq "statdns" || $system_type eq "dyndns" || $system_type eq "custom")) { output("Invalid Setting for System Type - either statdns, custom or dyndns are valid",1,2,1); }

	my($offline) = $host_v{$curr_host}{'offline'};

	# Update
	my $content = 'Content-Type: application/x-www-form-urlencoded';
	my $useragent = "User-Agent: $agent";


	chomp($coded = encode_base64("$uname:$passwd"));

	$msg = "GET http://$server_host/nic/update?";
	$msg .= "system=$system_type";
	$msg .= "&hostname=$updt_host";

	if($ip)           { $msg .= "&myip=$ip"; }
	if($offline)           { $msg .= "&offline=$offline"; }

	if($wildcard)      { $msg .= "&wildcard=$wildcard"; }
        else                    { $msg .= "&wildcard=NOCHG"; }

	if($mx)
                            { $msg .= '&mx=' . $mx; }

	if($backmx)        { $msg .= '&backmx=Y'; }
        else                    { $msg .= '&backmx=N'; }

	$msg .= " HTTP/1.1\r\n";
	$msg .= "Host: $server_host\r\n";
	$msg .= "Authorization: Basic $coded\r\n";
	$msg .= "$useragent\r\n";
	$msg .= "$content\r\n";
	$msg .= "\r\n";

	#tcp code
	my($proto,$iaddr,$sin,@return);
	my $socket_type = 'tcp';

	$proto = getprotobyname($socket_type);
	socket(UpdaTe, PF_INET, SOCK_STREAM, $proto) || die "socket: $!";
	$iaddr = gethostbyname($connect_host);
	$sin = sockaddr_in($port, $iaddr); 

	connect(UpdaTe, $sin) || die "connect: $!";
	output("$curr_host: performing update, ip=$ip",1,1);

	if($debug>1) {
		print "\n--Client Request--\n";
		print "$msg\n";
	}
	
	send(UpdaTe, $msg, 0);
	@return = <UpdaTe>;
	close(UpdaTe);

	if($debug > 1)
	{
		print "--Server Response--\n";
		
		foreach (@return) { print ; }

		print "------------------\n\n";
	}
	
	my $out = "@return";


	
# There are a couple return codes.  0 means success.  1 means falure, but resume anyways in case
# It's involked with --auto.  2 means it failed and stop updating this host no matter what.  If the host name is
# Blocked, the whole program will die.


	foreach (@return) {
		(/999/ || /911/) && output("Emergency code from server.",1,3,1);
		 /good/ && ($offline) && return(0, "Success - Host set offline");
		 /good/ && return(0, "Success");
		 (/nochg/ || /abuse/) && return(2, "Hostname Blocked due to Abuse");
		 (/notfqdn/) && return(2, "Bad Hostname (Malformed)");
		 (/nohost/) && return(2, "Bad Hostname (Does not exist)");
		 (/!yours/) && return(2, "Bad Hostname (Is not yours)");
		 (/numhost/) && do return(1, "Too many or too few hosts found (Server Error)"); 
		 (/dnserr/) && do {
			 chomp $_;
			 my $num;
			 ($_, $num) = split / /;
			 output("DNS ERROR: Report Packet ID to DynDNS -- $num --",1,3,1); 
		 };
		 /w(..)h/ && do {
			 output("Wait condition: $1 hours",1,3);
			 if($daemon_v{'running'}) { sleep $1*60*60; } else { die "Exiting.."; }
		 };
		 /w(..)m/ && do {
			 output("Wait condition: $1 minutes",1,3);
			 if($daemon_v{'running'}) { sleep $1*60; } else { die "Exiting.."; }
		 };
		 /w(..)s/ && do {
			 output("Wait condition: $1 seconds",1,3);
			 if($daemon_v{'running'}) { sleep $1; } else { die "Exiting.."; }
		 };
		 /badauth/ && return(2, "Access Denied: Bad Username/Password Pair");
		 /badsys/ && return(2, "Bad system parameter - Check config, then contact dph\@funtaff.com");
		 /badagent/ && return(2, "Client has been blocked.  Contact dph\@funtaff.com ASAP");
	}
	
	if($out eq "") {
		return(1, "Connection Error");
	} else {
		return(2, "Failure");
	}

	exit;

}

sub put_host_cache
{
	open(CACHE, ">" . $files{'cache'}) or output("warning: could not open cache file!",1,2); 
	foreach(keys %host_cache) {
		print "cachewrite: $_:$host_cache{$_}[0]:$host_cache{$_}[1]\n" if $debug>1;
		print CACHE "$_:$host_cache{$_}[0]:$host_cache{$_}[1]\n";
	}
	close(CACHE);
}

sub get_host_cache
{
	open(CACHE, $files{'cache'}) or output("warning: could not open cache file!",1,2); 
	my @lines = <CACHE>;
	my $empty = {};
	close(CACHE);

#%host_cache = $empty;
	unless($force) {
		foreach(@lines) {
#			next if ! /^([a-zA-Z0-9.-]+?):([0-9.].+?):([0-9]+?)$/;
			next if /^\s*$/;
			chomp;
			my($host, $ip, $time) = split(/:/);
			@{ $host_cache{$host}} = ($ip, $time);
		}	
	}
	else {
		put_host_cache();
	}
}


sub get_config
{	
	my %config_raw;	# raw text of each "section"
	my $flag;

	## Check for Rcfile, first in the home dir.

	if (! -s $files{'config'} ) {
		output("No configuration found. See README.",0,2,1);
	}

	open(CONFIGRC, $files{'config'}) or output("Can't open Config file!",0,2,1);
	my @listlines = <CONFIGRC>;
	close(CONFIGRC);
	
	my $text = join("", @listlines);  # Process entire file as a string in next block

	print "get_config: Looking for sections\n" if $debug==2;
	do {		# search through file for "sections"
		$flag=0;
		if($text =~ m/\[([\w-\._]+?)\]\s*?\{(.*?)\}/gs) {
			$config_raw{$1} = $2;
			$flag=1;
			print "get_config: \tFound Section \"$1\"\n" if $debug==2;
		}
	
	} while($flag==1);
		
	foreach my $section (keys %config_raw) {
		if($section =~ /^(default|main|daemon)$/) { 
		}
		else {
			foreach(keys %default) {
				$host_v{$section}{$_} = $default{$_}; 
			}
		}
		my @sec_lines = split(/\n/, $config_raw{$section});
		foreach my $line (@sec_lines) {
			$line =~ s/\#.*$//;	# Remove Comment
			$line =~ m/^\s*$/ && next;	# Line is empty, continue
			$line =~ m/^\s*([\w\-\._]+)\s*=\s*"?(.+?)"?\s*$/ && do {
				if($section eq "main") {
					$system_v{$1} = $2;
				}
				elsif($section eq "daemon") {
					$daemon_v{$1} = $2;
				}
				else {
					$host_v{$section}{$1} = $2;
				}
			} 
		}
	}
	
	if($debug > 1) {
		print "\n--Current Configuration--\n";
		print "Host Configurations:\n";
		foreach my $section (keys %host_v) {
			print "\tEntering Section: $section\n";
			foreach my $varname (keys %{ $host_v{$section} } ) {
				print "\t\t$varname = \"$host_v{$section}{$varname}\"\n";
			}
		}
		print "System Configuration:\n";
		foreach my $varname (keys %system_v) {
				print "\t\t$varname = \"$system_v{$varname}\"\n";
		}
		print "Daemon Options:\n";
		foreach my $varname (keys %daemon_v) {
				print "\t\t$varname = \"$daemon_v{$varname}\"\n";
		}
		print "System Defaults:\n";
		foreach my $varname (keys %default) {
				print "\t\t$varname = \"$default{$varname}\"\n";
		}
		print "--Current Configuration--\n\n";
	}
}

sub get_cmd_args
{
	foreach my $arg (@ARGV) {
		if($arg =~ /^(\-{1,2}[\w\_\.]*)=([^\s]*)$/) {
			my $var = $1;
			my $val = $2;
			foreach my $array (keys %cmd_args) {
				if($var eq $cmd_args{$array}[1] || $var eq $cmd_args{$array}[2]) {
					@{ $cmd_args{$array}}[0] = $val;
				}
			}
		}
		elsif($arg =~ /^(\-{1,2}[\w\_\.]*)$/) {
			my $var = $1;
			foreach my $array (keys %cmd_args) {
				if($var eq $cmd_args{$array}[1] || $var eq $cmd_args{$array}[2]) {
					@{ $cmd_args{$array}}[0] = 1;
				}
			}
		} 
		else {
			output("Error processing command line argument \"$arg\"",0,1,1);
		}
	}
	if($cmd_args{'help'}[0]) {
		print "Addns.pl $ver -- Created by David Hasson <$creator>\n";
		print "Usage: $0 [--option=value] [-flag]... etc.\n";
		print "\n";
		print "Updates your www.dyndns.org host\n";
		print "\n";
		print "Options:\n";
		foreach my $array (keys %cmd_args) {
			unless($cmd_args{$array}[4]==1) {
				print "\t$cmd_args{$array}[2] | $cmd_args{$array}[1]\n\t\t$cmd_args{$array}[3]\n";
			}
		}
		print "Flags:\n";
		foreach my $array (keys %cmd_args) {
			if($cmd_args{$array}[4]==1) {
				print "\t$cmd_args{$array}[2] | $cmd_args{$array}[1]\n\t\t$cmd_args{$array}[3]\n";
			}
		}
		exit(1);
	}

	if($cmd_args{'verbose'}[0]) {
		$debug = 1;
	}
	elsif($cmd_args{'morverbose'}[0]) {
		$debug = 2;
	}

	if($cmd_args{'daemon'}[0]) {
		$daemon_v{'running'} = 1;
	}

	if($cmd_args{'force'}[0]) {
		$force = 1;
	}

	
	if($cmd_args{'interval'}[0] ne "") {
		$daemon_v{'running'} = $cmd_args{'$interval'}[0];
	}

	if($debug>1) {
		print "Command Arguments:\n";
		foreach my $array (keys %cmd_args) {
			print "\t\t$array = \"@{ $cmd_args{$array}}[0]\"\n";
		}
	}
		
}

	

# Routine for determining hostname if none is given
sub get_hostname
{
	my $host;
	unless($host_v{$curr_host}{'update_host'}) {
		die unless defined($pid = open(KID, "-|"));
		if($pid) {
			chomp($host = <KID>);
			close(KID);
			$host_v{$curr_host}{'update_host'} = $host;
		} else {
			$EUID = $UID;
			$EGID = $GID;
			$ENV{PATH} = "/bin:/usr/bin";
			$ENV{ENV} = '';
			exec '/bin/hostname';
		}	
	}
}



sub web_ip_detect
{
        my $useragent = "User-Agent: $agent";

        my $server_name = $host_v{$curr_host}{'ip_detect_host'};
        my $port;
	$port = $host_v{$curr_host}{'ip_detect_port'} or $port = 80;
        my $socket_type = 'tcp';

        my $msg = "GET $host_v{$curr_host}{'ip_detect_url'}";


        $msg .= " HTTP/1.1\r\n";
        $msg .= "Host: $server_name\r\n";
        $msg .= "$useragent\r\n";
        $msg .= "\r\n";

        my($proto,$iaddr,$sin,@return);

        $proto = getprotobyname($socket_type);
        socket(SERVER, PF_INET, SOCK_STREAM, $proto) || do {
		output("WebIP Socket Failure: $!",1,1);
		return 0;
	};
        $iaddr = gethostbyname($server_name);
        $sin = sockaddr_in($port, $iaddr);

        connect(SERVER, $sin) || do {
		output("WebIP Connect Failure: $!",1,1);
		return 0;
	};
        send(SERVER, $msg, 0);
        @return = <SERVER>;
        close(SERVER);

        if($debug > 1)
        {
                print "\n--CheckIP Client Request--\n";
                print "$msg\n";
                print "\n--CheckIP Server Response--\n";

                foreach my $line (@return)
                {
                        print $line;
                }
                print "------------------\n\n";
        }

        my $out = "@return";
	print "$curr_host: IP webcheck via http://$host_v{$curr_host}{'ip_detect_host'}:$host_v{$curr_host}{'ip_detect_port'}$host_v{$curr_host}{'ip_detect_url'}, looking for \"$host_v{$curr_host}{'ip_detect_look'}\"\n" if $debug; 
 
        if($out =~ m/($host_v{$curr_host}{'ip_detect_look'})[ \t](\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})[<\s\n]/)
        {
                my $wip = $2;
                return $wip;
        } else
        {
                return 0;
        }
}

sub get_ip
{
#POST: outputs an ip address: 

	my $method = $host_v{$curr_host}{'detect_method'};
	my $ip;
	my $ifconfig_bin = '/sbin/ifconfig';

	print "get_ip: \"$curr_host\" trying method: $method\n" if($debug>1);

	if($method eq "manual") {
		if($cmd_args{'update_ip'}[0] ne '') {
			$ip = $cmd_args{'update_ip'}[0]; 
		} elsif($host_v{$curr_host}{'update_ip'}) {
			$ip = $host_v{$curr_host}{'update_ip'};
		} else
		{
			output("get_ip: manual ip not specified",1,3); 
		}
	} 
	elsif($method eq "iface") {
		my $iface = $host_v{$curr_host}{'iface'};
		$iface = s/\W//g;	# remove all but alphanumeric characters

		die unless defined($pid = open(KID, "-|"));
		if($pid) {
			$_ = join("\n", (<KID>));
			close(KID);
		} else {
			$EUID = $UID;
			$EGID = $GID;
			$ENV{PATH} = "/bin:/usr/bin:/sbin:/usr/sbin";
			$ENV{ENV} = '';
			exec "$ifconfig_bin $host_v{$curr_host}{'iface'}";
		}	
	
		if(s/^.*inet[ \t:]*(addr|adr)*[ \t:]*(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}).*$/$2/s) {
			if(/^192\.168\..*$/ || /^10\..*$/ || /^172\.((1[6-9])|(2\d)|(3[01]))\..*$/) {
				output("IP on $host_v{$curr_host}{'iface'} is an Private IP ($_).  Cannot Update",1,2,0);
			} else { 
				s/[\n\ \t]//g;
				$ip = $_;
				$host_v{$curr_host}{'up'} = 1;
			}
		} else {
			output("\"$iface\" down",1,1) unless ($host_v{$curr_host}{'up'} == 0);
			$host_v{$curr_host}{'up'} = 0;
		}	
	}
	elsif($method eq "webcheck") {
		$ip = web_ip_detect();
	}
	elsif($method eq "router") {
		$ip = web_ip_detect();
	}
	if($ip && $debug>1) {
		print "get_ip: \"$curr_host\" success $ip\n"; 
	} elsif($debug>1) {
		print "get_ip: \"$curr_host\" failed\n"; 
	}
		
	return $ip;
		
}
# Parent Function for each update
sub update() {
	$host_v{$curr_host}{'update_ip'} = get_ip();	
	unless($host_v{$curr_host}{'update_ip'}) {
		output("$curr_host: No Method resulted in an update IP",1,1,0);
		return 1;
	}
	my $hostname = $host_v{$curr_host}{'update_host'};

	if($hostname !~ /[\w\.]+/) {
		print("update: \"$curr_host\" - hostname not specified, trying autodetect\n") if($debug>1);
		$hostname = get_hostname();
	}
	if($hostname !~ /(\w*?\.)+\w{2,4}/) {
		output("$curr_host: No Method resulted in an update Hostname",1,1,0);
		return 2;
	}

	if($host_v{$curr_host}{'system_type'} !~ /^(statdns|dyndns|custom)$/)
	{
		output("$curr_host: system type not specified, using default - \"$defaultsystem\"",1,1,0);
		$host_v{$curr_host}{'system_type'} = $defaultsystem;
	}
 	
	$host_v{$curr_host}{'update_host'} = $hostname;

	

	print "$curr_host: Last IP was $host_cache{ $host_v{$curr_host}{'update_host'} }[0]\n" if $debug; 		
	unless(  
		( $host_v{$curr_host}{'update_ip'} eq
		   $host_cache{ $host_v{$curr_host}{'update_host'} }[0]) 		
		&& 
		( $host_cache{ $host_v{$curr_host}{'update_host'} }[1] +
		    25*60*60*24 > time()  )
		)
	{
		print "$curr_host: updating records for $hostname\n" if $debug;
		my($result, $result_msg) = send_update();
		if($result==0) {
			$host_cache{ $host_v{$curr_host}{'update_host'} }[0] = $host_v{$curr_host}{'update_ip'};
			$host_cache{ $host_v{$curr_host}{'update_host'} }[1] = time();
			put_host_cache();
			output("$curr_host: update successful\n",1,1);
		}
		elsif($result==1) {
			output("$curr_host: temporary update failure: $result_msg\n",1,1);
		}
		elsif($result==2) {
			output("$curr_host: failed update (removing from list): $result_msg\n",1,1);
		}
		return $result;
		 
	# output updated ip to host cache (todo later)
	
	} else {
		output("$curr_host: ip ($host_v{$curr_host}{'update_ip'}) already correct in database", 1, 1);
		return 3;
	}
	return 4;
}


sub run_updates 
{
	foreach my $section (keys %host_v) {
		$curr_host = $section;
		if($host_v{$curr_host}{'active'}) {
			my $res = update();
			if($res==2) {
				$host_v{$curr_host}{'active'} = 0;
			}		
		}
	}
}
	 

# main init code
# Check to see if autoupdate mode (-a, --auto) is specified and initiate a loop


get_cmd_args();
get_host_cache();
if( $cmd_args{'config_name'}[0] ) {
	get_config();
	unless($host_v{$cmd_args{'config_name'}[0] } ) {
		print "$cmd_args{'config_name'}[0]: not found\n";
		exit;
	}
	$curr_host = $cmd_args{'config_name'}[0];

	$host_v{$curr_host}{'update_host'} = $cmd_args{'update_host'}[0] if($cmd_args{'update_host'}[0] ne "");
	if($cmd_args{'update_ip'}[0] ne "") {
		$host_v{$curr_host}{'update_ip'} = $cmd_args{'update_ip'}[0] ;
		$host_v{'manual'}{'detect_method'} = "manual";
	} elsif($cmd_args{'iface'}[0] ne "") {
		$host_v{$curr_host}{'iface'} = $cmd_args{'iface'}[0] ;
		$host_v{'manual'}{'detect_method'} = "iface";
	}
	$host_v{$curr_host}{'username'} = $cmd_args{'username'}[0] if($cmd_args{'username'}[0] ne "");
	$host_v{$curr_host}{'password'} = $cmd_args{'password'}[0] if($cmd_args{'password'}[0] ne "");
	$host_v{$curr_host}{'system_type'} = $cmd_args{'system_type'}[0] if($cmd_args{'system_type'}[0] ne "");
	$host_v{$curr_host}{'detect_method'} = $cmd_args{'detect_method'}[0] if($cmd_args{'detect_method'}[0] ne "");
	$host_v{$curr_host}{'mx'} = $cmd_args{'mx'}[0] if($cmd_args{'mx'}[0] ne "");
	$host_v{$curr_host}{'back_mx'} = $cmd_args{'back_mx'}[0] if($cmd_args{'mx'}[0] ne "nochg");
	$host_v{$curr_host}{'offline'} = $cmd_args{'offline'}[0] if($cmd_args{'mx'}[0] ne "no");

	my $res = update();

	exit $res;

}
elsif ( $cmd_args{'update_host'}[0] eq "" &&
    $cmd_args{'update_ip'}[0] eq "" &&
    $cmd_args{'username'}[0] eq "" && 
    $cmd_args{'password'}[0] eq "" &&
    $cmd_args{'offline'}[0] == 0 &&
    $cmd_args{'back_mx'}[0] eq "" &&
    $cmd_args{'mx'}[0] eq "" &&
    $cmd_args{'detect_method'}[0] eq "" &&
    $cmd_args{'iface'}[0] eq "" )
{
} else
{
	# We are running in manual mode ;)
	$host_v{'manual'} = {};
	foreach(keys %default) {
		$host_v{'manual'}{$_} = $default{$_}; 
	}
	$host_v{'manual'}{'update_host'} = $cmd_args{'update_host'}[0] if($cmd_args{'update_host'}[0] ne "");
	$host_v{'manual'}{'update_ip'} = $cmd_args{'update_ip'}[0] if($cmd_args{'update_ip'}[0] ne "");
	$host_v{'manual'}{'iface'} = $cmd_args{'iface'}[0] if($cmd_args{'iface'}[0] ne "");
	$host_v{'manual'}{'username'} = $cmd_args{'username'}[0] if($cmd_args{'username'}[0] ne "");
	$host_v{'manual'}{'password'} = $cmd_args{'password'}[0] if($cmd_args{'password'}[0] ne "");
	$host_v{'manual'}{'detect_method'} = "manual";
	$host_v{'manual'}{'detect_method'} = $cmd_args{'detect_method'}[0] if($cmd_args{'detect_method'}[0] ne "");
	$host_v{'manual'}{'system_type'} = $cmd_args{'system_type'}[0] if($cmd_args{'system_type'}[0] ne "");
	$host_v{'manual'}{'mx'} = $cmd_args{'mx'}[0] if($cmd_args{'mx'}[0] ne "");
	$host_v{'manual'}{'back_mx'} = $cmd_args{'back_mx'}[0] if($cmd_args{'mx'}[0] ne "nochg");
	$host_v{'manual'}{'offline'} = $cmd_args{'offline'}[0] if($cmd_args{'mx'}[0] ne "no");

	$curr_host = "manual";
	my $res = update();

	exit $res;
}

	


if($daemon_v{'running'}) {
	
	get_config();

	my $interval;
	$interval = $daemon_v{'interval'} or $interval = 600;

	
	FORK: {
		if(my $pid = fork) {
			output("Starting Addns.pl ($pid) - $interval second interval",1,3);
			exit;
		} elsif (defined $pid) {
	
 			open(PIDFILE, ">" . $files{'pid'} ) || output("Cannot write to $files{'pid'}",1,1);
			print PIDFILE "$$";
			close(PIDFILE);
		
			while(1) {
				get_config();
				run_updates();
				sleep($interval)
			}
		} elsif ($! =~ /No more process/) {
			sleep 5;
			redo FORK;
		} else {
			# Fork Error
			die 'Cannot fork: $!\n';
		}
	}

} else {
	get_config();
	run_updates();

}

