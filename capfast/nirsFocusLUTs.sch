[schematic2]
uniq 61
[tools]
[detail]
w -520 -277 100 0 filterWheel1 inhier.filterWheel1.P -640 -288 -256 -288 free
w 528 139 100 0 n#58 hwin.hwin#57.in 128 960 288 960 288 128 816 128 ecad8.focusProc.INPG
w 1704 523 100 0 n#56 hwin.hwin#52.in 1712 512 1744 512 elongouts.car.DOL
w 2120 1035 100 0 OCID ecars.C.CLID 2080 1024 2208 1024 outhier.OCID.p
w 1296 651 100 0 OCID ecad8.focusProc.OCID 1136 640 1504 640 1504 1024 1760 1024 ecars.C.ICID
w 1808 619 100 0 n#55 elongouts.car.OUT 2000 448 2064 448 2064 608 1600 608 1600 1056 1760 1056 ecars.C.IVAL
w 1792 587 100 0 n#54 elongouts.car.FLNK 2000 512 2000 576 1632 576 1632 864 1760 864 ecars.C.SLNK
w 1376 11 100 0 n#53 ecad8.focusProc.FLNK 1136 0 1664 0 1664 480 1744 480 elongouts.car.SLNK
w 2088 1003 100 0 OMSS ecars.C.OMSS 2080 992 2144 992 2144 928 2208 928 outhier.OMSS.p
w 2136 1195 100 0 OVAL ecars.C.VAL 2080 1056 2112 1056 2112 1184 2208 1184 outhier.OVAL.p
w 2120 731 100 0 FLNK ecars.C.FLNK 2080 832 2080 720 2208 720 outhier.FLNK.p
w 544 331 100 0 n#44 hwin.hwin#43.in 128 1040 320 1040 320 320 816 320 ecad8.focusProc.INPD
w 592 523 100 0 n#42 hwin.hwin#32.in 128 1376 416 1376 416 512 816 512 ecad8.focusProc.INPA
w 584 459 100 0 n#41 hwin.hwin#33.in 128 1312 400 1312 400 448 816 448 ecad8.focusProc.INPB
w 576 395 100 0 n#40 hwin.hwin#37.in 128 1248 384 1248 384 384 816 384 ecad8.focusProc.INPC
w 568 203 100 0 n#39 hwin.hwin#35.in 128 1184 368 1184 368 192 816 192 ecad8.focusProc.INPF
w 560 75 100 0 n#38 hwin.hwin#36.in 128 1120 352 1120 352 64 816 64 ecad8.focusProc.INPH
w -514 -597 100 0 acquisition inhier.acquisition.P -640 -608 -256 -608 free
w 672 747 100 0 DIR inhier.DIR.P 576 736 816 736 ecad8.focusProc.DIR
w 382 843 100 0 DIR elongouts.focuspush.OUT 48 240 160 240 160 832 640 832 640 736 junction
w 1168 752 100 0 VAL ecad8.focusProc.VAL 1136 736 1344 736 outhier.VAL.p
w 1264 672 100 0 MESS ecad8.focusProc.MESS 1136 704 1440 704 outhier.MESS.p
w 640 715 100 0 ICID inhier.ICID.P 512 704 816 704 ecad8.focusProc.ICID
w -484 -469 100 0 camera inhier.camera.P -640 -480 -256 -480 free
w -478 -533 100 0 xdisp inhier.xdisp.P -640 -544 -256 -544 free
w -490 -405 100 0 grating inhier.grating.P -640 -416 -256 -416 free
w -520 -341 100 0 filterWheel2 inhier.filterWheel2.P -640 -352 -256 -352 free
w 1184 555 100 0 n#13 ecad8.focusProc.VALA 1136 544 1280 544 1280 256 1328 256 estringouts.focusStrout.SDIS
w 486 299 100 0 n#9 estringouts.focusbest.OUT -592 224 -320 224 -320 128 192 128 192 288 816 288 ecad8.focusProc.E
w -474 267 100 0 n#6 estringouts.focusbest.FLNK -592 256 -320 256 -320 272 -208 272 elongouts.focuspush.SLNK
w -274 315 100 0 n#5 hwin.hwin#4.in -304 368 -304 304 -208 304 elongouts.focuspush.DOL
[cell use]
use inhier -656 -329 100 0 filterWheel1
xform 0 -640 -288
use hwin -64 919 100 0 hwin#57
xform 0 32 960
p -176 992 100 0 -1 val(in):nirs:cc:fw1PosCad.B
use hwin -496 327 100 0 hwin#4
xform 0 -400 368
p -493 360 100 0 -1 val(in):2
use hwin -64 1335 100 0 hwin#32
xform 0 32 1376
p -176 1408 100 0 -1 val(in):nirs:cc:fw2PosCad.B
use hwin -64 1143 100 0 hwin#35
xform 0 32 1184
p -176 1200 100 0 -1 val(in):nirs:cc:xdispPosCad.B
use hwin -64 1079 100 0 hwin#36
xform 0 32 1120
p -176 1136 100 0 -1 val(in):nirs:cc:acqPosCad.B
use hwin -64 1271 100 0 hwin#33
xform 0 32 1312
p -176 1344 100 0 -1 val(in):nirs:cc:gratingPosCad.B
use hwin -64 1207 100 0 hwin#37
xform 0 32 1248
p -176 1264 100 0 -1 val(in):nirs:cc:cameraPosCad.B
use hwin -64 999 100 0 hwin#43
xform 0 32 1040
p -176 1072 100 0 -1 val(in):nirs:cc:focusStepsCad.A
use hwin 1520 471 100 0 hwin#52
xform 0 1616 512
p 1523 504 100 0 -1 val(in):$(CAR_BUSY)
use elongouts -208 183 100 0 focuspush
xform 0 -80 272
p -176 160 100 0 1 OMSL:closed_loop
p -96 176 100 1024 0 name:nirs:focus:$(I)
p 48 240 75 768 -1 pproc(OUT):PP
use elongouts 1744 391 100 0 car
xform 0 1872 480
p 1920 320 100 1024 1 name:nirs:focus:$(I)
use outhier 2176 679 100 0 FLNK
xform 0 2192 720
use outhier 2176 983 100 0 OCID
xform 0 2192 1024
use outhier 1312 768 100 0 VAL
xform 0 1328 736
use outhier 1408 663 100 0 MESS
xform 0 1424 704
use outhier 2176 1143 100 0 OVAL
xform 0 2192 1184
use outhier 2176 887 100 0 OMSS
xform 0 2192 928
use ecars 1760 775 100 0 C
xform 0 1920 944
p 1888 736 100 1024 1 name:nirs:focus:$(I)
use inhier -656 -585 100 0 xdisp
xform 0 -640 -544
use inhier -656 -521 100 0 camera
xform 0 -640 -480
use inhier -656 -457 100 0 grating
xform 0 -640 -416
use inhier -656 -393 100 0 filterWheel2
xform 0 -640 -352
use inhier 560 768 100 0 DIR
xform 0 576 736
use inhier -656 -649 100 0 acquisition
xform 0 -640 -608
use inhier 480 720 100 0 ICID
xform 0 512 704
use embbos -80 -89 100 0 mode
xform 0 48 0
p -80 -160 100 0 1 ONST:spatial
p -336 -34 100 0 0 PINI:YES
p -80 -192 100 0 1 TWST:imaging
p -80 -128 100 0 1 ZRST:spectral
p 32 -96 100 1024 0 name:nirs:focus:$(I)
use estringouts -848 167 100 0 focusbest
xform 0 -720 240
p -736 160 100 1024 0 name:nirs:focus:$(I)
use estringouts 1328 215 100 0 focusStrout
xform 0 1456 288
p 1568 240 100 0 -1 def(OUT):nirs:cc:focusStepsCad.A
p 1472 176 100 1024 1 name:nirs:focus:$(I)
use ecad8 816 -217 100 0 focusProc
xform 0 976 288
p 912 480 100 0 0 FTVA:LONG
p 912 0 100 0 0 SNAM:NIRSfocusProc
p 528 512 100 0 0 def(INPA):0.0
p 480 448 100 0 0 def(INPB):0.0
p 480 384 100 0 0 def(INPC):0.0
p 496 320 100 0 0 def(INPD):0.0
p 816 -448 100 0 0 def(INPE):0.0
p 480 192 100 0 0 def(INPF):0.0
p 816 -512 100 0 0 def(INPG):0.0
p 512 48 100 0 0 def(INPH):0.0
p 1200 464 100 0 -1 def(OUTB):nirs:focus:focusStrout.VAL
p 928 -224 100 1024 0 name:nirs:focus:$(I)
p 1136 448 75 768 -1 pproc(OUTB):PP
use ecad4 -816 567 100 0 loadFocusConf
xform 0 -656 944
p -880 368 100 0 0 FTVA:LONG
p -880 336 100 0 0 FTVB:LONG
p -880 16 100 0 0 SNAM:seqfocusLoadConfProc
p -704 560 100 1024 0 name:nirs:focus:$(I)
use bc200tr -1008 -968 -100 0 frame
xform 0 672 336
[comments]
