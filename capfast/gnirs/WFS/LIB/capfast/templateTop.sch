[schematic2]
uniq 47
[tools]
[detail]
n -480 384 1408 1440 180
@ @
@1@ This is a major kluge.  This file is used by genTop as a template
@2@ to generate top level capfast diagrams.  The symbols described below
@3@ are automatically defined at setup time, by replacing them with
@4@ the value of a shell variable from config.par.  The contents of
@5@ this comment block are also changed, as well as the name of the
@6@ symbol file.  Do not change this file unless you are SURE that you
@7@ know what the configuration scripts are doing!
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
p 2192 448 200 1536 -1 file:templateTop.sch
p -432 384 100 1280 -1 id:
use dummy 704 2464 100 768 dummy
xform 0 1040 2128
p 704 1760 100 768 1 set0:realtop xxxx:
p 704 1728 100 768 1 set1:name XXXX
p 704 1696 100 768 1 set2:top xxxx:
p 704 1664 100 768 1 set3:sadtop xxxx:
p 704 1632 100 768 1 set4:dctop xxxx:
p 704 1600 100 768 1 set5:dcsadtop xxxx:
p 704 1568 100 768 1 set6:agtop xx:
[comments]
