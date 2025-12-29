[schematic2]
uniq 1231
[tools]
[detail]
w -1108 227 100 0 n#1230 comp1Car.comp1Car#1194.OVAL -1088 224 -1056 224 combCar.combCar.IVALC
w -1108 195 100 0 n#1229 comp1Car.comp1Car#1194.OCID -1088 192 -1056 192 combCar.combCar.ICIDC
w -1108 171 100 0 n#1228 comp1Car.comp1Car#1194.FLNK -1088 160 -1056 160 combCar.combCar.SLNKC
w -1108 395 100 0 n#1227 comp1Car.comp1Car#1193.FLNK -1088 384 -1056 384 combCar.combCar.SLNKB
w -1108 419 100 0 n#1226 comp1Car.comp1Car#1193.OCID -1088 416 -1056 416 combCar.combCar.ICIDB
w -1108 451 100 0 n#1225 comp1Car.comp1Car#1193.OVAL -1088 448 -1056 448 combCar.combCar.IVALB
w -1108 619 100 0 n#1224 comp2Car.comp2Car#1192.FLNK -1088 608 -1056 608 combCar.combCar.SLNKA
w -1108 643 100 0 n#1223 comp2Car.comp2Car#1192.OCID -1088 640 -1056 640 combCar.combCar.ICIDA
w -1108 675 100 0 n#1222 comp2Car.comp2Car#1192.OVAL -1088 672 -1056 672 combCar.combCar.IVALA
w -684 675 -100 0 c#1207 bihier.OVAL.p -624 672 -672 672 combCar.combCar.OVAL
w -672 643 -100 0 OCID bihier.OCID.p -624 640 -672 640 combCar.combCar.OCID
w -664 611 -100 0 FLNK outhier.FLNK.p -608 608 -672 608 combCar.combCar.FLNK
s -16 -1408 400 1280 fol
[cell use]
use bihier -592 640 100 1536 OCID
xform 0 -624 640
use bihier -592 672 100 1536 OVAL
xform 0 -624 672
use outhier -592 592 100 768 FLNK
xform 0 -624 608
use combCar -864 672 100 1024 combCar
xform 0 -864 160
use comp1Car -1856 263 100 0 comp1Car#1193
xform 0 -1472 384
p -1744 320 100 768 1 FNAM:engSt.c
p -1744 288 100 768 1 SS:eng_ss
p -1776 416 100 768 1 set0:mech $(mech3)
p -1536 416 100 768 1 set1:desc $(desc3)
use comp1Car -1856 39 100 0 comp1Car#1194
xform 0 -1472 160
p -1744 96 100 768 1 FNAM:pseudoSt.stpp
p -1744 64 100 768 1 SS:pseudo_ss
p -1776 192 100 768 1 set0:mech $(mech)Set
p -1776 160 100 768 1 set1:desc $(desc) Parameters
use comp2Car -1856 487 100 0 comp2Car#1192
xform 0 -1472 608
p -1776 640 100 768 1 set0:mech $(mech12)
p -1536 640 100 768 1 set1:desc $(desc12)
use notes 160 -777 100 0 notes#394
xform 0 416 -592
p 160 -800 100 768 0 author:S.M. Beard
p 176 -480 100 768 -1 comment0:To add new commands for this particular
p 176 -512 100 768 -1 comment1:type of component, attach more CAD
p 176 -544 100 768 -1 comment2:records to the APPLY record here, or
p 176 -576 100 768 -1 comment3:add more lower level schematics
p 176 -608 100 768 -1 comment4:connected to the APPLY record.
p 416 -448 100 1024 -1 title:ICD 14
use bc200tr -2560 -1528 -100 0 frame
xform 0 -880 -224
p 0 -1360 100 0 1 author:H.T.Yamada
p 240 -1376 100 0 -1 border:C
p 0 -1392 100 0 1 checked:H.T.Yamada
p 272 -1376 100 0 -1 date:1999-10-08
p 524 -1372 100 1792 -1 page:1
p 240 -1248 100 0 -1 project:Near Infra-Red Imager
p -2560 -1480 100 0 0 revision:1.0
p 240 -1312 100 0 -1 title:Two-Axis CAD Records
[comments]
