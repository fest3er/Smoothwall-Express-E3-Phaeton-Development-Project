#!/usr/bin/perl

$swroot = '/var/smoothwall';

unless (-e "${swroot}/red/stayup") { exit; }

if (-e "${swroot}/red/active")
{
	&resetcount();
	exit;
}
else
{
	$count = &inccount();
	system('/usr/bin/logger', '-t', 'smoothwall', "Connection not up after $count minutes.");
	if ($count >= 5)
	{
		system('/usr/bin/logger', '-t', 'smoothwall', "Connection still not up; rebooting.");
		&resetcount();
		system('/usr/bin/smoothcom', 'systemrestart');
	}
}

exit;

sub inccount
{
 	my $current;

	open(FILE, "+<${swroot}/ppp/upcount") or die "Couldn't open count.";
	flock FILE, 2;
	$current = <FILE>; chomp $current;
	seek FILE, 0, 0;
	$current++;
	truncate FILE, 0;
	print FILE "$current\n";
	close(FILE);
	return $current;
}

sub resetcount
{
	open(FILE, ">${swroot}/ppp/upcount") or die "Couldn't open count.";
	flock FILE, 2;
	print FILE "0\n";
	close(FILE);
}
