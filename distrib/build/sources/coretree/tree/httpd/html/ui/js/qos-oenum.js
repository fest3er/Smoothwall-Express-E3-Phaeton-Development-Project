// qos-oenum.js
//
// Copyright 2009, Neal P. Murphy. All rights reserved.

// License is granted to use and distribute this software with the
//   free Smoothwall Express firewall system.
// Contact neal.p.murphy@alum.wpi.edu for more information.

// QoS Utilities
//
//   These are certain utility functions that didn't
// really fit anywhere else.
// boldface text
//


function boldFace(text) {
  return '<b>'+ text +'</b>';
}

// item_enum()
//
// Recursively enumerate the contents of an object

function item_enum(item, indent, name) {

  var key;
  var enum_txt, object_txt = '';
  var spaces='                        ';

  switch (typeof(item)) {

    // number or boolean
    case 'number':
    case 'boolean':
      return '"'+ item.toString() +'"';
      break;

    // string
    case 'string':
      return '"'+ item +'"';
      break;

    case 'object':
      switch (name) {
        case 'matches':
        case 'qNics':
        case 'qClasses':
        case 'qFilters':
          // is array
          enum_txt = '[\n';
          for (key in item) {
            object_txt += spaces.substring(0, indent);
            object_txt += item_enum(item[key], indent+2, key);
            object_txt += ',\n';
          }
          if (object_txt.length > 0) {
            object_txt = object_txt.substring(0, object_txt.length-2) +'\n';
          }
          enum_txt += object_txt;
          enum_txt += spaces.substring(0, indent-2) +']';
          break;
  
        default:
          // is object
          enum_txt = '{\n';
          for (key in item) {
            object_txt += spaces.substring(0, indent);
            object_txt += '"'+ key + '": ';
            object_txt += item_enum(item[key], indent+2, key);
            object_txt += ',\n';
          }
          if (object_txt.length != 0) {
            object_txt = object_txt.substring(0, object_txt.length-2) +'\n';
          }
          enum_txt += object_txt;
          enum_txt += spaces.substring(0, indent-2) +'}';
      }
      return enum_txt;
  }
}
