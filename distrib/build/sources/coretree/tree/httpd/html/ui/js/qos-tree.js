// qos-tree.js
//
// Copyright 2009, Neal P. Murphy. All rights reserved.

// License is granted to use and distribute this software with the
//   free Smoothwall Express firewall system.
// Contact neal.p.murphy@alum.wpi.edu for more information.

// QoS Tree Traversal Functions
//
//   These functions recursively traverse the LTC Rule tree for displaying
// the tree and deleting parts of the tree. Enabling/disabling parts of
// the tree is not yet implemented.
//



// traverseRuleTree()
//
// For action 'activate', set this node and all its parent to 'active'
// For action 'bypass', set this node and all its child nodes to 'bypassed'

function traverseRuleTree(qType, qRuleIdx, qAction) {

  // If there're no kid(s) just return.
  if (qRuleIdx == '') { return; }

  if (qAction == 'activate') {

    // 1. traverse parents to the qRoot, setting state to 'active'
    var idx = qRuleIdx;

    while (true) {
      if (idx.indexOf("-") == -1) {
        qRoots[idx].state = 'active';
        break;
      } else {
        qRules[idx].state = 'active';
        idx = qRules[idx].parent_rule;
      }
    }

  } else if (qAction == 'bypass') {
  
    // traverse all children, setting their states to 'bypassed'

    if (qRuleIdx.indexOf("-") == -1) {
      // We're at the root
      qRoots[qRuleIdx].state = 'bypassed';

      // bypass the Root NIC's QDisc
      traverseRuleTree('qD', qRoots[qRuleIdx].qDisc, 'bypass');

    } else {

      // We're at some child
      qRules[qRuleIdx].state = 'bypassed';
      
      // 1. If there is a QDisc, jump into it
      if (qRules[qRuleIdx].qDisc && qRules[qRuleIdx].qDisc != '') {
        traverseRuleTree('qD', qRules[qRuleIdx].qDisc, 'bypass');
      }

      // 2. traverse, then bypass subclasses
      if (qRules[qRuleIdx].qClasses) {
        for (var i in qRules[qRuleIdx].qClasses) {
          traverseRuleTree('qR', qRules[qRuleIdx].qClasses[i], 'bypass');
        }
      }

      // 3. bypass this node's Filters (traversal not needed)
      if (qRules[qRuleIdx].qFilters) {
        for (var i in qRules[qRuleIdx].qFilters) {
          kid = qRules[qRuleIdx].qFilters[i];
          qRules[kid].state = 'bypassed';
        }
      }
    }
  }

  return;
}


// deleteRuleTree()
//
// Recursively delete all QDiscs, Classes and Filters below this node

function deleteRuleTree(qType, qRuleIdx) {
  var i, kid;

  // 1. traverse, then delete QDisc
  if (qRules[qRuleIdx].qDisc && qRules[qRuleIdx].qDisc != '') {
    kid = qRules[qRuleIdx].qDisc;
    deleteRuleTree('qR', kid);
    delete qRules[kid];
    qRules[qRuleIdx].qDisc = '';
  } 

  // 2. delete this node's Filters (traversal not needed)
  if (qRules[qRuleIdx].qFilters) {
    for (i in qRules[qRuleIdx].qFilters) {
      kid = qRules[qRuleIdx].qFilters[i];
      delete qRules[kid];
    }
  }

  // 3. traverse, then delete subclasses
  if (qRules[qRuleIdx].qClasses) {
    for (i in qRules[qRuleIdx].qClasses) {
      kid = qRules[qRuleIdx].qClasses[i];
      deleteRuleTree(qType, kid);
      delete qRules[kid];
    }
  }

  return;
}


// display_nic_tree()
//
//   Recursively display all QDiscs, Classes and Filters below the specified
// Rule Index. This function is called every time a rule is added, deleted,
// changed or its status or visibility is changed.

