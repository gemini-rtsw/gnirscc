BAUD=1200
DEBUGLEVEL=0
OMEGA=TC
FD=2

XCC=/usr/local/vxworks/gnu/solaris.68k/bin/cc68k -B/usr/local/vxworks/gnu/solaris.68k/lib/gcc-lib/ -nostdinc -ansi -Wall -pedantic -m68040
XDEFINES=-DCPU=MC68040 -DCPU_FAMILY=MC680X0 -DvxWorks -DV5_vxWorks -DBAUD=${BAUD} -D${OMEGA} -DFD='(${FD})'
XINCLUDES=-I. -I.. -I.applTop/include/rec -I.applTop/include -I.applTop/epics/base/include -I.applTop/epics/extensions/include -I/usr/local/vxworks/h
XDEBUG=-O2 -DDEBUG=${DEBUGLEVEL}
DEBUG=-g -DDEBUG=${DEBUGLEVEL}

CC=gcc -Wall
DEFINES=-DBAUD=${BAUD} -D${OMEGA}

XCFLAGS=${XDEFINES} ${XINCLUDES} ${XDEBUG}

CFLAGS=${DEFINES} ${INCLUDES} ${DEBUG}

all: xy490.o serialTest.o stest otest ints.o omegaTest.o

stest.o: stest.c tests.h Makefile
	${CC} ${CFLAGS} -c stest.c

otest.o: otest.c Makefile
	${CC} ${CFLAGS} -c otest.c

serialTest.o: serialTest.c tests.h xy490.h Makefile

stest: stest.o
	${CC} ${CFLAGS} -o $@ stest.o

otest: otest.o
	${CC} ${CFLAGS} -o $@ otest.o

xy490.o: xy490.c xy490.h Makefile
	${XCC} ${XCFLAGS} -c xy490.c

ints.o: ints.c
	${XCC} ${XCFLAGS} -c ints.c

serialTest.o: serialTest.c
	${XCC} ${XCFLAGS} -c serialTest.c

omegaTest.o: omegaTest.c Makefile
	${XCC} ${XCFLAGS} -c omegaTest.c
