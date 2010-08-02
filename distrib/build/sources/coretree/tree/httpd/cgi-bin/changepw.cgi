#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothtype qw( :standard );

my %cgiparams;

&showhttpheaders();

$cgiparams{'ACTION_ADMIN'} = '';
$cgiparams{'ACTION_DIAL'} = '';

&getcgihash(\%cgiparams);

my $errormessage = '';

if ($cgiparams{'ACTION_ADMIN'} eq $tr{'save'})
{
	my $password1 = $cgiparams{'ADMIN_PASSWORD1'};
	my $password2 = $cgiparams{'ADMIN_PASSWORD2'};	
	if ($password1 eq $password2)
	{
		if ($password1 =~ m/\s|\"/) {
			$errormessage = $tr{'password contains illegal characters'};
		}
		elsif (length($password1) >= 6)
		{
			system('/usr/apache2/sbin/htpasswd', '-m', '-b', "${swroot}/auth/users", 'admin', "${password1}");
			&log($tr{'admin user password has been changed'});
		}
		else {
			$errormessage = $tr{'passwords must be at least 6 characters in length'}; }
	}
	else {
		$errormessage = $tr{'passwords do not match'}; }
}

if ($cgiparams{'ACTION_DIAL'} eq $tr{'save'})
{
	my $password1 = $cgiparams{'DIAL_PASSWORD1'};
	my $password2 = $cgiparams{'DIAL_PASSWORD2'};	
	if ($password1 eq $password2)
	{
		if($password1 =~ m/\s|\"/) {
			$errormessage = $tr{'password contains illegal characters'}; }
		elsif (length($password1) >= 6)
		{
			system('/usr/apache2/sbin/htpasswd', '-m', '-b', "${swroot}/auth/users", 'dial', "${password1}"); 
			&log($tr{'dial user password has been changed'});
		}
		else {
			$errormessage = $tr{'passwords must be at least 6 characters in length'}; }
	}
	else {
		$errormessage = $tr{'passwords do not match'}; }
}

&openpage($tr{'change passwords'}, 1, '', 'maintenance');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'administrator user password'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='15%' CLASS='base'>$tr{'password'}</TD>
	<TD WIDTH='30%'><INPUT TYPE='password' NAME='ADMIN_PASSWORD1' id='admin_password1' @{[jsvalidpassword('admin_password1','admin_password2','^([^ \|]*)$')]}></TD>

	<TD WIDTH='15%' CLASS='base'>$tr{'again'}</TD>
	<TD WIDTH='30%'><INPUT TYPE='password' NAME='ADMIN_PASSWORD2' id='admin_password2' @{[jsvalidpassword('admin_password2','admin_password1','^([^ \|]*)$')]} ></TD>
	<TD WIDTH='10%'><INPUT TYPE='submit' NAME='ACTION_ADMIN' VALUE='$tr{'save'}'></TD>
</TR>
</TABLE>
END
;
&closebox();

&openbox($tr{'dial user password'});
print <<END
<TABLE WIDTH='100%'>
<TR>
	<TD WIDTH='15%' CLASS='base'>$tr{'password'}</TD>
	<TD WIDTH='30%'><INPUT TYPE='password' NAME='DIAL_PASSWORD1'id='dial_password1' @{[jsvalidpassword('dial_password1','dial_password2','^([^ \|]*)$')]} ></TD>
	<TD WIDTH='15%' CLASS='base'>$tr{'again'}</TD>
	<TD WIDTH='30%'><INPUT TYPE='password' NAME='DIAL_PASSWORD2' id='dial_password2' @{[jsvalidpassword('dial_password2','dial_password1','^([^ \|]*)$')]} ></TD>
	<TD WIDTH='10%'><INPUT TYPE='submit' NAME='ACTION_DIAL' VALUE='$tr{'save'}'></TD>
</TR>
</TABLE>
END
;
&closebox();

print "</FORM>\n";

&alertbox('add', 'add');

&closebigbox();

&closepage();
