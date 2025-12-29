[schematic2]
uniq 150
[tools]
[detail]
w 1234 1835 100 0 n#149 combSadHealth.health.FLNK 1248 1824 1280 1824 wfsSysSad.system.SLNK
w 1234 1867 100 0 n#148 combSadHealth.health.OMSS 1248 1856 1280 1856 wfsSysSad.system.IMSS
w 1234 1899 100 0 n#147 combSadHealth.health.VAL 1248 1888 1280 1888 wfsSysSad.system.INP
w 914 1675 100 0 n#146 comp1mSad.filt.HFLNK 928 1664 960 1664 combSadHealth.health.SLNKB
w 914 1707 100 0 n#145 comp1mSad.filt.HOMSS 928 1696 960 1696 combSadHealth.health.IMSSB
w 914 1739 100 0 n#144 comp1mSad.filt.HVAL 928 1728 960 1728 combSadHealth.health.INPB
w 914 1515 100 0 n#140 lockSad.lock.HFLNK 928 1504 960 1504 combSadHealth.health.SLNKC
w 914 1547 100 0 n#139 lockSad.lock.HOMSS 928 1536 960 1536 combSadHealth.health.IMSSC
w 914 1579 100 0 n#138 lockSad.lock.HVAL 928 1568 960 1568 combSadHealth.health.INPC
w 914 1835 100 0 n#137 folSad.fol.HFLNK 928 1824 960 1824 combSadHealth.health.SLNKA
w 914 1867 100 0 n#136 folSad.fol.HOMSS 928 1856 960 1856 combSadHealth.health.IMSSA
w 914 1899 100 0 n#135 folSad.fol.HVAL 928 1888 960 1888 combSadHealth.health.INPA
s 1584 0 400 1280 niriWfsSad
n 1728 624 2208 976 100
The following additional variables are
defined in this NIRI schematic:
.
desc = Full name of mechanism.
.
mech = Brief name of mechanism.
.
.
New components can be added by adding
more instances of the "component"
schematic with different values for
"mech" (e.g. "filt", "slit", "grat",
"mask", etc...).
_
n 1728 1040 2208 1392 100
This is the under top level schematic
for the Core Instrument Control
System On-Instrument Wavefront Sensor
Status/Alarm Database. On this
schematic will be placed a symbol
pointing to the Status/Alarm Databases
for each of the NIRI mechanisms.
.
There are the following symbols:
.
system = systemwide database
.
Plus a symbol for each component.
in the database.
_
[cell use]
use combSadHealth 1104 1888 100 1024 health
xform 0 1104 1536
p 960 1120 100 768 1 set0:mech sys
p 960 1088 100 768 1 set1:desc $(name) Health
use folSad 464 1888 100 1024 fol
xform 0 624 1856
p 416 1856 100 768 1 set0:mech fol
p 416 1824 100 768 1 set1:desc A&G Interface
p 416 1792 100 768 1 set2:eng $(eng)wfs:
use wfsBeamSad 320 1056 100 768 wfsBeam
xform 0 416 976
use lockSad 464 1568 100 1024 lock
xform 0 624 1536
p 416 1536 100 768 1 set0:desc Interlock Subsystem
p 416 1504 100 768 1 set1:mech lock
p 416 1472 100 768 1 set2:eng $(eng)
use comp1mSad 464 1728 100 1024 filt
xform 0 624 1696
p 416 1696 100 768 1 set0:desc Filter Wheel
p 416 1664 100 768 1 set1:mech filt
p 416 1632 100 768 1 set2:eng $(eng)wfs:
use wfsSysSad 1408 1888 100 1024 system
xform 0 1408 1760
p 1280 1568 100 768 1 set0:desc system
p 1280 1536 100 768 1 set1:mech sys
use bc200tr -960 -120 -100 0 frame
xform 0 720 1184
p 1616 48 100 0 1 author:S.M.Beard
p 1840 32 100 0 -1 border:C
p 1616 16 100 0 1 checked:H.Yamada
p 1872 32 100 0 -1 date:1999-10-21
p 1840 160 100 0 -1 project:Gemini Near Infra-Red OIWFS
p 1616 160 100 768 1 revised:H.Yamada
p 1840 0 100 768 -1 revision:1.0
p 1840 96 100 0 -1 title:Under top level NIR WFS SAD schematic
[comments]
