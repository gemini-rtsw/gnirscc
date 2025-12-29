static struct
  {
      void *v;
      char *c;
  }
rcsid =
{
    &rcsid,
        "$Id: intRoutine.c,v 1.2 2009/05/27 19:32:31 fkraemer Exp $"
};
extern int intDisable;
#include <vxWorks.h>
#include <stdio.h>
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
/* #include "sdsu.h" */
/* #include "timeLib.h" */
#include <wdLib.h>
#include "gnDCADefs.h"
BOOL InterruptsAllowed = FALSE;


/* watchdog*/
extern int wdErrorTaskId;
extern WDOG_ID readoutWd;
extern int WdTime;        /* time for watchdog timer to wait for interrupt */



extern coAdSems sem;

int wdErrorHndlr(int, int, int, int, int, int, int, int, int, int);
int exposureMonitor(int, int, int, int, int, int, int, int, int, int);

void wdTimeOut(int);

int interruptInit(void)
{
 

    /* used by watchdog timer to send error message */
    sem.watchDog = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
    if(sem.watchDog == NULL)
    {
	printf("interruptInit: error in semBCreate");
	return ERROR;
    }
    /* watchdog error handling */
    wdErrorTaskId = taskSpawn("twdErrHndlr", 25, 0, 3000, wdErrorHndlr,
			      0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if(wdErrorTaskId == ERROR)
    {
	printf("interruptInit: error in taskSpawn\n");
	return ERROR;
    }
      
    sysIntDisable(COADD_INT_LEVEL);


    if (intConnect(INUM_TO_IVEC(COADD_INT_NUM),
		   coAddIntHndlr, 0) == ERROR) {
	logMessage(CICS_DB_ERROR,
		       "Unable to install data interrupt handler.");
	return ERROR;
    }
    if (intConnect(INUM_TO_IVEC(UNSCRAMBLE_INT_NUM),
		   unscrambleIntHndlr, 0) == ERROR) {
	logMessage(CICS_DB_ERROR,
		       "Unable to install data end interrupt handler.");
	return ERROR;
    }
    if (intConnect(INUM_TO_IVEC(TRANS_INT_NUM),
		   transIntHandlr, 0) == ERROR) {
	logMessage(CICS_DB_ERROR,
		       "Unable to install communications interrupt handler.");
	return ERROR;
    }
     
    /*
         * Re-enable  interrupts
         */

    if (sysIntEnable(INTERRUPT_LEVEL) == ERROR) {
	logMessage(CICS_DB_ERROR, "Unable to enable SDSU_INTERRUPT.");
	return ERROR;
    }
    logMessage(CICS_DB_MIN, "Interrupt handlers installed.\n");
      
        /* Create watchdog */
    if ((readoutWd = wdCreate()) == NULL) {
	logMessage(CICS_DB_ERROR, "Cannot create watchdog.");
	return ERROR;
    }
   

					      return OK;
}



static int wdErrorArg;

/*
 *+
 * FUNCTION NAME:
 * wdErrorHndlr
 *
 * INVOCATION:
 * wdErrorHndlr(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) a1 (int)     Unused
 * (>) a2 (int)     Unused
 * (>) a3 (int)     Unused
 * (>) a4 (int)     Unused
 * (>) a5 (int)     Unused
 * (>) a6 (int)     Unused
 * (>) a7 (int)     Unused
 * (>) a8 (int)     Unused
 * (>) a9 (int)     Unused
 * (>) a10 (int)    Unused
 *
 * FUNCTION VALUE:
 * (int) Returns OK (0) or ERROR (1) as status
 *
 * PURPOSE:
 * Spawned task to issue watchdog time-out messages
 *
 * DESCRIPTION:
 * Waits for a semaphore which signals that a watchdog timer has
 * expired.  Takes the message and sends it to the logging process.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
int wdErrorHndlr(int a1, int a2, int a3, int a4, int a5, int a6, int a7,
                 int a8, int a9, int a10)
{

    while (1)
    {
        semTake(sem.watchDog, WAIT_FOREVER);
        logMessage(CICS_DB_ERROR, "Didn't receive data.  Timeout at point %d",
		   wdErrorArg);
    }
    return OK;
}

/*
 *+
 * FUNCTION NAME:
 * wdTimeOut
 *
 * INVOCATION:
 * wdTimeOut(arg)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) arg (int)    Passed from the invocation of the watchdog start routine
 *
 * FUNCTION VALUE:
 * (void) Returns 'void' and hence has no STATUS value
 *
 * PURPOSE:
 * Handles expiration of a watchdog timer
 *
 * DESCRIPTION:
 * Sets the health to BAD, and releases the wdErrorHndlr to generate a
 * message.  This level of indirection is required because logMessage,
 * which handles all error logging, takes a semaphore, which is not
 * permitted in an interrupt context, which is where the watchdog timer
 * operates.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
void wdTimeOut(int arg)
{

   /*  sdsuG.health = BAD; */
    wdErrorArg = arg;
    semGive(sem.watchDog);
}


