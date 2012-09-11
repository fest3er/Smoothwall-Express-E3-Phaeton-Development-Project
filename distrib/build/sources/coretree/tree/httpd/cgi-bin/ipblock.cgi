#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL

#
# (c) SmoothWall Ltd 2003

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use smoothd qw( message );
use smoothtype qw( :standard );

my (%cgiparams,%checked,%selected);
my $filename = "${swroot}/ipblock/config";
my @vars;
my $var, $addr;
my $needrestart = 0;

&showhttpheaders();

$cgiparams{'ENABLED'} = 'off';
$cgiparams{'LOG'} = 'off';

$cgiparams{'COLUMN'} = 1;
$cgiparams{'ORDER'} = $tr{'log ascending'};

&getcgihash(\%cgiparams);




if ($ENV{'QUERY_STRING'} && $cgiparams{'ACTION'} eq '')
{
	@vars = split(/\&/, $ENV{'QUERY_STRING'});
	foreach $_ (@vars)
	{
		($var, $addr) = split(/\=/);
		if ($var eq 'ip')
		{
			if (&validipormask($addr))
			{
				open(FILE,">>$filename") or die 'Unable to open config file.';
				flock FILE, 2;
				print FILE "$addr,off,DROP,on\n";
				close(FILE);
				$needrestart = 1;
			}
		}
	}
	if ($needrestart)
	{
		my $success = message('setipblock');
		
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}


}

if ($ENV{'QUERY_STRING'} && ( not defined $cgiparams{'ACTION'} or $cgiparams{'ACTION'} eq "" ))
{
        my @temp = split(',',$ENV{'QUERY_STRING'});
        $cgiparams{'ORDER'}  = $temp[1] if ( defined $temp[ 1 ] and $temp[ 1 ] ne "" );
        $cgiparams{'COLUMN'} = $temp[0] if ( defined $temp[ 0 ] and $temp[ 0 ] ne "" );
}

my $errormessage = '';

