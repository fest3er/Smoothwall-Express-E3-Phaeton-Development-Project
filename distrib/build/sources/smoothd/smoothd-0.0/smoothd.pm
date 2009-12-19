# Perl Module for communications with the SmoothD system
# (c) 2004 SmoothWall Ltd
# Original Author: D.K.Taylor

package smoothd;
require Exporter;
@ISA    = qw( Exporter );
@EXPORT = qw( message );


# include relevant modules

use strict;
use IO::Socket;

sub message
{
	my @commands = @_;
	
	# create a local socket
	socket( my $localsocket, PF_UNIX, SOCK_STREAM, 0 );
	my $cmds = join(' ', @commands);
	if ( not defined $localsocket ) {
		`/usr/bin/logger -t SmoothDClient "Unable to bind socket for communications with SmoothD for $cmds"`;
		return undef; 
	}

	# connect the socket to the communications socket
	
	connect( $localsocket, sockaddr_un( "/dev/smoothd" ));
	
	if ( not defined $localsocket ){
		`/usr/bin/logger -t SmoothDClient "Unable to connect socket for communications with SmoothD for $cmds"`;
		return undef; 
	}

	# set autoflush which allows for instant comms channels
	$localsocket->autoflush( 1 );

	my $command_string = join ' ', @commands;

	print $localsocket "$command_string\n";
	# read from the socket ( to get the response )

	my $line;
	my $timeout = 30;

	eval {
	    local $SIG{ALRM} = sub { die "alarm timeout"; };
	    local $SIG{__DIE__} = sub { alarm 0; die @_ };
	    alarm $timeout;
	    $line=<$localsocket>;
	    alarm 0;        # cancel the alarm
	};
	die $@ if $@ && $@ !~ /alarm timeout/;
	if ($@) {
		`/usr/bin/logger -t SmoothDClient "Unable to communicate with SmoothD for $cmds"`;
	    return 'TIMEOUT';
	}

	if ( not defined $line or $line =~ /Error:/ ){
		if ( not defined $line ){
			`/usr/bin/logger -t SmoothDClient "Unable to execute command, no response for $cmds"`;
		} else {
			`/usr/bin/logger -t SmoothDClient "Unable to execute command $line for $cmds"`;
		}
		return undef; 
	}

	if ( $line =~ /Attempt to invoke unregistered command/ ){
                return undef;
        }

	#remove the newline
	chomp $line;
	return $line;
}

1;
