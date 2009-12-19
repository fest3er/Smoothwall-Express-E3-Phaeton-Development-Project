#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) SmoothWall Ltd 2002, 2003

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %netsettings;

&readhash("${swroot}/ethernet/settings", \%netsettings);

open (FILE, ">${swroot}/time/ntpd.conf");
print FILE <<END
listen on $netsettings{'GREEN_ADDRESS'}
END
;
if ($netsettings{'PURPLE_DEV'})
{
	print FILE <<END
listen on $netsettings{'PURPLE_ADDRESS'}
END
	;
}
print FILE <<END
listen on 127.0.0.1
server 127.0.0.1
END
;
close (FILE);
