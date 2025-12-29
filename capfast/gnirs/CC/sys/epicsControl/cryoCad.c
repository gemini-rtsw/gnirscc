static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: cryoCad.c,v 1.2 2013/06/06 01:54:27 gemvx Exp $"
};

/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	cryo.c
 *
 * DESCRIPTION 
 * 	Implements Gemini "cryo" command which controls the state of the cryo heads.
 *
 * 
 * FUNCTION NAME(S)
 *	cryoCad
 *
 *   
 * DEPENDENCIES
 * 	EPICS support libraries
 *
 *
 *INDENT-OFF*
 * $Log: cryoCad.c,v $
 * Revision 1.2  2013/06/06 01:54:27  gemvx
 * Checking in for fixes related to REL-1149.
 *
 * REL-1149 requires moving FW1 into a blocking position when the aquisition
 * mirror is moving out. This will inhibit, "squiggles" on the detector caused
 * by a bright star reflecting off of the aquisition mirror onto the detector.
 *
 * See: http://swgserv01.cl.gemini.edu:8080/browse/REL-1149
 *
 * tom.c
 *
 * Revision 1.1  2009/06/10 15:05:10  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */
#define ON 1
#define OFF 0
#define CRYO_SWITCH d

/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>

/* EPICS specific include files */
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsTasks.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>
#include <genSubRecord.h>
#include <genSub.h>

/*function prototypes*/
int cryoCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
			  int n9, int n10 );

/* global variables*/


static carVal = CAR_IDLE;/* this value will have the current state of the car*/
long cryoId;  /*pid of the cryo process spawned below*/
SEM_ID semCryo;/* semaphore used for communication with the cryo process*/

/*
 *+
 * FUNCTION NAME:
 *	cryoCadInit
 *
 * INVOCATION:
 *	struct cadRecord *pCad;
 * 	status = cryoCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 	> pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * 	long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * 	User defined function to initialize data structures necessary for the
 * "cryo" CAD record
 *
 *
 *
 * DESCRIPTION:
 *	This function is run during iocInit.  Creates the comminication semaphore
 *    and sets initial values of cad record.
 *
 *
 * EXTERNAL VARIABLES:
 *	none
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *
 *
 *
 *-
 */
int cryoCadInit(struct cadRecord *pCad)
{
	long status = CAD_ACCEPT;

	/* 	LOG_MSG(DEBUG2_MSG,"cryoCadInit****************************\n"); */
	/*create semaphore*/
/*	printf("create semaphore\n");*/
	semCryo = semBCreate(SEM_EMPTY,SEM_Q_FIFO);

	/*spawn task*/
/*	printf("spawn task\n");*/
	LOG_MSG(DEBUG2_MSG,"spawn task\n");
	if(status == CAD_ACCEPT)
		cryoId = taskSpawn("tCryoCtrl",50,VX_FP_TASK,4000,cryoCtrl, pCad,0,0,0,0,0,0,0,0,0);
	if(cryoId == ERROR)
	{
		status = CAD_REJECT;
		strncpy(MESSAGE,"Init: Error Spawning task",MAX_STRING_SIZE - 1);
	}
		
    
    /*valh is connected to car record*/
    *(long *)pCad->valh = carVal;

	/*set initial output state to off*/
	strcpy(pCad->vald,"OFF");

/*	printf("done\n");*/
	return status;
}

/*
 *+
 * FUNCTION NAME:
 *	cryoCad
 *
 * INVOCATION:
 *	struct cadRecord *pCad;
 * 	status = cryoCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 	> pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * 	long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * 	User defined function for "cryo" CAD record
 *
 *
 *
 * DESCRIPTION:
 *	This function is triggered by the cryo cad record.  When start
 * 	is received, it releases a semaphore to allow the cryoCtrl routine
 *	to run.
 *
 *
 * EXTERNAL VARIABLES:
 *	none
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *
 *
 *
 *-
 */
int cryoCad(struct cadRecord *pCad)
{
	static long sw;
	long status = CAD_ACCEPT;

	/* Switch according to the CAD directive in DIR field */
	switch (DIRECTIVE)
	{
		/* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
		LOG_MSG(DEBUG2_MSG, "cryo - MARK directive.");
		break;

		/* CAD PRESET directive detected.  Check the input argument. 
		   If it is acceptable then copy it to the output.    */
	
	  case CAD_PRESET:
		LOG_MSG(DEBUG2_MSG, "cryo - PRESET directive.");
		cicsLogString( 3, "Requested Cryo Level =", cadInput(CRYO_SWITCH));

	  
		if(strcmp(cadInput(CRYO_SWITCH),"ON") == 0)
			sw = ON;
		else if(strcmp(cadInput(CRYO_SWITCH),"OFF") == 0)
			sw = OFF;
		else
		{
			strncpy( MESSAGE, "Unrecognized cryo level",MAX_STRING_SIZE - 1 );
			cicsLogMessage(0, MESSAGE);
			status = CAD_REJECT;
		}        	 
		break;

		/* CAD CLEAR directive detected. Nothing needs to be done. */
	  case CAD_CLEAR:
		LOG_MSG(DEBUG2_MSG, "cryo - CLEAR directive.");
		break;

	
		break;

		/* CAD START directive detected. Set CAR to BUSY by processing the CAR
		   interface record, then call function to set the cryo level.  The CAR
		   will be set back to IDLE automatically by a seq record in the database. 
		*/ 
	  case CAD_START:
		LOG_MSG(DEBUG2_MSG, "cryo - START directive.");
	    carVal = CAR_BUSY;
		
		LOG_MSG(DEBUG1_MSG,"start directive cryoCad\n");
		/*set cryo head to the state of the epics input*/
	
		LOG_MSG(DEBUG1_MSG,"heads set cryoCad\n");
		/*set cryo sir to value of computer switch*/
		semGive(semCryo);

		break;

		/* CAD STOP directive detected. 
		 */
	  case CAD_STOP:
		cicsLogMessage( 1, "cryoCad: Cannot be stopped.");
		status = CAD_REJECT;
		strncpy( MESSAGE, "cryoCad: Cannot be stopped",MAX_STRING_SIZE - 1  );
		break;

		/* Unrecognised CAD directive detected. This is regarded as an error. */
	  default:
		strncpy( MESSAGE, "cryoCad: Unrecognized CAD directive",MAX_STRING_SIZE - 1 );
		status = CAD_REJECT;
		break;
	}
  
	LOG_MSG(DEBUG1_MSG,"exit cryoCad\n");
	return status;
}

