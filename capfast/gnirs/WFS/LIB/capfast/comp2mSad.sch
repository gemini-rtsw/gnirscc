[schematic2]
uniq 448
[tools]
[detail]
w -206 931 100 0 n#447 comp1mSadRec.comp1mSadRec#350.SFLNK -192 928 -160 928 comb2mSad.comb2mSad#351.SSLNK1
w -206 579 100 0 n#446 comp1mSadRec.comp1mSadRec#350.HFLNK -192 576 -160 576 comb2mSad.comb2mSad#351.HSLNK1
w -206 643 100 0 n#445 comp1mSadRec.comp1mSadRec#350.HVAL -192 640 -160 640 comb2mSad.comb2mSad#351.HINP1
w -206 611 100 0 n#444 comp1mSadRec.comp1mSadRec#350.HOMSS -192 608 -160 608 comb2mSad.comb2mSad#351.HIMSS1
w -206 899 100 0 n#443 comp1mSadRec.comp1mSadRec#350.DVAL -192 896 -160 896 comb2mSad.comb2mSad#351.DINP1
w -206 867 100 0 n#442 comp1mSadRec.comp1mSadRec#350.DFLNK -192 864 -160 864 comb2mSad.comb2mSad#351.DSLNK1
w -206 835 100 0 n#441 comp1mSadRec.comp1mSadRec#350.PVAL -192 832 -160 832 comb2mSad.comb2mSad#351.PINP1
w -206 803 100 0 n#440 comp1mSadRec.comp1mSadRec#350.PFLNK -192 800 -160 800 comb2mSad.comb2mSad#351.PSLNK1
w -206 771 100 0 n#439 comp1mSadRec.comp1mSadRec#350.NVAL -192 768 -160 768 comb2mSad.comb2mSad#351.NINP1
w -206 739 100 0 n#438 comp1mSadRec.comp1mSadRec#350.NFLNK -192 736 -160 736 comb2mSad.comb2mSad#351.NSLNK1
w -206 707 100 0 n#437 comp1mSadRec.comp1mSadRec#350.TVAL -192 704 -160 704 comb2mSad.comb2mSad#351.TINP1
w -206 675 100 0 n#436 comp1mSadRec.comp1mSadRec#350.TFLNK -192 672 -160 672 comb2mSad.comb2mSad#351.TSLNK1
w -206 451 100 0 n#435 comp1mSadRec.comp1mSadRec#349.SFLNK -192 448 -160 448 comb2mSad.comb2mSad#351.SSLNK1
w -206 99 100 0 n#434 comp1mSadRec.comp1mSadRec#349.HFLNK -192 96 -160 96 comb2mSad.comb2mSad#351.HSLNK2
w -206 163 100 0 n#433 comp1mSadRec.comp1mSadRec#349.HVAL -192 160 -160 160 comb2mSad.comb2mSad#351.HINP2
w -206 131 100 0 n#432 comp1mSadRec.comp1mSadRec#349.HOMSS -192 128 -160 128 comb2mSad.comb2mSad#351.HIMSS2
w -206 419 100 0 n#431 comp1mSadRec.comp1mSadRec#349.DVAL -192 416 -160 416 comb2mSad.comb2mSad#351.DINP2
w -206 387 100 0 n#430 comp1mSadRec.comp1mSadRec#349.DFLNK -192 384 -160 384 comb2mSad.comb2mSad#351.DSLNK2
w -206 355 100 0 n#429 comp1mSadRec.comp1mSadRec#349.PVAL -192 352 -160 352 comb2mSad.comb2mSad#351.PINP2
w -206 323 100 0 n#428 comp1mSadRec.comp1mSadRec#349.PFLNK -192 320 -160 320 comb2mSad.comb2mSad#351.PSLNK2
w -206 291 100 0 n#427 comp1mSadRec.comp1mSadRec#349.NVAL -192 288 -160 288 comb2mSad.comb2mSad#351.NINP2
w -206 259 100 0 n#426 comp1mSadRec.comp1mSadRec#349.NFLNK -192 256 -160 256 comb2mSad.comb2mSad#351.NSLNK2
w -206 227 100 0 n#425 comp1mSadRec.comp1mSadRec#349.TVAL -192 224 -160 224 comb2mSad.comb2mSad#351.TINP2
w -206 195 100 0 n#424 comp1mSadRec.comp1mSadRec#349.TFLNK -192 192 -160 192 comb2mSad.comb2mSad#351.TSLNK2
w 328 579 -100 0 FLNK outhier.HFLNK.p 384 576 320 576 comb2mSad.comb2mSad#351.HFLNK
w 314 611 -100 0 c#362 bihier.HOMSS.p 368 608 320 608 comb2mSad.comb2mSad#351.HOMSS
w 326 643 -100 0 VAL bihier.HVAL.p 368 640 320 640 comb2mSad.comb2mSad#351.HVAL
s 1200 -1056 400 512 comp2mSad
n 32 -1072 612 -720 100
This schematic contains the Status/Alarm
Database for a CICS 2-axis discrete mechanism.
The records are updated by channel access writes
from the state notation code or other databases,
which is why there are very few links.
.
SIR records are linked together in situations
where the same quantity is stored in two or
more different forms (e.g. engineering position
can be translated automatically into name).
.
The values of all the "Health" records in the
CICS are combined into one overall instrument
health record - hence the OHLT output link.
_
[cell use]
use outhier 400 576 100 1536 HFLNK
xform 0 368 576
use bihier 400 640 100 1536 HVAL
xform 0 368 640
use bihier 400 608 100 1536 HOMSS
xform 0 368 608
use comb2mSad -64 864 100 768 comb2mSad#351
xform 0 80 512
p -64 448 100 768 1 set0:mech $(mech)
p -64 416 100 768 1 set1:desc $(desc)
use comp1mSadRec -720 912 100 768 comp1mSadRec#350
xform 0 -480 752
p -688 608 100 768 1 set0:mech $(mech1)
p -688 576 100 768 1 set1:desc $(desc1)
use comp1mSadRec -720 432 100 768 comp1mSadRec#349
xform 0 -480 272
p -688 128 100 768 1 set0:mech $(mech2)
p -688 96 100 768 1 set1:desc $(desc2)
use bc200tr -1344 -1208 -100 0 frame
xform 0 336 96
p 1232 -1056 100 768 1 author:S.M.Beard
p 1456 -1056 100 0 -1 border:C
p 1232 -1088 100 768 1 checked:Yamada
p 1488 -1056 100 0 -1 date:1999-10-19
p 1472 -928 100 0 -1 project:Near Infra-Red Imager / IR OIWFS
p 1232 -928 100 768 1 revised:Yamada
p 1456 -1088 100 0 -1 revision:1.0
p 1456 -976 100 768 -1 title:Two-axis component Status/Alarm
p 1456 -1008 100 768 -1 title2:Database
[comments]
