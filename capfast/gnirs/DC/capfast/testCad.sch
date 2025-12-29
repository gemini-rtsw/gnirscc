[schematic2]
uniq 168
[tools]
[detail]
w -62 2187 100 0 n#162 ecad2.test.VAL -224 2080 -160 2080 -160 2176 96 2176 outhier.VAL.p
w -732 2155 100 0 n#159 inhier.DIR.P -764 2144 -640 2144 -640 2080 -544 2080 ecad2.test.DIR
w -94 2059 100 0 n#156 ecad2.test.MESS -224 2048 96 2048 outhier.MESS.p
s 248 1768 150 0 doTest
[cell use]
use task 32 1647 100 0 task#167
xform 0 328 1816
use CBorder -1028 -68 -100 0 frame
xform 0 652 1236
p 2044 64 75 1536 -1 Author:Janet E. Tvedt
p 1540 68 100 1536 1 Date:24 Apr 97
p 1644 164 300 1792 -1 Dnumber:
p 1772 148 150 1536 -1 Title:testCad.sch
use notes 1600 1843 100 0 notes#160
xform 0 1856 2028
p 1628 2154 100 0 -1 COMMENT1:This implements the test command.
p 1628 2122 100 0 -1 COMMENT2:.
use outhier 64 2135 100 0 VAL
xform 0 80 2176
use outhier 64 2007 100 0 MESS
xform 0 80 2048
use inhier -780 2103 100 0 DIR
xform 0 -764 2144
use ecad2 -416 2128 100 0 test
xform 0 -384 1824
p -432 1604 65 0 1 PV:$(top)
p -380 1588 65 1792 1 SNAM:testProc
p -856 1856 65 0 1 def(INPA):$(top)observeC.VAL
[comments]
