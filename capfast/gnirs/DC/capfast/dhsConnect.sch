[schematic2]
uniq 191
[tools]
[detail]
w 642 2123 100 0 CLID inhier.CLID.P 544 2080 608 2080 608 2112 736 2112 ecad20.dhsConnect.ICID
w 530 1963 100 0 n#188 estringouts.dhsConnectStr.OUT 352 2000 384 2000 384 1952 736 1952 ecad20.dhsConnect.A
w -30 1995 100 0 n#186 ebos.dhsConnectMenu.VAL -32 1984 32 1984 32 2048 96 2048 estringouts.dhsConnectStr.DOL
w 2 2027 100 0 n#185 ebos.dhsConnectMenu.FLNK -32 2016 96 2016 estringouts.dhsConnectStr.SLNK
w 1218 2059 100 0 MESS ecad20.dhsConnect.MESS 1056 2112 1088 2112 1088 2048 1408 2048 outhier.MESS.p
w 1218 2187 100 0 VAL ecad20.dhsConnect.VAL 1056 2144 1088 2144 1088 2176 1408 2176 outhier.VAL.p
w 612 2155 100 0 DIR inhier.DIR.P 548 2144 736 2144 ecad20.dhsConnect.DIR
s 1564 1460 150 0 doDhsConnect
[cell use]
use inhier 532 2103 100 0 DIR
xform 0 548 2144
use inhier 528 2039 100 0 CLID
xform 0 544 2080
use estringouts 96 1943 100 0 dhsConnectStr
xform 0 224 2016
p 112 2112 100 0 1 OMSL:closed_loop
p 160 2176 100 0 1 PV:$(top)
use ebos -208 2096 100 0 dhsConnectMenu
xform 0 -160 1984
p -208 2160 100 0 1 ONAM:connect
p -208 2064 100 0 1 PV:$(top)
p -208 2128 100 0 1 ZNAM:disconnect
use ecad20 832 2208 100 0 dhsConnect
xform 0 896 1312
p 832 2320 100 0 1 FTVA:LONG
p 832 2288 100 0 0 FTVB:STRING
p 832 2256 100 0 0 FTVC:STRING
p 832 1792 100 0 0 FTVD:STRING
p 848 384 100 0 1 PV:$(top)
p 800 336 100 0 1 SNAM:dhsConnectProc
p 432 1840 100 0 0 def(INPB):0.0
p 320 1616 100 0 0 def(INPD):0.0
p 464 1664 100 0 0 def(INPE):
use task 1392 1319 100 0 task#163
xform 0 1688 1488
use CBorder -1008 -92 -100 0 frame
xform 0 672 1212
p 2064 40 75 1536 -1 Author:Janet E. Tvedt
p 1560 44 100 1536 1 Date:24 Apr 97
p 1664 140 300 1792 -1 Dnumber:
p 1792 124 150 1536 -1 Title:dhsConnect.sch
use notes 1664 1895 100 0 notes#160
xform 0 1920 2080
p 1692 2206 100 0 -1 COMMENT1:This implements the dhsConnect command.
use outhier 1376 2007 100 0 MESS
xform 0 1392 2048
use outhier 1376 2135 100 0 VAL
xform 0 1392 2176
[comments]
