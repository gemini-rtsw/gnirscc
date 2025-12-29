[schematic2]
uniq 10
[tools]
[detail]
[cell use]
use lock 768 2208 100 768 lock
xform 0 992 2112
p 784 2112 100 768 1 set1:top $(top)lock
p 784 2080 100 768 1 set2:name $(name) Interlocks
use estringins 1376 2208 100 768 label
xform 0 1440 2160
p 1376 2080 100 768 1 VAL:$(name)
use sim 768 1696 100 768 sim
xform 0 992 1600
p 784 1600 100 768 1 set1:top $(top)shs
p 784 1568 100 768 1 set2:name $(name) Sim Control
use global 768 1952 100 768 global
xform 0 992 1856
p 784 1856 100 768 1 set1:top $(top)gbl
p 784 1824 100 768 1 set2:name $(name) Mech Global
use eborderC -608 231 100 0 eborderC#1
xform 0 1072 1536
p 2192 448 200 1536 -1 file:eng.sch
p 2656 352 100 1280 -1 id:$Id: eng.sch,v 1.2 2009/05/27 19:34:03 fkraemer Exp $
[comments]
