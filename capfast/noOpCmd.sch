[schematic2]
uniq 200
[tools]
[detail]
w 1666 1099 100 0 OMSS ecars.C.OMSS 1632 1088 1760 1088 1760 1024 1856 1024 outhier.OMSS.p
w 1106 1131 100 0 n#198 eseq.Toggle.LNK2 1088 1120 1184 1120 1184 1152 junction
w 1170 1163 100 0 n#198 eseq.Toggle.LNK1 1088 1152 1312 1152 ecars.C.IVAL
w 1714 1195 100 0 OVAL ecars.C.VAL 1632 1152 1664 1152 1664 1184 1824 1184 outhier.OVAL.p
w 1714 939 100 0 FLNK ecars.C.FLNK 1632 928 1856 928 outhier.FLNK.p
w 1698 1131 100 0 OCID ecars.C.CLID 1632 1120 1824 1120 bihier.OCID.p
w 66 875 100 0 n#192 ecad2.Cad.STLK -96 864 288 864 288 832 512 832 eseq.Toggle.SLNK
w -662 1323 100 0 n#191 inhier.CLID.P -704 1312 -416 1312 ecad2.Cad.ICID
w 482 1131 100 0 n#183 hwin.hwin#181.in 512 1120 512 1120 eseq.Toggle.DOL2
w 482 1163 100 0 n#182 hwin.hwin#180.in 512 1152 512 1152 eseq.Toggle.DOL1
w 18 1411 100 0 n#161 ecad2.Cad.VAL -96 1344 -32 1344 -32 1408 128 1408 outhier.VAL.p
w -14 1315 100 0 n#190 ecad2.Cad.MESS -96 1312 128 1312 outhier.MESS.p
w -622 1411 100 0 n#189 inhier.DIR.P -700 1408 -484 1408 -484 1344 -416 1344 ecad2.Cad.DIR
[cell use]
use outhier 1824 983 100 0 OMSS
xform 0 1840 1024
use outhier 1824 887 100 0 FLNK
xform 0 1840 928
use outhier 96 1367 100 0 VAL
xform 0 112 1408
use outhier 96 1271 100 0 MESS
xform 0 112 1312
use outhier 1792 1143 100 0 OVAL
xform 0 1808 1184
use bihier 1808 1079 100 0 OCID
xform 0 1824 1120
use inhier -720 1271 100 0 CLID
xform 0 -704 1312
use inhier -716 1367 100 0 DIR
xform 0 -700 1408
use hwin 320 1111 100 0 hwin#180
xform 0 416 1152
p 323 1144 100 0 -1 val(in):2
use hwin 320 1079 100 0 hwin#181
xform 0 416 1120
p 323 1112 100 0 -1 val(in):0
use ecars 1312 871 100 0 C
xform 0 1472 1040
p 1408 816 100 1024 1 name:$(top)$(cmd)$(I)
use eseq 512 743 100 0 Toggle
xform 0 800 1040
p 832 1150 100 0 1 DLY2:0.50e+00
p 624 672 100 1024 1 name:$(top)$(cmd)$(I)
p 480 1152 75 1280 -1 pproc(DOL1):NPP
p 480 1120 75 1280 -1 pproc(DOL2):NPP
p 1088 1152 75 768 -1 pproc(LNK1):PP
p 1088 1120 75 768 -1 pproc(LNK2):PP
use CBorder -900 -84 -100 0 frame
xform 0 780 1220
p 2172 48 75 1536 -1 Author:Peter Ruckle
p 1668 52 100 1536 1 Date:1-18-01
p 1772 148 300 1792 -1 Dnumber:
p 1900 132 200 1536 -1 Title:noOpCmd.sch
use ecad2 -268 1388 100 0 Cad
xform 0 -256 1088
p -328 732 100 0 1 SNAM:noopCad
p -240 848 100 0 1 name:$(top)$(cmd)$(I)
use notes 1512 1803 100 0 notes#154
xform 0 1768 1988
p 2040 1954 100 0 0 AUTHOR:AUTHOR
p 1540 2112 100 0 -1 COMMENT1:Notes: This schematic contains a CAD 
p 1540 2080 100 0 -1 COMMENT2:record which is replicated for each
p 1540 2048 100 0 -1 COMMENT3:(cmd).  The intention was to generalize
p 1540 2016 100 0 -1 COMMENT4:this so that it can be used for any
p 1540 1984 100 0 -1 COMMENT5:command that the detector may
p 1540 1952 100 0 -1 COMMENT6:legitimately reject or ignore.
p 1540 1920 100 0 -1 COMMENT7:
p 1540 1888 100 0 -1 COMMENT8:
p 1540 1856 100 0 -1 COMMENT9:
[comments]
