#!/usr/bin/perl
use lib "/usr/lib/smoothwall";
use header qw(:standard);
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
open (OUT, ">${swroot}/smoothinfo/etc/chains");
print OUT @names;
