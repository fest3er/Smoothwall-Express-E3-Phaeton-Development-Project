#!/usr/bin/perl

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %adslsettings;

&readhash("${swroot}/adsl/settings", \%adslsettings);

open(IN, '/usr/lib/smoothwall/cnxadsl.conf.in');
open(OUT, '>/usr/etc/cnxadsl/cnxadsl.conf');

while (<IN>) { print OUT $_; }

close (IN);

print OUT <<END
ATM_VPI		$adslsettings{'VPI'}
ATM_VCI		$adslsettings{'VCI'}
END
;

close (OUT);
