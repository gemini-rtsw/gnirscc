[schematic2]
uniq 979
[tools]
[detail]
w -1230 355 100 0 n#978 comp1Car.comp2.FLNK -1216 352 -1184 352 combCar.combCar.SLNKB
w -1230 387 100 0 n#977 comp1Car.comp2.OCID -1216 384 -1184 384 combCar.combCar.ICIDB
w -1230 419 100 0 n#976 comp1Car.comp2.OVAL -1216 416 -1184 416 combCar.combCar.IVALB
w -1230 579 100 0 n#975 comp1Car.comp1.FLNK -1216 576 -1184 576 combCar.combCar.SLNKA
w -1230 611 100 0 n#974 comp1Car.comp1.OCID -1216 608 -1184 608 combCar.combCar.ICIDA
w -1230 643 100 0 n#973 comp1Car.comp1.OVAL -1216 640 -1184 640 combCar.combCar.IVALA
w -806 643 -100 0 c#917 combCar.combCar.OVAL -800 640 -752 640 bihier.OVAL.p
w -792 579 -100 0 FLNK outhier.FLNK.p -736 576 -800 576 combCar.combCar.FLNK
w -806 611 -100 0 c#918 bihier.OCID.p -752 608 -800 608 combCar.combCar.OCID
s -16 -1408 400 1280 comp2Cad
[cell use]
use comp1Car -1840 640 100 1024 comp1
xform 0 -1600 576
p -1872 512 100 768 1 FNAM:engSt.stpp
p -1872 480 100 768 1 SS:eng_ss
p -1904 608 100 768 1 set0:mech $(mech1)
p -1904 352 100 768 1 set1:desc $(desc1)
use comp1Car -1840 416 100 1024 comp2
xform 0 -1600 352
p -1872 288 100 768 1 FNAM:engSt.stpp
p -1872 256 100 768 1 SS:eng_ss
p -1904 384 100 768 1 set0:mech $(mech2)
p -1904 576 100 768 1 set1:desc $(desc2)
use outhier -720 576 100 1536 FLNK
xform 0 -752 576
use bihier -672 608 100 2048 OCID
xform 0 -752 608
use bihier -672 640 100 2048 OVAL
xform 0 -752 640
use combCar -992 640 100 1024 combCar
xform 0 -992 128
use notes 160 -1193 100 0 notes#394
xform 0 416 -1008
p 160 -1216 100 768 0 author:S.M. Beard
p 176 -896 100 768 -1 comment0:To add new commands for this particular
p 176 -928 100 768 -1 comment1:type of component, attach more CAD
p 176 -960 100 768 -1 comment2:records to the APPLY record here, or
p 176 -992 100 768 -1 comment3:add more lower level schematics
p 176 -1024 100 768 -1 comment4:connected to the APPLY record.
p 416 -864 100 1024 -1 title:ICD 14
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
