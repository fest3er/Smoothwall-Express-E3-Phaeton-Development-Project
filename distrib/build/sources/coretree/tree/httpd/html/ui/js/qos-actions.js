// qos-actions.js
//
// Copyright 2009, Neal P. Murphy. All rights reserved.

// License is granted to use and distribute this software with the
//   free Smoothwall Express firewall system.
// Contact neal.p.murphy@alum.wpi.edu for more information.

// QoS Configuration Actions
//
//   These functions perform the actions requested by the user while
// building a Linux Traffic Control configuration.



// doSet: Set a line item's state to open, closed, active, bypassed,
//   or toggle the state or the view

function doSet(object, action) {
  switch (action) {
    case 'A':
      // Activate the node
      object.state = 'active';
      return;
      break;
    case 'B':
      // Bypass the node
      object.state = 'bypassed';
      return;
      break;
    case 'C':
      // Close the view
      object.view = 'closed';
      return;
      break;
    case 'O':
      // Open the view
      object.view = 'open';
      return;
      break;
    case 'S':
      // Toggle the state
      switch (object.state) {
        case 'active':
          object.state = 'bypassed';
          return;
        case 'bypassed':
          object.state = 'active';
          return;
      }
      break;
    case 'V':
      // Toggle the view
      switch (object.view) {
        case 'open':
          object.view = 'closed';
          return;
        case 'closed':
          object.view = 'open';
          return;
      }
      break;
  }
}


// Toggle a line item open/closed
//
//   Toggle an item (nic/qdisc/class/filter) between opened tree view
// and closed (summary item view only).

