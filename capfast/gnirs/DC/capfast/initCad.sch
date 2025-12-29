[schematic2]
uniq 190
[tools]
[detail]
w -28 1899 100 0 n#185 ecad2.init.VALA -224 1888 228 1888 simulMode.simulMode#179.simModeIn
w -94 1611 100 0 n#181 ecad2.init.STLK -224 1600 96 1600 96 1664 228 1664 simulMode.simulMode#179.SLNK
w -62 2187 100 0 n#176 ecad2.init.VAL -224 2080 -160 2080 -160 2176 96 2176 outhier.VAL.p
w 48 1327 100 0 n#172 ecad2.init.A -544 1888 -676 1888 -676 1316 832 1316 832 800 756 800 estringouts.simStr.OUT
w 260 959 100 0 n#171 embbi.initMenu.FLNK 196 956 384 956 384 816 500 816 estringouts.simStr.SLNK
w 380 851 100 0 n#170 embbi.initMenu.VAL 196 764 320 764 320 848 500 848 estringouts.simStr.DOL
w -732 2155 100 0 n#159 inhier.DIR.P -764 2144 -640 2144 -640 2080 -544 2080 ecad2.init.DIR
w -94 2059 100 0 n#156 ecad2.init.MESS -224 2048 96 2048 outhier.MESS.p
s 752 1928 150 0 doInit
s -224 1896 100 0 (simulation mode)
s -732 1896 100 0 (simulation mode)
s -612 248 100 0 Note:  This record is used by dm to present a menu to the user.
s -748 1116 100 0 Simulation mode menu
[cell use]
use task 532 1819 100 0 task#189
xform 0 828 1988
use CBorder -1056 -132 -100 0 frame
xform 0 624 1172
p 2016 0 75 1536 -1 Author:Janet E. Tvedt
p 1512 4 100 1536 1 Date:24 Apr 97
p 1616 100 300 1792 -1 Dnumber:
p 1744 84 150 1536 -1 Title:initCad.sch
use ecars 1692 1736 100 0 initC
xform 0 1728 1584
p 1692 1452 65 0 1 PV:$(top)
p 1836 1452 65 0 1 def(FLNK):$(top)combCars2.VAL
p 1340 1652 65 0 1 def(ICID):$(top)apply.CLID
use simulMode 228 1623 100 0 simulMode#179
xform 0 388 1792
use estringouts 584 880 100 0 simStr
xform 0 628 816
p 580 688 100 0 1 OMSL:closed_loop
p 580 720 100 0 1 PV:$(top)
use embbi -360 1168 100 0 initMenu
xform 0 -316 748
p -284 794 100 0 1 FRST:invalid
p -284 890 100 0 1 ONST:VSM
p -448 320 100 0 1 PV:$(top)
p -284 826 100 0 1 THST:FULL
p -284 858 100 0 1 TWST:FAST
p -284 922 100 0 1 ZRST:NONE
use notes 1544 1883 100 0 notes#161
xform 0 1800 2068
p 1572 2194 100 0 -1 COMMENT1:This implements the init command.
p 1572 2162 100 0 -1 COMMENT2:
use outhier 64 2135 100 0 VAL
xform 0 80 2176
use outhier 64 2007 100 0 MESS
xform 0 80 2048
use inhier -780 2103 100 0 DIR
xform 0 -764 2144
use ecad2 -416 2132 100 0 init
xform 0 -384 1824
p -440 1624 65 0 1 FTVA:LONG
p -448 1792 100 0 0 FTVB:LONG
p -440 1580 65 0 1 PV:$(top)
p -440 1600 65 0 1 SNAM:initProc
p -912 1860 100 0 0 def(INPA):0.0
p -848 1792 65 0 1 def(INPB):$(top)observeC.VAL
p -592 1856 75 1024 -1 pproc(INPA):NPP
[comments]
