[schematic2]
uniq 165
[tools]
[detail]
w -62 2187 100 0 n#162 ecad2.reboot.VAL -224 2080 -160 2080 -160 2176 96 2176 outhier.VAL.p
w -732 2155 100 0 n#159 inhier.DIR.P -764 2144 -640 2144 -640 2080 -544 2080 ecad2.reboot.DIR
w -94 2059 100 0 n#156 ecad2.reboot.MESS -224 2048 96 2048 outhier.MESS.p
[cell use]
use CBorder -1016 -24 -100 0 frame
xform 0 664 1280
p 2056 108 75 1536 -1 Author:Janet E. Tvedt
p 1552 112 100 1536 1 Date:23 Apr 97
p 1656 208 300 1792 -1 Dnumber:
p 1784 192 150 1536 -1 Title:rebootCad.sch
use notes 1508 1863 100 0 notes#160
xform 0 1764 2048
p 1536 2174 100 0 -1 COMMENT1:This implements the reboot command.
p 1536 2142 100 0 -1 COMMENT2:.
use outhier 64 2135 100 0 VAL
xform 0 80 2176
use outhier 64 2007 100 0 MESS
xform 0 80 2048
use inhier -780 2103 100 0 DIR
xform 0 -764 2144
use ecad2 -424 2128 100 0 reboot
xform 0 -384 1824
p -440 1604 65 0 1 PV:$(top)
p -380 1584 65 1792 1 SNAM:rebootProc
p -824 1888 65 0 1 def(INPA):$(top)observeC.VAL
[comments]
