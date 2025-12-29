static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: initCad.c,v 1.2 2009/08/01 04:28:41 mrippa Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * initCad.c
 *
 * DESCRIPTION 
 * Support for the "init" CAD record in Gemini EPICS database.
 *
 * 
 * FUNCTION NAME(S)
 * initCad
 * initChk
 * initCpyParms
 *   
 * DEPENDENCIES
 * Requires EPICS support libraries.
 *
 *
 *INDENT-OFF*
 * $Log: initCad.c,v $
 * Revision 1.2  2009/08/01 04:28:41  mrippa
 * START epicsControl modules were missing
 *
 * Revision 1.1  2009/06/10 15:05:12  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>

/* EPICS specific include files */
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsTasks.h"
#include "epicsCAint.h"
#include "gnirsCC.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h> 
#define MECH_VALS "CC/data/mechanisms.pv"
/* this value will have the current state of the car*/
static carVal = CAR_IDLE;

/* external variables*/
extern int noXYCOM ;
extern int noTempMon ; 
extern int noSenTorr;
/* Forward declarations of the functions in this file */
long initCad( struct cadRecord* pCad );
int initChk( struct cadRecord* pCad );
long initCpyParms( struct cadRecord* pCad );
extern int pvload( char *fname , char *macros);
/*
 *+
 * FUNCTION NAME:
 *	initCad
 *
 * INVOCATION:
 *	struct cadRecord* pCad;
 *	status = initCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	> pCad	(struct cadRecord*)		pointer to CAD record
 *
 * FUNCTION VALUE:
 *	long		status value written to CAD val field
 *
 * PURPOSE:
 *	Provides support for Gemini "init" command, via CAD record.
 *
 * DESCRIPTION:
 *   Called whenever initCad record is processed by EPICS.  It implements
 *   the "init" command, which initializes the parameters for an observing
 *   session.
 *
 *	The CAD record has the following inputs, all of which are strings:
 *
 *	(A)	simMode - level of simulation to use in execution
 *     
 *
 *	This function produces the following outputs in the CAD record:
 *
 *	(VALA)	simMode - amount of simulation to provide (LONG)
 *
 *
 * EXTERNAL VARIABLES:
 *	None
 *
 * PRIOR REQUIREMENTS:
 *   It is assumed that the CAD record has already been initialized, with all
 *   arguments loaded into the record data structure.
 *
 * DEFICIENCIES:
 *
 * HISTORY (optional):
 *	9 Feb 98	Original version created - kjr
 *      5 Jan 99        Removed code which set CAR to IDLE or ERROR - jet
 *   
 *
 *
 *-
 */

long initInitCad( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;
	
	return status;
}
long initCad( struct cadRecord* pCad )
{
    long status = CAD_ACCEPT;	     /* Initialize CAD status value	*/
    long result = OK;		     /* Initialize temp status		*/
  

	/* Take appropriate action, based on directive received	*/
	switch ( DIRECTIVE)
	{
	  case CAD_MARK:		     /* No action required*/
	    LOG_MSG(DEBUG2_MSG, "initCad - MARK directive" );
	    break;
      
	  case CAD_PRESET:		     /* Verify parameters*/
	    LOG_MSG(DEBUG2_MSG, "initCad - PRESET directive" );
	    result = initChk( pCad );
	   
      
	    if ( result != OK )
	    {
			printf("error in initChk\n");
			status = CAD_REJECT;
	    }
		else
		{
			strcpy (MESSAGE,"");
			printf("initChk OK\n");
			status = CAD_ACCEPT;
		}
     
	    break;
      
	  case CAD_CLEAR:		     /* Nothing to be done*/
	    LOG_MSG(DEBUG2_MSG, "initCad - CLEAR directive" );
	    break;
      
	  case CAD_START:	     /* Give semaphore to support task*/
	    LOG_MSG(DEBUG2_MSG, "initCad - START directive" );
     
	   
	    LOG_MSG(DEBUG2_MSG, "initCad - after initCpyParams" );
#if 0
	    /* check observation state*/
	    if (init == 0)
	    {
			init = 1;
			obsState = OBSERVATION_NOT_IN_PROGRESS;	 
	    }
	    else
	    {
			result = getDbInfoT(dbTop, OBSERVING ".VAL", MESSAGE, 
								DBF_ENUM,&obsState);
	    }


	    /* reject if currently observing*/
	    if((result == OK)&&(obsState == OBSERVATION_IN_PROGRESS))
	    {
			strncpy(MESSAGE,"Observe already in progress\n",MAX_STRING_SIZE - 1);
			status = CAD_REJECT;
	    }
	    else if(result != OK)
			status = CAD_REJECT;
#endif
	 	if(strcmp(pCad->h,"BUSY")== 0)
		{
		 	strncpy(MESSAGE,"Components controller is busy\n",MAX_STRING_SIZE - 1);
			status = CAD_REJECT;
		}
	    else
	    { 
			carVal = CAR_BUSY;
			/*valh is connected to car record*/
			*(long *)pCad->valh = carVal;
			result = initCpyParms( pCad ); 
			/* set car to busy and start initCtrl process*/
			LOG_MSG(DEBUG2_MSG, "initCad - set car" );
		
		    LOG_MSG(DEBUG2_MSG, "initCad - start initctrl" );

		    semGive( semInit );

	    }
	    break;
      
	  case CAD_STOP:		     /* Really can't stop this	*/
	    LOG_MSG(DEBUG2_MSG, "initCad - STOP directive" );
	    strncpy( MESSAGE, "initCad: Cannot stop",MAX_STRING_SIZE - 1 );
	    status = CAD_REJECT;
      
	    break;
      
	  default:			     /* Unknown directive	*/
	    strncpy( MESSAGE, "initCad: Unrecognized directive" ,MAX_STRING_SIZE - 1);
	    status = CAD_REJECT;
	    break;
	}
 
    

	return status;
}

