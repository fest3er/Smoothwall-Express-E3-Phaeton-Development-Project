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
    "nextMajor": "2",
    "NATted": ""
  },
  "PURPLE": {
    "gui_color": "purple",
    "view": "open",
    "state": "active",
    "qDisc": "PURPLE-1:0",
    "pending_action": "",
    "nextMajor": "2",
    "NATted": ""
  },
  "ORANGE": {
    "gui_color": "orange",
    "view": "open",
    "state": "active",
    "qDisc": "ORANGE-1:0",
    "pending_action": "",
    "nextMajor": "2",
    "NATted": ""
  },
  "RED": {
    "gui_color": "red",
    "view": "open",
    "state": "active",
    "qDisc": "RED-1:0",
    "pending_action": "",
    "nextMajor": "2",
    "NATted": "1"
  }
};

qRules = {
  "GREEN-1:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "GREEN",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "15000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "PURPLE-1:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "PURPLE",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "15000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "ORANGE-1:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "ORANGE",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "15000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  },
  "RED-1:0": {
    "comment": "Fair",
    "view": "open",
    "state": "active",
    "parent_rule": "RED",
    "pending_action": "",
    "type": "qdisc",
    "family": "sfq",
    "params": {
      "perturb": "10",
      "quantum": "15000"
    },
    "qClasses": [
    ],
    "qFilters": [
    ],
    "qDisc": "",
    "nextMinor": "1"
  }
};

