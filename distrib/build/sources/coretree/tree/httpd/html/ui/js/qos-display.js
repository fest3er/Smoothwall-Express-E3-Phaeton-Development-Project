// qos-display.js
//
// Copyright 2009, Neal P. Murphy. All rights reserved.

// License is granted to use and distribute this software with the
//   free Smoothwall Express firewall system.
// Contact neal.p.murphy@alum.wpi.edu for more information.

// QoS Display Functions
//
//   These functions build and display the LTC configuration items
// in a tree-like fashion. There are also function to display data
// entry forms as requested by the user.



// buildLineItem()
// Given the HTML and other ID stuff, create the specified line of HTML
//

function buildLineItem(qType, qRuleIdx, symbol, text, marginLeft, indentHTML) {
  var background = '';
  var html = '';
  var root_pattern = /root/;
  var rule_pattern = /q[RFL]/;

  var iMarginLeft = marginLeft + 8;
  var pMarginLeft = marginLeft + 36 + 16;
  var pIndentLeft = -pMarginLeft;

  // Set the alternating background and toggle the boolean
  if (bgBool) {
    background = '; background:#efefef';
  }
  bgBool = (bgBool+1) % 2;

  // build the line from a <div> and a <p> element
  html += spaces.substr(0,indentHTML);
  html += '<div id="'+ qType + qRuleIdx;
  html += '" style="margin:0; padding:1pt;'+ background +'">\n';
  if (qRules[qRuleIdx] && qRules[qRuleIdx].parent_rule.indexOf(':0') != -1) {
    // Not a root class, line is normal height
    html += '<p class="lineitem" style="margin:4pt 0 0 '+ pMarginLeft.toString();
  } else {
    // Is a root class, line is larger
    html += '<p class="lineitem" style="margin:0 0 0 '+ pMarginLeft.toString();
  }
  html += 'pt; text-indent:'+ pIndentLeft.toString() +'pt">\n';

  // Include the active/bypassed/nocfg icon
  if ((root_pattern.test(qType) && qRoots[qRuleIdx].state == 'active')  ||
      (rule_pattern.test(qType) && qRules[qRuleIdx].state == 'active')) {
    // Is active
    html += '<img src="/ui/img/active.png" style="margin:0 0 0 0; vertical-align:middle; display:inline"';
    html += ' onclick=\'return toggleState("'+ qType +'", "'+ qRuleIdx +'");\'';
    html += ' title="Click to Bypass">\n';
  } else if ((root_pattern.test(qType) && qRoots[qRuleIdx].state == 'bypassed')  ||
             (rule_pattern.test(qType) && qRules[qRuleIdx].state == 'bypassed')) {
    // Is bypassed
    html += '<img src="/ui/img/not-active.png" style="margin:0 0 0 0; vertical-align:middle; display:inline"';
    html += ' onclick=\'return toggleState("'+ qType +'", "'+ qRuleIdx +'");\'';
    html += ' title="Click to Activate">\n';
  } else if ((root_pattern.test(qType) && qRoots[qRuleIdx].state == 'nocfg')  ||
             (rule_pattern.test(qType) && qRules[qRuleIdx].state == 'nocfg')) {
    // Is not config'd
    html += '<img src="/ui/img/not-config.png" style="margin:0 0 0 0; vertical-align:middle; display:inline"';
    html += ' title="Not Configured!">\n';
  }

  html += spaces.substr(0,indentHTML);

  // If it's a toggleable item, include the onlick; otherwise, just the symbol
  if (symbol == '+' || symbol == '-') {
    if (symbol == '+') {
      html += '<img src="/ui/img/expand.png" style="display:inline; vertical-align:middle; margin:0 0 0 ';
    } else {
      html += '<img src="/ui/img/collapse.png" style="display:inline; vertical-align:middle; margin:0 0 0 ';
    }
    html += iMarginLeft.toString() +'pt;" ';
    html += 'onclick=\'return toggleView("'+ qType +'", "'+ qRuleIdx +'");\'';
    html += ' title="Click to Toggle View">\n';
  } else {
    html += spaces.substr(0,indentHTML+2);
    if (symbol == '|') {
      html += '<img src="/ui/img/continue.png" style="display:inline; vertical-align:middle; margin:0 0 0 ';
    } else {
      html += '<img src="/ui/img/nocfg.png" style="display:inline; vertical-align:middle; margin:0 0 0 ';
    }
    html += iMarginLeft.toString() +'pt;">\n';
  }

  // And include the rest of the line, including the onclick for the context menu
  html += spaces.substr(0,indentHTML+2);
  html += '<span style="vertical-align:middle; margin:0 0 0 4px" onclick='+ "'return " +'itemMenu("'+ qType +'", "'+ qRuleIdx +'");'+ "'>\n";
  html += spaces.substr(0,indentHTML+4);
  html += text +'\n';
  html += spaces.substr(0,indentHTML+2);
  html += '</span>\n';
  html += spaces.substr(0,indentHTML) +'</p>\n';

  // Output the data entry form, if needed
  //   qRoots don't allow change, only qRules can change
  //if (qType.substring(1,2)=='q' && qRules[qRuleIdx].formFlag==1) {
    //html += spaces.substr(0,indentHTML+2) +'<div style="margin:0">\n';
    //html += displayForm(qType, qRuleIdx);
    //html += spaces.substr(0,indentHTML+2) +'</div>\n';
  //}

  html += '</div>\n';

  return html;
}



// display_rule()
//
//   Display the 'open' qdisc or class rule. The 'closed' view is
// handled by the caller.
//

