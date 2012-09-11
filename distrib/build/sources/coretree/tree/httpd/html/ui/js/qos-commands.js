// qos-commands.js
//
// Copyright 2009, Neal P. Murphy. All rights reserved.

// License is granted to use and distribute this code with the free
//   Smoothwall Express firewall system. Any other use requires
//   a separate license.

// QoS Command Generation Functions
//
//   These functions build the tc() commands



// tweakTcMask()
//
// Make the mask proper for TC
//

function tweakTcMask(value, sep, hasMask, sansMask) {

  var maskLoc = value.indexOf(sep);
  if (maskLoc == -1) {
    // Mask not specified, add the default
    return value + hasMask + sansMask;
  } else {
    // Mask specified, change '/' to ' 0x'
    return value.replace(sep, hasMask);
  }
}

// getTrueCeil()
//
//   Get the true rate for this class; recurse back to root class
//

function getTrueCeil(qRuleIdx) {

  var qR_sh = qRules[qRuleIdx];
  var qRp_sh = qRules[qRuleIdx].params;

  var qRC = qR_sh.parent_rule.indexOf(':0');

  if (qRC == -1) {
    // Still working percentages
    return getTrueCeil(qR_sh.parent_rule);
  } else {
    // We've reached the root class
    switch (qRp_sh['rateUnit']) {
      case 'Mbit/s': return 1000*1000*qRp_sh['rate'];
      case 'kbit/s': return 1000*qRp_sh['rate'];
      case 'bit/s': return qRp_sh['rate'];
    }
  }

  // Parser, shut up!
  return 0;
}



// getTrueRate()
//
//   Get the true rate for this class; recurse back to root class
//

function getTrueRate(qRuleIdx) {

  var qR_sh = qRules[qRuleIdx];
  var qRp_sh = qRules[qRuleIdx].params;

  var qRC = qR_sh.parent_rule.indexOf(':0');

  if (qRC == -1) {
    // Still working percentages
    return qRp_sh['rate'] * getTrueRate(qR_sh.parent_rule) / 100;
  } else {
    // We've reached the root class
    switch (qRp_sh['rateUnit']) {
      case 'Mbit/s': return 1000*1000*qRp_sh['rate'];
      case 'kbit/s': return 1000*qRp_sh['rate'];
      case 'bit/s': return qRp_sh['rate'];
    }
  }

  // Parser, shut up!
  return 0;
}



// commands_rule()
//
//   Build the qdisc or class rule.
//

