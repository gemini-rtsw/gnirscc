[schematic2]
uniq 168
[tools]
[detail]
[cell use]
use esirs -160 391 100 0 gratingWvlength
xform 0 48 544
p -64 512 100 0 1 EGU:microns
p -64 480 100 0 1 FTVL:DOUBLE
p 32 320 100 0 0 PREC:4
p -32 432 100 0 1 SCAN:1 second
p -48 384 100 1024 0 name:$(sadtop)$(I)
use esirs -768 391 100 0 conID
xform 0 -560 544
p -704 352 100 0 1 DESC:Controller id
p -560 432 100 1024 1 name:$(sadtop)$(I)
use esirs -160 807 100 0 shutPos
xform 0 48 960
p -96 768 100 0 1 DESC:Shutter position
p 64 848 100 1024 1 name:$(sadtop)$(I)
use esirs 448 807 100 0 gratingOrder
xform 0 656 960
p 544 928 100 0 1 EGU:units
p 544 896 100 0 1 FTVL:LONG
p 576 848 100 0 1 SCAN:1 second
p 560 800 100 1024 0 name:$(sadtop)$(I)
use esirs -768 807 100 0 gratingTilt
xform 0 -560 960
p -672 928 100 0 1 EGU:degrees
p -672 896 100 0 1 FTVL:DOUBLE
p -576 736 100 0 0 PREC:4
p -640 848 100 0 1 SCAN:1 second
p -656 800 100 1024 0 name:$(sadtop)$(I)
use esirs 448 391 100 0 slitMask
xform 0 656 544
p 544 512 100 0 1 EGU:units
p 544 480 100 0 1 FTVL:STRING
p 576 432 100 0 1 SCAN:Passive
p 560 384 100 1024 0 name:$(sadtop)$(I)
use esirs 1024 391 100 0 decker
xform 0 1232 544
p 1120 512 100 0 1 EGU:units
p 1120 480 100 0 1 FTVL:STRING
p 1152 432 100 0 1 SCAN:Passive
p 1136 384 100 1024 0 name:$(sadtop)$(I)
use diagnosticTempSad 1784 103 100 0 diagnosticTempSad#155
xform 0 1936 176
use secondaryTempsSad 1736 -225 100 0 secondaryTempsSad#154
xform 0 1936 16
use primaryTempSad 1736 -385 100 0 primaryTempSad#153
xform 0 1936 -144
use cryoSad 1664 -569 100 0 cryoSad#150
xform 0 1792 -416
use motorSad 0 -1209 100 0 motorSad#149
xform 0 224 -960
p 96 -992 300 0 -1 set1:name acq
use motorSad -960 -1209 100 0 motorSad#148
xform 0 -736 -960
p -864 -992 300 0 -1 set1:name cover
use motorSad 480 -729 100 0 motorSad#147
xform 0 704 -480
p 576 -512 300 0 -1 set1:name focus
use motorSad 0 -729 100 0 motorSad#146
xform 0 224 -480
p 96 -512 300 0 -1 set1:name fw2
use motorSad -480 -729 100 0 motorSad#145
xform 0 -256 -480
p -384 -512 300 0 -1 set1:name fw1
use motorSad -960 -729 100 0 motorSad#144
xform 0 -736 -480
p -864 -512 300 0 -1 set1:name decker
use motorSad 960 -729 100 0 motorSad#143
xform 0 1184 -480
p 1056 -512 300 0 -1 set1:name slit
use motorSad 960 -1209 100 0 motorSad#142
xform 0 1184 -960
p 1056 -992 300 0 -1 set1:name xdisp
use motorSad 480 -1209 100 0 motorSad#141
xform 0 704 -960
p 576 -992 300 0 -1 set1:name grating
use motorSad -480 -1209 100 0 motorSad#140
xform 0 -256 -960
p -384 -992 300 0 -1 set1:name camera
use eborderC -1120 -1353 100 0 eborderC#94
xform 0 560 -48
p 1456 -1200 100 768 -1 author:Peter Ruckle
p 1440 -1232 100 768 -1 date:1-18-01
p 1680 -1152 200 768 -1 file:hdwrSad.sch
p 1952 -1200 100 0 -1 page:1
p 2064 -1200 100 0 -1 pages:1
p 1728 -1200 100 0 -1 revision:0
p 1680 -1088 150 768 -1 system:Gnirs Components Controller
use notes 1568 -1001 100 0 notes#38
xform 0 1824 -816
p 1596 -690 100 0 -1 COMMENT1:This schematic contains the Status Alarm
p 1596 -722 100 0 -1 COMMENT2:database records for the components
p 1596 -752 100 0 -1 COMMENT3:controller hardware.
p 1596 -848 100 0 -1 COMMENT6:The Status Alarm Database must be loaded
p 1596 -880 100 0 -1 COMMENT7:separately from the rest of the gnirsCcTop
p 1596 -912 100 0 -1 COMMENT8:database.
[comments]
