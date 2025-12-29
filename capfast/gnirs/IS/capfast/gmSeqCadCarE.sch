[schematic2]
uniq 158
[tools]
[detail]
w 1098 539 100 0 n#157 efanouts.PresetFan.LNK4 784 640 960 640 960 528 1296 528 elongouts.ccCadMark.SLNK
w 1114 779 100 0 n#156 efanouts.PresetFan.LNK3 784 672 992 672 992 768 1296 768 elongouts.dcarSetupMark.SLNK
w 1250 1251 100 0 n#155 elongouts.dcdrRoiSetupMark.DOL 1280 1248 1280 1248 hwin.hwin#153.in
w 1266 1027 100 0 n#152 hwin.hwin#151.in 1296 1024 1296 1024 elongouts.dcobsSetupMark.DOL
w 666 -477 100 0 n#130 efanouts.StartFan.FLNK 624 -80 1152 -80 1152 -480 240 -480 240 -672 384 -672 efanouts.StartFan2.SLNK
w 890 -141 100 0 n#105 efanouts.StartFan.LNK2 624 -144 1216 -144 1216 -720 1424 -720 eseqs.CarSeqVal.SLNK
w 844 43 100 0 n#104 efanouts.StartFan.LNK1 624 -112 848 -112 848 208 944 208 elongouts.CarBusy.SLNK
w 1066 995 100 0 n#103 efanouts.PresetFan.LNK2 784 704 944 704 944 992 1248 992 free
w 876 971 100 0 n#102 efanouts.PresetFan.LNK1 784 736 880 736 880 1216 1216 1216 free
w 226 -141 100 0 n#101 ecad8.ecad8#122.STLK 160 -144 352 -144 352 -192 384 -192 efanouts.StartFan.SLNK
w 364 267 100 0 n#100 ecad8.ecad8#122.PLNK 160 -112 368 -112 368 656 544 656 efanouts.PresetFan.SLNK
w 1428 -405 100 2 n#95 hwin.hwin#94.in 1424 -400 1424 -400 eseqs.CarSeqVal.DOL1
w 1300 555 100 2 n#92 hwin.hwin#91.in 1296 560 1296 560 elongouts.ccCadMark.DOL
w 1300 795 100 2 n#90 hwin.hwin#89.in 1296 800 1296 800 elongouts.dcarSetupMark.DOL
w 2448 -581 100 0 CFLK ecars.ecars#76.FLNK 2320 -624 2416 -624 2416 -592 2528 -592 outhier.CFLK.p
w 2352 -485 100 0 OERR ecars.ecars#76.OERR 2320 -496 2432 -496 2432 -512 2528 -512 outhier.OERR.p
w 2352 -453 100 0 OMSS ecars.ecars#76.OMSS 2320 -464 2432 -464 2432 -432 2528 -432 outhier.OMSS.p
w 2168 -293 100 0 c#80 junction 1984 -400 1984 -304 2400 -304 2400 -352 2528 -352 outhier.CAR.p
w 1848 -389 100 0 c#80 eseqs.CarSeqVal.LNK1 1744 -400 2000 -400 ecars.ecars#76.IVAL
w 1848 299 100 0 c#80 eseqs.CarSeqBusy.LNK1 1824 288 1920 288 1920 -400 junction
w 1272 251 100 0 n#62 elongouts.CarBusy.FLNK 1200 240 1392 240 1392 -32 1504 -32 eseqs.CarSeqBusy.SLNK
w 1288 219 100 0 n#61 elongouts.CarBusy.VAL 1200 208 1424 208 1424 288 1504 288 eseqs.CarSeqBusy.DOL1
w 948 235 100 2 n#52 elongouts.CarBusy.DOL 944 240 944 240 hwin.hwin#24.in
w 224 690 100 0 n#30 ecad8.ecad8#122.MESS 160 688 336 688 336 816 464 816 outhier.MESS.p
w 306 832 100 0 n#29 ecad8.ecad8#122.VAL 160 720 304 720 304 896 464 896 outhier.VAL.p
w -360 658 100 0 n#28 inhier.ICID.P -400 656 -272 656 -272 688 -160 688 ecad8.ecad8#122.ICID
w -288 722 100 0 n#27 inhier.DIR.P -400 736 -368 736 -368 720 -160 720 ecad8.ecad8#122.DIR
s 784 272 100 0 BUSY
s -304 1120 180 0 Sequence Command Type E
s -320 1024 150 0 Mark command in Component and
s -320 976 150 0 Detector Controllers
s 2272 1264 150 0 gmSeqCadCarE
s 1904 688 140 0 Write MARK directive to
s 1840 640 140 0 the subsystem sequence command CADs
[cell use]
use elongouts 1280 1127 100 0 dcdrRoiSetupMark
xform 0 1408 1216
p 1120 1358 100 0 0 EGU:CAD directive
p 1344 1072 100 0 1 OMSL:closed_loop
p 1616 1184 100 0 -1 def(OUT):$(dc)drRoiSet.DIR
p 1344 1088 100 1024 1 name:$(nirs)$(seqcommand)$(I)
p 1536 1184 75 768 -1 pproc(OUT):PP
use elongouts 1296 903 100 0 dcobsSetupMark
xform 0 1424 992
p 1136 1134 100 0 0 EGU:CAD directive
p 1360 848 100 0 1 OMSL:closed_loop
p 1632 960 100 0 -1 def(OUT):$(dc)obsSetup.DIR
p 1360 864 100 1024 1 name:$(nirs)$(seqcommand)$(I)
p 1552 960 75 768 -1 pproc(OUT):PP
use elongouts 944 119 100 0 CarBusy
xform 0 1072 208
p 784 350 100 0 0 EGU:CAR value
p 1008 48 100 0 1 OMSL:closed_loop
p 1120 80 100 1024 1 name:$(nirs)$(seqcommand)$(I)
use elongouts 1296 679 100 0 dcarSetupMark
xform 0 1424 768
p 1136 910 100 0 0 EGU:CAD directive
p 1360 624 100 0 1 OMSL:closed_loop
p 1632 736 100 0 -1 def(OUT):$(dc)arSetup.DIR
p 1360 640 100 1024 1 name:$(nirs)$(seqcommand)$(I)
p 1552 736 75 768 -1 pproc(OUT):PP
use elongouts 1296 439 100 0 ccCadMark
xform 0 1424 528
p 1136 670 100 0 0 EGU:CAD directive
p 1360 384 100 0 1 OMSL:closed_loop
p 1632 496 100 0 -1 def(OUT):$(cc)$(seqcommand).DIR
p 1392 400 100 1024 1 name:$(nirs)$(seqcommand)$(I)
p 1552 496 75 768 -1 pproc(OUT):PP
use hwin 1088 1207 100 0 hwin#153
xform 0 1184 1248
p 1091 1240 100 0 -1 val(in):$(CAD_MARK)
use hwin 1104 983 100 0 hwin#151
xform 0 1200 1024
p 1107 1016 100 0 -1 val(in):$(CAD_MARK)
use hwin -848 311 100 0 hwin#116
xform 0 -752 352
p -976 352 100 0 -1 val(in):$(nirs)fw2SeqLut.VAL
use hwin -848 375 100 0 hwin#115
xform 0 -752 416
p -992 416 100 0 -1 val(in):$(nirs)fw1SeqLut.VAL
use hwin -848 439 100 0 hwin#114
xform 0 -752 480
p -928 480 100 0 -1 val(in):$(nirs)dirLut.VAL
use hwin 776 200 100 0 hwin#24
xform 0 848 240
p 766 168 100 0 0 typ(in):val
p 755 232 100 0 -1 val(in):$(CAR_BUSY)
use hwin 1104 759 100 0 hwin#89
xform 0 1200 800
p 1107 792 100 0 -1 val(in):$(CAD_MARK)
use hwin 1104 519 100 0 hwin#91
xform 0 1200 560
p 1107 552 100 0 -1 val(in):$(CAD_MARK)
use hwin 1232 -441 100 0 hwin#94
xform 0 1328 -400
p 1235 -408 100 0 -1 val(in):$(CAR_IDLE)
use hwin -848 247 100 0 hwin#120
xform 0 -752 288
p -1024 288 100 0 -1 val(in):$(nirs)slitSeqLut.VAL
use hwin -848 183 100 0 hwin#127
xform 0 -752 224
p -1024 224 100 0 -1 val(in):$(nirs)gratingSeqLut.VAL
use hwout 928 -313 100 0 hwout#108
xform 0 1024 -272
p 1024 -281 100 0 -1 val(outp):$(nirs)covergetNames.VAL
use hwout 928 -217 100 0 hwout#131
xform 0 1024 -176
p 1024 -185 100 0 -1 val(outp):$(nirs)fw1getNames.VAL
use hwout 928 -249 100 0 hwout#133
xform 0 1024 -208
p 1024 -217 100 0 -1 val(outp):$(nirs)fw2getNames.VAL
use hwout 928 -281 100 0 hwout#135
xform 0 1024 -240
p 1024 -249 100 0 -1 val(outp):$(nirs)focusgetNames.VAL
use hwout 928 -633 100 0 hwout#137
xform 0 1024 -592
p 1024 -601 100 0 -1 val(outp):$(nirs)cameragetNames.VAL
use hwout 928 -665 100 0 hwout#139
xform 0 1024 -624
p 1024 -633 100 0 -1 val(outp):$(nirs)acqgetNames.VAL
use hwout 928 -697 100 0 hwout#141
xform 0 1024 -656
p 1024 -665 100 0 -1 val(outp):$(nirs)deckergetNames.VAL
use hwout 928 -729 100 0 hwout#143
xform 0 1024 -688
p 1024 -697 100 0 -1 val(outp):$(nirs)xdispgetNames.VAL
use hwout 928 -761 100 0 hwout#145
xform 0 1024 -720
p 1024 -729 100 0 -1 val(outp):$(nirs)slitgetNames.VAL
use hwout 928 -793 100 0 hwout#148
xform 0 1024 -752
p 1024 -761 100 0 -1 val(outp):$(nirs)gratinggetNames.VAL
use efanouts 384 -329 100 0 StartFan
xform 0 504 -176
p 416 -64 100 0 1 SELM:All
p 260 -968 100 0 1 TPRO:TRUE
p 512 -368 100 1024 1 name:$(nirs)$(seqcommand)$(I)
use efanouts 576 544 100 0 PresetFan
xform 0 664 672
p 576 800 100 0 1 SELM:All
p 688 496 100 1024 -1 name:$(nirs)$(seqcommand)$(I)
use efanouts 384 -809 100 0 StartFan2
xform 0 504 -656
p 384 -560 100 0 1 SELM:All
p 496 -816 100 1024 0 name:$(nirs)$(seqcommand)$(I)
use ecad8 -160 -233 100 0 ecad8#122
xform 0 0 272
p -64 -16 100 0 1 SNAM:$(snam)
p -140 -1060 100 0 1 TPRO:TRUE
p -48 -240 100 1024 1 name:$(nirs)$(seqcommand)
use outhier 2496 -633 100 0 CFLK
xform 0 2512 -592
use outhier 2496 -553 100 0 OERR
xform 0 2512 -512
use outhier 2496 -473 100 0 OMSS
xform 0 2512 -432
use outhier 2496 -393 100 0 CAR
xform 0 2512 -352
use outhier 456 856 100 0 VAL
xform 0 448 896
use outhier 456 776 100 0 MESS
xform 0 448 816
use ecars 2000 -681 100 0 ecars#76
xform 0 2160 -512
p 2176 -688 100 1024 1 name:$(nirs)is$(seqcommand)C
use eseqs 1448 -808 100 0 CarSeqVal
xform 0 1584 -560
p 1536 -512 100 0 1 DLY1:0.1
p 1776 -368 100 0 0 def(LNK1):0.0
p 1568 -848 100 1024 1 name:$(nirs)$(seqcommand)$(I)
p 1760 -400 75 1024 -1 pproc(LNK1):PP
use eseqs 1504 -128 100 0 CarSeqBusy
xform 0 1664 128
p 1584 174 100 0 1 DLY1:0.0
p 1584 174 100 0 0 DLY2:0.0
p 1872 320 100 0 0 def(LNK1):0.0
p 1664 -160 100 1024 1 name:$(nirs)$(seqcommand)$(I)
p 1504 320 75 1280 -1 palrm(SELL):NMS
p 1472 288 75 1280 -1 pproc(DOL1):NPP
p 1472 256 75 1280 -1 pproc(DOL2):NPP
p 1824 298 75 0 -1 pproc(LNK1):PP
p 1824 266 75 0 -1 pproc(LNK2):NPP
use oslBorderC -608 -1177 100 0 oslBorderC#48
xform 0 1072 128
use inhier -392 696 100 0 DIR
xform 0 -400 736
use inhier -392 616 100 0 ICID
xform 0 -400 656
[comments]
