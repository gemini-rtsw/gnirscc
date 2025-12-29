[schematic2]
uniq 20
[tools]
[detail]
w 496 1027 -100 0 n#19 bihier.MESS.p 496 1024 544 1024 comp2mCad.cad.MESS
w 496 1059 -100 0 n#18 bihier.VAL.p 496 1056 544 1056 comp2mCad.cad.VAL
w 496 1091 -100 0 n#17 bihier.CLID.p 496 1088 544 1088 comp2mCad.cad.CLID
w 502 1123 -100 0 DIR bihier.DIR.p 496 1120 544 1120 comp2mCad.cad.DIR
w 2112 1123 -100 0 c#10 bihier.OCID.p 2160 1120 2112 1120 comp2Car.car.OCID
w 2118 1155 -100 0 c#9 bihier.OVAL.p 2160 1152 2112 1152 comp2Car.car.OVAL
w 2126 1091 -100 0 c#5 comp2Car.car.FLNK 2112 1088 2176 1088 outhier.FLNK.p
s 2576 -112 400 1280 comp2m
s 976 2048 500 0 Template for NIRI schematics
n 224 1824 576 2176 100
SMALL NOTEBOX
Edit the .sch file and
type your comments in here.
Please leave the dots as
placeholders for blank lines.
.
_
n 224 1408 704 1760 100
LARGE NOTEBOX
Edit the .sch file and type your
comments in here.
Please leave the dots as placeholders
for blank lines.
.
.
.
.
.
.
.
.
.
_
[cell use]
use bihier 464 1024 100 2048 MESS
xform 0 496 1024
use bihier 464 1056 100 2048 VAL
xform 0 496 1056
use bihier 464 1120 100 2048 DIR
xform 0 496 1120
use bihier 464 1088 100 2048 CLID
xform 0 496 1088
use bihier 2192 1120 100 1536 OCID
xform 0 2160 1120
use bihier 2192 1152 100 1536 OVAL
xform 0 2160 1152
use outhier 2192 1088 100 1536 FLNK
xform 0 2160 1088
use comp2Car 1488 1152 100 1024 car
xform 0 1728 1088
use comp2mCad 688 1152 100 1024 cad
xform 0 928 1088
use bc200tr 32 -248 -100 0 1-Axis
xform 0 1712 1056
p 2608 -80 100 0 1 author:S.M.Beard
p 2832 -96 100 0 -1 border:C
p 2608 -112 100 0 1 checked:H.Yamada
p 2864 -96 100 768 -1 date:1999-11-07
p 2848 32 100 0 -1 project:Near Infra-Red Imager
p 2608 32 100 768 1 revised:H.Yamada
p 2832 -128 100 768 -1 revision:1.0
p 2848 -32 100 0 -1 title:1-Axis Mechanism
[comments]
