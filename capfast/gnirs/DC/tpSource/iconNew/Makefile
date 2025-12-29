PROC = 
include ../com.make

DEST = ../tld
OBJS = inst_main.trl inst_ctrl.trl inst_hka2d.trl inst_cmnds.trl \
	cmnd_funcs.trl arr_ctrl.trl inst_setvars.trl inst_rdvars.trl \
	inst_hdw.trl dev_funcs.trl $(COMMON_OBJS) 

rm = /bin/rm -f

TCC_INCLUDE = $(TPBIN)/../include
TP_INCLUDE = ../include
GLOBAL_INCLUDE = ../../include
INCLUDES = -I./ -I$(TCC_INCLUDE) -I$(TP_INCLUDE) -I$(GLOBAL_INCLUDE)

CFLAGS = -D$(INSTNAME) -D PWD=\"$(PWD)\" $(INCLUDES)

TARG=$(INSTNAME)/icon.tld

none:
	@echo "use \"make naac\""
naac:
	make ready -k INSTNAME=NAAC

ready:
	/bin/rm -f $(INSTNAME)/icon.tld
	/bin/rm -f config_st.h
	/bin/rm -f cnfg_inst.c
	/bin/rm -f arr_ctrl.c
	cp -p  $(INSTNAME)/config_$(INSTNAME).h config_st.h
	cp -p $(INSTNAME)/cnfg_$(INSTNAME).c cnfg_inst.c
	cp -p $(INSTNAME)/arr_ctrl_$(INSTNAME).c arr_ctrl.c
	make $(TARG) PWD=`pwd` INSTNAME=$(INSTNAME)

all: $(TARG)

clean:
	$(rm) *.trl *.tld *~

#install: $(TARG)
#	cp $(TARG) $(TLDDIR)

$(TARG): $(OBJS) $(INSTNAME)/cnfg_$(INSTNAME).c $(INSTNAME)/config_$(INSTNAME).h
	$(LINK) -e _ns_main -m map.map $(OBJS)
	cp -p inst_main.tld $(INSTNAME)/icon.tld
	cp -p inst_main.tld $(DEST)/$(INSTNAME)/icon.tld.new

include ../util.make

# DO NOT DELETE THIS LINE -- make depend depends on it.

arr_ctrl.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
arr_ctrl.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
arr_ctrl.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
arr_ctrl.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
arr_ctrl.trl: ../../include/irstd.h ../../include/protdefs.h
arr_ctrl.trl: ../include/protocol.h ../../include/common.h config_st.h
arr_ctrl.trl: DACmethods.h instdefs.h instvars.h instprocs.h devdefs.h
arr_ctrl.trl: ../include/prototypes.h cnfg_inst.c
cmnd_funcs.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
cmnd_funcs.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
cmnd_funcs.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
cmnd_funcs.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
cmnd_funcs.trl: ../../include/irstd.h ../../include/protdefs.h
cmnd_funcs.trl: ../include/protocol.h ../../include/common.h config_st.h
cmnd_funcs.trl: DACmethods.h instdefs.h instvars.h instprocs.h
ctio_devs.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
ctio_devs.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
ctio_devs.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
ctio_devs.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
ctio_devs.trl: ../../include/irstd.h ../../include/protdefs.h
ctio_devs.trl: ../include/protocol.h ../../include/common.h config_st.h
ctio_devs.trl: DACmethods.h instdefs.h instvars.h instprocs.h devdefs.h
ctio_devs.trl: ../../include/ierrorno.h
dev_funcs.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
dev_funcs.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
dev_funcs.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
dev_funcs.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
dev_funcs.trl: ../../include/irstd.h ../../include/protdefs.h
dev_funcs.trl: ../include/protocol.h ../../include/common.h config_st.h
dev_funcs.trl: DACmethods.h instdefs.h instvars.h instprocs.h
dev_funcs.trl: ../include/prototypes.h devdefs.h ../../include/ierrorno.h
inst_cmnds.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
inst_cmnds.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
inst_cmnds.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
inst_cmnds.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
inst_cmnds.trl: ../../include/irstd.h ../../include/protdefs.h
inst_cmnds.trl: ../include/protocol.h config_st.h DACmethods.h
inst_cmnds.trl: ../../include/common.h instdefs.h instvars.h
inst_ctrl.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
inst_ctrl.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
inst_ctrl.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
inst_ctrl.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
inst_ctrl.trl: ../../include/irstd.h ../../include/protdefs.h
inst_ctrl.trl: ../include/protocol.h ../../include/common.h
inst_ctrl.trl: ../include/prototypes.h config_st.h DACmethods.h instdefs.h
inst_ctrl.trl: instvars.h instprocs.h
inst_hdw.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
inst_hdw.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
inst_hdw.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
inst_hdw.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
inst_hdw.trl: ../../include/irstd.h ../../include/protdefs.h
inst_hdw.trl: ../include/protocol.h config_st.h DACmethods.h instdefs.h
inst_hdw.trl: instvars.h instprocs.h ../include/prototypes.h
inst_hka2d.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
inst_hka2d.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
inst_hka2d.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
inst_hka2d.trl: ../../include/irstd.h ../../include/protdefs.h
inst_hka2d.trl: ../include/protocol.h ../../include/common.h config_st.h
inst_hka2d.trl: DACmethods.h instdefs.h instvars.h instprocs.h
inst_hka2d.trl: ../include/prototypes.h
inst_main.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
inst_main.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
inst_main.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
inst_main.trl: ../../include/irstd.h ../../include/protdefs.h
inst_main.trl: ../include/protocol.h ../../include/common.h config_st.h
inst_main.trl: DACmethods.h instdefs.h devdefs.h instvars.h instprocs.h
inst_main.trl: ../include/prototypes.h
inst_rdvars.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
inst_rdvars.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
inst_rdvars.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
inst_rdvars.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
inst_rdvars.trl: /dlirim0/gn.4/tptools/bin/../include/string.h
inst_rdvars.trl: ../../include/irstd.h ../../include/protdefs.h
inst_rdvars.trl: ../include/protocol.h ../../include/common.h config_st.h
inst_rdvars.trl: DACmethods.h instdefs.h instvars.h instprocs.h devdefs.h
inst_rdvars.trl: ../../include/ierrorno.h ../include/prototypes.h
inst_setvars.trl: /dlirim0/gn.4/tptools/bin/../include/stdio.h
inst_setvars.trl: /dlirim0/gn.4/tptools/bin/../include/stdlib.h
inst_setvars.trl: /dlirim0/gn.4/tptools/bin/../include/conc.h
inst_setvars.trl: /dlirim0/gn.4/tptools/bin/../include/math.h
inst_setvars.trl: /dlirim0/gn.4/tptools/bin/../include/string.h
inst_setvars.trl: ../../include/irstd.h ../../include/protdefs.h
inst_setvars.trl: ../include/protocol.h config_st.h DACmethods.h instdefs.h
inst_setvars.trl: instvars.h instprocs.h devdefs.h ../../include/ierrorno.h
inst_setvars.trl: ../include/prototypes.h
