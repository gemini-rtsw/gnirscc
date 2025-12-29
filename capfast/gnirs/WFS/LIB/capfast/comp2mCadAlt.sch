[schematic2]
uniq 957
[tools]
[detail]
w -1344 483 -100 0 CVAL comp2mCadCmd.Move.CVAL -1344 480 -1296 480 bihier.CVAL.p
w -1342 451 -100 0 CFLNK outhier.CFLNK.p -1280 448 -1344 448 comp2mCadCmd.Move.CFLNK
w -2158 491 -100 0 VAL bihier.VAL.p -2144 480 -2112 480 comp2mCadCmd.Move.VAL
w -2152 451 -100 0 MESS bihier.MESS.p -2144 448 -2112 448 comp2mCadCmd.Move.MESS
w -2152 515 -100 0 CLID comp2mCadCmd.Move.CLID -2112 512 -2144 512 bihier.CLID.p
w -2146 547 -100 0 DIR bihier.DIR.p -2144 544 -2112 544 comp2mCadCmd.Move.DIR
s -1632 752 500 0 Template for NIRI schematics
s -192 -608 100 768 motor steps.
s -192 -512 100 768 These records are used to set
s -192 -544 100 768 the coordinate transformation
s -192 -576 100 768 from sky coordinates to
s 80 -1344 400 1280 comp2mCadAlt
s -2240 704 100 768 The alternate move command write to the SnlArg
s -2240 672 100 768 records for $(mech1) and $(mech2)
s -2240 640 100 768 mechanisms.
n 144 512 674 864 100
This schematic contains the Command
Action Directive (CAD) records for a
CICS 2 axis continuous component whose
axes can be moved simultaneously (e.g.
XY table). It contains the following:
.
Datm  - CAD record for datuming the
mechanism.
.
compcMoveAxis  - subschematic containing all
the CAD records for moving one axis.
.
Apply - APPLY record for sequencing all
the mechanism's CAD records.
_
[cell use]
use outhier -1264 448 100 1536 CFLNK
xform 0 -1296 448
use bihier -2176 448 100 2048 MESS
xform 0 -2144 448
use bihier -2176 480 100 2048 VAL
xform 0 -2144 480
use bihier -2176 512 100 2048 CLID
xform 0 -2144 512
use bihier -2176 544 100 2048 DIR
xform 0 -2144 544
use bihier -1264 480 100 1536 CVAL
xform 0 -1296 480
use eais -704 -96 100 768 A22
xform 0 -640 -144
p -704 -224 100 768 1 DESC:Coordinate transformation a22
p -704 -288 100 768 1 EGU:usteps / user unit
p -704 -320 100 768 1 PREC:5
p -704 -256 100 768 1 PV:$(top)$(mech)$(prefix)
use eais -704 160 100 768 A21
xform 0 -640 112
p -704 32 100 768 1 DESC:Coordinate transformation a21
p -704 -32 100 768 1 EGU:usteps / user unit
p -704 -64 100 768 1 PREC:5
p -704 0 100 768 1 PV:$(top)$(mech)$(prefix)
use eais -704 416 100 768 A12
xform 0 -640 368
p -704 288 100 768 1 DESC:Coordinate transformation a12
p -704 224 100 768 1 EGU:usteps / user unit
p -704 192 100 768 1 PREC:5
p -704 256 100 768 1 PV:$(top)$(mech)$(prefix)
use eais -704 672 100 768 A11
xform 0 -640 624
p -704 544 100 768 1 DESC:Coordinate transformation a11
p -704 480 100 768 1 EGU:usteps / user unit
p -704 448 100 768 1 PREC:5
p -704 512 100 768 1 PV:$(top)$(mech)$(prefix)
use eais -192 -96 100 768 I22
xform 0 -128 -144
p -192 -224 100 768 1 DESC:Coordinate inverse transformation a22
p -192 -288 100 768 1 EGU:$(alt) units / ustep
p -192 -320 100 768 1 PREC:5
p -192 -256 100 768 1 PV:$(top)$(mech)$(prefix)
use eais -192 160 100 768 I21
xform 0 -128 112
p -192 32 100 768 1 DESC:Coordinate inverse transformation a21
p -192 -32 100 768 1 EGU:$(alt) units / ustep
p -192 -64 100 768 1 PREC:5
p -192 0 100 768 1 PV:$(top)$(mech)$(prefix)
use eais -192 416 100 768 I12
xform 0 -128 368
p -192 288 100 768 1 DESC:Coordinate inverse transformation a12
p -192 224 100 768 1 EGU:$(alt) units / ustep
p -192 192 100 768 1 PREC:5
p -192 256 100 768 1 PV:$(top)$(mech)$(prefix)
use eais -192 672 100 768 I11
xform 0 -128 624
p -192 544 100 768 1 DESC:Coordinate inverse transformation a11
p -192 480 100 768 1 EGU:$(alt) units / ustep
p -192 448 100 768 1 PREC:5
p -192 512 100 768 1 PV:$(top)$(mech)$(prefix)
use elongins -704 -608 100 768 C2
xform 0 -640 -656
p -704 -736 100 768 1 DESC:Coordinate translation c2
p -704 -800 100 768 1 EGU:ustep
p -704 -768 100 768 1 PV:$(top)$(mech)$(prefix)
use elongins -704 -352 100 768 C1
xform 0 -640 -400
p -704 -480 100 768 1 DESC:Coordinate translation c1
p -704 -544 100 768 1 EGU:ustep
p -704 -512 100 768 1 PV:$(top)$(mech)$(prefix)
use comp2mCadCmd -1968 576 100 1024 Move
xform 0 -1728 512
p -2032 544 100 768 1 set0:cad $(prefix)Move
p -1712 544 100 768 1 set1:op $(opEngMove)
p -2032 512 100 768 1 set2:inam CADcompInit
p -1712 512 100 768 1 set3:snam CADcomp2mMove
p -2112 384 100 768 1 setim:inpm $(top)$(mech)$(prefix)A11
p -2112 352 100 768 1 setin:inpn $(top)$(mech)$(prefix)A12
p -2112 320 100 768 1 setio:inpo $(top)$(mech)$(prefix)A21
p -2112 288 100 768 1 setip:inpp $(top)$(mech)$(prefix)A22
p -2112 256 100 768 1 setiq:inpq $(top)$(mech)$(prefix)C1
p -2112 224 100 768 1 setir:inpr $(top)$(mech)$(prefix)C2
use notes 160 119 100 0 notes#394
xform 0 416 304
p 160 96 100 768 0 author:S.M. Beard
p 176 416 100 768 -1 comment0:To add new commands for this particular
p 176 382 100 768 -1 comment1:type of component, attach more CAD
p 176 350 100 768 -1 comment2:records to the APPLY record here, or
p 176 320 100 768 -1 comment3:add more lower level schematics
p 176 288 100 768 -1 comment4:connected to the APPLY record.
p 416 448 100 1024 -1 title:ICD 14
[comments]
