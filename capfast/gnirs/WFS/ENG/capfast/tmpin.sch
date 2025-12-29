[schematic2]
uniq 228
[tools]
[detail]
w -1702 1819 100 0 n#227 inhier.inp0.P -1744 1808 -1600 1808 eais.atod0.INP
w -1198 1955 100 0 n#222 eais.atod0.VAL -1344 1760 -1280 1760 -1280 1952 -1056 1952 esubs.tmpin.INPA
[cell use]
use inhier -1760 1808 100 2048 inp0
xform 0 -1744 1808
use eborderC 640 1120 100 1280 $Id:
xform 0 -944 2304
p 176 1216 200 1536 -1 file:tmpin.sch
p 640 1120 100 1280 -1 id:$Id: tmpin.sch,v 1.2 2009/05/27 19:34:05 fkraemer Exp $
use esubs -992 1984 100 768 tmpin
xform 0 -912 1744
p -992 1440 100 768 1 INAM:tmpInit
p -992 1376 100 768 1 PREC:1
p -992 1408 100 768 1 SCAN:1 second
p -992 1472 100 768 1 SNAM:tmpProcess
use eais -1536 1840 100 0 atod0
xform 0 -1472 1776
p -1536 1696 100 768 1 DTYP:$(atod)
p -1376 1600 100 768 1 EGUF:5.0
p -1536 1600 100 768 1 EGUL:-5.0
p -1376 1632 100 768 1 HOPR:1.5
p -1536 1632 100 768 1 LOPR:0.0
p -1536 1664 100 768 1 PREC:3
p -1376 1664 100 768 1 SCAN:.1 second
p -1536 1568 100 768 1 SMOO:0.99
[comments]
