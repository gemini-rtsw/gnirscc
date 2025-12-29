[schematic2]
uniq 151
[tools]
[detail]
w -862 -333 -100 0 c#125 inhier.SLNK.P -896 -336 -768 -336 estringouts.pushOmss.SLNK
w -862 -301 -100 0 c#124 inhier.IMSS.P -896 -304 -768 -304 estringouts.pushOmss.DOL
w -414 -253 100 0 n#107 estringouts.pushOmss.OUT -512 -352 -448 -352 -448 -256 -320 -256 esirs.health.IMSS
w -638 -221 -100 0 c#123 inhier.INP.P -896 -224 -320 -224 esirs.health.INP
w 1170 395 100 0 n#101 hwin.hwin#100.in 1152 384 1248 384 esirs.simMode.INP
s 480 976 500 1024 System Status Alarm Database
s -464 1136 500 0 NIRI Wavefront Sensor Components
s 1200 -992 500 512 wfsSysSad.sch
n -1088 576 -608 928 100
This schematic contains the systemwide
Status/Alarm Database for the NIRI.
It contains these universal records:
.
name   - Name of instrument (defaulted
to "NIRI".
.
state  - State of instrument (BOOTING,
INITIALIZING or RUNNING).
.
health - Health of instrument (GOOD,
WARNING or BAD).
.
plus others as shown.
_
[cell use]
use esirs 1248 -352 100 768 label
xform 0 1456 -192
p 1184 -384 100 0 0 ADEL:0.0
p 1184 -416 100 0 0 BRSV:NO_ALARM
p 1312 -416 100 0 1 DESC:The FDSC field describes the WFS Components Controller
p 1184 -480 100 0 0 DISS:NO_ALARM
p 1184 -512 100 0 0 DISV:1
p 1312 -576 100 0 1 EGU:
p 1184 -576 100 0 0 EVNT:0
p 1312 -448 100 0 1 FDSC:$(name)
p 1312 -480 100 0 1 FTVL:STRING
p 1312 -384 100 0 1 PV:$(sadtop)
p 1312 -512 100 0 1 SNAM:
p 1312 -544 100 0 1 VAL:$(name)
use esirs -320 -480 100 768 health
xform 0 -112 -320
p -256 -496 100 0 1 DESC:WFS Health [GOOD|WARNING|BAD]
p -256 -624 100 0 1 EGU:
p -256 -656 100 0 1 FDSC:WFS Health [GOOD|WARNING|BAD]
p -256 -528 100 0 1 FTVL:STRING
p -256 -592 100 0 1 PV:$(sadtop)
p -256 -560 100 0 1 SNAM:
use esirs 1248 135 100 0 simMode
xform 0 1456 288
p 1344 112 100 0 1 DESC:Simulation mode [NONE|FULL|FAST|VSM]
p 1344 -48 100 0 1 EGU:
p 1199 16 100 0 0 FDSC:Simulation mode [NONE|FULL|FAST|VSM]
p 1344 80 100 0 1 FTVL:STRING
p 1344 16 100 0 1 PV:$(sadtop)
p 1344 -16 100 0 1 SCAN:1 second
p 1344 48 100 0 1 SNAM:
use esirs 512 -345 100 0 historyLog
xform 0 720 -192
p 608 -368 100 0 1 DESC:History log record (should be BIGSTRING)
p 608 -528 100 0 1 EGU:
p 608 -496 100 0 1 FDSC:History log record (should be BIGSTRING)
p 608 -400 100 0 1 FTVL:STRING
p 608 -464 100 0 1 PV:$(sadtop)
p 608 -432 100 0 1 SNAM:
use esirs 512 135 100 0 issPort
xform 0 720 288
p 608 112 100 0 1 DESC:ISS port on which instrument is installed
p 608 48 100 0 1 EGU:number
p 463 16 100 0 0 FDSC:ISS port on which instrument is installed
p 608 80 100 0 1 FTVL:LONG
p 608 -16 100 0 1 PV:$(sadtop)
p 608 16 100 0 1 SNAM:
use esirs 512 615 100 0 debugMode
xform 0 720 768
p 608 592 100 0 1 DESC:Debugging mode [NOLOG|NONE|MIN|FULL]
p 608 464 100 0 1 EGU:
p 463 496 100 0 0 FDSC:Debugging mode [NOLOG|NONE|MIN|FULL]
p 608 560 100 0 1 FTVL:STRING
p 608 496 100 0 1 PV:$(sadtop)
p 608 528 100 0 1 SNAM:
use esirs -256 615 100 0 name
xform 0 -48 768
p -160 592 100 0 1 DESC:WFS Components Controller Name
p -160 464 100 0 1 EGU:
p -160 592 100 0 0 FDSC:WFS Components Controller Name (should be $(name))
p -160 560 100 0 1 FTVL:STRING
p -160 496 100 0 1 PV:$(sadtop)
p -160 528 100 0 1 SNAM:
use esirs -256 135 100 0 state
xform 0 -48 288
p -160 112 100 0 1 DESC:Inst. state [BOOTING|INITIALIZING|RUNNING|CONFIGURING]
p -160 -16 100 0 1 EGU:
p -305 16 100 0 0 FDSC:Inst. state [BOOTING|INITIALIZING|RUNNING|CONFIGURING]
p -160 80 100 0 1 FTVL:STRING
p -160 16 100 0 1 PV:$(sadtop)
p -160 48 100 0 1 SNAM:
use esirs 1248 615 100 0 heartBeat
xform 0 1456 768
p 1344 592 100 0 1 DESC:Heart beat record:  Shows that WFS software is alive
p 1344 464 100 0 1 EGU:count
p 1199 496 100 0 0 FDSC:Heart beat record:  Shows that WFS software is alive
p 1344 560 100 0 1 FTVL:LONG
p 1344 496 100 0 1 PV:$(sadtop)
p 1344 528 100 0 1 SNAM:
use inhier -928 -336 100 2048 SLNK
xform 0 -896 -336
use inhier -928 -304 100 2048 IMSS
xform 0 -896 -304
use inhier -928 -224 100 2048 INP
xform 0 -896 -224
use estringouts -656 -288 100 1024 pushOmss
xform 0 -640 -336
p -704 -416 100 768 1 OMSL:closed_loop
p -704 -448 100 768 1 PV:$(sadtop)
p -512 -352 75 768 -1 pproc(OUT):PP
use bc200tr -1344 -1144 -100 0 frame
xform 0 336 160
p 1232 -976 100 0 1 author:S.M.Beard
p 1456 -992 100 0 -1 border:C
p 1232 -1008 100 0 1 checked:H.Yamada
p 1488 -992 100 768 -1 date:1999-11-06
p 1472 -864 100 0 -1 project:Near Infra-Red Imager
p 1232 -864 100 768 1 revised:H.Yamada
p 1456 -1024 100 768 -1 revision:1.0
p 1456 -912 100 0 -1 title:Components Controller Status and Alarm
p 1456 -944 100 768 -1 title2:Database
use hwin 960 343 100 0 hwin#100
xform 0 1056 384
p 963 376 100 0 -1 val(in):$(eng)shsMode
[comments]
