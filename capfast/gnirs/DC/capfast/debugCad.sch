[schematic2]
uniq 174
[tools]
[detail]
w -110 1611 100 0 n#173 ecad2.debug.STLK -224 1600 64 1600 64 1856 164 1856 embbis.debugMode.SLNK
w -60 1899 100 0 n#172 ecad2.debug.VALA -224 1888 164 1888 embbis.debugMode.INP
w -62 2187 100 0 n#168 ecad2.debug.VAL -224 2080 -160 2080 -160 2176 96 2176 outhier.VAL.p
w 64 1263 100 0 n#166 ecad2.debug.A -544 1888 -740 1888 -740 1252 928 1252 928 768 832 768 estringouts.dbgStr.OUT
w 452 891 100 0 n#165 embbi.debugMenu.FLNK 336 1008 448 1008 448 784 576 784 estringouts.dbgStr.SLNK
w 426 819 100 0 n#164 embbi.debugMenu.VAL 336 816 576 816 estringouts.dbgStr.DOL
w -732 2155 100 0 n#159 inhier.DIR.P -764 2144 -640 2144 -640 2080 -544 2080 ecad2.debug.DIR
w -94 2059 100 0 n#156 ecad2.debug.MESS -224 2048 96 2048 outhier.MESS.p
s -592 1168 100 0 Debug Mode Menu
s -680 1900 100 0 (debug mode)
s -216 1904 100 0 (debug mode)
[cell use]
use CBorder -988 -96 -100 0 frame
xform 0 692 1208
p 2084 36 75 1536 -1 Author:Janet E. Tvedt
p 1580 40 100 1536 1 Date:24 Apr 97
p 1684 136 300 1792 -1 Dnumber:
p 1812 120 150 1536 -1 Title:debugCad.sch
use embbis 244 1920 100 0 debugMode
xform 0 292 1856
p 248 1768 65 0 1 ONST:NONE
p 100 1870 100 0 0 ONVL:1
p 248 1816 65 0 1 PV:$(top)
p 248 1736 65 0 1 THST:FULL
p 100 1806 100 0 0 THVL:3
p 248 1752 65 0 1 TWST:MIN
p 100 1838 100 0 0 TWVL:2
p 248 1784 65 0 1 ZRST:NOLOG
use estringouts 652 844 100 0 dbgStr
xform 0 704 784
p 660 668 100 0 1 OMSL:closed_loop
p 660 692 100 0 1 PV:$(top)
use embbi -252 1216 100 0 debugMenu
xform 0 -176 800
p -144 846 100 0 1 FRST:invalid
p -144 942 100 0 1 ONST:NONE
p -476 348 100 0 1 PV:$(top)
p -144 878 100 0 1 THST:FULL
p -144 910 100 0 1 TWST:MIN
p -144 974 100 0 1 ZRST:NOLOG
use notes 1600 1835 100 0 notes#160
xform 0 1856 2020
p 1628 2146 100 0 -1 COMMENT1:This implements the debug command.
use outhier 64 2135 100 0 VAL
xform 0 80 2176
use outhier 64 2007 100 0 MESS
xform 0 80 2048
use inhier -780 2103 100 0 DIR
xform 0 -764 2144
use ecad2 -412 2132 100 0 debug
xform 0 -384 1824
p -436 1608 65 0 1 FTVA:LONG
p -448 1792 100 0 0 FTVB:LONG
p -436 1552 65 0 1 PV:$(top)
p -436 1584 65 0 1 SNAM:debugProc
[comments]
