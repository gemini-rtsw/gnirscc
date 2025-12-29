[schematic2]
uniq 227
[tools]
[detail]
w -1374 1931 100 0 n#226 hwout.hwout#225.outp -1344 1920 -1344 1920 ebos.out.OUT
[cell use]
use hwout -1344 1879 100 0 hwout#225
xform 0 -1248 1920
p -1248 1911 100 0 -1 val(outp):$(hwout)
use ebos -1536 2016 100 768 out
xform 0 -1472 1952
p -1536 1856 100 768 1 DTYP:XYCOM-240
use bc200tr -2032 744 -100 0 frame
xform 0 -352 2048
[comments]