function toggleView(qType, qRule, qAction) {
  var tmpObject;
  
  // qAction: C=collapseAll, O=expandAll, V=toggle

  if (arguments.length == 2) {
    qAction = 'V';
  }

  // Get the right object
  switch (qType) {
    case 'root':
      tmpObject = qRoots[qRule];
      doSet (tmpObject, qAction);
      if (qAction != 'V') {
        // Recurse through qDisc, as needed
        toggleView('qR', tmpObject.qDisc, qAction);
      }
      break;
    case 'qR':
      tmpObject = qRules[qRule];
      doSet (tmpObject, qAction);
      // Recurse and do all children if not toggling
      if (qAction != 'V') {
        for (var i=0; i<tmpObject.qClasses.length; i++) {
          // Recurse through classes
          toggleView('qR', tmpObject.qClasses[i], qAction);
        }
        for (var i=0; i<tmpObject.qFilters.length; i++) {
          // Handle filters here; no recursion
          doSet (qRules[tmpObject.qFilters[i]], qAction);
        }
      }
      break;
    case 'qF':
      tmpObject = qRules[qRule];
      // Never recursion for filters
      doSet (tmpObject, qAction);
      break;
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


// Toggle a line item active/bypassed
//
//   Toggle an item (nic/qdisc/class/filter) between active (rule is generated)
// and bypassed (rule is skipped during generation).

function toggleState(qType, qRule, qAction) {
  var tmpObject;
  
  // qAction: A=activateAll, B=bypassAll, S=toggle

  if (arguments.length == 2) {
    qAction = 'S';
  }

  // Get the right object
  switch (qType) {
    case 'root':
      tmpObject = qRoots[qRule];
      doSet (tmpObject, qAction);
      if (qAction != 'S') {
        // Recurse through qDisc, as needed
        toggleState('qR', tmpObject.qDisc, qAction);
      }
      break;
    case 'qR':
      tmpObject = qRules[qRule];
      doSet (tmpObject, qAction);
      // Recurse and do all children if not toggling
      if (qAction != 'S') {
        for (var i=0; i<tmpObject.qClasses.length; i++) {
          // Recurse through classes
          toggleState ('qR', tmpObject.qClasses[i], qAction);
        }
        for (var i=0; i<tmpObject.qFilters.length; i++) {
          // Handle filters here; no recursion
          doSet (qRules[tmpObject.qFilters[i]], qAction);
        }
        // Recurse through sub-qdisc (in case it's classful)
        //if (tmpObject.qdisc !== undefined && tmpObject.qdisc != "") {
        if (tmpObject['qDisc'] != "") {
          toggleState ('qR', tmpObject['qDisc'], qAction);
        }
      }
      break;
    case 'qF':
      tmpObject = qRules[qRule];
      // Never recursion for filters
      doSet (tmpObject, qAction);
      break;
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


// Delete a Rule (lop off its entire tree)
//
//   Delete a QDisc/Class/Filter. That is, remove it from the
// configuration and recursively remove all of its child items.

function deleteRule(qType, qRuleIdx) {
  var i;

  if (qRuleIdx.indexOf('-') == -1) {

    // Is a Root NIC
    //
    // 1. Traverse this node's tree and this node
    if (qRoots[qRuleIdx].qDisc != '') {
      deleteRuleTree('qD', qRoots[qRuleIdx].qDisc);

      // 2. Delete the child QDisc
      delete qRules[qRoots[qRuleIdx].qDisc];
    }

    // 3. Reset items in the Root NIC
    qRoots[qRuleIdx].qDisc = '';
    qRoots[qRuleIdx].view = 'nocfg';
    qRoots[qRuleIdx].state = 'nocfg';
    qRoots[qRuleIdx].nextMajor = '1';

  } else {
    
    // Is a Rule
    //
    // 1. First, clear the parent's pointer to this node
    //

    if ( qRules[qRuleIdx].parent_rule.indexOf('-') == -1) {

      // Parent is a NIC; clear the parent's matching qDisc
      qRoots[qRules[qRuleIdx].parent_rule].qDisc = '';
      qRoots[qRules[qRuleIdx].parent_rule].state = 'nocfg';
      qRoots[qRules[qRuleIdx].parent_rule].view = 'nocfg';
      qRoots[qRules[qRuleIdx].parent_rule].nextMajor = '1';

    } else {

      // Parent is a Rule

      // Check Parent's QDisc
      if (qRules[qRules[qRuleIdx].parent_rule].qDisc==qRuleIdx) {
        // Clear parent's matching QDisc
        qRules[qRules[qRuleIdx].parent_rule].qDisc = '';
      }

      // Check parent's Classes...
      for (i=qRules[qRules[qRuleIdx].parent_rule].qClasses.length-1; i>=0; i--) {
        if (qRules[qRules[qRuleIdx].parent_rule].qClasses[i]==qRuleIdx) {
          // Clear parent's matching Class
          qRules[qRules[qRuleIdx].parent_rule].qClasses.splice(i, 1);
        }
      }

      // Check parent's Filters...
      for (i=qRules[qRules[qRuleIdx].parent_rule].qFilters.length-1; i>=0; i--) {
        if (qRules[qRules[qRuleIdx].parent_rule].qFilters[i]==qRuleIdx) {
          // Clear parent's matching Filter
          qRules[qRules[qRuleIdx].parent_rule].qFilters.splice(i, 1);
        }
      }
    }

    // 2. Traverse this node's tree, then delete this node
    deleteRuleTree(qType, qRuleIdx);
    delete qRules[qRuleIdx];
  }

  // 3. Activate the SAVE button

  // 4. Clear the global HTML accumulator and redisplay the tree
  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


///////////////////////////
////                   ////
////  QDisc Functions  ////
////                   ////
///////////////////////////

// addChangeQDisc()
// Activate the QDisc form
//

function addChangeQDisc(qRuleIdxQ, fType) {
  switch (fType) {
    case 'change':
      // Set the SHOW flag
      qRules[qRuleIdxQ]['showForm'] = fType;
      qRules[qRuleIdxQ].view = 'open';
      break;

    case 'add':
      // When adding, it is the parent's index
      var qSplit = qRuleIdxQ.indexOf('-');

      // figure parent, make new index
      if (qSplit != -1) {
        // is inner QDisc
        var parent_handle = qRuleIdxQ.substring(qSplit+1);
        var nextMajor = qRoots[qRuleIdxQ.substring(0,qSplit)].nextMajor;
        var newRuleIdx = qRuleIdxQ.substring(0, qSplit+1) + nextMajor.toString() +':0';
        // Add the class to the parent's list
        qRules[qRuleIdxQ].qDisc = newRuleIdx;
        qRules[qRuleIdxQ].view = 'open';
      } else {
        // is Root NIC QDisc
        var parent_handle = 'root';
        var newRuleIdx = qRuleIdxQ +'-'+ qRoots[qRuleIdxQ].nextMajor.toString() +':0';
        // Add the class to the parent's list
        qRoots[qRuleIdxQ].qDisc = newRuleIdx;
        qRoots[qRuleIdxQ].view = 'open';
        qRoots[qRuleIdxQ].state = 'active';
      }

      // Create the new qdisc
      qRules[newRuleIdx] = {
        "comment": "NEW",
        "view": "open",
        "state": "nocfg",
        "parent_rule": qRuleIdxQ,
        "pending_action": "",
        "type": "qdisc",
        "family": "",
        "params": {
        },
        "qClasses": new Array(),
        "qFilters": new Array(),
        "qDisc": "",
        "nextMinor": "1",
        "showForm": "add"
      };

      break;
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}
    


// saveQDisc()
// Save the changes to the qdisc

function saveQDisc(qRuleIdx, fType) {

  var thisForm = document.getElementById(qRuleIdx);

  var ruleObj = qRules[qRuleIdx];
  var parObj = qRules[qRuleIdx].params;
  var i;

  for (i=0; i<thisForm.qType.length; i++) {
    var tF = thisForm.qType[i];

    if (tF.checked == true) {
      if (tF.value == 'rHtb') {
        ruleObj['family'] = 'htb';
        ruleObj['comment'] = tF.form.comment.value;
        parObj['qdefault'] = '';
      } else if (tF.value == 'rPfifoFast') {
        ruleObj['family'] = 'pfifofast';
        ruleObj['comment'] = tF.form.comment.value;
        // No configurable parameters!
      } else if (tF.value == 'rBfifo') {
        ruleObj['family'] = 'bfifo';
        ruleObj['comment'] = tF.form.comment.value;
        parObj['limit'] = tF.form.limit.value;
      } else if (tF.value == 'rPfifo') {
        ruleObj['family'] = 'pfifo';
        ruleObj['comment'] = tF.form.comment.value;
        parObj['limit'] = tF.form.limit.value;
      } else if (tF.value == 'rSfq') {
        ruleObj['family'] = 'sfq';
        ruleObj['comment'] = tF.form.comment.value;
        parObj['perturb'] = tF.form.perturb.value;
        parObj['quantum'] = tF.form.quantum.value;
      } else if (tF.value == 'rTbf') {
        ruleObj['family'] = 'tbf';
        ruleObj['comment'] = tF.form.comment.value;
        parObj['limit'] = tF.form.limit.value;
        parObj['latency'] = tF.form.latency.value;
        parObj['burst'] = tF.form.burst.value;
        parObj['mpu'] = tF.form.mpu.value;
        parObj['rate'] = tF.form.rate.value;
        parObj['rateUnit'] = tF.form.rateUnit.value;
      }
    break;
    }
  } 
  delete qRules[qRuleIdx].showForm;

  // Mark as configured if need be
  if (qRules[qRuleIdx].state == 'nocfg') {
    qRules[qRuleIdx].state = 'bypassed';
  }

  // Adding new root qdisc was successsful, so increment nextMajor.
  if (fType == 'add') {
    qdash = qRuleIdx.indexOf('-');
    hdlRoot = qRuleIdx.substring(0,qdash);
    qRoots[hdlRoot].nextMajor++;
  }
  
  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


// dropQDiscForm()
// Erase the form from the display
//

function dropQDiscForm(qRuleIdx, fType) {

  switch (fType) {
    case 'change':
       // Clear the SHOW flag
       delete qRules[qRuleIdx].showForm;
       break;

    case 'add':
      // Delete the NEW qdisc
      if (qRules[qRuleIdx].parent_rule.indexOf('-') == -1) {
        // Is Root NIC
        qRoots[qRules[qRuleIdx].parent_rule].qDisc = '';
        qRoots[qRules[qRuleIdx].parent_rule].view = 'nocfg';
        qRoots[qRules[qRuleIdx].parent_rule].state = 'nocfg';
      } else {
        // Is Inner QDisc
        qRules[qRules[qRuleIdx].parent_rule].qDisc = '';
      }
      delete qRules[qRuleIdx];
      break;
  }

  // Don't change qRoot's nextMajor

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


///////////////////////////
////                   ////
////  Class Functions  ////
////                   ////
///////////////////////////

// addChangeHTBClass()
// Activate the HTB Class form
//

function addChangeHTBClass(qRuleIdxC, fType) {
  switch (fType) {
    case 'change':
      // Set the SHOW flag
      qRules[qRuleIdxC]['showForm'] = fType;
      qRules[qRuleIdxC].view = 'open';
      break;

    case 'add':
      // When adding, it is the parent's index
      var qDash = qRuleIdxC.indexOf('-');
      var qColon = qRuleIdxC.indexOf(':');
      var parent_handle = qRuleIdxC.substring(qDash+1);

      var nextMinor = qRules[qRuleIdxC.substring(0, qColon) +':0'].nextMinor;

      // make new index
      qColon = qRuleIdxC.indexOf(':');
      var newRuleIdx = qRuleIdxC.substring(0, qColon) +':'+ nextMinor.toString();
      qRC = qRuleIdxC.indexOf(':0');

      // Create the new class
      qRules[newRuleIdx] = {
        "comment": "NEW",
        "view": "open",
        "state": "nocfg",
        "parent_rule": qRuleIdxC,
        "pending_action": "",
        "type": "class",
        "family": "htb",
        "params": {
          "parent": parent_handle,
          "priority": "0",
          "rate": "100",
          "rateUnit": "Mbit/s",
          "burstR": "100",
          "burstRUnit": "kB",
          "ceil": "100",
          "ceilUnit": "Mbit/s",
          "burstC": "50",
          "burstCUnit": "kB"
        },
        "qClasses": new Array(),
        "qFilters": new Array(),
        "qDisc": "",
        "showForm": "add",
        "setAsDefault": ""
      };

      // If the new class belongs to a QDisc, clear the ceiling bits
      if (qRC != -1) {
        qRules[newRuleIdx].params.ceil = "";
        qRules[newRuleIdx].params.ceilUnit = "";
        qRules[newRuleIdx].params.burstC = "";
        qRules[newRuleIdx].params.burstCUnit = "";
      }

      // Add the class to the parent's list
      qRules[qRuleIdxC].qClasses.push(newRuleIdx);
      qRules[qRuleIdxC].view = 'open';
      break;
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}
    


// saveHTBClass()
// Save the changes to the class

function saveHTBClass(qRuleIdx, fType) {

  var thisForm = document.getElementById(qRuleIdx);

//
// Check handle vs. name; if changed, delete/add, etc.
//

  qRules[qRuleIdx].comment = thisForm.comment.value;
  qRules[qRuleIdx].params.priority = thisForm.priority.value;
  qRules[qRuleIdx].params.rate = thisForm.rate.value;
  qRules[qRuleIdx].params.rateUnit = thisForm.rateUnit.value;
  qRules[qRuleIdx].params.burstR = thisForm.burstR.value;
  qRules[qRuleIdx].params.burstRUnit = thisForm.burstRUnit.value;
  if (qRules[qRuleIdx].params.parent.indexOf(":0")==-1) {
    qRules[qRuleIdx].params.ceil = thisForm.ceil.value;
    qRules[qRuleIdx].params.ceilUnit = thisForm.ceilUnit.value;
    qRules[qRuleIdx].params.burstC = thisForm.burstC.value;
    qRules[qRuleIdx].params.burstCUnit = thisForm.burstCUnit.value;
  }

  // Set this rule as default?
  if (thisForm.setasdefault.checked) {
    var qcolon = qRuleIdx.indexOf(':');
    qRules[qRuleIdx.substring(0,qcolon+1) +'0'].params.qdefault = qRuleIdx.substring(qcolon+1);
  }

  delete qRules[qRuleIdx].showForm;
  delete qRules[qRuleIdx].setAsDefault;

  // Set as configured, if need be
  if (qRules[qRuleIdx].state == 'nocfg') {
   qRules[qRuleIdx].state = 'bypassed';
  }

  // Adding new root class was successsful, so increment nextMinor.
  if (fType == 'add') {
    qdash = qRuleIdx.indexOf(':');
    hdlQdisc = qRuleIdx.substring(0,qdash) +':0';
    qRules[hdlQdisc].nextMinor++;
  }
  
  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


// dropHTBClassForm()
// Erase the form from the display
//

function dropHTBClassForm(qRuleIdx, fType) {

  switch (fType) {
    case 'change':
       // Clear the SHOW flag
       delete qRules[qRuleIdx].showForm;
       break;

    case 'add':
      // Delete the NEW class
      for (i in qRules[qRules[qRuleIdx].parent_rule].qClasses) {
        if (qRules[qRules[qRuleIdx].parent_rule].qClasses[i] == qRuleIdx) {
          qRules[qRules[qRuleIdx].parent_rule].qClasses.splice(i,1);
          break;
        }
      }
      delete qRules[qRuleIdx];
      break;
  }

  // Don't change qRoot's nextMajor

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


////////////////////////////
////                    ////
////  Filter Functions  ////
////                    ////
////////////////////////////

// addChangeFilter()
// Activate the U32 Filter form
//

function addChangeFilter(qRuleIdxF, fType) {
  switch (fType) {
    case 'change':
      // Set the SHOW flag
      qRules[qRuleIdxF]['showForm'] = fType;
      break;

    case 'add':
      // When adding, it is the parent class' index
      var qRuleIdx = qRuleIdxF;
      var qFilterIdx =  qRules[qRuleIdx].qFilters.length;
      var newRuleIdx = qRuleIdx +'.'+ qFilterIdx;

      // Find parent QDisc handle
      var tmpIdx = qRuleIdx;
      while (true) {
        if (qRules[tmpIdx].type == 'qdisc') break;
        tmpIdx = qRules[tmpIdx].parent_rule;
      }
      qDiv = tmpIdx.indexOf('-');
      parent_qdisc = tmpIdx.substring(qDiv+1);

      // Get class handle
      qDiv = qRuleIdx.indexOf('-');
      parent_class = tmpIdx.substring(qDiv+1);

      // Create the new filter
      qRules[newRuleIdx] = {
        "comment": "NEW",
        "view": "open",
        "state": "bypassed",
        "parent_rule": qRuleIdx,
        "pending_action": "",
        "type": "filter",
        "family": "u32",
        "params": {"protocol": "ip",
                   "parent": parent_qdisc,
                   "class": parent_class,
                   "priority": "1"
        },
        "matches": [],
        "showForm": "add"
      };
      // Add the filter to the parent's list
      qRules[qRuleIdx].qFilters.splice(qFilterIdx, 0, newRuleIdx);
      qRules[qRuleIdx].view = 'open';
      break;
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}
    


// saveFilter()
// Save the changes to the filter

function saveFilter(qRuleIdx, qFilterIdx) {

  var qRuleIdxF = qRuleIdx +'.'+ qFilterIdx;
  var thisForm = document.getElementById(qRuleIdxF);

  qRules[qRuleIdxF].comment = thisForm.comment.value;
  qRules[qRuleIdxF].params.priority = thisForm.priority.value;
  delete qRules[qRuleIdxF].showForm;
  
  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


// dropU32FilterForm()
// Erase the form from the display
//

function dropU32FilterForm(qRuleIdxF, fType) {
  var qSplit = qRuleIdxF.lastIndexOf('.');
  var qRuleIdx = qRuleIdxF.substring(0,qSplit);
  var qFilterIdx = qRuleIdxF.substring(qSplit+1);

  var qF_sh =  qRules[qRuleIdx];

  switch (fType) {
    case 'change':
       // Clear the SHOW flag
       delete qRules[qRuleIdxF].showForm;
       break;

    case 'add':
      // Delete the NEW filter
      qRules[qRuleIdx].qFilters.splice(qFilterIdx, 1);
      delete qRules[qRuleIdxF].showForm;
      break;
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


//////////////////////////////////
////                          ////
////  Match Clause Functions  ////
////                          ////
//////////////////////////////////

// addChangeMatchClause()
// Activate the U32 Match form
//

function addChangeMatchClause(qRuleIdxM, fType) {
  switch (fType) {
    case 'change':
      var qSplit = qRuleIdxM.lastIndexOf('-');
      var qRuleIdx = qRuleIdxM.substring(0,qSplit);
      var qMatchIdx = qRuleIdxM.substring(qSplit+1);
      // Set the SHOW flag
      qRules[qRuleIdx].matches[qMatchIdx]['showForm'] = fType;
      break;

    case 'add':
      qRuleIdx = qRuleIdxM;
      qMatchIdx =  qRules[qRuleIdx].matches.length;
      qRules[qRuleIdx].matches.push({"type":"","value":"","showForm":"add"});
      qRules[qRuleIdx].view = 'open';
      break;
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}



// saveMatchClause()
// Save the changes to the match clause

function saveMatchClause(qRuleIdx, qMatchIdx) {

  var thisForm = document.getElementById(qRuleIdx +'-'+ qMatchIdx);
  var thisObj = {};
  var i;
  var nic = qRuleIdx.split("-");

  for (i=0; i<thisForm.qType.length; i++) {
    var tF = thisForm.qType[i];

    if (tF.checked == true) {
      var a=tF;
      if (tF.value == 'rSaddr') {
        thisObj['type'] = 'saddr';
        thisObj['value'] = tF.form.saddr.value;
      } else if (tF.value == 'rSaddrhost') {
        thisObj['type'] = 'saddrhost';
        thisObj['value'] = '$'+ nic[0] +'_ADDRESS';
      } else if (tF.value == 'rSport') {
        thisObj['type'] = 'sport';
        thisObj['value'] = tF.form.sport.value;
      } else if (tF.value == 'rDaddr') {
        thisObj['type'] = 'daddr';
        thisObj['value'] = tF.form.daddr.value;
      } else if (tF.value == 'rDport') {
        thisObj['type'] = 'dport';
        thisObj['value'] = tF.form.dport.value;
      } else if (tF.value == 'rProtocol') {
        thisObj['type'] = 'protocol';
        thisObj['value'] = tF.form.protocol.value;
      } else if (tF.value == 'rTos') {
        thisObj['type'] = 'tos';
        thisObj['value'] = tF.form.tos.value;
      } else if (tF.value == 'rU8') {
        thisObj['type'] = 'u8';
        thisObj['value'] = tF.form.u8.value;
        if (tF.form.rOffset[0].checked) thisObj[tF.form.rOffset[0].value] = tF.form.offset.value;
        if (tF.form.rOffset[1].checked) thisObj[tF.form.rOffset[1].value] = tF.form.offset.value;
      } else if (tF.value == 'rU16') {
        thisObj['type'] = 'u16';
        thisObj['value'] = tF.form.u16.value;
        if (tF.form.rOffset[0].checked) thisObj[tF.form.rOffset[0].value] = tF.form.offset.value;
        if (tF.form.rOffset[1].checked) thisObj[tF.form.rOffset[1].value] = tF.form.offset.value;
      } else if (tF.value == 'rU32') {
        thisObj['type'] = 'u32';
        thisObj['value'] = tF.form.u32.value;
        if (tF.form.rOffset[0].checked) thisObj[tF.form.rOffset[0].value] = tF.form.offset.value;
        if (tF.form.rOffset[1].checked) thisObj[tF.form.rOffset[1].value] = tF.form.offset.value;
      }
    qRules[qRuleIdx].matches[qMatchIdx] = thisObj;
    break;
    }
  } 
  
  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


// Delete a filter's match clause
//

function deleteMatch(qType, qRule) {
  // Split the index to get the real rule and the match index
  var qDiv = qRule.lastIndexOf('-');
  var qRealRule = qRule.substring(0, qDiv);
  var qMatchIdx = qRule.substring(qDiv+1);
  qRules[qRealRule].matches.splice(qMatchIdx, 1);

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}
    


// dropU32MatchForm()
// Erase the form from the display
//

function dropU32MatchForm(qRuleIdxM, fType) {
  var qSplit = qRuleIdxM.lastIndexOf('-');
  var qRuleIdx = qRuleIdxM.substring(0,qSplit);
  var qMatchIdx = qRuleIdxM.substring(qSplit+1);

  var qF_sh =  qRules[qRuleIdx];
  var qFm_sh =  qRules[qRuleIdx].matches[qMatchIdx];

  switch (fType) {
    case 'change':
      // Clear the SHOW flag
      delete qRules[qRuleIdx].matches[qMatchIdx].showForm;
      break;

    case 'add':
      // Delete the NEW match
      qRules[qRuleIdx].matches.splice(qMatchIdx,1);
      break;
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}



//////////////////////////////////
////                          ////
////  Window Mgmt. Functions  ////
////                          ////
//////////////////////////////////



// applyToolConfig()

function applyToolConfig() {

  var frm = document.ToolConfig;
  var chk, col, i, j, val;
    
  for (j=0; j<9; j++) {
    val = frm.nicNam[j].value;
    nat = frm.nicNATted[j].checked;
    if (val != "") {
      qNics[j] = val;
      if (!qRoots[val]) {
        qRoots[val] = {
          "gui_color": "",
          "view": "nocfg",
          "state": "nocfg",
          "qDisc": "",
          "pending_action": "",
          "NATted": "",
          "nextMajor": "1"
        };
      }
      if (nat)
      {
        qRoots[val].NATted = "1";
      }
      else
      {
        qRoots[val].NATted = "";
      }
      for (i=0; i<frm['Rcolors['+ j +']'].length; i++) {
        chk = frm['Rcolors['+ j +']'][i].checked;
        if (chk) {
          col = frm['Rcolors['+ j +']'][i].value;
          if (qRoots[val]) qRoots[val].gui_color = col;
          break;
        }
      }
    }
  }

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}


// importConfig()

function importConfig() {

  eval(document.SaveConfig.textconfig.value);
    

  changePanel('tree_div', 'worksheetbutton_div');

  // And don't do the default button click
  return false;
}



// changePanel()

function changePanel(panel, button) {

  // Hide the panels
  document.getElementById('tree_div').style.display = 'none';
  document.getElementById('config_div').style.display = 'none';
  document.getElementById('save_div').style.display = 'none';
  document.getElementById('tcgen_div').style.display = 'none';

  // Clear the borders
  document.getElementById('worksheetbutton_div').style.border = 'none';
  document.getElementById('configbutton_div').style.border = 'none';
  document.getElementById('savebutton_div').style.border = 'none';
  document.getElementById('tcgenbutton_div').style.border = 'none';

  // Show the target panel and set the button's border
  document.getElementById(panel).style.display = 'block';
  document.getElementById(button).style.border = '1pt dashed gray';

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}



// clearConfig()

function clearConfig() {

  qNics = [];
  qRoots = {};
  qRules = {};

  // Switch to config panel
  changePanel("config_div", "configbutton_div");

  // And don't do the default button click
  return false;
}



// compactConfig()

function compactConfig() {

  // Unconditionally compact the tree major/minor number space
  reNumberQDiscTree();

  // Switch to worksheet panel
  changePanel("tree_div", "worksheetbutton_div");

  // Redisplay
  UnTip();
  nictree = '';
  display_qNics();

  // And don't do the default button click
  return false;
}
