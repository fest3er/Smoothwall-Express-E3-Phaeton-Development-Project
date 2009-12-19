#!/usr/bin/perl

use lib "/usr/lib/smoothwall";
use header qw(:standard );

use IO::Socket;

my %proxy;

&readhash("${swroot}/main/proxy", \%proxy);

my $host; my $port;
unless ($proxy{'SERVER'})
{
	$host = 'x3.smoothwall.org';
        $port = 80;
} else {
	$host = $proxy{'SERVER'};
	$port = $proxy{'PORT'};
}

my $sock;

unless ($sock = new IO::Socket::INET (PeerAddr => $host, PeerPort => $port, Proto => 'tcp', Timeout => 5)) {
	print STDERR "unable to connect\n";
	$errormessage = $tr{'could not connect to smoothwall org'};
	exit 0;
}

print $sock "GET http://x3.smoothwall.org/cgi-bin/banners.cgi?version=$version HTTP/1.1\r\nHost: x3.smoothwall.org\r\nConnection: close\r\n\r\n";

my $return = '';

while (<$sock>) {
	$return .= $_; 
}
close($sock);

if ($return =~ m/^HTTP\/\d+\.\d+ 200/) {
	unless(open(LIST, ">${swroot}/banners/available")) {
		die "Could not open available lists database."; 
	}

	flock LIST, 2;
	@this = split(/----START LIST----\n/,$return);
	print LIST $this[1];
	close(LIST);
} else {
	die "Could not download banner list. $return";
}


unless(open(LIST, "<${swroot}/banners/available")) {
	die "Could not open available lists database."; 
}

my %seen = ( 'frontpage' => 'true', 'frontpage.x3' => 'true' );

my @proxy_opt = ();
if ($proxy{'SERVER'}) {
	my $server = $proxy{'SERVER'};
	my $port = $proxy{'PORT'} || 80;
	@proxy_opt = ("-e", "http_proxy = http://$server:$port/");
}

while ( my $input = <LIST> ){
	my ( $url, $md5, $link, $alt ) = ( $input =~/([^\|]*)\|([^\|]*)\|([^\|]*)\|(.*)/ );
	
	$seen{$md5} = 'true';

	if ( !-e "/httpd/html/ui/img/frontpage/$md5.jpg" ){
		# we need to download this file 
		my @commands = ( "/usr/bin/wget", @proxy_opt, "-O", "/httpd/html/ui/img/frontpage/$md5.jpg", "$url" );
		my ( $status, $pid_out );
		open(PIPE, '-|') || exec( @commands );
	        while (<PIPE>) { 
			$status .= $_; 
		}
		close(PIPE);
	}
}


foreach my $filename ( glob "/httpd/html/ui/img/frontpage/*" ){
	my ($image) = ( $filename =~ /\/httpd\/html\/ui\/img\/frontpage\/(.*)\.jpg/i );
	if ( not defined $seen{$image} ){
		unlink "/httpd/html/ui/img/frontpage/$image.jpg";
	}
}
