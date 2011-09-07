#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

# 9/1/2011 Neal P. Murphy; Tweaked to handle NAT traversal and sites behind NAT 

use lib "/usr/lib/smoothwall";
use header qw( :standard );

# Get the RED address, wherever.
my %netsettings;
&readhash ("${swroot}/ethernet/settings", \%netsettings);
if ($netsettings{'RED_ADDRESS'} eq '0.0.0.0')
{
  open(RADDR, "<${swroot}/red/local-ipaddress") or die 'Unable to open get RED IP address.';
  $netsettings{'RED_ADDRESS'} = <RADDR>;
  chomp $netsettings{'RED_ADDRESS'};
  close RADDR;
}

my $filename = "${swroot}/vpn/config";

my %conn;

open(FILE, "$filename") or die 'Unable to open config file.';
my @current = <FILE>;
close(FILE);

open(CONF, ">${swroot}/vpn/ipsec.conf") or die 'unable to open conf file';
open(SECRETS, ">${swroot}/vpn/ipsec.secrets") or die 'unabe to open secrets file.';
flock CONF, 2;
flock SECRETS, 2;
print CONF "version 2\n";
print CONF "\n";
print CONF "config setup\n";
print CONF "\tinterfaces=%defaultroute\n";
print CONF "\tklipsdebug=none\n";
print CONF "\tplutodebug=none\n";
print CONF "\tplutowait=no\n";
print CONF "\tuniqueids=yes\n";
print CONF "\tprotostack=klips\n";
print CONF "\tnat_traversal=yes\n";
print CONF "\n";

foreach my $conn ('clear', 'clear-or-private', 'private-or-clear',
	'private', 'block', 'packetdefault')
{
	print CONF "conn $conn\n";
	print CONF "\tauto=ignore\n";
	print CONF "\n";
}

my $id = 0;
my $line;
foreach $line (@current)   
{
	chomp($line);
	my @temp = split(/\,/,$line);
	if (!defined $temp[9]) { $temp[9] = '' };
	if (!defined $temp[10]) { $temp[10] = '' };
	$conn{'NAME'} = $temp[0];
	$conn{'LEFT'} = $temp[1];
	$conn{'LEFT_SUBNET'} = $temp[2];   
	$conn{'RIGHT'} = $temp[3];      
	$conn{'RIGHT_SUBNET'} = $temp[4];   
	$conn{'SECRET'} = $temp[5];
        $conn{'ENABLED'} = $temp[6];
        $conn{'COMPRESSION'} = $temp[7];                
        $conn{'COMMENT'} = $temp[8];
        $conn{'LEFT_PRIVATE'} = $temp[9];   
        $conn{'RIGHT_PRIVATE'} = $temp[10];
        
        my $connname = $conn{'NAME'};
        $connname =~ s/\s+//g;
	
	if ($conn{'ENABLED'} eq 'on')
	{
		print SECRETS "$conn{'LEFT'} $conn{'RIGHT'} : PSK \"$conn{'SECRET'}\"\n";

		print CONF "conn $connname\n";
		print CONF "\tike=3des-md5\n";
                print CONF "\tesp=3des-md5\n";
                print CONF "\tauthby=secret\n";
                print CONF "\tkeyingtries=0\n";
                print CONF "\tleft=$conn{'LEFT'}\n";
                if ($conn{'LEFT_PRIVATE'} ne '')
                {
                  print CONF "\tleftsourceip=$conn{'LEFT_PRIVATE'}\n";
                }
                print CONF "\tleftsubnet=$conn{'LEFT_SUBNET'}\n";
                print CONF "\tleftnexthop=%defaultroute\n";
                print CONF "\tright=$conn{'RIGHT'}\n";
                if ($conn{'RIGHT_PRIVATE'} ne '')
		{
                  print CONF "\trightsourceip=$conn{'RIGHT_PRIVATE'}\n";
                }
                print CONF "\trightsubnet=$conn{'RIGHT_SUBNET'}\n";
                print CONF "\trightnexthop=%defaultroute\n";
                if ($conn{'COMPRESSION'} eq 'on') {
                        print CONF "\tcompress=yes\n"; }
                else {
                        print CONF "\tcompress=no\n"; }

		# Shorthand
		my $left_private = $conn{'LEFT_PRIVATE'};
		my $right_private = $conn{'RIGHT_PRIVATE'};
		my $red_address = $netsettings{'RED_ADDRESS'};

		# if neither is NATted or the remote is not NATted, start
		# (auto=start). Otherwise, just authorize (auto=add).
		if ($left_private eq '' && $right_private eq '')
		{
		  # Neither private address is set, so no NAT; autostart.
                  print CONF "\tauto=start\n";
	        }
		elsif (($conn{'LEFT'} eq '%any' && $left_private ne $red_address)
	               or
		       ($conn{'RIGHT'} eq '%any' && $right_private ne $red_address))
                {
		  # If the remote is NATted, wait for her to initiate;
		  # authorize only
                  print CONF "\tauto=add\n";
                }
                else
                {
		  # All other cases should be good to autostart.
                  print CONF "\tauto=start\n";
                }
                print CONF "\n";
        }
        $id++;
}

close(CONF);
close(SECRETS);

