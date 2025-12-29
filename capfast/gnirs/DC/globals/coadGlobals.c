static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: coadGlobals.c,v 1.2 2009/05/27 19:32:24 fkraemer Exp $"
};
#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wdLib.h>
#include "gnDCADefs.h"
#undef MAIN
#include "gnDCAVars.h"
#include "saver.h"

int saverSfd; /* socket file descriptor for sun saver     local*/
SEM_ID semTransDone;

/* data coadder globals*/
int dqDebug = 0;

/*message queue*/
MSG_Q_ID sendQ,receiveQ;

/* watchdog*/
int wdErrorTaskId;
WDOG_ID readoutWd;
int WdTime;        /* time for watchdog timer to wait for interrupt */
int intDisable = 1;
/*semaphores*/
coAdSems sem;
SEM_ID semSetupRDD;	/* semaphore to start the RDD setup task */
SEM_ID semSetupRRD;	/* semaphore to start the RRD setup tasks */
SEM_ID semSetupTEST;	/* semaphore to start the TEST setup tasks */
SEM_ID semSetupSEP;	/* semaphore to start the SEP setup tasks */

SEM_ID semSetup; /* semaphore for return from the setup tasks */

SEM_ID semRDD;
SEM_ID semRRD;
SEM_ID semTEST;
SEM_ID semSEP;
SEM_ID obsDone;
SEM_ID semDMA;
SEM_ID dcaRegLock;
SEM_ID coaddSem;


