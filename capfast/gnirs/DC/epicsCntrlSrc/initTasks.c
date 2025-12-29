 static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: initTasks.c,v 1.4 2010/08/16 19:57:41 mrippa Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * initTasks.c
 *
 * DESCRIPTION 
 * This file contains the functions used for spawning VxWorks tasks.  It also
 * contains the VxWorks control tasks for the three individual top level commands.
 * 
 * FUNCTION NAME(S)
 * initTasks - starts up all the control tasks
 * initTask  - spawns a single task
 * doArSetup - control task for the arSetup command
 * doObsSetup- control task for the obsSetup command
 * doDrRoiSet- control task for the drRoiSet command
 *   
 * DEPENDENCIES
 * initTasks must be called from the IOC startup script to ensure that
 * all the tasks have been spawned and are waiting to be activated.
 *
 *INDENT-OFF*
 * $Log: initTasks.c,v $
 * Revision 1.4  2010/08/16 19:57:41  mrippa
 * Removed delay after bias adjustment
 *
 * Revision 1.3  2010/07/08 21:26:17  mrippa
 * Bias delay in place for testing. We'll sleep 3.1 seconds here
 * allowing HK time to scan again. Sleeping here should not
 * block HK scan.
 *
 * Revision 1.2  2009/05/27 19:32:21  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:50  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:13:48  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:27  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */

/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>

/* NAAC specific include files */
#include <epCommon.h>
#include <naacTasks.h>

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>