if ($cgiparams{'ACTION'} eq $tr{'add'})
{
	unless(&validipormask($cgiparams{'SRC_IP'})) { $errormessage = $tr{'source ip bad'}; }
	unless ( &validcomment( $cgiparams{'COMMENT'} ) ){ $errormessage = $tr{'invalid comment'}; }	

	open(FILE, $filename) or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);
	unless ($errormessage)
	{
		open(FILE,">>$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		print FILE "$cgiparams{'SRC_IP'},$cgiparams{'LOG'},$cgiparams{'TARGET'},$cgiparams{'ENABLED'},$cgiparams{'COMMENT'}\n";
		close(FILE);

		my $column = $cgiparams{ 'COLUMN' };
		my $order  = $cgiparams{ 'ORDER' };

		undef %cgiparams;

		$cgiparams{ 'COLUMN' } = $column;
		$cgiparams{ 'ORDER' } = $order;

		&log($tr{'ip block rule added'});

		my $success = message('setipblock');
		
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}
if ($cgiparams{'ACTION'} eq $tr{'remove'} || $cgiparams{'ACTION'} eq $tr{'edit'})
{
	open(FILE, "$filename") or die 'Unable to open config file.';
	my @current = <FILE>;
	close(FILE);

	my $count = 0;
	my $id = 0;
	my $line;
	foreach $line (@current)
	{
		$id++;
		if ($cgiparams{$id} eq "on") {
			$count++; }
	}
	if ($count == 0) {
		$errormessage = $tr{'nothing selected'}; }
	if ($count > 1 && $cgiparams{'ACTION'} eq $tr{'edit'}) {
		$errormessage = $tr{'you can only select one item to edit'}; }
	unless ($errormessage)
	{
		open(FILE, ">$filename") or die 'Unable to open config file.';
		flock FILE, 2;
		$id = 0;
		foreach $line (@current)
		{
			$id++;
			unless ($cgiparams{$id} eq "on") {
				print FILE "$line"; }
			elsif ($cgiparams{'ACTION'} eq $tr{'edit'})
			{
				chomp($line);
				my @temp = split(/\,/,$line);
				$cgiparams{'SRC_IP'} = $temp[0];
				$cgiparams{'LOG'} = $temp[1];
				$cgiparams{'TARGET'} = $temp[2];
				$cgiparams{'ENABLED'} = $temp[3];
				$cgiparams{'COMMENT'} = $temp[4];
			}
		}
		close(FILE);
		&log($tr{'ip block rule removed'});

		my $success = message('setipblock');
		
		if (not defined $success) {
			$errormessage = $tr{'smoothd failure'}; }
	}
}
if ($cgiparams{'ACTION'} eq '')
{
	$cgiparams{'TARGET'} = 'DROP';
	$cgiparams{'ENABLED'} = 'on';
}

$checked{'ENABLED'}{'off'} = '';
$checked{'ENABLED'}{'on'} = '';  
$checked{'ENABLED'}{$cgiparams{'ENABLED'}} = 'CHECKED';

$checked{'LOG'}{'off'} = '';
$checked{'LOG'}{'on'} = '';  
$checked{'LOG'}{$cgiparams{'LOG'}} = 'CHECKED';

$checked{'TARGET'}{'DROP'} = '';
$checked{'TARGET'}{'REJECT'} = '';
$checked{'TARGET'}{$cgiparams{'TARGET'}} = 'CHECKED';

&openpage($tr{'ip block configuration'}, 1, '', 'networking');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

print "<FORM METHOD='POST'>\n";

&openbox($tr{'add a new rule'});
print <<END
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='20%' CLASS='base'><FONT COLOR='$colourred'>$tr{'source ip or networkc'}</FONT></TD>
<TD WIDTH='20%'><INPUT TYPE='TEXT' NAME='SRC_IP' VALUE='$cgiparams{'SRC_IP'}' SIZE='15' id='src_ip' @{[jsvalidipormask('src_ip')]}></TD>
<TD WIDTH='20%' CLASS='base' style='text-align:left'>
<INPUT TYPE='radio' NAME='TARGET' VALUE='DROP' $checked{'TARGET'}{'DROP'}>$tr{'drop packet'}
</TD>
<TD WIDTH='20%' CLASS='base' style='text-align:left'>
<INPUT TYPE='radio' NAME='TARGET' VALUE='REJECT' $checked{'TARGET'}{'REJECT'}>$tr{'reject packet'}
</TD>
<TD WIDTH='20%' CLASS='base' style='text-align:left'>
$tr{'logc'}<INPUT TYPE='checkbox' NAME='LOG' $checked{'LOG'}{'on'}>
</TD>
</TR>
<tr>
	<td class='base'>$tr{'commentc'}</td>
	<td colspan='3'><input type='text' style='width: 80%;' name='COMMENT' value='$cgiparams{'COMMENT'}' id='comment' @{[jsvalidcomment('comment')]}  ></td>
</tr>
</TABLE>
<TABLE WIDTH='100%'>
<TR>
<TD WIDTH='50%' CLASS='base' ALIGN='CENTER'>$tr{'enabled'}<INPUT TYPE='CHECKBOX' NAME='ENABLED' $checked{'ENABLED'}{'on'}></TD>
<TD WIDTH='50%' ALIGN='CENTER'><INPUT TYPE='SUBMIT' NAME='ACTION' VALUE='$tr{'add'}'></TD>
</TR>
</TABLE>
END
;
&closebox();

&openbox($tr{'current rules'});

my %render_settings = (
			'url'     => "/cgi-bin/ipblock.cgi?[%COL%],[%ORD%]",
			'columns' => [ 
				{ 
					column => '1',
					title  => "$tr{'source ip'}",
					size   => 30,
					sort   => \&ipcompare,
					tr     => {
						'0.0.0.0/0' => 'N/A',
					},					
					valign => 'top',
					maxrowspan => 2,
				},
				{
					column => '3',
					title  => "$tr{'action'}", 
					size   => 20,
					tr     => {
						'REJECT' => 'REJECT',
						'DROP'   => 'DROP',
						'RETURN' => 'EXCEPTION',
					},
				},
				{
					column => '2',
					title => "$tr{'log'}",
					size   => 20,
					tr     => 'onoff',
					align  => 'center',
				},
				{
					column => '4',
					title  => "$tr{'enabledtitle'}",
					size   => 15,
					tr     => 'onoff',
					align  => 'center',
				},
				{
					title  => "$tr{'mark'}", 
					size   => 15,
					mark   => ' ',
				},
				{ 
					column => '5',
					title => "$tr{'comment'}",
					break => 'line',
					spanadj => -1,
				}
			]
			);

&dispaliastab( $filename, \%render_settings, $cgiparams{'ORDER'}, $cgiparams{'COLUMN'} );

print <<END
<table class='blank'>
<tr>
<td style='text-align: center;'><input type='submit' name='ACTION' value='$tr{'remove'}'></td>
<td style='text-align: center;'><input type='submit' name='ACTION' value='$tr{'edit'}'></td>
</tr>
</table>
END
;

&alertbox('add', 'add');

&closebox();

&closebigbox();

&closepage($errormessage);

