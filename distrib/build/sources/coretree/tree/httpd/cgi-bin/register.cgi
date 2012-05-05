#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );
use IO::Socket;

my %ownership;
&readhash( "/var/smoothwall/main/ownership", \%ownership );

my (%settings,$errormessage);
&getcgihash(\%settings);

my $sysid = &getsystemid();

if ( defined $settings{'ACTION'} and $settings{'ACTION'} eq $tr{'get system id'} ){
	if ( -e "/var/smoothwall/notregistered" ) {
        	system( '/usr/bin/smoothwall/machine_reg.pl');
		if ( $? eq 0 ){
                	unlink "/var/smoothwall/notregistered";
		} else {
               		# Register: Failed :(
               	}
	}
}

if ($settings{'ACTION'} eq $tr{'no thanks'} ){
	#&dont_register();
	print "Status: 302 Moved\nLocation: /cgi-bin/index.cgi\n\n";
	exit(0);
} else {
	&showhttpheaders();
	&openpage($tr{'register title'}, 1, "", 'register');
	&openbigbox();
	&alertbox($errormessage);
	&register_page();
}


&closebigbox();
&closepage();

sub dont_register
{
	my ( $settings ) = @_;
	
	$ownership{ 'ADDED_TO_X3' } = -1;
	&writehash( "/var/smoothwall/main/ownership", \%ownership );
}

sub register_page
{

  &openbox();

  print qq!
<div align="center">
  <h2 style="font-size:15pt">SmoothWall Express $version</h2>
  <p>User interface version $webuirevision
</div>
!;

  &closebox();

# START x3 add bit
&openbox();

if ( not defined $ownership{'ADDED_TO_X3'} or $ownership{'ADDED_TO_X3'} ne "1" )
{
  print qq!
<div style="margin:0; text-align:centered">
  <p style="margin:0">
    <a target='_new' href='https://my.smoothwall.org'><img src='/ui/img/frontpage/frontpage.x3.jpg' title='my.SmoothWall'/></a>
  </p>
</div>
!;
&closebox();

&openbox();
  print qq!
<div style="margin:4pt 4pt 4pt 4pt">
$tr{'x3 reg info'}
</div>
!;

  if ( (-e "/var/smoothwall/notregistered" or
        ( not defined $ownership{'ID'} or
        $ownership{'ID'} eq "" )) and
      -e "/var/smoothwall/red/active")
  {
    print qq!
<div style='width: 100%; text-align: justify;'>
  $tr{'missing installation id'}
</div>
<div style='width: 100%; text-align: center;'>
  <form method='post'>
    <input type='submit' name='ACTION' value='$tr{'get system id'}'>
  </form>
</div>
!;
  } else {
    print qq!
<div style="text-align:center">
<div style="display:inline-block">
  <form method='post' action='https://my.smoothwall.org/cgi-bin/signin.cgi' target='_blank'>
    <input type="hidden" name=id value='$sysid'>
    <input name="ACTION" type='submit' value="$tr{'register'}"
           style="margin:8pt 40pt 8pt 8pt; display:inline-block">
  </form>
</div>
!;

    if ( not defined $ownership{'ADDED_TO_X3'} or
         $ownership{'ADDED_TO_X3'} ne "-1" )
    {
      print qq!
<div style="display:inline-block">
  <form method='post'>
    <input name="ACTION" type='submit' value='$tr{'no thanks'}'
           style="margin:8pt 8pt 8pt 40pt; display:inline-block">
  </form>
</div>
!;
    }

    print qq!
</div>
!;
  }
} else {
	print <<END
<table class='centered'>
	<tr>
		<td>
			<a target='_new' href='https://my.smoothwall.org'><img src='/ui/img/frontpage/frontpage.jpg' title='my.SmoothWall'/></a>
			<br/>
		</td>
	</tr>
</table>
END
;
}



&closebox();
# END x3 add bit

&openbox();

print <<END
<div style="text-align:left; margin:4pt 4pt 4pt 4pt">
  <div style='width:35%; text-align:center; vertical-align:top;
              margin:0 0 0 8pt; display:inline-block; float:right;
              border:1pt dashed gray'>
    <p style="margin:4pt; text-align:center">
      For more information about SmoothWall Express, please visit our website at
      <a href="http://smoothwall.org/" target="_breakoutWindow">
        http://smoothwall.org/</a>.
    </p>
    <p style="margin:4pt">
      For more information about SmoothWall products, please visit 
      our website at
      <a href="http://www.smoothwall.net/"
         target="_breakoutWindow">
        http://www.smoothwall.net/</a>.
    </p>
    <p style="margin:4pt">
      Visit the
      <a href="http://community.smoothwall.org/forum/viewforum.php?f=91"
         target="_blank">
        Phaeton/Roadster fora
      </a>
      and the
      <a href="http://roadster.agcl.us">
        Roadster download site
      </a> to learn more about the Phaeton/Roadster Development Project.
    </p>
  </div>

    <p style="margin:8pt 4pt 8pt 4pt; text-align:center">
      SmoothWall Express $version<br>
      Copyright &copy; 2000 - 2007 the
      <a href="http://smoothwall.org/team/" target="_breakoutWindow">
        SmoothWall Team</a>.
    </p>
    <p>
      A full team listing can be found
      <a href="http://smoothwall.org/team/" target="_breakoutWindow">
        on our website</a>.
      The copyrights to ortions of this software are held by the original
      authors. The source code of such portions are 
      <a href="http://smoothwall.org/sources.html" target="_breakoutWindow">
        available under the terms of the appropriate licenses</a>.
    </p>
  <p>
    Roadster is a development and testing version of Smoothwall Express that
    showcases new features and updated software. The newer software packages
    used to build Roadster usually preclude full compatibility with
    Smoothwall Express.
    </p>
    <p>
    SmoothWall&trade; is a trademark of SmoothWall Limited.
    Linux<sup>&reg;</sup> is a registered trademark of Linus Torvalds.
    All trademarks and copyrights are the property of their
    respective owners.
    Stock photography used courtesy of
    <a href="http://istockphoto.com/">iStockphoto.com</a>.
  </p>
</div>
END
;

&closebox();
}
