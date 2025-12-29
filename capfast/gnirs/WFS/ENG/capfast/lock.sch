[schematic2]
uniq 598
[tools]
[detail]
w -990 923 100 0 n#597 elongins.TmpRd.VAL -1056 864 -1024 864 -1024 912 -896 912 embbis.Tmp.INP
w -1308 907 100 2 n#596 hwin.hwin#474.in -1312 912 -1312 912 elongins.TmpRd.INP
w -798 379 100 0 n#589 hwin.hwin#590.in -768 368 -768 368 elongins.TmpHB.INP
w -1246 331 100 0 n#583 esubs.TmpExtSub.INPB -1216 320 -1216 320 hwin.hwin#584.in
w -910 163 100 0 n#582 elongouts.TmpExtWr.DOL -832 160 -928 160 esubs.TmpExtSub.VAL
w -1246 363 100 0 n#578 hwin.hwin#575.in -1216 352 -1216 352 esubs.TmpExtSub.INPA
w -606 107 100 0 n#570 elongouts.TmpExtWr.OUT -576 96 -576 96 hwout.hwout#574.outp
w -1038 1323 100 0 n#564 elutouts.TmpSelect.OUTA -1248 1312 -768 1312 -768 1216 -800 1216 elongouts.ForceTmpWr.VAL
w -798 1187 100 0 n#549 elongouts.ForceTmpWr.OUT -800 1184 -736 1184 hwout.hwout#551.outp
w -1342 1979 100 0 n#548 hwin.hwin#547.in -1312 1968 -1312 1968 elongins.CfgBusy.INP
w -830 2011 100 0 n#546 hwin.hwin#545.in -800 2000 -800 2000 elongins.InitBusy.INP
w -1342 1595 100 0 n#544 hwin.hwin#543.in -1312 1584 -1312 1584 elongins.GenBusy.INP
w -1342 1787 100 0 n#542 hwin.hwin#541.in -1312 1776 -1312 1776 elongins.ObsBusy.INP
w -814 715 100 0 n#468 embbos.TmpMenu.OUT -1056 608 -960 608 -960 704 -608 704 -608 608 -640 608 elongouts.TmpWr.VAL
w 420 772 100 768 n#531 embbos.GenMenu.OUT 352 736 416 736 416 832 junction
w 322 835 100 0 n#531 elutouts.GenSelect.OUTA -128 832 832 832 832 736 768 736 elongouts.GenWr.VAL
w 412 1316 100 768 n#529 embbos.ObsMenu.OUT 352 1280 416 1280 416 1376 junction
w 306 1372 100 768 n#529 elutouts.ObsSelect.OUTA -128 1376 800 1376 800 1280 768 1280 elongouts.ObsWr.VAL
w 412 1860 100 768 n#525 embbos.CfgMenu.OUT 352 1824 416 1824 416 1920 junction
w 306 1916 100 768 n#525 elutouts.CfgSelect.OUTA -128 1920 800 1920 800 1824 768 1824 elongouts.CfgWr.VAL
w 770 1251 100 0 n#380 elongouts.ObsWr.OUT 768 1248 832 1248 hwout.hwout#383.outp
w 1026 1019 100 0 n#509 hwin.hwin#501.in 1056 1008 1056 1008 elongins.MaxMove.INP
w 1282 747 100 0 n#508 hwout.hwout#504.outp 1312 736 1312 736 elongouts.MaxMoveSet.OUT
w 610 331 100 0 n#507 embbos.InitMenu.OUT 352 224 448 224 448 320 832 320 832 224 768 224 elongouts.InitWr.VAL
w 770 195 100 0 n#497 elongouts.InitWr.OUT 768 192 832 192 hwout.hwout#498.outp
w 370 451 100 0 n#488 elongins.InitRd.VAL 352 448 448 448 448 512 512 512 embbis.Init.INP
w 66 507 100 0 n#486 hwin.hwin#494.in 96 496 96 496 elongins.InitRd.INP
w -670 587 100 0 n#465 hwout.hwout#470.outp -640 576 -640 576 elongouts.TmpWr.OUT
w 1026 1723 100 0 n#403 hwin.hwin#404.in 1056 1712 1056 1712 elongins.Active.INP
w 1026 1499 100 0 n#399 hwin.hwin#401.in 1056 1488 1056 1488 elongins.Moving.INP
w 370 2051 100 0 n#396 elongins.CfgRd.VAL 352 2048 448 2048 448 2112 512 2112 embbis.Cfg.INP
w 770 1795 100 0 n#390 elongouts.CfgWr.OUT 768 1792 832 1792 hwout.hwout#393.outp
w 66 2107 100 0 n#389 hwin.hwin#391.in 96 2096 96 2096 elongins.CfgRd.INP
w 370 1507 100 0 n#386 elongins.ObsRd.VAL 352 1504 448 1504 448 1568 512 1568 embbis.Obs.INP
w 66 1563 100 0 n#379 hwin.hwin#381.in 96 1552 96 1552 elongins.ObsRd.INP
w 370 963 100 0 n#372 elongins.GenRd.VAL 352 960 448 960 448 1024 512 1024 embbis.Gen.INP
w 770 707 100 0 n#370 elongouts.GenWr.OUT 768 704 832 704 hwout.hwout#367.outp
w 66 1019 100 0 n#369 hwin.hwin#365.in 96 1008 96 1008 elongins.GenRd.INP
s -512 160 100 768 control the temperature interlock.
s -512 192 100 768 This record allows an arbitrary record to
s 864 512 200 0 Do not rename these records!
s 864 448 200 0 They are accessed by name from
s 864 384 200 0 the lockSad database and the 
s 864 320 200 0 lockCad SNL code.
s -1536 2160 100 768 A negative Busy value 
s -1536 2128 100 768 indicates that the Busy
s -1536 2096 100 768 value should be ignored.
[cell use]
use embbis -832 928 100 768 Tmp
xform 0 -768 880
p -832 704 100 768 1 ONST:COLD
p -832 800 100 768 1 SCAN:.2 second
p -832 736 100 768 1 TWST:WARM
p -832 768 100 768 1 ZRST:CHANGING
p -928 912 75 1280 -1 pproc(INP):PP
use hwin -960 327 100 0 hwin#590
xform 0 -864 368
p -957 360 100 0 -1 val(in):@hsLockTmpHB
use hwin -1408 279 100 0 hwin#584
xform 0 -1312 320
p -1405 312 100 0 -1 val(in):$(tmphb)
use hwin -1408 311 100 0 hwin#575
xform 0 -1312 352
p -1405 344 100 0 -1 val(in):$(tmpvar)
use hwin 864 967 100 0 hwin#501
xform 0 960 1008
p 867 1000 100 0 -1 val(in):@hsMaxMoving
use hwin -1504 871 100 0 hwin#474
xform 0 -1408 912
p -1501 904 100 0 -1 val(in):@hsLockTmp
use hwin -96 2055 100 0 hwin#391
xform 0 0 2096
p -93 2088 100 0 -1 val(in):@hsLockCfg
use hwin -96 1511 100 0 hwin#381
xform 0 0 1552
p -93 1544 100 0 -1 val(in):@hsLockObs
use hwin -96 967 100 0 hwin#365
xform 0 0 1008
p -93 1000 100 0 -1 val(in):@hsLockGen
use hwin 864 1447 100 0 hwin#401
xform 0 960 1488
p 867 1480 100 0 -1 val(in):@hsMoving
use hwin 864 1671 100 0 hwin#404
xform 0 960 1712
p 867 1704 100 0 -1 val(in):@hsActive
use hwin -96 455 100 0 hwin#494
xform 0 0 496
p -93 488 100 0 -1 val(in):@hsLockInit
use hwin -1504 1735 100 0 hwin#541
xform 0 -1408 1776
p -1501 1768 100 0 -1 val(in):-1
use hwin -1504 1543 100 0 hwin#543
xform 0 -1408 1584
p -1501 1576 100 0 -1 val(in):-1
use hwin -992 1959 100 0 hwin#545
xform 0 -896 2000
p -989 1992 100 0 -1 val(in):-1
use hwin -1504 1927 100 0 hwin#547
xform 0 -1408 1968
p -1501 1960 100 0 -1 val(in):-1
use hwin -1168 983 100 0 hwin#594
xform 0 -1072 1024
p -1165 1016 100 0 -1 val(in):2
use elongins -704 384 100 768 TmpHB
xform 0 -640 336
p -704 256 100 768 1 DTYP:vxWorks Variable (INST_IO)
p -704 256 100 768 1 SCAN:.2 second
use elongins -736 1888 100 768 TmpBusy
xform 0 -672 1840
use elongins 1120 1024 100 768 MaxMove
xform 0 1184 976
p 1120 896 100 768 1 DTYP:vxWorks Variable (INST_IO)
p 1120 864 100 768 1 SCAN:.2 second
use elongins -1248 928 100 768 TmpRd
xform 0 -1184 880
p -1248 800 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongins 160 2112 100 768 CfgRd
xform 0 224 2064
p 160 1984 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongins 160 1568 100 768 ObsRd
xform 0 224 1520
p 160 1440 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongins 160 1024 100 768 GenRd
xform 0 224 976
p 160 896 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongins 1120 1504 100 768 Moving
xform 0 1184 1456
p 1120 1376 100 768 1 DTYP:vxWorks Variable (INST_IO)
p 1120 1344 100 768 1 SCAN:.2 second
use elongins 1120 1728 100 768 Active
xform 0 1184 1680
p 1120 1600 100 768 1 DTYP:vxWorks Variable (INST_IO)
p 1120 1568 100 768 1 SCAN:.2 second
use elongins 160 512 100 768 InitRd
xform 0 224 464
p 160 384 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongins -1248 1984 100 768 CfgBusy
xform 0 -1184 1936
use elongins -1248 1792 100 768 ObsBusy
xform 0 -1184 1744
use elongins -1248 1600 100 768 GenBusy
xform 0 -1184 1552
use elongins -736 2016 100 768 InitBusy
xform 0 -672 1968
use esubs -1152 384 100 768 TmpExtSub
xform 0 -1072 144
p -1152 -128 100 768 1 INAM:tmpExtInit
p -1152 -160 100 768 1 SNAM:tmpExtSub
use hwout -576 55 100 0 hwout#574
xform 0 -480 96
p -480 87 100 0 -1 val(outp):@hsLockTmp
use hwout -736 1143 100 0 hwout#551
xform 0 -640 1184
p -640 1175 100 0 -1 val(outp):@hsForceLockTmp
use hwout -640 535 100 0 hwout#470
xform 0 -544 576
p -544 567 100 0 -1 val(outp):@hsLockTmp
use hwout 832 1751 100 0 hwout#393
xform 0 928 1792
p 928 1783 100 0 -1 val(outp):@hsLockCfg
use hwout 832 663 100 0 hwout#367
xform 0 928 704
p 928 695 100 0 -1 val(outp):@hsLockGen
use hwout 832 151 100 0 hwout#498
xform 0 928 192
p 928 183 100 0 -1 val(outp):@hsLockInit
use hwout 1312 695 100 0 hwout#504
xform 0 1408 736
p 1408 727 100 0 -1 val(outp):@hsMaxMoving
use hwout 832 1207 100 0 hwout#383
xform 0 928 1248
p 928 1239 100 0 -1 val(outp):@hsLockObs
use elongouts -768 192 100 768 TmpExtWr
xform 0 -704 128
p -768 32 100 768 1 DTYP:vxWorks Variable (INST_IO)
p -768 -32 100 768 1 OMSL:closed_loop
p -768 0 100 768 1 SCAN:$(tmpscan)
p -864 160 75 1280 -1 pproc(DOL):PP
use elongouts -992 1280 100 768 ForceTmpWr
xform 0 -928 1216
p -992 1120 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongouts -832 672 100 768 TmpWr
xform 0 -768 608
p -832 512 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongouts 576 1888 100 768 CfgWr
xform 0 640 1824
p 576 1728 100 768 1 DTYP:vxWorks Variable (INST_IO)
p 352 1742 100 0 0 OMSL:supervisory
use elongouts 576 800 100 768 GenWr
xform 0 640 736
p 576 640 100 768 1 DTYP:vxWorks Variable (INST_IO)
p 352 654 100 0 0 OMSL:supervisory
use elongouts 576 288 100 768 InitWr
xform 0 640 224
p 576 128 100 768 1 DTYP:vxWorks Variable (INST_IO)
p 352 142 100 0 0 OMSL:supervisory
use elongouts 1120 832 100 768 MaxMoveSet
xform 0 1184 768
p 1120 672 100 768 1 DTYP:vxWorks Variable (INST_IO)
use elongouts 576 1344 100 768 ObsWr
xform 0 640 1280
p 576 1184 100 768 1 DTYP:vxWorks Variable (INST_IO)
p 352 1198 100 0 0 OMSL:supervisory
use embbos 160 1344 100 768 ObsMenu
xform 0 224 1280
p 160 1152 100 768 1 ONST:LOCKED
p 160 1184 100 768 1 ZRST:UNLOCKED
p 352 1280 75 768 -1 pproc(OUT):PP
use embbos 160 1888 100 768 CfgMenu
xform 0 224 1824
p 160 1696 100 768 1 ONST:LOCKED
p 160 1728 100 768 1 ZRST:UNLOCKED
p 352 1824 75 768 -1 pproc(OUT):PP
use embbos 160 288 100 768 InitMenu
xform 0 224 224
p 160 96 100 768 1 ONST:INITIALIZING
p 160 128 100 768 1 ZRST:READY
p 352 224 75 768 -1 pproc(OUT):PP
use embbos -1248 672 100 768 TmpMenu
xform 0 -1184 608
p -1248 480 100 768 1 ONST:COLD
p -1248 448 100 768 1 TWST:WARM
p -1248 512 100 768 1 ZRST:CHANGING
p -1056 608 75 768 -1 pproc(OUT):PP
use embbos 160 800 100 768 GenMenu
xform 0 224 736
p 160 608 100 768 1 ONST:LOCKED
p 160 640 100 768 1 ZRST:UNLOCKED
p 352 736 75 768 -1 pproc(OUT):PP
use elutouts -320 960 100 768 GenSelect
xform 0 -256 768
p -320 544 100 768 1 FDIR:./ENG/data
p -320 512 100 768 1 FNAM:lockGen.lut
p -320 480 100 768 1 FTVA:LONG
p -128 832 75 768 -1 pproc(OUTA):PP
use elutouts -320 2048 100 768 CfgSelect
xform 0 -256 1856
p -320 1632 100 768 1 FDIR:./ENG/data
p -320 1600 100 768 1 FNAM:lockCfg.lut
p -320 1568 100 768 1 FTVA:LONG
p -128 1920 75 768 -1 pproc(OUTA):PP
use elutouts -320 1504 100 768 ObsSelect
xform 0 -256 1312
p -320 1088 100 768 1 FDIR:./ENG/data
p -320 1056 100 768 1 FNAM:lockObs.lut
p -320 1024 100 768 1 FTVA:LONG
p -128 1376 75 768 -1 pproc(OUTA):PP
use elutouts -1440 1440 100 768 TmpSelect
xform 0 -1376 1248
p -1440 1024 100 768 1 FDIR:./ENG/data
p -1440 992 100 768 1 FNAM:lockTmp.lut
p -1440 960 100 768 1 FTVA:LONG
p -1248 1312 75 768 -1 pproc(OUTA):PP
use embbis 576 2128 100 768 Cfg
xform 0 640 2080
p 576 1936 100 768 1 ONST:LOCKED
p 576 2000 100 768 1 SCAN:.2 second
p 576 1968 100 768 1 ZRST:UNLOCKED
p 480 2112 75 1280 -1 pproc(INP):PP
use embbis 576 1584 100 768 Obs
xform 0 640 1536
p 576 1392 100 768 1 ONST:LOCKED
p 576 1456 100 768 1 SCAN:.2 second
p 576 1424 100 768 1 ZRST:UNLOCKED
p 480 1568 75 1280 -1 pproc(INP):PP
use embbis 576 1040 100 768 Gen
xform 0 640 992
p 576 848 100 768 1 ONST:LOCKED
p 576 912 100 768 1 SCAN:.2 second
p 576 880 100 768 1 ZRST:UNLOCKED
p 480 1024 75 1280 -1 pproc(INP):PP
use embbis 576 528 100 768 Init
xform 0 640 480
p 576 336 100 768 1 ONST:INITIALIZING
p 576 400 100 768 1 SCAN:.2 second
p 576 368 100 768 1 ZRST:READY
p 480 512 75 1280 -1 pproc(INP):PP
use common 1216 1831 100 0 common#406
xform 0 1376 1968
use eborderC 1600 -160 100 1280 $Id:
xform 0 16 1024
p 1136 -64 200 1536 -1 file:lock.sch
p 1600 -160 100 1280 -1 id:$Id: lock.sch,v 1.3 2009/11/19 00:46:28 mrippa Exp $
[comments]
