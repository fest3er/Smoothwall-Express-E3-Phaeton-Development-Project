#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team
#
# $Id: help.cgi 311 2009-07-04 12:35:17Z steve@domesticsecurity.com.au $
#

use lib "/usr/lib/smoothwall";
use header qw( :standard );

# load the glossary of terms.
# this is similar to the TR hash, but only required here, and therefore not
# included in it, (it could do with replacing anyhow)

require "/usr/lib/smoothwall/langs/glossary.base.pl";
require "/usr/lib/smoothwall/langs/glossary.${language}.pl";
# Add Mod Glossary
my @modgloss = `find /var/smoothwall/mods -regex '.*/usr/lib/smoothwall/langs/glossary.${language}.pl'`;
foreach my $file (sort @modgloss) {
        open(GLOSS, $file) or die($!);
        while(<GLOSS>) {
                chomp($_);
                next if /^$/;
                my @line = split(' => ', $_);
                $glossary{$line[0]} = $line[1];
        }
        close(GLOSS);
}

#my $needhelpwith = $ENV{'QUERY_STRING'};
my @temp = split(',',$ENV{'QUERY_STRING'});
my $needhelpwith = $temp[0];

&showhttpheaders();
&openpage($tr{'help'}, 1, '', 'help');

&openbigbox();

&openbox('');

my $helpfile;
if ($temp[1]) {
	$helpfile = "/var/smoothwall/mods/$temp[1]/httpd/html/help/$needhelpwith.html.$language";
}
else {
	$helpfile = "/httpd/html/help/$needhelpwith.html.$language";
}

open (FILE, $helpfile);
my $line;
while ( <FILE> ){
	$line =~s/\n/ /g;
	$line .= $_;
}
close (FILE);
print <<END
<table>
<tr>
	<td class='helpheader'>
		<a href="javascript:window.close();"><img src="/ui/img/help.footer.png" alt="SmoothWall Express Online Help - click to close window"></a>
	</td>
</tr>
<tr>
	<td style='text-align: justify; font-size: 11px;'>
END
;

foreach my $term ( keys %glossary ){
	$line =~s/([\W])($term)([^\w:])/$1\01$2\02$term\03$3/ig;
}

$line =~s/\01([^\02]*)\02([^\03]*)\03/<span style='color: #008b00;' onMouseOver="return escape('$glossary{$2}');">$1<\/span>/ig;
print $line;

print <<END
	</td>
</tr>
<tr>
	<td class='helpfooter'>
		<a href="javascript:window.close();"><img alt="Close this window" src="/ui/img/help.footer.png" border="0"></a>
	</td>
</tr>
</table>
END
;

&closebox();

&closebigbox();

&closepage('blank');

