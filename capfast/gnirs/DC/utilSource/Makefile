DIRS = b014Drv ucdl wfire gemLogMsg epicstp socket vxDrvrApplic healthChk time vxddldnet

all install:
	for x in $(DIRS); \
	do \
	    (cd $$x; gmake ) \
	done
clean:
	for x in $(DIRS); \
	do \
	    (cd $$x; gmake clean) \
	done
