#!/usr/bin/perl

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my @current;
my %adslsettings;
my $vpi; my $vci;

&readhash("${swroot}/adsl/settings", \%adslsettings);

open(FILE, '>/etc/eagle-usb/eagle-usb.conf');
$vpi = sprintf("%08X", $adslsettings{'VPI'});
$vci = sprintf("%08X", $adslsettings{'VCI'});

print FILE <<END
<eaglectrl>
VPI=$vpi
VCI=$vci
Encapsulation=00000005
Linetype=0000000A
RatePollFreq=00000009
</eaglectrl>
END
;

close(FILE);
