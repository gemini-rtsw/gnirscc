[schematic2]
uniq 72
[tools]
[detail]
w 3600 3227 100 0 n#71 link2Dir.link2Dir#65.OUT 3184 3248 3456 3248 3456 3216 3792 3216 elongouts.ccgetNames.DOL
w 3376 3291 100 0 n#70 link2Dir.link2Dir#65.FLNK 3184 3280 3616 3280 3616 3184 3792 3184 elongouts.ccgetNames.SLNK
w 3824 3443 100 0 n#69 elongouts.ccgetNames.VAL 4048 3184 4144 3184 4144 3440 3552 3440 3552 3312 3664 3312 hwout.hwout#61.outp
w 2896 3635 100 0 MESS outhier.MESS.p 3008 3632 2832 3632 ecad2.getNames.MESS
w 2902 3667 100 0 VAL outhier.VAL.p 3008 3664 2832 3664 ecad2.getNames.VAL
w 2374 3667 100 0 DIR inhier.DIR.P 2272 3664 2512 3664 ecad2.getNames.DIR
w 2368 3635 100 0 ICID inhier.ICID.P 2272 3632 2512 3632 ecad2.getNames.ICID
w 3768 3163 100 0 n#60 hwin.hwin#57.in 3792 3152 3792 3152 elongouts.ccgetNames.SDIS
w 2888 3163 100 0 n#33 ecad2.getNames.SPLK 2832 3152 2992 3152 link2Dir.link2Dir#65.SPLK
w 2888 3195 100 0 n#32 ecad2.getNames.STLK 2832 3184 2992 3184 link2Dir.link2Dir#65.STLK
w 2888 3227 100 0 n#31 ecad2.getNames.PLNK 2832 3216 2992 3216 link2Dir.link2Dir#65.PLNK
w 2888 3259 100 0 n#30 ecad2.getNames.CLNK 2832 3248 2992 3248 link2Dir.link2Dir#65.CLNK
w 2888 3291 100 0 n#29 ecad2.getNames.MLNK 2832 3280 2992 3280 link2Dir.link2Dir#65.MLNK
[cell use]
use link2Dir 2992 3031 100 0 link2Dir#65
xform 0 3088 3184
p 3032 2984 100 0 1 set0:mech getNames
use inhier 2240 3584 100 0 ICID
xform 0 2272 3632
p 2224 3568 100 0 0 IO:input
p 2160 3536 100 0 0 model:connector
p 2160 3504 100 0 0 revision:2.2
use inhier 2256 3696 100 0 DIR
xform 0 2272 3664
p 2224 3600 100 0 0 IO:input
p 2160 3568 100 0 0 model:connector
p 2160 3536 100 0 0 revision:2.2
use outhier 2976 3584 100 0 MESS
xform 0 2992 3632
p 2960 3568 100 0 0 IO:output
p 2912 3536 100 0 0 model:connector
p 2912 3504 100 0 0 revision:2.2
use outhier 2976 3696 100 0 VAL
xform 0 2992 3664
p 2960 3600 100 0 0 IO:output
p 2912 3568 100 0 0 model:connector
p 2912 3536 100 0 0 revision:2.2
use hwout 3664 3271 100 0 hwout#61
xform 0 3760 3312
p 3760 3303 100 0 -1 val(outp):$(top)cc:getNames.DIR 
use hwout 2432 2199 100 0 hwout#52
xform 0 2528 2240
p 2528 2231 100 0 -1 val(outp):$(top)acqstart.DIR PP NMS
use hwout 2432 2231 100 0 hwout#50
xform 0 2528 2272
p 2528 2263 100 0 -1 val(outp):$(top)focusstart.DIR PP NMS
use hwout 3312 2951 100 0 hwout#48
xform 0 3408 2992
p 3408 2983 100 0 -1 val(outp):$(top)fw1start.DIR PP NMS
use hwout 2432 2295 100 0 hwout#46
xform 0 2528 2336
p 2528 2327 100 0 -1 val(outp):$(top)xdispstart.DIR PP NMS
use hwout 2432 2327 100 0 hwout#44
xform 0 2528 2368
p 2528 2359 100 0 -1 val(outp):$(top)slitstart.DIR PP NMS
use hwout 2432 2359 100 0 hwout#42
xform 0 2528 2400
p 2528 2391 100 0 -1 val(outp):$(top)gratingstart.DIR PP NMS
use hwout 2432 2391 100 0 hwout#40
xform 0 2528 2432
p 2528 2423 100 0 -1 val(outp):$(top)deckerstart.DIR PP NMS
use hwout 2432 2423 100 0 hwout#38
xform 0 2528 2464
p 2528 2455 100 0 -1 val(outp):$(top)camerastart.DIR PP NMS
use hwout 2432 2455 100 0 hwout#36
xform 0 2528 2496
p 2528 2487 100 0 -1 val(outp):$(top)coverstart.DIR PP NMS
use hwout 3312 2983 100 0 hwout#34
xform 0 3408 3024
p 3408 3015 100 0 -1 val(outp):$(top)fw2start.DIR PP NMS
use hwin 3600 3111 100 0 hwin#57
xform 0 3696 3152
p 3603 3144 100 0 -1 val(in):$(top)ccDisabled
use elongouts 3792 3095 100 0 ccgetNames
xform 0 3920 3184
p 3760 3216 75 1280 -1 pproc(DOL):PP
p 4048 3152 75 768 -1 pproc(OUT):PP
use ecad2 2512 3095 100 0 getNames
xform 0 2672 3408
use ukatcBorderC 1840 1495 100 0 ukatcBorderC#0
xform 0 3520 2800
p 4780 1744 120 256 -1 Title:NIRS IS - Lookup Table Name Records
[comments]
