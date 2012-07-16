#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) SmoothWall Ltd, 2002-2003

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );

my (%advnetsettings,%checked);

&showhttpheaders();

$advnetsettings{'ENABLE_NOPING'} = 'off';
$advnetsettings{'ENABLE_COOKIES'} = 'off';
$advnetsettings{'ENABLE_NOIGMP'} = 'off';
$advnetsettings{'ENABLE_NOMULTICAST'} = 'off';
$advnetsettings{'ENABLE_UPNP'} = 'off';
$advnetsettings{'BAD_TRAFFIC'} = 'REJECT';
$advnetsettings{'ACTION'} = '';
&getcgihash(\%advnetsettings);

$errormessage = '';
if ($advnetsettings{'ACTION'} eq $tr{'save'})
{
	&writehash("${swroot}/advnet/settings", \%advnetsettings);
	
	my $success = message('setadvnet');
	
	if (not defined $success) {
		$errormessage .= $tr{'smoothd failure'}."<br />"; }	
	
	my $success = message('upnpdrestart');
                
        if (not defined $success) {
                $errormessage .= $tr{'smoothd failure'}."<br />"; }

        $success = message('setoutgoing');
                
        if (not defined $success) {
                $errormessage .= $tr{'smoothd failure'}."<br />"; }
}

&readhash("${swroot}/advnet/settings", \%advnetsettings);

$checked{'ENABLE_NOPING'}{'off'} = '';
$checked{'ENABLE_NOPING'}{'on'} = '';
$checked{'ENABLE_NOPING'}{$advnetsettings{'ENABLE_NOPING'}} = 'CHECKED';

$checked{'ENABLE_COOKIES'}{'off'} = '';
$checked{'ENABLE_COOKIES'}{'on'} = '';
$checked{'ENABLE_COOKIES'}{$advnetsettings{'ENABLE_COOKIES'}} = 'CHECKED';

$checked{'ENABLE_NOIGMP'}{'off'} = '';
$checked{'ENABLE_NOIGMP'}{'on'} = '';
$checked{'ENABLE_NOIGMP'}{$advnetsettings{'ENABLE_NOIGMP'}} = 'CHECKED';

$checked{'ENABLE_NOMULTICAST'}{'off'} = '';
$checked{'ENABLE_NOMULTICAST'}{'on'} = '';
$checked{'ENABLE_NOMULTICAST'}{$advnetsettings{'ENABLE_NOMULTICAST'}} = 'CHECKED';

$checked{'ENABLE_UPNP'}{'off'} = '';
$checked{'ENABLE_UPNP'}{'on'} = '';
$checked{'ENABLE_UPNP'}{$advnetsettings{'ENABLE_UPNP'}} = 'CHECKED';

$selected{'BAD_TRAFFIC'}{'REJECT'} = '';
$selected{'BAD_TRAFFIC'}{'DROP'} = '';
$selected{'BAD_TRAFFIC'}{$advnetsettings{'BAD_TRAFFIC'}} = 'SELECTED';

&openpage($tr{'advanced networking features'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'advanced networking featuresc'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'block icmp ping'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE_NOPING' $checked{'ENABLE_NOPING'}{'on'}></TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'enable syn cookies'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE_COOKIES' $checked{'ENABLE_COOKIES'}{'on'}></TD>
</TR>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'block and ignore igmp packets'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE_NOIGMP' $checked{'ENABLE_NOIGMP'}{'on'}></TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'block and ignore multicast traffic'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE_NOMULTICAST' $checked{'ENABLE_NOMULTICAST'}{'on'}></TD>
</TR>
<TR>
	<TD WIDTH='25%' CLASS='base'>$tr{'upnp support'}</TD>
	<TD WIDTH='25%'><INPUT TYPE='checkbox' NAME='ENABLE_UPNP' $checked{'ENABLE_UPNP'}{'on'}></TD>
	<TD WIDTH='25%' CLASS='base'>$tr{'action to perform on bad external traffic'}</TD>
	<TD WIDTH='25%'>
	<SELECT NAME='BAD_TRAFFIC'>
	<OPTION VALUE='REJECT' $selected{'BAD_TRAFFIC'}{'REJECT'}>$tr{'reject'}
	<OPTION VALUE='DROP' $selected{'BAD_TRAFFIC'}{'DROP'}>$tr{'drop'}
	</SELECT>
	</TD>
</TR>
</TABLE>
END
;
&closebox();

print <<END
<DIV ALIGN='CENTER'>
<TABLE WIDTH='60%'>
<TR>
	<TD ALIGN='CENTER'><INPUT TYPE='submit' NAME='ACTION' VALUE='$tr{'save'}'></TD> 
</TR>
</TABLE>
</DIV>
END
;

print "</FORM>\n";

&alertbox('add', 'add');

&closebigbox();

&closepage($errormessage);
