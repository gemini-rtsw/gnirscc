[schematic2]
uniq 181
[tools]
[detail]
w -102 -245 100 0 c#179 inhier.IMSSF.P -144 -256 0 -256 egenSub.Mess.INPF
w -1006 267 -100 0 c#177 inhier.ICIDF.P -1120 256 -832 256 egenSubD.CombCar.INPL
w -1118 -213 -100 0 c#176 inhier.SLNKF.P -1120 -224 -1056 -224 -1056 -192 junction
w -1054 -53 -100 0 c#176 inhier.SLNKA.P -1120 -64 -928 -64 -928 -96 -832 -96 egenSubD.CombCar.SLNK
w -1118 -85 -100 0 c#176 inhier.SLNKB.P -1120 -96 -1056 -96 -1056 -64 junction
w -1118 -117 -100 0 c#176 inhier.SLNKC.P -1120 -128 -1056 -128 -1056 -96 junction
w -1118 -149 -100 0 c#176 inhier.SLNKD.P -1120 -160 -1056 -160 -1056 -128 junction
w -1118 -181 -100 0 c#176 inhier.SLNKE.P -1120 -192 -1056 -192 -1056 -160 junction
w -1006 299 -100 0 IVALF inhier.IVALF.P -1120 288 -832 288 egenSubD.CombCar.INPK
w -190 -501 100 0 n#171 egenSubD.CombCar.VALC -544 480 -320 480 -320 -512 0 -512 egenSub.Mess.INPJ
w 484 -117 100 0 n#170 egenSub.Mess.FLNK 288 -640 480 -640 480 416 704 416 ecars.C.SLNK
w 412 299 100 0 n#169 egenSub.Mess.OUTA 288 64 416 64 416 544 704 544 ecars.C.IMSS
w -238 -597 100 0 n#168 egenSubD.CombCar.FLNK -544 -128 -416 -128 -416 -608 0 -608 egenSub.Mess.SLNK
w -102 -189 100 0 c#159 inhier.IMSSE.P -144 -192 0 -192 egenSub.Mess.INPE
w -102 -125 100 0 c#161 inhier.IMSSD.P -144 -128 0 -128 egenSub.Mess.INPD
w -102 -61 100 0 c#162 inhier.IMSSC.P -144 -64 0 -64 egenSub.Mess.INPC
w -102 3 100 0 c#163 inhier.IMSSB.P -144 0 0 0 egenSub.Mess.INPB
w -102 67 100 0 c#164 inhier.IMSSA.P -144 64 0 64 egenSub.Mess.INPA
w 1096 547 100 0 OMSS outhier.OMSS.p 1216 544 1024 544 ecars.C.OMSS
w 98 619 100 0 n#153 egenSubD.CombCar.OUTA -544 576 -448 576 -448 608 704 608 ecars.C.IVAL
w 1114 715 100 0 OVAL ecars.C.VAL 1024 608 1056 608 1056 704 1232 704 outhier.OVAL.p
w 1034 395 -100 0 FLNK outhier.FLNK.p 1104 384 1024 384 ecars.C.FLNK
w 1026 587 -100 0 c#142 bihier.OCID.p 1088 576 1024 576 ecars.C.CLID
w -1022 331 -100 0 c#139 inhier.ICIDE.P -1120 320 -832 320 egenSubD.CombCar.INPJ
w -1014 363 -100 0 c#140 inhier.IVALE.P -1120 352 -832 352 egenSubD.CombCar.INPI
w -1022 395 -100 0 c#131 inhier.ICIDD.P -1120 384 -832 384 egenSubD.CombCar.INPH
w -1022 427 -100 0 c#128 inhier.IVALD.P -1120 416 -832 416 egenSubD.CombCar.INPG
w -1030 491 -100 0 c#127 inhier.IVALC.P -1120 480 -832 480 egenSubD.CombCar.INPE
w -1022 459 -100 0 c#124 inhier.ICIDC.P -1120 448 -832 448 egenSubD.CombCar.INPF
w -1022 523 -100 0 c#123 inhier.ICIDB.P -1120 512 -832 512 egenSubD.CombCar.INPD
w -1022 555 -100 0 c#120 inhier.IVALB.P -1120 544 -832 544 egenSubD.CombCar.INPC
w -1030 587 -100 0 c#119 inhier.ICIDA.P -1120 576 -832 576 egenSubD.CombCar.INPB
w -1022 619 -100 0 IVALA inhier.IVALA.P -1120 608 -832 608 egenSubD.CombCar.INPA
w 168 587 100 0 n#74 egenSubD.CombCar.VALB -544 544 -320 544 -320 576 704 576 ecars.C.ICID
s 1104 976 500 512 $(desc) Command Action Response Records
s 624 -1120 500 512 combCar
s -448 848 100 0 NOTE: The CAR.IVAL field is connected to genSub.OUTA rather than VALA
s -448 800 100 0 because CAR.IVAL is a value field. Conversely, CAR.ICID is a link field
s -448 752 100 0 and has to be connected to VALB.
s 560 144 100 0 These "longout" records forward CAR events to the "genSub" record
s 560 112 100 0 which determines the overall instrument CAR status. This in
s 560 80 100 0 turn is output to the "applyC" CAR record.
n -1648 -1120 -1088 -768 100
This schematic contains the Command
Action Response (CAR) records for a
2 axis CICS component (of any kind).
.
There is only one record:
.
C (expands to "cics:"mech"C") - Report
status of "mech" selector mechanism.
.
Note that it is not possible to set
the client ID of a CAR record directly,
due to the properties of the "ICID"
field, so this is fed in through a
"longin" record.
_
[cell use]
use inhier -1216 240 100 0 ICIDF
xform 0 -1120 256
use inhier -1216 -240 100 0 SLNKF
xform 0 -1120 -224
use inhier -1152 -64 100 2048 SLNKA
xform 0 -1120 -64
use inhier -1152 384 100 2048 ICIDD
xform 0 -1120 384
use inhier -1152 416 100 2048 IVALD
xform 0 -1120 416
use inhier -1152 480 100 2048 IVALC
xform 0 -1120 480
use inhier -1152 448 100 2048 ICIDC
xform 0 -1120 448
use inhier -1152 512 100 2048 ICIDB
xform 0 -1120 512
use inhier -1152 544 100 2048 IVALB
xform 0 -1120 544
use inhier -1152 576 100 2048 ICIDA
xform 0 -1120 576
use inhier -1152 608 100 2048 IVALA
xform 0 -1120 608
use inhier -1152 320 100 2048 ICIDE
xform 0 -1120 320
use inhier -1152 352 100 2048 IVALE
xform 0 -1120 352
use inhier -1152 -96 100 2048 SLNKB
xform 0 -1120 -96
use inhier -1152 -128 100 2048 SLNKC
xform 0 -1120 -128
use inhier -1152 -160 100 2048 SLNKD
xform 0 -1120 -160
use inhier -1152 -192 100 2048 SLNKE
xform 0 -1120 -192
use inhier -176 -192 100 2048 IMSSE
xform 0 -144 -192
use inhier -176 -128 100 2048 IMSSD
xform 0 -144 -128
use inhier -176 -64 100 2048 IMSSC
xform 0 -144 -64
use inhier -176 0 100 2048 IMSSB
xform 0 -144 0
use inhier -176 64 100 2048 IMSSA
xform 0 -144 64
use inhier -1216 272 100 0 IVALF
xform 0 -1120 288
use inhier -224 -272 100 0 IMSSF
xform 0 -144 -256
use egenSubD -768 656 100 0 CombCar
xform 0 -688 240
p -848 -192 100 0 1 DESC:Combine CAR values
p -1008 624 100 0 1 FTA:LONG
p -1055 -411 100 0 0 FTB:LONG
p -1055 -443 100 0 0 FTC:LONG
p -1055 -475 100 0 0 FTD:LONG
p -1055 -507 100 0 0 FTE:LONG
p -1055 -571 100 0 0 FTF:LONG
p -1055 -571 100 0 0 FTG:LONG
p -1055 -603 100 0 0 FTH:LONG
p -1055 -635 100 0 0 FTI:LONG
p -1055 -667 100 0 0 FTJ:LONG
p -1055 -411 100 0 0 FTK:LONG
p -464 624 100 0 1 FTVA:LONG
p -464 560 100 0 1 FTVB:LONG
p -464 496 100 0 1 FTVC:LONG
p -816 -352 100 0 1 INAM:combCarInit
p -1120 254 100 0 0 LFLG:IGNORE
p -784 -304 100 0 1 PV:$(top)$(mech)
p -816 -240 100 0 1 SNAM:cicsCarValCombine
p -704 -288 100 1024 1 name:$(top)$(mech)
use egenSub 0 -697 100 0 Mess
xform 0 144 -272
p 80 48 100 0 1 FTA:STRING
p 80 -16 100 0 1 FTB:STRING
p 80 -64 100 0 1 FTC:STRING
p 96 -128 100 0 1 FTD:STRING
p 96 -208 100 0 1 FTE:STRING
p 80 -288 100 0 1 FTF:STRING
p 96 -512 100 0 1 FTJ:LONG
p -223 -923 100 0 0 FTVA:STRING
p -288 -322 100 0 0 SNAM:cicsCarMessSelect
p 112 -704 100 1024 0 name:$(top)$(mech)$(I)
use outhier 1184 503 100 0 OMSS
xform 0 1200 544
use outhier 1120 384 100 1536 FLNK
xform 0 1088 384
use outhier 1200 663 100 0 OVAL
xform 0 1216 704
use bihier 1120 576 100 1536 OCID
xform 0 1088 576
use ecars 768 640 100 768 C
xform 0 864 496
p 768 320 100 768 1 DESC:$(desc) top level CAR record
p 768 288 100 768 1 PV:$(top)$(mech)
p 864 240 100 1024 1 name:$(top)$(I)
use bc200tr -1856 -1304 -100 0 frame
xform 0 -176 0
p 720 -1136 100 0 1 author:S.M.Beard
p 944 -1152 100 0 -1 border:C
p 720 -1168 100 0 1 checked:S.M.Beard
p 976 -1152 100 0 -1 date:22 Jan 97
p 944 -1024 100 0 -1 project:Near Infra-Red Spectrograph
p 944 -1088 100 0 -1 title:Lock Component Car Record
[comments]
