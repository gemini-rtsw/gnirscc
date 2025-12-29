[schematic2]
uniq 113
[tools]
[detail]
w 2130 1307 100 0 OMSS outhier.OMSS.p 2208 1296 2112 1296 2112 1344 2048 1344 ecars.cryoC.OMSS
w 1778 1003 100 0 n#111 elongouts.cryocar.OUT 1952 576 2048 576 2048 992 1568 992 1568 1408 1728 1408 ecars.cryoC.IVAL
w 1778 939 100 0 n#110 elongouts.cryocar.FLNK 1952 640 1984 640 1984 928 1632 928 1632 1216 1728 1216 ecars.cryoC.SLNK
w 1666 651 100 0 n#109 hwin.hwin#108.in 1696 640 1696 640 elongouts.cryocar.DOL
w 2098 1451 100 0 OVAL ecars.cryoC.VAL 2048 1408 2080 1408 2080 1440 2176 1440 outhier.OVAL.p
w 1474 1547 100 0 n#105 ecad8.cryo.OCID 1376 1536 1632 1536 1632 1376 1728 1376 ecars.cryoC.ICID
w 1474 771 100 0 n#103 ecad8.cryo.STLK 1376 768 1632 768 1632 608 1696 608 elongouts.cryocar.SLNK
w 2088 1379 100 0 OCID bihier.OCID.p 2176 1376 2048 1376 ecars.cryoC.CLID
w 2088 1187 100 0 FLNK outhier.FLNK.p 2176 1184 2048 1184 ecars.cryoC.FLNK
w 442 587 100 0 n#101 estringouts.estringouts#96.OUT 16 576 928 576 928 1248 1056 1248 ecad8.cryo.D
w -312 603 100 0 n#99 ebis.cryoSwitch.FLNK -384 656 -336 656 -336 592 -240 592 estringouts.estringouts#96.SLNK
w -336 635 100 0 n#98 ebis.cryoSwitch.VAL -384 624 -240 624 estringouts.estringouts#96.DOL
w 228 1051 100 0 n#93 embbis.C_M_SWi.FLNK 128 1408 224 1408 224 704 448 704 egenSub.cryoSub.SLNK
w 872 1291 100 0 n#91 egenSub.cryoSub.VALC 736 1280 1056 1280 ecad8.cryo.INPC
w 872 1355 100 0 n#90 egenSub.cryoSub.VALB 736 1344 1056 1344 ecad8.cryo.INPB
w 872 1419 100 0 n#89 egenSub.cryoSub.VALA 736 1408 1056 1408 ecad8.cryo.INPA
w 268 1067 100 0 n#86 embbis.C_SWi.VAL 160 896 272 896 272 1248 448 1248 egenSub.cryoSub.INPC
w 320 1315 100 0 n#85 embbis.M_SWi.VAL 192 1120 240 1120 240 1312 448 1312 egenSub.cryoSub.INPB
w 264 1379 100 0 n#84 embbis.C_M_SWi.VAL 128 1376 448 1376 egenSub.cryoSub.INPA
w -200 955 100 0 n#83 elongins.C_SW.VAL -368 880 -256 880 -256 944 -96 944 embbis.C_SWi.INP
w -256 923 100 0 n#82 elongins.C_SW.FLNK -368 912 -96 912 embbis.C_SWi.SLNK
w -248 1147 100 0 n#81 elongins.M_SW.FLNK -384 1136 -64 1136 embbis.M_SWi.SLNK
w -184 1179 100 0 n#80 elongins.M_SW.VAL -384 1104 -256 1104 -256 1168 -64 1168 embbis.M_SWi.INP
w -232 1435 100 0 n#79 elongins.C_M_SW.VAL -384 1360 -288 1360 -288 1424 -128 1424 embbis.C_M_SWi.INP
w -280 1403 100 0 n#78 elongins.C_M_SW.FLNK -384 1392 -128 1392 embbis.C_M_SWi.SLNK
w -648 939 100 0 n#59 hwin.hwin#58.in -624 928 -624 928 elongins.C_SW.INP
w -664 1163 100 0 n#57 hwin.hwin#56.in -640 1152 -640 1152 elongins.M_SW.INP
w -664 1419 100 0 n#55 hwin.hwin#54.in -640 1408 -640 1408 elongins.C_M_SW.INP
w 952 1611 100 0 CLID inhier.CLID.P 832 1664 896 1664 896 1600 1056 1600 ecad8.cryo.ICID
w 1448 1603 100 0 MESS ecad8.cryo.MESS 1376 1600 1568 1600 outhier.MESS.p
w 1470 1731 100 0 VAL ecad8.cryo.VAL 1376 1632 1408 1632 1408 1728 1568 1728 outhier.VAL.p
w 924 1691 100 0 n#39 inhier.DIR.P 832 1760 928 1760 928 1632 1056 1632 ecad8.cryo.DIR
[cell use]
use outhier 2176 1255 100 0 OMSS
xform 0 2192 1296
use outhier 2144 1399 100 0 OVAL
xform 0 2160 1440
use outhier 1536 1687 100 0 VAL
xform 0 1552 1728
use outhier 1536 1559 100 0 MESS
xform 0 1552 1600
use outhier 2144 1143 100 0 FLNK
xform 0 2160 1184
use hwin -816 887 100 0 hwin#58
xform 0 -720 928
p -813 920 100 0 -1 val(in):@cryoCpuControl
use hwin -832 1111 100 0 hwin#56
xform 0 -736 1152
p -829 1144 100 0 -1 val(in):@cryoOnOffSw
use hwin -832 1367 100 0 hwin#54
xform 0 -736 1408
p -829 1400 100 0 -1 val(in):@cryoSelectSw
use hwin 1504 599 100 0 hwin#108
xform 0 1600 640
p 1507 632 100 0 -1 val(in):$(CAR_BUSY)
use elongouts 1696 519 100 0 cryocar
xform 0 1824 608
use ecars 1728 1127 100 0 cryoC
xform 0 1888 1296
use estringouts -240 519 100 0 estringouts#96
xform 0 -112 592
p -304 398 100 0 0 OMSL:closed_loop
use embbis -96 839 100 0 C_SWi
xform 0 32 912
p 32 926 100 0 0 ONST:ON
p -160 926 100 0 0 ONVL:1
p 32 894 100 0 0 TWST:ON
p 32 958 100 0 0 ZRST:OFF
use embbis -64 1063 100 0 M_SWi
xform 0 64 1136
p 64 1150 100 0 0 ONST:ON
p -128 1150 100 0 0 ONVL:1
p 64 1118 100 0 0 TWST:ON
p 64 1182 100 0 0 ZRST:OFF
use embbis -128 1319 100 0 C_M_SWi
xform 0 0 1392
p -32 1376 100 0 0 DTYP:Soft Channel
p 0 1406 100 0 0 ONST:MANUAL
p -192 1406 100 0 0 ONVL:1
p 0 1438 100 0 0 ZRST:COMPUTER
use egenSub 448 615 100 0 cryoSub
xform 0 592 1040
p 528 1376 100 0 1 FTA:STRING
p 528 1312 100 0 1 FTB:STRING
p 528 1232 100 0 1 FTC:STRING
p 528 1184 100 0 1 FTD:STRING
p 225 389 100 0 0 FTVA:STRING
p 225 389 100 0 0 FTVB:STRING
p 225 357 100 0 0 FTVC:STRING
p 225 325 100 0 0 FTVD:STRING
p 512 592 100 0 1 SNAM:cryoSub
use ebis -640 567 100 0 cryoSwitch
xform 0 -512 640
p -813 715 100 0 0 DESC:Cryo head switch user input
p -864 478 100 0 0 ONAM:ON
p -864 542 100 0 0 PINI:YES
p -864 510 100 0 0 ZNAM:OFF
use elongins -624 823 100 0 C_SW
xform 0 -496 896
p -640 976 100 0 1 DESC:Computer switch value
p -448 800 100 0 1 DTYP:Soft Channel
p -880 894 100 0 0 EGU:units
p -448 832 100 0 1 SCAN:Passive
p -512 816 100 1024 0 name:$(top)$(I)
use elongins -640 1047 100 0 M_SW
xform 0 -512 1120
p -656 1200 100 0 1 DESC:Cryo head Manual switch Value
p -464 1024 100 0 1 DTYP:Soft Channel
p -896 1118 100 0 0 EGU:units
p -464 1056 100 0 1 SCAN:Passive
p -528 1040 100 1024 0 name:$(top)$(I)
use elongins -640 1303 100 0 C_M_SW
xform 0 -512 1376
p -656 1456 100 0 1 DESC:Cryo Computer Manual switch
p -464 1280 100 0 1 DTYP:Soft Channel
p -896 1374 100 0 0 EGU:units
p -464 1312 100 0 1 SCAN:Passive
p -528 1296 100 1024 0 name:$(top)$(I)
use bihier 2160 1335 100 0 OCID
xform 0 2176 1376
use ecad8 1056 679 100 0 cryo
xform 0 1216 1184
p 1152 1536 100 0 0 DESC:Cryo cad record
p 1136 1408 100 0 1 FTVA:STRING
p 1136 1344 100 0 1 FTVB:STRING
p 1136 1280 100 0 1 FTVC:STRING
p 1136 1216 100 0 1 FTVD:STRING
p 1152 1184 100 0 0 FTVG:LONG
p 1152 1152 100 0 0 FTVH:LONG
p 1136 656 100 0 1 INAM:cryoCadInit
p 1152 960 100 0 0 SCAN:Passive
p 1136 608 100 0 1 SNAM:cryoCad
p 1280 560 100 1024 1 name:$(top)$(I)
p 1024 1408 75 1280 -1 pproc(INPA):NPP
use inhier 816 1623 100 0 CLID
xform 0 832 1664
use inhier 816 1719 100 0 DIR
xform 0 832 1760
use bc200tr -1008 -264 -100 0 frame
xform 0 672 1040
p 1584 -96 100 0 -1 author:Peter Ruckle
p 1584 -128 100 0 -1 date:5-26-2001
p 1824 -48 200 0 -1 filename:cryoCad.sch
p 1824 16 200 0 -1 system:GNIRS CC
[comments]