function display_rule(nic, qRule, margin, indent) {
  var item_text = '';
  var html = '';

  var qR_sh = qRules[qRule];
  var qRp_sh = qRules[qRule].params;

  switch (qR_sh.type) {

    // Queueing disciplines
    case 'qdisc':
      switch (qR_sh.family) {
        case '':
          // Not configured
          item_text += qRule +' <i>'+ qR_sh.comment +'</i>';
          item_text += ' ' + boldFace('NOT CONFIGURED');
          html =  buildLineItem('qR', qRule, '-', item_text, margin, indent);
          break;
        case 'htb':
          // Hierarchical Token Bucket (classful)
          item_text += qRule +' <i>'+ qR_sh.comment +'</i>';
          item_text += ' ' + boldFace('HTB');
          if (qRp_sh['qdefault'] != '') {
            var qcolon = qRule.indexOf(':');
            var idx = qRule.substring(0,qcolon+1);
            item_text += ' default:'+ boldFace(idx+qRp_sh['qdefault']);
          }

          // Set which items appear in the menu
          // This should really be done with a common function
          html =  buildLineItem('qR', qRule, '-', item_text, margin, indent);
          break;

        case 'pfifofast':
          // Packet FIFO_FAST (classless); this is the default qdisc; no parameters
          item_text += qRule +' <i>'+ qR_sh.comment +'</i>';
          item_text += ' ' + boldFace('PFIFO_FAST');

          // Again with the menu selection
          html =  buildLineItem('qL', qRule, '|', item_text, margin, indent);
          break;

        case 'pfifo':
          // Packet FIFO (classless) - has some logging/counting capacity
          item_text += qRule +' <i>'+ qR_sh.comment +'</i>';
          item_text += ' ' + boldFace('PFIFO');
          if (qRp_sh['limit'] != '')
            item_text += ' limit:'+ boldFace(qRp_sh['limit'] +' packets');

          // Again with the menu selection
          html =  buildLineItem('qL', qRule, '|', item_text, margin, indent);
          break;

        case 'bfifo':
          // Byte FIFO (classless) - has some logging/counting capacity
          item_text += qRule +' <i>'+ qR_sh.comment +'</i>';
          item_text += ' ' + boldFace('BFIFO');
          if (qRp_sh['limit'] != '')
            item_text += ' limit:'+ boldFace(qRp_sh['limit'] +' bytes');

          // Again with the menu selection
          html =  buildLineItem('qL', qRule, '|', item_text, margin, indent);
          break;

        case 'tbf':
          // Token Bucket Filter (classless)
          item_text += qRule +' <i>'+ qR_sh.comment +'</i>';
          item_text += ' ' + boldFace('TBF');
          if (qRp_sh['limit'] != '')
            item_text += ' limit:'+ boldFace(qRp_sh['limit'] +' bytes');
          if (qRp_sh['latency'] != '')
            item_text += ' latency:'+ boldFace(qRp_sh['latency'] +'ms');
          if (qRp_sh['burst'] != '')
            item_text += ' burst:'+ boldFace(qRp_sh['burst'] +' bytes');
          if (qRp_sh['mpu'] != '')
            item_text += ' mpu:'+ boldFace(qRp_sh['mpu'] +' bytes');
          if (qRp_sh['rate'] != '')
            item_text += ' rate:'+ boldFace(qRp_sh['rate']+qRp_sh['rateUnit']);

          // Again with the menu selection
          html =  buildLineItem('qL', qRule, '|', item_text, margin, indent);
          break;

        case 'sfq':
          // Stochastic Fair Queueing (classless)
          item_text += qRule +' <i>'+ qR_sh.comment +'</i>';
          item_text += ' ' + boldFace('SFQ');
          if (qRp_sh['perturb'] != '')
            item_text += ' perturb:'+ boldFace(qRp_sh['perturb'] +'sec');
          if (qRp_sh['quantum'] != '')
            item_text += ' quantum:'+ boldFace(qRp_sh['quantum'] +' bytes');

          // Again with the menu selection
          html =  buildLineItem('qL', qRule, '|', item_text, margin, indent);
          break;
      }
      if (qRules[qRule].showForm) {
        html += displayQDiscForm(nic, qRule,
                                      qRules[qRule].showForm,
                                      margin+72, indent+4);
      }
      return html;
      break;

    // Classes
    case 'class':
      switch (qR_sh.family) {
        case 'htb':
          item_text += qRule +' <i>'+ qR_sh.comment +'</i>';
          item_text += ' '+ boldFace('HTB');
          if (qRp_sh['priority'] != '')
            item_text += ' priority:'+ boldFace(qRp_sh['priority']);

          if (qRp_sh.parent.indexOf(":0")!=-1 && qRp_sh['rate'] != '') {
            // Parent is root: display the actual rate
            item_text += ' Speed Limit:'+ boldFace(qRp_sh['rate']+qRp_sh['rateUnit']);
          } else {
            // Parent is a subclass: display as %
            item_text += ' Speed Limit:'+ boldFace(qRp_sh['rate']) +'%';
          }

          if (qRp_sh.parent.indexOf(":0")==-1 && qRp_sh['ceil'] != '')
            item_text += ' Courtesy Speed:'+ boldFace(qRp_sh['ceil']) +'%';
          if (qRp_sh['burstR'] != '')
            item_text += ' Burst Size:'+ boldFace(qRp_sh['burstR']+qRp_sh['burstRUnit']);
          if (qRp_sh.parent.indexOf(":0")==-1 && qRp_sh['burstC'] != '')
            item_text += ' Courtesy Burst Size:'+ boldFace(qRp_sh['burstC']+qRp_sh['burstCUnit']);

          var allocatedBandwidth = 0;
          var qClass, classCount=0;
          var qClasses = qRules[qRule].qClasses;
          for (var i in qClasses) {
            // A spot of shorthand
            classCount++;
            qClass = qR_sh.qClasses[i];
            allocatedBandwidth += Number(qRules[qClass].params.rate);
          }
          if (classCount>0) {
            if (allocatedBandwidth>=0 && allocatedBandwidth<=100) {
              item_text += ' Allocated Speed:'+ boldFace(allocatedBandwidth) +'%';
            } else {
              item_text += ' Allocated Speed:';
              item_text += '(<span style="font-size:1.3em; color:red; text-decoration:blink"><i>';
              item_text += boldFace(allocatedBandwidth);
              item_text += '</i></span>)%';
            }
          }

          // Again with the menu selection
          html = buildLineItem('qR', qRule, '-', item_text, margin, indent);
          if (qRules[qRule].showForm) {
            html += displayHTBClassForm(nic, qRule,
                                             qRules[qRule].showForm,
                                             margin+72, indent+4);
          }
          return html;
          break;
      }
      break;
  }
}



// display_filter()
//
//   Display the filter's details. Filters don't get any special
// treatment; they just tag along after their parent.
//

