[schematic2]
uniq 276
[tools]
[detail]
w -542 1003 100 0 n#273 hwin.hwin#272.in -560 960 -560 992 -464 992 elongins.wcsnpoints.INP
w -582 891 100 0 n#271 ecad8.selectWcs.INPG -832 1408 -912 1408 -912 880 -192 880 -192 944 -208 944 elongins.wcsnpoints.VAL
w 722 1979 100 0 n#170 ecad4.setWcs.MESS 592 1968 912 1968 outhier.MESS.p
w 130 2059 100 0 n#274 ecad8.selectWcs.MESS -512 1984 -384 1984 -384 2048 704 2048 free
w 642 2011 100 0 n#172 ecad4.setWcs.VAL 592 2000 752 2000 752 2100 912 2100 outhier.VAL.p
w 114 2091 100 0 n#275 ecad8.selectWcs.VAL -512 2016 -416 2016 -416 2080 704 2080 free
w -926 1483 100 0 n#268 embbos.wcsaoname.VAL -992 1392 -960 1392 -960 1472 -832 1472 ecad8.selectWcs.INPF
w -1142 1179 100 0 n#264 ecalcs.wcsORdisable.FLNK -960 528 -800 528 -800 960 -896 960 -896 1168 -1328 1168 -1328 1936 -1296 1936 free
w 138 2011 100 0 n#263 elongouts.wcspush.OUT -32 1904 64 1904 64 2000 272 2000 ecad4.setWcs.DIR
w -922 2027 100 0 DIR inhier.DIR.P -1040 2064 -976 2064 -976 2016 -832 2016 ecad8.selectWcs.DIR
w -1206 811 100 0 n#261 ebos.wcsoverride.FLNK -1120 1088 -1008 1088 -1008 800 -1344 800 -1344 304 -1248 304 ecalcs.wcsORdisable.SLNK
w -1158 1195 100 0 n#260 ecalcs.wcsORdisable.VAL -960 496 -832 496 -832 896 -944 896 -944 1184 -1312 1184 -1312 1888 -1280 1888 free
w -1230 843 100 0 n#259 ebos.wcsoverride.VAL -1120 1056 -1024 1056 -1024 832 -1376 832 -1376 688 -1248 688 ecalcs.wcsORdisable.INPA
w -1022 1035 100 0 n#256 ebos.wcsoverride.OUT -1120 1024 -864 1024 hwout.hwout#255.outp
w -326 2139 100 0 n#252 hwin.hwin#251.in -304 2128 -288 2128 -288 1968 -288 1968 elongouts.wcspush.DOL
w -222 1835 100 0 n#250 estringouts.wcsfile.FLNK -48 1760 -48 1776 -32 1776 -32 1824 -352 1824 -352 1936 -288 1936 elongouts.wcspush.SLNK
w -926 1835 100 0 n#247 embbos.wcsport.OUT -992 1920 -960 1920 -960 1824 -832 1824 ecad8.selectWcs.A
w -486 1291 100 0 n#245 ecad8.selectWcs.FLNK -512 1280 -400 1280 -400 1744 -304 1744 estringouts.wcsfile.SLNK
w -414 1787 100 0 n#244 ecad8.selectWcs.VALB -512 1760 -464 1760 -464 1776 -304 1776 estringouts.wcsfile.DOL
w 2 1739 100 0 n#243 estringouts.wcsfile.OUT -48 1728 112 1728 112 1808 272 1808 ecad4.setWcs.A
w -390 1723 100 0 n#240 ecad8.selectWcs.VALA -512 1824 -416 1824 -416 1712 -304 1712 estringouts.wcsfile.SDIS
s 1152 1328 150 0 doSetWcs
[cell use]
use hwin -752 919 100 0 hwin#272
xform 0 -656 960
p -736 928 100 0 -1 val(in):@globalWcsnPts
use hwin -496 2087 100 0 hwin#251
xform 0 -400 2128
p -493 2120 100 0 -1 val(in):3
use elongins -464 887 100 0 wcsnpoints
xform 0 -336 960
p -604 1208 100 0 0 DTYP:vxWorks Variable (INST_IO)
use embbos -1248 1335 100 0 wcsaoname
xform 0 -1120 1424
p -1088 1390 100 0 0 FRST:4
p -1088 1358 100 0 0 FVST:5
p -1152 1312 100 0 1 ONST:IN
p -1088 1422 100 0 0 THST:3
p -1088 1454 100 0 0 TWST:2
p -1152 1280 100 0 1 ZRST:park-pos.
p -992 1424 75 768 -1 pproc(OUT):NPP
use embbos -1248 1831 100 0 wcsport
xform 0 -1120 1920
p -1088 1886 100 0 0 FRST:4
p -1088 1854 100 0 0 FVST:5
p -1088 1982 100 0 0 ONST:1
p -1088 1918 100 0 0 THST:3
p -1088 1950 100 0 0 TWST:2
p -1088 2014 100 0 0 ZRST:0
p -992 1920 75 768 -1 pproc(OUT):NPP
use ecad8 -832 1063 100 0 selectWcs
xform 0 -672 1568
p -736 1840 100 0 1 FTVA:LONG
p -752 1952 100 0 1 SNAM:selectWcs
p -1280 1744 100 0 1 def(INPB):$(cctop)cameraPosCad.B
p -1216 1664 100 0 1 def(INPC):$(agtop)port:nirs
p -1216 1600 100 0 1 def(INPD):$(top)setWcs.VALA
p -1216 1552 100 0 1 def(INPE):$(agtop)aoName
p -864 1664 75 1280 -1 pproc(INPC):CP
p -864 1536 75 1280 -1 pproc(INPE):CP
p -864 1408 75 1280 -1 pproc(INPG):PP
use ecalcs -1248 215 100 0 wcsORdisable
xform 0 -1104 480
p -1152 512 100 0 -1 CALC:A>0?0:1
use ebos -1376 967 100 0 wcsoverride
xform 0 -1248 1056
p -1360 864 100 0 -1 DTYP:vxWorks Variable (INST_IO)
p -1344 928 100 0 1 ONAM:Manual_Control
p -1344 896 100 0 1 ZNAM:AG_Control
use hwout -864 983 100 0 hwout#255
xform 0 -768 1024
p -768 1015 100 0 -1 val(outp):@overridePort
use elongouts -288 1847 100 0 wcspush
xform 0 -160 1936
p -32 1904 75 768 -1 pproc(OUT):PP
use CBorder -1520 -296 -100 0 frame
xform 0 160 1008
p 1552 -164 75 1536 -1 Author:Janet E. Tvedt
p 1048 -160 100 1536 1 Date:24 Apr 97
p 1152 -64 300 1792 -1 Dnumber:
p 1280 -80 150 1536 -1 Title:setWcsCad.sch
use estringouts 848 999 100 0 wcs_radecsys
xform 0 976 1072
use estringouts 432 423 100 0 wcs_ctype2
xform 0 560 496
use estringouts 432 999 100 0 wcs_ctype1
xform 0 560 1072
use estringouts -304 1671 100 0 wcsfile
xform 0 -176 1744
p -224 1648 100 0 1 OMSL:closed_loop
use egenSub -96 71 100 0 astCtx
xform 0 48 496
p 0 640 100 0 1 FTA:DOUBLE
p 0 608 100 0 1 FTB:STRING
p 0 576 100 0 1 FTC:STRING
p 0 544 100 0 1 FTD:DOUBLE
p 208 800 100 0 1 FTVB:STRING
p 208 752 100 0 1 FTVC:STRING
p 16 432 100 0 1 NOA:39
p 0 368 100 0 1 PINI:YES
p 32 64 100 0 1 SCAN:Passive
p -48 32 100 0 1 SNAM:updateAstCtx
p -528 832 100 0 1 def(INPA):tcs:ak:astCtx.VALA
p -608 752 100 0 1 def(INPB):tcs:sad:sourceATrackFrame
p -576 688 100 0 1 def(INPC):tcs:sad:sourceATrackEq
p -608 640 100 0 1 def(INPD):tcs:sad:sourceAWavelength
use ecad4 272 1303 100 0 setWcs
xform 0 432 1680
p 336 1440 100 0 1 SNAM:initWcsCad
use eaos 1312 71 100 0 wcs_cd22
xform 0 1440 160
use eaos 1312 327 100 0 wcs_cd12
xform 0 1440 416
use eaos 880 327 100 0 wcs_cd11
xform 0 1008 416
p 624 270 100 0 0 PREC:14
use eaos 880 71 100 0 wcs_cd21
xform 0 1008 160
use eaos 848 583 100 0 wcs_mjdobs
xform 0 976 672
use eaos 848 775 100 0 wcs_equinox
xform 0 976 864
use eaos 432 231 100 0 wcs_crpix2
xform 0 560 320
use eaos 432 7 100 0 wcs_crval2
xform 0 560 96
use eaos 432 583 100 0 wcs_crval1
xform 0 560 672
use eaos 432 775 100 0 wcs_crpix1
xform 0 560 864
use task 944 1207 100 0 task#180
xform 0 1240 1376
use notes 1008 1639 100 0 notes#160
xform 0 1264 1824
p 1036 1950 100 0 -1 COMMENT1:This implements the setWcs command.
use outhier 880 2059 100 0 VAL
xform 0 896 2100
use outhier 880 1927 100 0 MESS
xform 0 896 1968
use inhier -1056 2023 100 0 DIR
xform 0 -1040 2064
[comments]
