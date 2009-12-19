#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );

my %uploadsettings;

&showhttpheaders();

&getcgihash(\%uploadsettings);

my $errormessage = '';
my $extramessage;

if ($uploadsettings{'ACTION'} eq $tr{'upload'})
{
	if (length($uploadsettings{'FH'}) > 1)
	{
		open(FILE, ">${swroot}/adsl/mgmt.o") or $errormessage = $tr{'could not create file'};
		flock FILE, 2;
		print FILE $uploadsettings{'FH'};
		close (FILE);
		$extramessage = $tr{'upload successful'};
	}
	undef $uploadsettings{'FH'};

	my $success = message('alcateladslfw');
		
	if (not defined $success) {
		$errormessage = $tr{'smoothd failure'}; }	
}

&openpage($tr{'usb adsl setup'}, 1, '', 'maintenance');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

&openbox($tr{'mgmt upload'});
print <<END
$tr{'usb adsl help'}
<P>
<TABLE>
<TR>
<FORM METHOD='post' ENCTYPE='multipart/form-data'>
<TD ALIGN='right' CLASS='base'>$tr{'upload filec'}</TD>
<TD><INPUT TYPE="file" NAME="FH"> <INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'upload'}'></TD>
END
;
print <<END
</FORM>
</TR>
</TABLE>
END
;

print "<FONT SIZE='5' CLASS='base'><DIV ALIGN='center'>$extramessage</DIV></FONT>\n";

&closebox();

&alertbox('add','add');

&closebigbox();

&closepage();

