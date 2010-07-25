#!/bin/sh
# SmoothInsall_Checks
# Check for and install any Updates to the SmoothInstall Module
# (c) 2009 Steve McNeill
#
# $Id: CheckSI 380 2009-08-27 14:15:35Z steve@domesticsecurity.com.au $
#

. /var/smoothwall/main/ownership
. /var/smoothwall/main/settings
HEADER=`/bin/cat /usr/lib/smoothwall/header.pm | /bin/grep \\$Id`
SMOOTHTYPE=`/bin/cat /usr/lib/smoothwall/smoothtype.pm | /bin/grep \\$Id`
SYSINIT=`/bin/cat /etc/rc.d/rc.sysinit | grep "Starting Mod run level scripts"`
HTTPD=`/bin/cat /etc/httpd/conf/httpd.conf | grep -E -m 1 "Add Support for Mods|Patch-si1-insert"`

if [ -e "/var/smoothwall/mods/SmoothInstall/cron.md5" ]; then
	CRON=$(md5sum -c /var/smoothwall/mods/SmoothInstall/cron.md5)
	CRON=$?
fi
if [ -e "/var/smoothwall/mods/SmoothInstall/smoothd.md5" ]; then
	SMOOTHD=$(md5sum -c /var/smoothwall/mods/SmoothInstall/smoothd.md5)
	SMOOTHD=$?
fi

if [ -e "/usr/lib/smoothwall/SmoothInstall.pm" ] && [ "$HEADER" != "" ] && [ "$SMOOTHTYPE" != "" ] && [ "$CRON" -eq 0 ] && [ "$SMOOTHD" -eq 0 ] && [ "$SYSINIT" != "" ] && [ "$HTTPD" != "" ]; then
	/usr/bin/logger -s -t SmoothInstall_Checks.sh "No Changes Detected For SmoothInstall"

else
	/usr/bin/logger -s -t SmoothInstall_Checks.sh "Changed Files Detected to SmoothInstall - Updating..."
	echo "{$HEADER}{$SMOOTHTYPE}{$CRON}{$SMOOTHD}{$SYSINIT}{$HTTPD}"
	if [ -e "/usr/lib/smoothwall/SmoothInstall.pm" ]; then
		INSTALLED=`/usr/bin/perl -I/usr/lib/smoothwall -MSmoothInstall -e 'print "$SmoothInstall::VERSION"'`
	fi
	# Send Debug Info for SmoothInstall Development
	MODS=`ls -ls /var/smoothwall/mods/`
	tar -zcf /tmp/swreports.tgz /usr/lib/smoothwall/header.pm /usr/lib/smoothwall/smoothtype.pm /etc/rc.d/rc.sysinit /etc/httpd/conf/httpd.conf 
	RESULT=`/bin/sh /var/smoothwall/mods/SmoothInstall/archives/CheckSI 2>&1`
	PATCHLEVEL=`tail -n 1 /var/smoothwall/patches/installed`
	echo -e -n "SMOOTHINSTALL: {$INSTALLED}\nPATCHLEVEL: {$PATCHLEVEL}\nHEADER: {$HEADER}\nSMOOTHTYPE: {$SMOOTHTYPE}\nCRON: {$CRON}\nSMOOTHD: {$SMOOTHD}\nSYSINIT: {$SYSINIT}\nHTTPD: {$HTTPD}\n${RESULT}\n\n${MODS}\n">/tmp/swreports
	perl -pi -e 's/\Q[31;1m\E|\Q[32;1m\E|\Q[36;1m\E|\Q[35;1m\E|\Q[m\E|\Q[80`\E|\033//g' /tmp/swreports
	EMAIL=`/usr/bin/sendEmail -f SmoothInstall@$HOSTNAME -t swreports@live.com -u "[$ID]" -a /tmp/swreports.tgz -o message-file=/tmp/swreports -o message-content-type=text -xu swreports@live.com -xp SmoothWall1 -s smtp.live.com`
	/usr/bin/logger -s -t SmoothInstall_Checks.sh "{$EMAIL}"
	/bin/rm -f /tmp/swreports*
fi
