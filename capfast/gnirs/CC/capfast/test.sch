[schematic2]
uniq 422
[tools]
[detail]
w -190 -309 100 0 OUTJ outhier.OUTJ.p 32 -320 -352 -320 -352 64 -640 64 edfans.fan.OUTB
w -174 -181 100 0 OUTI outhier.OUTI.p 32 -192 -320 -192 -320 96 -640 96 edfans.fan.OUTA
w -176 -48 100 0 OUTH outhier.OUTH.p 32 -64 -288 -64 -288 512 -608 512 edfans.edfans#390.OUTH
w -144 64 100 0 OUTG outhier.OUTG.p 32 64 -256 64 -256 544 -608 544 edfans.edfans#390.OUTG
w -160 192 100 0 OUTF outhier.OUTF.p 32 192 -224 192 -224 576 -608 576 edfans.edfans#390.OUTF
w -430 619 100 0 OUTE outhier.OUTE.p 32 320 -192 320 -192 608 -608 608 edfans.edfans#390.OUTE
w -414 651 100 0 OUTD edfans.edfans#390.OUTD -608 640 -160 640 -160 448 32 448 outhier.OUTD.p
w -398 683 100 0 OUTC outhier.OUTC.p 32 560 -128 560 -128 672 -608 672 edfans.edfans#390.OUTC
w -312 707 100 0 OUTB outhier.OUTB.p 32 704 -608 704 edfans.edfans#390.OUTB
w -248 835 100 0 OUTA edfans.edfans#390.OUTA -608 736 -480 736 -480 832 32 832 outhier.OUTA.p
w -2116 -213 100 0 n#411 inhier.SPLK.P -2240 -288 -2112 -288 -2112 -128 -1968 -128 link2Dir.link2Dir#366.SPLK
w -2086 -85 100 0 STLK inhier.STLK.P -2240 -192 -2144 -192 -2144 -96 -1968 -96 link2Dir.link2Dir#366.STLK
w -2102 -53 100 0 PLNK inhier.PLNK.P -2240 -96 -2176 -96 -2176 -64 -1968 -64 link2Dir.link2Dir#366.PLNK
w -2174 11 100 0 CLNK inhier.CLNK.P -2240 0 -2048 0 -2048 -32 -1968 -32 link2Dir.link2Dir#366.CLNK
w -2158 139 100 0 MLNK inhier.MLNK.P -2240 128 -2016 128 -2016 0 -1968 0 link2Dir.link2Dir#366.MLNK
w -798 323 100 0 n#392 edfans.fan.VAL -640 160 -576 160 -576 320 -960 320 -960 704 -864 704 edfans.edfans#390.DOL
w -846 259 100 0 n#391 edfans.fan.FLNK -640 192 -608 192 -608 256 -1024 256 -1024 672 -864 672 edfans.edfans#390.SLNK
w -1134 43 100 0 n#382 elongins.elongins#379.FLNK -1312 32 -896 32 edfans.fan.SLNK
w -1038 67 100 0 n#381 elongins.elongins#379.VAL -1312 0 -1120 0 -1120 64 -896 64 edfans.fan.DOL
w -1470 -125 100 0 n#381 link2Dir.link2Dir#366.OUT -1776 -32 -1664 -32 -1664 -128 -1216 -128 -1216 0 junction
w -1702 19 100 0 n#380 link2Dir.link2Dir#366.FLNK -1776 0 -1776 16 -1568 16 elongins.elongins#379.SLNK
s -80 -1232 500 512 test.sch
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
use inhier -2256 87 100 0 MLNK
xform 0 -2240 128
use inhier -2256 -41 100 0 CLNK
xform 0 -2240 0
use inhier -2256 -137 100 0 PLNK
xform 0 -2240 -96
use inhier -2256 -233 100 0 STLK
xform 0 -2240 -192
use inhier -2256 -329 100 0 SPLK
xform 0 -2240 -288
use outhier 0 791 100 0 OUTA
xform 0 16 832
use outhier 0 663 100 0 OUTB
xform 0 16 704
use outhier 0 519 100 0 OUTC
xform 0 16 560
use outhier 0 407 100 0 OUTD
xform 0 16 448
use outhier 0 279 100 0 OUTE
xform 0 16 320
use outhier 0 151 100 0 OUTF
xform 0 16 192
use outhier 0 23 100 0 OUTG
xform 0 16 64
use outhier 0 -105 100 0 OUTH
xform 0 16 -64
use outhier 0 -233 100 0 OUTI
xform 0 16 -192
use outhier 0 -361 100 0 OUTJ
xform 0 16 -320
use edfans -896 -185 100 0 fan
xform 0 -768 32
p -832 -192 100 768 1 EGU:CAD directive
p -832 -224 100 768 1 OMSL:closed_loop
p -832 -256 100 0 1 PV:$(top)
p -640 96 75 768 -1 pproc(OUTA):PP
p -640 64 75 768 -1 pproc(OUTB):PP
p -640 32 75 768 -1 pproc(OUTC):PP
p -640 0 75 768 -1 pproc(OUTD):PP
p -640 -32 75 768 -1 pproc(OUTE):PP
p -640 -64 75 768 -1 pproc(OUTF):PP
p -640 -96 75 768 -1 pproc(OUTG):PP
p -640 -128 75 768 -1 pproc(OUTH):PP
use edfans -864 455 100 0 edfans#390
xform 0 -736 672
p -800 448 100 768 1 EGU:CAD directive
p -800 416 100 768 1 OMSL:closed_loop
p -800 384 100 0 1 PV:$(top)
p -608 736 75 768 -1 pproc(OUTA):PP
p -608 704 75 768 -1 pproc(OUTB):PP
p -608 672 75 768 -1 pproc(OUTC):PP
p -608 640 75 768 -1 pproc(OUTD):PP
p -608 608 75 768 -1 pproc(OUTE):PP
p -608 576 75 768 -1 pproc(OUTF):PP
p -608 544 75 768 -1 pproc(OUTG):PP
p -608 512 75 768 -1 pproc(OUTH):PP
use elongins -1568 -57 100 0 elongins#379
xform 0 -1440 16
use link2Dir -1968 -249 100 0 link2Dir#366
xform 0 -1872 -96
p -1948 -276 100 0 1 set0:mech testMech
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
