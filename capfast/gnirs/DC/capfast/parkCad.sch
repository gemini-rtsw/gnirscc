[schematic2]
uniq 165
[tools]
[detail]
w -732 2155 100 0 n#159 inhier.DIR.P -764 2144 -640 2144 -640 2080 -544 2080 ecad2.park.DIR
w -62 2187 100 0 n#157 ecad2.park.VAL -224 2080 -160 2080 -160 2176 96 2176 outhier.VAL.p
w -94 2059 100 0 n#156 ecad2.park.MESS -224 2048 96 2048 outhier.MESS.p
s 304 1768 150 0 doPark
[cell use]
use ecars 1024 1996 100 0 parkC
xform 0 1056 1840
p 1008 1708 65 0 1 PV:$(top)
p 1160 1708 65 0 1 def(FLNK):$(top)combCars3.VAL
p 692 1904 65 0 1 def(ICID):$(top)apply.CLID
use task 96 1655 100 0 task#163
xform 0 392 1824
use CBorder -1056 -132 -100 0 frame
xform 0 624 1172
p 2016 0 75 1536 -1 Author:Janet E. Tvedt
p 1512 4 100 1536 1 Date:24 Apr 97
p 1616 100 300 1792 -1 Dnumber:
p 1744 84 150 1536 -1 Title:parkCad.sch
use notes 1568 1867 100 0 notes#160
xform 0 1824 2052
p 1596 2178 100 0 -1 COMMENT1:This implements the park command.
use outhier 64 2135 100 0 VAL
xform 0 80 2176
use outhier 64 2007 100 0 MESS
xform 0 80 2048
use inhier -780 2103 100 0 DIR
xform 0 -764 2144
use ecad2 -412 2128 100 0 park
xform 0 -384 1824
p -428 1612 65 0 1 PV:$(top)
p -428 1592 65 0 1 SNAM:parkProc
p -844 1856 65 0 1 def(INPA):$(top)observeC.VAL
[comments]
