#!/usr/bin/perl
#
# SmoothWall CGIs
#
# This code is distributed under the terms of the GPL
#
# (c) The SmoothWall Team

use lib "/usr/lib/smoothwall";
use header qw( :standard );

&showhttpheaders();

&openpage($tr{'msecure shell'}, 1, '', 'tools');

&openbigbox('100%', 'LEFT');

&alertbox($errormessage);

&openbox($tr{'secure shellc'}, '');
print <<END
<DIV ALIGN='CENTER'>
<applet codebase="/"
archive="/jta25.jar"
code="de.mud.jta.Applet" 
width="590" height="360">
<param name="socket.host" value="$ENV{'SERVER_ADDR'}">
<param name="plugins" value="Status,Socket,SSH,Terminal">
<param name="pluginPath" value="de.mud.jta.plugin">
<param name="layout.Status" value="South">
<param name="layout.Terminal" value="Center">
<param name="Terminal.resize" value="screen">
<param name="Terminal.scrollBar" value="East">
<param name="Applet.disconnect" value="false">
<param name="SSH.id" value="root">
<param name="Socket.port" value="222">
<param name="Terminal.foreground" value="#FFFFFF">
<param name="Terminal.background" value="#000000">
<param name="Terminal.id" value="linux">
You require <a href="http://java.sun.com/">Java</a> to use the shell applet
</applet>
</DIV>
END
;

&closebox();

print "</FORM>\n";

&alertbox('add','add');

&closebigbox();

&closepage();
