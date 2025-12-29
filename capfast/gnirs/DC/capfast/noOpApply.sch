[schematic2]
uniq 441
[tools]
[detail]
w -1438 459 100 0 n#440 hwout.hwout#439.outp -1408 448 -1408 448 cadCar.cadCar#438.FLNK
w -2202 131 100 0 n#245 inhier.DIR.P -2360 124 -2360 128 -1984 128 eapply.noopApply.DIR
w -1592 99 100 0 MESS eapply.noopApply.MESS -1600 96 -1536 96 -1536 128 -1472 128 outhier.MESS.p
w -1538 195 100 0 VAL eapply.noopApply.VAL -1600 128 -1568 128 -1568 192 -1472 192 outhier.VAL.p
w 244 123 100 0 n#424 hwin.hwin#423.in 256 112 292 112 noOpCar.noOpCar#422.CLID
w -1310 -1117 100 0 n#411 noOpCmd.datum.VAL -560 -192 -320 -192 -320 -1120 -2240 -1120 -2240 -224 -1984 -224 eapply.noopApply.INPE
w -1310 -1085 100 0 n#410 noOpCmd.datum.MESS -560 -224 -352 -224 -352 -1088 -2208 -1088 -2208 -256 -1984 -256 eapply.noopApply.INME
w -1056 -1053 100 0 n#409 noOpCmd.noOpCmd#421.VAL -64 -192 128 -192 128 -1056 -2180 -1056 -2180 -288 -1984 -288 eapply.noopApply.INPF
w -1054 -1021 100 0 n#407 noOpCmd.noOpCmd#421.MESS -64 -224 96 -224 96 -1024 -2144 -1024 -2144 -320 -1984 -320 eapply.noopApply.INMF
w -1120 999 100 0 n#374 eapply.noopApply.INMD -1984 -192 -2244 -192 -2244 996 64 996 64 232 -68 232 -68 228 noOpCmd.endGuide.MESS
w -1120 963 100 0 n#368 eapply.noopApply.INPD -1984 -160 -2208 -160 -2208 960 28 960 28 260 -68 260 noOpCmd.endGuide.VAL
w -1294 935 100 0 n#366 eapply.noopApply.INMC -1984 -128 -2176 -128 -2176 932 -352 932 -352 224 -560 224 noOpCmd.guide.MESS
w -1296 899 100 0 n#365 eapply.noopApply.INPC -1984 -96 -2148 -96 -2148 896 -384 896 -384 256 -560 256 noOpCmd.guide.VAL
w -1088 867 100 0 n#364 eapply.noopApply.INMB -1984 -64 -2112 -64 -2112 864 -4 864 -4 680 -64 680 -64 676 noOpCmd.endVerify.MESS
w -1086 835 100 0 n#361 eapply.noopApply.INPB -1984 -32 -2080 -32 -2080 832 -32 832 -32 708 -64 708 noOpCmd.endVerify.VAL
w -1280 803 100 0 n#360 eapply.noopApply.INMA -1984 0 -2052 0 -2052 800 -448 800 -448 672 -560 672 noOpCmd.verify.MESS
w -1280 771 100 0 n#359 eapply.noopApply.INPA -1984 32 -2020 32 -2020 768 -480 768 -480 704 -560 704 noOpCmd.verify.VAL
w -718 -397 100 0 n#357 eapply.noopApply.OUTF -1600 -288 -1120 -288 -1120 -400 -256 -400 -256 -192 -224 -192 noOpCmd.noOpCmd#421.DIR
w -1230 -221 100 0 n#355 eapply.noopApply.OUTE -1600 -224 -800 -224 -800 -192 -720 -192 noOpCmd.datum.DIR
w -1206 -157 100 0 n#353 eapply.noopApply.OUTD -1600 -160 -752 -160 -752 48 -288 48 -288 256 -228 256 -228 260 noOpCmd.endGuide.DIR
w -1238 -93 100 0 n#351 eapply.noopApply.OUTC -1600 -96 -816 -96 -816 256 -720 256 noOpCmd.guide.DIR
w -1262 -29 100 0 n#349 eapply.noopApply.OUTB -1600 -32 -864 -32 -864 512 -256 512 -256 708 -224 708 noOpCmd.endVerify.DIR
w -1286 35 100 0 n#347 eapply.noopApply.OUTA -1600 32 -912 32 -912 688 -720 688 -720 704 noOpCmd.verify.DIR
[cell use]
use hwout -1408 407 100 0 hwout#439
xform 0 -1312 448
p -1312 439 100 0 -1 val(outp):$(top)ovrNoopCar.VAL
use cadCar -1568 359 100 0 cadCar#438
xform 0 -1488 472
p -1568 352 100 0 1 set1:cad always
use hwin 64 71 100 0 hwin#423
xform 0 160 112
p 67 104 100 0 -1 val(in):$(top)apply.CLID
use noOpCar 288 -41 100 0 noOpCar#422
xform 0 368 64
use noOpCmd -224 -345 100 0 noOpCmd#421
xform 0 -144 -240
p -224 -352 100 0 1 set1:cmd endObserve
use noOpCmd -720 -345 -100 0 datum
xform 0 -640 -240
p -720 -348 100 0 1 set1:cmd datum
use noOpCmd -228 107 -100 0 endGuide
xform 0 -148 212
p -244 104 100 0 1 set1:cmd endGuide
use noOpCmd -720 103 -100 0 guide
xform 0 -640 208
p -732 100 100 0 1 set1:cmd guide
use noOpCmd -224 555 -100 0 endVerify
xform 0 -144 660
p -236 548 100 0 1 set1:cmd endVerify
use noOpCmd -720 551 -100 0 verify
xform 0 -640 656
p -728 548 100 0 1 set1:cmd verify
use CBorder -2496 -1392 -100 0 frame
xform 0 -816 -88
p 576 -1260 75 1536 -1 Author:Janet E. Tvedt
p 72 -1256 100 1536 1 Date:10 Jun 97
p 176 -1160 300 1792 -1 Dnumber:
p 304 -1176 150 1536 -1 Title:noOpApply.sch
use notes 160 619 100 0 notes#382
xform 0 416 804
p 184 916 100 0 -1 COMMENT1:Notes: This schematic contains system
p 184 884 100 0 -1 COMMENT2:level commands that will be rejected
p 184 852 100 0 -1 COMMENT3:or ignored by the detector controller.
p 184 820 100 0 -1 COMMENT4:The noOpCmd is replicated for each
p 184 788 100 0 -1 COMMENT5:command defined by the cmd macro.
p 184 756 100 0 -1 COMMENT6:All the (cmd) commands are ignored.
use outhier -1504 87 100 0 MESS
xform 0 -1488 128
use outhier -1504 151 100 0 VAL
xform 0 -1488 192
use inhier -2376 83 100 0 DIR
xform 0 -2360 124
use eapply -1844 208 100 0 noopApply
xform 0 -1792 -144
p -1896 -512 100 0 1 DESC:NAAC noop commands APPLY record
p -1896 -544 100 0 1 PV:$(top)
[comments]