function commands_rule(qRule) {
  var cmd_txt = '';
  var html = '';

  var qR_sh = qRules[qRule];
  var qRp_sh = qRules[qRule].params;

  var qDash = qRule.indexOf('-');
  var pDash, qDev, qPrio, qHandle, pHandle;

  var cmd_pref = '\n  # '+ qR_sh.type+ ': ' +qR_sh.comment+ '\n';

  qDev = qRule.substring(0, qDash);
  qHandle = qRule.substring(qDash+1);

  pDash = qRules[qRule].parent_rule.indexOf('-');
  pHandle =  qRules[qRule].parent_rule.substring(pDash+1);

  switch (qR_sh.type) {

    // Queueing disciplines
    case 'ingress':
      if (pDash == -1) {
        cmd_pref += '  tc qdisc add dev $'+ qDev +'_DEV ingress handle ffff';
      }
    case 'qdisc':
      if (pDash == -1) {
        cmd_pref += '  tc qdisc add dev $'+ qDev +'_DEV root handle '+ qHandle.toHex();
      } else {
        cmd_pref += '  tc qdisc add dev $'+ qDev +'_DEV parent '+ pHandle.toHex() + ' handle '+ qHandle.toHex();
      }
      switch (qR_sh.family) {
        case '':
          // Not configured, do nothing
          break;
        case 'htb':
          // Hierarchical Token Bucket (classful)
          cmd_txt += cmd_pref +' htb';
          if (qRules[qRule].params['qdefault'] != '') {
            cmd_txt += ' default '+ String(qRules[qRule].params['qdefault']).toHex(16);
          }
          break;

        case 'pfifofast':
          // Packet FIFO_FAST (classless); this is the default qdisc; no parameters
          cmd_txt += "# Can't add PFIFO_FAST; it is the default root qDisc.";
          break;

        case 'pfifo':
          // Packet FIFO (classless) - has some logging/counting capacity
          cmd_txt += cmd_pref +' pfifo limit '+ qRp_sh['limit'];
          break;

        case 'bfifo':
          // Byte FIFO (classless) - has some logging/counting capacity
          cmd_txt += cmd_pref +' bfifo limit '+ qRp_sh['limit'] +'B';
          break;

        case 'tbf':
          // Token Bucket Filter (classless)
          cmd_txt += cmd_pref +' tbf';
          if (qRp_sh['limit'] != '')
            cmd_txt += ' limit '+ qRp_sh['limit'];
          if (qRp_sh['latency'] != '')
            cmd_txt += ' latency '+ qRp_sh['latency'];
          if (qRp_sh['burst'] != '')
            cmd_txt += ' burst '+ qRp_sh['burst'];
          if (qRp_sh['mpu'] != '')
            cmd_txt += ' mpu '+ qRp_sh['mpu'];
          if (qRp_sh['rate'] != '') {
            cmd_txt += ' rate ';
            switch (qRp_sh['rateUnit']) {
              case 'Mbps': cmd_txt += qRp_sh['rate'] * 1000 * 1000; break;
              case 'kbps': cmd_txt += qRp_sh['rate'] * 1000; break;
              case 'bps': cmd_txt += qRp_sh['rate']; break;
              default: break;
            }
          }
          break;

        case 'sfq':
          // Stochastic Fair Queueing (classless)
          cmd_txt += cmd_pref +' sfq';
          if (qRp_sh['perturb'] != '')
            cmd_txt += ' perturb '+ qRp_sh['perturb'];
          if (qRp_sh['quantum'] != '')
            cmd_txt += ' quantum '+ qRp_sh['quantum'];
          break;
      }
      return cmd_txt +'\n';
      break;

    // Classes
    case 'class':
      if (qDash == -1) {
        cmd_pref += '  tc class add dev $'+ qDev +'_DEV root handle '+ qHandle;
      } else {
        pDash = qRules[qRule].parent_rule.indexOf('-');
        pHandle =  qRules[qRule].parent_rule.substring(pDash+1);
        cmd_pref += '  tc class add dev $'+ qDev +'_DEV parent '+ pHandle.toHex() + ' classid '+ qHandle.toHex();
      }
      switch (qR_sh.family) {
        case 'htb':
          cmd_txt += cmd_pref +' htb';
          if (qRp_sh['priority'] != '')
            qPrio = qRp_sh['priority'];
            if (qPrio>7) qPrio=7;
            cmd_txt += ' prio '+ qPrio +' \\\n    ';
          if (qRp_sh['rate'] != '') {
            var fRate = getTrueRate(qRule);
            var iRate = parseInt(fRate);
            if ((fRate - iRate) > .5) {
              iRate += 1;
            }
            cmd_txt += ' rate ' + iRate;
          }
          if (qRp_sh['burstR'] != '') {
            cmd_txt += ' burst ';
            switch (qRp_sh['burstRUnit']) {
              case 'kB': cmd_txt += qRp_sh['burstR'] * 1000; break;
              case 'Bytes': cmd_txt += qRp_sh['burstR']; break;
              default: break;
            }
          }

          if (qRules[qRule].params.parent.indexOf(':0') == -1) {
            if (qRp_sh['ceil'] != '') {
              var fCeil = qRp_sh['ceil'] * getTrueCeil(qRule) / 100;
              var iCeil = parseInt(fCeil);
              if ((fCeil - iCeil) > .5) {
                iCeil += 1;
              }
              cmd_txt += ' ceil ' + iCeil;
            }
            if (qRp_sh['burstC'] != '') {
              cmd_txt += ' cburst ';
              switch (qRp_sh['burstCUnit']) {
                case 'kB': cmd_txt += qRp_sh['burstC'] * 1000; break;
                case 'Bytes': cmd_txt += qRp_sh['burstC']; break;
                default: break;
              }
            }
          }
          return cmd_txt +'\n';
          break;
      }
      break;
  }
}



