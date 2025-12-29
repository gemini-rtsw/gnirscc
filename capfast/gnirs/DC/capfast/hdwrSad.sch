[schematic2]
uniq 101
[tools]
[detail]
w -568 475 100 0 n#97 hwout.hwout#96.outp -544 464 -544 464 esirs.detID.VAL
w -56 -469 100 0 n#91 hwin.hwin#90.in -64 -480 0 -480 esirs.numCoAdds.INP
w -56 11 100 0 n#88 hwin.hwin#89.in -64 0 0 0 esirs.detTemp.INP
w 1512 -21 100 0 n#86 hwin.hwin#87.in 1504 -32 1568 -32 esirs.numDAvgs.INP
w -24 -901 100 0 n#85 hwin.hwin#84.in -32 -912 32 -912 esirs.procMode.INP
w 1512 459 100 0 n#82 hwin.hwin#83.in 1504 448 1568 448 esirs.bias.INP
w -792 -901 100 0 n#81 hwin.hwin#80.in -800 -912 -736 -912 esirs.ucodeName.INP
w 664 11 100 0 n#77 hwin.hwin#76.in 656 0 720 0 esirs.mntTemp.INP
w 648 -469 100 0 n#74 hwin.hwin#75.in 640 -480 704 -480 esirs.numLNR.INP
w -792 -469 100 0 n#73 hwin.hwin#72.in -800 -480 -736 -480 esirs.tempErr.INP
w -792 11 100 0 n#70 hwin.hwin#71.in -800 0 -736 0 esirs.detState.INP
w -792 1067 100 0 n#66 hwin.hwin#67.in -800 1056 -736 1056 esirs.detType.INP
w 664 491 100 0 n#50 hwin.hwin#49.in 656 480 720 480 esirs.detCurMaxRow.INP
w -56 491 100 0 n#48 hwin.hwin#47.in -64 480 0 480 esirs.detCurMaxCol.INP
w 664 1003 100 0 n#44 hwin.hwin#43.in 656 992 720 992 esirs.detMaxRow.INP
w -72 939 100 0 n#42 hwin.hwin#41.in -80 928 -16 928 esirs.detMaxCol.INP
[cell use]
use esirs 880 -1145 100 0 ucDlCount
xform 0 1088 -992
p 976 -1184 100 0 1 DESC:ucode download counter
p 816 -1440 100 0 0 FTVL:LONG
p 1072 -1280 100 0 0 SCAN:1 second
p 992 -1152 100 1024 0 name:$(sadtop)$(I)
use hwout -544 423 100 0 hwout#96
xform 0 -448 464
p -448 455 100 0 -1 val(outp):$(top)detID
use hwin 464 439 100 0 hwin#49
xform 0 560 480
p 467 472 100 0 -1 val(in):$(top)curMaxRow.VAL
use hwin -256 439 100 0 hwin#47
xform 0 -160 480
p -253 472 100 0 -1 val(in):$(top)curMaxCol.VAL
use hwin 464 951 100 0 hwin#43
xform 0 560 992
p 467 984 100 0 -1 val(in):$(top)maxRow.VAL
use hwin -272 887 100 0 hwin#41
xform 0 -176 928
p -269 920 100 0 -1 val(in):$(top)maxCol.VAL
use hwin -992 1015 100 0 hwin#67
xform 0 -896 1056
p -989 1048 100 0 -1 val(in):$(top)detType.VAL
use hwin -992 -41 100 0 hwin#71
xform 0 -896 0
p -989 -8 100 0 -1 val(in):&(top)activate.VAL
use hwin -992 -521 100 0 hwin#72
xform 0 -896 -480
p -989 -488 100 0 -1 val(in):$(top)rdFootHGain.VAL
use hwin 448 -521 100 0 hwin#75
xform 0 544 -480
p 451 -488 100 0 -1 val(in):$(top)obsSetup.VALD
use hwin 464 -41 100 0 hwin#76
xform 0 560 0
p 467 -8 100 0 -1 val(in):$(top)mntLGain.VAL
use hwin 1344 935 100 0 hwin#79
xform 0 1440 976
use hwin -992 -953 100 0 hwin#80
xform 0 -896 -912
p -989 -920 100 0 -1 val(in):$(top)arSetupB.VAL
use hwin 1312 407 100 0 hwin#83
xform 0 1408 448
p 1315 440 100 0 -1 val(in):$(top)achvdBias.VAL
use hwin -224 -953 100 0 hwin#84
xform 0 -128 -912
p -221 -920 100 0 -1 val(in):$(top)pModeStr.VAL
use hwin 1312 -73 100 0 hwin#87
xform 0 1408 -32
p 1315 -40 100 0 -1 val(in):$(top)obsSetup.VALC
use hwin -256 -41 100 0 hwin#89
xform 0 -160 0
p -253 -8 100 0 -1 val(in):$(top)footLGain.VAL
use hwin -256 -521 100 0 hwin#90
xform 0 -160 -480
p -253 -488 100 0 -1 val(in):$(top)obsSetup.VALE
use esirs 32 -1161 100 0 procMode
xform 0 240 -1008
p 128 -1200 100 0 1 DESC:Process Mode
p 224 -1296 100 0 0 SCAN:1 second
p 144 -1168 100 1024 0 name:$(sadtop)$(I)
use esirs -736 -1161 100 0 ucodeName
xform 0 -528 -1008
p -640 -1200 100 0 1 DESC:ucode name
p -544 -1296 100 0 0 SCAN:1 second
p -624 -1168 100 1024 0 name:$(sadtop)$(I)
use esirs 1568 663 100 0 ucodeDL
xform 0 1776 816
p 1664 624 100 0 1 DESC:ucode downloaded
p 1760 528 100 0 0 SCAN:1 second
p 1680 656 100 1024 0 name:$(sadtop)$(I)
use esirs 1568 199 100 0 bias
xform 0 1776 352
p 1664 160 100 0 1 DESC:bias achieved
p 1760 64 100 0 0 SCAN:1 second
p 1680 192 100 1024 0 name:$(sadtop)$(I)
use esirs 1568 -281 100 0 numDAvgs
xform 0 1776 -128
p 1632 -320 100 0 1 DESC:Number of digital averages
p 1760 -416 100 0 0 SCAN:1 second
p 1680 -288 100 1024 0 name:$(sadtop)$(I)
use esirs 704 -729 100 0 numLNR
xform 0 912 -576
p 768 -768 100 0 1 DESC:Number of Low Noise Reads
p 896 -864 100 0 0 SCAN:1 second 
p 816 -736 100 1024 0 name:$(sadtop)$(I)
use esirs 0 -729 100 0 numCoAdds
xform 0 208 -576
p 96 -800 100 0 1 DESC:Number of Coadds
p 192 -864 100 0 0 SCAN:1 second
p 112 -736 100 1024 0 name:$(sadtop)$(I)
use esirs 720 231 100 0 detCurMaxRow
xform 0 928 384
p 816 176 100 0 1 DESC:Current max rows
p 816 112 100 0 1 EGU:pixels
p 656 -32 100 0 0 FDSC:Current max rows
p 816 144 100 0 1 FTVL:LONG
p 816 48 100 0 1 PV:$(sadtop)
p 816 208 100 0 1 SCAN:1 second
p 816 80 100 0 1 SNAM:
p 832 224 100 1024 0 name:$(sadtop)$(I)
use esirs 0 231 100 0 detCurMaxCol
xform 0 208 384
p 96 176 100 0 1 DESC:Current max columns
p 96 112 100 0 1 EGU:pixels
p 352 176 100 0 0 FDSC:Current max columns
p 96 144 100 0 1 FTVL:LONG
p 96 48 100 0 1 PV:$(sadtop)
p 96 208 100 0 1 SCAN:1 second
p 96 80 100 0 1 SNAM:
p 112 224 100 1024 0 name:$(sadtop)$(I)
use esirs -960 247 100 0 detID
xform 0 -752 400
p -864 224 100 0 1 DESC:Detector chip identifier
p -1024 48 100 0 0 EGU:
p -1024 -16 100 0 0 FDSC:Detector chip identifier
p -864 192 100 0 1 FTVL:STRING
p -864 128 100 0 1 PV:$(sadtop)
p -768 112 100 0 0 SCAN:1 second
p -864 160 100 0 1 SNAM:
p -848 240 100 1024 0 name:$(sadtop)$(I)
use esirs -736 807 100 0 detType
xform 0 -528 960
p -640 784 100 0 1 DESC:Detector array type
p -800 608 100 0 0 EGU:
p -800 544 100 0 0 FDSC:Detector array type
p -640 752 100 0 1 FTVL:STRING
p -640 688 100 0 1 PV:$(sadtop)
p -544 672 100 0 0 SCAN:1 second
p -640 720 100 0 1 SNAM:
p -624 800 100 1024 0 name:$(sadtop)$(I)
use esirs 720 743 100 0 detMaxRow
xform 0 928 896
p 816 688 100 0 1 DESC:Detector max rows
p 816 624 100 0 1 EGU:pixels
p 656 480 100 0 0 FDSC:Detector max rows
p 816 656 100 0 1 FTVL:LONG
p 816 560 100 0 1 PV:$(sadtop)
p 816 720 100 0 1 SCAN:1 second
p 816 592 100 0 1 SNAM:
p 832 736 100 1024 0 name:$(sadtop)$(I)
use esirs -16 679 100 0 detMaxCol
xform 0 192 832
p 80 624 100 0 1 DESC:Detector max columns
p 80 560 100 0 1 EGU:pixels
p 336 624 100 0 0 FDSC:Detector max columns
p 80 592 100 0 1 FTVL:LONG
p 112 720 100 0 1 PV:$(sadtop)
p 80 656 100 0 1 SCAN:1 second
p 80 528 100 0 0 SNAM:
p 96 672 100 1024 0 name:$(sadtop)$(I)
use esirs -736 -249 100 0 detState
xform 0 -528 -96
p -640 -272 100 0 1 DESC:Detector state
p -800 -448 100 0 0 EGU:
p -800 -512 100 0 0 FDSC:Detector State [IDLE|...]
p -640 -304 100 0 1 FTVL:STRING
p -640 -368 100 0 1 PV:$(sadtop)
p -544 -384 100 0 0 SCAN:1 second
p -640 -336 100 0 1 SNAM:
p -624 -256 100 1024 0 name:$(sadtop)$(I)
use esirs 0 -249 100 0 detTemp
xform 0 208 -96
p 96 -320 100 0 1 DESC:detector temperature
p -64 -448 100 0 0 EGU:degrees K
p 192 -384 100 0 0 SCAN:1 second
p 112 -256 100 1024 0 name:$(sadtop)$(I)
use esirs 720 -249 100 0 mntTemp
xform 0 928 -96
p 816 -320 100 0 1 DESC:mount temperature
p 912 -384 100 0 0 SCAN:1 second
p 832 -256 100 1024 0 name:$(sadtop)$(I)
use esirs -736 -729 100 0 tempErr
xform 0 -528 -576
p -640 -800 100 0 1 DESC:temperature error
p -544 -864 100 0 0 SCAN:1 second
p -624 -736 100 1024 0 name:$(sadtop)$(I)
use CBorder -1120 -1384 -100 0 frame
xform 0 560 -80
p 1952 -1252 75 1536 -1 Author:Steven M. Beard
p 1448 -1248 100 1536 1 Date:13 Jun 97
p 1552 -1152 300 1792 -1 Dnumber:
p 1680 -1168 150 1536 -1 Title:hdwrSad.sch
use notes 1568 -1001 100 0 notes#38
xform 0 1824 -816
p 1596 -690 100 0 -1 COMMENT1:This schematic contains the Status Alarm
p 1596 -722 100 0 -1 COMMENT2:database records for the GNAAC
p 1596 -752 100 0 -1 COMMENT3:detector hardware.
p 1596 -848 100 0 -1 COMMENT6:The Status Alarm Database must be loaded
p 1596 -880 100 0 -1 COMMENT7:separately from the rest of the GNAAC
p 1596 -912 100 0 -1 COMMENT8:database.
[comments]
