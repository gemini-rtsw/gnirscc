[schematic2]
uniq 103
[tools]
[detail]
w 306 123 100 0 n#102 hwin.hwin#101.in 336 112 336 112 ecad4.ecad4#100.INPC
w 1904 267 100 0 FLNK elongouts.dcCadMark.FLNK 1712 256 2144 256 outhier.FLNK.p
w 1088 227 100 0 n#95 ecad4.ecad4#100.PLNK 656 -112 768 -112 768 224 1456 224 elongouts.dcCadMark.SLNK
w 208 187 100 0 n#94 hwin.hwin#93.in 128 176 336 176 ecad4.ecad4#100.INPB
w 1368 315 100 0 n#90 hwin.hwin#88.in 1360 304 1424 304 1424 256 1456 256 elongouts.dcCadMark.DOL
w 720 434 100 0 n#30 ecad4.ecad4#100.MESS 656 432 832 432 832 560 960 560 outhier.MESS.p
w 802 576 100 0 n#29 ecad4.ecad4#100.VAL 656 464 800 464 800 640 960 640 outhier.VAL.p
w 136 402 100 0 n#28 inhier.ICID.P 96 400 224 400 224 432 336 432 ecad4.ecad4#100.ICID
w 208 466 100 0 n#27 inhier.DIR.P 96 480 128 480 128 464 336 464 ecad4.ecad4#100.DIR
s 720 928 150 0 MARK command to Detector Controller only
s 816 1024 180 0 Sequence Command Type C
s 2320 1264 150 0 gmSeqCadCarC
[cell use]
use hwin 144 71 100 0 hwin#101
xform 0 240 112
p 147 104 100 0 -1 val(in):$(nirs)DcReadoutMonChange.VAL
use hwin 1168 263 100 0 hwin#88
xform 0 1264 304
p 1171 296 100 0 -1 val(in):$(CAD_MARK)
use hwin -64 135 100 0 hwin#93
xform 0 32 176
p -61 168 100 0 -1 val(in):$(nirs)dcDisabled.VAL
use ecad4 336 -233 100 0 ecad4#100
xform 0 496 144
p 432 -144 100 0 1 SNAM:$(snam)
p 448 -240 100 1024 1 name:$(nirs)$(seqcommand)
use outhier 2112 215 100 0 FLNK
xform 0 2128 256
use outhier 952 520 100 0 MESS
xform 0 944 560
use outhier 952 600 100 0 VAL
xform 0 944 640
use elongouts 1456 135 100 0 dcCadMark
xform 0 1584 224
p 1296 366 100 0 0 EGU:CAD directive
p 1520 48 100 0 1 OMSL:closed_loop
p 1808 192 100 0 -1 def(OUT):$(dc)$(seqcommand).DIR
p 1568 80 100 1024 1 name:$(nirs)$(seqcommand)$(I)
p 1712 192 75 768 -1 pproc(OUT):PP
use oslBorderC -608 -1177 100 0 oslBorderC#48
xform 0 1072 128
p 2332 -928 120 256 -1 Title:NIRS IS - detector controller command (type C)
use inhier 104 360 100 0 ICID
xform 0 96 400
use inhier 104 440 100 0 DIR
xform 0 96 480
[comments]
