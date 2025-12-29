[schematic2]
uniq 365
[tools]
[detail]
w -382 -477 100 0 n#364 edfans.Fan.OUTE -896 176 -704 176 -704 -480 0 -480 0 -304 -96 -304 elongouts.OutE.VAL
w -318 -181 100 0 n#363 edfans.Fan.OUTD -896 208 -576 208 -576 -192 0 -192 0 -64 -96 -64 elongouts.OutD.VAL
w 68 -181 100 0 OUTE elongouts.OutE.OUT -96 -336 64 -336 64 -16 128 -16 outhier.OUTE.p
w 36 3 100 0 OUTD elongouts.OutD.OUT -96 -96 32 -96 32 112 128 112 outhier.OUTD.p
w -14 523 -100 0 OUTA elongouts.OutA.OUT -96 576 -32 576 -32 496 128 496 outhier.OUTA.p
w -718 251 100 0 n#356 edfans.Fan.OUTC -896 240 -480 240 -480 64 -64 64 -64 160 -96 160 elongouts.OutC.VAL
w 34 251 -100 0 OUTC elongouts.OutC.OUT -96 128 0 128 0 240 128 240 outhier.OUTC.p
w -62 347 -100 0 OUTB elongouts.OutB.OUT -96 336 32 336 32 368 128 368 outhier.OUTB.p
w -478 283 100 0 n#352 edfans.Fan.OUTB -896 272 0 272 0 368 -96 368 elongouts.OutB.VAL
w -1662 355 100 0 n#350 link2Dir.link2Dir#304.FLNK -1696 352 -1568 352 -1568 288 -1504 288 elongins.Long.SLNK
w -1230 283 100 0 n#349 elongins.Long.VAL -1248 272 -1152 272 edfans.Fan.DOL
w -1470 131 100 0 n#349 link2Dir.link2Dir#304.OUT -1696 320 -1632 320 -1632 128 -1248 128 -1248 272 junction
w -350 483 100 0 n#346 edfans.Fan.OUTA -896 304 -640 304 -640 480 0 480 0 608 -96 608 elongouts.OutA.VAL
w -1212 267 100 0 n#332 elongins.Long.FLNK -1248 304 -1216 304 -1216 240 -1152 240 edfans.Fan.SLNK
w -2104 35 100 0 SPLK inhier.SPLK.P -2208 32 -1952 32 -1952 224 -1888 224 link2Dir.link2Dir#304.SPLK
w -2120 163 100 0 STLK inhier.STLK.P -2208 160 -1984 160 -1984 256 -1888 256 link2Dir.link2Dir#304.STLK
w -2072 291 100 0 PNLK inhier.PNLK.P -2208 288 -1888 288 link2Dir.link2Dir#304.PLNK
w -2120 419 100 0 CLNK inhier.CLNK.P -2208 416 -1984 416 -1984 320 -1888 320 link2Dir.link2Dir#304.CLNK
w -2104 547 100 0 MLNK inhier.MLNK.P -2208 544 -1952 544 -1952 352 -1888 352 link2Dir.link2Dir#304.MLNK
s -1200 528 100 0 the subsystems connected to the output simultaneously.
s -1200 576 100 0 This "data fanout" record triggers the CAD records of all
s -80 -1232 500 512 cadFanout.sch
s -2176 560 100 0 Mark link
s -2176 432 100 0 Clear link
s -2176 304 100 0 Preset link
s -2176 176 100 0 Start link
s -2176 48 100 0 Stop link
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
use elongouts -352 -393 100 0 OutE
xform 0 -224 -304
p -288 -400 100 768 1 PV:$(top)$(cad)
p -96 -336 75 768 -1 pproc(OUT):PP
use elongouts -352 -153 100 0 OutD
xform 0 -224 -64
p -288 -160 100 768 1 PV:$(top)$(cad)
p -96 -96 75 768 -1 pproc(OUT):PP
use elongouts -352 519 100 0 OutA
xform 0 -224 608
p -288 512 100 768 1 PV:$(top)$(cad)
p -96 576 75 768 -1 pproc(OUT):PP
use elongouts -352 279 100 0 OutB
xform 0 -224 368
p -288 272 100 768 1 PV:$(top)$(cad)
p -96 336 75 768 -1 pproc(OUT):PP
use elongouts -352 71 100 0 OutC
xform 0 -224 160
p -288 64 100 768 1 PV:$(top)$(cad)
p -96 128 75 768 -1 pproc(OUT):PP
use outhier 96 455 100 0 OUTA
xform 0 112 496
use outhier 96 327 100 0 OUTB
xform 0 112 368
use outhier 96 199 100 0 OUTC
xform 0 112 240
use outhier 96 71 100 0 OUTD
xform 0 112 112
use outhier 96 -57 100 0 OUTE
xform 0 112 -16
use inhier -2224 503 100 0 MLNK
xform 0 -2208 544
use inhier -2224 375 100 0 CLNK
xform 0 -2208 416
use inhier -2224 247 100 0 PNLK
xform 0 -2208 288
use inhier -2224 119 100 0 STLK
xform 0 -2208 160
use inhier -2224 -9 100 0 SPLK
xform 0 -2208 32
use elongins -1504 215 100 0 Long
xform 0 -1376 288
p -1440 192 100 0 1 EGU:CAD directive
p -1440 160 100 0 1 PV:$(top)$(cad)
use edfans -1152 23 100 0 Fan
xform 0 -1024 240
p -1088 16 100 768 1 EGU:CAD directive
p -1088 -16 100 768 1 OMSL:closed_loop
p -1088 -48 100 0 1 PV:$(top)$(cad)
p -896 304 75 768 -1 pproc(OUTA):PP
p -896 272 75 768 -1 pproc(OUTB):PP
p -896 240 75 768 -1 pproc(OUTC):PP
p -896 208 75 768 -1 pproc(OUTD):PP
p -896 176 75 768 -1 pproc(OUTE):PP
p -896 144 75 768 -1 pproc(OUTF):PP
p -896 112 75 768 -1 pproc(OUTG):PP
p -896 80 75 768 -1 pproc(OUTH):PP
use link2Dir -1888 103 100 0 link2Dir#304
xform 0 -1792 256
p -1868 76 100 0 1 set0:mech $(mech)$(cad)
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
