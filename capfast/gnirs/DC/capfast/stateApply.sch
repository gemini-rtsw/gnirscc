[schematic2]
uniq 438
[tools]
[detail]
w -1118 -789 100 0 n#437 continueCad.continueCad#431.MESS -64 -224 32 -224 32 -800 -2208 -800 -2208 -320 -2000 -320 -2000 -324 eapply.actApply.INMF
w -1118 -821 100 0 n#436 continueCad.continueCad#431.VAL -64 -192 64 -192 64 -832 -2240 -832 -2240 -288 -2000 -288 -2000 -292 eapply.actApply.INPF
w -814 -437 100 0 n#435 eapply.actApply.OUTF -1616 -292 -1152 -292 -1152 -448 -416 -448 -416 -192 -224 -192 continueCad.continueCad#431.DIR
w -1422 -661 100 0 n#434 pauseCad.pauseCad#430.MESS -832 -224 -736 -224 -736 -672 -2048 -672 -2048 -256 -2000 -256 -2000 -260 eapply.actApply.INME
w -1422 -693 100 0 n#433 pauseCad.pauseCad#430.VAL -832 -192 -704 -192 -704 -704 -2080 -704 -2080 -224 -2000 -224 -2000 -228 eapply.actApply.INPE
w -1366 -217 100 0 n#432 eapply.actApply.OUTE -1616 -228 -1056 -228 -1056 -192 -992 -192 pauseCad.pauseCad#430.DIR
w -2152 -185 100 0 n#374 eapply.actApply.INMD -2000 -196 -2244 -196 -2244 996 64 996 64 232 -68 232 -68 228 stopCad.stopCad#427.MESS
w -1120 971 100 0 n#368 eapply.actApply.INPD -2000 -164 -2208 -164 -2208 960 28 960 28 260 -68 260 stopCad.stopCad#427.VAL
w -1472 943 100 0 n#366 eapply.actApply.INMC -2000 -132 -2176 -132 -2176 932 -708 932 -708 228 -832 228 abortCad.abortCad#426.MESS
w -1472 907 100 0 n#365 eapply.actApply.INPC -2000 -100 -2148 -100 -2148 896 -736 896 -736 256 -832 256 -832 260 abortCad.abortCad#426.VAL
w -1088 875 100 0 n#364 eapply.actApply.INMB -2000 -68 -2112 -68 -2112 864 -4 864 -4 680 -64 680 -64 676 parkCad.parkCad#424.MESS
w -1536 219 100 0 n#363 eapply.actApply.MESS -1616 92 -1540 92 -1540 356 -1760 356 -1760 464 -1640 464 outhier.MESS.p
w -1788 437 100 0 n#362 eapply.actApply.VAL -1616 124 -1568 124 -1568 324 -1792 324 -1792 560 -1644 560 outhier.VAL.p
w -1086 843 100 0 n#361 eapply.actApply.INPB -2000 -36 -2080 -36 -2080 832 -32 832 -32 708 -64 708 parkCad.parkCad#424.VAL
w -1440 811 100 0 n#360 eapply.actApply.INMA -2000 -4 -2052 -4 -2052 800 -768 800 -768 672 -832 672 -832 676 observeCad.observeCad#425.MESS
w -1440 779 100 0 n#359 eapply.actApply.INPA -2000 28 -2020 28 -2020 768 -800 768 -800 704 -832 704 -832 708 observeCad.observeCad#425.VAL
w -814 75 100 0 n#353 eapply.actApply.OUTD -1616 -164 -1152 -164 -1152 64 -416 64 -416 256 -228 256 -228 260 stopCad.stopCad#427.DIR
w -1462 -89 100 0 n#351 eapply.actApply.OUTC -1616 -100 -1248 -100 -1248 260 -992 260 abortCad.abortCad#426.DIR
w -910 523 100 0 n#349 eapply.actApply.OUTB -1616 -36 -1344 -36 -1344 512 -416 512 -416 708 -224 708 parkCad.parkCad#424.DIR
w -1436 361 100 0 n#347 eapply.actApply.OUTA -1616 28 -1440 28 -1440 704 -992 704 -992 708 observeCad.observeCad#425.DIR
w -2210 127 100 0 n#245 inhier.DIR.P -2360 124 -2000 124 eapply.actApply.DIR
[cell use]
use continueCad -224 -345 100 0 continueCad#431
xform 0 -144 -240
use pauseCad -992 -345 100 0 pauseCad#430
xform 0 -912 -240
use CBorder -2520 -1268 -100 0 frame
xform 0 -840 36
p 552 -1136 75 1536 -1 Author:Janet E. Tvedt
p 48 -1132 100 1536 1 Date:10 Jun 97
p 152 -1036 300 1792 -1 Dnumber:
p 280 -1052 150 1536 -1 Title:stateApply.sch
use stopCad -228 107 100 0 stopCad#427
xform 0 -148 212
use abortCad -992 107 100 0 abortCad#426
xform 0 -912 212
use observeCad -992 555 100 0 observeCad#425
xform 0 -912 660
use parkCad -224 555 100 0 parkCad#424
xform 0 -144 660
use notes 132 647 100 0 notes#420
xform 0 388 832
p 160 958 100 0 -1 COMMENT1:This contains system level commands
p 160 926 100 0 -1 COMMENT2:obeyed by the detector controller which
p 160 896 100 0 -1 COMMENT3:cause some specific action to occur.
p 160 864 100 0 -1 COMMENT4:Each command appears on a separate 
p 160 832 100 0 -1 COMMENT5:schematic.  
p 160 800 100 0 -1 COMMENT6:
p 160 768 100 0 -1 COMMENT7:NOTE: The pause and continue commands are
p 160 736 100 0 -1 COMMENT8:rejected, but appear here to allow them
p 160 704 100 0 -1 COMMENT9:to be added easily in a later version.
use outhier -1676 519 100 0 VAL
xform 0 -1660 560
use outhier -1672 423 100 0 MESS
xform 0 -1656 464
use inhier -2376 83 100 0 DIR
xform 0 -2360 124
use eapply -1868 204 100 0 actApply
xform 0 -1808 -148
p -1912 -516 100 0 1 DESC:NAAC active commands APPLY record
p -1912 -548 100 0 1 PV:$(top)
[comments]
