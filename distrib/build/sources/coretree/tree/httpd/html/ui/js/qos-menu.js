// qos-menu.js
//
// Copyright 2009, Neal P. Murphy. All rights reserved.

// License is granted to use and distribute this software with the
//   free Smoothwall Express firewall system.
// Contact neal.p.murphy@alum.wpi.edu for more information.

// QoS Menu Functions
//
//   These functions dynamically build and display the context-sensitive
// menu when the user clicks a line item.
//
// The menu system uses Walter Zorn's wz-tooltip.js, licensed under the
// GNU LGPL. See wz.tooltip.js for details.



// itemActions()
//
//   Given an item's type, family and state, prepare the menu action items
// that make sense, to prevent the user from shooting herself in the
// foot. It works for every Root NIC, QDisc, Class and Filter.

function itemActions(qType, qRule) {
  var ala_carte = '';


  if (qType=='root') {
    // Root NIC, can only add Qdisc if there isn't one
    if (qRoots[qRule].qDisc=='') {
      ala_carte += '  <p class="menuitem" onclick=\'return addChangeQDisc("';
      ala_carte += qRule +'", "add");\'>Add QDisc</p>\n';
    }
    if (qRoots[qRule].view != 'nocfg') {
      ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
      ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
      if (qRoots[qRule].view=='open') {
        ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
        ala_carte += qType +'", "'+ qRule +'");\'>Collapse</p>\n';
      } else {
        ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
        ala_carte += qType +'", "'+ qRule +'");\'>Expand</p>\n';
      }
      if (qRoots[qRule].state=='active') {
        ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
        ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
      } else {
        ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
        ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
      }
      // Let 'em do ALL, if they desire.
      ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
      ala_carte += qType +'", "'+ qRule +'", "O");\'>Expand All</p>\n';
      ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
      ala_carte += qType +'", "'+ qRule +'", "C");\'>Collapse All</p>\n';
      ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
      ala_carte += qType +'", "'+ qRule +'", "B");\'>Bypass All</p>\n';
      ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
      ala_carte += qType +'", "'+ qRule +'", "A");\'>Activate All</p>\n';
    }

  } else if (qType=='qM') {
    // Filter Match clause
    ala_carte += '  <p class="menuitem" onclick=\'return addChangeMatchClause("';
    ala_carte += qRule +'", "change");\'>Change</p>\n';
    ala_carte += '  <p class="menuitem" onclick=\'return deleteMatch("';
    ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
  } else {

    switch (qRules[qRule].type) {
      case 'qdisc':
        switch (qRules[qRule].family) {
          case 'htb':
            // Classful, has its own rules
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeHTBClass("';
            ala_carte += qRule +'", "add");\'>Add Subclass</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeQDisc("';
            ala_carte += qRule +'", "change");\'>Change QDisc</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
            ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
            if (qRules[qRule].view=='open') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Collapse</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Expand</p>\n';
            }
            if (qRules[qRule].state=='active') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
            }
            // Let 'em do ALL, if they desire.
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "O");\'>Expand All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "C");\'>Collapse All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "B");\'>Bypass All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "A");\'>Activate All</p>\n';
            break;

          case 'pfifofast':
            // Classless, no params to change, but we can change the family
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeQDisc("';
            ala_carte += qRule +'", "change");\'>Change QDisc</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
            ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
            if (qRules[qRule].state=='active') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
            }
            // Let 'em do ALL, if they desire.
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "O");\'>Expand All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "C");\'>Collapse All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "B");\'>Bypass All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "A");\'>Activate All</p>\n';
            break;

          default:
            // All other Qdiscs have changeable parameters
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeQDisc("';
            ala_carte += qRule +'", "change");\'>Change QDisc</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
            ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
            if (qRules[qRule].state=='active') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
            }
            // Let 'em do ALL, if they desire.
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "O");\'>Expand All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "C");\'>Collapse All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "B");\'>Bypass All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "A");\'>Activate All</p>\n';
        }
        break;
      case 'class':
        //if (qRules[qRule].qClasses.length == 0) {
        if (qRules[qRule].qClasses == '') {
          // No subclasses
          if (qRules[qRule].qDisc == '' && qRules[qRule].qFilters == '') {
            // No subclasses, no qdisc, no filters, can do anything
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeQDisc("';
            ala_carte += qRule +'", "add");\'>Add QDisc</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeFilter("';
            ala_carte += qRule +'", "add");\'>Add Filter</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeHTBClass("';
            ala_carte += qRule +'", "add");\'>Add Subclass</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeHTBClass("';
            ala_carte += qRule +'", "change");\'>Change Class</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
            ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
            if (qRules[qRule].view=='open') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Collapse</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Expand</p>\n';
            }
            if (qRules[qRule].state=='active') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
            }
          } else if (qRules[qRule].qDisc != '') {
            // No subclasses, has a qdisc, can add filter
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeFilter("';
            ala_carte += qRule +'", "add");\'>Add Filter</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeHTBClass("';
            ala_carte += qRule +'", "change");\'>Change Class</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
            ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
            if (qRules[qRule].view=='open') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Collapse</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Expand</p>\n';
            }
            if (qRules[qRule].state=='active') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
            }
            // Let 'em do ALL, if they desire.
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "O");\'>Expand All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "C");\'>Collapse All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "B");\'>Bypass All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "A");\'>Activate All</p>\n';
          } else if (qRules[qRule].qFilters != '') {
            // No subclasses, has filter(s), can add qdisc and filters only
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeQDisc("';
            ala_carte += qRule +'", "add");\'>Add QDisc</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeFilter("';
            ala_carte += qRule +'", "add");\'>Add Filter</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeHTBClass("';
            ala_carte += qRule +'", "change");\'>Change Class</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
            ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
            if (qRules[qRule].view=='open') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Collapse</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Expand</p>\n';
            }
            if (qRules[qRule].state=='active') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
            }
            // Let 'em do ALL, if they desire.
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "O");\'>Expand All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "C");\'>Collapse All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "B");\'>Bypass All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "A");\'>Activate All</p>\n';
          }
        } else {
          // Has subclass(es)
          if (qRules[qRule].qDisc == '') {
            // Subclasses, no qdisc, add subclass
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeHTBClass("';
            ala_carte += qRule +'", "add");\'>Add Subclass</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return addChangeHTBClass("';
            ala_carte += qRule +'", "change");\'>Change Class</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
            ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
            if (qRules[qRule].view=='open') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Collapse</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
              ala_carte += qType +'", "'+ qRule +'");\'>Expand</p>\n';
            }
            if (qRules[qRule].state=='active') {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
            } else {
              ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
              ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
            }
            // Let 'em do ALL, if they desire.
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "O");\'>Expand All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'", "C");\'>Collapse All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "B");\'>Bypass All</p>\n';
            ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
            ala_carte += qType +'", "'+ qRule +'", "A");\'>Activate All</p>\n';
          } else {
            // WTF!?! Subclasses AND a qdisc? Item is incontinent.
            ala_carte += '  <p class="menuitem" onclick="alert(\'Cannot have both subclasses AND a queueing discipline! Delete something!\');UnTip();return false;">Incontinence!</p>\n';
          }
        }
        break;
      case 'filter':
        ala_carte += '  <p class="menuitem" onclick=\'return addChangeMatchClause("';
        ala_carte += qRule +'", "add");\'>Add Match Clause</p>\n';
        ala_carte += '  <p class="menuitem" onclick=\'return addChangeFilter("';
        ala_carte += qRule +'", "change");\'>Change Filter</p>\n';
        ala_carte += '  <p class="menuitem" onclick=\'return deleteRule("';
        ala_carte += qType +'", "'+ qRule +'");\'>Delete</p>\n';
          if (qRules[qRule].view=='open') {
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'");\'>Collapse</p>\n';
          } else {
            ala_carte += '  <p class="menuitem" onclick=\'return toggleView("';
            ala_carte += qType +'", "'+ qRule +'");\'>Expand</p>\n';
          }
        if (qRules[qRule].state=='active') {
          ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
          ala_carte += qType +'", "'+ qRule +'");\'>Bypass</p>\n';
        } else {
          ala_carte += '  <p class="menuitem" onclick=\'return toggleState("';
          ala_carte += qType +'", "'+ qRule +'");\'>Activate</p>\n';
        }
        break;
    }
  }

  return ala_carte;
}


