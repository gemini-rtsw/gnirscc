.SUFFIXES:

.SUFFIXES: .c .trl .tld

TPBIN =  /home/mrippa/root/src/tptools/bin
TCC_INCLUDE = $(TPBIN)/../include
TP_INCLUDE = ../include
GLOBAL_INCLUDE = ../../include
INCLUDES = -I$(TCC_INCLUDE) -I$(TP_INCLUDE) -I$(GLOBAL_INCLUDE)

PFLAGS  =
CFLAGS = $(TFLAGS) 
COMPILE = $(TPBIN)/tcc -c $(CFLAGS) $(INCLUDES) $(PFLAGS) $(PROC) 
LINK = $(TPBIN)/tcc $(CFLAGS) $(PROC) # -o $@

.c.trl:
	$(COMPILE) $< # -o $@

COMMON_OBJS =  	../common/mem.trl ../common/control.trl ../common/debug.trl\
		../common/talk.trl ../common/var_sr.trl ../common/queue.trl\
		../common/var.trl ../common/message.trl

OPTIONAL_OBJS = ../common/proc.trl
