[schematic2]
uniq 208
[tools]
[detail]
w 610 1227 100 0 n#199 ebis.dwnLdState.VAL 448 1216 832 1216 outhier.downLoaded.p
[cell use]
use eais 1216 1656 100 0 minInt
xform 0 1280 1600
p 896 1342 100 0 0 EGU:Secs
p 1120 1566 100 0 0 LOLO:0.0000000e+00
p 1152 1632 100 0 0 LOW:0.056
p 896 1438 100 0 0 PREC:3
p 1232 1576 65 1536 1 PV:$(top)
use eais 1216 1816 100 0 minRead
xform 0 1280 1760
p 896 1502 100 0 0 EGU:Secs
p 1120 1726 100 0 0 LOLO:0.0000000e+00
p 1152 1792 100 0 0 LOW:0.056
p 896 1598 100 0 0 PREC:3
p 1232 1736 65 1536 1 PV:$(top)
use eais 1216 1976 100 0 eais#205
xform 0 1280 1920
p 896 1662 100 0 0 EGU:Secs
p 1120 1886 100 0 0 LOLO:0.0000000e+00
p 1152 1952 100 0 0 LOW:0.056
p 896 1758 100 0 0 PREC:3
p 1232 1896 65 1536 1 PV:$(top)
use eais 256 2296 100 0 ucMinInt
xform 0 320 2240
p -64 1982 100 0 0 EGU:Seconds
p 160 2206 100 0 0 LOLO:0.0000000e+00
p 192 2272 100 0 0 LOW:0.056
p -64 2078 100 0 0 PREC:3
p 272 2216 65 1536 1 PV:$(top)
use eais 256 2152 100 0 ucMinRead
xform 0 320 2096
p -64 1838 100 0 0 EGU:Secs
p 160 2062 100 0 0 LOLO:0.0000000e+00
p 192 2128 100 0 0 LOW:0.056
p -64 1934 100 0 0 PREC:3
p 272 2072 65 1536 1 PV:$(top)
use eais 256 1992 100 0 ucMinDly
xform 0 320 1936
p -64 1678 100 0 0 EGU:Secs
p 160 1902 100 0 0 LOLO:0.0000000e+00
p 192 1968 100 0 0 LOW:0.056
p -64 1774 100 0 0 PREC:3
p 272 1912 65 1536 1 PV:$(top)
use eais 256 1832 100 0 ucDAvgsDly
xform 0 320 1776
p -64 1518 100 0 0 EGU:uSecs
p 160 1742 100 0 0 LOLO:0.0000000e+00
p 192 1808 100 0 0 LOW:0.056
p -64 1614 100 0 0 PREC:3
p 272 1752 65 1536 1 PV:$(top)
use eais 800 2304 100 0 maxPhotonTime
xform 0 864 2240
p 480 1982 100 0 0 EGU:Secs
p 480 2078 100 0 0 PREC:3
use eais 800 2152 100 0 ucFirstDly
xform 0 864 2096
p 480 1838 100 0 0 EGU:Secs
p 704 2062 100 0 0 LOLO:0.0000000e+00
p 736 2128 100 0 0 LOW:0.056
p 480 1934 100 0 0 PREC:3
p 816 2072 65 1536 1 PV:$(top)
use ebis 264 1288 100 0 dwnLdState
xform 0 320 1232
p -32 1070 100 0 0 ONAM:Downloaded
p 264 1208 65 1536 1 PV:$(top)
p -32 1102 100 0 0 ZNAM:Not_Downloaded
p 160 1264 75 1280 -1 pproc(INP):NPP
use ebis 264 1120 100 0 dwnLdFailed
xform 0 320 1064
p -32 902 100 0 0 ONAM:Download OK
p 264 1040 65 1536 1 PV:$(top)
p -32 934 100 0 0 ZNAM:DownLoad Failed
p 160 1096 75 1280 -1 pproc(INP):NPP
use CBorder -1020 -68 -100 0 frame
xform 0 660 1236
p 1548 68 100 1536 1 Date:23 Apr 97
p 1652 164 300 1792 -1 Dnumber:
p 1780 148 150 1536 -1 Title:uCodeDwnLd.sch
use notes -480 1543 100 0 notes#200
xform 0 -224 1728
p -452 1854 100 0 -1 COMMENT1:Routine: uCodeDwnLd.
p -464 1664 65 0 -1 COMMENT10:     naac:dc:tldFile and naac:dc:cmdFile
p -452 1822 65 1536 -1 COMMENT2:Purpose: handles the downloading of the sequencer microcode
p -456 1808 65 1536 -1 COMMENT3:        and the setting of variables which depend on the uCode.
p -448 1776 65 0 -1 COMMENT4:Called by: doArSetup task.
p -452 1744 65 0 -1 COMMENT5:Parameters:
p -456 1728 65 0 -1 COMMENT6:    tldFile - char [1024] - the full file name of the uCode
p -452 1712 65 0 -1 COMMENT7:    cmdFile - char [1024] - the full file name of the uCode
p -464 1696 65 0 -1 COMMENT8:              command file.
p -456 1680 65 0 -1 COMMENT9:    both parameters are store in vxWorks Variables named,
use outhier 752 1176 100 0 downLoaded
xform 0 816 1216
use elongouts 264 1480 100 0 ucFrmsPCycle
xform 0 320 1408
p 148 1800 100 0 0 DTYP:Soft Channel
p 256 1550 100 0 0 HHSV:NO_ALARM
p 272 1364 65 0 1 PV:$(top)
p 264 1320 100 0 1 TPRO:1
use elongouts 264 1672 100 0 uCodeType
xform 0 320 1600
p 148 1992 100 0 0 DTYP:Soft Channel
p 256 1742 100 0 0 HHSV:NO_ALARM
p 520 1672 65 0 1 HIGH:4
p 520 1688 65 0 1 HIHI:4
p 520 1640 65 0 1 LOLO:1
p 520 1656 65 0 1 LOW:1
p 272 1556 65 0 1 PV:$(top)
[comments]
