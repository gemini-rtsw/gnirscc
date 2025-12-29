[schematic2]
uniq 753
[tools]
[detail]
w -358 -629 -100 0 c#751 bihier.TVAL.p -304 -640 -352 -640 esirs.InToler.VAL
w -350 -605 -100 0 c#752 outhier.TFLNK.p -288 -608 -352 -608 esirs.InToler.FLNK
w 1218 115 100 0 n#712 ecalcs.AgDiffCalc.FLNK 1184 112 1312 112 esirs.AgDiff.SLNK
w 516 91 100 0 n#711 esirs.AgEngDiff.FLNK 480 304 512 304 512 -112 896 -112 ecalcs.AgDiffCalc.SLNK
w 786 211 100 0 n#704 hwin.hwin#731.in 736 208 896 208 ecalcs.AgDiffCalc.INPC
w 658 275 100 0 n#702 esirs.AgEngDiff.VAL 480 272 896 272 ecalcs.AgDiffCalc.INPA
w 786 243 100 0 n#699 hwin.hwin#733.in 736 240 896 240 ecalcs.AgDiffCalc.INPB
w 1212 171 100 0 n#698 ecalcs.AgDiffCalc.VAL 1184 80 1216 80 1216 272 1312 272 esirs.AgDiff.INP
w -262 1219 -100 0 c#631 esirs.State.FLNK -256 1216 -208 1216 outhier.SFLNK.p
w -1166 291 100 0 n#629 hwin.hwin#628.in -1184 288 -1088 288 esirs.Reject.INP
w 1272 1091 -100 0 c#625 comp1mSadHealth.comp1mSadHealth#619.VAL 1280 1088 1312 1088 bihier.HVAL.p
w 1274 1027 -100 0 c#622 comp1mSadHealth.comp1mSadHealth#619.FLNK 1280 1024 1328 1024 outhier.HFLNK.p
w 1272 1059 -100 0 c#623 comp1mSadHealth.comp1mSadHealth#619.OMSS 1280 1056 1312 1056 bihier.HOMSS.p
w 1306 -437 -100 0 c#617 bihier.NVAL.p 1360 -448 1312 -448 esirs.Name.VAL
w 348 -453 100 0 n#609 esirs.EngName.FLNK 256 -288 352 -288 352 -608 512 -608 elutins.NameLut.SLNK
w 354 -317 -100 0 n#608 elutins.NameLut.INPA 512 -320 256 -320 esirs.EngName.VAL
w 802 -413 100 0 n#607 elutins.NameLut.VAL 768 -416 896 -416 esirs.Name.INP
w 796 -485 100 0 n#606 elutins.NameLut.FLNK 768 -384 800 -384 800 -576 896 -576 esirs.Name.SLNK
w 1314 -413 -100 0 c#613 outhier.NFLNK.p 1376 -416 1312 -416 esirs.Name.FLNK
w -312 -285 100 0 n#604 hwin.hwin#612.in -416 -288 -160 -288 esirs.EngName.INP
w 402 715 -100 0 c#585 outhier.PFLNK.p 480 704 384 704 esirs.Parked.FLNK
w 394 683 -100 0 c#587 bihier.PVAL.p 464 672 384 672 esirs.Parked.VAL
w -734 611 100 0 n#577 hwin.hwin#583.in -832 608 -576 608 egenSub.ParkSub.INPB
w -734 675 100 0 n#576 hwin.hwin#584.in -832 672 -576 672 egenSub.ParkSub.INPA
w -190 707 100 0 n#575 egenSub.ParkSub.VALA -288 704 -32 704 esirs.Parked.INP
w 570 1219 -100 0 c#573 esirs.Datumed.FLNK 576 1216 624 1216 outhier.DFLNK.p
w 568 1187 -100 0 c#574 esirs.Datumed.VAL 576 1184 608 1184 bihier.DVAL.p
s 1120 -80 100 768 This "calc" record
s 1120 -112 100 768 converts engineering
s 1120 -144 100 768 position back to
s 1120 -176 100 768 user units.
s 976 -576 100 768 position.
s 976 -544 100 768 position into a named
s 976 -512 100 768 Convert an engineering
s -80 -384 100 768 Engineering name
s -80 -416 100 768 of position.
s 352 576 100 768 Parked output and link
s 1040 -1024 400 1280 comp1mSadState
s -592 1120 100 768 This SIR record is
s -592 1024 100 768 notation code.
s -592 1088 100 768 updated via channel
s -592 1056 100 768 access from the state
s -1264 1120 100 768 This text provides a
s -1264 1024 100 768 of a component.
s -1264 1088 100 768 convenient way to
s -1264 1056 100 768 obtain a description
s 528 1072 100 0 1=Not datumed
s 528 1104 100 0 0=Datumed
s 240 1120 100 768 The "Datumed" SIR
s 240 1088 100 768 record is updated by
s 240 1056 100 768 channel access from
s 240 1024 100 768 the state notation
s 240 992 100 768 code.
[cell use]
use outhier -272 -608 100 1536 TFLNK
xform 0 -304 -608
use outhier 1392 -416 100 1536 NFLNK
xform 0 1360 -416
use outhier 496 704 100 1536 PFLNK
xform 0 464 704
use outhier 640 1216 100 1536 DFLNK
xform 0 608 1216
use outhier 1344 1024 100 1536 HFLNK
xform 0 1312 1024
use outhier -192 1216 100 1536 SFLNK
xform 0 -224 1216
use bihier -272 -640 100 1536 TVAL
xform 0 -304 -640
use bihier 1392 -448 100 1536 NVAL
xform 0 1360 -448
use bihier 496 672 100 1536 PVAL
xform 0 464 672
use bihier 640 1184 100 1536 DVAL
xform 0 608 1184
use bihier 1344 1056 100 1536 HOMSS
xform 0 1312 1056
use bihier 1344 1088 100 1536 HVAL
xform 0 1312 1088
use esirs 1376 304 100 768 AgDiff
xform 0 1520 176
p 1376 16 100 768 1 DESC:Offset error in real-world units
p 1263 -400 100 0 0 EGU:user units
p 1376 -16 100 768 1 FDSC:Offset error in real-world units
p 1376 -48 100 768 1 FTVL:DOUBLE
p 1376 -144 100 768 1 PREC:3
p 1376 -112 100 768 1 PV:$(sadtop)$(mech)
p 1376 -80 100 768 1 SNAM:
use esirs 128 336 100 768 AgEngDiff
xform 0 272 208
p 128 48 100 768 1 DESC:Offset error in engineering units
p 15 -368 100 0 0 EGU:ustep
p 128 16 100 768 1 FDSC:Offset error in engineering units
p 128 -16 100 768 1 FTVL:DOUBLE
p 128 -112 100 768 1 PREC:3
p 128 -80 100 768 1 PV:$(sadtop)$(mech)
p 128 -48 100 768 1 SNAM:
p 64 304 75 1280 -1 palrm(INP):MS
p 16 304 75 1024 -1 pproc(INP):PP
use esirs -1024 320 100 768 Reject
xform 0 -880 192
p -1024 32 100 768 1 DESC:Combined CAR state [GOOD=0|REJECT=-1]
p -1024 -160 100 768 1 EGU:0/-1
p -1024 0 100 768 1 FDSC:Combined CAR state [GOOD=0|REJECT=-1]
p -1024 -32 100 768 1 FTVL:LONG
p -1024 -96 100 768 1 PV:$(sadtop)$(mech)
p -1024 -128 100 768 1 SCAN:.5 second
p -1024 -64 100 768 1 SNAM:
use esirs -96 -256 100 768 EngName
xform 0 48 -384
p -96 -544 100 768 1 DESC:Engineering name of position
p -96 -736 100 768 1 EGU:
p -96 -576 100 768 1 FDSC:Engineering name of position
p -96 -608 100 768 1 FTVL:STRING
p -96 -704 100 768 1 PV:$(sadtop)$(mech)
p -96 -672 100 768 1 SCAN:.5 second
p -96 -640 100 768 1 SNAM:
p -160 -288 75 1280 -1 palrm(INP):MS
use esirs 960 -384 100 768 Name
xform 0 1104 -512
p 960 -672 100 768 1 DESC:Real-world name of position
p 847 -1088 100 0 0 EGU:
p 960 -704 100 768 1 FDSC:Real-world name of position
p 960 -736 100 768 1 FTVL:STRING
p 960 -800 100 768 1 PV:$(sadtop)$(mech)
p 960 -768 100 768 1 SNAM:
p 896 -416 75 1280 -1 palrm(INP):MS
use esirs 32 736 100 768 Parked
xform 0 176 608
p 32 448 100 768 1 DESC:Parked? [YES=0|NO=1]
p 32 256 100 768 1 EGU:0/1
p 32 416 100 768 1 FDSC:Parked? [YES=0|NO=1]
p 32 384 100 768 1 FTVL:LONG
p 32 320 100 768 1 PV:$(sadtop)$(mech)
p 32 288 100 768 1 SCAN:.5 second
p 32 352 100 768 1 SNAM:
p -80 704 75 1024 -1 pproc(INP):PP
use esirs -608 1248 100 768 State
xform 0 -464 1120
p -608 960 100 768 1 DESC:SNL state and messages
p -576 784 100 0 0 EGU:
p -608 928 100 768 1 FDSC:SNL state and messages
p -608 896 100 768 1 FTVL:STRING
p -608 832 100 768 1 PV:$(sadtop)$(mech)
p -608 864 100 768 1 SNAM:
use esirs -1280 1248 100 768 Label
xform 0 -1136 1120
p -1280 960 100 768 1 DESC:The FDSC field describes the component
p -1393 544 100 0 0 EGU:
p -1280 928 100 768 1 FDSC:$(name) $(desc)
p -1280 896 100 768 1 FTVL:STRING
p -1280 800 100 768 1 PV:$(sadtop)$(mech)
p -1280 864 100 768 1 SNAM:
p -1280 832 100 768 1 VAL:$(name) $(desc)
use esirs 224 1248 100 768 Datumed
xform 0 368 1120
p 224 960 100 768 1 DESC:Datumed? [YES=0|NO=1]
p 111 544 100 0 0 EGU:0/1
p 224 928 100 768 1 FDSC:Datumed? [YES=0|NO=1]
p 224 896 100 768 1 FTVL:LONG
p 224 832 100 768 1 PV:$(sadtop)$(mech)
p 224 864 100 768 1 SNAM:
use esirs -704 -576 100 768 InToler
xform 0 -560 -704
p -704 -864 100 768 1 DESC:Is offset in tolerance? [YES=0|NO=1]
p -672 -1040 100 0 0 EGU:0/1
p -704 -896 100 768 1 FDSC:Is offset in tolerance? [YES=0|NO=1]
p -704 -928 100 768 1 FTVL:LONG
p -704 -992 100 768 1 PV:$(sadtop)$(mech)
p -704 -960 100 768 1 SNAM:
use hwin 544 199 100 0 hwin#733
xform 0 640 240
p 560 224 100 768 -1 val(in):$(top)$(mech)Scale.VAL
use hwin 544 167 100 0 hwin#731
xform 0 640 208
p 560 192 100 768 -1 val(in):$(top)$(mech)Offset.VAL
use hwin -1376 247 100 0 hwin#628
xform 0 -1280 288
p -1373 280 100 0 -1 val(in):$(top)$(mech)Val
use hwin -608 -329 100 0 hwin#612
xform 0 -512 -288
p -605 -296 100 0 -1 val(in):$(eng)$(mech)InvSelect.VAL
use hwin -1024 631 100 0 hwin#584
xform 0 -928 672
p -1021 664 100 0 -1 val(in):$(eng)$(mech)InvSelect.VAL
use hwin -1024 567 100 0 hwin#583
xform 0 -928 608
p -1021 600 100 0 -1 val(in):$(eng)$(mech)Hallstep.DATM
use ecalcs 960 304 100 768 AgDiffCalc
xform 0 1040 64
p 960 -208 100 768 1 CALC:{A-C}/B
p 608 -50 100 0 0 EGU:user units
p 960 -272 100 768 1 PREC:7
p 960 -240 100 768 1 PV:$(sadtop)$(mech)
p 896 272 75 1280 -1 palrm(INPA):MS
p 896 240 75 1280 -1 palrm(INPB):MS
p 896 208 75 1280 -1 palrm(INPC):MS
use comp1mSadHealth 896 967 100 0 comp1mSadHealth#619
xform 0 1088 1056
use elutins 576 -224 100 768 NameLut
xform 0 640 -432
p 576 -672 100 768 1 FDIR:$(pvdir)
p 576 -704 100 768 1 FNAM:$(mech).lut
p 512 -288 100 1280 1 FTVA:STRING
p 1408 -1668 100 0 0 FTVG:STRING
p 512 -434 100 0 0 PRIO:LOW
p 576 -736 100 768 1 PV:$(sadtop)$(mech)
p 576 -736 100 0 0 SELB:15
p 608 -432 100 0 -1 Type:lutin
p 908 -1168 100 0 0 def(INPB):0.000000000000000e+00
p 480 -320 75 768 -1 palrm(INPA):MS
p 560 -162 100 0 0 primitive:elutins
use comp1mSadEng 896 711 100 0 comp1mSadEng#588
xform 0 1088 800
use egenSub -512 736 100 768 ParkSub
xform 0 -432 336
p -720 704 100 768 1 FTA:STRING
p -720 640 100 768 1 FTB:LONG
p -720 576 100 768 1 FTC:STRING
p -720 512 100 768 1 FTD:STRING
p -720 448 100 768 1 FTE:STRING
p -720 384 100 768 1 FTF:STRING
p -224 704 100 768 1 FTVA:LONG
p -512 -96 100 768 1 INAM:engParkInit
p -512 -160 100 768 1 PV:$(sadtop)$(mech)
p -512 -128 100 768 1 SNAM:engParkSub
p -624 682 75 0 -1 pproc(INPA):PP
use notes -1376 -665 100 0 notes#542
xform 0 -1120 -464
p -1376 -672 100 768 0 author:Hubert Yamada
p -1360 -352 100 768 -1 comment0:This schematic provides a standard
p -1360 -386 100 768 -1 comment1:interface from the low-level engineering
p -1360 -418 100 768 -1 comment2:database to the high level SIR-based
p -1360 -448 100 768 -1 comment3:status and alarm database.  It also
p -1360 -480 100 768 -1 comment4:provides a few SIR records which are
p -1360 -512 100 768 -1 comment5:present in all components.
p -1120 -320 100 1024 -1 title:comp1mSadState
use notes -1376 -1049 100 0 notes#543
xform 0 -1120 -848
p -1376 -1056 100 768 0 author:S.M. Beard
p -1360 -736 100 768 -1 comment0:SIR records are linked together in
p -1360 -770 100 768 -1 comment1:situations where the same quantity is
p -1360 -802 100 768 -1 comment2:stored in two or more different forms
p -1360 -832 100 768 -1 comment3:(e.g. engineering position can be
p -1360 -864 100 768 -1 comment4:translated automatically into name).
use bc200tr -1504 -1176 -100 0 frame
xform 0 176 128
p 1072 -896 100 768 1 author:S.M.Beard
p 1296 -1024 100 0 -1 border:C
p 1072 -1024 100 768 1 checked:H.Yamada
p 1328 -1024 100 0 -1 date:1999-10-19
p 1580 -1020 100 1792 -1 page:1
p 1296 -896 100 0 -1 project:Near Infra-Red Imager / IR OIWFS
p 1072 -928 100 768 1 revised:H.Yamada
p 1072 -1040 100 0 1 revision:1.0
p 1296 -944 100 0 -1 title:Interface to low-level engineering
p 1296 -976 100 768 -1 title2:status records.
[comments]
