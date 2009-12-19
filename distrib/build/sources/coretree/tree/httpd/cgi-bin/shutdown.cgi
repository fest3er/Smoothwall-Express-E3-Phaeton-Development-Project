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

my %cgiparams;
my $death = 0;
my $rebirth = 0;

&showhttpheaders();

$cgiparams{'ACTION'} = '';
&getcgihash(\%cgiparams);

if ($cgiparams{'ACTION'} eq $tr{'shutdown'})
{
	$death = 1;
	
	&log($tr{'shutting down smoothwall'});
	
	my $success = message('systemshutdown');
	
	if (not defined $success) {
		$errormessage = $tr{'smoothd failure'}; }	
}
elsif ($cgiparams{'ACTION'} eq $tr{'reboot'})
{
	$rebirth = 1;
	
	&log($tr{'rebooting smoothwall'});
	
	my $success = message('systemrestart');
	
	if (not defined $success) {
		$errormessage = $tr{'smoothd failure'}; }
}
	
if ($death == 0 && $rebirth == 0)
{
	&openpage($tr{'shutdown control'}, 1, '', 'maintenance');

	&openbigbox('100%', 'LEFT');

	&alertbox($errormessage);

	print "<FORM METHOD='POST'>\n";

	&openbox($tr{'shutdown2'});
	print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'reboot'}'
 onClick="if(confirm('Are you sure you want to reboot this SmoothWall?')) {return true;} return false;"
></TD>
	<TD ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'shutdown'}'
 onClick="if(confirm('Are you sure you want to shutdown this SmoothWall?')) {return true;} return false;"
></TD>
</TR>
</TABLE>
END
	;
	&closebox();

	print "</FORM>\n";
}
else
{
	my ($message,$title);
	if ($death)
	{
		$title = $tr{'shutting down'};
		$message = $tr{'smoothwall is shutting down'};
	}
	else
	{
		$title = $tr{'rebooting'};
		$message = $tr{'smoothwall is rebooting'};
	}
	&openpage($title, 1, '', 'shutdown');

	&openbigbox('100%', 'CENTER');
	print <<END
<DIV ALIGN='CENTER'>
<TABLE BGCOLOR='#ffffff'>
<TR><TD ALIGN='CENTER'>
<A HREF='/' BORDER='0'><IMG SRC='/ui/img/smoothwall_big.gif'></A><BR><BR>
END
	;

	&alertbox($message);

	print <<END
</TD></TR>
</TABLE>
</DIV>
END
	;
}

&alertbox('add', 'add');

&closebigbox();

&closepage();