function display_filter(nic, qFilter, margin, indent) {
  var html = '';
  var ret_html = '';
  var i, qFm_sh, qsplit;
  var showForm = 0;

  var qF_sh =  qRules[qFilter].params;

  // Build the filter
  if (qRules[qFilter].family == 'u32') {
    html += '<i>'+ qRules[qFilter].comment +'</i> '+ boldFace('U32');
    if (qF_sh['protocol'] != '') html += ' protocol:'+ boldFace(qF_sh['protocol']);
    if (qF_sh['parent'] != '') html += ' parent:'+ boldFace(qF_sh['parent']);
    if (qF_sh['priority'] != '') html += ' priority:'+ boldFace(qF_sh['priority']);
    if (qRules[qFilter].view == 'closed') {
      ret_html += buildLineItem ("qF", qFilter, "+", html, margin, indent);
    } else {
      ret_html += buildLineItem ("qF", qFilter, "-", html, margin, indent);
      if (qRules[qFilter].showForm) {
        ret_html += displayU32FilterForm(nic, qFilter,
                                         qRules[qFilter].showForm,
                                         margin+72, indent+4);
      }
    
      // Build the matches
      html = '';
      for (i in qRules[qFilter].matches) {
        // A spot of shorthand
        qFm_sh = qRules[qFilter].matches[i];

        // Handle the type of match
        switch (qFm_sh.type) {
          case "":
            html = '<i>NEW</i>';
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
          case "saddr":
            html = 'SrcAddr';
            html += '&nbsp;'+ boldFace(qFm_sh['value']);
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
          case "saddrhost":
            html = 'SrcAddrSelf';
            html += '&nbsp;'+ boldFace(qFm_sh['value']);
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
          case "sport":
            html = 'SrcPort';
            html += '&nbsp;'+ boldFace(qFm_sh['value']);
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
          case "daddr":
            html = 'DstAddr';
            html += '&nbsp;'+ boldFace(qFm_sh['value']);
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
          case "dport":
            html = 'DstPort';
            html += '&nbsp;'+ boldFace(qFm_sh['value']);
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
          case "protocol":
            html = 'Protocol';
            html += '&nbsp;'+ boldFace(qFm_sh['value']);
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
          case "tos":
            html = 'ToS';
            html += '&nbsp;'+ boldFace(qFm_sh['value']);
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
          case "u8":
          case "u16":
          case "u32":
            html = 'U'+ qFm_sh.type.substring(1);
            html += '&nbsp;val/msk:'+ boldFace(qFm_sh['value']); 
            if (qFm_sh.offset) { html += '&nbsp;offset:'+ boldFace(qFm_sh['offset']); }
            if (qFm_sh.nexthdr) { html += '&nbsp;nexthdroffset:'+ boldFace(qFm_sh['nexthdr']); }
            html += '\n';
            ret_html += buildLineItem ("qM", qFilter +'-'+ i.toString(), "|", html, margin+36, indent+4);
            break;
        }
        if (qFm_sh.showForm) {
          ret_html += displayU32MatchForm(nic, qFilter +'-'+ i.toString(),
                                          qFm_sh.showForm,
                                          margin+72, indent+4);
        }
      }
    }
    return ret_html;
  }
}



// display_qNics()
//
//   The top of the tree display path. Display all included NICs. If the
// NIC's state is 'open', reecursively display them in a tree-like manner.

function display_qNics() {
  var nic = '';
  var kid = '';
  var tmp, tree_div, nicNAT;

  command_qNics();
  displayConfigDiv();
  displaySaveDiv();

  // Open the div for all the trees
  nictree += '<div style="margin:0">\n';

  // Loop through all the specified NICs; this should be generated by CGI
  for (var i=0; i<qNics.length; i++) {
    nic = qNics[i];
    bgBool = 0;
    var indentHTML = 2;
    var marginLeft = 0;

    // open div for NIC with color
    nictree += spaces.substr(0,indentHTML);
    nictree += '<div style="margin:6pt 0 0 0; color:#';
    nictree += qColors[qRoots[nic].gui_color] +'; font-family:sans">\n';

    // Show if NATted
    if (qRoots[qNics[i]].NATted == '1')
    {
      nicNAT = nic +' <i>(NAT)</i>';
    }
    else
    {
      nicNAT = nic;
    }

    // Handle the NIC, be it open, closed or unconfigured
    switch (qRoots[nic].view) {
      case 'open':
        // If open, display with '-' and show kid tree
        nictree += buildLineItem('root', nic, '-', nicNAT, marginLeft, indentHTML+2);

        // A NIC has only a qdisc, so display it.
        kid = qRoots[nic].qDisc;
        display_nic_tree(nic, kid, marginLeft+18, indentHTML+4);
        break;

      case 'closed':
        // If closed, display name with '+'
        nictree += buildLineItem('root', nic, '+', nicNAT, marginLeft, indentHTML);
        break;

      case 'nocfg':
        // If not configured, display name with 'x'
        nictree += buildLineItem('root', nic, 'x', nicNAT, marginLeft, indentHTML);
        break;
    }
    // Close the NIC's div
    nictree += spaces.substr(0,indentHTML) +'</div>\n';
  }

  // Close the tree's div, get the object, and store the HTML
  nictree += '</div>\n';
  tree_div = document.getElementById('tree_div');
  tree_div.innerHTML = nictree;
  if ((tmp = document.getElementById('FocusMe'))) {
    tmp.scrollIntoView(true);
    window.scrollBy(0, -100);
  }

  nictree = '';

}


// buildFormTableRow()
// Build a form's table row
//

function buildFormTableRow(colspan, label, field, comment) {
  var html = '    <tr style="vertical-align:baseline">';
  if (!colspan) {
    html += '      <td style="text-align:right; padding-right:4pt; width:140px">';
    html += '        <p class="lineitem" style="margin:0">'+ label +'</p>';
    html += '      </td>';
    html += '      <td style="text-align:left; padding-left:4pt; width:400px">';
    html += '        <p class="lineitem" style="margin:0">';
    html += '          '+ field;
    if (comment) {
      html += '&nbsp;'+ comment;
    }
    html += '        </p>';
    html += '      </td>';

  } else {

    html += '      <td colspan="'+ colspan.toString() +'" style="text-align:center">';
    html += '        <p class="lineitem">'+ label +'</p>';
    html += '      </td>';
  }
  html += '    </tr>';

  return html;
}


// displayQDiscForm()
// Given the type and index, display the proper data entry form
//

