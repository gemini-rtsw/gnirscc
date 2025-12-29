[schematic2]
uniq 47
[tools]
[detail]
n -480 384 1408 1568 180
@ @
@1@ This file was automatically generated from
@2@ templateTop.sch at setup time.  Do not edit
@3@ it by hand.  If you need to change a value,
@4@ change the appropriate environment variable,
@5@ then rerun niriSetup.  See templateTop.sch
@6@ for more details.
@7@ 
@8@
@ @
@ @ Var        Shell Variable  Description
@ @ ========== =============== =========================================
@ @ realtop    $prefix         The default prefix before redefinition
@ @ top        $prefix         The default prefix
@ @ sadtop     $sadtop         The Status and Alarm database prefix
@ @ dctop      $dctop          The detector controller prefix
@ @ dcsadtop   $dcsadtop       The detector controller SAD prefix
@ @ name       $name           Instrument abbreviation (4 characters)
@ @ tmpscan    $tmpscan        How often to monitor tmp interlock
@ @ tmpvar     $tmpvar         What field to monitor for tmp interlock
@ @ tmphb      $tmphb          What field to monitor for tmp heartbeat
@ @
_
[cell use]
use eborderC -608 231 100 0 eborderC#1
xform 0 1072 1536
p 1968 400 100 1536 -1 author:H.T. Yamada
p 2192 448 200 1536 -1 file:ccSimTop.sch
p -432 384 100 1280 -1 id:
use ccSimSet 1472 2624 100 768 ccSimSet
xform 0 1776 2304
p 1472 1952 100 768 1 set1:realtop nirs:
p 1472 1920 100 768 1 set2:name NIRI
p 1472 1888 100 768 1 set3:top nirs:
p 1472 1856 100 768 1 set4:dctop xxxx:
p 1472 1824 100 768 1 set5:dcsadtop xxxx:
p 1472 1792 100 768 1 set6:sadtop xxxx:
p 1472 1760 100 768 1 set7:tmpscan 
p 1472 1728 100 768 1 set8:tmpvar nirs::tempState
p 1472 1696 100 768 1 set9:tmphb nirs::heartBeat
[comments]
