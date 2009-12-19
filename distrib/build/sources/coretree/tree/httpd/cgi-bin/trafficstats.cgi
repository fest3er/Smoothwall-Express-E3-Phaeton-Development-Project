#!/usr/bin/perl
my $ADDRS_TO_SHOW = 5;
use lib "/usr/lib/smoothwall";
# use header qw(:standard);
print "Pragma: no-cache\n";
print "Cache-control: no-cache\n";
print "Connection: close\n";
print "content-type: text/html\n\n";
my @out = ();
open INPUT, "</var/log/quicktrafficstats";
my (%incaddr,%outaddr);
while ( my $line = <INPUT> ){
	next if ( not $line =~ /^cur_/ );
	my ( $rule, $interface, $value ) = ( $line =~ /cur_(inc|out)_rate_([^=]+)=([\d\.]+)$/i );
	next if $value == 0;
	if($interface =~ /^\d+\.\d+\.\d+\.\d+/) {
		if($rule eq 'out') {
			$outaddr{$interface} = [$value,$line];
		}
		if($rule eq 'inc') {
			$incaddr{$interface} = [$value,$line];
		}
	
	}
	else {
		push @out, $line;
	}
}
my @biggest_users = sort { $incaddr{$b}->[0] cmp $incaddr{$a}->[0]; } keys %incaddr;
my $num_users = scalar(@biggest_users);
if($num_users > $ADDRS_TO_SHOW) {
	splice(@biggest_users,$ADDRS_TO_SHOW);
	warn "Ignoring " . ($num_users - $ADDRS_TO_SHOW);
}
print @out;
for(@biggest_users) {
	print $incaddr{$_}->[1], $outaddr{$_}->[1];
}
print "\n";
close INPUT;

