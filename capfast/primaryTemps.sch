[schematic2]
uniq 57
[tools]
[detail]
w -24 283 100 0 n#49 hwin.hwin#22.in 0 272 0 272 estringins.Health.INP
w 584 299 100 0 n#34 hwin.hwin#16.in 608 288 608 288 estringins.State.INP
w 1160 267 100 0 n#33 hwin.hwin#26.in 1184 256 1184 256 estringins.Position.INP
w 1128 -165 100 0 n#32 hwin.hwin#13.in 1152 -176 1152 -176 elongins.Datumed.INP
w 552 -165 100 0 n#31 hwin.hwin#9.in 576 -176 576 -176 elongins.Parked.INP
w 8 -165 100 0 n#29 hwin.hwin#7.in 32 -176 32 -176 elongins.ParkPos.INP
w -536 -165 100 0 n#28 hwin.hwin#4.in -512 -176 -512 -176 elongins.Eng.INP
w -504 299 100 0 n#27 hwin.hwin#1.in -480 288 -480 288 estringins.Name.INP
[cell use]
use estringins 0 167 100 0 Health
xform 0 128 240
p 54 406 100 0 0 DESC:Component Controller Health
p 92 454 100 0 0 DTYP:vxWorks Variable (INST_IO)
p 0 302 100 0 0 SCAN:Passive
p 112 224 100 1024 0 name:$(top)$(name)$(I)
use estringins 1184 151 100 0 Position
xform 0 1312 224
p 1136 112 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 1232 16 100 0 1 SCAN:Passive
p 1328 48 100 1024 1 name:$(top)$(name)$(I)
use estringins 608 183 100 0 State
xform 0 736 256
p 560 144 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 640 48 100 0 1 SCAN:Passive
p 752 80 100 1024 1 name:$(top)$(name)$(I)
use estringins -480 183 100 0 Name
xform 0 -352 256
p -528 144 100 0 1 DTYP:vxWorks Variable (INST_IO)
p -480 64 100 0 1 SCAN:Passive
p -400 96 100 1024 1 name:$(top)$(name)$(I)
use elongins 1152 -281 100 0 Datumed
xform 0 1280 -208
p 1088 -320 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 1216 -400 100 0 1 SCAN:Passive
p 1248 -368 100 1024 1 name:$(top)$(name)$(I)
use elongins 576 -281 100 0 Parked
xform 0 704 -208
p 512 -320 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 624 -400 100 0 1 SCAN:Passive
p 688 -368 100 1024 1 name:$(top)$(name)$(I)
use elongins 32 -281 100 0 ParkPos
xform 0 160 -208
p -32 -320 100 0 1 DTYP:vxWorks Variable (INST_IO)
p 64 -416 100 0 1 SCAN:Passive
p 144 -368 100 1024 1 name:$(top)$(name)$(I)
use elongins -512 -281 100 0 Eng
xform 0 -384 -208
p -576 -320 100 0 1 DTYP:vxWorks Variable (INST_IO)
p -480 -400 100 0 1 SCAN:Passive
p -400 -368 100 1024 1 name:$(top)$(name)$(I)
use hwin 992 215 100 0 hwin#26
xform 0 1088 256
p 995 248 100 0 -1 val(in):@mechPos[$(NS)]
use hwin -192 231 100 0 hwin#22
xform 0 -96 272
p -189 264 100 0 -1 val(in):@mechHealth[$(NS)]
use hwin 416 247 100 0 hwin#16
xform 0 512 288
p 368 288 100 0 -1 val(in):@mechState[$(NS)]
use hwin 960 -217 100 0 hwin#13
xform 0 1056 -176
p 963 -184 100 0 -1 val(in):@mechDatumed[$(N)]
use hwin 384 -217 100 0 hwin#9
xform 0 480 -176
p 387 -184 100 0 -1 val(in):@mechParked[$(N)]
use hwin -160 -217 100 0 hwin#7
xform 0 -64 -176
p -157 -184 100 0 -1 val(in):@mechParkPos[$(N)]
use hwin -672 247 100 0 hwin#1
xform 0 -576 288
p -669 280 100 0 -1 val(in):@mechName[$(NS)]
use hwin -704 -217 100 0 hwin#4
xform 0 -608 -176
p -701 -184 100 0 -1 val(in):@mechEng[$(N)]
use bb200tr -896 -968 -100 0 frame
xform 0 384 -144
p 880 -800 100 0 -1 author:Peter Ruckle
p 912 -832 100 0 -1 date:2/9/2001
p 1136 -752 100 0 -1 filename:motorSadIntrfc.sch
p 1152 -688 100 0 -1 system:GNIRS CC Sad
[comments]