/*
 *+
 * FUNCTION NAME:
 * transIntHandlr
 *
 * INVOCATION:
 * transIntHandlr(dummy)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) dummy (int)
 *
 * FUNCTION VALUE:
 * (void) Returns 'void' and hence has no STATUS value
 *
 * PURPOSE:
 * Interrupt handler for controller command processing
 *
 * DESCRIPTION:
 * This interrupt-handler is used to alert any command processing
 * routine that a response has been received.  All it does is "give"
 * a "response" semaphore, so that any command process which is waiting
 * can continue to whatever it next needs to do.
 * 
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
void transIntHandlr(int dummy)
{
    /*give semaphore*/
  
    if(semGive (sem.transFrame) == ERROR)
	/*print error*/
	return;
  
}



/*
 *+
 * FUNCTION NAME:
 * coAddIntHndlr
 *
 * INVOCATION:
 * coAddIntHndlr(dummy)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) dummy (int)
 *
 * FUNCTION VALUE:
 * (void) Returns 'void' and hence has no STATUS value
 *
 * PURPOSE:
 * Interrrupt handler for data interrupts (vector 241)
 *
 * DESCRIPTION:
 * An interrupt is received when sdsuD.bpint blocks have been transmitted.
 * Depending on the state of the exposure, different actions are required.
 * 
 * If exposing, we need to note the time of the end of the exposure.
 * The MarkTime routine deals with doubles, so the floating point
 * registers must be saved and restored.
 * 
 * If the exposure is paused, the pause time serves as the end of
 * exposure time.
 * 
 * If we are reading data, each interrupt tells the data handling task
 * that more data is available.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */


void coAddIntHndlr(int dummy)
{
    /* save float registers if necessary*/
  wdCancel(readoutWd);
  /* release semaphore*/
   
  if(intDisable ==0)
    {
      if(semGive (sem.coAdFrame) == ERROR)
        /*print error*/
        ;
/*       logMessage(CICS_DB_NOLOG, "coadd int asserted.\n"); */
    }
}

/*
 *+
 * FUNCTION NAME:
 * unscrambleIntHndlr
 *
 * INVOCATION:
 * unscrambleIntHndlr(dummy)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) dummy (int)
 *
 * FUNCTION VALUE:
 * (void) Returns 'void' and hence has no STATUS value
 *
 * PURPOSE:
 * Interrrupt handler for end of data interrupts (vector 242)
 *
 * DESCRIPTION:
 * Marks the end of the exposure, recording the time and sending
 * a message to the data handling task.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
void unscrambleIntHndlr(int dummy)
{
    /* release sem*/
    if(intDisable ==0)
    {
	if(semGive (sem.unScrambleFrame) == ERROR)
	    /*print error*/
	    ; 
	if(semGive (sem.coAdFrame) == ERROR)
	    /*print error*/
	    ; 
/* 	logMessage(CICS_DB_NOLOG, "unscramble int asserted.\n"); */
    }
}
void intTest()
{
    unscrambleIntHndlr(0);
}
