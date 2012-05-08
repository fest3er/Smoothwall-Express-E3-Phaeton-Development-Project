#! /bin/bash

cd /var/smoothwall/outgoing

. ../main/productdata-RESTORED
OIFS=$IFS; IFS="-"; set $RELEASE; IFS=$OIFS
oldVersion=$2

if [ "$oldVersion" == "2.1.0" ]; then
  # just use the restored entries
  cp config-RESTORED config
elif [ "$oldVersion" == "2.0.2" -o "$oldVersion" == "2.0.1" ]; then
  echo "    Roadster's outgoing policy is now 'closed with exceptions'."
  echo "    Upgrading your outgoing rules to the new TOFC format."
  if grep -i "openness=open" ../main/settings-RESTORED; then
    # Upgrade the old to new format, followed by new rules
    cat config-RESTORED config-FRESH | /tmp/upgrade-outgoing.pl > config
    echo "    Your original rules have been inserted *before* the fresh rules."
    sleep 3
  else
    # Upgrade to the new TOFC format
    /tmp/upgrade-outgoing.pl <config-RESTORED >config
  fi
  echo "    Check your outgoing (TOFC) rules to verify correct operation."
  echo "    The rules most likely need to be editted."
else
  # just use the new entries
  cp config-FRESH config
fi