// commands_filter()
//
//   Generate the filter's tc() command.
//

function commands_filter(qFilter) {
  var cmd_txt = '';
  var ipt_txt = '';
  var ret_html = '';
  var i, qFm_sh, qsplit;
  var showForm = 0;

  var qF_sh =  qRules[qFilter].params;

  var qDash = qFilter.indexOf('-');
  var qColon = qFilter.indexOf(':');
  var qLDash = qFilter.lastIndexOf('.');
  var qDev, qHandle, qFlowId, pDash, pHandle;
  var mustBuildClassify = 0, mustBuildFilter = 0, mustBuildNATfilter = 0;
  var havePort, haveProtocol;

  qDev = qFilter.substring(0, qDash);
  qFlowId = qFilter.substring(qDash+1, qLDash);

  pDash = qRules[qFilter].parent_rule.indexOf('-');
  pHandle =  qRules[qFilter].parent_rule.substring(pDash+1, qColon);

  // 2011-04-28
  // First find out if the NIC is NATted.
  // If so, find out if the filter has S_Addr, S_Port or Protocol set,
  //   and if that's *all* that's set.
  // If it is NATted and sources are set, generate the iptables CLASSIFY command
  // If it is NATted and non-sources are set, or if it is not NATted,
  //   generate the tc_filter command, and don't include the sources if it is
  //   NATted.

  if (qRules[qFilter].family == 'u32') {

    // Determine which commands need to be built. We may only need to build one
    //   or t'other. This also fixes when a filter is created but has no
    //   match clauses.
    for (i in qRules[qFilter].matches) {
      // A spot of shorthand
      qFm_sh = qRules[qFilter].matches[i];

      havePort = 0;  // no port matches found
      haveProtocol = 6;  // default to TCP
      switch (qFm_sh.type) {
        case "sport":
          havePort = 1;
        case "saddr":
        case "saddrhost":
          // saddr and sport force a CLASSIFY command if the NIC is NATted
          // But saddrhost forces a filter command if the NIC is NATted, because
          // the srcaddr is natural (not NATted) in this case.
          if (qRoots[qDev].NATted == '1') {
            if (qFm_sh.type == "saddrhost") {
              mustBuildNATfilter = 1;
            }
            else
            {
              mustBuildClassify = 1;
            }
          }
          else
          {
            mustBuildFilter = 1;
          }
          break;

        case "dport":
          havePort = 1;
          break;

        case "protocol":
          // Select the protocol if specified
          haveProtocol = qFm_sh.value;
          // Fall through!

        default:
          // All other matches force a FILTER command
          mustBuildFilter = 1;
          break;
      }
    }

    // If the packet will come from the F/W, build filter, not iptables
    //
    // Note: If the source address is the firewall, then a filter is built because
    //   the firewall's IP address is the natural source (it is not NATted).
    if (mustBuildNATfilter) {
      mustBuildClassify = 0;
      mustBuildFilter = 1;
    }
    // If a CLASSIFY must be built, do not build a filter.
    if (mustBuildClassify) {
      mustBuildFilter = 0;
    }

    // Build the CLASSIFY, if needed

    if (mustBuildClassify) {
      ipt_txt = '  iptables -t mangle -A trafficControl -o $'+ qDev +'_DEV';
      ipt_txt += ' --protocol '+ haveProtocol;
      for (i in qRules[qFilter].matches) {
        qFm_sh = qRules[qFilter].matches[i];

        // In case we need it.
        var tmp = qFm_sh.value.split("/");

        switch (qFm_sh.type) {
          case "saddr":
            ipt_txt += ' \\\n    --source '+ tmp[0];
            if (tmp[1] != 32) ipt_txt += "/"+ tmp[1];
            break;
          case "sport":
            ipt_txt += ' \\\n    -m multiport --sports '+ tmp[0];
            break;
          default:
            // Do nothing; we only care about source addr and/or port
            //
            // Note: This is wrong; if the packet is NATted, the whole
            //   classifier must be written as an iptables command.
            // Again note that packets from the F/W itself are not NATted.
        }
      }
      ipt_txt += ' \\\n    -j CLASSIFY --set-class '+ qFlowId.toHex() +'\n';
      // Add it to the commands
      cmd_txt += ipt_txt;
      // Add the 'delete' to the stop commands
      ipt_stop_txt += ipt_txt.replace('-A', '-D');
    }

  // Build the filter, if needed
    if (mustBuildFilter) {
      cmd_txt += '  tc filter add dev $'+ qDev +'_DEV';
      cmd_txt += ' protocol ip parent '+ pHandle.toHex() +':0';
      cmd_txt += ' prio '+  qF_sh['priority'] +' u32';
  
      // Build the matches
      for (i in qRules[qFilter].matches) {
        // A spot of shorthand
        qFm_sh = qRules[qFilter].matches[i];

        // Handle the type of match
        var tmpVal = '';
        switch (qFm_sh.type) {
          case "saddrhost":
            cmd_txt += ' \\\n    match ip src '+ qFm_sh.value +'/32';
            break;
          case "saddr":
            tmpVal = tweakTcMask (qFm_sh.value, '/', '/', '32');
            cmd_txt += ' \\\n    match ip src '+ qFm_sh.value;
            break;
          case "sport":
            tmpVal = tweakTcMask (qFm_sh.value, '/', ' 0x', 'ffff');
            cmd_txt += ' \\\n    match ip sport '+ tmpVal;
            break;
          case "daddr":
            cmd_txt += ' \\\n    match ip dst '+ qFm_sh.value;
            break;
          case "dport":
            tmpVal = tweakTcMask (qFm_sh.value, '/', ' 0x', 'ffff');
            cmd_txt += ' \\\n    match ip dport '+ tmpVal;
            break;
          case "protocol":
            tmpVal = tweakTcMask (qFm_sh.value, '/', ' 0x', 'ff');
            cmd_txt += ' \\\n    match ip protocol '+ tmpVal;
            break;
          case "tos":
            tmpVal = tweakTcMask (qFm_sh.value, '/', ' 0x', 'ff');
            cmd_txt += ' \\\n    match ip tos '+ tmpVal;
            break;
          case "u8":
            tmpVal = tweakTcMask (qFm_sh.value, '/', ' 0x', 'ff');
            cmd_txt += ' \\\n    match u8 '+ tmpVal;
            if (qFm_sh['offset']) {
              cmd_txt += ' at '+ qFm_sh['offset'];
            } else if (qFm_sh['nexthdr']) {
              cmd_txt += ' at nxthdr+'+ qFm_sh['nexthdr'];
            }
            break;
          case "u16":
            tmpVal = tweakTcMask (qFm_sh.value, '/', ' 0x', 'ffff');
            cmd_txt += ' \\\n    match u16 '+ tmpVal;
            if (qFm_sh['offset']) {
              cmd_txt += ' at '+ qFm_sh['offset'];
            } else if (qFm_sh['nexthdr']) {
              cmd_txt += ' at nxthdr+'+ qFm_sh['nexthdr'];
            }
            break;
          case "u32":
            tmpVal = tweakTcMask (qFm_sh.value, '/', ' 0x', 'ffffffff');
            cmd_txt += ' \\\n    match u32 '+ tmpVal;
            if (qFm_sh['offset']) {
              cmd_txt += ' at '+ qFm_sh['offset'];
            } else if (qFm_sh['nexthdr']) {
              cmd_txt += ' at nxthdr+'+ qFm_sh['nexthdr'];
            }
            break;
        }
      }
      cmd_txt += ' \\\n    flowid '+ qFlowId.toHex();
    }
    return cmd_txt +'\n';
  }
}



