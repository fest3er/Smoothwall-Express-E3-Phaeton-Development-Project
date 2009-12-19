SNMP mod example
================

This is an example of what can be done with Smoothie Express 3.0 mods.  The
example in question is a simple snmp service, based on net-snmp.  The main
purpose is to show the nicest way to do mods, not to be useful. :)

It shows the following things:

1. Module layout

Each mod should be a dir under sources/mods.  In this sources/mods dir are a
couple of Makefiles that can be used when building a mod and building a
smoothd plugin for a mod.  Using these templates, compiling up smoothd
plugins is easy.

In each mod dir, there is a Makefile listing the packages in the mod, its
name, and a list of files that need to be in the mod patch (beside the setup
script and patch.tar.gz).

2. Coretree stuff

Each mod probably needs a coretree, which has additional CGIs etc.  This is
an overlay onto the smoothie directory structure.  The Makefile for the
coretree also fixes up the permissions.

2.a. Menu structure

The SNMP mod contains a new page under services.  Look in
/usr/lib/smoothwall/menu.  The 'sssnmp' is a tr key for the menu text.

2.b. New CGI

snmp.cgi is a trivial CGI that has only 2 fields, an enabled ticky and a
string to set the SNMP "community".  Other things are of course possible.

3. Additional programs

The bulk of this mod is the net-snmp package Makefile.  net-snmp is a
trivial program to build, but a small patch is used to make the PID file
readable to the webserver.

Building
========

Just type "make" in the directory you are in.  The module will be compiled,
packaged and deposited in this directory with the name
<modname>-<arch>.tar.gz

Limitatations
=============

Net-SNMP does not like listening on all interfaces, so in this mod it only
listens on GREEN and PURPLE.

The tr hash is extended in a cheesy way using the setup script.

There is no log file section added by this mod: to do so would need a patch
of the logging cgi.

Likewise for the status indicator.

There is no online help, but this could easily be added.

Close
=====

Hope someone finds this little mod template useful.  Of course, you don't
have to make mods this way - but I think its the nicest way.  I have no
time, unfortunately, to help mod-authors. :( Sorry.

Lawrence Manning (aslak)
August 20, 2007
