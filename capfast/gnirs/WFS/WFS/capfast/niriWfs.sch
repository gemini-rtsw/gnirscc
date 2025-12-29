[schematic2]
uniq 302
[tools]
[detail]
w -654 2123 100 0 n#301 eapplyx.apply.INPB -672 2112 -576 2112 lock.lock.VAL
w 178 1699 100 0 n#299 fol.fol.FLNK 192 1696 224 1696 combCar.combCar#205.SLNKC
w 178 1923 100 0 n#298 comp1m.filt.FLNK 192 1920 224 1920 combCar.combCar#205.SLNKB
w 178 2147 100 0 n#297 lock.lock.FLNK 192 2144 224 2144 combCar.combCar#205.SLNKA
w 178 1731 100 0 n#287 fol.fol.OCID 192 1728 224 1728 combCar.combCar#205.ICIDC
w 178 1763 100 0 n#286 fol.fol.OVAL 192 1760 224 1760 combCar.combCar#205.IVALC
w 594 2147 100 0 n#285 combCar.combCar#205.FLNK 608 2144 640 2144 wfsSysCar.wfsSysC.SLNK
w 594 2179 100 0 n#284 combCar.combCar#205.OCID 608 2176 640 2176 wfsSysCar.wfsSysC.ICID
w 594 2211 100 0 n#283 combCar.combCar#205.OVAL 608 2208 640 2208 wfsSysCar.wfsSysC.IVAL
w 178 1955 100 0 n#281 comp1m.filt.OCID 192 1952 224 1952 combCar.combCar#205.ICIDB
w 178 1987 100 0 n#280 comp1m.filt.OVAL 192 1984 224 1984 combCar.combCar#205.IVALB
w 178 2179 100 0 n#278 lock.lock.OCID 192 2176 224 2176 combCar.combCar#205.ICIDA
w 178 2211 100 0 n#277 lock.lock.OVAL 192 2208 224 2208 combCar.combCar#205.IVALA
w -654 1699 100 0 n#241 eapplyx.apply.OCLD -672 1696 -576 1696 fol.fol.CLID
w -654 1731 100 0 n#240 eapplyx.apply.OUTD -672 1728 -576 1728 fol.fol.DIR
w -654 1635 100 0 n#239 fol.fol.MESS -576 1632 -672 1632 eapplyx.apply.INMD
w -654 1667 100 0 n#238 fol.fol.VAL -576 1664 -672 1664 eapplyx.apply.INPD
w -654 2083 100 0 n#181 lock.lock.MESS -576 2080 -672 2080 eapplyx.apply.INMB
w -654 2147 100 0 n#179 eapplyx.apply.OCLB -672 2144 -576 2144 lock.lock.CLID
w -654 2179 100 0 n#178 eapplyx.apply.OUTB -672 2176 -576 2176 lock.lock.DIR
w -654 1923 100 0 n#137 eapplyx.apply.OCLC -672 1920 -576 1920 comp1m.filt.CLID
w -654 1955 100 0 n#136 eapplyx.apply.OUTC -672 1952 -576 1952 comp1m.filt.DIR
w -654 1859 100 0 n#123 comp1m.filt.MESS -576 1856 -672 1856 eapplyx.apply.INMC
w -654 1891 100 0 n#122 comp1m.filt.VAL -576 1888 -672 1888 eapplyx.apply.INPC
w -654 2307 100 0 n#121 wfsSystem.wfsSys.MESS -576 2304 -672 2304 eapplyx.apply.INMA
w -654 2339 100 0 n#196 wfsSystem.wfsSys.VAL -576 2336 -672 2336 eapplyx.apply.INPA
w -654 2371 100 0 n#119 eapplyx.apply.OCLA -672 2368 -576 2368 wfsSystem.wfsSys.CLID
w -654 2403 100 0 n#118 eapplyx.apply.OUTA -672 2400 -576 2400 wfsSystem.wfsSys.DIR
s 1424 224 400 1280 niriWfs
s -976 528 100 0 This "apply" record sequences the components
s -976 480 100 0 in the order A, B, C, D, E, F.
n 1632 448 2112 800 100
The following additional variables are
defined in this NIRI schematic:
.
desc = Full name of mechanism.
.
mech = Brief name of mechanism.
.
.
.
New components can be added by adding
more instances of the "component"
schematic with different values for
"mech" (e.g. "filt", "slit", "grat",
"mask", etc...).
_
n 1632 864 2112 1216 100
This is the under top level schematic
for the Near Infra-Red
On-Instrument Wavefront Sensor.
The schematic has been split arbitrarily
into two subschematics because of the
limited number of connections on the
top level APPLY record.
.
There are the following symbols:
system = systemwide database
.
.
The components are sequenced by
the top-level "apply" record.
_
[cell use]
use comp1m -432 1984 100 1024 filt
xform 0 -192 1920
p -496 1952 100 768 1 set0:mech filt
p -496 1920 100 768 1 set1:desc The Filter Wheel
use snl 128 2055 100 0 snl#261
xform 0 416 2368
p 272 2336 100 0 1 FNAM:cicsSt.stpp
p 272 2304 100 0 1 SS:sys_ss
use wfsSysCar 784 2432 100 1024 wfsSysC
xform 0 784 2256
p 720 2112 100 768 1 set0:mech sys
p 720 2080 100 768 1 set1:desc WFS system
use combCar 288 1159 100 0 combCar#205
xform 0 416 1696
p 336 1376 100 768 1 set0:mech wfs0
p 336 1344 100 768 1 set1:desc $(name)
use fol -432 1760 100 1024 fol
xform 0 -192 1696
use lock -432 2208 100 1024 lock
xform 0 -192 2144
use eapplyx -864 2576 100 0 apply
xform 0 -800 1616
p -864 640 100 768 1 DESC:$(name) top level APPLY record
p -864 608 100 768 1 PV:$(top)
use wfsSystem -432 2432 100 1024 wfsSys
xform 0 -192 2368
p -496 2400 100 768 1 set0:mech sys
p -496 2368 100 768 1 set1:desc WFS system
use bc200tr -1120 104 -100 0 frame
xform 0 560 1408
p 1456 272 100 0 1 author:S.M.Beard
p 1680 256 100 0 -1 border:C
p 1456 224 100 768 1 checked:H.Yamada
p 1712 256 100 0 -1 date:1999-10-21
p 1696 384 100 0 -1 project:Gemini Near Infra-Red OIWFS
p 1456 384 100 768 1 revised:H.Yamada
p 1680 224 100 768 -1 revision:1.0
p 1696 320 100 0 -1 title:Under top level NIRI WFS schematic
[comments]