/*
 *+
 * FUNCTION NAME:
 *	cryoSub
 *
 * INVOCATION:
 *	struct genSubRecord *pgenSub;
 * 	status = cryoSub( pgenSub );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 	> pgenSub   (struct genSubRecord *)   pointer to GENSUB data structure
 *  > pgenSub->a    - 
 *  > pgenSub->b    - 
 *  > pgenSub->c    - 
 *  < pgenSub->vala - 
 *  < pgenSub->valb - 
 *  < pgenSub->valc - 
 *  < pgenSub->vald - 
 *
 * FUNCTION VALUE:
 * 	long  Status value written to GENSUB VAL field
 *
 * PURPOSE:
 * 	User defined function for "cryo" GENSUB record
 *
 *
 *
 * DESCRIPTION:
 *	This function is triggered by the cryo genSub record. When triggered
 *   it releases a semaphore to allow the cryoCtrl routine
 *	 to run.
 *
 *
 * EXTERNAL VARIABLES:
 *	none
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *
 *
 *
 *-
 */
int cryoSub(struct genSubRecord *pgenSub)

{
    long status = OK;
    /* Action to be taken depends on directive received		*/
   
/* 	LOG_MSG(DEBUG2_MSG, "cryoSub - START "); */
	

	/*switch is set to computer control of cryo heads*/
	if(strcmp(pgenSub->a, "COMPUTER")== 0)
	{
		/* 			printf("computer\n"); */
		strcpy(pgenSub->vala,"COMPUTER");

		/*set output to ON if input is on otherwise set to OFF*/
		if(strcmp(pgenSub->c,"ON")==0)
		{
			strcpy (pgenSub->vald,"ON");
		}
		else
		{
			strcpy (pgenSub->vald,"OFF");
		}

	}
	else  /* switch is set to manual*/
	{
		
		strcpy(pgenSub->vala,"MANUAL");
		if(strcmp(pgenSub->b,"ON") == 0)
		{
			strcpy (pgenSub->vald,"ON");
			/* putDbInfoT(dbTop,"cryoSwitch",err,DBF_LONG,&zero); */
		}
		else	
		{			
			strcpy (pgenSub->vald,"OFF");
			/* putDbInfoT(dbTop,"cryoSwitch",err,DBF_LONG,&one); */
				
		}
	}

	if(strcmp(pgenSub->b, "ON") == 0)
		strcpy(pgenSub->valb,"ON");
	else
		strcpy(pgenSub->valb,"OFF");

	if(strcmp(pgenSub->c, "ON") ==0)
		strcpy(pgenSub->valc,"ON");
	else
		strcpy(pgenSub->valc,"OFF");
	
   
    return status;
 
}

/*
 *+
 * FUNCTION NAME:
 * 	cryoCtrl
 *
 * INVOCATION:
 *	Spawned from initTasks routine.
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	 Set of 10 integer variables, most of which mean nothing, some of which
 *      	are interpreted in various ways.
 *
 * FUNCTION VALUE:
 *	int             Result of function...should never be seen.
 *
 * PURPOSE:
 *	perform a cryo.
 *
 * DESCRIPTION:
 *	Started by initTasks, when system boots up.  Waits on a semaphore
 *	which is released by the cryoCad routine.  Tells low level software
 *	to perform a cryo.
 *
 *
 * EXTERNAL VARIABLES:
 *	none
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *
 *
 *
 *-
 */
int cryoCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8,
			  int n9, int n10 )
{
	char msg[80];
    char errMess[80];
    char err[80];
    struct cadRecord* pCad;
    long status = OK;


    /* Convert first parameter to CAD address	*/
    pCad = (struct cadRecord *) n1;

    /* Repeat as inifinite loop	*/
    while(1)
    {
		status = OK;
		LOG_MSG(DEBUG2_MSG, "Task tcryoCtrl sleeping...\n");
		/* Wait for our semaphore   */
		semTake(semCryo, WAIT_FOREVER);
		LOG_MSG(DEBUG2_MSG, "Task tcryoCtrl awake..."); 
		carVal = CAR_BUSY;
		sprintf(msg,"a = %s, b = %s, c = %s, input = %s\n",pCad->a,pCad->b,pCad->c,pCad->d);
		LOG_MSG(DEBUG2_MSG,msg ); 
		if(strcmp(pCad->d,"OFF") == 0)
		{
			cryoHead(0);
		}
		else	
		{
			cryoHead(1);
		}
		if(status == OK)
		{
			carVal = CAR_IDLE;
			if(setCar(CRYO_CAR,CAR_IDLE,OK,"",err) != OK)
			{
				LOG_MSG(ERROR_MSG,err);
			}
		}
		else
		{
			carVal = CAR_ERROR;
			if(setCar(CRYO_CAR,CAR_ERROR,ERROR,errMess,err) != OK)
			{
				LOG_MSG(ERROR_MSG,err);
			}

		}
    }
    return status;
}
