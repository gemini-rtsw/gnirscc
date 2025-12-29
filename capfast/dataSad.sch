[schematic2]
uniq 134
[tools]
[detail]
[cell use]
use esirs 1712 295 100 0 rdspeed
xform 0 1920 448
p 1648 192 100 0 0 DESC:readout rate
p 1888 400 100 0 -1 FTVL:STRING
use esirs 1808 -121 100 0 darktime
xform 0 2016 32
p 1984 -16 100 0 -1 FTVL:DOUBLE
use esirs -848 167 100 0 elapsed
xform 0 -640 320
p -752 144 100 0 1 DESC:Total elapsed time
p -752 80 100 0 1 EGU:seconds
p -752 112 100 0 1 FTVL:DOUBLE
use esirs -224 167 100 0 exposed
xform 0 -16 320
p -128 144 100 0 1 DESC:Actual total integration time
p -128 80 100 0 1 EGU:seconds
p -128 112 100 0 1 FTVL:DOUBLE
use esirs 352 199 100 0 exposedRQ
xform 0 560 352
p 448 176 100 0 1 DESC:Requested total integration time
p 448 112 100 0 1 EGU:seconds
p 448 144 100 0 1 FTVL:LONG
p 448 80 100 0 1 SCAN:1 second
use esirs 976 -697 100 0 utend
xform 0 1184 -544
p 1072 -720 100 0 1 DESC:UT at exposure end
use esirs -192 -1113 100 0 utstart
xform 0 16 -960
p -96 -1136 100 0 1 DESC:UT at exposure start
use esirs -864 -1113 100 0 utnow
xform 0 -656 -960
p -768 -1136 100 0 1 DESC:Universal Time now
p -768 -1168 100 0 1 SCAN:1 second
use esirs 352 615 100 0 dataLabel
xform 0 560 768
p 448 592 100 0 1 DESC:Latest data label
p 288 416 100 0 0 EGU:Data label
use esirs 352 -233 100 0 numRois
xform 0 560 -80
p 448 -256 100 0 1 DESC:Number of ROIs defined
p 448 -288 100 0 1 FTVL:LONG
use esirs 496 -1193 100 0 roiCnt
xform 0 704 -1040
p 432 -1488 100 0 0 FTVL:LONG
use esirs 1040 -1193 100 0 dhsHealth
xform 0 1248 -1040
use roiSad -288 -1337 100 0 roiSad#100
xform 0 -176 -1232
p -288 -1344 100 0 0 set1:grp GRPNUM
use eborderC -1040 -1497 100 0 eborderC#64
xform 0 640 -192
p 1536 -1344 100 768 -1 author:Peter Ruckle
p 1520 -1376 100 768 -1 date:1--18-01
p 1760 -1296 200 768 -1 file:dataSad.sch
p 2032 -1344 100 0 -1 page:1
p 2144 -1344 100 0 -1 pages:1
p 1808 -1344 100 0 -1 revision:0
p 1760 -1232 150 768 -1 system:Gnirs Components Controller
use notes 1600 -1177 100 0 notes#38
xform 0 1856 -992
p 1628 -866 100 0 -1 COMMENT1:This schematic contains the Status Alarm
p 1628 -898 100 0 -1 COMMENT2:database records for nirs data
p 1628 -928 100 0 -1 COMMENT3:handling.
p 1628 -1024 100 0 -1 COMMENT6:The Status Alarm Database must be loaded
p 1628 -1056 100 0 -1 COMMENT7:separately from the rest of the nirs
p 1628 -1088 100 0 -1 COMMENT8:database.
[comments]
