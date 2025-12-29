[schematic2]
uniq 230
[tools]
[detail]
w 712 747 100 0 n#229 ebis.DACsFailed.VAL 552 736 932 736 outhier.voltageSet.p
w 4 1955 100 0 {VSet} inhier.{VSet}.P -536 1952 616 1952 616 1856 552 1856 eaos.VSet.VAL
w 620 931 100 0 n#197 eaos.VggCl2.OUT 552 928 748 928 hwout.hwout#190.outp
w 620 1155 100 0 n#195 eaos.VggCl1.OUT 552 1152 748 1152 hwout.hwout#189.outp
w 618 1379 100 0 n#194 eaos.VddCl2.OUT 552 1376 744 1376 hwout.hwout#188.outp
w 618 1603 100 0 n#193 eaos.VddCl1.OUT 552 1600 744 1600 hwout.hwout#187.outp
w 616 1835 100 0 n#192 eaos.VSet.OUT 552 1824 740 1824 hwout.hwout#186.outp
w 12 1059 100 0 c#226 inhier.{VggCl2}.P -536 1056 620 1056 620 960 552 960 eaos.VggCl2.VAL
w 10 1283 100 0 n#210 inhier.{VggCl1}.P -536 1280 616 1280 616 1184 552 1184 eaos.VggCl1.VAL
w 10 1507 100 0 n#211 inhier.{VddCl2}.P -536 1504 616 1504 616 1408 552 1408 eaos.VddCl2.VAL
w 10 1731 100 0 n#212 inhier.{VddCl1}.P -536 1728 616 1728 616 1632 552 1632 eaos.VddCl1.VAL
[cell use]
use outhier 900 695 100 0 voltageSet
xform 0 916 736
use ebis 376 812 100 0 DACsFailed
xform 0 424 752
p 72 590 100 0 0 ONAM:SET
p 72 622 100 0 0 ZNAM:UNSET
use CBorder -816 -72 -100 0 frame
xform 0 864 1232
p 1752 64 100 1536 1 Date:23 Apr 97
p 1856 160 300 1792 -1 Dnumber:
p 1984 144 150 1536 -1 Title:setVoltages.sch
use inhier -552 1687 100 0 {VddCl1}
xform 0 -536 1728
use inhier -552 1463 100 0 {VddCl2}
xform 0 -536 1504
use inhier -552 1239 100 0 {VggCl1}
xform 0 -536 1280
use inhier -552 1015 100 0 {VggCl2}
xform 0 -536 1056
use inhier -552 1911 100 0 {VSet}
xform 0 -536 1952
use hwout 740 1783 100 0 hwout#186
xform 0 836 1824
p 820 1820 65 1536 -1 val(outp):@Node=2,Var=9,Grp=3,Idx=0
use hwout 744 1559 100 0 hwout#187
xform 0 840 1600
p 824 1600 65 1536 -1 val(outp):@Node=2,Var=9,Grp=6,Idx=0
use hwout 744 1335 100 0 hwout#188
xform 0 840 1376
p 832 1376 65 1536 -1 val(outp):@Node=2,Var=9,Grp=7,Idx=0
use hwout 748 1111 100 0 hwout#189
xform 0 844 1152
p 836 1152 65 1536 -1 val(outp):@Node=2,Var=9,Grp=4,Idx=0
use hwout 748 887 100 0 hwout#190
xform 0 844 928
p 836 928 65 1536 -1 val(outp):@Node=2,Var=9,Grp=5,Idx=0
use eaos 364 1928 100 0 VSet
xform 0 424 1856
p 76 2042 100 0 0 DESC:Set VSet voltage Dac
p 183 2090 100 0 0 DTYP:wFireVarMsg
p 384 1816 65 0 1 HIGH:4.0
p 384 1828 65 0 1 HIHI:4.6
p 264 1838 100 0 0 LOLO:0.0000000e+00
p 40 1710 100 0 0 PREC:4
p 364 1776 65 1536 1 PV:$(top)
p 40 1404 100 0 0 typ(DOL):path
use eaos 364 1704 100 0 VddCl1
xform 0 424 1632
p 76 1818 100 0 0 DESC:set VddCl1 voltage Dac
p 183 1866 100 0 0 DTYP:wFireVarMsg
p 376 1580 65 0 1 HIGH:4.5
p 376 1596 65 0 1 HIHI:4.5
p 40 1486 100 0 0 PREC:4
p 364 1552 65 1536 1 PV:$(top)
use eaos 364 1484 100 0 VddCl2
xform 0 424 1408
p 183 1642 100 0 0 DTYP:wFireVarMsg
p 380 1356 65 0 1 HIGH:4.5
p 380 1372 65 0 1 HIHI:4.5
p 40 1262 100 0 0 PREC:4
p 364 1328 65 1536 1 PV:$(top)
use eaos 364 1260 100 0 VggCl1
xform 0 424 1184
p 183 1418 100 0 0 DTYP:wFireVarMsg
p 380 1132 65 0 1 HIGH:5.0
p 380 1152 65 0 1 HIHI:5.0
p 40 1038 100 0 0 PREC:4
p 364 1104 65 1536 1 PV:$(top)
use eaos 364 1032 100 0 VggCl2
xform 0 424 960
p 183 1194 100 0 0 DTYP:wFireVarMsg
p 380 908 65 0 1 HIGH:5.0
p 380 928 65 0 1 HIHI:5.0
p 40 814 100 0 0 PREC:4
p 364 880 65 1536 1 PV:$(top)
[comments]