function displayQDiscForm(qType, qRuleIdxQ, fType, margin, indent, redisplay) {
  var htbChecked = '';
  var pfifoFastChecked = '';
  var bfifoChecked = '';
  var pfifoChecked = '';
  var sfqChecked = '';
  var tbfChecked = '';

  var html = '';
  var label, field, comment, preSelect;

  var qRuleIdx = qRuleIdxQ;

  var qR_sh =  qRules[qRuleIdx];
  var qRp_sh =  qRules[qRuleIdx].params;

  // Open the form
  html = '';

  html += '<form id="'+ qRuleIdx +'">';
  html += '  <table cellpadding="0" border="0" style="margin:4pt 0 0 ';
  html += margin.toString() +'pt; border:1pt solid black; padding:4px">';

  // Toss in the heading
  switch (fType) {
    case 'add': label = '<b>Add QDisc</b>'; break;
    case 'change': label = '<b>Change QDisc</b>'; break;
  }
  html += buildFormTableRow(2, label);

  // Which showeth we as selected?
  if (redisplay) {
    // the one that was clicked
    preSelect = redisplay;
  } else if (fType == 'change') {
    // the one in the config
    preSelect = qR_sh.family;
  } else {
    // none are selected
    preSelect = '';
  }
  switch (preSelect) {
    case       'htb': htbChecked = ' checked'; break;
    case 'pfifofast': pfifoFastChecked = ' checked'; break;
    case     'bfifo': bfifoChecked = ' checked'; break;
    case     'pfifo': pfifoChecked = ' checked'; break;
    case       'sfq': sfqChecked = ' checked'; break;
    case       'tbf': tbfChecked = ' checked'; break;
    default: break;
  }

  // Place the match type radio buttons
  // HTB
  label  = 'Type:';
  field  = '<input id="FocusMe" type="radio" name="qType" value="rHtb"';
  field += htbChecked +' style="margin-left:8pt"';
  field += ' size=15';
  field += ' onclick=\'return displayQDiscForm("'+ qType +'", "'+ qRuleIdxQ;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "htb");\'>HTB';

  // PFIFO_FAST
  field += '<input type="radio" name="qType" value="rPfifoFast"';
  field += pfifoFastChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayQDiscForm("'+ qType +'", "'+ qRuleIdxQ;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "pfifofast");\'>PFIFO_FAST';

  // BFIFO
  field += '<input type="radio" name="qType" value="rBfifo"';
  field += bfifoChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayQDiscForm("'+ qType +'", "'+ qRuleIdxQ;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "bfifo");\'>BFIFO';
 
  // PFIFO
  field += '<input type="radio" name="qType" value="rPfifo"';
  field += pfifoChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayQDiscForm("'+ qType +'", "'+ qRuleIdxQ;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "pfifo");\'>PFIFO';
 
  // SFQ
  field += '<br>';
  field += '<input type="radio" name="qType" value="rSfq"';
  field += sfqChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayQDiscForm("'+ qType +'", "'+ qRuleIdxQ;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "sfq");\'>SFQ';

  // TBF
  field += '<input type="radio" name="qType" value="rTbf"';
  field += tbfChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayQDiscForm("'+ qType +'", "'+ qRuleIdxQ;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "tbf");\'>TBF';
  html += buildFormTableRow(null, label, field);

  // Get the handle
  //label = 'QDisc Handle:';
  //field = '<input class="qForm" type="text" name="qdiscid" value="';
  //qdash = qRuleIdx.indexOf('-');
  //qcolon = qRuleIdx.indexOf(':');
  //field += qRuleIdx.substring(qdash+1, qcolon);
  //field += '" size=5>';
  //comment = '1-fffe <b><i>and</i></b> unused in this NIC\'s tree';
  //html += buildFormTableRow(null, label, field, comment);

  // Display the respective entry field(s)
  if (htbChecked != '') {
    // HTB
    label = 'Comment:';
    field = '<input class="qForm" type="text" name="comment" value="';
    if (qR_sh.comment) field += qR_sh.comment;
    field += '" size=25>';
    comment = '';
    html += buildFormTableRow(null, label, field, comment);

    //label = 'Default:';
    //field = '<input class="qForm" type="text" name="qdefault" value="';
    //if (qRp_sh.qdefault) field += qRp_sh.qdefault;
    //field += '" size=5>';
    //comment = 'Default class\' minor #';
    //html += buildFormTableRow(null, label, field, comment);

  } else if (pfifoFastChecked != '') {
    // PFIFO_FAST
    label = 'Comment:';
    field = '<input class="qForm" type="text" name="comment" value="';
    if (qR_sh.comment) field += qR_sh.comment;
    field += '" size=55>';
    comment = '';
    html += buildFormTableRow(null, label, field, comment);

    label = 'PFIFO_FAST has no configurable parameters';
    html += buildFormTableRow(2, label);

  } else if (bfifoChecked != '') {
    // BFIFO
    label = 'Comment:';
    field = '<input class="qForm" type="text" name="comment" value="';
    if (qR_sh.comment) field += qR_sh.comment;
    field += '" size=55>';
    comment = '';
    html += buildFormTableRow(null, label, field, comment);

    label = 'Limit:';
    field = '<input class="qForm" type="text" name="limit" value="';
    if (qRp_sh.limit) field += qRp_sh.limit;
    field += '" size=15>';
    comment = 'bytes';
    html += buildFormTableRow(null, label, field, comment);

  } else if (pfifoChecked != '') {
    // PFIFO
    label = 'Comment:';
    field = '<input class="qForm" type="text" name="comment" value="';
    if (qR_sh.comment) field += qR_sh.comment;
    field += '" size=55>';
    comment = '';
    html += buildFormTableRow(null, label, field, comment);

    label = 'Limit:';
    field = '<input class="qForm" type="text" name="limit" value="';
    if (qRp_sh.limit) field += qRp_sh.limit;
    field += '" size=15>';
    comment = 'packets';
    html += buildFormTableRow(null, label, field, comment);

  } else if (sfqChecked != '') {
    // SFQ
    label = 'Comment:';
    field = '<input class="qForm" type="text" name="comment" value="';
    if (qR_sh.comment) field += qR_sh.comment;
    field += '" size=55>';
    comment = '';
    html += buildFormTableRow(null, label, field, comment);

    label = 'Perturb:';
    field = '<input class="qForm" type="text" name="perturb" value="';
    if (qRp_sh.perturb) field += qRp_sh.perturb;
    field += '" size=5>';
    comment = 'sec';
    html += buildFormTableRow(null, label, field, comment);
    label = 'Quantum:';
    field = '<input class="qForm" type="text" name="quantum" value="';
    if (qRp_sh.quantum) field += qRp_sh.quantum;
    field += '" size=5>';
    comment = 'bytes';
    html += buildFormTableRow(null, label, field, comment);

  } else if (tbfChecked != '') {
    // TBF
    label = 'Comment:';
    field = '<input class="qForm" type="text" name="comment" value="';
    if (qR_sh.comment) field += qR_sh.comment;
    field += '" size=55>';
    comment = '';
    html += buildFormTableRow(null, label, field, comment);

    label = 'Limit:';
    field = '<input class="qForm" type="text" name="limit" value="';
    if (qRp_sh.limit) field += qRp_sh.limit;
    field += '" size=10>';
    comment = 'bytes';
    html += buildFormTableRow(null, label, field, comment);
    label = 'Latency:';
    field = '<input class="qForm" type="text" name="latency" value="';
    if (qRp_sh.latency) field += qRp_sh.latency;
    field += '" size=10>';
    comment = 'ms';
    html += buildFormTableRow(null, label, field, comment);
    label = 'Burst:';
    field = '<input class="qForm" type="text" name="burst" value="';
    if (qRp_sh.burst) field += qRp_sh.burst;
    field += '" size=10>';
    comment = 'bytes';
    html += buildFormTableRow(null, label, field, comment);
    label = 'mpu:';
    field = '<input class="qForm" type="text" name="mpu" value="';
    if (qRp_sh.mpu) field += qRp_sh.mpu;
    field += '" size=10>';
    comment = 'bytes';
    html += buildFormTableRow(null, label, field, comment);
    label = 'Rate:';
    field = '<input class="qForm" type="text" name="rate" value="';
    if (qRp_sh.rate) field += qRp_sh.rate;
    field += '" size=10>';
    field += '<select class="qForm" name="rateUnit">';
    field += '<option value="';
    if (qRp_sh.rateUnit) {
      field += qRp_sh.rateUnit +'">'+ qRp_sh.rateUnit +'</option>';
    } else {
      field += 'Mbit/s">Mbit/s</option>';
    }
    field += '<option value="bit/s">bits/s</option>';
    field += '<option value="kbit/s">kbit/s</option>';
    field += '<option value="Mbit/s">Mbit/s</option>';
    field += '</select>';
    comment = '';
    html += buildFormTableRow(null, label, field, comment);
  }

  // Now insert the form controls
  html += '    <tr>';
  html += '      <td colspan="2">';
  html += '        <p class="lineitem" style="margin:8pt; text-align:center">';
  html += '          <input class="qForm" type="submit" name="Save" value="Save"';
  html += '                 onclick=\'return saveQDisc("'+ qRuleIdx +'", "'+ fType +'");\'>';
  html += '          <input class="qForm" type="submit" name="cancel" value="Cancel"';
  html += '                 onclick=\'return dropQDiscForm("'+ qRuleIdxQ +'", "'+ fType +'");\'>';
  html += '        </p>';
  html += '      </td>';
  html += '    </tr>';
  html += '  </table>';
  html += '</form>';

  if (!redisplay) {
    // Only include the div if it's a brandy-new form
    return '<div id="qDiscForm'+ qRuleIdx +'">'+ html +'</div>';

  } else {

    // Redisplaying, so replace the form and return false
    document.getElementById('qDiscForm'+ qRuleIdx).innerHTML = html;
    return false;
  }
}


// displayHTBClassForm()
// Given the type and index, display the proper data entry form
//

function displayHTBClassForm(qType, qRuleIdx, fType, margin, indent) {
  var html = '';
  var label, field, comment;

  var qF_sh =  qRules[qRuleIdx];
  var qFp_sh =  qRules[qRuleIdx].params;
  var qDiv = qRuleIdx.indexOf(':NEW');

  // Is parent a QDisc?
 // var qRC = qRuleIdx.indexOf(':0');
  var qRC = qF_sh.parent_rule.indexOf(':0');

  // Open the form
  html = '';

  html += '<form id="'+ qRuleIdx +'">';
  html += '  <table cellpadding="0" border="0" style="margin:4pt 0 0 ';
  html += margin.toString() +'pt; border:1pt solid black; padding:4px">';

  // Toss in the heading
  switch (fType) {
    case 'add': label = '<b>Add Class</b>'; break;
    case 'change': label = '<b>Change Class</b>'; break;
  }
  html += buildFormTableRow(2, label);

  // Display the comment entry field
  label = 'Comment:';
  field = '<input id="FocusMe" class="qForm" type="text" name="comment" value="';
  if (qF_sh.comment) field += qF_sh.comment;
  field += '" size=20>';
  comment = 'Terse description of class';
  html += buildFormTableRow(null, label, field, comment);

  // Display the class ID
  //label = 'Class ID:';
  //field = '<input class="qForm" type="text" name="classid" value="';
  //qcolon = qRuleIdx.indexOf(':');
  //field += qRuleIdx.substring(qcolon+1);
  //field += '" size=5>';
  //comment = '1-fffe <b><i>and</i></b> unused in this QDisc\'s tree';
  //html += buildFormTableRow(null, label, field, comment);

  // Display the rate entry field
  if (qRC != -1) {
    // Is root class, display the rate unit menu
    label = 'Link Bandwidth:';
    field = '<input class="qForm" type="text" name="rate" value="';
    if (qFp_sh.rate) field += qFp_sh.rate;
    field += '" size=5>';

    field += '<select class="qForm" name="rateUnit">';
    field += '<option value="'+ qFp_sh.rateUnit +'">'+ qFp_sh.rateUnit +'</option>';
    field += '<option value="bit/s">bits/s</option>';
    field += '<option value="kbit/s">kbit/s</option>';
    field += '<option value="Mbit/s">Mbit/s</option>';
    field += '</select>';
    comment = 'Allowed bit rate (k=10^3; M=10^6)';
  } else {
    label = 'Speed Limit:';
    field = '<input class="qForm" type="text" name="rate" value="';
    if (qFp_sh.rate) field += qFp_sh.rate;
    field += '" size=5>';
    field += '<input type="hidden" name="rateUnit" value="">';
    comment = '% of parent class\' speed';
  }
  html += buildFormTableRow(null, label, field, comment);
    // is subclass, display as percentage
  // Display the rate bucket size entry field
  label = 'Burst Buffer:';
  field = '<input class="qForm" type="text" name="burstR" value="';
  if (qFp_sh.burstR) field += qFp_sh.burstR;
  field += '" size=5>';
  // Display the rate bucket size unit entry field
  field += '<select class="qForm" name="burstRUnit">';
  field += '<option value="'+ qFp_sh.burstRUnit +'">'+ qFp_sh.burstRUnit +'</option>';
  field += '<option value="Bytes">Bytes</option>';
  field += '<option value="kB">kB</option>';
  field += '</select>';
  comment = 'Size of \'rate\' bucket (k=10^3)';
  html += buildFormTableRow(null, label, field, comment);

  if (qRC == -1) {
    // Not a root class, so show the ceiling stuff

    // Display the ceiling entry field
    label = 'Courtesy Speed:';
    field = '<input class="qForm" type="text" name="ceil" value="';
    if (qFp_sh.ceil) field += qFp_sh.ceil;
    field += '" size=5>';
    field += '<input type="hidden" name="ceilUnit" value="">';
    comment = '% of root class\' speed';
    html += buildFormTableRow(null, label, field, comment);
  
    // Display the ceiling bucket size entry field
    label = 'Courtesy Buffer Size:';
    field = '<input class="qForm" type="text" name="burstC" value="';
    if (qFp_sh.burstC) field += qFp_sh.burstC;
    field += '" size=5>';
    // Display the ceiling bucket size unit entry field
    field += '<select class="qForm" name="burstCUnit">';
    field += '<option value="'+ qFp_sh.burstCUnit +'">'+ qFp_sh.burstCUnit +'</option>';
    field += '<option value="Bytes">Bytes</option>';
    field += '<option value="kB">kB</option>';
    field += '</select>';
    comment = 'Size of \'ceiling\' bucket (k=10^3)';
    html += buildFormTableRow(null, label, field, comment);
  } else {
      var hidden_field = '<input type="hidden" name="ceil" value="">';
      hidden_field += '<input type="hidden" name="ceilUnit" value="">';
      hidden_field += '<input type="hidden" name="burstC" value="">';
      hidden_field += '<input type="hidden" name="burstCUnit" value="">';
  }

  // Display the priority entry field
  label = 'Priority:';
  field = '<input class="qForm" type="text" name="priority" value="';
  if (qFp_sh.priority) field += qFp_sh.priority;
  field += '" size=5>';
  comment = '(lower # = greater priority)';
  if (hidden_field) {
    comment += hidden_field;
  }
  html += buildFormTableRow(null, label, field, comment);

  // Display the setAsDefault checkbox
  label = 'Set As Default:';
  field = '<input class="qForm" type="checkbox" name="setasdefault">';
  comment = '(Make this the root\'s default class)';
  if (hidden_field) {
    comment += hidden_field;
  }
  html += buildFormTableRow(null, label, field, comment);

  // Now insert the form controls
  html += '    <tr>';
  html += '      <td colspan="2">';
  html += '        <p class="lineitem" style="margin:8pt; text-align:center">';
  html += '          <input class="qForm" type="submit" name="Save" value="Save"';
  html += '                 onclick=\'return saveHTBClass("'+ qRuleIdx +'", "'+ fType +'");\'>';
  html += '          <input class="qForm" type="submit" name="cancel" value="Cancel"';
  html += '                 onclick=\'return dropHTBClassForm("'+ qRuleIdx +'", "'+ fType +'");\'>';
  html += '        </p>';
  html += '      </td>';
  html += '    </tr>';
  html += '  </table>';
  html += '</form>';

  // Include the div on the brandy-new form
    return '<div id="HTBClassForm'+ qRuleIdx +'">'+ html +'</div>';
}


// displayU32FilterForm()
// Given the type and index, display the proper data entry form
//

function displayU32FilterForm(qType, qRuleIdxF, fType, margin, indent) {
  var html = '';
  var label, field, comment;

  var qSplit = qRuleIdxF.lastIndexOf('.');
  var qRuleIdx = qRuleIdxF.substring(0,qSplit);
  var qFilterIdx = qRuleIdxF.substring(qSplit+1);

  var qF_sh =  qRules[qRuleIdxF];
  var qFp_sh =  qRules[qRuleIdxF].params;

  // Open the form
  html = '';

  html += '<form id="'+ qRuleIdxF +'">';
  html += '  <table cellpadding="0" border="0" style="margin:4pt 0 0 ';
  html += margin.toString() +'pt; border:1pt solid black; padding:4px">';

  // Toss in the heading
  switch (fType) {
    case 'add': label = '<b>Add Filter</b>'; break;
    case 'change': label = '<b>Change Filter</b>'; break;
  }
  html += buildFormTableRow(2, label);

  // Display the comment entry field
  label = 'Comment:';
  field = '<input id="FocusMe" class="qForm" type="text" name="comment" value="';
  if (qF_sh.comment) field += qF_sh.comment;
  field += '" size=20>';
  comment = 'Terse description of filter';
  html += buildFormTableRow(null, label, field, comment);

  // Display the priority entry field
  label = 'Priority:';
  field = '<input class="qForm" type="text" name="priority" value="';
  if (qFp_sh.priority) field += qFp_sh.priority;
  field += '" size=5>';
  comment = '(lower # = greater priority)';
  html += buildFormTableRow(null, label, field, comment);

  // Now insert the form controls
  html += '    <tr>';
  html += '      <td colspan="2">';
  html += '        <p class="lineitem" style="margin:8pt; text-align:center">';
  html += '          <input class="qForm" type="submit" name="Save" value="Save"';
  html += '                 onclick=\'return saveFilter("'+ qRuleIdx +'", "'+ qFilterIdx +'");\'>';
  html += '          <input class="qForm" type="submit" name="cancel" value="Cancel"';
  html += '                 onclick=\'return dropU32FilterForm("'+ qRuleIdxF +'", "'+ fType +'");\'>';
  html += '        </p>';
  html += '      </td>';
  html += '    </tr>';
  html += '  </table>';
  html += '</form>';

  // Include the div on the brandy-new form
    return '<div id="u32FilterForm'+ qRuleIdxF +'">'+ html +'</div>';
}


// displayU32MatchForm()
// Given the type and index, display the proper data entry form
//

function displayU32MatchForm(qType, qRuleIdxM, fType, margin, indent, redisplay) {
  var saddrhostChecked = '';
  var saddrChecked = '';
  var daddrChecked = '';
  var sportChecked = '';
  var dportChecked = '';
  var protocolChecked = '';
  var tosChecked = '';
  var u8Checked = '';
  var u16Checked = '';
  var u32Checked = '';
  var offsetChecked = '';
  var nexthdrChecked = '';
  var html = '';
  var preSelect, label, field, comment;

  var qNicSplit = qRuleIdxM.indexOf('-');
  var qNic = qRuleIdxM.substring(0,qNicSplit);
  var disabled;

  var qSplit = qRuleIdxM.lastIndexOf('-');
  var qRuleIdx = qRuleIdxM.substring(0,qSplit);
  var qMatchIdx = qRuleIdxM.substring(qSplit+1);

  var qF_sh =  qRules[qRuleIdx];
  var qFm_sh =  qRules[qRuleIdx].matches[qMatchIdx];

  // Open the form
  html = '';

  html += '<form id="'+ qRuleIdxM +'">';
  html += '  <table cellpadding="0" border="0" style="margin:4pt 0 0 ';
  html += margin.toString() +'pt; border:1pt solid black; padding:0">';

  // Toss in the heading
  switch (fType) {
    case 'add': label = '<b>Add Match</b>'; break;
    case 'change': label = '<b>Change Match Clause</b>'; break;
  }
  html += buildFormTableRow(2, label);

  // Which showeth we as selected?
  if (redisplay) {
    // the one that was clicked
    preSelect = redisplay;
  } else if (fType == 'change') {
    // the one in the config
    preSelect = qFm_sh.type;
  } else {
    // none are selected
    preSelect = '';
  }
  switch (preSelect) {
    case 'saddrhost': saddrhostChecked = ' checked'; break;
    case     'saddr': saddrChecked = ' checked'; break;
    case     'sport': sportChecked = ' checked'; break;
    case     'daddr': daddrChecked = ' checked'; break;
    case     'dport': dportChecked = ' checked'; break;
    case  'protocol': protocolChecked = ' checked'; break;
    case       'tos': tosChecked = ' checked'; break;
    case        'u8': u8Checked = ' checked'; break;
    case       'u16': u16Checked = ' checked'; break;
    case       'u32': u32Checked = ' checked'; break;
    default: break;
  }

  // Place the match type radio buttons
  label  = 'Type:';

  // NATted or not, treat even the source options as though they are usable
  //   U32 items. We'll treat them differently when generating the TC script.
  // The code below was an experiment. I think it should suffice to generate
  //   IPTables CLASSIFY commands for source addrs and ports 'behind' NATted NICs.
  disabled = '';

  // If the NIC is NATted, disable the 'source' options, since they are NATted
  //   into oblivion.
  //if (qRoots[qNic].NATted == '1')
  //{
    //disabled = ' disabled="disabled"';
  //}
  //else
  //{
    //disabled = '';
  //}

  // Src Addr
  field  = '<input id="FocusMe" type="radio" name="qType" value="rSaddr"'+ disabled;
  field += saddrChecked +' style="margin-left:8pt"';
  field += ' size=15';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "saddr");\'>&nbsp;S_Addr\n';

  // Src Addr (Self/Host)
  field += '<input type="radio" name="qType" value="rSaddrhost"'+ disabled;
  field += saddrhostChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "saddrhost");\'>&nbsp;S_Addr_Self\n';

  // Src Port
  field += '<input type="radio" name="qType" value="rSport"'+ disabled;
  field += sportChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "sport");\'>&nbsp;S_Port\n';

  // Dest. Addr
  field += '<br>';
  field += '<input type="radio" name="qType" value="rDaddr"';
  field += daddrChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "daddr");\'>&nbsp;D_Addr\n';
 
  // Dest. Port
  field += '<input type="radio" name="qType" value="rDport"';
  field += dportChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "dport");\'>&nbsp;D_Port\n';

  // Protocol
  field += '<br>';
  field += '<input type="radio" name="qType" value="rProtocol"';
  field += protocolChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "protocol");\'>&nbsp;Protocol\n';

  // Type of Service
  field += '<input type="radio" name="qType" value="rTos"';
  field += tosChecked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "tos");\'>&nbsp;ToS\n';

  // U8 binary
  field += '<input type="radio" name="qType" value="rU8"';
  field += u8Checked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "u8");\'>&nbsp;U8\n';

  // U16 binary
  field += '<input type="radio" name="qType" value="rU16"';
  field += u16Checked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "u16");\'>&nbsp;U16\n';

  // U32 binary
  field += '<input type="radio" name="qType" value="rU32"';
  field += u32Checked +' style="margin-left:8pt"';
  field += ' onclick=\'return displayU32MatchForm("'+ qType +'", "'+ qRuleIdxM;
  field += '", "'+ fType +'", "'+ margin +'", "'+ indent +'", "u32");\'>&nbsp;U32\n';
  html += buildFormTableRow(null, label, field);

  // Display the respective entry field
  if (saddrChecked != '') {
    // Src Addr
    label = 'Source Address/Net:';
    field = '<input class="qForm" type="text" name="saddr" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'a.b.c.d/e;<br>(Ex. 1.2.3.4/24)';
    html += buildFormTableRow(null, label, field, comment);

  } else if (saddrhostChecked != '') {
    // Src Addr (Self/Host)
    label = 'Source Address:';
    field = '<input type="hidden" name="saddrhost" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '">';
    comment = 'The firewall'+"'"+'s own address will be used';
    html += buildFormTableRow(null, label, field, comment);

  } else if (sportChecked != '') {
    // Src Port
    label = 'Source Port:';
    field = '<input class="qForm" type="text" name="sport" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'dec/hex<br>(port/mask; ex: 45/ffff)';
    html += buildFormTableRow(null, label, field, comment);

  } else if (daddrChecked != '') {
    // Dest. Addr
    label = 'Dest. Address/Net:';
    field = '<input class="qForm" type="text" name="daddr" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'a.b.c.d/e;<br>(Ex. 1.2.3.4/24)';
    html += buildFormTableRow(null, label, field, comment);

  } else if (dportChecked != '') {
    // Dest. Port
    label = 'Dest. Port:';
    field = '<input class="qForm" type="text" name="dport" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'dec/hex<br>(port/mask; ex: 45/ffff)';
    html += buildFormTableRow(null, label, field, comment);

  } else if (protocolChecked != '') {
    // Protocol
    label = 'Protocol/Mask';
    field = '<input class="qForm" type="text" name="protocol" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'dec/hex (ex. 5/ff)';
    html += buildFormTableRow(null, label, field, comment);

  } else if (tosChecked != '') {
    // Type of Service
    label = 'ToS/Mask';
    field = '<input class="qForm" type="text" name="tos" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'dec/hex (ex. 5/ff)';
    html += buildFormTableRow(null, label, field, comment);

  } else if (u8Checked != '') {
    // U8 binary
    label = 'Value/Mask:';
    field = '<input class="qForm" type="text" name="u8" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'hex/hex (Ex. 3f/fb)';
    html += buildFormTableRow(null, label, field, comment);
    label = 'Offset:';
    field = '<input class="qForm" type="text" name="offset" value="';
    if (qFm_sh.offset) field += qFm_sh.offset;
    if (qFm_sh.nexthdr) field += qFm_sh.nexthdr;
    field += '" size=15>';
    comment = 'dec. displacement';
    html += buildFormTableRow(null, label, field, comment);

    if (qFm_sh.offset) offsetChecked = ' checked';
    if (qFm_sh.nexthdr) nexthdrChecked = ' checked';
    label = '';
    field  = '<input type="radio" name="rOffset" value="offset"';
    field += offsetChecked +' width="54pt">Offset';
    field += '<input type="radio" name="rOffset" value="nexthdr"';
    field += nexthdrChecked +' width="54pt">Next Header Offset';
    html += buildFormTableRow(null, label, field);

  } else if (u16Checked != '') {
    // U16 binary
    label = 'Value/Mask:';
    field = '<input class="qForm" type="text" name="u16" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'hex/hex (Ex. 2300/fade)';
    html += buildFormTableRow(null, label, field, comment);
    label = 'Offset:';
    field = '<input class="qForm" type="text" name="offset" value="';
    if (qFm_sh.offset) field += qFm_sh.offset;
    if (qFm_sh.nexthdr) field += qFm_sh.nexthdr;
    field += '" size=15>';
    comment = 'dec. displacement';
    html += buildFormTableRow(null, label, field, comment);

    if (qFm_sh.offset) offsetChecked = ' checked';
    if (qFm_sh.nexthdr) nexthdrChecked = ' checked';
    label = '';
    field  = '<input type="radio" name="rOffset" value="offset"';
    field += offsetChecked +' width="54pt">Offset';
    field += '<input type="radio" name="rOffset" value="nexthdr"';
    field += nexthdrChecked +' width="54pt">Next Header Offset';
    html += buildFormTableRow(null, label, field);

  } else if (u32Checked != '') {
    // U32 binary
    label = 'Value/Mask:';
    field = '<input class="qForm" type="text" name="u32" value="';
    if (qFm_sh.value) field += qFm_sh.value;
    field += '" size=15>';
    comment = 'hex/hex (Ex. 1234213f/deadbeef)';
    html += buildFormTableRow(null, label, field, comment);
    label = 'Offset:';
    field = '<input class="qForm" type="text" name="offset" value="';
    if (qFm_sh.offset) field += qFm_sh.offset;
    if (qFm_sh.nexthdr) field += qFm_sh.nexthdr;
    field += '" size=15>';
    comment = 'dec. displacement';
    html += buildFormTableRow(null, label, field, comment);

    if (qFm_sh.offset) offsetChecked = ' checked';
    if (qFm_sh.nexthdr) nexthdrChecked = ' checked';
    label = '';
    field  = '<input type="radio" name="rOffset" value="offset"';
    field += offsetChecked +' width="54pt">Offset';
    field += '<input type="radio" name="rOffset" value="nexthdr"';
    field += nexthdrChecked +' width="54pt">Next Header Offset';
    html += buildFormTableRow(null, label, field);
  }

  // If NATted, remind the user
  if (disabled != '')
  {
    html += '    <tr>';
    html += '      <td colspan="2">';
    html += '        <p class="lineitem" style="margin:2pt; text-align:center">';
    html += '          <i>NAT active; src options disabled';
    html += '        </p>';
    html += '      </td>';
    html += '    </tr>';
  }

  // Now insert the form controls
  html += '    <tr>';
  html += '      <td colspan="2">';
  html += '        <p class="lineitem" style="margin:8pt; text-align:center">';
  html += '          <input class="qForm" type="submit" name="Save"';
  html += ' value="Save" style="margin-right:36pt"';
  html += '                 onclick=\'return saveMatchClause("'+ qRuleIdx +'", "'+ qMatchIdx +'");\'>';
  html += '          <input class="qForm" type="submit" name="cancel" value="Cancel"';
  html += '                 onclick=\'return dropU32MatchForm("'+ qRuleIdxM +'", "'+ fType +'");\'>';
  html += '        </p>';
  html += '      </td>';
  html += '    </tr>';
  html += '  </table>';
  html += '</form>';

  if (!redisplay) {
    // Only include the div if it's a brandy-new form
    return '<div id="u32MatchForm' +qRuleIdxM +'">'+ html +'</div>';

  } else {

    // Redisplaying, so replace the form in the DIV and return false
    document.getElementById('u32MatchForm'+ qRuleIdxM).innerHTML = html;
    return false;
  }
}


