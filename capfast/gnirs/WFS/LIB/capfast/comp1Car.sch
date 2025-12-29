[schematic2]
uniq 60
[tools]
[detail]
w 1008 139 -100 0 FLNK outhier.FLNK.p 1072 128 992 128 ecars.C.FLNK
w 1000 331 -100 0 c#54 bihier.OCID.p 1056 320 992 320 ecars.C.CLID
w 552 331 100 0 n#53 ecars.C.ICID 672 320 480 320 elongins.ID.VAL
w 670 491 100 0 n#52 bihier.OVAL.p 768 480 608 480 608 352 672 352 ecars.C.IVAL
s 160 688 100 0 turn is output to the "applyC" CAR record.
s 160 720 100 0 which determines the overall instrument CAR status. This in
s 160 752 100 0 These "longout" records forward CAR events to the "genSub" record
s 1104 976 500 512 $(desc) Command Action Response Records
s 624 -1120 500 512 comp1Car.sch
n -1728 -1152 -1168 -800 100
This schematic contains the Command
Action Response (CAR) records for a
1 axis NIRI component (of any kind).
.
There is only one record:
.
C (expands to "name":"mech"C) - Report
status of "mech" component actions.
.
Note that it is not possible to set
the client ID of a CAR record directly,
due to the properties of the "ICID"
field, so this is fed in through a
"longin" record.
_
[cell use]
use compSnlArg -736 199 100 0 compSnlArg#58
xform 0 -512 320
use snl -256 215 100 0 snl#57
xform 0 -96 320
use outhier 1088 112 100 768 FLNK
xform 0 1056 128
use bihier 1088 320 100 1536 OCID
xform 0 1056 320
use bihier 800 480 100 1536 OVAL
xform 0 768 480
use elongins 288 384 100 768 ID
xform 0 352 336
p 288 256 100 768 1 EGU:client ID
p 288 224 100 768 1 PV:$(top)$(mech)
use bc200tr -1856 -1304 -100 0 frame
xform 0 -176 0
p 720 -1136 100 0 1 author:S.M.Beard
p 944 -1152 100 0 -1 border:C
p 720 -1168 100 0 1 checked:S.M.Beard
p 976 -1152 100 0 -1 date:19 Dec 96
p 944 -1024 100 0 -1 project:Core Instrument Control System
p 944 -1088 100 0 -1 title:Component CAR records
use ecars 736 384 100 768 C
xform 0 832 240
p 736 64 100 768 1 DESC:$(desc) CAR record
p 736 32 100 768 1 PV:$(top)$(mech)
[comments]