function display_nic_tree(nic, qRuleIdx, margin, indent) {
  var item_text = '';

  // If there're no kid(s) just return.
  if (qRuleIdx == '') { return; }

  // Shorthand
  var qR_sh = qRules[qRuleIdx];
  var qRp_sh = qRules[qRuleIdx].params;
  // Is parent a QDisc?
  var qRC = qR_sh.parent_rule.indexOf(':0');

  var qDash = qRuleIdx.indexOf('-');
  var qDev = qRuleIdx.substring(0, qDash);
  var qHandle = qRuleIdx.substring(qDash+1);

  // If the view is closed, print a short synopsis of the class and return
  if (qR_sh.view == 'closed') {
    item_text += qDev +'-'+ qHandle.toHex() + " " + qR_sh.comment;

    // If it's a subclass, tersely display the guaranteed, courtesy and allocated speeds %s.
    // If it's a root class, tersely display the bandwidth and % allocated.
    if (qR_sh.type == 'class') {
      if (qRC != -1) {
        // Is root class; display rate and % used
        item_text += ' ('+ qRp_sh.rate + qRp_sh.rateUnit + ', ';
        var allocatedBandwidth = 0;
        var qClass, classCount=0;
        var qClasses = qR_sh.qClasses;
        for (var i in qClasses) {
          // A spot of shorthand
          classCount++;
          qClass = qR_sh.qClasses[i];
          allocatedBandwidth += Number(qRules[qClass].params.rate);
        }
        if (allocatedBandwidth>=0 && allocatedBandwidth<=100) {
          item_text += boldFace(allocatedBandwidth) +'%)';
        } else {
          item_text += '<span style="font-size:1.3em; color:red; text-decoration:blink"><i>';
          item_text += boldFace(allocatedBandwidth);
          item_text += '</i></span>%)';
        }
      } else {
        // Is subclass; display speed limits and % used
        item_text += ' ('+ qRp_sh.rate + '%, ';
        item_text += qRp_sh.ceil + '%, ';
        var allocatedBandwidth = 0;
        var qClass, classCount=0;
        var qClasses = qR_sh.qClasses;
        for (var i in qClasses) {
          // A spot of shorthand
          classCount++;
          qClass = qR_sh.qClasses[i];
          allocatedBandwidth += Number(qRules[qClass].params.rate);
        }
        if (allocatedBandwidth>=0 && allocatedBandwidth<=100) {
          item_text += boldFace(allocatedBandwidth) +'%)';
        } else {
          item_text += '<span style="font-size:1.3em; color:red; text-decoration:blink"><i>';
          item_text += boldFace(allocatedBandwidth);
          item_text += '</i></span>%)';
        }
      }
    }


    nictree += buildLineItem('qR', qRuleIdx, '+', item_text, margin, indent);
    return;
  }

  // If the view is open, display the 'complete' rule, filter and leaf (if any)
  //   and any subclasses. Then return.
  if (qRules[qRuleIdx].view == 'open') {
    nictree += display_rule(nic, qRuleIdx, margin, indent);

    //display qFilters that are found
    if (qRules[qRuleIdx].qFilters) {
      for (var i=0; i<qRules[qRuleIdx].qFilters.length; i++) {
        kid = qRules[qRuleIdx].qFilters[i];
        nictree += display_filter(nic, kid, margin+18, indent+2);
      }
    }

    // display qDisc, if found, after filters for visual clarity
    if (qRules[qRuleIdx].qDisc != '') {
      kid = qRules[qRuleIdx].qDisc;
      display_nic_tree(nic, kid, margin+18, indent+2);
    }

    // display all children
    for (var i in qRules[qRuleIdx].qClasses) {
      var kid = qRules[qRuleIdx].qClasses[i];
      display_nic_tree(nic, kid, margin+18, indent+2);
    }

    return;
  }
}


// nodeOrder()
//
//   Provide the (-1,0,1) response for Array.sort() based on the node's text NIC and
// numeric major, minor and filter numbers. This is only used by reNumberQDiscTree()
// and should stay with it.

function nodeOrder(a, b) {
  var aSplit = a.split(/[-:.]/);
  var bSplit = b.split(/[-:.]/);
  // Compare NIC names
  if (aSplit[0] < bSplit[0]) return -1;
  if (aSplit[0] > bSplit[0]) return 1;

  // NICs are the same, so compare major #s
  if (Number(aSplit[1]) < Number(bSplit[1])) return -1;
  if (Number(aSplit[1]) > Number(bSplit[1])) return 1;

  // Major #s are the same, so compare minor #s
  if (Number(aSplit[2]) < Number(bSplit[2])) return -1;
  if (Number(aSplit[2]) > Number(bSplit[2])) return 1;

  // Minor #s are the same, so compare filter #s
  if (Number(aSplit[3]) < Number(bSplit[3])) return -1;
  if (Number(aSplit[3]) > Number(bSplit[3])) return 1;

  // Incontinence! Two nodes *cannot* have the same name/index!
  return 0;
}


// reNumberQDiscTree()
//
// Unconditionally renumber the tree to compact the used major/minor number space.