/*
 *+
 * FUNCTION NAME:
 *   initChk
 *
 * INVOCATION:
 *   struct cadRecord* pCad;
 *   status = initChk( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *   > pCad    (struct cadRecord*)      pointer to CAD record
 *
 * FUNCTION VALUE:
 *   int       Result of checking variables - OK if all good.
 *
 * PURPOSE:
 *   Test to see if all parameters to initCad are legal.
 *
 * DESCRIPTION:
 *   Called by initCad() to verify parameters before copying to output
 *   of CAD record.
 *
 * EXTERNAL VARIABLES:
 *   None
 *
 * PRIOR REQUIREMENTS:
 *   It is assumed that the CAD record has already been initialized, with all
 *   arguments loaded into the record data structure.
 *
 * DEFICIENCIES:
 *   
 *
 * HISTORY (optional):
 *   9 Feb 98 Created by Ken Ramey, using ideas "borrowed" from GNAAC
 *
 *
 *-
 */

int initChk( struct cadRecord* pCad )
{
	int retVal = OK;
	char msg[100];

	LOG_MSG(DEBUG2_MSG,"\n\nRoutine: initChk **********\n");
 
  
	/* First parameter is simulation mode (NONE/VSM/FAST/FULL)		*/
	sprintf(msg,"Requested sim mode = %s", cadInput(SIM));
	LOG_MSG(DEBUG2_MSG, msg);
/* 	printf("hsim = %s, sim = %s\n",cadInput(HSIM),cadInput(SIM)); */
 
	if ((strcmp(cadInput(SIM), "NONE") != 0) &&
		(strcmp(cadInput(SIM), "VSM") != 0) &&
		(strcmp(cadInput(SIM), "FAST") != 0) &&
		(strcmp(cadInput(SIM), "FULL") != 0))
	{
		retVal = ERROR;
		strncpy(MESSAGE, "initCad: Invalid simulation mode."
				,MAX_STRING_SIZE - 1);
		LOG_MSG(DEBUG2_MSG, MESSAGE);
   
	}
	if ((strcmp(cadInput(HSIM), "NONE") != 0) &&
		(strcmp(cadInput(HSIM), "VSM") != 0) &&
		(strcmp(cadInput(HSIM), "FAST") != 0) &&
		(strcmp(cadInput(HSIM), "FULL") != 0))
	{
		retVal = ERROR;
		strncpy(MESSAGE, "initCad: Invalid hardware simulation mode."
				,MAX_STRING_SIZE - 1);
		LOG_MSG(DEBUG2_MSG, MESSAGE);
   
	}
  
 
	LOG_MSG(DEBUG2_MSG,"\nRoutine: initChk end **********\n");
	return retVal;
  
}

