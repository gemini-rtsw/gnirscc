[schematic2]
uniq 52
[tools]
[detail]
w 796 -885 100 0 n#51 ecalcs.sp.SDIS 928 -992 800 -992 800 -768 704 -768 ecalcs.se.VAL
w 828 -853 100 0 n#49 ecalcs.se.FLNK 704 -736 832 -736 832 -960 928 -960 ecalcs.sp.SLNK
w 1352 -733 100 0 n#45 ecalcs.sp.FLNK 1216 -736 1536 -736 outhier.SPLK.p
w 72 -701 -100 0 STAT inhier.STAT.P -96 -704 288 -704 288 -576 416 -576 ecalcs.se.INPA
w 284 -261 -100 0 STAT junction 288 -576 288 64 416 64 ecalcs.st.INPB
w 1368 -765 -100 0 SPID junction 1248 -768 1536 -768 outhier.SPID.p
w 1032 -509 -100 0 SPID ecalcs.sp.VAL 1216 -768 1248 -768 1248 -512 864 -512 864 -576 928 -576 ecalcs.sp.INPA
w 864 -93 -100 0 STID junction 736 -96 1040 -96 outhier.STID.p
w 520 163 -100 0 STID ecalcs.st.VAL 704 -96 736 -96 736 160 352 160 352 96 416 96 ecalcs.st.INPA
w 188 -757 -100 0 SPIN inhier.SPIN.P -96 -544 192 -544 192 -960 416 -960 ecalcs.se.SLNK
w 136 -285 -100 0 STIN inhier.STIN.P -96 -288 416 -288 ecalcs.st.SLNK
s 608 -1472 500 0 cadPlus.sch
s -64 -272 100 0 Start input link
s -64 -528 100 0 Stop input link
s -64 -688 100 0 CAD status
s 992 -80 100 512 Start counter
s 1488 -752 100 512 Stop counter
n -672 -224 -320 128 100
This represents the contents
of a hierarchical symbol
"cadPlus", to be used to
count the number of START
and STOP activations of each
CAD record.
_
[cell use]
use ecalcs 928 -1049 100 0 sp
xform 0 1072 -784
p 992 -1072 100 0 1 CALC:A+1
p 640 -898 100 0 0 EGU:count
p 992 -1104 100 0 1 PV:$(top)$(mech)$(cad):
use ecalcs 416 -377 100 0 st
xform 0 560 -112
p 480 -400 100 0 1 CALC:B=0?A+1:A
p 128 -226 100 0 0 EGU:count
p 480 -432 100 0 1 PV:$(top)$(mech)$(cad):
use ecalcs 416 -1049 100 0 se
xform 0 560 -784
p 480 -1072 100 0 1 CALC:!!A
p 480 -1104 100 0 1 PV:$(top)$(mech)$(cad):
use outhier 1552 -768 100 1536 SPID
xform 0 1520 -768
use outhier 1056 -96 100 1536 STID
xform 0 1024 -96
use outhier 1552 -736 100 1536 SPLK
xform 0 1520 -736
use bc200tr -1248 -1656 -100 0 frame
xform 0 432 -352
p 1328 -1488 100 0 1 author:S.M.Beard
p 1552 -1488 100 0 -1 border:C
p 1312 -1520 100 0 1 checked:S.M.Beard
p 1584 -1488 100 0 -1 date:4 Feb 97
p 1552 -1376 100 0 -1 project:Core Instrument Control System
p 1552 -1440 100 0 -1 title:CAD start and stop counter
use inhier -112 -704 100 2048 STAT
xform 0 -96 -704
use inhier -112 -544 100 2048 SPIN
xform 0 -96 -544
use inhier -128 -288 100 2048 STIN
xform 0 -96 -288
[comments]
