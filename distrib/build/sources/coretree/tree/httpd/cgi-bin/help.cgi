#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

# load the glossary of terms.
# this is similar to the TR hash, but only required here, and therefore not
# included in it, (it could do with replacing anyhow)

require "/usr/lib/smoothwall/langs/glossary.en.pl";
if (${language} ne "en")
{
  foreach $key (sort keys %baseglossary)
  {
    $baseglossary{$key} = "[$baseglossary{$key}]"
  }
  if (-f "/usr/lib/smoothwall/langs/glossary.${language}.pl")
  {
    require "/usr/lib/smoothwall/langs/glossary.${language}.pl";
  }
}
require "/usr/lib/smoothwall/langs/glossary.base.pl";

my $needhelpwith = $ENV{'QUERY_STRING'};

unless ($needhelpwith =~ /^[A-Za-z0-9\.]+$/) {
	$needhelpwith = 'index.cgi';
}

&showhttpheaders();

&openpage($tr{'help'}, 1, '', 'help');

&openbigbox();

&openbox('');

if (-f "/httpd/html/help/$needhelpwith.html.$language")
{
  open (FILE, "/httpd/html/help/$needhelpwith.html.$language");
}
else
{
  open (FILE, "/httpd/html/help/$needhelpwith.html.en");
}
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