// getScriptAndConfig()
//

function getScriptAndConfig() {
  document.getElementById('configJS').value = enumerateNodes();;
  document.getElementById('rcScript').value = buildScript();;

  // Return true to execute the button's default action.
  return true;
}


// enumerateNodes()
//

function enumerateNodes() {
  var htmlcfg = '';

  htmlcfg  = 'qNics = '+ item_enum(qNics, 2, 'qNics') + ';\n\n';
  htmlcfg += 'qRoots = '+ item_enum(qRoots, 2, 'qRoots') + ';\n\n';
  htmlcfg += 'qRules = '+ item_enum(qRules, 2, 'qRules') + ';\n\n';

  return htmlcfg;
}


// displaySaveDiv()
//

function displaySaveDiv() {
  var html = '';

  // Open the ImpEx form
  html += '<form name="SaveConfig" style="text-align:left; margin:12pt" method="post"\n';
  html += '      enctype="multipart/form-data"\n';
  html += '      action="'+ location +'">\n';
  html += '  <div style="width:25em; text-align:right; float:left">\n';
  html += '  <input type="hidden" name="CONFIG_FILENAME" value="'+ cfg_file +'">\n';
  html += '  <input type="FILE" style="margin:0 0 0 0; display:inline; width:20em"\n';
  html += '         size="20"\n';
  html += '         name="cfg_file" /><br />\n';
  html += '  <input align="center" type="submit" name="LOAD_CONFIG" value="Load"\n';
  html += '         style="margin:2pt 0 0 0; background-color:#dfdf00; font-weight:bold" /><br />\n';
  html += '  <input align="center" type="submit" name="SAVE_CONFIG" value="Save"\n';
  html += '         style="margin:2pt 0 0 0; background-color:#dfdf00; font-weight:bold" /><br />\n';
  html += '  <input align="center" type="submit" name="importconfig" value="Import"\n';
  html += '         style="margin:2pt 0 0 0; background-color:#dfdf00; font-weight:bold"\n';
  html += '         onclick="return importConfig();" /><br />\n';
  html += '  <input align="center" type="submit" name="clearconfig" value="Clear"\n';
  html += '         style="margin:12pt 0 0 0; background-color:#dfdf00; font-weight:bold"\n';
  html += '         onclick="return clearConfig();" /><br />\n';
  html += '  <input align="center" type="submit" name="compactconfig" value="Compact Maj/Min"\n';
  html += '         style="margin:2pt 0 0 0; background-color:#dfdf00; font-weight:bold"\n';
  html += '         onclick="return compactConfig();" /><br />\n';
  html += '  </div>\n';
  html += '  <textarea id=textconfig name=textconfig rows=25 cols=50\n';
  html += '            style="float:right; margin:0 12pt">';
  html += enumerateNodes();
  html += '</textarea><br clear=right>\n';
  html += '</form>\n';

  document.getElementById('save_div').innerHTML = html;
}



