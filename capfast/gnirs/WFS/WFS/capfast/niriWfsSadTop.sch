[schematic2]
uniq 47
[tools]
[detail]
n -480 384 1408 1440 180
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
@ @ Var        Shell variable  Description
@ @ ========== =============== ======================================
@ @ realtop    $prefix         The default prefix before redefinition
@ @ top        $prefix         The default prefix
@ @ sadtop     $sadtop         The Status and Alarm database prefix
@ @ dctop      $dctop          The detector controller prefix
@ @ dcsadtop   $dcsadtop       The detector controller SAD prefix
@ @ name       $name           Instrument abbreviation (4 characters)
@ @
_
[cell use]
use eborderC -608 231 100 0 eborderC#1
xform 0 1072 1536
p 1968 400 100 1536 -1 author:H.T. Yamada
p 2192 448 200 1536 -1 file:niriWfsSadTop.sch
p -432 384 100 1280 -1 id:
use niriWfsSadSet 704 2464 100 768 niriWfsSadSet
xform 0 1040 2128
p 704 1760 100 768 1 set0:realtop nirs:
p 704 1728 100 768 1 set1:name NIRS
p 704 1696 100 768 1 set2:top nirs:
p 704 1664 100 768 1 set3:sadtop nirs:
p 704 1632 100 768 1 set4:dctop nirs:
p 704 1600 100 768 1 set5:dcsadtop nirs:
p 704 1568 100 768 1 set6:agtop nirs:
[comments]
