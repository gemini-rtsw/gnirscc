[schematic2]
uniq 14
[tools]
[detail]
s 1520 48 400 1280 niriWfsSadTop
s 736 2144 500 1024 Top level NIRI On-Instrument Wavefront Sensor
s 768 2016 500 1024 Status/Alarm Database schematic.
n 1536 416 2016 768 100
This is the top level schematic for the
Gemini Near Infrared Imager
On-Instrument Wavefront Sensor Status/Alarm
Database.
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
_
[cell use]
use niriWfsSad 384 1376 100 768 niriWfsSad
xform 0 560 1280
p 384 1152 100 768 1 set0:top $(top)wfs:
p 384 1120 100 768 1 set1:sadtop $(sadtop)wfs:
p 384 1088 100 768 1 set2:name $(name) WFS
p 384 1056 100 768 1 set3:pvdir ./WFS/data
p 384 1024 100 768 1 set4:eng $(realtop)eng:
use bc200tr -1024 -88 -100 0 frame
xform 0 656 1216
p 1552 80 100 0 1 author:S.M.Beard
p 1776 64 100 0 -1 border:C
p 1552 48 100 0 1 checked:H.Yamada
p 1808 64 100 0 -1 date:1999-10-21
p 1776 192 100 0 -1 project:Gemini Near Infra-Red OIWFS
p 1552 192 100 768 1 revised:H.Yamada
p 1776 32 100 768 -1 revision:1.0
p 1792 128 100 0 -1 title:Top level NIRI WFS SAD schematic
[comments]
