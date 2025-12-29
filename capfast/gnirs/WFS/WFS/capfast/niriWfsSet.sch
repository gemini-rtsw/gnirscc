[schematic2]
uniq 14
[tools]
[detail]
s 1488 80 500 512 niriWfsSet
s -1008 2160 500 0 Top level NIRI On-Instrument Wavefront Sensor schematic
n 1536 416 2016 768 100
This is the top level schematic for the
Gemini Near Infra-Red
On-Instrument Wavefront Sensor.
.
It contains no database records, but
defines the following macro variables
and includes the "cics" schematic:
.
top    = Top level record name prefix.
.
sadtop = Status/Alarm Database prefix.
.
.
.
_
[cell use]
use niriWfs 80 999 100 0 niriWfs#13
xform 0 560 1280
p 384 1152 100 768 1 set0:name $(name) WFS
p 384 864 100 768 1 set10:opDatm 6
p 384 832 100 768 1 set11:opStop 7
p 384 800 100 768 1 set12:opFollow 8
p 384 1120 100 768 1 set1:top $(top)wfs:
p 384 1088 100 768 1 set2:sadtop $(sadtop)wfs:
p 384 1056 100 768 1 set3:pvdir ./WFS/data
p 384 1024 100 768 1 set4:opSel 1
p 384 992 100 768 1 set5:opPark 2
p 384 960 100 768 1 set6:opEngMove 3
p 384 928 100 768 1 set7:opDiag 4
p 384 896 100 768 1 set8:opRdtm 5
use bc200tr -1024 -104 -100 0 frame
xform 0 656 1200
p 1552 64 100 0 1 author:S.M.Beard
p 1776 48 100 0 -1 border:C
p 1552 32 100 0 1 checked:H.Yamada
p 1808 48 100 0 -1 date:1999-10-21
p 1792 160 100 768 -1 project:Gemini Near Infra-Red OIWFS
p 1552 176 100 768 1 revised:H.Yamada
p 1776 16 100 768 -1 revision:1.0
p 1792 112 100 0 -1 title:Top level NIRI WFS schematic
[comments]
