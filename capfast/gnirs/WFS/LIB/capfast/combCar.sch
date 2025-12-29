[schematic2]
uniq 152
[tools]
[detail]
w -1118 -181 -100 0 SLNKB inhier.SLNKE.P -1120 -192 -1056 -192 -1056 -160 junction
w -1118 -149 -100 0 SLNKB inhier.SLNKD.P -1120 -160 -1056 -160 -1056 -128 junction
w -1118 -117 -100 0 SLNKB inhier.SLNKC.P -1120 -128 -1056 -128 -1056 -96 junction
w -1118 -85 -100 0 SLNKB inhier.SLNKB.P -1120 -96 -1056 -96 -1056 -64 junction
w 394 395 -100 0 FLNK outhier.FLNK.p 464 384 384 384 ecars.C.FLNK
w 386 587 -100 0 c#142 bihier.OCID.p 448 576 384 576 ecars.C.CLID
w 112 715 -100 0 OVAL bihier.OVAL.p 240 704 32 704 32 608 junction
w -312 611 100 0 OVAL egenSub.CombCar.OUTA -544 608 -32 608 64 608 ecars.C.IVAL
w -1006 -53 -100 0 SLNKB inhier.SLNKA.P -1120 -64 -832 -64 egenSub.CombCar.SLNK
w -1006 43 -100 0 c#139 inhier.ICIDE.P -1120 32 -832 32 egenSub.CombCar.INPJ
w -1006 107 -100 0 c#140 inhier.IVALE.P -1120 96 -832 96 egenSub.CombCar.INPI
w -1006 171 -100 0 c#131 inhier.ICIDD.P -1120 160 -832 160 egenSub.CombCar.INPH
w -1006 235 -100 0 c#128 inhier.IVALD.P -1120 224 -832 224 egenSub.CombCar.INPG
w -1006 363 -100 0 c#127 inhier.IVALC.P -1120 352 -832 352 egenSub.CombCar.INPE
w -1006 299 -100 0 c#124 inhier.ICIDC.P -1120 288 -832 288 egenSub.CombCar.INPF
w -1006 427 -100 0 c#123 inhier.ICIDB.P -1120 416 -832 416 egenSub.CombCar.INPD
w -1006 491 -100 0 c#120 inhier.IVALB.P -1120 480 -832 480 egenSub.CombCar.INPC
w -1006 555 -100 0 c#119 inhier.ICIDA.P -1120 544 -832 544 egenSub.CombCar.INPB
w -1006 619 -100 0 IVALA inhier.IVALA.P -1120 608 -832 608 egenSub.CombCar.INPA
w -452 155 100 0 n#66 egenSub.CombCar.FLNK -544 -96 -448 -96 -448 416 64 416 ecars.C.SLNK
w -120 579 100 0 n#74 egenSub.CombCar.VALB -544 576 -256 576 64 576 ecars.C.ICID
s 560 80 100 0 turn is output to the "applyC" CAR record.
s 560 112 100 0 which determines the overall instrument CAR status. This in
s 560 144 100 0 These "longout" records forward CAR events to the "genSub" record
s -448 752 100 0 and has to be connected to VALB.
s -448 800 100 0 because CAR.IVAL is a value field. Conversely, CAR.ICID is a link field
s -448 848 100 0 NOTE: The CAR.IVAL field is connected to genSub.OUTA rather than VALA
s 624 -1120 500 512 combCar
s 1104 976 500 512 $(desc) Command Action Response Records
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
use inhier -1152 -192 100 2048 SLNKE
xform 0 -1120 -192
use inhier -1152 -160 100 2048 SLNKD
xform 0 -1120 -160
use inhier -1152 -128 100 2048 SLNKC
xform 0 -1120 -128
use inhier -1152 -96 100 2048 SLNKB
xform 0 -1120 -96
use outhier 480 384 100 1536 FLNK
xform 0 448 384
use bihier 480 576 100 1536 OCID
xform 0 448 576
use bihier 272 704 100 1536 OVAL
xform 0 240 704
use inhier -1152 96 100 2048 IVALE
xform 0 -1120 96
use inhier -1152 32 100 2048 ICIDE
xform 0 -1120 32
use inhier -1152 608 100 2048 IVALA
xform 0 -1120 608
use inhier -1152 544 100 2048 ICIDA
xform 0 -1120 544
use inhier -1152 480 100 2048 IVALB
xform 0 -1120 480
use inhier -1152 416 100 2048 ICIDB
xform 0 -1120 416
use inhier -1152 288 100 2048 ICIDC
xform 0 -1120 288
use inhier -1152 352 100 2048 IVALC
xform 0 -1120 352
use inhier -1152 224 100 2048 IVALD
xform 0 -1120 224
use inhier -1152 160 100 2048 ICIDD
xform 0 -1120 160
use inhier -1152 -64 100 2048 SLNKA
xform 0 -1120 -64
use egenSub -768 672 100 768 CombCar
xform 0 -688 272
p -768 -192 100 0 1 DESC:Combine CAR values
p -896 640 100 1280 1 FTA:LONG
p -896 576 100 1280 1 FTB:LONG
p -896 512 100 1280 1 FTC:LONG
p -896 448 100 1280 1 FTD:LONG
p -896 384 100 1280 1 FTE:LONG
p -896 320 100 1280 1 FTF:LONG
p -896 256 100 1280 1 FTG:LONG
p -896 192 100 1280 1 FTH:LONG
p -896 128 100 1280 1 FTI:LONG
p -896 64 100 1280 1 FTJ:LONG
p -480 640 100 768 1 FTVA:LONG
p -480 576 100 768 1 FTVB:LONG
p -480 512 100 768 1 FTVC:LONG
p -768 -256 100 0 1 PV:$(top)$(mech)
p -768 -224 100 0 1 SNAM:cicsCarValCombine
use ecars 128 640 100 768 C
xform 0 224 496
p 128 320 100 768 1 DESC:$(desc) top level CAR record
p 128 288 100 768 1 PV:$(top)$(mech)
use bc200tr -1856 -1304 -100 0 frame
xform 0 -176 0
p 720 -1136 100 0 1 author:S.M.Beard
p 944 -1152 100 0 -1 border:C
p 720 -1168 100 0 1 checked:S.M.Beard
p 976 -1152 100 0 -1 date:22 Jan 97
p 944 -1024 100 0 -1 project:Near Infra-Red Imager
p 944 -1088 100 0 -1 title:Lock Component Car Record
[comments]
