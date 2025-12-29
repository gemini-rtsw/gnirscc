[schematic2]
uniq 165
[tools]
[detail]
w -732 2155 100 0 n#159 inhier.DIR.P -764 2144 -640 2144 -640 2080 -544 2080 ecad2.pause.DIR
w -62 2187 100 0 n#157 ecad2.pause.VAL -224 2080 -160 2080 -160 2176 96 2176 outhier.VAL.p
w -94 2059 100 0 n#156 ecad2.pause.MESS -224 2048 96 2048 outhier.MESS.p
s 304 1708 150 0 doPause
[cell use]
use notask 48 1479 100 0 notask#164
xform 0 360 1712
use CBorder -1000 -76 -100 0 frame
xform 0 680 1228
p 2072 56 75 1536 -1 Author:Steven M. Beard
p 1568 60 100 1536 1 Date:10 Jun 97
p 1672 156 300 1792 -1 Dnumber:
p 1800 140 150 1536 -1 Title:pauseCad.sch
use notes 1664 1899 100 0 notes#160
xform 0 1920 2084
p 1692 2210 100 0 -1 COMMENT1:This implements the pause command.
use outhier 64 2007 100 0 MESS
xform 0 80 2048
use outhier 64 2135 100 0 VAL
xform 0 80 2176
use inhier -780 2103 100 0 DIR
xform 0 -764 2144
use ecad2 -416 2132 100 0 pause
xform 0 -384 1824
p -428 1600 65 0 1 PV:$(top)
p -428 1580 65 0 1 SNAM:pauseProc
p -848 1860 65 0 1 def(INPA):$(top)observeC.VAL
[comments]
