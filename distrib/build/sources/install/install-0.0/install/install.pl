#! /bin/perl

use Newt;

Newt::Init();
Newt::Cls();

$root_title = "Smoothwall Express 3.0 (Phaeton) Installation";
$rt_hpos = 40 - strlen($root_title)/2;
$rt_vpos = 1;
Newt::DrawRootText($rt_hpos, $rt_vpos, $root_title);
Newt::PushHelpLine("");

$knob = 1;
while (true) {
  switch ($knob) {
    case 1:
      
#A lot of Newt operations...

Newt::Finished();
