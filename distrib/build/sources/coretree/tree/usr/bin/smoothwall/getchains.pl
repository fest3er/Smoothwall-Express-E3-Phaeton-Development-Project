#!/usr/bin/perl
use lib "/usr/lib/smoothwall";
use header qw(:standard);

$SIdir="${swroot}/smoothinfo";

open (IN, "-|", '/usr/sbin/iptables', '-L', '-n');
my @iptables = <IN>;
foreach (@iptables)
	{
	if (/chain/i) {push (@chains, $_);}
	}
foreach (@chains)
	 {
	 @tmp = split /\s/,$_;
	 push (@names,"$tmp[1]\n");
	 }
open (OUT, ">${SIdir}/etc/chains");
print OUT @names;
