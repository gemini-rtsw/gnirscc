/* xy240_driver.c */
/* base/src/drv $Id: drvXy240.c,v 1.2 2009/05/27 19:32:09 fkraemer Exp $ */
/*
 *	routines used to test and interface with Xycom240
 *	digital i/o module
 *
 * 	Author:      B. Kornke
 * 	Date:        11/20/91
 *	Experimental Physics and Industrial Control System (EPICS)
 *
 *	Copyright 1991, the Regents of the University of California,
 *	and the University of Chicago Board of Governors.
 *
 *	This software was produced under  U.S. Government contracts:
 *	(W-7405-ENG-36) at the Los Alamos National Laboratory,
 *	and (W-31-109-ENG-38) at Argonne National Laboratory.
 *
 *	Initial development by:
 *		The Controls and Automation Group (AT-8)
 *		Ground Test Accelerator
 *		Accelerator Technology Division
 *		Los Alamos National Laboratory
 *
 *	Co-developed with
 *		The Controls and Computing Group
 *		Accelerator Systems Division
 *		Advanced Photon Source
 *		Argonne National Laboratory
 *
 * Modification Log:
 * -----------------
 * .01	06-25-92	bg	Added driver to code.  Added xy240_io_report
 *				to it. Added copyright disclaimer.
 * .02	08-10-92	joh	merged xy240_driver.h into this source
 * .03	08-11-92	joh	fixed use of XY240 where XY240_BI or XY240_BO
 *				should have been used
 * .04  08-11-92	joh	now allows for runtime reconfiguration of
 *				the addr map
 * .05  08-25-92        mrk     added DSET; made masks a macro
 * .06  08-26-92        mrk     support epics I/O event scan
 * .07	08-26-92	joh 	task params from task params header
 * .08	08-26-92	joh 	removed STDIO task option	
 * .09	08-26-92	joh 	increased stack size for V5
 * .10	08-26-92	joh 	increased stack size for V5
 * .11	08-27-92	joh	fixed no status return from bo driver
 * .12	09-03-92	joh	fixed wrong index used when testing for card
 *				present 
 * .13	09-03-92	joh	fixed structural problems in the io
 *				report routines which caused messages to
 *				be printed even when no xy240's are present 
 * .14	09-17-92	joh	io report now tabs over detailed info
 * .15	09-18-92	joh	documentation
 * .16	08-02-93	mrk	Added call to taskwdInsert
 * .17	08-04-93	mgb	Removed V5/V4 and EPICS_V2 conditionals
        24jan96         bdg     fixed ANSI bugs for solaris
 *
 * .xx  02-22-00	rjw	emasculated for use with GNIRS...no EPICS1
 */

#include <vxWorks.h>
#include "gnirs.h"

#define masks(K) ((1<<K))

/*xy240 memory structure*/
struct dio_xy240{
    unsigned char intInputs;            /* interrupt inputs */
    unsigned char status;               /* control status register*/
    unsigned char intMask;              /* interrupt masks */
    unsigned char intClear;             /* interrupt clear */
    unsigned char intPending;           /* interrupts pending */
    unsigned char intVector;            /* interrupt service routine*/
    unsigned char flagOutput;           /* flag outputs */
    unsigned char portDirection;        /* port direction*/
    unsigned char port[8];              /* i/o ports */
};

struct dio_xy240 *pdio;


/*DIO DRIVER INIT
 *
 *initialize xy240 dig i/o card
 */
int xy240_init() {

	pdio = (struct dio_xy240 *)XYCOM_REGISTERS;

	pdio->status = 0x3;     /* turn off red light, turn on green */
        pdio->intMask = 0;
        pdio->intClear = 0xFF;
        pdio->intVector = 0;
	pdio->portDirection = XYCOM_OUTPUTS;
				
	return VME_OK;
} 	


/*
 * XY240_BI_DRIVER
 *
 *interface to binary inputs
 */

int xy240_bi_driver(int port, unsigned char mask, unsigned char *prval) {
	register unsigned char	work;

	work = pdio->port[port];
	*prval = work & mask;

	return VME_OK;
}

/*
 *
 *XY240_BO_READ
 *
 *interface to binary outputs
 */

int xy240_bo_read(int port, unsigned char mask, unsigned char *prval) {
	register unsigned char	work;
 
              
    if ( !(1 << port) & XYCOM_OUTPUTS ) {
        gnirsLogMessage(CICS_DB_ERROR, "Write to XY240 input port: %d", port);
        return VME_ERROR;
    }

    work = pdio->port[port];
                            
    *prval = work &= mask;

    return VME_OK;
 }

/* XY240_DRIVER
 *
 *interface to binary outputs
 */

int xy240_bo_driver(int port, unsigned char val, unsigned char mask) {
	register unsigned char	work;

	work = pdio->port[port];

	work = (work & ~mask) | (val & mask);

	pdio->port[port] = (unsigned short)work;

	return VME_OK;
 }


/*XY240_WRITE
 *
 *command line interface to test bo driver
 *
 */
int xy240_write(int port, unsigned char val)
 {
    return xy240_bo_driver(port,val,0xff);
 }
 

void printPortBits(int, unsigned char);

void xy240_io_report() {
    int i;
    char ports;
    unsigned char val;

    printf("XY240 Binary IN Channels:\n");
    ports = ~XYCOM_OUTPUTS;
    for ( i = 0; i < 8 ; i++ ) {
        if (ports & 1) {
            xy240_bi_driver(i,0xFF,&val);
            printPortBits(i, val);
        }
        ports >>= 1;
    }

    printf("XY240 Binary OUT Channels:\n");
    ports = XYCOM_OUTPUTS;
    for ( i = 0; i < 8 ; i++ ) {
        if (ports & 1) {
            xy240_bo_read(i,0xFF,&val);
            printPortBits(i, val);
        }
        ports >>= 1;
    }
}

void printPortBits(int port, unsigned char val) {
    int i;
    char bits[24];
    char *p;

    p = bits;
    for (i = 0; i < 8; i++ ) {
        *p++ = ' ';
        if (val & (1 << (7-i)))
            *p++ = '1';
        else
            *p++ = '0';
    }
    *p++ = '\0';

    printf("    Port %d:%s\n", port, bits);

}
