[schematic2]
uniq 70
[tools]
[detail]
w -600 -277 100 0 n#69 hwin.hwin#51.in -576 -288 -576 -288 elongins.pLim.INP
w -56 -277 100 0 n#68 hwin.hwin#54.in -32 -288 -32 -288 elongins.nLim.INP
w -56 -581 100 0 n#67 hwin.hwin#63.in -32 -592 -32 -592 elongins.OT.INP
w 520 -277 100 0 n#66 hwin.hwin#57.in 544 -288 544 -288 elongins.Home.INP
w 1096 -277 100 0 n#65 hwin.hwin#60.in 1120 -288 1120 -288 elongins.Fault.INP
w -56 459 100 0 n#49 hwin.hwin#22.in -32 448 -32 448 estringins.Health.INP
w 552 475 100 0 n#34 hwin.hwin#16.in 576 464 576 464 estringins.State.INP
w 1128 443 100 0 n#33 hwin.hwin#26.in 1152 432 1152 432 estringins.Position.INP
w 1096 11 100 0 n#32 hwin.hwin#13.in 1120 0 1120 0 elongins.Datumed.INP
w 520 11 100 0 n#31 hwin.hwin#9.in 544 0 544 0 elongins.Parked.INP
w -24 11 100 0 n#29 hwin.hwin#7.in 0 0 0 0 elongins.ParkPos.INP
w -568 11 100 0 n#28 hwin.hwin#4.in -544 0 -544 0 elongins.Eng.INP
w -536 475 100 0 n#27 hwin.hwin#1.in -512 464 -512 464 estringins.Name.INP
[cell use]
use hwin -736 -41 100 0 hwin#4
xform 0 -640 0
p -733 -8 100 0 -1 val(in):@mechEng[$(N)]
use hwin -704 423 100 0 hwin#1
xform 0 -608 464
p -701 456 100 0 -1 val(in):@mechName[$(NS)]
use hwin -192 -41 100 0 hwin#7
xform 0 -96 0
p -189 -8 100 0 -1 val(in):@mechParkPos[$(N)]
use hwin 352 -41 100 0 hwin#9
xform 0 448 0
p 355 -8 100 0 -1 val(in):@mechParked[$(N)]
use hwin 928 -41 100 0 hwin#13
xform 0 1024 0
p 931 -8 100 0 -1 val(in):@mechDatumed[$(N)]
use hwin 384 423 100 0 hwin#16
xform 0 480 464
p 336 464 100 0 -1 val(in):@mechState[$(NS)]
use hwin -224 407 100 0 hwin#22
xform 0 -128 448
p -221 440 100 0 -1 val(in):@mechHealth[$(NS)]
use hwin 960 391 100 0 hwin#26
xform 0 1056 432
p 963 424 100 0 -1 val(in):@mechPos[$(NS)]
use hwin -768 -329 100 0 hwin#51
xform 0 -672 -288
p -765 -296 100 0 -1 val(in):@mechpLim[$(N)]
use hwin -224 -329 100 0 hwin#54
xform 0 -128 -288
p -221 -296 100 0 -1 val(in):@mechnLim[$(N)]
use hwin 352 -329 100 0 hwin#57
xform 0 448 -288
p 355 -296 100 0 -1 val(in):@mechHome[$(N)]
use hwin 928 -329 100 0 hwin#60
xform 0 1024 -288
p 931 -296 100 0 -1 val(in):@mechFault[$(N)]
use hwin -224 -633 100 0 hwin#63
xform 0 -128 -592
p -221 -600 100 0 -1 val(in):@mechOT[$(N)]
use elongins -544 -105 100 0 Eng
xform 0 -416 -32
p -608 -144 100 0 1 DTYP:vxWorks Variable (INST_IO)
p -512 -224 100 0 1 SCAN:1 second
p -432 -192 100 1024 1 name:$(top)$(name)$(I)
use elongins 0 -105 100 0 ParkPos
xform 0 128 -32
p -64 -144 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 32 -240 100 0 1 SCAN:1 second
p 112 -192 100 1024 1 name:$(top)$(name)$(I)
use elongins 544 -105 100 0 Parked
xform 0 672 -32
p 480 -144 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 592 -224 100 0 1 SCAN:1 second
p 656 -192 100 1024 1 name:$(top)$(name)$(I)
use elongins 1120 -105 100 0 Datumed
xform 0 1248 -32
p 1056 -144 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 1184 -224 100 0 1 SCAN:1 second
p 1216 -192 100 1024 1 name:$(top)$(name)$(I)
use elongins -576 -393 100 0 pLim
xform 0 -448 -320
p -640 -432 100 0 1 DTYP:vxWorks Variable (INST_IO)
p -544 -512 100 0 1 SCAN:1 second
p -464 -480 100 1024 1 name:$(top)$(name)$(I)
use elongins -32 -393 100 0 nLim
xform 0 96 -320
p -96 -432 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 0 -512 100 0 1 SCAN:1 second
p 80 -480 100 1024 1 name:$(top)$(name)$(I)
use elongins 544 -393 100 0 Home
xform 0 672 -320
p 480 -432 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 576 -512 100 0 1 SCAN:1 second
p 656 -480 100 1024 1 name:$(top)$(name)$(I)
use elongins 1120 -393 100 0 Fault
xform 0 1248 -320
p 1056 -432 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 1152 -512 100 0 1 SCAN:1 second
p 1232 -480 100 1024 1 name:$(top)$(name)$(I)
use elongins -32 -697 100 0 OT
xform 0 96 -624
p -96 -736 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 0 -816 100 0 1 SCAN:1 second
p 80 -784 100 1024 1 name:$(top)$(name)$(I)
use estringins -512 359 100 0 Name
xform 0 -384 432
p -560 320 100 0 1 DTYP:vxWorks Variable (INST_IO)
p -512 240 100 0 1 SCAN:1 second
p -432 272 100 1024 1 name:$(top)$(name)$(I)
use estringins 576 359 100 0 State
xform 0 704 432
p 528 320 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 608 224 100 0 1 SCAN:1 second
p 720 256 100 1024 1 name:$(top)$(name)$(I)
use estringins 1152 327 100 0 Position
xform 0 1280 400
p 1104 288 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 1200 192 100 0 1 SCAN:1 second
p 1296 224 100 1024 1 name:$(top)$(name)$(I)
use estringins -32 343 100 0 Health
xform 0 96 416
p 22 582 100 0 0 DESC:Component Controller Health
p 60 630 100 0 0 DTYP:vxWorks Variable (INST_IO)
p -32 478 100 0 0 SCAN:1 second
p 80 400 100 1024 0 name:$(top)$(name)$(I)
use bb200tr -896 -968 -100 0 frame
xform 0 384 -144
p 880 -800 100 0 -1 author:Peter Ruckle
p 912 -832 100 0 -1 date:2/9/2001
p 1136 -752 160 0 -1 filename:motorSadIntrfc.sch
p 1152 -688 100 0 -1 system:GNIRS CC Sad
[comments]