#include <gnerrno.h>
#include "gnDCADefs.h"
#include  "saver.h"
#include <subRecord.h>
/* #define MAX_ACTIVATION_TEMP 77 */
#define MAX_ACTIVATION_TEMP 70
long activationFlag = 0; /*when one, user disables temperature deactivation.*/
extern char *dbTop;
extern char *dbSadTop;
extern saverParams svrP;
extern SEM_ID semInitWcs;
char tmp[80];
/* Forward declarations of the functions in this file */
int doArSetup( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
int doObsSetup( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
int doDrRoiSet( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		  int n8, int n9, int n10);
int initTask( SEM_ID *semID, SEM_B_STATE semState, char *recTop, char *recName,
               int priority, int options, int stack, FUNCPTR func);
long uCodeDwnLd(char* tldFile, char* cmdFile, struct cadRecord* cadPtr, 
			 long *errNo, char* errText);
long setVoltages( struct cadRecord* cadPtr, long *errNo, char* errText );
long setSeqROI( struct cadRecord* cadPtr, long *errNo, char* errText,long numDAvgs,long numLNRs);
long setSeqVars(struct cadRecord* cadPtr, long *errNo, char* errText);
long setHdrVars(struct cadRecord* cadPtr, long *errNo, char* errText);
long setObsState(struct cadRecord* cadPtr, long *errNo, char* errText);
long setIntTime(struct cadRecord* cadPtr, long *errNo, char* errText);
long setBias(struct cadRecord* cadPtr, long *errNo, char* errText);
long setDrRoi(struct cadRecord* cadPtr, long *errNo, char* errText);
long chkActiv(struct cadRecord* cadPtr, long *errNo, char* errText);
double calcMinInt(long arSizeVal, long DAvgs, long Lnrs);
long ldWaveFormGen(char *sPath, char *sName, char *cPath);
long parseCmd(char *cmdFile, struct cadRecord *cadPtr );
int setHWVolts(char* target, char* check, struct cadRecord* errRef, double value);
int setHWVoltsT(char *top,char* target, char* check, struct cadRecord* errRef, double value);
int initWcsCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
			int n9, int n10 );
long setArrayActivation(long detState);

/*
 *+
 * FUNCTION NAME:
 * initTasks
 *
 * INVOCATION:
 * initTasks();
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None
 *
 * FUNCTION VALUE:
 * None
 *
 * PURPOSE:
 * To startup each of the control tasks with appropriate parameters.
 *
 * DESCRIPTION:
 * This function should be called from the IOC startup script.  It calls
 * initTask for every control task that needs to be spawned.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed that this function is invoked from the VxWorkd startup script
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 * 17-Mar-1997  Original version.				Janet Tvedt
 * 10-Oct-1997  Minor changes to activChk		Ken Ramey
 *
 *-
 */
void initTasks(void)
{
    semDcaReady = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
    semFrameReady = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
   
   initTask(&semArSetup,SEM_EMPTY,dbTop,ARSETUP_CAD,50,VX_FP_TASK,10000,doArSetup);
   initTask(&semObsSetup,SEM_EMPTY,dbTop,OBSSETUP_CAD,50,VX_FP_TASK,4000,doObsSetup);
   initTask(&semDrRoiSet,SEM_EMPTY,dbTop,DRROISET_CAD,50,VX_FP_TASK,4000,doDrRoiSet);

   initTask(&semTest,SEM_EMPTY,dbTop,TEST_CAD,60,VX_FP_TASK,4000,doTest);
   initTask(&semInit,SEM_EMPTY,dbTop,INIT_CAD,60,VX_FP_TASK,4000,doInit);
   initTask(&semInitWcs,SEM_EMPTY,dbTop,SETWCS_CAD,60,VX_FP_TASK,10000,initWcsCtrl);
   initTask(&semSetDhsInfo,SEM_EMPTY,dbTop,SETDHSINFO_CAD,60,VX_FP_TASK,4000,doSetDhsInfo);
   initTask(&semDhsConnect,SEM_EMPTY,dbTop,DHSCONNECT_CAD,60,VX_FP_TASK,4000,doDhsConnect);

   initTask(&semObserve,SEM_EMPTY,dbTop,OBSERVE_CAD,60,VX_FP_TASK,10000,doObserve); 
   initTask(&semPark,SEM_EMPTY,dbTop,PARK_CAD,60,VX_FP_TASK,6000,doPark);
   initTask(&semReboot,SEM_EMPTY,dbTop,REBOOT_CAD,60,VX_FP_TASK,6000,doReboot);
   initTask(&semAbort,SEM_EMPTY,dbTop,ABORT_CAD,60,VX_FP_TASK,10000,doAbort);
   initTask(&semStop,SEM_EMPTY,dbTop,STOP_CAD,60,VX_FP_TASK,10000,doStop);
  
}


/*
 *+
 * FUNCTION NAME:
 * initTask
 *
 * INVOCATION:
 * SEM_ID *semId;
 * SEM_B_STATE semState;
 * char *recTop;
 * char *recName;
 * int priority;
 * int options;
 * int stack;
 * FUNCPTR func;
 * int status;
 *
 * status = initTask( semId, semState, recTop, recName, priority, options, stack, func)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  semId	(SEM_ID)      pointer to the semaphore ID
 *  semState	(SEM_B_STATE) the creation state of the binary semaphor
 *  recTop	(char *)      pointer to the db prefix
 *  recName	(char *)      pointer to the CAD record name associated with the task
 *  priority    (int)	      priority to assign to the task
 *  options	(int)	      options to give to send to taskSpawn
 *  stack       (int)	      stack size to allow for the task
 *  func	(FUNCPTR)     pointer to the starting address of the task
 *
 * FUNCTION VALUE:
 * int	- the ID of the created task
 *
 * PURPOSE:
 * To spawn a VxWorks control task
 *
 * DESCRIPTION:
 * This function creates a binary semaphore for the task, determines the address
 * of its associated CAD record.  The CAD address is passed as the first (and
 * only used) paramater of the control task.  This function then spawns the
 * task using the supplied input parameters.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY (optional):
 * 17-Mar-1997  Original version.			Janet Tvedt
 *
 *-
 */

int initTask( SEM_ID *semId, SEM_B_STATE semState, char *recTop, char *recName,
	      int priority, int options, int stack, FUNCPTR func)
{
    char taskName[MAX_STRING_SIZE];
    char dbRec[MAX_STRING_SIZE];
    struct dbAddr addr;
    long ret;
    int taskId;
    int iAddr;
    
    /* Create a semaphore for the task */
    *semId = semBCreate(SEM_Q_FIFO,semState);    

    /* Get the address of the data structure containing command arguments */
    sprintf(dbRec,"%s%s.OUTA",recTop,recName);
    ret=dbNameToAddr (dbRec,&addr);
    iAddr = (int) addr.precord;

    /* If successful, then create a task name and spawn it */
    if(ret == 0)
    {
	sprintf(taskName,"%c%s",'t',recName);
	taskId = taskSpawn(taskName,priority,options,stack,func,iAddr,0,0,0,0,0,0,0,0,0);
    }

    /* Otherwise, print an error message and return -1 */
    else
    {
	cicsLogLong(0, "failed with dbNameToAddr error = %ld", ret);
	cicsLogString(0, "dbName = ", dbRec);
	taskId = -1;
    }

    return taskId;
    
}

int doArSetup( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		int n8, int n9, int n10)
{
	long dlCount;
	struct cadRecord *pCad;
	short newCode, reDoDrRoiSet=FALSE;
	long status = 0;
	long error;
	long lVal;
	int forceDwnLdVal;
	long numDAvg,numLNRs,arSize;
	int i;
	long binFalse = (long)FALSE;
	long binTrue = (long)TRUE;

	unsigned short usVal;
	char errMess[MAX_STRING_SIZE];
	char dummy[MAX_STRING_SIZE];

	/* Sleep until there is something to do */
	while( 1 )
	{
		cicsLogMessage(DBG_MIN,"task tDoArSetup sleeping ...");
		semTake(semArSetup,WAIT_FOREVER); 
		cicsLogMessage(DBG_MIN, "task tDoArSetup awake");

		/* Get the aaddress of the CAD record so command arguments
			are accessible */	
		pCad = (struct cadRecord *) n1;
		printCadVals(3, pCad, 9);
		/* set Car to Busy */
		setCar(ARSETUP_CAR,CAR_BUSY,OK,"",dummy);

		cicsLogMessage(DBG_MIN, "task tDoArSetup awake 1");
		if ((status = putDbInfoT(dbTop, HKFREEZE ".VAL", dummy, DBF_LONG,
						&binTrue)) != OK)
		{
			cicsLogMessage( 3, "Could not freeze Housekeeping readback");
			continue;
		}
		cicsLogMessage(DBG_MIN, "task tDoArSetup awake 2");

		/*Dowload ucode if a different one is specified or if download forced*/
		status = getDbInfoT(dbTop, ARSETUP_CAD ".VALJ", pCad->mess,
				DBF_LONG, &forceDwnLdVal);
		newCode = 0;
		printf("new = %s %s, old = %s %s",(char *)pCad->vala,(char *)pCad->valb,(char *)pCad->olda,(char *)pCad->oldb);
		if ((strcmp(pCad->vala, pCad->olda) != 0) ||
				(strcmp(pCad->valb, pCad->oldb) != 0) || 
				(forceDwnLdVal != 0)) 
		{
			newCode = TRUE;
			reDoDrRoiSet=TRUE;	    
			putDbInfoT(dbTop, DWNLD_STATE ".VAL", dummy, DBF_LONG, &binFalse);
			/* 	    sleep(1,0); */
			cicsLogMessage( DBG_MIN, "Downloading uCode");
			status = uCodeDwnLd(pCad->vala, pCad->valb, pCad, &error, errMess);
			/* 	    sleep(1,0); */
			/* EEE Handle error and correct continue on error code here */
		}

		cicsLogMessage(DBG_MIN, "task tDoArSetup awake 3");
		cicsLogMessage(DBG_MIN,"unfreezing hk");
		putDbInfoT(dbTop, HKFREEZE ".VAL", dummy, DBF_LONG, &binFalse);
		/* 	sleep(1,0); */
		/* Invoke setVoltages */
		if (status == OK)
		{
			cicsLogMessage( 2, "Setting voltages");
			status = setVoltages(pCad, &error, errMess);
		}

		/* Invoke setBias */
		if (status == OK)
		{
			cicsLogMessage( 2, "Setting array bias");
			status = setBias(pCad, &error, errMess);
		}

		cicsLogMessage(DBG_MIN, "task tDoArSetup awake 4");

		/* If status is OK and new ucode was in fact downloaded, then set the status
			of obsSetup and drRoiSet to UNKNOWN */
		if ((status == OK) && newCode)
		{
			/* NOTE 1 should check if the OBS done record is set to busy 
				if so there is no need to set this to unknown, the OBS setup
				task is waiting for us to finish and will imediately reset it
				anyway */

			getDbInfoT(dbSadTop, DL_COUNT ".VAL", dummy, DBF_LONG, &dlCount);
			dlCount ++;
			putDbInfoT(dbSadTop, DL_COUNT ".VAL", dummy, DBF_LONG, &dlCount);
			usVal = NAAC_UNKNOWN;
			putDbInfoT(dbTop, OBSSETUP_DONE ".VAL", dummy, DBF_ENUM, &usVal);
			/* see NOTE 1 the same thing applies to the DRROISET DONE flag */
			if (reDoDrRoiSet == TRUE)
			{
				usVal = NAAC_UNKNOWN;
				putDbInfoT(dbTop, DRROISET_DONE ".VAL", dummy, DBF_ENUM, &usVal);
			}
		}

		cicsLogMessage(DBG_MIN, "task tDoArSetup awake 5");

		/* If status is OK, set CAR to IDLE and DONE record to DONE */
		if (status == OK)
		{
			fprintf(stderr,"****OK in setup**** \n");
			setCar(ARSETUP_CAR,CAR_IDLE,OK,"",dummy);
			usVal = NAAC_DONE;
			putDbInfoT(dbTop, ARSETUP_DONE ".VAL", dummy, DBF_ENUM, &usVal);

			/*	
			Mark obsSetup CAD record /for processing.
			*/
			lVal = CAD_MARK;
			putDbInfoT(dbTop, OBSSETUP_CAD ".DIR", dummy, DBF_LONG, &lVal);
		}
		else  /* Otherwise, set CAR and DONE records to ERROR.  set error code
					and error message of the CAR record */
		{
			sprintf(tmp,"****error in setup**** %s\n", errMess);
			cicsLogMessage(DBG_NOLOG,tmp);
			setCar(ARSETUP_CAR,CAR_ERROR,error,errMess,dummy);
			usVal = NAAC_ERROR;
			putDbInfoT(dbTop, ARSETUP_DONE ".VAL", dummy, DBF_ENUM, &usVal);
		}	
		getDbInfoT(dbTop,OBSSETUP_CAD ".A",dummy,DBF_LONG,&arSize);
		getDbInfoT(dbTop,OBSSETUP_CAD ".C",dummy,DBF_LONG,&numDAvg);
		getDbInfoT(dbTop,OBSSETUP_CAD ".D",dummy,DBF_LONG,&numLNRs);
		if((numDAvg <=0)||(!pow2(numDAvg,&i)))
			numDAvg = 1;
		if((numLNRs <=0)||(!pow2(numLNRs,&i)))
			numLNRs = 1;
		if((arSize != 1024)&&(arSize != 512)&&(arSize != 256)&&(arSize != 768))
			arSize = 1024;
		calcMinInt(arSize,numDAvg,numLNRs);
		printf("calcminint\n");
	}

	return status;
}


int doObsSetup( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		int n8, int n9, int n10)
{
	static int old;
	struct cadRecord *pCad;
	short newSeqRoiSize=FALSE;
	long status = 0;
	long error;
	long lVal,numDAvgs,numLNRs,arSize;
	unsigned short usVal;
	char dummy[MAX_STRING_SIZE];
	char errMess[MAX_STRING_SIZE];
	char name[MAX_STRING_SIZE];	

	/* Sleep until there is something to do */
	while( 1 )
	{
		status = OK;/*8-16-01 pbr*/
		cicsLogMessage(DBG_MIN,"task tDoObsSetup sleeping ...");
		/* Wait for semaphore and completion of the arSetup task */
		semTake(semObsSetup,WAIT_FOREVER);
		/* EEE we should make some provision to bomb out of here if It is not
			set to DONE for some timeout period maybe one minute? */
		while( naacStatus(dbTop,ARSETUP_DONE) != NAAC_DONE)
			taskDelay(sysClkRateGet()/10);
		cicsLogMessage(DBG_MIN, "task tDoObsSetup awake");

		/* Set the CAR reocrd to BUSY */
		setCar(OBSSETUP_CAR,CAR_BUSY,OK,"",dummy);

		/* Get the address of CAD record so command arguments are accessible */
		pCad = (struct cadRecord *) n1;
		printCadVals(3,pCad,15);

		/* Set the sequencer ROI size if a different one is specified */
		newSeqRoiSize = FALSE; 
		/*   printf("setup status = %d\n",status); */

		/*  	if ((*((long *)pCad->vala)) != (*((long *)pCad->olda)))  */
		if (getDbInfoT(dbTop, OBSSETUP_CAD ".VALC", dummy, DBF_LONG, 
					&lVal) != OK)
		{
			sprintf(errMess, "Error getting numDavgs value");
			error = NDAVGS_ERROR;
		}
		else
			if (putDbInfoT(dbTop, NUM_DAVGS ".VAL", dummy, DBF_LONG, &lVal) != OK)
			{
				sprintf(errMess, "Error setting numDavgs value");
				error = NDAVGS_ERROR;
			}
		numDAvgs = lVal;
		/*  printf("setup status = %d\n",status); */
		if (getDbInfoT(dbTop, OBSSETUP_CAD ".VALD", dummy, DBF_LONG,
					&lVal) != OK)
		{
			sprintf(errMess, "Error getting LNR value");
			error = LNR_ERROR;
		}
		else
			if (putDbInfoT(dbTop, NUM_LNRS ".VAL", dummy, DBF_LONG, &lVal) != OK)
			{
				sprintf(errMess, "Error setting LNR value");
				error = LNR_ERROR;
			}
		numLNRs = lVal;
		/*  printf("setup status = %d\n",status); */
		if ((*((long *)pCad->vala)) != old) 
		{ 
			old = *((long *)pCad->vala);
			newSeqRoiSize = TRUE;
			cicsLogMessage( 2, "Setting seqRoiSize");
			status += setSeqROI(pCad, &error, errMess,numDAvgs,numLNRs);
			cicsLogMessage( 2, "seqRoiSize is set");
		}

		/*  printf("setup status = %d\n",status); */
		if (getDbInfoT(dbTop, OBSSETUP_CAD ".VALE", dummy, DBF_LONG, 
					&lVal) != OK)
		{
			sprintf(errMess, "Error getting numCoadds");
			error = COADD_ERROR;
		}
		else
			if (putDbInfoT(dbTop, NUM_COADDS ".VAL", dummy, DBF_LONG, &lVal) != OK)
			{
				sprintf(errMess, "Error setting numCoadds");
				error = COADD_ERROR;
			}

		/*   printf("setup status = %d\n",status); */


		if (getDbInfoT(dbTop, OBSSETUP_CAD ".VALF", dummy, DBF_LONG, 
					&lVal) != OK)
		{
			sprintf(errMess, "Error getting numPics value");
			error = NPICS_ERROR;
		}
		else
			if (putDbInfoT(dbTop, NUM_PICS ".VAL", dummy, DBF_LONG, &lVal) != OK)
			{
				sprintf(errMess, "Error setting numPics value");
				error = NPICS_ERROR;
			}
		/*  printf("setup status = %d\n",status); */

		cicsLogMessage( 2, "Checking array activation");
		status += chkActiv(pCad, &error, errMess);
		cicsLogMessage( 2, "Array activation complete");


		/*   printf("setup status = %d\n",status); */
		cicsLogMessage( 2, "Setting int time");
		status += setIntTime(pCad, &error, errMess);
		cicsLogMessage( 2, "int time set");

		/*    printf("setup status = %d\n",status); */
		cicsLogMessage( 2, "Setting header vars");
		status += setHdrVars(pCad, &error, errMess);
		cicsLogMessage( 2, "header vars set");

		/*  printf("setup status = %d\n",status); */
		cicsLogMessage( 2, "Setting obs state");
		status += setObsState(pCad, &error, errMess);
		cicsLogMessage( 2, "obs state set");

		/*  printf("setup status = %d\n",status); */
		arSize =*(long *) pCad->vala;
		calcMinInt(arSize, numDAvgs, numLNRs);
		/* If status is OK and new seqRoiSize was in fact set, then
			set the status of drRoiSet to UNKNOWN EEE see NOTE 1 above the
			same applies here */
		/*   printf("setup status = %d\n",status); */

		if((status == OK) && newSeqRoiSize)
		{
			usVal = NAAC_UNKNOWN;
			putDbInfoT(dbTop, DRROISET_DONE ".VAL", dummy, DBF_ENUM, &usVal);
		}

		/*   printf("setup status = %d\n",status); */
		/* If status is OK, set CAR to IDLE and DONE record to DONE */
		if(status == OK)
		{
			setCar(OBSSETUP_CAR,CAR_IDLE,OK,"",dummy);
			sprintf(name,"%s%s.VAL",dbTop,OBSSETUP_DONE);
			usVal = NAAC_DONE;
			putDbInfo(name, dummy, DBF_ENUM, &usVal);
			lVal = CAD_MARK;
			putDbInfoT(dbTop, DRROISET_CAD ".DIR", dummy, DBF_LONG, &lVal);
		}
		else /* Otherwise, set CAR and DONE records to ERROR.  Set error code
				  and error message of the CAR record 	*/
		{
			setCar(OBSSETUP_CAR,CAR_ERROR,error,errMess,dummy);
			usVal = NAAC_ERROR;
			putDbInfoT(dbTop, OBSSETUP_DONE ".VAL", dummy, DBF_ENUM, &usVal);
		}
	}
	return status;
}


int doDrRoiSet( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
		int n8, int n9, int n10)
{
    struct cadRecord *pCad;
    long status;
    long error;
    unsigned short usVal;
    char dummy[MAX_STRING_SIZE];
    char errMess[MAX_STRING_SIZE];
    char name[MAX_STRING_SIZE];

    /* Sleep until there is something to do */
    while(1)
    {
	cicsLogMessage(DBG_MIN,"task tDoDrRoiSet sleeping ...");

	/* Wait for semaphore and completion of obsSetup task */
	semTake(semDrRoiSet,WAIT_FOREVER);
	/* EEE we should make some provision to bomb out of here if It is not
	   set to DONE for some timeout period maybe one minute? */

	while( naacStatus(dbTop, OBSSETUP_DONE) != NAAC_DONE)
	    taskDelay(sysClkRateGet()/10);
	cicsLogMessage(DBG_MIN, "task tDoDrRoiSet awake");

	/* Set the CAR to BUSY */
	setCar(DRROISET_CAR,CAR_BUSY,OK,"",dummy);

	/* Get the address of the CAD record so command arguments
	   are accessible 
	   */
	pCad = (struct cadRecord *) n1;
	printCadVals(3,pCad,17);
	cicsLogMessage( 2, "Setting data reduction ROIs");
	status = setDrRoi(pCad, &error, errMess);
	cicsLogMessage( 2, "Data reduction ROIs set");

	/* If status is OK, set CAR to IDLE and DONE record to DONE */
	if(status == OK)
	{
	    setCar(DRROISET_CAR,CAR_IDLE,OK,"",dummy);
	    sprintf(name,"%s%s.VAL",dbTop,DRROISET_DONE);
	    usVal = NAAC_DONE;
	    putDbInfo(name, dummy, DBF_ENUM, &usVal);
	}
	else /* Otherwise, set CAR and DONE records to ERROR. Set error code
		and error message of the CAR record */
	{
	    setCar(DRROISET_CAR,CAR_ERROR,error,errMess,dummy);
	    sprintf(name,"%s%s.VAL",dbTop,DRROISET_DONE);
	    usVal = NAAC_ERROR;
	    putDbInfo(name, dummy, DBF_ENUM, &usVal);
	}
    }
    return status;

}

long uCodeDwnLd(char* path, char* name, struct cadRecord* cadPtr, 
		long *errNo, char* errText   )
{
	/* These are variables so that pointers can be passed */
	long binFalse = (long)FALSE;
	long binTrue = (long)TRUE;
	char buf[80];
	char biState[80], biFailed[80], dummy[80];
	char sPath[256], sName[256];

	long status = (long)OK;

	sprintf(biState, "%s%s.VAL", dbTop, DWNLD_STATE);
	sprintf(biFailed, "%s%s.VAL", dbTop, DWNLD_FAILED);
	strcpy (sPath,path);
	if(sPath[strlen(sPath) - 1] != '/') 
		strcat(sPath,"/");
	strcpy (sName,name);
	sprintf (tmp,"ucode: Path=<%s>, Name=<%s>",sPath,name);
	cicsLogMessage(DBG_NONE,tmp);
	status = ldWaveFormGen(sPath, sName , sPath);

	strcpy (sName,sPath);
	strncat (sName,name,strcspn (name,"."));
	strcat (sName,".cmd");
	sprintf (tmp,"ucode: command file = %s\n",sName);
	cicsLogMessage(DBG_NONE,tmp);


	if ( status == 0 )/* we only want to parse command file on success?*/
	{
		/* 	status = parseCmd( sName, cadPtr ); */
		sprintf(buf,"top=%s,sadtop=%s",dbTop,dbSadTop);
		printf("%s\n",buf);
		status = pvload(sName, buf);
	}

	if ( status != 0 )
	{	/* EEE do we need a CICS log message here ??*/
		/* set dwnLdState.VAL true to indicate that the ucode is downloaded  */
		putDbInfo(biState, dummy, DBF_LONG, &binFalse);
		putDbInfo(biFailed, dummy, DBF_LONG, &binTrue);
		*errNo = 1;
		sprintf(errText, "Download failed");
	}
	else
	{
		putDbInfo(biState, dummy, DBF_LONG, &binTrue);
		putDbInfo(biFailed, dummy, DBF_LONG, &binFalse);
		/* 	putDbInfoT(dbTop, "arSetup.I", dummy, DBF_LONG, &binFalse); */
		putDbInfoT(dbTop, FORCE_DWN_LD ".VAL", dummy, DBF_LONG, &binFalse);
		*errNo = 0;
	}

	return status;
}


long setVoltages( struct cadRecord* cadPtr, long *errNo, char* errText )
{
	int success = TRUE;
	double vSetVal, vdd1Val, vdd2Val, vgg1Val, vgg2Val;
	long status;
	double val;

	/* TRUE and FALSE variables to be used as parameters to db functions    */
	long binFalse = (long)FALSE;
	long binTrue = (long)TRUE;
	long *tfval;

	/* Set vSet voltage */
	status = getDbInfoT(dbTop, ARSETUP_CAD ".VALC", cadPtr->mess, DBF_DOUBLE, &vSetVal);
	if (status == OK)    {
		success &= (getDbInfoT(dbTop, HK_VSET ".VAL", cadPtr->mess, DBF_DOUBLE, &val) == OK);
		if (fabs(fabs(val) - fabs(vSetVal))>  VOLTS_TOLERANCE)
		{

			success &= setHWVoltsT(dbTop, VSET ".VAL",  HK_VSET ".VAL",
					cadPtr, vSetVal);
		}
	}
	else
		success = FALSE;

	/* Set Vdd1 voltage */
	status = getDbInfoT(dbTop, ARSETUP_CAD ".VALD", cadPtr->mess, 
			DBF_DOUBLE, &vdd1Val);
	if (status == OK)
	{
		success &= (getDbInfoT(dbTop, HK_VDDCL1 ".VAL", cadPtr->mess, 
					DBF_DOUBLE, &val) == OK);
		if (fabs(fabs(val) - fabs(vdd1Val))>  VOLTS_TOLERANCE)
		{

			success &= setHWVoltsT(dbTop, VDDCL1 ".VAL",  HK_VDDCL1 ".VAL",
					cadPtr, vdd1Val);
		}
	}
	else
		success = FALSE;

	/* Set Vdd2 voltage */
	status = getDbInfoT(dbTop, ARSETUP_CAD ".VALE", cadPtr->mess, 
			DBF_DOUBLE, &vdd2Val);
	if (status == OK)
	{
		success &= (getDbInfoT(dbTop, HK_VDDCL2 ".VAL", cadPtr->mess,
					DBF_DOUBLE, &val) == OK);
		if (fabs(fabs(val) - fabs(vdd2Val))>  VOLTS_TOLERANCE)
		{
			/* Temporarily commented for eventual removal...see above
				success &= (putDbInfoT(dbTop, VDDCL2 ".VAL", cadPtr->mess, DBF_DOUBLE, &vdd2Val) == 0);
				*/
			success &= setHWVoltsT(dbTop, VDDCL2 ".VAL",  HK_VDDCL2 ".VAL", 
					cadPtr, vdd2Val);
		}
	}
	else
		success = FALSE;

	/* Set Vgg1 voltage */
	status = getDbInfoT(dbTop, ARSETUP_CAD ".VALF", cadPtr->mess, DBF_DOUBLE, 
			&vgg1Val);
	if (status == OK)
	{
		success &= (getDbInfoT(dbTop, HK_VGGCL1 ".VAL", cadPtr->mess, 
					DBF_DOUBLE, &val) == OK);
		if (fabs(fabs(val) - fabs(vgg1Val))>  VOLTS_TOLERANCE)
		{

			success &= setHWVoltsT(dbTop, VGGCL1 ".VAL",  HK_VGGCL1 ".VAL",
					cadPtr, vgg1Val);
		}
	}
	else
		success = FALSE;

	/* Set Vgg2 voltage */
	status = getDbInfoT(dbTop, ARSETUP_CAD ".VALG", cadPtr->mess, DBF_DOUBLE,
			&vgg2Val);
	if (status == OK)
	{
		success &= (getDbInfoT(dbTop, HK_VGGCL2 ".VAL", cadPtr->mess,
					DBF_DOUBLE, &val) == OK);
		if (fabs(fabs(val) - fabs(vgg2Val))>  VOLTS_TOLERANCE)
		{

			success &= setHWVoltsT(dbTop, VGGCL2 ".VAL",  HK_VGGCL2 ".VAL", 
					cadPtr, vgg2Val);
		}
	}
	else
		success = FALSE;

	if (success == FALSE)
	{
		status = ERROR;
		*errNo = VOLT_SET_FAILED;
		sprintf(errText, "Error setting voltages");
		tfval = &binTrue;
	}
	else
	{
		status = GNAAC_OK;
		*errNo = GNAAC_OK;
		sprintf(errText, "All OK");
		tfval = &binFalse;
	}
	putDbInfoT(dbTop, DACS_FAILED ".VAL", cadPtr->mess, DBF_DOUBLE, tfval);

	return status;

}

long setSeqROI( struct cadRecord* cadPtr, long *errNo, char* errText,long numDAvgs,long numLNRs)
{

	/*	char strings to hold names of EPICS records to be manipulated	*/
	char rowHigh[80], colHigh[80], is1024[80], is768[80];
	char is512[80], is384[80], is256[80], is128[80];
	char curMaxCol[80], curMaxRow[80], drROIDone[80];

	long arSizeVal;		/* holds chosen ROI size*/
	long status, sizeOK, colCnt;

	/*	TRUE and FALSE variables to be used as parameters to db functions	*/
	long binFalse = FALSE;
	long binTrue = TRUE;

	/*	long pointers for putDbInfo() parameters */
	long *is256Val, *is512Val, *is768Val, *is1024Val;

	/* 	Set up EPICS records names */
	sprintf(rowHigh, "%s%s.VAL", dbTop, ROW_HI);
	sprintf(colHigh, "%s%s.VAL", dbTop, COL_HI);
	sprintf(is1024, "%s%s.VAL", dbTop, IS1024);
	sprintf(is768, "%s%s.VAL", dbTop, IS768);
	sprintf(is512, "%s%s.VAL", dbTop, IS512);
	sprintf(is384, "%s%s.VAL", dbTop, IS384);
	sprintf(is256, "%s%s.VAL", dbTop, IS256);
	sprintf(is128, "%s%s.VAL", dbTop, IS128);
	sprintf(curMaxCol, "%s%s.VAL", dbTop, CUR_MAX_COL);
	sprintf(curMaxRow, "%s%s.VAL", dbTop, CUR_MAX_ROW);
	sprintf(drROIDone, "%s%s.VAL", dbTop, DRROISET_DONE);

	/* Get array size value from CAD record */
	status = getDbInfoT(dbTop, OBSSETUP_CAD ".VALA", cadPtr->mess,
			DBF_LONG, &arSizeVal);

	if (status == 0)
	{
	switch (arSizeVal)
	{
	  case 256:	
	      is256Val = &binTrue;
	      is512Val = &binFalse;
	      is1024Val = &binFalse;
	      is768Val = &binFalse;
	      sizeOK=TRUE;
	      colCnt=8;
	      break;

	  case 512:
	      is256Val = &binFalse;
	      is512Val = &binTrue;
	      is768Val = &binFalse;
	      is1024Val = &binFalse;
	      sizeOK=TRUE;
	      colCnt=16;
	      break;

	  case 768:
	      is256Val = &binFalse;
	      is512Val = &binFalse;
	      is768Val = &binTrue;
	      is1024Val = &binFalse;
	      sizeOK=TRUE;
	      colCnt=24;
	      break;

	  case 1024:
	      is256Val = &binFalse;
	      is512Val = &binFalse;
	      is768Val = &binFalse;
	      is1024Val = &binTrue;
	      sizeOK=TRUE;
	      colCnt=32;
	      break;

	  default:
	      is256Val = &binFalse;
	      is512Val = &binFalse;
	      is768Val = &binFalse;
	      is1024Val = &binFalse;
	      status = 100;
	      sizeOK=FALSE;
	      colCnt=32;
	      break;
	}
	status += putDbInfo(is128, cadPtr->mess, DBF_LONG, &binFalse);
	status += putDbInfo(is256, cadPtr->mess, DBF_LONG, is256Val);
	status += putDbInfo(is384, cadPtr->mess, DBF_LONG, &binFalse);
	status += putDbInfo(is512, cadPtr->mess, DBF_LONG, is512Val);
	status += putDbInfo(is768, cadPtr->mess, DBF_LONG, is768Val);
	status += putDbInfo(is1024, cadPtr->mess, DBF_LONG, is1024Val);
	status += putDbInfo(curMaxCol, cadPtr->mess, DBF_LONG, &arSizeVal);
	status += putDbInfo(curMaxRow, cadPtr->mess, DBF_LONG, &arSizeVal);

	status += putDbInfo(rowHigh, cadPtr->mess, DBF_LONG, &arSizeVal);
	status += putDbInfo(colHigh, cadPtr->mess, DBF_LONG, &arSizeVal);
	status += putDbInfo(curMaxCol, cadPtr->mess, DBF_LONG, &arSizeVal);
	status += putDbInfo(curMaxRow, cadPtr->mess, DBF_LONG, &arSizeVal);
    }

    if (status == 0 && sizeOK == TRUE)
    {
	status = putDbInfoT(dbTop, SEQCOLCNTSET ".VAL", cadPtr->mess, DBF_LONG, &colCnt);
    }
    
    if (status != 0)
    {
	*errNo = 4;
	sprintf(errText, "Failure setting Seq ROI");
    }
    else
	*errNo = 0;

    return status;
}

long setSeqVars(struct cadRecord* cadPtr, long *errNo, char* errText)
{
    char cadNumCoAdds[80], cadNumLNRs[80], cadNumDAvgs[80], cadNumPics[80];
    char numCoAdds[80], numLNRs[80], numDAvgs[80], numPics[80];

    double numCoAddsVal, numLNRsVal, numDAvgsVal, numPicsVal;

    int status = 0;

    sprintf(cadNumCoAdds, "%s%s.VALE", dbTop, OBSSETUP_CAD);
    sprintf(cadNumLNRs, "%s%s.VALD", dbTop, OBSSETUP_CAD);
    sprintf(cadNumDAvgs, "%s%s.VALC", dbTop, OBSSETUP_CAD);
    sprintf(cadNumPics, "%s%s.VALF", dbTop, OBSSETUP_CAD);

    sprintf(numCoAdds, "%s%s.VAL", dbTop, NUM_COADDS);
    sprintf(numLNRs, "%s%s.VAL", dbTop, NUM_LNRS);
    sprintf(numDAvgs, "%s%s.VAL", dbTop, NUM_DAVGS);
    sprintf(numPics, "%s%s.VAL", dbTop, NUM_PICS);

    status  = getDbInfo(cadNumCoAdds, cadPtr->mess, DBF_DOUBLE, &numCoAddsVal);
    status += putDbInfo(numCoAdds, cadPtr->mess, DBF_DOUBLE, &numCoAddsVal);
    status += getDbInfo(cadNumLNRs, cadPtr->mess, DBF_DOUBLE, &numLNRsVal);
    status += putDbInfo(numLNRs, cadPtr->mess, DBF_DOUBLE, &numLNRsVal);
    status += getDbInfo(cadNumDAvgs, cadPtr->mess, DBF_DOUBLE, &numDAvgsVal);
    status += putDbInfo(numDAvgs, cadPtr->mess, DBF_DOUBLE, &numDAvgsVal);
    status += getDbInfo(cadNumPics, cadPtr->mess, DBF_DOUBLE, &numPicsVal);
    status += putDbInfo(numPics, cadPtr->mess, DBF_DOUBLE, &numPicsVal);

    if (status != 0)
    {
	*errNo = 10;
	sprintf(errText, "Error setting Seq vars");
    }
    else
	*errNo = 0;

    return status;
}

long setHdrVars(struct cadRecord* cadPtr, long *errNo, char* errText)
{
    char cadHdrDetail[80], cadTitle[80], cadSeqNum[80], cadComment[80];
    char hdrDetail[80], title[80], seqNum[80], comment[80];

    char titleVal[80], commentVal[80];
    double hdrDetailVal, seqNumVal;

    int status;

    sprintf(cadHdrDetail, "%s%s.VALH", dbTop, OBSSETUP_CAD);
    sprintf(cadTitle, "%s%s.VALI", dbTop, OBSSETUP_CAD);
    sprintf(cadSeqNum, "%s%s.VALJ", dbTop, OBSSETUP_CAD);
    sprintf(cadComment, "%s%s.VALK", dbTop, OBSSETUP_CAD);

    sprintf(hdrDetail, "%s%s.VAL", dbTop, HDR_DETAIL);
    sprintf(title, "%s%s.VAL", dbTop, TITLE);
    sprintf(seqNum, "%s%s.VAL", dbTop, SEQ_NUM);
    sprintf(comment, "%s%s.VAL", dbTop, COMMENT);

    status  = getDbInfo(cadHdrDetail, cadPtr->mess, DBF_LONG, &hdrDetailVal);
    status += putDbInfo(hdrDetail, cadPtr->mess, DBF_LONG, &hdrDetailVal);
    status += getDbInfo(cadTitle, cadPtr->mess, DBF_STRING, titleVal);
    status += putDbInfo(title, cadPtr->mess, DBF_STRING, titleVal);
    status += getDbInfo(cadSeqNum, cadPtr->mess, DBF_LONG, &seqNumVal);
    status += putDbInfo(seqNum, cadPtr->mess, DBF_LONG, &seqNumVal);
    status += getDbInfo(cadComment, cadPtr->mess, DBF_STRING, commentVal);
    status += putDbInfo(comment, cadPtr->mess, DBF_STRING, commentVal);

    if (status != 0)
    {
	*errNo = 7;
	sprintf(errText, "Error setting heading vars");
    }
    else
	*errNo = 0;

    return status;
}

long setObsState(struct cadRecord* cadPtr, long *errNo, char* errText)
{
/* EEE
   This function is responsible for setting the HouseKeeping state of the
   instrument and the processing mode.  These settings are determined by the
   user, entered into DM screens attached to a CAD record, and then read from
   the CAD record by this code and written to hardware channels.
*/
    char cadHkState[80], cadProcMode[80];/* Vars to hold cad record */
    char hkState[80], procMode[80];	     /* Vars to hold other records  */
    long hkStateVal;		     /* Holder for HK State setting */
 

    int status;			     /* Cumulative process status    */

/* Set up strings that hold names of CAD records+fields to get user settings */

    sprintf(cadHkState, "%s%s.VALL", dbTop, OBSSETUP_CAD);
    sprintf(cadProcMode, "%s%s.VALM", dbTop, OBSSETUP_CAD);

/* These strings hold names of stringout and longout records to set values
	according to user-specified values*/

    sprintf(hkState, "%s%s.VAL", dbTop, HK_STATE);
    sprintf(procMode, "%s%s.VAL", dbTop, PROC_MODE);

/*	Get and set housekeeping mode*/

    status  = getDbInfo(cadHkState, cadPtr->mess, DBF_LONG, &hkStateVal);
    status += putDbInfo(hkState, cadPtr->mess, DBF_LONG, &hkStateVal);

/* Get processing mode from user via CAD record and set to hardware through
   a string out record*/

    status += getDbInfo(cadProcMode, cadPtr->mess, DBF_LONG, &svrP.procMode);
    status += putDbInfo(procMode, cadPtr->mess, DBF_LONG, &svrP.procMode);

    if (status != 0)
    {
	*errNo = 8;
	sprintf(errText, "Unable to set Obs state");
    }
    else
	*errNo = 0;

    return status;
}

long setIntTime(struct cadRecord* cadPtr, long *errNo, char* errText)
{

/* This function sets the integration time, based on input from the user, via
   the Obs Setup CAD record. */
    char cadSeqIntTime[80],
	 cadSeqFDly[80];	/* CAD record field names */
    char intTime[80], fDly[80],
	 errIntTime[80];	/* non-CAD record fields */
    double intTimeVal, fDlyVal;	/* Variables to hold settings from CAD	*/

    int status;				/* Return status code	*/

    int binFalse = FALSE;	/* Two binary variables so they can be	*/
    int binTrue = TRUE;		/*  passed by pointer to putDbInfo()	*/

/* Set up names of all records and fields of records so that we can get and set
   values from EPICS records */	

    sprintf(cadSeqIntTime, "%s%s.VALG", dbTop, OBSSETUP_CAD);
    sprintf(cadSeqFDly, "%s%s.VALO", dbTop, OBSSETUP_CAD);
    sprintf(intTime, "%s%s.VAL", dbTop, INT_TIME);
    sprintf(fDly, "%s%s.VAL", dbTop, FDELAY);
    sprintf(errIntTime, "%s%s.VAL", dbTop, ERR_INT_TIME);

/* Get values from CAD record and set them into ao records to pass to hardware
*/

    status  = getDbInfoT(dbTop, OBSSETUP_CAD ".VALG", cadPtr->mess,
			 DBF_DOUBLE, &intTimeVal);
    status += putDbInfo(intTime, cadPtr->mess, DBF_DOUBLE, &intTimeVal);
    status += getDbInfo(cadSeqFDly, cadPtr->mess, DBF_DOUBLE, &fDlyVal);
    status += putDbInfo(fDly, cadPtr->mess, DBF_DOUBLE, &fDlyVal);
    
    if (status == 0)	/* No errors in get/set of EPICS fields	 */
    {
	putDbInfo(errIntTime, cadPtr->mess, DBF_LONG, &binFalse);
	*errNo = 0;
    }
    else	/* One or more db access resulted in error */
    {
	putDbInfo(errIntTime, cadPtr->mess, DBF_LONG, &binTrue);
	*errNo = 6;
	sprintf(errText, "Error setting integration time");
    }
	
    return status;
}

#define BIAS_MARGIN .050 /* achieved bias must be within 50 mv of request */

long setBias(struct cadRecord* cadPtr, long *errNo, char* errText)
{
    double biasVal, vdducVal, actBias, chanVdetVal;
    long status;

    int *rVal;
    int binFalse = FALSE;	/* Two binary variables so they can be	*/
    int binTrue = TRUE;		/*  passed by pointer to putDbInfo()	*/

    status = getDbInfoT(dbTop, ARSETUP_CAD ".VALH", cadPtr->mess,
		       					DBF_DOUBLE, &biasVal);

    status += putDbInfoT(dbTop, DBIAS ".VAL", cadPtr->mess, DBF_DOUBLE, &biasVal);
	sleep(0,5000000); /* 5 ms for bias to settle */
    status += putDbInfoT(dbTop, HK_VDDUC ".VAL", cadPtr->mess, DBF_DOUBLE, 
			 &vdducVal);
    status += getDbInfoT(dbTop, HK_VDDUC ".VAL", cadPtr->mess, DBF_DOUBLE,
			 &vdducVal);
    status += putDbInfoT(dbTop, HK_VDET ".VAL", cadPtr->mess, DBF_DOUBLE,
			 &chanVdetVal);
    status += getDbInfoT(dbTop, HK_VDET ".VAL", cadPtr->mess, DBF_DOUBLE,
			 &chanVdetVal);

    actBias = fabs(fabs(vdducVal) - fabs(chanVdetVal));

    if (status != GNAAC_OK)
    {
	sprintf(errText, "Bias set failed %d", status);
	rVal = &binTrue;
	*errNo = BIASFAILED;
    }
    else if (fabs(fabs(actBias) - fabs(biasVal)) > VOLTS_TOLERANCE)
    {

	sprintf(errText, "Bias not achieved");
	rVal = &binTrue;
	*errNo = BIASFAILED;
    }
    else
    {
	rVal = &binFalse;
	sprintf(errText, "%s", "All OK");
    }

    printf("DC: Bias level is: %f\n", actBias);	
    status += putDbInfoT(dbTop, BIAS_FAILED ".VAL", cadPtr->mess,
							DBF_DOUBLE, rVal);
    return status;
}

long setDrRoi(struct cadRecord* pCad, long *errNo, char* errText)
{
    long **pval;
    struct roi {
	char lowRow[80];
	char lowCol[80];
	char hiRow[80];
	char hiCol[80];
    } roiTable[4];		/* holds record name strings for ROIs	*/
    long status;		/* function return code		*/
    int i;			/* miscellaneous index value		*/
    char cadNumROI[MAX_STRING_SIZE];
    int numRoiVal;

    for (i=0; i<=3; i++)	/* initialize record names		*/
    {
	sprintf(roiTable[i].lowRow, "%s%s%1d.VAL", dbTop,LOWROW, i+1);
	sprintf(roiTable[i].lowCol, "%s%s%1d.VAL", dbTop,LOWCOL, i+1);
	sprintf(roiTable[i].hiRow, "%s%s%1d.VAL", dbTop,HIROW, i+1);
	sprintf(roiTable[i].hiCol, "%s%s%1d.VAL", dbTop,HICOL, i+1);
    }


    sprintf(cadNumROI, "%s%s.VALA", dbTop, DRROISET_CAD);

 

/*
  Start off by getting the number of ROIs defined by the user, then setting up
  those ROI regions in the EPICS records  */

    status = getDbInfo(cadNumROI, pCad->mess, DBF_LONG, &numRoiVal);

/*
  For each defined ROI, get the low and high row and col values from the 
  CAD record and transfer them to the lower-level records that define 
  those regions for the  hardware system  */
    pval = (long **)&pCad->valb;

    for (i = 0; i < numRoiVal; i++)
    {

	printf("lowRow %d = %d\n",i,**pval);
	status += putDbInfo(roiTable[i].lowRow, pCad->mess, DBF_LONG,
			    	*pval);

	pval++;

	printf("lowCol %d = %d\n",i,**pval);
	status += putDbInfo(roiTable[i].lowCol, pCad->mess, DBF_LONG,
			    	*pval);
	pval++;

	printf("HiRow %d = %d\n",i,**pval);
	status += putDbInfo(roiTable[i].hiRow, pCad->mess, DBF_LONG, 
				*pval);
	pval++;

	printf("hicol %d = %d\n",i,**pval);
	status += putDbInfo(roiTable[i].hiCol, pCad->mess, DBF_LONG,
			    	*pval);
	pval++;
    }

    if (status != 0)
    {
	*errNo = 9;
	sprintf(errText, "Error in DR ROI setting");
    }
    else
	*errNo = 0;

    return status;
}

#define ARRAY_INACTIVE	0
#define ARRAY_ACTIVE	1
#define ARRAY_ERR		2

long chkActiv(struct cadRecord* cadPtr, long *errNo, char* errText)
{
    long status=OK;
    double vdducVal, vDetVal, bokVal;
    double vDetDist, vdducDist;
    long *rVal, detState;
    long binTrue = TRUE;
    long binFalse = FALSE;
    int arrStat;
	double temp;


    char activate[MAX_STRING_SIZE], activChk[MAX_STRING_SIZE];
    char vDDuc[MAX_STRING_SIZE],vDet[MAX_STRING_SIZE], bOK[MAX_STRING_SIZE];

    sprintf(activate, "%s%s.VAL", dbTop, ACTIVATE);
    sprintf(activChk, "%s%s.VAL", dbTop, ACTIVE_CHK);
    sprintf(vDDuc, "%s%s.VAL", dbTop, VDDUC_CHAN);
    sprintf(vDet, "%s%s.VAL", dbTop, VDET_CHAN);
    sprintf(bOK, "%s%s.VAL", dbTop, BOK);

    /*See what state the user wants the array to be in by checking VALB of the
	obsSetup CAD record.*/
	status = getDbInfoT(dbTop, OBSSETUP_CAD ".VALB", cadPtr->mess, 
			    DBF_LONG, &detState);

#if 0
	/*pbr 8-13-01 addes this to ensure array doesn't get activated when warm */
	if(activationFlag == 0)
	{
		status = getDbInfoT(dbTop, TEMP_DETABS , cadPtr->mess, DBF_DOUBLE, &temp);
	
		if(status == OK)
		  {
			if(temp > MAX_ACTIVATION_TEMP)
			{
				
			  if(detState == 1)
				{
				  detState = 0;
				  sprintf(errText,"Array is too warm to activate\n");
				  printf("temp = %f detstate = %d, MAX = %d\n",temp,detState,MAX_ACTIVATION_TEMP);
				  printf(errText);
				  status = ERROR;
				}
			}
		  }
		else
		  printf("error getting temp\n");
		
	}
#endif
	
	/* Kick things off by pulsing the "activate" record to set the array 
	   to either active or not, depending on user input.  Then check the 
	   three channel values to see what the result is.*/
	if(status == OK)
	{
		if ( detState == 1)
			status += putDbInfo(activate, cadPtr->mess, DBF_LONG, &binTrue);
		else
			status += putDbInfo(activate, cadPtr->mess, DBF_LONG, &binFalse);

		/* 	printf("status = %d\n",status); */
		/*
		  Check on the current status of the array and show it to the user.
		*/

		status += putDbInfoT(dbTop, READHK ".VAL", cadPtr->mess,  DBF_DOUBLE, &vdducVal);
			   
		/* 	printf("status = %d\n",status);  */

		status += getDbInfo(vDDuc, cadPtr->mess, DBF_DOUBLE, &vdducVal);
		/* 	printf("status = %d\n",status); */
		status += getDbInfo(vDet, cadPtr->mess, DBF_DOUBLE, &vDetVal);
		/* 	printf("status = %d\n",status); */
		status += getDbInfo(bOK, cadPtr->mess, DBF_DOUBLE, &bokVal);
		/* 	printf("status = %d\n",status); */

		if (status != 0)	/* something is wrong...horribly wrong!	*/
		{
			arrStat = ARRAY_ERR;
			*errNo = 5;
			sprintf(errText, "Array activation error");
			printf(errText);
		}
		else		/* see what state the array was left in	*/
		{
			printf("status = OK\n");
			*errNo = 0;
			strcpy(errText,"");
			rVal = &binTrue;
			vDetDist = abs(bokVal - vDetVal);
			vdducDist = abs(bokVal - vdducVal);
			if (vDetDist < vdducDist)
				arrStat = ARRAY_ACTIVE;
			else
				arrStat = ARRAY_INACTIVE;
		}
	}

	/* Post array status to EPICS record */

    status += putDbInfo(activChk, cadPtr->mess, DBF_LONG, &arrStat);
	/* 	printf("status = %d\n",status); */
    return status;
}
long initSub(struct subRecord *pSub)
{
	return OK;
}
long deactivateWarm(struct subRecord *pSub)
{
	long status = OK;
/* 	printf("temp = %f, max = %d\n",pSub->a,MAX_ACTIVATION_TEMP); */
	if((pSub->a > MAX_ACTIVATION_TEMP)&&(activationFlag == 0))
		setArrayActivation(0);
	return status;
}
long setArrayActivation(long detState)
{
	char errText[80];
    long status;
    double vdducVal, vDetVal, bokVal;
    double vDetDist, vdducDist;
    long *rVal;
    long binTrue = TRUE;
    long binFalse = FALSE;
    int arrStat;
	char mess[80];


    char activate[MAX_STRING_SIZE], activChk[MAX_STRING_SIZE];
    char vDDuc[MAX_STRING_SIZE],vDet[MAX_STRING_SIZE], bOK[MAX_STRING_SIZE];

    sprintf(activate, "%s%s.VAL", dbTop, ACTIVATE);
    sprintf(activChk, "%s%s.VAL", dbTop, ACTIVE_CHK);
    sprintf(vDDuc, "%s%s.VAL", dbTop, VDDUC_CHAN);
    sprintf(vDet, "%s%s.VAL", dbTop, VDET_CHAN);
    sprintf(bOK, "%s%s.VAL", dbTop, BOK);

	
	
	/* Kick things off by pulsing the "activate" record to set the array 
	   to either active or not, depending on user input.  Then check the 
	   three channel values to see what the result is.*/
	if ( detState == 1)
	{
		activationFlag = 1;
	    status += putDbInfo(activate, mess, DBF_LONG, &binTrue);
	}
	else
	{
		activationFlag = 0;
	    status += putDbInfo(activate, mess, DBF_LONG, &binFalse);
	}

/*
	Check on the current status of the array and show it to the user.
*/

	status += putDbInfoT(dbTop, READHK ".VAL", mess, 
			     DBF_DOUBLE, &vdducVal);

	status += getDbInfo(vDDuc, mess, DBF_DOUBLE, &vdducVal);
	status += getDbInfo(vDet, mess, DBF_DOUBLE, &vDetVal);
	status += getDbInfo(bOK, mess, DBF_DOUBLE, &bokVal);

    if (status != 0)	/* something is wrong...horribly wrong!	*/
    {
		arrStat = ARRAY_ERR;
		/* 	*errNo = 5; */
		sprintf(errText, "Array activation error");
    }
    else		/* see what state the array was left in	*/
    {
		/* 	*errNo = 0; */
		rVal = &binTrue;
		vDetDist = abs(bokVal - vDetVal);
		vdducDist = abs(bokVal - vdducVal);
		if (vDetDist < vdducDist)
			arrStat = ARRAY_ACTIVE;
		else
			arrStat = ARRAY_INACTIVE;
    }

/* Post array status to EPICS record */

    status += putDbInfo(activChk, mess, DBF_LONG, &arrStat);
    return status;
}

double calcMinInt(long arSizeVal, long DAvgs, long Lnrs)
{
  /*
  Calculate minimum integration time, which is a function of array size,
  number of low-noise reads, and number of digital averages.
  
  base = [((maxRow*maxCol)/32) * (3.1us + minDly)]
  deadTime = minRead - minInt
  overhead = minRead - base
  minInt = [(size^2/32) * (3.1us + (dAvgs * minDly))] * lnrs + deadTime + overhead
*/
  double fudge = 0;
  double photonTime, speed, minDly, firstDly;
  double minInt, minRead, deadTime, overhead, dAvgsDly;
  long numDAvgs;
  long codeType, numPixels, maxRow, maxCol, numReads;
  double dAvgTime, baseTime;
  long status = OK;
  char dummy[40];
  
  status += getDbInfoT(dbTop, UC_MINREAD ".VAL", dummy, DBF_DOUBLE, &minRead);
 
  status += getDbInfoT(dbTop, UC_MINDLY ".VAL", dummy, DBF_DOUBLE, &minDly);
  status += getDbInfoT(dbTop, UC_FIRSTDLY ".VAL", dummy, DBF_DOUBLE, &firstDly);
  status += getDbInfoT(dbTop, UC_DAVGDLY ".VAL", dummy, DBF_DOUBLE, &dAvgsDly);
  status += getDbInfoT(dbTop, UC_CODETYPE ".VAL", dummy, DBF_LONG, &codeType);
  status += getDbInfoT(dbTop, MAXROW ".VAL", dummy, DBF_LONG, &maxRow);
  status += getDbInfoT(dbTop, MAXCOL ".VAL", dummy, DBF_LONG, &maxCol);
/*   printf("status = %d, %d, arSizeVal = %d\n",OK,status,arSizeVal); */
/*   printf("mindly = %f, firstdly = %f, davgdly = %f, minRead = %f\n",minDly,firstDly,dAvgsDly,minRead ); */
  if (status == OK)
    {
	switch (arSizeVal)
	{
	  case 256:
	    fudge = .008;
	  case 512:
	    fudge = .004;
	  case 768:
	    fudge = .002;
	}
	      
      switch (codeType)
	{
	case RD:		/* Not yet defined							*/ 
	  status += getDbInfoT(dbTop, UC_MININT ".VAL", dummy, DBF_DOUBLE, 
			       &minInt);
	
	  break;
	  
	case RDD:
	default:
	  numDAvgs = DAvgs;
	  baseTime = ((arSizeVal * arSizeVal)/32) * firstDly;

	  deadTime = minDly;
	  overhead = minRead - ((ARRAY_SZ * ARRAY_SZ)/32) * firstDly;;
	  numPixels = arSizeVal * arSizeVal;
	  numReads = numPixels/32;
	  dAvgTime = ((numDAvgs-1.0) * dAvgsDly);
	  minRead = (baseTime + (numReads * dAvgTime) + overhead);
	  minInt = (minRead + deadTime) * Lnrs + fudge;
	  photonTime = 2*minInt;
	  speed = 1/photonTime;
/* 	  printf("fudge = %f, minInt = %f\n",fudge,minInt); */
	  
	  break;	
	}
   
      putDbInfoT(dbTop,MININT,dummy,DBF_DOUBLE,&minInt);
      putDbInfoT(dbTop,MINREAD,dummy,DBF_DOUBLE,&minRead);
      putDbInfoT(dbTop,MAX_PHOTON_TIME,dummy,DBF_DOUBLE,&photonTime);
      putDbInfoT(dbTop,MAX_SPEED,dummy,DBF_DOUBLE,&speed);
    }
  else
    minInt = -1;
 /*  setEpicsAlarmT(dbTop,"integTime",minInt,100000,minInt,100001); */
  return minInt;
}

/*This function is intended to set and verify voltages in the Hardware.
  It uses EPICS calls to set requested values, waits for the voltages to 
  settle, then strobes the READHK line.  After waiting 3 seconds for the 
  HK read to complete, EPICS is again used to read the achieved voltages.  
  Those voltages are then compared against the requested values.  If the 
  difference between the requested and achieved voltages is greater than 
  VOLTS_TOLERANCE, then the attempt was unsuccessful and we report that 
  back to the caller.*/

int setHWVoltsT(char *top,char* target, char* check, struct cadRecord* errRef, double value)
{
  char n[80],c[80];
  strcpy(n,top);
  strcat(n,target);
  strcpy(c,top);
  strcat(c,check);
  return setHWVolts(n, c, errRef, value);
  
}
int setHWVolts(char* target, char* check, struct cadRecord* errRef, double value)
{
    int success = TRUE;
    int retVal = OK;
    double margin, rbValue=1.0;
    
    retVal = putDbInfo(target, errRef->mess, DBF_DOUBLE, &value);
    sleep(0,500000000);
    success |= putDbInfo(check, errRef->mess, DBF_DOUBLE, &rbValue);
    success |= getDbInfo(check, errRef->mess, DBF_DOUBLE, &rbValue);
    margin = fabs(fabs(value) - fabs(rbValue));
    if ((margin > VOLTS_TOLERANCE) || (retVal != OK))
    {
	sprintf(tmp,"Error setting %s. %f %f %f \n", target,value,rbValue,
		margin);
	cicsLogMessage(DBG_NOLOG,tmp);
	success = FALSE;
    }
    return success;
}
