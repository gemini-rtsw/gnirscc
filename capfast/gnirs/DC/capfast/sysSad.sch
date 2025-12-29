[schematic2]
uniq 82
[tools]
[detail]
w 1588 -397 100 0 n#74 ecalcs.calcInitDone.FLNK 1552 -304 1584 -304 1584 -480 1696 -480 esirs.initDone.SLNK
w 1240 -197 100 0 n#73 hwin.hwin#72.in 1264 -208 1264 -208 ecalcs.calcInitDone.INPC
w 1240 -165 100 0 n#71 hwin.hwin#70.in 1264 -176 1264 -176 ecalcs.calcInitDone.INPB
w 1240 -133 100 0 n#69 hwin.hwin#68.in 1264 -144 1264 -144 ecalcs.calcInitDone.INPA
w 1624 -309 100 0 n#67 ecalcs.calcInitDone.VAL 1552 -336 1600 -336 1600 -320 1696 -320 esirs.initDone.INP
w 584 -21 100 0 n#63 hwin.hwin#62.in 608 -32 608 -32 esirs.drRoiSetDone.INP
w 584 459 100 0 n#61 hwin.hwin#60.in 608 448 608 448 esirs.obsSetupDone.INP
w 584 939 100 0 n#59 hwin.hwin#58.in 608 928 608 928 esirs.arSetupDone.INP
w -760 -469 100 0 n#50 hwin.hwin#49.in -736 -480 -736 -480 esirs.historyLog.INP
[cell use]
use esirs 1216 679 100 0 gnaacHeartbeat
xform 0 1424 832
p 1312 624 100 0 1 DESC:GNAAC heartbeat
p 1408 544 100 0 0 SCAN:1 second 
p 1328 672 100 1024 0 name:$(sadtop)$(I)
use esirs 1216 199 100 0 coaddsCount
xform 0 1424 352
p 1312 144 100 0 1 DESC:Coadds countdown
p 1408 64 100 0 0 SCAN:1 second 
p 1328 192 100 1024 0 name:$(sadtop)$(I)
use esirs -736 679 100 0 name
xform 0 -528 832
p -640 656 100 0 1 DESC:Detector Controller Name
p -785 256 100 0 0 EGU: 
p -640 656 100 0 0 FDSC:Detector Controller Name - GNAAC
p -640 624 100 0 1 FTVL:STRING
p -640 560 100 0 1 PV:$(sadtop)
p -640 592 100 0 1 SNAM:
p -544 416 100 0 0 def(INP):$(top)
p -624 672 100 1024 0 name:$(sadtop)$(I)
use esirs -736 199 100 0 state
xform 0 -528 352
p -640 176 100 0 1 DESC:Detector Controller state
p -785 -224 100 0 0 EGU: 
p -785 80 100 0 0 FDSC:Detector Controller state [BOOTING|INITIALIZING|RUNNING|CONFIGURING]
p -640 144 100 0 1 FTVL:STRING
p -640 80 100 0 1 PV:$(sadtop)
p -544 64 100 0 0 SCAN:1 second
p -640 112 100 0 1 SNAM:
p -640 48 100 0 1 VAL:BOOTING
p -624 192 100 1024 0 name:$(sadtop)$(I)
use esirs -736 -281 100 0 health
xform 0 -528 -128
p -640 -304 100 0 1 DESC:Detector Controller health
p -785 -704 100 0 0 EGU: 
p -785 -400 100 0 0 FDSC:Detector Controller health [GOOD|WARNING|BAD]
p -640 -336 100 0 1 FTVL:STRING
p -640 -400 100 0 1 PV:$(sadtop)
p -544 -416 100 0 0 SCAN:1 second
p -640 -368 100 0 1 SNAM:
p -624 -288 100 1024 0 name:$(sadtop)$(I)
use esirs -64 -281 100 0 rdout
xform 0 144 -128
p 32 -304 100 0 1 DESC:Detector readout flag (0/1)
p -128 -480 100 0 0 EGU:0/1
p -113 -400 100 0 0 FDSC:Readout flag (1 when idle;0 when reading out detector)
p 32 -336 100 0 1 FTVL:LONG
p 32 -400 100 0 1 PV:$(sadtop)
p 32 -368 100 0 1 SNAM:
p 32 -432 100 0 1 VAL:1
p 48 -288 100 1024 0 name:$(sadtop)$(I)
use esirs -64 199 100 0 acq
xform 0 144 352
p 32 176 100 0 1 DESC:Acquisition flag (0/1)
p -128 0 100 0 0 EGU:0/1
p -113 80 100 0 0 FDSC:Acquisition flag (1 when idle;0 when acquiring data)
p 32 144 100 0 1 FTVL:LONG
p 32 80 100 0 1 PV:$(sadtop)
p 32 112 100 0 1 SNAM:
p 32 48 100 0 1 VAL:1
p 48 192 100 1024 0 name:$(sadtop)$(I)
use esirs -64 679 100 0 prep
xform 0 144 832
p 32 656 100 0 1 DESC:Preparation flag (0/1)
p -128 480 100 0 0 EGU:0/1
p -113 560 100 0 0 FDSC:Preparation flag (1 when idle;0 when preparing detector)
p 32 624 100 0 1 FTVL:LONG
p 32 560 100 0 1 PV:$(sadtop)
p 32 592 100 0 1 SNAM:
p 32 528 100 0 1 VAL:1
p 48 672 100 1024 0 name:$(sadtop)$(I)
use esirs -64 -729 100 0 heartBeat
xform 0 144 -576
p 32 -752 100 0 1 DESC:Detector Controller Heart Beat
p -128 -928 100 0 0 EGU:counter
p -128 -992 100 0 0 FDSC:Detector Controller Heart Beat
p 32 -784 100 0 1 FTVL:LONG
p 32 -848 100 0 1 PV:$(sadtop)
p 32 -816 100 0 1 SNAM:
p 48 -736 100 1024 0 name:$(sadtop)$(I)
use esirs -736 -729 100 0 historyLog
xform 0 -528 -576
p -640 -752 100 0 1 DESC:Detector Controller History Log
p -785 -1152 100 0 0 EGU: 
p -785 -848 100 0 0 FDSC:History Log
p -640 -784 100 0 1 FTVL:STRING
p -640 -848 100 0 1 PV:$(sadtop)
p -544 -864 100 0 1 SCAN:Passive
p -640 -816 100 0 1 SNAM:
p -624 -736 100 1024 0 name:$(sadtop)$(I)
use esirs 608 679 100 0 arSetupDone
xform 0 816 832
p 704 624 100 0 1 DESC:array setup done
p 800 544 100 0 0 SCAN:1 second 
p 720 672 100 1024 0 name:$(sadtop)$(I)
use esirs 608 199 100 0 obsSetupDone
xform 0 816 352
p 704 160 100 0 1 DESC:observation setup done
p 800 64 100 0 0 SCAN:1 second
p 720 192 100 1024 0 name:$(sadtop)$(I)
use esirs 608 -281 100 0 drRoiSetDone
xform 0 816 -128
p 720 -320 100 0 1 DESC:data reduction Roi setup done
p 800 -416 100 0 0 SCAN:1 second 
p 720 -288 100 1024 0 name:$(sadtop)$(I)
use esirs 608 -729 100 0 dataSim
xform 0 816 -576
p 704 -800 100 0 1 DESC:data simulation mode
p 800 -864 100 0 0 SCAN:Passive
p 720 -736 100 1024 0 name:$(sadtop)$(I)
use esirs 1696 -569 100 0 initDone
xform 0 1904 -416
p 1632 -864 100 0 0 FTVL:LONG
p 1888 -704 100 0 0 SCAN:Passive
p 1808 -576 100 1024 0 name:$(sadtop)$(I)
p 1648 -320 75 1024 -1 pproc(INP):PP
use esirs 1712 199 100 0 timeLeft
xform 0 1920 352
p 1808 144 100 0 1 DESC:Exposure time countdown
p 1904 64 100 0 0 SCAN:1 second 
p 1904 -64 100 0 0 def(INP):$(top)timeLeft
p 1824 192 100 1024 0 name:$(sadtop)$(I)
use hwin 416 -457 100 0 hwin#64
xform 0 512 -416
use hwin 416 -73 100 0 hwin#62
xform 0 512 -32
p 419 -40 100 0 -1 val(in):$(top)drRoiSetDone.VAL
use hwin 416 407 100 0 hwin#60
xform 0 512 448
p 419 440 100 0 -1 val(in):$(top)obsSetupDone.VAL
use hwin 416 887 100 0 hwin#58
xform 0 512 928
p 419 920 100 0 -1 val(in):$(top)arSetupDone.VAL
use hwin -224 999 100 0 hwin#56
xform 0 -128 1040
use hwin -192 471 100 0 hwin#54
xform 0 -96 512
use hwin -240 23 100 0 hwin#52
xform 0 -144 64
use hwin -256 -457 100 0 hwin#51
xform 0 -160 -416
use hwin -928 -521 100 0 hwin#49
xform 0 -832 -480
use hwin -944 -73 100 0 hwin#48
xform 0 -848 -32
p -941 -40 100 0 -1 val(in):$(top)dcHealth.VAL
use hwin -928 487 100 0 hwin#46
xform 0 -832 528
p -925 520 100 0 -1 val(in):$(top)state.VAL
use hwin 1072 -185 100 0 hwin#68
xform 0 1168 -144
p 1075 -152 100 0 -1 val(in):$(top)drRoiSetDone
use hwin 1072 -217 100 0 hwin#70
xform 0 1168 -176
p 1075 -184 100 0 -1 val(in):$(top)obsSetupDone
use hwin 1072 -249 100 0 hwin#72
xform 0 1168 -208
p 1075 -216 100 0 -1 val(in):$(top)arSetupDone
use ecalcs 1264 -617 100 0 calcInitDone
xform 0 1408 -352
p 1171 -136 100 0 0 CALC:(A=0)&&(B=0)&&(C=0)
p 976 -242 100 0 0 SCAN:1 second
p 1376 -624 100 1024 0 name:$(sadtop)$(I)
p 1232 -144 75 1280 -1 pproc(INPA):PP
p 1232 -176 75 1280 -1 pproc(INPB):PP
p 1232 -208 75 1280 -1 pproc(INPC):PP
use CBorder -1120 -1384 -100 0 frame
xform 0 560 -80
p 1952 -1252 75 1536 -1 Author:Steven M. Beard
p 1448 -1248 100 1536 1 Date:13 Jun 97
p 1552 -1152 300 1792 -1 Dnumber:
p 1680 -1168 150 1536 -1 Title:sysSad.sch
use notes 1568 -1001 100 0 notes#38
xform 0 1824 -816
p 1596 -690 100 0 -1 COMMENT1:This schematic contains all the
p 1596 -722 100 0 -1 COMMENT2:compulsory Status Alarm Database
p 1596 -752 100 0 -1 COMMENT3:records describing the overall state
p 1596 -784 100 0 -1 COMMENT4:of the detector controller.
p 1596 -848 100 0 -1 COMMENT6:The Status Alarm Database must be loaded
p 1596 -880 100 0 -1 COMMENT7:separately from the rest of the GNAAC
p 1596 -912 100 0 -1 COMMENT8:database.
[comments]