/*
 *+
 * FUNCTION NAME:
 *   initCpyParms
 *
 * INVOCATION:
 *   struct cadRecord* pCad;
 *   status = initCpyParms( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *   > pCad    (struct cadRecord*)      pointer to CAD record
 *
 * FUNCTION VALUE:
 *   long      OK if all parameters copied successfully.
 *
 * PURPOSE:
 *   Move verified parameters from input to output, with appropriate
 *   data conversion.
 *
 * DESCRIPTION:
 *   Called by initCad() after parameters are verified.
 *
 * EXTERNAL VARIABLES:
 *   None
 *
 * PRIOR REQUIREMENTS:
 *   It is assumed that the CAD record has already been initialized, with all
 *   arguments loaded into the record data structure and verified to be
 *   correct.
 *
 * DEFICIENCIES:
 *  
 *
 * HISTORY (optional):
 *   9 Feb 98      Created by Ken Ramey, using ideas "borrowed" from GNAAC
 *  25 Jan 99      Added output B to CAD record to support string value
 *                 in the status/alarm database.  Janet Tvedt
 *
 *
 *-
 */

long initCpyParms( struct cadRecord* pCad )
{
	long status = OK;
	enum SIMULATION_MODE simMode;

  
	DPRINT(DPdebug,DEBUG0_MSG,"\n\nRoutine: initCpyParms \n");
 
  
	/* Get simulation mode and convert to enumeration			*/
	LOG_MSG(DEBUG0_MSG,"Setting simulation mode...\n");
	if (strcmp(cadInput(SIM), "NONE") == 0)
		simMode = SIM_NONE;
	else if (strcmp(cadInput(SIM), "VSM") == 0)
		simMode = SIM_VSM;
	else if (strcmp(cadInput(SIM), "FAST") == 0)
		simMode = SIM_FAST;
	else
		simMode = SIM_FULL;

	status = assignVal(type(SIM), &simMode, cadOutput(SIM), MESSAGE);

  
	DPRINT(DPdebug,DEBUG2_MSG,"Leaving initCpyParms...\n");
  
	return status;
  
}
/*
 *+
 * FUNCTION NAME:
 * initCtrl
 *
 * INVOCATION:  spawned by initTasks()
 *  struct cadRecord *pCad;
 *  initCtrl( pCad, 0,0,0,0,0,0,0,0,0);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * >pCad  (struct cadRecord *) Address of associated CAD record
 * others not used but are required by VxWorks for spawned tasks
 *
 * FUNCTION VALUE:
 * int -  except the function never returns
 *
 * PURPOSE:
 * initialize the Leach controller and set epics simulation mode
 *
 * DESCRIPTION:
 *	This routine calls vmeInit which sets up the hardware, and Sets up a 
 *		connection to the dhs if we are using it.  
 *
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 * 10 Feb 98	Initial version - kjr
 *  5 Jan 99    Added code to update DONE and CAR records - Janet Tvedt
 * 19 Jan 99    Added taskDelay() so CAR update can be seen - Janet Tvedt
 * 25 Jan 99    Added state and health record updates
 *  4 Feb 99    Changed task name to {cmd}Ctrl.  Janet Tvedt
 *  4 Feb 99    Removed writes to CAD MESS field.  Janet Tvedt
 *
 *-
 */
