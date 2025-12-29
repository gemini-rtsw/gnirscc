[schematic2]
uniq 110
[tools]
[detail]
w 5704 3451 100 0 n#95 elongouts.drroi.DOL 5728 3440 5728 3440 hwin.hwin#82.in
w 4208 3200 100 0 SDIS ewaits.obsWait.SDIS 4400 3232 4128 3232 4128 4000 3920 4000 inhier.SDIS.P
w 4336 4099 100 0 SDIS junction 4128 4000 4128 4096 4592 4096 ewaits.arWait.SDIS
w 5064 4163 100 0 n#46 ewaits.arWait.VAL 4816 4160 5360 4160 ecalcs.arCalc.INPA
w 5036 3979 100 0 n#45 ewaits.arWait.FLNK 4816 4192 5040 4192 5040 3776 5360 3776 ecalcs.arCalc.SLNK
w 5442 3243 100 0 n#44 ecalcs.obsCalc.VAL 5360 3232 5584 3232 5584 3376 5728 3376 elongouts.drroi.SDIS
w 5600 3411 100 0 n#43 ecalcs.obsCalc.FLNK 5360 3264 5520 3264 5520 3408 5728 3408 elongouts.drroi.SLNK
w 4762 3307 100 0 n#42 ewaits.obsWait.VAL 4624 3296 4960 3296 4960 3424 5072 3424 ecalcs.obsCalc.INPA
w 4884 3179 100 0 n#41 ewaits.obsWait.FLNK 4624 3328 4880 3328 4880 3040 5072 3040 ecalcs.obsCalc.SLNK
w 4370 3371 100 0 n#40 ewaits.obsWait.INAN 4400 3360 4400 3360 hwin.hwin#81.in
w 4448 4227 100 0 n#39 ewaits.arWait.INAN 4592 4224 4352 4224 hwin.hwin#87.in
w 5872 3971 100 0 n#38 ecalcs.arCalc.VAL 5648 3968 6144 3968 6144 4064 6352 4064 elongouts.obs.SDIS
w 6322 4139 100 0 n#37 elongouts.obs.DOL 6352 4128 6352 4128 hwin.hwin#88.in
w 5824 4003 100 0 n#36 ecalcs.arCalc.FLNK 5648 4000 6048 4000 6048 4096 6352 4096 elongouts.obs.SLNK
[cell use]
use inhier 3984 3952 100 1792 SDIS
xform 0 3920 4000
p 3872 3936 100 0 0 IO:input
p 3808 3904 100 0 0 model:connector
p 3808 3872 100 0 0 revision:2.2
use bc200tr 3664 1944 -100 0 frame
xform 0 5344 3248
use ecalcs 5072 2951 100 0 obsCalc
xform 0 5216 3216
p 5168 3152 100 0 1 CALC:A#0
use ecalcs 5360 3687 100 0 arCalc
xform 0 5504 3952
p 5456 3888 100 0 1 CALC:A#0
use hwin 6160 4087 100 0 hwin#88
xform 0 6256 4128
p 6163 4120 100 0 -1 val(in):$(CAD_START)
use hwin 4160 4183 100 0 hwin#87
xform 0 4256 4224
p 4163 4216 100 0 -1 val(in):$(prefix)arSetupDone
use hwin 5536 3399 100 0 hwin#82
xform 0 5632 3440
p 5539 3432 100 0 -1 val(in):$(CAD_START)
use hwin 4208 3319 100 0 hwin#81
xform 0 4304 3360
p 4211 3352 100 0 -1 val(in):$(prefix)obsSetupDone
use elongouts 6376 4008 100 0 obs
xform 0 6480 4096
p 6464 3936 100 0 1 def(OUT):$(prefix)obsSetup.DIR
p 6608 4064 75 768 -1 pproc(OUT):PP
use elongouts 5752 3320 100 0 drroi
xform 0 5856 3408
p 6080 3376 100 0 1 def(OUT):$(prefix)drRoiSet.DIR
p 5984 3376 75 768 -1 pproc(OUT):PP
use ewaits 4592 4039 100 0 arWait
xform 0 4704 4160
p 4704 4160 100 256 -1 CALC:A
p 4592 3984 100 0 1 DOPT:Use VAL
p 4576 4192 100 1280 -1 INBP:No
p 4576 4160 100 1280 -1 INCP:No
p 4576 3936 100 768 1 OOPT:Every Time
p 4576 3904 100 0 1 SCAN:I/O Intr
use ewaits 4400 3175 100 0 obsWait
xform 0 4512 3296
p 4512 3296 100 256 -1 CALC:A
p 4400 3120 100 0 1 DOPT:Use VAL
p 4384 3328 100 1280 -1 INBP:No
p 4384 3296 100 1280 -1 INCP:No
p 4400 3072 100 768 1 OOPT:Every Time
p 4400 3040 100 0 1 SCAN:I/O Intr
[comments]
