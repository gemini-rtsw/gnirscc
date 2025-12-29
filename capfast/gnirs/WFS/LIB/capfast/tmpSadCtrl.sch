[schematic2]
uniq 591
[tools]
[detail]
w 290 1035 -100 0 TMP bihier.TMP.p 352 1024 288 1024 esirs.Tmp.VAL
w 1384 1027 -100 0 HLNK esirs.Health.FLNK 1376 1024 1440 1024 outhier.HLNK.p
w 1376 995 -100 0 HVAL bihier.OHLT.p 1424 992 1376 992 esirs.Health.VAL
w 1376 963 -100 0 HMSS bihier.HMSS.p 1424 960 1376 960 esirs.Health.OMSS
w 882 1027 100 0 n#555 esirs.Health.INP 960 1024 864 1024 hwin.hwin#566.in
w -254 1059 100 0 n#545 esirs.Tmp.INP -128 1056 -320 1056 hwin.hwin#538.in
w -254 483 100 0 n#544 hwin.hwin#539.in -320 480 -128 480 esirs.Setp.INP
s 1040 -1024 500 512 tmpSadCtrl.sch
[cell use]
use bihier 384 1024 100 1536 TMP
xform 0 352 1024
use bihier 1456 992 100 1536 OHLT
xform 0 1424 992
use bihier 1456 960 100 1536 HMSS
xform 0 1424 960
use outhier 1456 1024 100 1536 HLNK
xform 0 1424 1024
use esirs -64 512 100 768 Setp
xform 0 80 384
p -64 0 100 768 1 DESC:$(desc) set point temperature
p -64 32 100 768 1 EGU:K
p -64 224 100 768 1 FDSC:$(desc) set point temperature
p -64 192 100 768 1 FTVL:DOUBLE
p -64 64 100 768 1 PREC:1
p -64 96 100 768 1 PV:$(sadtop)$(mech)
p -64 128 100 768 1 SCAN:1 second
p -64 160 100 768 1 SNAM:
p -128 480 75 1280 -1 palrm(INP):MS
use esirs -64 1088 100 768 Tmp
xform 0 80 960
p -64 576 100 768 1 DESC:Temperature
p -64 608 100 768 1 EGU:K
p -64 800 100 768 1 FDSC:$(desc) temperature
p -64 768 100 768 1 FTVL:DOUBLE
p -64 640 100 768 1 PREC:1
p -64 672 100 768 1 PV:$(sadtop)$(mech)
p -64 704 100 768 1 SCAN:1 second
p -64 736 100 768 1 SNAM:
use esirs 1024 1056 100 768 Health
xform 0 1168 928
p 1024 608 100 768 1 DESC:Health: Temperature controller
p 1024 640 100 768 1 EGU:
p 1024 768 100 768 1 FDSC:Health: $(desc) temp ctrl
p 1024 736 100 768 1 FTVL:STRING
p 1024 672 100 768 1 PV:$(sadtop)$(mech)
p 1024 704 100 768 1 SNAM:SIRengCtlrHealth
p 912 1024 75 1024 -1 pproc(INP):PP
use hwin -512 439 100 0 hwin#539
xform 0 -416 480
p -509 472 100 0 -1 val(in):$(eng)$(mech).SETP
use hwin -512 1015 100 0 hwin#538
xform 0 -416 1056
p -509 1048 100 0 -1 val(in):$(eng)$(mech)Tmp.VAL
use hwin 672 983 100 0 hwin#566
xform 0 768 1024
p 675 1016 100 0 -1 val(in):$(eng)$(mech).VAL
use bc200tr -1504 -1176 -100 0 frame
xform 0 176 128
p 1072 -1008 100 0 1 author:H.T. Yamada
p 1296 -1024 100 0 -1 border:C
p 1072 -1040 100 0 1 checked:H.T. Yamada
p 1328 -1024 100 0 -1 date:26 Nov 1997
p 1312 -896 100 0 -1 project:NIRI/GNIRS CC/WFS Interlocks
p 1312 -960 100 0 -1 title:Interlock Status/Alarm Database
[comments]