/* #define LOG_MSG(a,b) printf(b) */
int initCtrl( int n1, int n2, int n3, int n4, int n5, int n6, int n7,
			  int n8, int n9, int n10 )
{
  char name[80];
  static void *ch;
  long lval;
  long           simMode, status;
  unsigned short usVal=SCCD_DONE;
  char           dummy[40];
  char buf[80];
  char           errMess[MAX_STRING_SIZE];
  struct         cadRecord* pCad;
  pCad = (struct cadRecord *) n1;

  while( 1 )
  {
    status = OK;
    LOG_MSG(DEBUG2_MSG, "Task tinitCtrl sleeping...");

    semTake(semInit, WAIT_FOREVER);
    LOG_MSG(DEBUG2_MSG, "Task tinitCtrl awake...");
    sleep(1,0);	

    if(hwdbg)
       printf("starting PVLOAD of MECHNAMES\n");

    sprintf(buf,"top=%s,sadtop=%s",dbTop,sadTop);
    status = pvload (MECH_VALS,buf);

    /* Find out what simulation mode is active		*/
    simMode = *(long *)cadOutput(SIM);

    if(status == OK)
    {


      switch (simMode)
      {
	case SIM_NONE:           /* Real processing here       */
	  setSimulation(SIM_NONE);
	  DPRINT(DPdebug,DEBUG1_MSG,
	      "Initializing GNIRS CC interface.\n");
	  if ( vmeInit() == OK)
	  {
	    DPRINT(DPdebug,DEBUG2_MSG,"Initialized GNIRS CC Interface.\n");
	    usVal = SCCD_DONE;
	  }
	  else
	  {
	    usVal = ERROR_VMEINIT;
	    strcpy(errMess,"vmeInit error\n");

	  }	      

	  LOG_MSG(DEBUG1_MSG,
	      "Initializing GNIRS CC interface.\n");

	  break;

	case SIM_FAST:

	  DPRINT(DPdebug,DEBUG2_MSG,"initCtrl simFast\n");
	  setSimulation(SIM_FAST);
	  noXYCOM = 1;
	  noTempMon = 1;
	  noSenTorr = 1;
	  if ( vmeInit() == OK)
	  {
	    DPRINT(DPdebug,DEBUG2_MSG,"Initialized GNIRS CC Interface.\n");
	    usVal = SCCD_DONE;
	  }
	  else
	  {
	    usVal = ERROR_VMEINIT;
	    strcpy(errMess,"vmeInit error\n");

	  }	     

	  break;
	case SIM_FULL:
	  DPRINT(DPdebug,DEBUG2_MSG,"initCtrl simFull\n");
	  setSimulation(SIM_FULL);
	  noXYCOM = 1;
	  noTempMon = 1;
	  noSenTorr = 1;
	  if ( vmeInit() == OK)
	  {
	    DPRINT(DPdebug,DEBUG2_MSG,"Initialized GNIRS CC Interface.\n");
	    usVal = SCCD_DONE;
	  }
	  else
	  {
	    usVal = ERROR_VMEINIT;
	    strcpy(errMess,"vmeInit error\n");

	  }	     
	  break;
	case SIM_VSM:
	  DPRINT(DPdebug,DEBUG2_MSG,"initCtrl simVsm\n");
	  setSimulation(SIM_VSM);
	  noXYCOM = 1;
	  noTempMon = 1;
	  noSenTorr = 1;	
	  if ( vmeInit() == OK)
	  {
	    DPRINT(DPdebug,DEBUG2_MSG,"Initialized GNIRS CC Interface.\n");
	    usVal = SCCD_DONE;
	  }
	  else
	  {
	    usVal = ERROR_VMEINIT;
	    strcpy(errMess,"vmeInit error\n");

	  }	     
#if 0
	  updateState("INITIALIZING");
#endif
	  DPRINT(DPdebug,DEBUG2_MSG,"Doing nothing...\n");
	  taskDelay(sysClkRateGet());  /* Added delay so toggling of CAR can be seen */
#if 0
	  usVal = SCCD_DONE; 
	  if(updateState("RUNNING") != OK)
	  {
	    LOG_MSG(ERROR_MSG,"Error setting controller state");
	  }
	  updateHealth("GOOD");
#endif
	  break;

	default:		/* Don't know what to do       */
	  strncpy(errMess,"Can't determine sim mode.\n",MAX_STRING_SIZE - 1);
	  DPRINT(DPdebug,DEBUG2_MSG,errMess);
	  usVal = ERROR_SIM;
	  break;
      }


    }	

    if(usVal == SCCD_DONE)
    {
      lval = 1;
      sprintf(name,"%s%s.VAL",sadTop,INIT_DONE);
      if(putEpics(name,  &lval,&ch) != OK)
	LOG_MSG(ERROR_MSG,"error setting initialized\n");
      /* Set CAR record to IDLE or ERROR depending on init DONE status */
      carVal = CAR_IDLE;
      if(setCar(INIT_CAR,CAR_IDLE,OK,"",dummy)!= OK)
	LOG_MSG(ERROR_MSG,dummy);
      /* Update DONE record */

    }
    else
    {
      carVal = CAR_ERROR;
      if(setCar(INIT_CAR,CAR_ERROR,(long)usVal,errMess,dummy)!= OK)
	LOG_MSG(ERROR_MSG,dummy);
      LOG_MSG(ERROR_MSG,errMess);


    }

  }  /* End of while(1) */
}

