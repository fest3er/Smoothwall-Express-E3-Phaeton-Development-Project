#! /usr/bin/perl

my $order = 0;
my $action = "REJECT";

# Is the door open or closed?
my $door = `grep OPENNESS ../main/settings`;
$door =~ s/.*=//;
if ($door eq "open")
{
  # Bien venu, wilcommen, hola, come in it's open; bounce miscreants out
  $action = "REJECT";
} else {
  # Katy barred the door! Admit only those recognized
  $action = "ACCEPT";
}

# Convert
while (<STDIN>) {
  chomp;
  $order++;
  my @frm = split('\+');  # crack time frame(s) off
  my @cfg = split(",", $frm[0]);   # crowbar other fields apart
  # The first 4 fields already exist
  for (my $i=0; $i<4; $i++)
  {
    print "$cfg[$i],";
  }
  # The next 5 don't exist r2.0.2->r2.0.3, but should thereafter
  for ($i=4; $i<10; $i++)
  {
    if (defined $cfg[$i])
    {
      # add the existing item
      if ($i == 7)
      {
        # But do partake in the revolution
        print "$order,"                      # New world order
      } else {
        print "$cfg[$i],";
      }
    } else {
      # add the new 'default' item, based on original actions
      print "TCP&UDP," if $i == 4;         # Original outgoing applied to both
      print "N/A," if $i == 5;             # Original had no address
      print "$action," if $i == 6;         # Action
      print "$order," if $i == 7;          # New world order
      print "off," if $i == 8 or $i == 9;  # Default is off
      # Leave time frame [10] blank
    }
  }
  if ($frm[1] ne "")
  {
    print "+$frm[1]";
  }
  # Finish the line
  print "\n";
}
