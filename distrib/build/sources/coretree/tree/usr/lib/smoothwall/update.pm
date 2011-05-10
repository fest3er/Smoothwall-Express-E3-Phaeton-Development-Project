# SmoothWall Express 3.0 Update Mechanism 
#
# Common functions and procedures for updating a SmoothWall system
#
# (c) 2004-2006 SmoothWall Ltd
# 
# Original Author: Darren Taylor
# Based upon updateslists.pl by Lawrence Manning and others
# 
# This Code is distributed under the terms of the GPL V2.0

package update;
require Exporter;

use lib "/usr/lib/smoothwall/";
use header qw(:standard);

@ISA = qw(Exporter);

# Define the export lists .

@EXPORT		= qw();
@EXPORT_OK	= qw(downloadlist);
%EXPORT_TAGS	= ( 
			standard => [@EXPORT_OK]
		  );

# Download function, retrieves the Updates list from SmoothWall's website and stores 
# it locally.

sub downloadlist
{
	my %proxy;
	&readhash("${swroot}/main/proxy", \%proxy);

	my $host; my $port;
        unless ($proxy{'SERVER'})
        {
                $host = 'roadster.agcl.us';
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
		print STDERR "unable to connect\n";
		$errormessage = $tr{'could not connect to Roadster updates'};
		return 0;
	}
	print $sock "GET http://$host/updates/$major/info HTTP/1.1\r\nHost: $host\r\nConnection: close\r\n\r\n";
	my $ret = '';
	while (<$sock>) {
		$ret .= $_; }
	close($sock);
	return $ret;
}

1;
