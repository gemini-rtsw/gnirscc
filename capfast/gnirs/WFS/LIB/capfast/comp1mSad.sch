[schematic2]
uniq 631
[tools]
[detail]
w -70 643 -100 0 c#625 comp1mSadRec.comp1mSadRec#630.HVAL -64 640 -16 640 bihier.HVAL.p
w -62 579 -100 0 c#622 comp1mSadRec.comp1mSadRec#630.HFLNK -64 576 0 576 outhier.HFLNK.p
w -70 611 -100 0 c#623 comp1mSadRec.comp1mSadRec#630.HOMSS -64 608 -16 608 bihier.HOMSS.p
s 1040 -1024 400 1280 comp1mSad
[cell use]
use comp1mSadRec -608 567 100 0 comp1mSadRec#630
xform 0 -368 752
p -576 608 100 768 1 set0:mech $(mech)
p -576 576 100 768 1 set1:desc $(desc)
use bihier 16 640 100 1536 HVAL
xform 0 -16 640
use bihier 16 608 100 1536 HOMSS
xform 0 -16 608
use outhier 16 576 100 1536 HFLNK
xform 0 -16 576
use bc200tr -1504 -1176 -100 0 frame
xform 0 176 128
p 1072 -896 100 768 1 author:S.M.Beard
p 1296 -1024 100 0 -1 border:C
p 1072 -1024 100 768 1 checked:H.Yamada
p 1328 -1024 100 0 -1 date:1999-10-19
p 1580 -1020 100 1792 -1 page:1
p 1296 -896 100 0 -1 project:Near Infra-Red Imager / IR OIWFS
p 1072 -928 100 768 1 revised:H.Yamada
p 1072 -1040 100 0 1 revision:1.0
p 1296 -944 100 0 -1 title:Interface to low-level engineering
p 1296 -976 100 768 -1 title2:status records.
[comments]
