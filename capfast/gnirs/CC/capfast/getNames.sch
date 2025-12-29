[schematic2]
uniq 99
[tools]
[detail]
w 2932 -21 100 0 n#98 ecad2.getNames.FLNK 1728 672 1888 672 1888 704 2928 704 2928 -736 3792 -736 eseq.getNamesSeq.SLNK
w 3768 -437 100 0 n#96 hwin.hwin#78.in 3792 -448 3792 -448 eseq.getNamesSeq.DOL2
w 2248 651 100 0 n#94 hwout.hwout#19.outp 2272 640 2272 640 cadFan.cadFan#84.OUTA
w 2248 619 100 0 n#93 hwout.hwout#21.outp 2272 608 2272 608 cadFan.cadFan#84.OUTB
w 1880 523 100 0 n#88 ecad2.getNames.SPLK 1728 512 2080 512 cadFan.cadFan#84.SPLK
w 1880 547 100 0 n#95 ecad2.getNames.STLK 1728 544 2080 544 cadFan.cadFan#84.STLK
w 1880 587 100 0 n#87 ecad2.getNames.PLNK 1728 576 2080 576 cadFan.cadFan#84.PLNK
w 1880 619 100 0 n#86 ecad2.getNames.CLNK 1728 608 2080 608 cadFan.cadFan#84.CLNK
w 1880 651 100 0 n#85 ecad2.getNames.MLNK 1728 640 2080 640 cadFan.cadFan#84.MLNK
w 4384 -445 100 0 n#83 eseq.getNamesSeq.LNK2 4368 -448 4448 -448 4448 -368 junction
w 4444 227 100 0 n#83 eseq.getNamesSeq.LNK1 4368 -416 4448 -416 4448 880 3776 880 3776 1264 3904 1264 ecars.getNamesC.IVAL
w 4288 1051 100 0 FLNK ecars.getNamesC.FLNK 4224 1040 4400 1040 outhier.FLNK.p
w 4288 1211 100 0 OMSS ecars.getNamesC.OMSS 4224 1200 4400 1200 outhier.OMSS.p
w 4288 1243 100 0 OCID ecars.getNamesC.CLID 4224 1232 4400 1232 bihier.OCID.p
w 4288 1275 100 0 OVAL ecars.getNamesC.VAL 4224 1264 4400 1264 outhier.OVAL.p
w 3768 -405 100 0 n#34 hwin.hwin#11.in 3792 -416 3792 -416 eseq.getNamesSeq.DOL1
w 2824 1235 100 0 n#15 ecad2.getNames.OCID 1728 928 1792 928 1792 1232 3904 1232 ecars.getNamesC.ICID
w 1822 1035 100 0 VAL ecad2.getNames.VAL 1728 1024 1952 1024 outhier.VAL.p
w 1870 939 100 0 MESS ecad2.getNames.MESS 1728 992 1824 992 1824 928 1952 928 outhier.MESS.p
w 1294 1003 100 0 CLID inhier.CLID.P 1216 992 1408 992 ecad2.getNames.ICID
w 1380 1051 100 0 DIR ecad2.getNames.DIR 1408 1024 1376 1024 1376 1088 1312 1088 inhier.DIR.P
[cell use]
use cadFan 2080 199 100 0 cadFan#84
xform 0 2176 448
p 2100 172 100 0 1 set1:cad getNames
use bd200tr 976 -1800 -100 0 frame
xform 0 3616 -96
use hwin 3600 -489 100 0 hwin#78
xform 0 3696 -448
p 3616 -448 100 0 -1 val(in):$(CAR_IDLE)
use hwin 3360 -585 100 0 hwin#77
xform 0 3456 -544
p 3363 -552 100 0 -1 val(in):$(CAD_START)
use hwin 3360 -521 100 0 hwin#74
xform 0 3456 -480
p 3363 -488 100 0 -1 val(in):$(CAD_MARK)
use hwin 3360 -553 100 0 hwin#73
xform 0 3456 -512
p 3376 -512 100 0 -1 val(in):$(CAD_START)
use hwin 3360 -489 100 0 hwin#39
xform 0 3456 -448
p 3376 -448 100 0 -1 val(in):$(CAD_MARK)
use hwin 3600 -457 100 0 hwin#11
xform 0 3696 -416
p 3603 -424 100 0 -1 val(in):$(CAR_BUSY)
use hwout 4800 -569 100 0 hwout#70
xform 0 4896 -528
p 5008 -528 100 0 -1 val(outp):$(top)fw1getNames.INPB
use hwout 4800 -649 100 0 hwout#69
xform 0 4896 -608
p 5008 -624 100 0 -1 val(outp):$(top)fw1getNames.A
use hwout 2272 567 100 0 hwout#21
xform 0 2368 608
p 2480 592 100 0 -1 val(outp):$(top)fw2getNamesStart.DIR PP NMS
use hwout 2272 599 100 0 hwout#19
xform 0 2368 640
p 2480 640 100 0 -1 val(outp):$(top)fw1getNamesStart.DIR PP NMS
use eseq 3792 -825 100 0 getNamesSeq
xform 0 4080 -528
p 4160 -416 100 0 1 DLY2:1.0
p 4112 -450 100 0 1 DLY3:0
p 4112 -482 100 0 1 DLY4:0
p 4112 -514 100 0 1 DLY5:0
p 4112 -546 100 0 1 DLY6:0
p 3408 -932 100 0 0 def(LNK2):$(top)fw1getNames.SLNK
p 4368 -416 75 768 -1 pproc(LNK1):PP
p 4368 -448 75 768 -1 pproc(LNK2):PP
p 4368 -480 75 768 -1 pproc(LNK3):PP
p 4368 -512 75 768 -1 pproc(LNK4):PP
p 4368 -544 75 768 -1 pproc(LNK5):PP
use outhier 4368 999 100 0 FLNK
xform 0 4384 1040
use outhier 4432 1184 100 0 OMSS
xform 0 4384 1200
use outhier 4432 1248 100 0 OVAL
xform 0 4384 1264
use outhier 1920 887 100 0 MESS
xform 0 1936 928
use outhier 1920 983 100 0 VAL
xform 0 1936 1024
use bihier 4448 1216 100 0 OCID
xform 0 4400 1232
use ecars 3904 983 100 0 getNamesC
xform 0 4064 1152
use inhier 1200 951 100 0 CLID
xform 0 1216 992
use inhier 1296 1047 100 0 DIR
xform 0 1312 1088
use ecad2 1408 455 100 0 getNames
xform 0 1568 768
p 1504 480 100 0 0 SNAM:
p 1744 544 75 1024 -1 pproc(STLK):NPP
[comments]
