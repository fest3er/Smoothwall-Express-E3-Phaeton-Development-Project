#!/bin/sh

. /var/smoothwall/adsl/settings
. /var/smoothwall/adsl/eci/$ECITYPE

echo "Modem is a $FULLNAME"
/usr/bin/eciadsl-firmware $VID1 $PID1 $VID2 $PID2 /etc/eciadsl/$FIRM1
/usr/bin/eciadsl-synch -mc GS7470 $VID2 $PID2 /etc/eciadsl/$FIRM2

