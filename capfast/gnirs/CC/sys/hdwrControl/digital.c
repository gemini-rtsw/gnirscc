static struct
  {
      void *v;
      char *c;
  }
sccsid =
{
    &sccsid,
        "@(#)digital.c	1.4 01/16/03"
};
void readbit(int x, int y);
#include <vxWorks.h>
/* #include <stdio.h> */
#include <fcntl.h>
#include <ioLib.h>
#include <vme.h>
#include <memLib.h>
#include <usrLib.h>             /* Debugging */
#include <cacheLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <intLib.h>
#include <logLib.h>
#include <iv.h>
#include <vxLib.h>
#include <ctype.h>
#include "stdarg.h"
#include "gnirsCC.h"
#include "timeLib.h"
#include "epicsTypes.h"

/*
 * Much of the digital I/O code is modeled on (taken from) the
 * EPICS device driver (drvXy240.c).  This includes the ability to
 * read/write multiple bits at once, which is not used in the software
 * except in initIOBits().
 */

/* If board not present, set to non-zero */
int noXYCOM = 0;

/* For one board, the following 2 (or 3) declarations are fine.  If
 * there is more than one board, these will have to be bundled in
 * a structure, and changes made throughout this file.
 */

/* Interrupt handling for the digital I/O board.  */
PFV xycomInterruptFunctions[8];

struct dio_xy240 *pdio = NULL;
SEM_ID semXYCOM;

void printPortBits(int, unsigned char);

int testXYCOM() {
    int i;
    char response[32];
    char *p;
    unsigned short *q;

    if (noXYCOM)
        return VME_OK;
    if (gnirsG.simulation)
        return VME_OK;
    if (pdio == NULL) {
        gnirsLogMessage(CICS_DB_ERROR, "Calling 'testXYCOM' before init");
        return VME_ERROR;
    }

    q = (unsigned short *)(XYCOM_BASE);
    p = response;
    for (i = 0; i < 20; i++ ) {
        *p++ = (char) *q++;
    }
    *p = 0;
    if (strncmp(response, xycomID, strlen(xycomID))) {
        gnirsLogMessage(CICS_DB_ERROR, "XYCOM ID wrong -- got <%s>", response);
        return VME_ERROR;
    }
    
    return VME_OK;
}

int initXycom() {

    if (noXYCOM) {
	pdio = (struct dio_xy240 *)NULL;
        return VME_OK;
    }

    if (semXYCOM)
        semDelete(semXYCOM);
    semXYCOM = semBCreate(SEM_Q_FIFO, SEM_FULL);
    if (semXYCOM == NULL) {
        gnirsLogMessage(CICS_DB_ERROR, "No semaphore for XYCOM");
        return VME_ERROR;
    }

    if (gnirsG.simulation) {
	pdio = (struct dio_xy240 *)NULL;
        return VME_OK;
    }

    pdio = (struct dio_xy240 *)XYCOM_BASE;
    if(hwdbg)
      printf("toggle xycom\n");
    pdio->status = XYCOM_SRESET;  /* soft reset */
    pdio->status = 0x3;   /* clear reset, turn off red light, turn on green */
    pdio->portDirection = xycomOutputs;
    /* Set up for interrupts */
    pdio->intMask = 0;
    pdio->intClear = 0xFF;
    pdio->intVector = GNIRS_XYCOM0_INT_NUM;
    pdio->status = 0x3 | XYCOM_INTR_ENABLE;
    if(hwdbg)
      printf("toggle xycom done\n");
    
    /* Don't set flag register here; see initIOBits */

    return VME_OK;
} 

void xycomIntHndlr(int dummy) {
    int i, bit;
    void (*f)();

    if (! pdio) /* shouldn't happen, but ... defensive programming */
        return;
    bit = 1;

    for (i = 0; i < 8; i++) {
        if (pdio->intPending & bit) {   /* If there's something to do */
            f = xycomInterruptFunctions[i];
            if (f) {                      /* and someone to do it */
                f();
            }
        }
        bit <<= 1;
    }
    pdio->intClear = 0xFF;  /* Clear all at once, rather than one at a time */
}
    
