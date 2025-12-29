[schematic2]
uniq 47
[tools]
[detail]
n -480 384 1408 1568 180
@ @
@1@ This is a major kluge.  This file is used by genTop as a template
@2@ to generate top level capfast diagrams.  The symbols described below
@3@ are automatically defined at setup time, by replacing them with
@4@ the value of a shell variable.  The contents of this comment
@5@ block are also changed, as well as the name of the symbol file.
@6@ Do not change this file unless you are SURE that you know what the 
@7@ setup scripts are doing!  The consequences can be very far reaching!
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
p 2192 448 200 1536 -1 file:templateTop.sch
p -432 384 100 1280 -1 id:
use dummy 1472 2624 100 768 dummy
xform 0 1776 2304
p 1472 1952 100 768 1 set1:realtop xxxx:
p 1472 1920 100 768 1 set2:name XXXX
p 1472 1888 100 768 1 set3:top xxxx:
p 1472 1856 100 768 1 set4:dctop xxxx:
p 1472 1824 100 768 1 set5:dcsadtop xxxx:
p 1472 1792 100 768 1 set6:sadtop xxxx:
p 1472 1760 100 768 1 set7:tmpscan Passive
p 1472 1728 100 768 1 set8:tmpvar 0
p 1472 1696 100 768 1 set9:tmphb 0
[comments]
