qNics = [
  "GREEN",
  "PURPLE",
  "ORANGE",
  "RED"
];

qRoots = {
  "GREEN": {
    "gui_color": "green",
    "view": "open",
    "state": "active",
    "qDisc": "GREEN-1:0",
    "pending_action": "",
    "nextMajor": "16",
    "NATted": ""
  },
  "PURPLE": {
    "gui_color": "purple",
    "view": "open",
    "state": "active",
    "qDisc": "PURPLE-1:0",
    "pending_action": "",
    "nextMajor": "13",
    "NATted": ""
  },
  "ORANGE": {
    "gui_color": "orange",
    "view": "open",
    "state": "active",
    "qDisc": "ORANGE-1:0",
    "pending_action": "",
    "nextMajor": "13",
    "NATted": ""
  },
  "RED": {
    "gui_color": "red",
    "view": "open",
    "state": "active",
    "qDisc": "RED-1:0",
    "pending_action": "",
    "nextMajor": "13",
    "NATted": "1"
  }
};

qRules = {
  "GREEN-1:0": {
    "comment": "Throttled With Exceptions",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN",
    "pending_action": "",
    "type": "qdisc",
    "family": "htb",
    "params": {
      "qdefault": "9"
    },
    "qClasses": [
      "GREEN-1:2",
      "GREEN-1:9"
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "22"
  },
  "GREEN-1:2": {
    "comment": "Unthrottled Exceptions",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:0",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:0",
      "priority": "1",
      "rate": "900",
      "rateUnit": "Mbit/s",
      "burstR": "30",
      "burstRUnit": "kB"
    },
    "qClasses": [
      "GREEN-1:3",
      "GREEN-1:6"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "GREEN-1:3": {
    "comment": "Admin",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:2",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:2",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "GREEN-1:4",
      "GREEN-1:5"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "GREEN-1:4": {
    "comment": "SSH",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:3",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:3",
      "priority": "1",
      "rate": "30",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6000",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:4.0"
    ],
    "qDisc": ""
  },
  "GREEN-1:5": {
    "comment": "HTTP",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:3",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:3",
      "priority": "1",
      "rate": "70",
      "rateUnit": "",
      "burstR": "3000",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:5.0",
      "GREEN-1:5.1"
    ],
    "qDisc": ""
  },
  "GREEN-1:4.0": {
    "comment": "SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:4",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "0"
    },
    "matches": [
      {
        "type": "sport",
        "value": "222/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$GREEN_ADDRESS"
      }
    ]
  },
  "GREEN-1:5.0": {
    "comment": "SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:5",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "0"
    },
    "matches": [
      {
        "type": "sport",
        "value": "81/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$GREEN_ADDRESS"
      }
    ]
  },
  "GREEN-1:6": {
    "comment": "Through Traffic",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:2",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:2",
      "priority": "1",
      "rate": "80",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "GREEN-1:10",
      "GREEN-1:11",
      "GREEN-1:12",
      "GREEN-1:13",
      "GREEN-1:14",
      "GREEN-1:15",
      "GREEN-1:16",
      "GREEN-1:18"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "GREEN-1:9": {
    "comment": "Throttled Traffic",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:0",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:0",
      "priority": "7",
      "rate": "56",
      "rateUnit": "kbit/s",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "",
      "ceilUnit": "",
      "burstC": "",
      "burstCUnit": ""
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "GREEN-4:0"
  },
  "GREEN-1:10": {
    "comment": "File Transfer",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "15",
      "rateUnit": "",
      "burstR": "16",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "1",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:10.0",
      "GREEN-1:10.1",
      "GREEN-1:10.2",
      "GREEN-1:10.3",
      "GREEN-1:10.4"
    ],
    "qDisc": "GREEN-12:0"
  },
  "GREEN-1:10.0": {
    "comment": "DST FTP",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "20/fffe"
      }
    ]
  },
  "GREEN-1:10.1": {
    "comment": "SRC FTP",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "20/fffe"
      }
    ]
  },
  "GREEN-1:11": {
    "comment": "DNS",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "0",
      "rate": "1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "1",
      "ceilUnit": "",
      "burstC": "1500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:11.0",
      "GREEN-1:11.1"
    ],
    "qDisc": "GREEN-11:0"
  },
  "GREEN-1:12": {
    "comment": "Instant Messaging",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "2",
      "rate": "1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "1",
      "ceilUnit": "",
      "burstC": "1500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:12.0",
      "GREEN-1:12.1",
      "GREEN-1:12.2",
      "GREEN-1:12.3",
      "GREEN-1:12.4",
      "GREEN-1:12.5",
      "GREEN-1:12.6",
      "GREEN-1:12.7"
    ],
    "qDisc": "GREEN-10:0"
  },
  "GREEN-1:12.0": {
    "comment": "SRC 5190-5191",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5190/fffe"
      }
    ]
  },
  "GREEN-1:12.1": {
    "comment": "DST 5190-5191",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5190/fffe"
      }
    ]
  },
  "GREEN-1:12.2": {
    "comment": "SRC 5192-5193",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5192/fffe"
      }
    ]
  },
  "GREEN-1:12.3": {
    "comment": "DST 5192-5193",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5192/fffe"
      }
    ]
  },
  "GREEN-1:12.4": {
    "comment": "SRC 1863",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1863/ffff"
      }
    ]
  },
  "GREEN-1:12.5": {
    "comment": "DST 1863",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1863/ffff"
      }
    ]
  },
  "GREEN-1:12.6": {
    "comment": "SRC 6880-6911 (6891-6901)",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "6880/ffe0"
      }
    ]
  },
  "GREEN-1:12.7": {
    "comment": "DST 6880-6911 (6891-6901)",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "6880/ffe0"
      }
    ]
  },
  "GREEN-1:13": {
    "comment": "Web Surfing",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "125",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "250",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:13.0",
      "GREEN-1:13.1",
      "GREEN-1:13.2",
      "GREEN-1:13.3",
      "GREEN-1:13.4",
      "GREEN-1:13.5",
      "GREEN-1:13.6",
      "GREEN-1:13.7",
      "GREEN-1:13.8",
      "GREEN-1:13.9",
      "GREEN-1:13.10",
      "GREEN-1:13.11",
      "GREEN-1:13.12",
      "GREEN-1:13.13",
      "GREEN-1:13.14",
      "GREEN-1:13.15"
    ],
    "qDisc": "GREEN-9:0"
  },
  "GREEN-1:13.0": {
    "comment": "SRC 80",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "80/ffff"
      }
    ]
  },
  "GREEN-1:13.1": {
    "comment": "DST 80",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "80/ffff"
      }
    ]
  },
  "GREEN-1:13.2": {
    "comment": "SRC 443",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "443/ffff"
      }
    ]
  },
  "GREEN-1:13.3": {
    "comment": "DST 443",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "443/ffff"
      }
    ]
  },
  "GREEN-1:13.4": {
    "comment": "SRC 800",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "800/ffff"
      }
    ]
  },
  "GREEN-1:13.5": {
    "comment": "DST 800",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "800/ffff"
      }
    ]
  },
  "GREEN-1:13.6": {
    "comment": "SRC 3128",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "3128/ffff"
      }
    ]
  },
  "GREEN-1:13.7": {
    "comment": "DST 3128",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "3128/ffff"
      }
    ]
  },
  "GREEN-1:13.8": {
    "comment": "SRC 8008",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8008/ffff"
      }
    ]
  },
  "GREEN-1:13.9": {
    "comment": "DST 8008",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8008/ffff"
      }
    ]
  },
  "GREEN-1:13.10": {
    "comment": "SRC 8080",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8080/ffff"
      }
    ]
  },
  "GREEN-1:13.11": {
    "comment": "DST 8080",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8080/ffff"
      }
    ]
  },
  "GREEN-1:14": {
    "comment": "Email",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "250",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "1000",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:14.0",
      "GREEN-1:14.1",
      "GREEN-1:14.2",
      "GREEN-1:14.3",
      "GREEN-1:14.4",
      "GREEN-1:14.5",
      "GREEN-1:14.6",
      "GREEN-1:14.7",
      "GREEN-1:14.8",
      "GREEN-1:14.9",
      "GREEN-1:14.10",
      "GREEN-1:14.11"
    ],
    "qDisc": "GREEN-8:0"
  },
  "GREEN-1:14.0": {
    "comment": "SRC 25",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "25/ffff"
      }
    ]
  },
  "GREEN-1:14.1": {
    "comment": "DST 25",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "25/ffff"
      }
    ]
  },
  "GREEN-1:14.2": {
    "comment": "SRC 110",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "110/ffff"
      }
    ]
  },
  "GREEN-1:14.3": {
    "comment": "DST 110",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "110/ffff"
      }
    ]
  },
  "GREEN-1:14.4": {
    "comment": "SRC 143",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "443/ffff"
      }
    ]
  },
  "GREEN-1:14.5": {
    "comment": "DST 143",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "443/ffff"
      }
    ]
  },
  "GREEN-1:14.6": {
    "comment": "SRC 993",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "993/ffff"
      }
    ]
  },
  "GREEN-1:14.7": {
    "comment": "DST 993",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "993/ffff"
      }
    ]
  },
  "GREEN-1:15": {
    "comment": "SSH",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "8",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:15.0",
      "GREEN-1:15.1",
      "GREEN-1:15.2",
      "GREEN-1:15.3"
    ],
    "qDisc": "GREEN-7:0"
  },
  "GREEN-1:15.0": {
    "comment": "SRC 22",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "22/ffff"
      }
    ]
  },
  "GREEN-1:15.1": {
    "comment": "DST 22",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "22/ffff"
      }
    ]
  },
  "GREEN-1:16": {
    "comment": "VoIP",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "0",
      "rate": ".1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": ".3",
      "ceilUnit": "",
      "burstC": "4500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:16.0",
      "GREEN-1:16.1",
      "GREEN-1:16.2",
      "GREEN-1:16.3",
      "GREEN-1:16.4",
      "GREEN-1:16.5",
      "GREEN-1:16.6",
      "GREEN-1:16.7"
    ],
    "qDisc": "GREEN-6:0"
  },
  "GREEN-1:16.0": {
    "comment": "SRC 1503",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1503/ffff"
      }
    ]
  },
  "GREEN-1:16.1": {
    "comment": "DST 1503",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1503/ffff"
      }
    ]
  },
  "GREEN-1:16.2": {
    "comment": "SRC 1720",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1720/ffff"
      }
    ]
  },
  "GREEN-1:16.3": {
    "comment": "DST 1720",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1720/ffff"
      }
    ]
  },
  "GREEN-1:16.4": {
    "comment": "SRC 3782",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "3782/ffff"
      }
    ]
  },
  "GREEN-1:16.5": {
    "comment": "DST 3782",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "3782/ffff"
      }
    ]
  },
  "GREEN-1:16.6": {
    "comment": "SRC 5056-5071 (5060-5070)",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5056/fff0"
      }
    ]
  },
  "GREEN-1:16.7": {
    "comment": "DST 5056-5071 (5060-5070)",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5056/fff0"
      }
    ]
  },
  "GREEN-1:13.12": {
    "comment": "SRC 441",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "441/ffff"
      }
    ]
  },
  "GREEN-1:13.13": {
    "comment": "DST 441",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "441/ffff"
      }
    ]
  },
  "GREEN-1:15.2": {
    "comment": "SRC 8627",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8627/ffff"
      }
    ]
  },
  "GREEN-1:15.3": {
    "comment": "DST 8627",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8627/ffff"
      }
    ]
  },
  "GREEN-4:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:9",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-6:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:16",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "1500"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-7:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:15",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-8:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-9:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-10:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:12",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-11:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:11",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-12:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:10",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-1:5.1": {
    "comment": "SRC 441",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:5",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "441/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$GREEN_ADDRESS"
      }
    ]
  },
  "GREEN-1:13.14": {
    "comment": "WebPro Dst 2083",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "2083/ffff"
      }
    ]
  },
  "GREEN-1:13.15": {
    "comment": "WebPro Src 2083",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "2083/ffff"
      }
    ]
  },
  "GREEN-1:14.8": {
    "comment": "SRC gmail POP3",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "995/ffff"
      }
    ]
  },
  "GREEN-1:14.9": {
    "comment": "DST gmail POP3",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "995/ffff"
      }
    ]
  },
  "GREEN-1:14.10": {
    "comment": "SRC 587",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "587/ffff"
      }
    ]
  },
  "GREEN-1:14.11": {
    "comment": "DST 587",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "587/ffff"
      }
    ]
  },
  "GREEN-1:18": {
    "comment": "VPN",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "9",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "18",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "GREEN-1:19",
      "GREEN-1:20",
      "GREEN-1:21"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "GREEN-1:19": {
    "comment": "PPTP",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:19.0",
      "GREEN-1:19.1"
    ],
    "qDisc": "GREEN-13:0"
  },
  "GREEN-1:19.0": {
    "comment": "SRC 1732",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:19",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1732/ffff"
      }
    ]
  },
  "GREEN-1:19.1": {
    "comment": "GRE",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:19",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "47/ff"
      }
    ]
  },
  "GREEN-1:20": {
    "comment": "IPSEC",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:20.0",
      "GREEN-1:20.1",
      "GREEN-1:20.2",
      "GREEN-1:20.3",
      "GREEN-1:20.4",
      "GREEN-1:20.5"
    ],
    "qDisc": "GREEN-14:0"
  },
  "GREEN-1:20.0": {
    "comment": "ESP",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "50/ff"
      }
    ]
  },
  "GREEN-1:20.1": {
    "comment": "AH",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "51/ff"
      }
    ]
  },
  "GREEN-1:20.2": {
    "comment": "IKE SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "500/ffff"
      }
    ]
  },
  "GREEN-1:20.3": {
    "comment": "NAT-T SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "4500/ffff"
      }
    ]
  },
  "GREEN-1:20.4": {
    "comment": "NAT-T DST",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "4500/ffff"
      }
    ]
  },
  "GREEN-1:20.5": {
    "comment": "IKE DST",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "500/ffff"
      }
    ]
  },
  "GREEN-1:21": {
    "comment": "OpenVPN",
    "view": "closed",
    "state": "active",
    "parent_rule": "GREEN-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "GREEN-1:21.0",
      "GREEN-1:21.1",
      "GREEN-1:21.2",
      "GREEN-1:21.3"
    ],
    "qDisc": "GREEN-15:0"
  },
  "GREEN-1:21.0": {
    "comment": "SRC 1194",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1194/ffff"
      }
    ]
  },
  "GREEN-1:21.1": {
    "comment": "DST 1194",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1194/ffff"
      }
    ]
  },
  "GREEN-1:21.2": {
    "comment": "SRC 1195",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1195/ffff"
      }
    ]
  },
  "GREEN-1:21.3": {
    "comment": "DST 1195",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1195/ffff"
      }
    ]
  },
  "GREEN-13:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:19",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-14:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:20",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-15:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:21",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "GREEN-1:10.2": {
    "comment": "SRC GIT",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "9418/ffff"
      }
    ]
  },
  "GREEN-1:10.3": {
    "comment": "DST GIT",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "9418/ffff"
      }
    ]
  },
  "GREEN-1:11.0": {
    "comment": "SRC DNS UDP",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:11",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "53/ffff"
      },
      {
        "type": "protocol",
        "value": "15/ff"
      }
    ]
  },
  "GREEN-1:11.1": {
    "comment": "DST DNS UDP",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:11",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "53/ffff"
      },
      {
        "type": "protocol",
        "value": "15/ff"
      }
    ]
  },
  "PURPLE-1:0": {
    "comment": "Throttled With Exceptions",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE",
    "pending_action": "",
    "type": "qdisc",
    "family": "htb",
    "params": {
      "qdefault": "9"
    },
    "qClasses": [
      "PURPLE-1:2",
      "PURPLE-1:9"
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "22"
  },
  "PURPLE-1:2": {
    "comment": "Unthrottled Exceptions",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:0",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:0",
      "priority": "1",
      "rate": "28",
      "rateUnit": "Mbit/s",
      "burstR": "30",
      "burstRUnit": "kB"
    },
    "qClasses": [
      "PURPLE-1:3",
      "PURPLE-1:6"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "PURPLE-1:3": {
    "comment": "Admin",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:2",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:2",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "PURPLE-1:4",
      "PURPLE-1:5"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "PURPLE-1:4": {
    "comment": "SSH",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:3",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:3",
      "priority": "1",
      "rate": "30",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6000",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:4.0"
    ],
    "qDisc": ""
  },
  "PURPLE-1:5": {
    "comment": "HTTP",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:3",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:3",
      "priority": "1",
      "rate": "70",
      "rateUnit": "",
      "burstR": "3000",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:5.0",
      "PURPLE-1:5.1"
    ],
    "qDisc": ""
  },
  "PURPLE-1:4.0": {
    "comment": "SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:4",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "0"
    },
    "matches": [
      {
        "type": "sport",
        "value": "222/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$PURPLE_ADDRESS"
      }
    ]
  },
  "PURPLE-1:5.0": {
    "comment": "SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:5",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "0"
    },
    "matches": [
      {
        "type": "sport",
        "value": "81/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$PURPLE_ADDRESS"
      }
    ]
  },
  "PURPLE-1:6": {
    "comment": "Through Traffic",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:2",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:2",
      "priority": "1",
      "rate": "80",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "PURPLE-1:10",
      "PURPLE-1:11",
      "PURPLE-1:12",
      "PURPLE-1:13",
      "PURPLE-1:14",
      "PURPLE-1:15",
      "PURPLE-1:16",
      "PURPLE-1:18"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "PURPLE-1:9": {
    "comment": "Throttled Traffic",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:0",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:0",
      "priority": "7",
      "rate": "56",
      "rateUnit": "kbit/s",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "",
      "ceilUnit": "",
      "burstC": "",
      "burstCUnit": ""
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "PURPLE-4:0"
  },
  "PURPLE-1:10": {
    "comment": "File Transfer",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "15",
      "rateUnit": "",
      "burstR": "16",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "1",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:10.0",
      "PURPLE-1:10.1",
      "PURPLE-1:10.2",
      "PURPLE-1:10.3"
    ],
    "qDisc": "PURPLE-12:0"
  },
  "PURPLE-1:10.0": {
    "comment": "DST FTP",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "20/fffe"
      }
    ]
  },
  "PURPLE-1:10.1": {
    "comment": "SRC FTP",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "20/fffe"
      }
    ]
  },
  "PURPLE-1:11": {
    "comment": "DNS",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "0",
      "rate": "1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "1",
      "ceilUnit": "",
      "burstC": "1500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:11.0",
      "PURPLE-1:11.1"
    ],
    "qDisc": "PURPLE-11:0"
  },
  "PURPLE-1:12": {
    "comment": "Instant Messaging",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "2",
      "rate": "1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "1",
      "ceilUnit": "",
      "burstC": "1500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:12.0",
      "PURPLE-1:12.1",
      "PURPLE-1:12.2",
      "PURPLE-1:12.3",
      "PURPLE-1:12.4",
      "PURPLE-1:12.5",
      "PURPLE-1:12.6",
      "PURPLE-1:12.7"
    ],
    "qDisc": "PURPLE-10:0"
  },
  "PURPLE-1:12.0": {
    "comment": "SRC 5190-5191",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5190/fffe"
      }
    ]
  },
  "PURPLE-1:12.1": {
    "comment": "DST 5190-5191",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5190/fffe"
      }
    ]
  },
  "PURPLE-1:12.2": {
    "comment": "SRC 5192-5193",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5192/fffe"
      }
    ]
  },
  "PURPLE-1:12.3": {
    "comment": "DST 5192-5193",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5192/fffe"
      }
    ]
  },
  "PURPLE-1:12.4": {
    "comment": "SRC 1863",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1863/ffff"
      }
    ]
  },
  "PURPLE-1:12.5": {
    "comment": "DST 1863",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1863/ffff"
      }
    ]
  },
  "PURPLE-1:12.6": {
    "comment": "SRC 6880-6911 (6891-6901)",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "6880/ffe0"
      }
    ]
  },
  "PURPLE-1:12.7": {
    "comment": "DST 6880-6911 (6891-6901)",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "6880/ffe0"
      }
    ]
  },
  "PURPLE-1:13": {
    "comment": "Web Surfing",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "125",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "250",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:13.0",
      "PURPLE-1:13.1",
      "PURPLE-1:13.2",
      "PURPLE-1:13.3",
      "PURPLE-1:13.4",
      "PURPLE-1:13.5",
      "PURPLE-1:13.6",
      "PURPLE-1:13.7",
      "PURPLE-1:13.8",
      "PURPLE-1:13.9",
      "PURPLE-1:13.10",
      "PURPLE-1:13.11",
      "PURPLE-1:13.12",
      "PURPLE-1:13.13",
      "PURPLE-1:13.14",
      "PURPLE-1:13.15"
    ],
    "qDisc": "PURPLE-9:0"
  },
  "PURPLE-1:13.0": {
    "comment": "SRC 80",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "80/ffff"
      }
    ]
  },
  "PURPLE-1:13.1": {
    "comment": "DST 80",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "80/ffff"
      }
    ]
  },
  "PURPLE-1:13.2": {
    "comment": "SRC 443",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "443/ffff"
      }
    ]
  },
  "PURPLE-1:13.3": {
    "comment": "DST 443",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "443/ffff"
      }
    ]
  },
  "PURPLE-1:13.4": {
    "comment": "SRC 800",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "800/ffff"
      }
    ]
  },
  "PURPLE-1:13.5": {
    "comment": "DST 800",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "800/ffff"
      }
    ]
  },
  "PURPLE-1:13.6": {
    "comment": "SRC 3128",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "3128/ffff"
      }
    ]
  },
  "PURPLE-1:13.7": {
    "comment": "DST 3128",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "3128/ffff"
      }
    ]
  },
  "PURPLE-1:13.8": {
    "comment": "SRC 8008",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8008/ffff"
      }
    ]
  },
  "PURPLE-1:13.9": {
    "comment": "DST 8008",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8008/ffff"
      }
    ]
  },
  "PURPLE-1:13.10": {
    "comment": "SRC 8080",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8080/ffff"
      }
    ]
  },
  "PURPLE-1:13.11": {
    "comment": "DST 8080",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8080/ffff"
      }
    ]
  },
  "PURPLE-1:14": {
    "comment": "Email",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "250",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "1000",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:14.0",
      "PURPLE-1:14.1",
      "PURPLE-1:14.2",
      "PURPLE-1:14.3",
      "PURPLE-1:14.4",
      "PURPLE-1:14.5",
      "PURPLE-1:14.6",
      "PURPLE-1:14.7",
      "PURPLE-1:14.8",
      "PURPLE-1:14.9",
      "PURPLE-1:14.10",
      "PURPLE-1:14.11"
    ],
    "qDisc": "PURPLE-8:0"
  },
  "PURPLE-1:14.0": {
    "comment": "SRC 25",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "25/ffff"
      }
    ]
  },
  "PURPLE-1:14.1": {
    "comment": "DST 25",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "25/ffff"
      }
    ]
  },
  "PURPLE-1:14.2": {
    "comment": "SRC 110",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "110/ffff"
      }
    ]
  },
  "PURPLE-1:14.3": {
    "comment": "DST 110",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "110/ffff"
      }
    ]
  },
  "PURPLE-1:14.4": {
    "comment": "SRC 143",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "443/ffff"
      }
    ]
  },
  "PURPLE-1:14.5": {
    "comment": "DST 143",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "443/ffff"
      }
    ]
  },
  "PURPLE-1:14.6": {
    "comment": "SRC 993",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "993/ffff"
      }
    ]
  },
  "PURPLE-1:14.7": {
    "comment": "DST 993",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "993/ffff"
      }
    ]
  },
  "PURPLE-1:15": {
    "comment": "SSH",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "8",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:15.0",
      "PURPLE-1:15.1",
      "PURPLE-1:15.2",
      "PURPLE-1:15.3"
    ],
    "qDisc": "PURPLE-7:0"
  },
  "PURPLE-1:15.0": {
    "comment": "SRC 22",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "22/ffff"
      }
    ]
  },
  "PURPLE-1:15.1": {
    "comment": "DST 22",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "22/ffff"
      }
    ]
  },
  "PURPLE-1:16": {
    "comment": "VoIP",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "0",
      "rate": ".1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": ".3",
      "ceilUnit": "",
      "burstC": "4500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:16.0",
      "PURPLE-1:16.1",
      "PURPLE-1:16.2",
      "PURPLE-1:16.3",
      "PURPLE-1:16.4",
      "PURPLE-1:16.5",
      "PURPLE-1:16.6",
      "PURPLE-1:16.7"
    ],
    "qDisc": "PURPLE-6:0"
  },
  "PURPLE-1:16.0": {
    "comment": "SRC 1503",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1503/ffff"
      }
    ]
  },
  "PURPLE-1:16.1": {
    "comment": "DST 1503",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1503/ffff"
      }
    ]
  },
  "PURPLE-1:16.2": {
    "comment": "SRC 1720",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1720/ffff"
      }
    ]
  },
  "PURPLE-1:16.3": {
    "comment": "DST 1720",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1720/ffff"
      }
    ]
  },
  "PURPLE-1:16.4": {
    "comment": "SRC 3782",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "3782/ffff"
      }
    ]
  },
  "PURPLE-1:16.5": {
    "comment": "DST 3782",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "3782/ffff"
      }
    ]
  },
  "PURPLE-1:16.6": {
    "comment": "SRC 5056-5071 (5060-5070)",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5056/fff0"
      }
    ]
  },
  "PURPLE-1:16.7": {
    "comment": "DST 5056-5071 (5060-5070)",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5056/fff0"
      }
    ]
  },
  "PURPLE-1:13.12": {
    "comment": "SRC 441",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "441/ffff"
      }
    ]
  },
  "PURPLE-1:13.13": {
    "comment": "DST 441",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "441/ffff"
      }
    ]
  },
  "PURPLE-1:15.2": {
    "comment": "SRC 8627",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8627/ffff"
      }
    ]
  },
  "PURPLE-1:15.3": {
    "comment": "DST 8627",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8627/ffff"
      }
    ]
  },
  "PURPLE-4:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:9",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-6:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:16",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "1500"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-7:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:15",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-8:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-9:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-10:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:12",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-11:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:11",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-12:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:10",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-1:5.1": {
    "comment": "SRC 441",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:5",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "441/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$PURPLE_ADDRESS"
      }
    ]
  },
  "PURPLE-1:13.14": {
    "comment": "WebPro Dst 2083",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "2083/ffff"
      }
    ]
  },
  "PURPLE-1:13.15": {
    "comment": "WebPro Src 2083",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "2083/ffff"
      }
    ]
  },
  "PURPLE-1:14.8": {
    "comment": "SRC gmail POP3",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "995/ffff"
      }
    ]
  },
  "PURPLE-1:14.9": {
    "comment": "DST gmail POP3",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "995/ffff"
      }
    ]
  },
  "PURPLE-1:14.10": {
    "comment": "SRC 587",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "587/ffff"
      }
    ]
  },
  "PURPLE-1:14.11": {
    "comment": "DST 587",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "587/ffff"
      }
    ]
  },
  "PURPLE-1:18": {
    "comment": "VPN",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "9",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "18",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "PURPLE-1:19",
      "PURPLE-1:20",
      "PURPLE-1:21"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "PURPLE-1:19": {
    "comment": "PPTP",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:19.0",
      "PURPLE-1:19.1"
    ],
    "qDisc": "PURPLE-13:0"
  },
  "PURPLE-1:19.0": {
    "comment": "SRC 1732",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:19",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1732/ffff"
      }
    ]
  },
  "PURPLE-1:19.1": {
    "comment": "GRE",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:19",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "47/ff"
      }
    ]
  },
  "PURPLE-1:20": {
    "comment": "IPSEC",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:20.0",
      "PURPLE-1:20.1",
      "PURPLE-1:20.2",
      "PURPLE-1:20.3",
      "PURPLE-1:20.4",
      "PURPLE-1:20.5"
    ],
    "qDisc": "PURPLE-14:0"
  },
  "PURPLE-1:20.0": {
    "comment": "ESP",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "50/ff"
      }
    ]
  },
  "PURPLE-1:20.1": {
    "comment": "AH",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "51/ff"
      }
    ]
  },
  "PURPLE-1:20.2": {
    "comment": "IKE SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "500/ffff"
      }
    ]
  },
  "PURPLE-1:20.3": {
    "comment": "NAT-T SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "4500/ffff"
      }
    ]
  },
  "PURPLE-1:20.4": {
    "comment": "NAT-T DST",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "4500/ffff"
      }
    ]
  },
  "PURPLE-1:20.5": {
    "comment": "IKE DST",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "500/ffff"
      }
    ]
  },
  "PURPLE-1:21": {
    "comment": "OpenVPN",
    "view": "closed",
    "state": "active",
    "parent_rule": "PURPLE-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "PURPLE-1:21.0",
      "PURPLE-1:21.1",
      "PURPLE-1:21.2",
      "PURPLE-1:21.3"
    ],
    "qDisc": "PURPLE-15:0"
  },
  "PURPLE-1:21.0": {
    "comment": "SRC 1194",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1194/ffff"
      }
    ]
  },
  "PURPLE-1:21.1": {
    "comment": "DST 1194",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1194/ffff"
      }
    ]
  },
  "PURPLE-1:21.2": {
    "comment": "SRC 1195",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1195/ffff"
      }
    ]
  },
  "PURPLE-1:21.3": {
    "comment": "DST 1195",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1195/ffff"
      }
    ]
  },
  "PURPLE-13:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:19",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-14:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:20",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-15:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:21",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-1:10.2": {
    "comment": "SRC GIT",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "9418/ffff"
      }
    ]
  },
  "PURPLE-1:10.3": {
    "comment": "DST GIT",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "9418/ffff"
      }
    ]
  },
  "PURPLE-1:11.0": {
    "comment": "SRC DNS UDP",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:11",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "53/ffff"
      },
      {
        "type": "protocol",
        "value": "15/ff"
      }
    ]
  },
  "PURPLE-1:11.1": {
    "comment": "DST DNS UDP",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE-1:11",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "53/ffff"
      },
      {
        "type": "protocol",
        "value": "15/ff"
      }
    ]
  },
  "ORANGE-1:0": {
    "comment": "Throttled With Exceptions",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE",
    "pending_action": "",
    "type": "qdisc",
    "family": "htb",
    "params": {
      "qdefault": "9"
    },
    "qClasses": [
      "ORANGE-1:2",
      "ORANGE-1:9"
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "22"
  },
  "ORANGE-1:2": {
    "comment": "Unthrottled Exceptions",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:0",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:0",
      "priority": "1",
      "rate": "91",
      "rateUnit": "Mbit/s",
      "burstR": "30",
      "burstRUnit": "kB"
    },
    "qClasses": [
      "ORANGE-1:3",
      "ORANGE-1:6"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "ORANGE-1:3": {
    "comment": "Admin",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:2",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:2",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "ORANGE-1:4",
      "ORANGE-1:5"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "ORANGE-1:4": {
    "comment": "SSH",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:3",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:3",
      "priority": "1",
      "rate": "30",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6000",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:4.0"
    ],
    "qDisc": ""
  },
  "ORANGE-1:5": {
    "comment": "HTTP",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:3",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:3",
      "priority": "1",
      "rate": "70",
      "rateUnit": "",
      "burstR": "3000",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:5.0",
      "ORANGE-1:5.1"
    ],
    "qDisc": ""
  },
  "ORANGE-1:4.0": {
    "comment": "SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:4",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "0"
    },
    "matches": [
      {
        "type": "sport",
        "value": "222/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$ORANGE_ADDRESS"
      }
    ]
  },
  "ORANGE-1:5.0": {
    "comment": "SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:5",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "0"
    },
    "matches": [
      {
        "type": "sport",
        "value": "81/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$ORANGE_ADDRESS"
      }
    ]
  },
  "ORANGE-1:6": {
    "comment": "Through Traffic",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:2",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:2",
      "priority": "1",
      "rate": "80",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "ORANGE-1:10",
      "ORANGE-1:11",
      "ORANGE-1:12",
      "ORANGE-1:13",
      "ORANGE-1:14",
      "ORANGE-1:15",
      "ORANGE-1:16",
      "ORANGE-1:18"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "ORANGE-1:9": {
    "comment": "Throttled Traffic",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:0",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:0",
      "priority": "7",
      "rate": "56",
      "rateUnit": "kbit/s",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "",
      "ceilUnit": "",
      "burstC": "",
      "burstCUnit": ""
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "ORANGE-4:0"
  },
  "ORANGE-1:10": {
    "comment": "File Transfer",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "15",
      "rateUnit": "",
      "burstR": "16",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "1",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:10.0",
      "ORANGE-1:10.1",
      "ORANGE-1:10.2",
      "ORANGE-1:10.3"
    ],
    "qDisc": "ORANGE-12:0"
  },
  "ORANGE-1:10.0": {
    "comment": "DST FTP",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "20/fffe"
      }
    ]
  },
  "ORANGE-1:10.1": {
    "comment": "SRC FTP",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "20/fffe"
      }
    ]
  },
  "ORANGE-1:11": {
    "comment": "DNS",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "0",
      "rate": "1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "1",
      "ceilUnit": "",
      "burstC": "1500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:11.0",
      "ORANGE-1:11.1"
    ],
    "qDisc": "ORANGE-11:0"
  },
  "ORANGE-1:12": {
    "comment": "Instant Messaging",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "2",
      "rate": "1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "1",
      "ceilUnit": "",
      "burstC": "1500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:12.0",
      "ORANGE-1:12.1",
      "ORANGE-1:12.2",
      "ORANGE-1:12.3",
      "ORANGE-1:12.4",
      "ORANGE-1:12.5",
      "ORANGE-1:12.6",
      "ORANGE-1:12.7"
    ],
    "qDisc": "ORANGE-10:0"
  },
  "ORANGE-1:12.0": {
    "comment": "SRC 5190-5191",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5190/fffe"
      }
    ]
  },
  "ORANGE-1:12.1": {
    "comment": "DST 5190-5191",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5190/fffe"
      }
    ]
  },
  "ORANGE-1:12.2": {
    "comment": "SRC 5192-5193",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5192/fffe"
      }
    ]
  },
  "ORANGE-1:12.3": {
    "comment": "DST 5192-5193",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5192/fffe"
      }
    ]
  },
  "ORANGE-1:12.4": {
    "comment": "SRC 1863",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1863/ffff"
      }
    ]
  },
  "ORANGE-1:12.5": {
    "comment": "DST 1863",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1863/ffff"
      }
    ]
  },
  "ORANGE-1:12.6": {
    "comment": "SRC 6880-6911 (6891-6901)",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "6880/ffe0"
      }
    ]
  },
  "ORANGE-1:12.7": {
    "comment": "DST 6880-6911 (6891-6901)",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "6880/ffe0"
      }
    ]
  },
  "ORANGE-1:13": {
    "comment": "Web Surfing",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "125",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "250",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:13.0",
      "ORANGE-1:13.1",
      "ORANGE-1:13.2",
      "ORANGE-1:13.3",
      "ORANGE-1:13.4",
      "ORANGE-1:13.5",
      "ORANGE-1:13.6",
      "ORANGE-1:13.7",
      "ORANGE-1:13.8",
      "ORANGE-1:13.9",
      "ORANGE-1:13.10",
      "ORANGE-1:13.11",
      "ORANGE-1:13.12",
      "ORANGE-1:13.13",
      "ORANGE-1:13.14",
      "ORANGE-1:13.15"
    ],
    "qDisc": "ORANGE-9:0"
  },
  "ORANGE-1:13.0": {
    "comment": "SRC 80",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "80/ffff"
      }
    ]
  },
  "ORANGE-1:13.1": {
    "comment": "DST 80",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "80/ffff"
      }
    ]
  },
  "ORANGE-1:13.2": {
    "comment": "SRC 443",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "443/ffff"
      }
    ]
  },
  "ORANGE-1:13.3": {
    "comment": "DST 443",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "443/ffff"
      }
    ]
  },
  "ORANGE-1:13.4": {
    "comment": "SRC 800",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "800/ffff"
      }
    ]
  },
  "ORANGE-1:13.5": {
    "comment": "DST 800",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "800/ffff"
      }
    ]
  },
  "ORANGE-1:13.6": {
    "comment": "SRC 3128",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "3128/ffff"
      }
    ]
  },
  "ORANGE-1:13.7": {
    "comment": "DST 3128",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "3128/ffff"
      }
    ]
  },
  "ORANGE-1:13.8": {
    "comment": "SRC 8008",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8008/ffff"
      }
    ]
  },
  "ORANGE-1:13.9": {
    "comment": "DST 8008",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8008/ffff"
      }
    ]
  },
  "ORANGE-1:13.10": {
    "comment": "SRC 8080",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8080/ffff"
      }
    ]
  },
  "ORANGE-1:13.11": {
    "comment": "DST 8080",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8080/ffff"
      }
    ]
  },
  "ORANGE-1:14": {
    "comment": "Email",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "250",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "1000",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:14.0",
      "ORANGE-1:14.1",
      "ORANGE-1:14.2",
      "ORANGE-1:14.3",
      "ORANGE-1:14.4",
      "ORANGE-1:14.5",
      "ORANGE-1:14.6",
      "ORANGE-1:14.7",
      "ORANGE-1:14.8",
      "ORANGE-1:14.9",
      "ORANGE-1:14.10",
      "ORANGE-1:14.11"
    ],
    "qDisc": "ORANGE-8:0"
  },
  "ORANGE-1:14.0": {
    "comment": "SRC 25",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "25/ffff"
      }
    ]
  },
  "ORANGE-1:14.1": {
    "comment": "DST 25",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "25/ffff"
      }
    ]
  },
  "ORANGE-1:14.2": {
    "comment": "SRC 110",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "110/ffff"
      }
    ]
  },
  "ORANGE-1:14.3": {
    "comment": "DST 110",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "110/ffff"
      }
    ]
  },
  "ORANGE-1:14.4": {
    "comment": "SRC 143",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "443/ffff"
      }
    ]
  },
  "ORANGE-1:14.5": {
    "comment": "DST 143",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "443/ffff"
      }
    ]
  },
  "ORANGE-1:14.6": {
    "comment": "SRC 993",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "993/ffff"
      }
    ]
  },
  "ORANGE-1:14.7": {
    "comment": "DST 993",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "993/ffff"
      }
    ]
  },
  "ORANGE-1:15": {
    "comment": "SSH",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "8",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:15.0",
      "ORANGE-1:15.1",
      "ORANGE-1:15.2",
      "ORANGE-1:15.3"
    ],
    "qDisc": "ORANGE-7:0"
  },
  "ORANGE-1:15.0": {
    "comment": "SRC 22",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "22/ffff"
      }
    ]
  },
  "ORANGE-1:15.1": {
    "comment": "DST 22",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "22/ffff"
      }
    ]
  },
  "ORANGE-1:16": {
    "comment": "VoIP",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "0",
      "rate": ".1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": ".3",
      "ceilUnit": "",
      "burstC": "4500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:16.0",
      "ORANGE-1:16.1",
      "ORANGE-1:16.2",
      "ORANGE-1:16.3",
      "ORANGE-1:16.4",
      "ORANGE-1:16.5",
      "ORANGE-1:16.6",
      "ORANGE-1:16.7"
    ],
    "qDisc": "ORANGE-6:0"
  },
  "ORANGE-1:16.0": {
    "comment": "SRC 1503",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1503/ffff"
      }
    ]
  },
  "ORANGE-1:16.1": {
    "comment": "DST 1503",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1503/ffff"
      }
    ]
  },
  "ORANGE-1:16.2": {
    "comment": "SRC 1720",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1720/ffff"
      }
    ]
  },
  "ORANGE-1:16.3": {
    "comment": "DST 1720",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1720/ffff"
      }
    ]
  },
  "ORANGE-1:16.4": {
    "comment": "SRC 3782",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "3782/ffff"
      }
    ]
  },
  "ORANGE-1:16.5": {
    "comment": "DST 3782",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "3782/ffff"
      }
    ]
  },
  "ORANGE-1:16.6": {
    "comment": "SRC 5056-5071 (5060-5070)",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5056/fff0"
      }
    ]
  },
  "ORANGE-1:16.7": {
    "comment": "DST 5056-5071 (5060-5070)",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5056/fff0"
      }
    ]
  },
  "ORANGE-1:13.12": {
    "comment": "SRC 441",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "441/ffff"
      }
    ]
  },
  "ORANGE-1:13.13": {
    "comment": "DST 441",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "441/ffff"
      }
    ]
  },
  "ORANGE-1:15.2": {
    "comment": "SRC 8627",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8627/ffff"
      }
    ]
  },
  "ORANGE-1:15.3": {
    "comment": "DST 8627",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8627/ffff"
      }
    ]
  },
  "ORANGE-4:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:9",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-6:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:16",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "1500"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-7:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:15",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-8:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-9:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-10:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:12",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-11:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:11",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-12:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:10",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-1:5.1": {
    "comment": "SRC 441",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:5",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "441/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$ORANGE_ADDRESS"
      }
    ]
  },
  "ORANGE-1:13.14": {
    "comment": "WebPro Dst 2083",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "2083/ffff"
      }
    ]
  },
  "ORANGE-1:13.15": {
    "comment": "WebPro Src 2083",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "2083/ffff"
      }
    ]
  },
  "ORANGE-1:14.8": {
    "comment": "SRC gmail POP3",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "995/ffff"
      }
    ]
  },
  "ORANGE-1:14.9": {
    "comment": "DST gmail POP3",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "995/ffff"
      }
    ]
  },
  "ORANGE-1:14.10": {
    "comment": "SRC 587",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "587/ffff"
      }
    ]
  },
  "ORANGE-1:14.11": {
    "comment": "DST 587",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "587/ffff"
      }
    ]
  },
  "ORANGE-1:18": {
    "comment": "VPN",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "9",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "18",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "ORANGE-1:19",
      "ORANGE-1:20",
      "ORANGE-1:21"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "ORANGE-1:19": {
    "comment": "PPTP",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:19.0",
      "ORANGE-1:19.1"
    ],
    "qDisc": "ORANGE-13:0"
  },
  "ORANGE-1:19.0": {
    "comment": "SRC 1732",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:19",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1732/ffff"
      }
    ]
  },
  "ORANGE-1:19.1": {
    "comment": "GRE",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:19",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "47/ff"
      }
    ]
  },
  "ORANGE-1:20": {
    "comment": "IPSEC",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:20.0",
      "ORANGE-1:20.1",
      "ORANGE-1:20.2",
      "ORANGE-1:20.3",
      "ORANGE-1:20.4",
      "ORANGE-1:20.5"
    ],
    "qDisc": "ORANGE-14:0"
  },
  "ORANGE-1:20.0": {
    "comment": "ESP",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "50/ff"
      }
    ]
  },
  "ORANGE-1:20.1": {
    "comment": "AH",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "51/ff"
      }
    ]
  },
  "ORANGE-1:20.2": {
    "comment": "IKE SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "500/ffff"
      }
    ]
  },
  "ORANGE-1:20.3": {
    "comment": "NAT-T SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "4500/ffff"
      }
    ]
  },
  "ORANGE-1:20.4": {
    "comment": "NAT-T DST",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "4500/ffff"
      }
    ]
  },
  "ORANGE-1:20.5": {
    "comment": "IKE DST",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "500/ffff"
      }
    ]
  },
  "ORANGE-1:21": {
    "comment": "OpenVPN",
    "view": "closed",
    "state": "active",
    "parent_rule": "ORANGE-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "ORANGE-1:21.0",
      "ORANGE-1:21.1",
      "ORANGE-1:21.2",
      "ORANGE-1:21.3"
    ],
    "qDisc": "ORANGE-15:0"
  },
  "ORANGE-1:21.0": {
    "comment": "SRC 1194",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1194/ffff"
      }
    ]
  },
  "ORANGE-1:21.1": {
    "comment": "DST 1194",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1194/ffff"
      }
    ]
  },
  "ORANGE-1:21.2": {
    "comment": "SRC 1195",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1195/ffff"
      }
    ]
  },
  "ORANGE-1:21.3": {
    "comment": "DST 1195",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1195/ffff"
      }
    ]
  },
  "ORANGE-13:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:19",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-14:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:20",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-15:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:21",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-1:10.2": {
    "comment": "SRC GIT",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "9418/ffff"
      }
    ]
  },
  "ORANGE-1:10.3": {
    "comment": "DST GIT",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "9418/ffff"
      }
    ]
  },
  "ORANGE-1:11.0": {
    "comment": "SRC DNS UDP",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:11",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "53/ffff"
      },
      {
        "type": "protocol",
        "value": "15/ff"
      }
    ]
  },
  "ORANGE-1:11.1": {
    "comment": "DST DNS UDP",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE-1:11",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "53/ffff"
      },
      {
        "type": "protocol",
        "value": "15/ff"
      }
    ]
  },
  "RED-1:0": {
    "comment": "Throttled With Exceptions",
    "view": "open",
    "state": "active",
    "parent_rule": "RED",
    "pending_action": "",
    "type": "qdisc",
    "family": "htb",
    "params": {
      "qdefault": "9"
    },
    "qClasses": [
      "RED-1:2",
      "RED-1:9"
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "22"
  },
  "RED-1:2": {
    "comment": "Unthrottled Exceptions",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:0",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:0",
      "priority": "1",
      "rate": "3.0",
      "rateUnit": "Mbit/s",
      "burstR": "30",
      "burstRUnit": "kB"
    },
    "qClasses": [
      "RED-1:3",
      "RED-1:6"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "RED-1:3": {
    "comment": "Admin",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:2",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:2",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "RED-1:4",
      "RED-1:5"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "RED-1:4": {
    "comment": "SSH",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:3",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:3",
      "priority": "1",
      "rate": "30",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6000",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:4.0"
    ],
    "qDisc": ""
  },
  "RED-1:5": {
    "comment": "HTTP",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:3",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:3",
      "priority": "1",
      "rate": "70",
      "rateUnit": "",
      "burstR": "3000",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:5.0",
      "RED-1:5.1"
    ],
    "qDisc": ""
  },
  "RED-1:4.0": {
    "comment": "SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:4",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "0"
    },
    "matches": [
      {
        "type": "sport",
        "value": "222/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$RED_ADDRESS"
      }
    ]
  },
  "RED-1:5.0": {
    "comment": "SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:5",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "0"
    },
    "matches": [
      {
        "type": "sport",
        "value": "81/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$RED_ADDRESS"
      }
    ]
  },
  "RED-1:6": {
    "comment": "Through Traffic",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:2",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:2",
      "priority": "1",
      "rate": "80",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "15",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "RED-1:10",
      "RED-1:11",
      "RED-1:12",
      "RED-1:13",
      "RED-1:14",
      "RED-1:15",
      "RED-1:16",
      "RED-1:18"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "RED-1:9": {
    "comment": "Throttled Traffic",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:0",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:0",
      "priority": "7",
      "rate": "56",
      "rateUnit": "kbit/s",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "",
      "ceilUnit": "",
      "burstC": "",
      "burstCUnit": ""
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "RED-4:0"
  },
  "RED-1:10": {
    "comment": "File Transfer",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "15",
      "rateUnit": "",
      "burstR": "16",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "1",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:10.0",
      "RED-1:10.1",
      "RED-1:10.2",
      "RED-1:10.3"
    ],
    "qDisc": "RED-12:0"
  },
  "RED-1:10.0": {
    "comment": "DST FTP",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "20/fffe"
      }
    ]
  },
  "RED-1:10.1": {
    "comment": "SRC FTP",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "20/fffe"
      }
    ]
  },
  "RED-1:11": {
    "comment": "DNS",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "0",
      "rate": "1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "1",
      "ceilUnit": "",
      "burstC": "1500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:11.0",
      "RED-1:11.1"
    ],
    "qDisc": "RED-11:0"
  },
  "RED-1:12": {
    "comment": "Instant Messaging",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "2",
      "rate": "1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "1",
      "ceilUnit": "",
      "burstC": "1500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:12.0",
      "RED-1:12.1",
      "RED-1:12.2",
      "RED-1:12.3",
      "RED-1:12.4",
      "RED-1:12.5",
      "RED-1:12.6",
      "RED-1:12.7"
    ],
    "qDisc": "RED-10:0"
  },
  "RED-1:12.0": {
    "comment": "SRC 5190-5191",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5190/fffe"
      }
    ]
  },
  "RED-1:12.1": {
    "comment": "DST 5190-5191",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5190/fffe"
      }
    ]
  },
  "RED-1:12.2": {
    "comment": "SRC 5192-5193",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5192/fffe"
      }
    ]
  },
  "RED-1:12.3": {
    "comment": "DST 5192-5193",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5192/fffe"
      }
    ]
  },
  "RED-1:12.4": {
    "comment": "SRC 1863",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1863/ffff"
      }
    ]
  },
  "RED-1:12.5": {
    "comment": "DST 1863",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1863/ffff"
      }
    ]
  },
  "RED-1:12.6": {
    "comment": "SRC 6880-6911 (6891-6901)",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "6880/ffe0"
      }
    ]
  },
  "RED-1:12.7": {
    "comment": "DST 6880-6911 (6891-6901)",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "6880/ffe0"
      }
    ]
  },
  "RED-1:13": {
    "comment": "Web Surfing",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "125",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "250",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:13.0",
      "RED-1:13.1",
      "RED-1:13.2",
      "RED-1:13.3",
      "RED-1:13.4",
      "RED-1:13.5",
      "RED-1:13.6",
      "RED-1:13.7",
      "RED-1:13.8",
      "RED-1:13.9",
      "RED-1:13.10",
      "RED-1:13.11",
      "RED-1:13.12",
      "RED-1:13.13",
      "RED-1:13.14",
      "RED-1:13.15"
    ],
    "qDisc": "RED-9:0"
  },
  "RED-1:13.0": {
    "comment": "SRC 80",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "80/ffff"
      }
    ]
  },
  "RED-1:13.1": {
    "comment": "DST 80",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "80/ffff"
      }
    ]
  },
  "RED-1:13.2": {
    "comment": "SRC 443",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "443/ffff"
      }
    ]
  },
  "RED-1:13.3": {
    "comment": "DST 443",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "443/ffff"
      }
    ]
  },
  "RED-1:13.4": {
    "comment": "SRC 800",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "800/ffff"
      }
    ]
  },
  "RED-1:13.5": {
    "comment": "DST 800",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "800/ffff"
      }
    ]
  },
  "RED-1:13.6": {
    "comment": "SRC 3128",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "3128/ffff"
      }
    ]
  },
  "RED-1:13.7": {
    "comment": "DST 3128",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "3128/ffff"
      }
    ]
  },
  "RED-1:13.8": {
    "comment": "SRC 8008",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8008/ffff"
      }
    ]
  },
  "RED-1:13.9": {
    "comment": "DST 8008",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8008/ffff"
      }
    ]
  },
  "RED-1:13.10": {
    "comment": "SRC 8080",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8080/ffff"
      }
    ]
  },
  "RED-1:13.11": {
    "comment": "DST 8080",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8080/ffff"
      }
    ]
  },
  "RED-1:14": {
    "comment": "Email",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "250",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "1000",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:14.0",
      "RED-1:14.1",
      "RED-1:14.2",
      "RED-1:14.3",
      "RED-1:14.4",
      "RED-1:14.5",
      "RED-1:14.6",
      "RED-1:14.7",
      "RED-1:14.8",
      "RED-1:14.9",
      "RED-1:14.10",
      "RED-1:14.11"
    ],
    "qDisc": "RED-8:0"
  },
  "RED-1:14.0": {
    "comment": "SRC 25",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "25/ffff"
      }
    ]
  },
  "RED-1:14.1": {
    "comment": "DST 25",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "25/ffff"
      }
    ]
  },
  "RED-1:14.2": {
    "comment": "SRC 110",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "110/ffff"
      }
    ]
  },
  "RED-1:14.3": {
    "comment": "DST 110",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "110/ffff"
      }
    ]
  },
  "RED-1:14.4": {
    "comment": "SRC 143",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "443/ffff"
      }
    ]
  },
  "RED-1:14.5": {
    "comment": "DST 143",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "443/ffff"
      }
    ]
  },
  "RED-1:14.6": {
    "comment": "SRC 993",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "993/ffff"
      }
    ]
  },
  "RED-1:14.7": {
    "comment": "DST 993",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "993/ffff"
      }
    ]
  },
  "RED-1:15": {
    "comment": "SSH",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "8",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:15.0",
      "RED-1:15.1",
      "RED-1:15.2",
      "RED-1:15.3"
    ],
    "qDisc": "RED-7:0"
  },
  "RED-1:15.0": {
    "comment": "SRC 22",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "22/ffff"
      }
    ]
  },
  "RED-1:15.1": {
    "comment": "DST 22",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "22/ffff"
      }
    ]
  },
  "RED-1:16": {
    "comment": "VoIP",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "0",
      "rate": ".1",
      "rateUnit": "",
      "burstR": "1500",
      "burstRUnit": "Bytes",
      "ceil": ".3",
      "ceilUnit": "",
      "burstC": "4500",
      "burstCUnit": "Bytes"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:16.0",
      "RED-1:16.1",
      "RED-1:16.2",
      "RED-1:16.3",
      "RED-1:16.4",
      "RED-1:16.5",
      "RED-1:16.6",
      "RED-1:16.7"
    ],
    "qDisc": "RED-6:0"
  },
  "RED-1:16.0": {
    "comment": "SRC 1503",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1503/ffff"
      }
    ]
  },
  "RED-1:16.1": {
    "comment": "DST 1503",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1503/ffff"
      }
    ]
  },
  "RED-1:16.2": {
    "comment": "SRC 1720",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1720/ffff"
      }
    ]
  },
  "RED-1:16.3": {
    "comment": "DST 1720",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1720/ffff"
      }
    ]
  },
  "RED-1:16.4": {
    "comment": "SRC 3782",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "3782/ffff"
      }
    ]
  },
  "RED-1:16.5": {
    "comment": "DST 3782",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "3782/ffff"
      }
    ]
  },
  "RED-1:16.6": {
    "comment": "SRC 5056-5071 (5060-5070)",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "5056/fff0"
      }
    ]
  },
  "RED-1:16.7": {
    "comment": "DST 5056-5071 (5060-5070)",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "5056/fff0"
      }
    ]
  },
  "RED-1:13.12": {
    "comment": "SRC 441",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "441/ffff"
      }
    ]
  },
  "RED-1:13.13": {
    "comment": "DST 441",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "441/ffff"
      }
    ]
  },
  "RED-1:15.2": {
    "comment": "SRC 8627",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "8627/ffff"
      }
    ]
  },
  "RED-1:15.3": {
    "comment": "DST 8627",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:15",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "8627/ffff"
      }
    ]
  },
  "RED-4:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:9",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-6:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:16",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "1500"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-7:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:15",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-8:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-9:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-10:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:12",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-11:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:11",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-12:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:10",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-1:5.1": {
    "comment": "SRC 441",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:5",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "441/ffff"
      },
      {
        "type": "saddrhost",
        "value": "$RED_ADDRESS"
      }
    ]
  },
  "RED-1:13.14": {
    "comment": "WebPro Dst 2083",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "2083/ffff"
      }
    ]
  },
  "RED-1:13.15": {
    "comment": "WebPro Src 2083",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:13",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "2083/ffff"
      }
    ]
  },
  "RED-1:14.8": {
    "comment": "SRC gmail POP3",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "995/ffff"
      }
    ]
  },
  "RED-1:14.9": {
    "comment": "DST gmail POP3",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "995/ffff"
      }
    ]
  },
  "RED-1:14.10": {
    "comment": "SRC 587",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "587/ffff"
      }
    ]
  },
  "RED-1:14.11": {
    "comment": "DST 587",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:14",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "587/ffff"
      }
    ]
  },
  "RED-1:18": {
    "comment": "VPN",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:6",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:6",
      "priority": "1",
      "rate": "20",
      "rateUnit": "",
      "burstR": "9",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "18",
      "burstCUnit": "kB"
    },
    "qClasses": [
      "RED-1:19",
      "RED-1:20",
      "RED-1:21"
    ],
    "qFilters": [
    ],
    "qDisc": ""
  },
  "RED-1:19": {
    "comment": "PPTP",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:19.0",
      "RED-1:19.1"
    ],
    "qDisc": "RED-13:0"
  },
  "RED-1:19.0": {
    "comment": "SRC 1732",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:19",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1732/ffff"
      }
    ]
  },
  "RED-1:19.1": {
    "comment": "GRE",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:19",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "47/ff"
      }
    ]
  },
  "RED-1:20": {
    "comment": "IPSEC",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:20.0",
      "RED-1:20.1",
      "RED-1:20.2",
      "RED-1:20.3",
      "RED-1:20.4",
      "RED-1:20.5"
    ],
    "qDisc": "RED-14:0"
  },
  "RED-1:20.0": {
    "comment": "ESP",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "50/ff"
      }
    ]
  },
  "RED-1:20.1": {
    "comment": "AH",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "protocol",
        "value": "51/ff"
      }
    ]
  },
  "RED-1:20.2": {
    "comment": "IKE SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "500/ffff"
      }
    ]
  },
  "RED-1:20.3": {
    "comment": "NAT-T SRC",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "4500/ffff"
      }
    ]
  },
  "RED-1:20.4": {
    "comment": "NAT-T DST",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "4500/ffff"
      }
    ]
  },
  "RED-1:20.5": {
    "comment": "IKE DST",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:20",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "500/ffff"
      }
    ]
  },
  "RED-1:21": {
    "comment": "OpenVPN",
    "view": "closed",
    "state": "active",
    "parent_rule": "RED-1:18",
    "pending_action": "",
    "type": "class",
    "family": "htb",
    "params": {
      "parent": "1:18",
      "priority": "1",
      "rate": "33",
      "rateUnit": "",
      "burstR": "3",
      "burstRUnit": "kB",
      "ceil": "100",
      "ceilUnit": "",
      "burstC": "6",
      "burstCUnit": "kB"
    },
    "qClasses": [
    ],
    "qFilters": [
      "RED-1:21.0",
      "RED-1:21.1",
      "RED-1:21.2",
      "RED-1:21.3"
    ],
    "qDisc": "RED-15:0"
  },
  "RED-1:21.0": {
    "comment": "SRC 1194",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1194/ffff"
      }
    ]
  },
  "RED-1:21.1": {
    "comment": "DST 1194",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1194/ffff"
      }
    ]
  },
  "RED-1:21.2": {
    "comment": "SRC 1195",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "1195/ffff"
      }
    ]
  },
  "RED-1:21.3": {
    "comment": "DST 1195",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:21",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "1195/ffff"
      }
    ]
  },
  "RED-13:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:19",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-14:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:20",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-15:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:21",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "10000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-1:10.2": {
    "comment": "SRC GIT",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "9418/ffff"
      }
    ]
  },
  "RED-1:10.3": {
    "comment": "DST GIT",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "9418/ffff"
      }
    ]
  },
  "RED-1:11.0": {
    "comment": "SRC DNS UDP",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:11",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "53/ffff"
      },
      {
        "type": "protocol",
        "value": "15/ff"
      }
    ]
  },
  "RED-1:11.1": {
    "comment": "DST DNS UDP",
    "view": "open",
    "state": "active",
    "parent_rule": "RED-1:11",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "dport",
        "value": "53/ffff"
      },
      {
        "type": "protocol",
        "value": "15/ff"
      }
    ]
  },
  "GREEN-1:10.4": {
    "comment": "28656",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN-1:10",
    "pending_action": "",
    "type": "filter",
    "family": "u32",
    "params": {
      "protocol": "ip",
      "parent": "1:0",
      "class": "1:0",
      "priority": "1"
    },
    "matches": [
      {
        "type": "sport",
        "value": "28656/ffff"
      }
    ]
  }
};

