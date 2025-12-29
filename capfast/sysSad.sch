[schematic2]
uniq 159
[tools]
[detail]
w -830 571 100 0 n#151 hwin.hwin#135.in -800 560 -800 560 embbis.healthmbbi.INP
w -222 -325 100 0 n#150 hwin.hwin#144.in -192 -336 -192 -336 embbis.statembbi.INP
[cell use]
use healthSad 1456 -121 100 0 healthSad#158
xform 0 1728 176
use hwin -384 -377 100 0 hwin#144
xform 0 -288 -336
p -381 -344 100 0 -1 val(in):$(top)state
use hwin -992 519 100 0 hwin#135
xform 0 -896 560
p -989 552 100 0 -1 val(in):$(top)health
use esirs 896 -89 100 0 parked
xform 0 1104 64
p 832 -384 100 0 0 FTVL:LONG
p 1088 -224 100 0 0 SCAN:1 second
p 1008 -96 100 1024 0 name:$(sadtop)$(I)
use esirs 896 263 100 0 datumed
xform 0 1104 416
p 832 -32 100 0 0 FTVL:LONG
p 1088 128 100 0 0 SCAN:1 second
p 1056 368 100 1024 0 name:$(sadtop)$(I)
use esirs 1456 647 100 0 shutHealth
xform 0 1664 800
p 1488 608 100 0 1 FTVL:STRING
p 1432 -40 100 0 0 NORD: 
p 1648 704 100 1024 1 name:$(sadtop)$(I)
use esirs 880 695 100 0 name
xform 0 1088 848
p 976 672 100 0 1 DESC:Detector Controller Name
p 831 272 100 0 0 EGU: 
p 976 608 100 0 1 FDSC:
p 976 640 100 0 1 FTVL:STRING
p 856 8 100 0 0 NORD: 
p 1072 560 100 0 0 SCAN:Passive
p 976 608 100 0 0 SNAM:
p 1072 432 100 0 0 def(INP):
p 1088 752 100 1024 1 name:$(sadtop)$(I)
use esirs 224 -633 100 0 state
xform 0 432 -480
p 320 -656 100 0 1 DESC:Detector Controller state
p 175 -1056 100 0 0 EGU: 
p 175 -752 100 0 0 FDSC:Detector Controller state [BOOTING|INITIALIZING|RUNNING|CONFIGURING]
p 320 -688 100 0 1 FTVL:STRING
p 320 -752 100 0 1 PINI:NO
p 320 -752 100 0 0 SCAN:Passive
p 320 -720 100 0 0 SNAM:
p 320 -720 100 0 1 VAL:BOOTING
p 448 -576 100 1024 1 name:$(sadtop)$(I)
use esirs -384 263 100 0 health
xform 0 -176 416
p -288 240 100 0 1 DESC:Detector Controller health
p -433 -160 100 0 0 EGU: 
p -433 144 100 0 0 FDSC:Detector Controller health [GOOD|WARNING|BAD]
p -288 208 100 0 1 FTVL:STRING
p -288 176 100 0 0 SCAN:1 second
p -288 176 100 0 0 SNAM:
p -176 304 100 1024 1 name:$(sadtop)$(I)
p -384 512 75 1280 -1 palrm(INP):MS
use esirs -224 711 100 0 heartBeat
xform 0 -16 864
p -128 688 100 0 1 DESC:Detector Controller Heart Beat
p -288 512 100 0 0 EGU:counter
p -288 448 100 0 0 FDSC:Detector Controller Heart Beat
p -128 656 100 0 1 FTVL:LONG
p -32 576 100 0 0 SCAN:Passive
p -128 624 100 0 0 SNAM:
p -16 768 100 1024 1 name:$(sadtop)$(I)
use esirs -416 -249 100 0 initialized
xform 0 -208 -96
p -480 -544 100 0 0 FTVL:LONG
p -304 -256 100 1024 0 name:$(sadtop)$(I)
use esirs 976 -457 100 0 simMode
xform 0 1184 -304
p 912 -752 100 0 0 FTVL:LONG
p 1168 -592 100 0 0 SCAN:1 second
p 1088 -464 100 1024 0 name:$(sadtop)$(I)
use esirs 1024 -825 100 0 hSimMode
xform 0 1232 -672
p 960 -1120 100 0 0 FTVL:LONG
p 1216 -960 100 0 0 SCAN:1 second
p 1136 -832 100 1024 0 name:$(sadtop)$(I)
use embbis -192 -441 100 0 statembbi
xform 0 -64 -368
p -64 -450 100 0 0 FRST:DISCONNECTED
p -64 -482 100 0 0 FVST:ERROR
p -64 -354 100 0 0 ONST:INITIALIZING
p -256 -354 100 0 0 ONVL:1
p -480 -322 100 0 0 SCAN:1 second
p -64 -418 100 0 0 THST:CONFIGURING
p -64 -386 100 0 0 TWST:RUNNING
p -64 -322 100 0 0 ZRST:BOOTING
p -80 -368 100 1024 0 name:$(sadtop)$(I)
use embbis -800 455 100 0 healthmbbi
xform 0 -672 528
p -672 542 100 0 0 ONST:WARNING
p -864 542 100 0 0 ONVL:1
p -1088 574 100 0 0 SCAN:1 second
p -672 510 100 0 0 TWST:BAD
p -672 574 100 0 0 ZRST:GOOD
p -688 448 100 1024 0 name:$(sadtop)$(I)
use pressureSad -768 -1209 100 0 pressureSad#127
xform 0 -544 -960
use tempSad -144 -1161 100 0 tempSad#126
xform 0 80 -912
use eborderC -1104 -1385 100 0 eborderC#65
xform 0 576 -80
p 1472 -1232 100 768 -1 author:Peter Ruckle
p 1456 -1264 100 768 -1 date:1-18-01
p 1696 -1184 200 768 -1 file:sysSad.sch
p 1968 -1232 100 0 -1 page:1
p 2080 -1232 100 0 -1 pages:1
p 1744 -1232 100 0 -1 revision:0
p 1696 -1120 150 768 -1 system:Gnirs Components Controller
use notes 1648 -1097 100 0 notes#38
xform 0 1904 -912
p 1676 -786 100 0 -1 COMMENT1:This schematic contains all the Status
p 1676 -818 100 0 -1 COMMENT2:Alarm Database records describing the
p 1676 -848 100 0 -1 COMMENT3:the overall state of the detector
p 1676 -880 100 0 -1 COMMENT4:controller.
p 1676 -944 100 0 -1 COMMENT6:The Status Alarm Database must be loaded
p 1676 -976 100 0 -1 COMMENT7:separately from the rest of the nirsCcTop
p 1676 -1008 100 0 -1 COMMENT8:database.
[comments]