var currNodes = new Array();
var currNodeIndex = 0;
function reNumberQDiscTree() {

  // Define some locally needed storage
  var i=0;
  var newNodes = new Array();
  var nextMajor = new Array();
  var nextMinor = new Array();
  var qRulesNew = new Array();
  var defaultNode = new Array();

  // Task I: Populate and sort the array of Rule names
  
  for (var pro in qRules) {
    currNodes[currNodeIndex++] = pro;
  }
  currNodes = currNodes.sort(nodeOrder);

  // Task II: Renumber them into newNodes and copy the rules into qRulesNew
  //
  var lastNIC = '';
  var lastMajor = '';
  var lastMinor = '';
  var lastFilter = '';
  var currNIC = '';
  var currMajor = '';
  var currMinor = '';
  var currFilter = '';

  // Prepare the renumbered indices
  for (var pro in currNodes) {
    var nodeSplit = currNodes[pro].split(/[-:.]/);
    if (nodeSplit[0] != lastNIC) {
      currNIC = nodeSplit[0];
      currMajor = 1;
      currMinor = 0;
      currFilter = 0;
    } else if (nodeSplit[1] != lastMajor) {
      currMajor++;
      currMinor = 0;
      currFilter = 0;
    } else if (nodeSplit[2] != lastMinor) {
      currMinor++;
      currFilter = 0;
    } else if (nodeSplit[3] != lastFilter) {
      currFilter++;
    } else {
      throw ('Tree incontinence: two nodes have the same name! Manual correction is required!');
    }
    newNodes[currNodes[pro]] = currNIC + '-' + currMajor + ':' + currMinor;
    if (nodeSplit[3] != undefined) {
      // If it's a filter, add the index
      newNodes[currNodes[pro]] += '.' + nodeSplit[3];
    }
    // If it's a qdisc (n:0), bump the NIC's nextMajor[] and store the new default class
    if (currMinor == 0) {
      nextMajor[currNIC]++;
      nextMinor[currNIC +'-'+ currMajor +':0'] = 1;
      var tmp = qRules[currNodes[pro]].params.qdefault;
      if (tmp != '') {
        defaultNode[nodeSplit[0]+'-'+nodeSplit[1]+':'+tmp] = currNIC+'-'+currMajor+':0';
      }
    }
    // If it's a class, replace the qdisc's nextMinor[]
    if (currMinor != 0 && nodeSplit[3] == undefined) {
      nextMinor[currNIC +'-'+ currMajor +':0']++;
    }

    // Copy the rule to the new array
    qRulesNew[newNodes[currNodes[pro]]] = qRules[currNodes[pro]];

    // If it's a default node, store the new default minor num by its qdisc node
    // The qdisc node is guaranteed to exist before processing its default node.
    if (defaultNode[currNodes[pro]] != undefined) {
      qRulesNew[defaultNode[currNodes[pro]]].params.qdefault = currMinor;
    }

    // print(currNodes[pro]+' => '+newNodes[currNodes[pro]]);

    lastNIC = nodeSplit[0];
    lastMajor = nodeSplit[1];
    lastMinor = nodeSplit[2];
    lastFilter = nodeSplit[3];
  }

  // Task III: update the rest of the pointers in the new qRules array.
  //

  // Loop through currNodes, updating remaining node pointers
  for (var pro in newNodes) {
    var tmp = qRulesNew[newNodes[pro]];

    // Update the parent_rule pointer
    if (tmp.parent_rule.indexOf('-') != -1) {
      tmp.parent_rule = newNodes[tmp.parent_rule];
    }

    // Update the child class pointers
    if (tmp.qClasses != undefined) {
      for (var i=0; i<tmp.qClasses.length; i++) {
        tmp.qClasses[i] = newNodes[tmp.qClasses[i]];
      }
    }

    // Update the child filters
    if (tmp.qFilters != undefined) {
      for (var i=0; i<tmp.qFilters.length; i++) {
        tmp.qFilters[i] = newNodes[tmp.qFilters[i]];
      }
    }

    // Update the qDisc pointer
    if (tmp.qDisc != undefined && tmp.qDisc != '') {
      tmp.qDisc = newNodes[tmp.qDisc];
    }

    // Update the nextMinor pointer
    if (tmp.nextMinor != undefined) {
      tmp.nextMinor = nextMinor[newNodes[pro]];
    }

    // Update filter's .parent and .class
    if (tmp.family == 'u32' && tmp.type == 'filter') {
      var pRuleSplit = tmp.parent_rule.split(/[-:]/);
      tmp.params.parent = pRuleSplit[1] + ':0';
      tmp.params.class = pRuleSplit[1] + ':' + pRuleSplit[2];
    } else if (tmp.params.parent != undefined) {
      var pRuleSplit = tmp.parent_rule.split('-');
      tmp.params.parent = pRuleSplit[1];
    }
  }

  // Task IV: Copy the new qRules to qRules
    qRules = qRulesNew;

    return false;
}


// commands_nic_tree()
//
//   Recursively generates all QDisc, Class and Filter tc() commands beginning with
// the specified node.

function commands_nic_tree(qRuleIdx) {

  // If there're no kid(s) just return. (This really shouldn't happen.)
  if (qRuleIdx == '') { return; }

  // To be clear, any state other than 'active' is ignored for cmd gen.
  if (qRules[qRuleIdx].state != 'active') {
    return;
  }

  // Generate the command for this node.
  commands += commands_rule(qRuleIdx);

  // generate command for each qFilter
  if (qRules[qRuleIdx].qFilters) {
    for (var i=0; i<qRules[qRuleIdx].qFilters.length; i++) {
      kid = qRules[qRuleIdx].qFilters[i];
      if (qRules[kid].state == 'active') { commands += commands_filter(kid); }
    }
  }

  // recurse into qDisc, if found, to generate its command(s)
  if (qRules[qRuleIdx].qDisc != '') {
    kid = qRules[qRuleIdx].qDisc;
    commands_nic_tree(kid);
  }

  // recurse into all child classes to generate their commands
  for (var i in qRules[qRuleIdx].qClasses) {
    var kid = qRules[qRuleIdx].qClasses[i];
    commands_nic_tree(kid);
  }

  return;
}
