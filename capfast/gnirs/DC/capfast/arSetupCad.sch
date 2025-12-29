[schematic2]
uniq 127
[tools]
[detail]
w 210 2595 100 0 n#120 ebis.forceDownLd.VAL 96 2528 160 2528 160 2592 320 2592 ecad20.arSetup.INPI
w 178 2699 100 0 n#113 eais.biasValue.VAL 96 2688 320 2688 ecad20.arSetup.H
w 180 3083 100 0 n#112 estringouts.uCNameStr.OUT 100 3072 320 3072 ecad20.arSetup.B
w 212 3147 100 0 n#111 estringouts.uCPathStr.OUT 96 3232 164 3232 164 3136 320 3136 ecad20.arSetup.A
w 814 3307 100 0 n#4 ecad20.arSetup.MESS 640 3296 1024 3296 outhier.MESS.p
w 722 3339 100 0 n#3 ecad20.arSetup.VAL 640 3328 840 3328 840 3424 1024 3424 outhier.VAL.p
w 206 3339 100 0 n#2 inhier.DIR.P -32 3424 128 3424 128 3328 320 3328 ecad20.arSetup.DIR
s 168 2624 100 0 (forceDownLd)
s 632 3016 100 0 (VSet)
s 632 2952 100 0 (VddCl1)
s 632 2888 100 0 (VddCl2)
s 632 2824 100 0 (VggCl1)
s 632 2760 100 0 (VggCl2)
s 632 2696 100 0 (dBias)
s 644 2632 100 0 (VDet)
s 184 3136 100 0 (uCodePath)
s 184 3072 100 0 (uCodeName)
s 192 3008 100 0 (VSet)
s 192 2944 100 0 (VddCl1)
s 192 2880 100 0 (VddCl2)
s 192 2816 100 0 (VggCl1)
s 192 2752 100 0 (VggCl2)
s 192 2688 100 0 (dBias)
s 1084 2620 150 0 doArSetup
[cell use]
use estringins 904 1620 100 0 arrayIDStr
xform 0 960 1552
use estringins 1248 1628 100 0 arrayTypeStr
xform 0 1304 1560
use ebis -96 2600 100 0 forceDownLd
xform 0 -32 2544
p -384 2382 100 0 0 ONAM:TRUE
p -384 2414 100 0 0 ZNAM:FALSE
use ebis 2240 2288 100 0 pucDone
xform 0 2304 2224
p 1952 2062 100 0 0 ONAM:TRUE
p 2256 2200 65 1536 1 PV:$(top)
p 1952 2094 100 0 0 ZNAM:FALSE
use eaos 1736 1712 100 0 minReadout
xform 0 1792 1632
use eaos 1740 1932 100 0 maxSpeed
xform 0 1796 1852
p 1636 1578 100 0 0 EGU:Hz.
p 1412 1706 100 0 0 PREC:2
use eais 2240 3032 100 0 pucMinDly
xform 0 2304 2976
p 1965 3049 100 0 0 DESC:holding record for PreSetting ucDownLd commands
p 2256 2952 65 1536 1 PV:$(top)
use eais 2240 3224 100 0 pucMinRead
xform 0 2304 3168
p 1965 3241 100 0 0 DESC:holding record for PreSetting ucDownLd commands
p 2256 3144 65 1536 1 PV:$(top)
use eais 2240 3400 100 0 pucMinInt
xform 0 2304 3344
p 1965 3417 100 0 0 DESC:holding record for PreSetting ucDownLd commands
p 2256 3320 65 1536 1 PV:$(top)
use eais 2624 3400 100 0 pucDAvgsDly
xform 0 2688 3344
p 2349 3417 100 0 0 DESC:holding record for PreSetting ucDownLd commands
p 2640 3320 65 1536 1 PV:$(top)
use eais -88 2764 100 0 biasValue
xform 0 -32 2704
p -192 2638 100 0 0 HIGH:.9
p -192 2702 100 0 0 HIHI:1.2
p -192 2606 100 0 0 LOW:.05
p -416 2542 100 0 0 PREC:4
use estringouts -88 3308 100 0 uCPathStr
xform 0 -32 3248
use estringouts -84 3148 100 0 uCNameStr
xform 0 -28 3088
use task 1064 2531 100 0 task#107
xform 0 1184 2684
use uCodeDwnLd 1696 2920 100 0 uCodeDwnLd
xform 0 1760 2800
use ecars 1488 3464 100 0 arSetupC
xform 0 1544 3312
p 1488 3184 65 0 1 PV:$(top)
p 1716 3216 65 0 1 def(FLNK):$(top)combCars1.VAL
p 1132 3420 65 0 0 def(ICID):0.0
use CBorder -328 1184 -100 0 frame
xform 0 1352 2488
p 2240 1320 100 1536 1 Date: 23 Apr 97
p 2344 1416 300 1792 -1 Dnumber:
p 2452 1396 150 1536 -1 Title: arSetupCad.sch
p 2244 1352 100 1536 1 revision:0.9
use notes 2328 1539 100 0 notes#62
xform 0 2584 1724
p 2356 1850 65 0 -1 COMMENT1:arSetupCad - checks inputs on preset including loading info
p 2368 1836 65 0 -1 COMMENT2:from uCode command file to set preset variables needed to 
p 2368 1820 65 0 -1 COMMENT3:check other parameters which depend on the ucode.  On a START
p 2368 1804 65 0 -1 COMMENT4:a VxWorks task is fired and handles actually executing the 
p 2368 1788 65 0 -1 COMMENT5:appropriate functions and firing records in the correct order.
p 2368 1772 65 0 -1 COMMENT6:Some connections are obvious only in the C code which
p 2368 1756 65 0 -1 COMMENT7:implements the task.  Several parameters are VxWorks
p 2368 1740 65 0 -1 COMMENT8:strings to avoid the 40 character limit of Epics.
p 2376 1716 65 0 -1 COMMENT9:The task fires records with wFireMessage device support
p 2368 1700 65 0 -1 COMMENTA:and waits for the response message from the transputers.
p 2368 1684 65 0 -1 COMMENTB:The task routine also handles the control of the CAR record
p 2368 1668 65 0 -1 COMMENTC:and the handling and logging of error states.
use setBias 1696 2312 100 0 setBias
xform 0 1776 2192
use setVoltages 1728 2608 100 0 setVoltages
xform 0 1760 2496
use embbis 2240 2104 100 0 arSetupDone
xform 0 2304 2048
p 2304 2062 100 0 0 ONST:UNKNOWN
p 2112 2062 100 0 0 ONVL:1
p 1888 1966 100 0 0 PINI:NO
p 2256 2024 65 1536 1 PV:$(top)
p 2304 1998 100 0 0 THST:ERROR
p 2112 1998 100 0 0 THVL:3
p 2304 2030 100 0 0 TWST:BUSY
p 2112 2030 100 0 0 TWVL:2
p 2304 2094 100 0 0 ZRST:DONE
use elongins 2624 3224 100 0 pucFrmsPCycle
xform 0 2688 3168
p 2632 3144 65 1536 1 PV:$(top)
p 2624 3104 100 0 1 TPRO:1
use elongins 2624 3048 100 0 pucuCodeType
xform 0 2688 2992
p 2349 3065 100 0 0 DESC:holding record for PreSetting ucDownLd commands
p 2640 2968 65 1536 1 PV:$(top)
use outhier 992 3383 100 0 VAL
xform 0 1008 3424
use outhier 992 3255 100 0 MESS
xform 0 1008 3296
use inhier -48 3383 100 0 DIR
xform 0 -32 3424
use ecad20 392 3376 100 0 arSetup
xform 0 480 2496
p 416 3192 65 1536 1 FTVA:STRING
p 416 3176 65 1536 1 FTVB:STRING
p 416 3160 65 1536 1 FTVC:DOUBLE
p 416 3144 65 1536 1 FTVD:DOUBLE
p 416 3128 65 1536 1 FTVE:DOUBLE
p 416 3112 65 1536 1 FTVF:DOUBLE
p 416 3096 65 1536 1 FTVG:DOUBLE
p 416 3080 65 1536 1 FTVH:DOUBLE
p 416 3064 65 1536 1 FTVI:DOUBLE
p 424 2872 60 1536 1 FTVJ:LONG
p 416 2752 100 0 0 FTVK:DOUBLE
p 416 2720 100 0 0 FTVL:DOUBLE
p 416 2688 100 0 0 FTVM:DOUBLE
p 416 2656 100 0 0 FTVN:DOUBLE
p 416 2624 100 0 0 FTVO:DOUBLE
p 416 2592 100 0 0 FTVP:DOUBLE
p 416 2560 100 0 0 FTVQ:DOUBLE
p 416 2528 100 0 0 FTVR:DOUBLE
p 416 2496 100 0 0 FTVS:DOUBLE
p 416 2464 100 0 0 FTVT:DOUBLE
p 416 3044 65 0 1 PREC:3
p 416 3224 65 1536 1 PV:$(top)
p 416 3208 65 1536 1 SNAM:arSetupChk
p 320 2592 75 1280 -1 palrm(INPI):NMS
p 288 2592 75 1280 -1 pproc(INPI):PP
[comments]
