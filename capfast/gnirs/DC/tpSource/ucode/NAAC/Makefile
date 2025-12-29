PXX:  gnaacBits.h $(CODE).c gnaacWFire.h
	sdc -b gnaacBits.h -P -m 0x0007ffff 0xfff80555 $(CODE).c
	lpr -Plw6 ps
	enscript -2rG -dlw1 $(CODE).c
	enscript -2rG -dlw1 gnaacBits.h
	rm -f ps

GXX:  gnaacBits.h $(CODE).c gnaacWFire.h
	sdc -b gnaacBits.h -o $(CODE) $(CODE).c
	tcc -e _ns_main $(CODE).trl -m map.map -o $(CODE).tld
	rm -f *.trl

GNAII1R: gnaacBits.h gnAII_1024x1_RR.c gnaacWFire.h
	sdc -b gnaacBits.h -o gnAII_1024x1_RR gnAII_1024x1_RR.c
	tcc -e _ns_main gnAII_1024x1_RR.trl -m map.map -o gnAII_1024x1_RR.tld
	rm -f *.trl

GNAII1R1: gnaacBits.h gnAII_1024x1_RR1.c gnaacWFire.h
	sdc -b gnaacBits.h -o gnAII_1024x1_RR1 gnAII_1024x1_RR1.c
	tcc -e _ns_main gnAII_1024x1_RR1.trl -m map.map -o gnAII_1024x1_RR1.tld
	rm -f *.trl

GNAII2: gnaacBits.h gnAII_1024x1_02.c gnaacWFire.h
	sdc -b gnaacBits.h -o gnAII_1024x1_02 gnAII_1024x1_02.c
	tcc -e _ns_main gnAII_1024x1_02.trl -m map.map -o gnAII_1024x1_02.tld
	rm -f *.trl

GNAIIA2: gnaacBits.h gnAII_1024xA_02.c gnaacWFire.h
	sdc -b gnaacBits.h -o gnAII_1024xA_02 gnAII_1024xA_02.c
	tcc -e _ns_main gnAII_1024xA_02.trl -m map.map -o gnAII_1024xA_02.tld
	rm -f *.trl

GNAIIA3: gnaacBits.h gnAII_1024xA_03.c gnaacWFire.h
	sdc -b gnaacBits.h -o gnAII_1024xA_03 gnAII_1024xA_03.c
	tcc -e _ns_main gnAII_1024xA_03.trl -m map.map -o gnAII_1024xA_03.tld
	rm -f *.trl

GNAIIA3R: gnaacBits.h gnAII_1024xA_03_RR.c gnaacWFire.h
	sdc -b gnaacBits.h -o gnAII_1024xA_03_RR gnAII_1024xA_03_RR.c
	tcc -e _ns_main gnAII_1024xA_03_RR.trl -m map.map -o gnAII_1024xA_03_RR.tld
	rm -f *.trl

GNAIII: gnaacBits.h gnAIII_1024xSU01_4u.c gnaacWFire.h
	sdc -b gnaacBits.h -o gnAIII_1024xSU01_4u gnAIII_1024xSU01_4u.c
	tcc -e _ns_main gnAIII_1024xSU01_4u.trl -m map.map -o gnAIII_1024xSU01_4u.tld
	rm -f *.trl

GNTEST: gnaacBits.h gnTest.c gnaacWFire.h
	sdc -b gnaacBits.h -o gnTest gnTest.c
	tcc -e _ns_main gnTest.trl -m map.map -o gnTest.tld
	rm -f *.trl

clean:
	rm -f *.trl *.tld

