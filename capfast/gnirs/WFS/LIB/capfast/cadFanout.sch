[schematic2]
uniq 359
[tools]
[detail]
w 114 235 -100 0 OUTA elongouts.OutA.OUT 32 288 96 288 96 208 256 208 outhier.OUTA.p
w -590 -37 100 0 n#356 edfans.Fan.OUTC -768 -48 -352 -48 -352 -224 64 -224 64 -128 32 -128 elongouts.OutC.VAL
w 162 -37 -100 0 OUTC elongouts.OutC.OUT 32 -160 128 -160 128 -48 256 -48 outhier.OUTC.p
w 66 59 -100 0 OUTB elongouts.OutB.OUT 32 48 160 48 160 80 256 80 outhier.OUTB.p
w -350 -5 100 0 n#352 edfans.Fan.OUTB -768 -16 128 -16 128 80 32 80 elongouts.OutB.VAL
w -1534 67 100 0 n#350 link2Dir.link2Dir#304.FLNK -1568 64 -1440 64 -1440 0 -1376 0 elongins.Long.SLNK
w -1342 -157 100 0 n#349 link2Dir.link2Dir#304.OUT -1568 32 -1504 32 -1504 -160 -1120 -160 -1120 -16 junction
w -1102 -5 100 0 n#349 elongins.Long.VAL -1120 -16 -1024 -16 edfans.Fan.DOL
w -158 -301 100 0 n#348 edfans.Fan.OUTE -768 -112 -512 -112 -512 -304 256 -304 outhier.OUTE.p
w -174 -269 100 0 n#347 edfans.Fan.OUTD -768 -80 -480 -80 -480 -272 192 -272 192 -176 256 -176 outhier.OUTD.p
w -222 195 100 0 n#346 edfans.Fan.OUTA -768 16 -512 16 -512 192 128 192 128 320 32 320 elongouts.OutA.VAL
w -1084 -21 100 0 n#332 elongins.Long.FLNK -1120 16 -1088 16 -1088 -48 -1024 -48 edfans.Fan.SLNK
w -1976 -253 100 0 SPLK inhier.SPLK.P -2080 -256 -1824 -256 -1824 -64 -1760 -64 link2Dir.link2Dir#304.SPLK
w -1992 -125 100 0 STLK inhier.STLK.P -2080 -128 -1856 -128 -1856 -32 -1760 -32 link2Dir.link2Dir#304.STLK
w -1944 3 100 0 PNLK inhier.PNLK.P -2080 0 -1760 0 link2Dir.link2Dir#304.PLNK
w -1992 131 100 0 CLNK inhier.CLNK.P -2080 128 -1856 128 -1856 32 -1760 32 link2Dir.link2Dir#304.CLNK
w -1976 259 100 0 MLNK inhier.MLNK.P -2080 256 -1824 256 -1824 64 -1760 64 link2Dir.link2Dir#304.MLNK
s -2048 -240 100 0 Stop link
s -2048 -112 100 0 Start link
s -2048 16 100 0 Preset link
s -2048 144 100 0 Clear link
s -2048 272 100 0 Mark link
s -80 -1232 500 512 cadFanout.sch
s -1072 288 100 0 This "data fanout" record triggers the CAD records of all
s -1072 240 100 0 the subsystems connected to the output simultaneously.
n 96 -1024 576 -672 100
This schematic allows one
incoming link to result in a
directive being sent to up to
five CAD records simultaneously.
.
The schematic is used by the
Instrument Sequencer, for example
to fan commands out to the
Components Controller and
Detector Controller subsystems.
.
The longout's are a kludge, needed
because the dfanout has problems
with records in other ioc's.
_
[cell use]
use elongouts -224 -217 100 0 OutC
xform 0 -96 -128
p -160 -224 100 768 1 PV:$(top)$(cad)
p 32 -160 75 768 -1 pproc(OUT):PP
use elongouts -224 -9 100 0 OutB
xform 0 -96 80
p -160 -16 100 768 1 PV:$(top)$(cad)
p 32 48 75 768 -1 pproc(OUT):PP
use elongouts -224 231 100 0 OutA
xform 0 -96 320
p -160 224 100 768 1 PV:$(top)$(cad)
p 32 288 75 768 -1 pproc(OUT):PP
use outhier 224 -345 100 0 OUTE
xform 0 240 -304
use outhier 224 -217 100 0 OUTD
xform 0 240 -176
use outhier 224 -89 100 0 OUTC
xform 0 240 -48
use outhier 224 39 100 0 OUTB
xform 0 240 80
use outhier 224 167 100 0 OUTA
xform 0 240 208
use inhier -2096 -297 100 0 SPLK
xform 0 -2080 -256
use inhier -2096 -169 100 0 STLK
xform 0 -2080 -128
use inhier -2096 -41 100 0 PNLK
xform 0 -2080 0
use inhier -2096 87 100 0 CLNK
xform 0 -2080 128
use inhier -2096 215 100 0 MLNK
xform 0 -2080 256
use elongins -1376 -73 100 0 Long
xform 0 -1248 0
p -1312 -96 100 0 1 EGU:CAD directive
p -1312 -128 100 0 1 PV:$(top)$(cad)
use edfans -1024 -265 100 0 Fan
xform 0 -896 -48
p -960 -272 100 768 1 EGU:CAD directive
p -960 -304 100 768 1 OMSL:closed_loop
p -960 -336 100 0 1 PV:$(top)$(cad)
p -768 16 75 768 -1 pproc(OUTA):PP
p -768 -16 75 768 -1 pproc(OUTB):PP
p -768 -48 75 768 -1 pproc(OUTC):PP
p -768 -80 75 768 -1 pproc(OUTD):PP
p -768 -112 75 768 -1 pproc(OUTE):PP
p -768 -144 75 768 -1 pproc(OUTF):PP
p -768 -176 75 768 -1 pproc(OUTG):PP
p -768 -208 75 768 -1 pproc(OUTH):PP
use link2Dir -1760 -185 100 0 link2Dir#304
xform 0 -1664 -32
p -1740 -212 100 0 1 set0:mech $(mech)$(cad)
use bc200tr -2560 -1400 -100 0 frame
xform 0 -880 -96
p 0 -1232 100 0 1 author:S.M.Beard
p 240 -1248 100 0 -1 border:C
p 0 -1264 100 0 1 checked:S.M.Beard
p 272 -1248 100 0 -1 date:13 Feb 97
p 524 -1244 100 1792 -1 page:1
p 240 -1120 100 0 -1 project:Core Instrument Control System
p 240 -1184 100 0 -1 title:Fan out control from CAD record
[comments]
