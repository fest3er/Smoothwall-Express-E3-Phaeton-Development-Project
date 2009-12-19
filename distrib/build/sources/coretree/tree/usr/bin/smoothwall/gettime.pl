#!/usr/bin/perl

# (c) SmoothWall Ltd, 2002

use lib "/usr/lib/smoothwall";
use header qw( :standard );

my %timesettings;
my $count;

&readhash("${swroot}/time/settings", \%timesettings);

if ($timesettings{'ENABLED'} ne 'on') {
	exit 0; }

if ($ARGV[0] eq 'FORCE')
{
	&ntpgettime();
	&resetcount();
}
else
{
	$count = &inccount();

	if ($count >= $timesettings{'NTP_INTERVAL'})
	{
		&ntpgettime();
		&resetcount();
	}
}

exit 0;

sub ntpgettime
{
	my @allservers; my $totalallservers;
	my @servers;
	my @command;
	my $count;

	unless (-e "${swroot}/red/active") {
		return; }

	if ($timesettings{'NTP_SERVER_TYPE'} eq 'RANDOM')
	{
		open (FILE, "${swroot}/time/timeservers") or die "Unable to open timeservers.";
		while (<FILE>)
		{
		        chomp;
		        @temp = split(/\|/);
			push(@allservers, $temp[2]);
		}

		$totalallservers = scalar @allservers;

		for ($count = 0; $count < 5; $count++)
		{
			$i = int (rand $totalallservers);
			push(@servers, $allservers[$i]);
		}
	}
	elsif ($timesettings{'NTP_SERVER_TYPE'} eq 'SELECTED') {
		push(@servers, $timesettings{'NTP_SERVER_SELECTED'}); }
	else {
		push(@servers, $timesettings{'NTP_SERVER_USERDEFINED'}); }

	@command = ('/usr/sbin/ntpdate', '-su', @servers);

	if (system(@command) == 0)
	{
		&log("System clock successfully updated; using server(s) @servers.");
		if ($timesettings{'NTP_RTC'} eq 'on') {
			system('/sbin/hwclock', '--systohc', '--utc'); }
	}
	else {
		&log("Unable to update system clock; using server(s) @servers"); }
}

sub inccount
{
	my $current;

	open(FILE, "+<${swroot}/time/timecount") or die "Couldn't load ntpcount.";
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
	open(FILE, ">${swroot}/time/timecount") or die "Couldn't load ntpcount.";
	flock FILE, 2;
	print FILE "0\n";
	close(FILE);
}
