PROC = 
include ../com.make


DEST = ../tld
DIRS =  NAAC
OBJS = commands.trl main.trl vars.trl $(COMMON_OBJS) $(OPTIONAL_OBJS)
CFLAGS = $(TFLAGS) -D PWD=\"$(PWD)\"
naac all: seq.tld

seq:
	make seq.tld PWD=`pwd`


seq.tld: $(OBJS)
#	$(LINK)  -e _ns_main -w 0x8000f000 $(OBJS)
	$(LINK)  -e _ns_main -m map.map $(OBJS)
	mv commands.tld seq.tld
	for x in $(DIRS); \
	do \
	    (cp -p seq.tld $(DEST)/$$x/seq.tld.new) \
	done

clean:
	rm -f $(OBJS)

install: all

include ../util.make
# DO NOT DELETE THIS LINE -- make depend depends on it.

commands.trl: /home/mrippa/root/src/tptools/bin/../include/conc.h
commands.trl: ../../include/common.h seq_defs.h seq_vars.h seqhdw.h seq.h
main.trl: /home/mrippa/root/src/tptools/bin/../include/conc.h ../../include/common.h
main.trl: seq_defs.h seq_vars.h seqhdw.h
vars.trl: /home/mrippa/root/src/tptools/bin/../include/conc.h ../include/protocol.h
vars.trl: ../../include/common.h seqhdw.h seq_vars.h