void xycomSetInterrupt(int which, PFV func) {
    int bit;
    unsigned char mask;

    if (!pdio)
        return;
    mask = pdio->intMask;
    bit = 1 << which;
    xycomInterruptFunctions[which] = func;
    if (func)
        pdio->intMask = mask | bit;
    else
        pdio->intMask = mask & ~bit; 
}


int initIOBits() {
    int i, rc;

    if (noXYCOM)
        return VME_OK;
    if (gnirsG.simulation)
        return VME_OK;

    for ( i = 0; i < XYCOM_NPORTS; i++ ) {
        if ( !((1 << i) & xycomOutputs) )
            continue;
        rc = xycomBoWrite(i, (unsigned char) 0xFF, xycomOutputInit[i]);
        if (rc != VME_OK)
            return rc;
    }
    return rc;
}

/* binary inputs */
int xycomBiRead(int port, unsigned char mask, unsigned char *prval) {
    register unsigned char work;

    if ( (1 << port) & xycomOutputs ) {
        gnirsLogMessage(CICS_DB_ERROR,
                "Calling BiRead on output port %d, mask %x", port, mask);
        return VME_ERROR;
    }
    if (gnirsG.simulation || noXYCOM)
        work = 0;         /* what else can we do? */
    else
        work = pdio->port[port];

    *prval = work & mask;

    return VME_OK;
}


/* binary outputs */

int xycomBoRead(int port, unsigned char mask, unsigned char * prval) {
    register unsigned char work;
              
    if ( !((1 << port) & xycomOutputs )) {
        gnirsLogMessage(CICS_DB_ERROR,
                "Calling BoRead on input port %d mask %x", port, mask);
        return VME_ERROR;
    }

    if (gnirsG.simulation || noXYCOM)
        work = 0;
    else
        if (port == FLAG_PORT)
            work = pdio->flagOutput;
        else
            work = pdio->port[port];

    *prval = work & mask;

    return VME_OK;
 }

int xycomBoWrite(int port, unsigned char mask, unsigned char val) {
    register unsigned char work;
    int rc;

    if ( !((1 << port) & xycomOutputs )) {
        gnirsLogMessage(CICS_DB_ERROR,
                "Calling BoWrite on input port %d, mask %x, val %x",
                    port, mask, val);
        return VME_ERROR;
    }
    if (gnirsG.simulation || noXYCOM) {
        return VME_OK;
    }

    rc = semTake(semXYCOM, WAIT_FOREVER);
    if (rc == ERROR) {
        gnirsLogMessage(CICS_DB_ERROR, "semTake error in xycomBoWrite");
        return VME_ERROR;
    }
    if (port == FLAG_PORT)
        work = pdio->flagOutput;
    else
        work = pdio->port[port];
    work = (work & ~mask) | (val & mask);
    if (port == FLAG_PORT)
        pdio->flagOutput = (unsigned short)work;
    else
        pdio->port[port] = (unsigned short)work;
    semGive(semXYCOM);

    return VME_OK;
 }


unsigned char readBit(ioBit iob) {
    unsigned char mask, val;
    int port;
    int rc;

    port = iob.port;
    mask = 1 << iob.bit;
    if ((1 << port) & xycomOutputs)
        rc = xycomBoRead(port, mask, &val);
    else
        rc = xycomBiRead(port, mask, &val);
    return val;
}

int isSet(ioBit iob) {
    return readBit(iob);
}

int isClear(ioBit iob) {
    return !readBit(iob);
}

int setBit(ioBit iob) {
	int status;
/* 	readbit(6,5); */
/* 	if((iob.port == 6)&&(iob.bit==5)) */
/* 	printf("\nSet port = %d, bit = %d\n",iob.port,iob.bit); */
	status = xycomBoWrite(iob.port, 1 << iob.bit, 1 << iob.bit); 
/* 	readbit(6,5); */
	return status;
}

int clearBit(ioBit iob) {
	int status;
/* 	if((iob.port == 6)&&(iob.bit==5)) */
/* 	readbit(6,5); */
/* 		printf("\nClear port = %d, bit = %d\n",iob.port,iob.bit); */
    status = xycomBoWrite(iob.port, 1 << iob.bit, 0);
/* 	readbit(6,5); */
	return status;
}