// displayConfigDiv()
// Given the type and index, display the proper data entry form
//

function displayConfigDiv() {
  var html = '';
  var NATform = '';
  var checkNAT = '';

  // Open the NIC config form
  html += '<form name="ToolConfig" style="text-align:center">\n';
  html += '  <table align="center" cellspacing=0 style="border:1pt solid black">\n';
  html += '    <tr>\n';
  html += '      <th class="nicForm" style="text-align:center; border-bottom:1pt solid black">NIC</th>\n';
  html += '      <th class="nicForm" style="text-align:center; border-bottom:1pt solid black">NAT<br>in use</th>\n';
  html += '      <th class="nicForm" colspan=9 style="text-align:center; border-bottom:1pt solid black; border-left:1pt solid black">Color</th>\n';
  html += '    </tr>\n';
  for (var i=0; i<9; i++) {
    html += '    <tr style="margin-bottom:1px; vertical-align:middle">\n';
    html += '      <td class="nicForm">\n';
    html += '        <p class="nicForm" style="text-align:center">\n';
    // If the NIC is NATted...
    if (qNics[i]) {
      html += '          <input class="nicForm" type="hidden" name=nicNam value="'+ qNics[i] +'" size="8">\n';
      html += '          '+ qNics[i] +'\n';
    }
    else
    {
      html += '          <input class="nicForm" type="text" name=nicNam value="" size="8">\n';
    }
    html += '        </p>\n';
    html += '      </td>\n';
    html += '      <td class="nicForm">\n';
    html += '        <p class="nicForm" style="text-align:center">\n';

    checkNAT = '';
    // If the NIC is NATted...
    if (qNics[i]) {
      if (qRoots[qNics[i]].NATted == undefined) {
        qRoots[qNics[i]].NATted = '';
      }
      if (qRoots[qNics[i]].NATted == '1') {
        checkNAT = ' checked="checked"';
      }
    }
    html += '          <input class="nicForm" type="checkbox" name=nicNATted'+ checkNAT +'>\n';
    html += '        </p>\n';
    html += '      </td>\n';

    for (var j in qColors) {
      html += '    <td class="nicForm" style="text-align:center;background-color:#'+ qColors[j] +'; border-left:1pt solid black"><input class="qForm" type="radio" name=Rcolors['+ i +'] value="'+ j +'"';
      if (qRoots[qNics[i]] && qRoots[qNics[i]].gui_color == j) {
        html += ' checked';
      }
      html += '></td>\n';
    }
    html += '</tr>\n';
  }
  html += '  </table>\n';
  html += '<input align=center type="submit" name="saveconfig" value="Apply"\n';
  html += '  style="margin-top:8pt; background-color:#dfdf00; font-weight:bold"\n';
  html += '  onclick=\'return applyToolConfig();\'>\n';
  html += '</form>\n';

  document.getElementById('config_div').innerHTML = html;
}