// itemMenu()
//
//   Build the context-sensitive menu when the user clicks an item.

function itemMenu(qType, qRuleIdx) {
  var menu_html = '';

  // Build a nice label for the menu
  switch (qType) {
    case 'root':
      menuLabel  = '<b>NIC Ops</b><br><i>'+ qRuleIdx +'</i>';
      break;
    case 'qR':
      if (qRules[qRuleIdx].type == 'class') {
        menuLabel = '<b>Class Ops</b></br><i> '+ qRuleIdx +'</i>';
      } else if (qRules[qRuleIdx].type == 'qdisc') {
        if (qRules[qRuleIdx].family == 'htb') {
          menuLabel = '<b>QDisc Ops</b></br><i> '+ qRuleIdx +'</i>';
        } else {
          menuLabel = '<b>Leaf Ops</b></br><i> '+ qRuleIdx +'</i>';
        }
      }
      break;
    case 'qF':
      menuLabel = '<b>Filter Ops</b></br><i> '+ qRuleIdx +'</i>';
      break;
    case 'qL':
      menuLabel = '<b>Leaf Ops</b></br><i> '+ qRuleIdx +'</i>';
      break;
    case 'qM':
      menuLabel = '<b>Match Clause</b></br><i> '+ qRuleIdx +'</i>';
      break;
  }

  // Assemble the HTML
  menu_html += '  <p class="menuitem" style="text-align:center;';
  menu_html += ' font-weight:normal; border-bottom: solid 2pt black"';
  menu_html += ' title="Click to dismiss"';
  menu_html += ' onclick="UnTip();">'+ menuLabel +'</p>\n';

  // Add the allowable menu actions
  menu_html += itemActions(qType, qRuleIdx);


  // Show the menu
  Tip(menu_html, OFFSETX, 10, OFFSETY, -12, WIDTH, 0, HEIGHT, 0,
                 FONTSIZE, '16px', BGCOLOR, '#dfdfdf', FOLLOWMOUSE, false, DELAY, 0);
}
