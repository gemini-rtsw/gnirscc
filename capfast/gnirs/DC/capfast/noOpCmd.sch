[schematic2]
uniq 178
[tools]
[detail]
w 1602 939 100 0 n#177 ecars.C.FLNK 1568 928 1696 928 efanouts.Fan.SLNK
w 914 1131 100 0 n#174 eseq.toggle.LNK2 896 1120 992 1120 992 1152 junction
w 1042 1163 100 0 n#174 eseq.toggle.LNK1 896 1152 1248 1152 ecars.C.IVAL
w 242 1131 100 0 n#170 hwin.hwin#168.in 224 1088 224 1120 320 1120 eseq.toggle.DOL2
w 276 1163 100 0 n#169 hwin.hwin#167.in 292 1152 320 1152 eseq.toggle.DOL1
w 20 1423 100 0 n#161 ecad2.cmd.VAL -96 1312 -32 1312 -32 1412 132 1412 132 1408 outhier.VAL.p
w -10 1283 100 0 n#159 ecad2.cmd.MESS -96 1280 136 1280 outhier.MESS.p
w -622 1419 100 0 n#156 inhier.DIR.P -700 1408 -484 1408 -484 1312 -416 1312 ecad2.cmd.DIR
s 12 1108 100 0 CAR_IDLE=0
s 96 1176 100 0 CAR_BUSY=2
[cell use]
use efanouts 1776 1072 100 0 Fan
xform 0 1816 944
p 1776 800 65 0 1 PV:$(top)$(cmd)
p 2016 1008 65 0 1 def(LNK1):$(top)noopCars1.VAL
p 2016 976 65 0 1 def(LNK2):$(top)noopCars2.VAL
p 1936 1008 75 768 -1 pproc(LNK1):PP
p 1936 976 75 768 -1 pproc(LNK2):PP
use ecars 1376 1196 100 0 C
xform 0 1408 1040
p 1360 908 65 0 1 PV:$(top)$(cmd)
p 1044 1104 65 0 1 def(ICID):$(top)apply.CLID
use CBorder -900 -84 -100 0 frame
xform 0 780 1220
p 2172 48 75 1536 -1 Author:Janet E. Tvedt
p 1668 52 100 1536 1 Date:24 Apr 97
p 1772 148 300 1792 -1 Dnumber:
p 1900 132 150 1536 -1 Title:noOpCmd.sch
use hwin 32 1047 100 0 hwin#168
xform 0 128 1088
p 35 1080 100 0 -1 val(in):0
use hwin 100 1111 100 0 hwin#167
xform 0 196 1152
p 103 1144 100 0 -1 val(in):2
use eseq 552 1332 100 0 toggle
xform 0 608 1040
p 640 1150 100 0 1 DLY2:1.5e+00
p 520 792 65 0 1 name:$(top)$(cmd)Seq
p 896 1152 75 768 -1 pproc(LNK1):PP
p 896 1120 75 768 -1 pproc(LNK2):PP
use outhier 104 1239 100 0 MESS
xform 0 120 1280
use outhier 100 1367 100 0 VAL
xform 0 116 1408
use inhier -716 1367 100 0 DIR
xform 0 -700 1408
use ecad2 -268 1356 100 0 cmd
xform 0 -256 1056
p -328 700 100 0 1 SNAM:$(cmd)Proc
p -328 728 100 0 1 name:$(top)$(cmd)
use notes 1512 1803 100 0 notes#154
xform 0 1768 1988
p 2040 1954 100 0 0 AUTHOR:AUTHOR
p 1540 2112 100 0 -1 COMMENT1:Notes: This schematic contains records
p 1540 2080 100 0 -1 COMMENT2:which are replicated for each (cmd).
p 1540 2048 100 0 -1 COMMENT3:The intention was to generalize this
p 1540 2016 100 0 -1 COMMENT4:so that it can be used for any command
p 1540 1984 100 0 -1 COMMENT5:that the detector may legitimately.
p 1540 1952 100 0 -1 COMMENT6:reject or ignore.
p 1540 1920 100 0 -1 COMMENT7:The seq record is used to toggle the CAR
p 1540 1888 100 0 -1 COMMENT8:to BUSY for 1.5 sec then back to IDLE.
p 1540 1856 100 0 -1 COMMENT9:
[comments]
