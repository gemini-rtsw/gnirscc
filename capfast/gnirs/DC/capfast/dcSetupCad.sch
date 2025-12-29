[schematic2]
uniq 251
[tools]
[detail]
w -230 955 100 0 n#250 ecad2.ecad2#242.FLNK 32 1184 144 1184 144 944 -544 944 -544 784 -416 784 elongouts.dcWcsmark.SLNK
w -488 827 100 0 CLID elongouts.dcWcsmark.DOL -416 816 -512 816 -512 896 -448 896 -448 1344 junction
w 674 1131 100 0 n#248 ecars.ecars#241.FLNK 672 1120 736 1120 hwout.hwout#247.outp
w -392 1355 100 0 CLID inhier.CLID.P -544 1456 -448 1456 -448 1344 -288 1344 ecad2.ecad2#242.A
w -446 1547 100 0 DIR inhier.DIR.P -544 1536 -288 1536 ecad2.ecad2#242.DIR
w 82 1515 100 0 MESS ecad2.ecad2#242.MESS 32 1504 192 1504 192 1440 272 1440 outhier.MESS.p
w 122 1547 100 0 VAL ecad2.ecad2#242.VAL 32 1536 272 1536 outhier.VAL.p
s -688 1344 100 0 always be marked
s 256 896 100 0 All this is done from the dcSetup SNL state logic
[cell use]
use elongouts -416 695 100 0 dcWcsmark
xform 0 -288 784
p -352 656 100 0 1 OMSL:closed_loop
p -192 704 100 0 1 def(OUT):$(top)selectWcs.H
use hwout 736 1079 100 0 hwout#247
xform 0 832 1120
p 832 1111 100 0 -1 val(outp):$(top)combCars5
use outhier 240 1495 100 0 VAL
xform 0 256 1536
use outhier 240 1399 100 0 MESS
xform 0 256 1440
use inhier -560 1495 100 0 DIR
xform 0 -544 1536
use inhier -560 1415 100 0 CLID
xform 0 -544 1456
use ecad2 -288 967 100 0 ecad2#242
xform 0 -128 1280
p -128 1600 100 1024 -1 name:$(top)dcSetup
use ecars 352 1063 100 0 ecars#241
xform 0 512 1232
p 512 1392 100 1024 -1 name:$(top)dcSetupC
use CBorder -952 -40 -100 0 frame
xform 0 728 1264
p 2120 92 75 1536 -1 Author:Matthieu Bec
p 1616 96 100 1536 1 Date:05312002
p 1720 192 300 1792 -1 Dnumber:
p 1848 176 150 1536 -1 Title:dcSetupCad.sch
use drRoiCad 1152 903 100 0 drRoiCad#180
xform 0 1232 1008
use obsSetupCad 1152 1127 100 0 obsSetupCad#177
xform 0 1232 1232
use arSetupCad 1152 1351 100 0 arSetupCad#175
xform 0 1232 1456
use notes 1732 1923 100 0 notes#172
xform 0 1988 2108
p 1760 2234 100 0 -1 COMMENT1:This is the top level schematic for the
p 1760 2202 100 0 -1 COMMENT2:detector controller.  It contains the
p 1760 2172 100 0 -1 COMMENT3:Apply, CAD and CAR database records.
p 1760 2140 100 0 -1 COMMENT4:The Client ID from the top level apply
p 1760 2108 100 0 -1 COMMENT5:record is the only CLID maintained
p 1760 2076 100 0 -1 COMMENT6:throughout the system.
p 1760 2044 100 0 -1 COMMENT7:
[comments]
