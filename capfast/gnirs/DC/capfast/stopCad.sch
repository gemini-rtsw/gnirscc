[schematic2]
uniq 165
[tools]
[detail]
w -732 2155 100 0 n#159 inhier.DIR.P -764 2144 -640 2144 -640 2080 -544 2080 ecad4.stop.DIR
w -62 2187 100 0 n#157 ecad4.stop.VAL -224 2080 -160 2080 -160 2176 96 2176 outhier.VAL.p
w -94 2059 100 0 n#156 ecad4.stop.MESS -224 2048 96 2048 outhier.MESS.p
s 332 1664 150 0 doStop
s -732 1900 100 0 (data label)
s -224 1896 100 0 (data label)
s -664 1832 100 0 (filename)
s -224 1832 100 0 (filename)
[cell use]
use task 112 1555 100 0 task#164
xform 0 408 1724
use CBorder -1036 -124 -100 0 frame
xform 0 644 1180
p 2036 8 75 1536 -1 Author:Janet E. Tvedt
p 1532 12 100 1536 1 Date:24 Apr 97
p 1636 108 300 1792 -1 Dnumber:
p 1764 92 150 1536 -1 Title:stopCad.sch
use ecad4 -408 2132 100 0 stop
xform 0 -384 1760
p -424 1468 65 0 1 PV:$(top)
p -428 1444 65 0 1 SNAM:stopProc
p -856 1792 65 0 1 def(INPC):$(top)observeC.VAL
use notes 1660 1895 100 0 notes#160
xform 0 1916 2080
p 1688 2206 100 0 -1 COMMENT1:This implements the stop command.
use outhier 64 2007 100 0 MESS
xform 0 80 2048
use outhier 64 2135 100 0 VAL
xform 0 80 2176
use inhier -780 2103 100 0 DIR
xform 0 -764 2144
[comments]