// buildScript()
//
//   The top of the tree command path. Build tc() commands for all included NICs.
// Build the command recursively from each NIC.

function buildScript() {
  var nic = '';
  var kid = '';

  commands = '';

  // Clear the ipt -D commands
  ipt_stop_txt = '';


  // Start generating the shell script
  commands += '#! /bin/bash\n';
  commands += '\n# Get the NIC definitions\n';
  commands += '. /var/smoothwall/ethernet/settings\n';
  commands += 'if [ $RED_ADDRESS == "0.0.0.0" ]; then\n';
  commands += '  RED_ADDRESS=`cat /var/smoothwall/red/local-ipaddress`\n';
  commands += 'fi\n';

  commands += '\nexport PATH="/sbin:/usr/sbin:/bin:/usr/bin"\n';
  commands += '\n';
  commands += '# function usage() displays a usage statement and exits \'false\'\n';
  commands += '#\n';
  commands += 'function usage () {\n';
  commands += '  echo "Usage: $0 stop | start | restart"\n';
  commands += '  exit 1\n';
  commands += '}\n';
  commands += '\n';


  // Generate the 'restart' action
  commands += '# function restart calls stop() and start()\n';
  commands += '#\n';
  commands += 'function restart () {\n';
  commands += '  stop\n';
  commands += '  start\n';
  commands += '}\n';
  commands += '\n';


  // Generate the 'start' action
  commands += '# function start loads the scheduling modules as needed and enables TC\n';
  commands += '#\n';
  commands += 'function start () {\n';
  commands += '  # First be sure the needed modules are loaded\n';
  commands += '  /sbin/modprobe cls_basic\n';
  commands += '  /sbin/modprobe cls_flow\n';
  commands += '  /sbin/modprobe cls_fw\n';
  commands += '  /sbin/modprobe cls_route\n';
  commands += '  /sbin/modprobe cls_tcindex\n';
  commands += '  /sbin/modprobe cls_u32\n';
  commands += '  /sbin/modprobe sch_cbq\n';
  commands += '  /sbin/modprobe sch_dsmark\n';
  commands += '  /sbin/modprobe sch_gred\n';
  commands += '  /sbin/modprobe sch_hfsc\n';
  commands += '  /sbin/modprobe sch_htb\n';
  commands += '  /sbin/modprobe sch_prio\n';
  commands += '  /sbin/modprobe sch_red\n';
  commands += '  /sbin/modprobe sch_sfq\n';
  commands += '  /sbin/modprobe sch_tbf\n';
  commands += '  /sbin/modprobe sch_teql\n';
  commands += '\n';
  // Clear the IPTables guck
  commands += '  # Clear the iptables TC guck\n';
  commands += '  iptables -t mangle -D FORWARD -o $RED_DEV -j trafficControl >/dev/null 2>&1\n';
  commands += '  iptables -t mangle -F trafficControl >/dev/null 2>&1\n';
  commands += '  iptables -t mangle -X trafficControl >/dev/null 2>&1\n';
  // Prepare the IPTables guck
  commands += '  # Prepare the iptables TC guck\n';
  commands += '  iptables -t mangle -N trafficControl\n';
  commands += '  iptables -t mangle -A FORWARD -o $RED_DEV -j trafficControl\n';
  commands += '\n';

  // Loop through all the specified NICs; this should be generated by CGI
  for (var i=0; i<qNics.length; i++) {
    nic = qNics[i];
    bgBool = 0;
    var indentHTML = 2;
    var marginLeft = 2;

    // Mark beginning of NIC
    commands += '  # Start of commands for '+ nic +'\n  #\n  #\n';

    // Clear the NIC of TC
    commands += '\n  if [ "$'+ nic +'_DEV" != "" ]; then\n';
    commands += '    # Always silently clear the NIC, configured or not, if it has an IF\n';
    commands += '    tc qdisc del dev $'+ nic +'_DEV root >/dev/null 2>&1\n';
    commands += '  fi\n';

    // Handle the NIC, be it open, closed or unconfigured
    switch (qRoots[nic].view) {
      case 'open':
      case 'closed':
        // A NIC has only a qDisc, so start recursing with the qDisc.
        if ((kid = qRoots[nic].qDisc) != '') {
          if (qRoots[nic].state == 'active') {
            commands += '    echo "Starting TC on '+ nic +' ($' + nic + '_DEV)"\n';
            commands_nic_tree(kid);
          }
        }
        break;

      case 'nocfg':
        // If not configured, ignore it.
        break;
    }
    // Mark end of NIC
    commands += '\n  #\n  #\n  # End of commands for '+ nic +'\n\n';
  }
  commands += '}\n';
  commands += '\n';


  // Generate the 'stop' action
  commands += '# function stop disables TC; the system restores the default qdisc\n';
  commands += '#\n';
  commands += 'function stop () {\n';

  // Clear the IPTables guck
  commands += '  # Clear the iptables TC guck\n';
  commands += '  iptables -t mangle -D FORWARD -o $RED_DEV -j trafficControl >/dev/null 2>&1\n';
  commands += '  iptables -t mangle -F trafficControl >/dev/null 2>&1\n';
  commands += '  iptables -t mangle -X trafficControl >/dev/null 2>&1\n';

  // Loop through all the specified NICs; this should be generated by CGI
  for (var i=0; i<qNics.length; i++) {
    nic = qNics[i];

    // Stop TC on the NIC, regardless of status
    commands += '\n  if [ "$'+ nic +'_DEV" != "" ]; then\n';
    commands += '    echo "Stopping TC on '+ nic +' ($' + nic + '_DEV)"\n';
    commands += '    tc qdisc del dev $'+ nic +'_DEV root >/dev/null 2>&1\n';
    commands += '  fi\n';
  }
  commands += '}\n';
  commands += '\n';


  // Generate the action selector
  commands += '# The main actions\n';
  commands += '#\n';
  commands += 'case $1 in\n';
  commands += '  stop) stop; ;;\n';
  commands += '  start) start; ;;\n';
  commands += '  restart) restart; ;;\n';
  commands += '  *) usage; ;;\n';
  commands += 'esac\n';
  commands += '\n';
  commands += 'exit\n';
  commands += '\n';

  return commands;
}


// command_qNics()
//
//   The top of the tree command path. Build tc() commands for all included NICs.
// Build the command recursively from each NIC. Put it in a form.

function command_qNics() {
  var commands_outer;
  var tcgen_div;

  commands_outer = '<form name="SaveScript" style="text-align:center" method="post"\n';
  commands_outer += '      action="'+ location +'">\n';
  commands_outer += '  <input align="center" type="submit" name="SAVE_SCRIPT" value="Save"\n';
  commands_outer += '         style="margin-top:8pt; background-color:#dfdf00; font-weight:bold">\n';
  commands_outer += '  <textarea id="textscript" name="textscript" rows="15" cols="80"\n';
  commands_outer += '            style="font-size:9pt">';
  commands_outer += buildScript();
  commands_outer += '</textarea><br>';
  commands_outer += '</form>\n';

  // Get the object, and store the generated commands
  tcgen_div = document.getElementById('tcgen_div');
  tcgen_div.innerHTML = commands_outer;

  commands = '';
  commands_outer = '';
}
