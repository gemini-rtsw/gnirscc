static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: initTasks.c,v 1.2 2013/06/06 01:54:28 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in 
 *	Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * initTasks.c
 *
 * DESCRIPTION 
 * This file contains the functions used for spawning VxWorks tasks. 
 *	It also contains the VxWorks control tasks for the three
 *	individual top level commands.
 * 
 * FUNCTION NAME(S)
 * initTasks - starts up all the control tasks
 * initTask  - spawns a single task
 * setHdrVars - sets variables in the data header (currently not used)
 *   
 * DEPENDENCIES
 * initTasks must be called from the IOC startup script to ensure that
 * all the tasks have been spawned and are waiting to be activated.
 *
 *INDENT-OFF*
 * $Log: initTasks.c,v $
 * Revision 1.2  2013/06/06 01:54:28  gemvx
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
 * Revision 1.1  2009/06/10 15:05:12  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>

/* Controller specific include files */
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsTasks.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>
#include "epicsCAint.h"

/* Forward declarations of the functions used in this file 	*/
int initTask( SEM_ID *semId, SEM_B_STATE semState, char *recTop, char *recName,int priority, int options, int stack,  FUNCPTR func);
long setHdrVars(struct cadRecord* cadPtr, long *errNo, char* errText);

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
 * This function should be called from the IOC startup script.  It 
 *	calls initTask for every control task that needs to be	
 *	spawned.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed that this function is invoked from the VxWorks 
 *	startup script
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 *	15 Jan 98    Original version by Ken Ramey
 *
 *      25 Jan 99    Changed task names to (cmd)Ctrl to match the 
 *			documentation in the CapFast diagrams.     
 *						Janet Tvedt
 *
 *      28 Jan 99    Added initialization of more EPICS variables.
 *						  Janet Tvedt
 *
 *      3  Feb 99    Changed task names to {cmd}Ctrl.  Janet Tvedt
 *
 *-
 */

int initTasks(void)
{
	char errMsg[80];
    unsigned short usVal;
	long taskId;
    /* Initialize indicator variables in EPICS   */
    usVal = SCCD_UNKNOWN;
   

	/* Start command tasks						*/

    DPRINT(DPdebug,DEBUG0_MSG,"dcSetup started.\n");
    if(initTask(&semInit,SEM_EMPTY,dbTop,INIT_CAD,50,VX_FP_TASK,6000,
	     initCtrl) == ERROR)
	
   {
       updateHealth("BAD");
       return ERROR;
   }
    DPRINT(DPdebug,DEBUG0_MSG,"init started.\n");
 
 
    if(initTask(&semTest,SEM_EMPTY,dbTop,TEST_CAD,50,
	     VX_FP_TASK, 6000,testCtrl) == ERROR)
   {
       updateHealth("BAD");
       return ERROR;
   }

  
   if( initTask(&semReboot,SEM_EMPTY,dbTop,REBOOT_CAD,50,
	     VX_FP_TASK, 6000,rebootCtrl) == ERROR)
   {
       updateHealth("BAD");
       return ERROR;
   }
   /*park*/
   if( initTask(&semPark,SEM_EMPTY,dbTop,PARK_CAD,50,
	     VX_FP_TASK, 6000,parkCtrl) == ERROR)
   {
       updateHealth("BAD");
       return ERROR;
   }
   /*datum*/
   if( initTask(&semDatm,SEM_EMPTY,dbTop,DATUM_CAD,50,
	     VX_FP_TASK, 6000,datumCtrl) == ERROR)
   {
       updateHealth("BAD");
       return ERROR;
   }
   /*diagnose*/ 
   if( initTask(&semDiagnose,SEM_EMPTY,dbTop,DIAGNOSE_CAD,50,
	     VX_FP_TASK, 6000,diagnoseCtrl) == ERROR)
   {
       updateHealth("BAD");
       return ERROR;
   }
   if( initTask(&semDebug,SEM_EMPTY,dbTop,DEBUG_CAD,50,
	     VX_FP_TASK, 6000,debugCtrl) == ERROR)
   {
       updateHealth("BAD");
       return ERROR;
   } 
   taskId = taskSpawn("tHealthScan",50,VX_FP_TASK, 6000,healthScan,NULL, 0,0,0,0,0,0,0,0,0);
   if (taskId == ERROR)
   {
	   sprintf(errMsg, "Unable to spawn %s\n", "tHealthScan");
	   DPRINT(DPdebug,0,errMsg);
	   updateHealth("BAD");
       return ERROR;
   }
   
   
   return OK;
   
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
 * status = initTask( semId, semState, recTop, recName, priority, 
 *	options, stack, func)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  semId	(SEM_ID)      pointer to the semaphore ID
 *  semState	(SEM_B_STATE) the creation state of the binary 
 *				semaphore
 *  recTop	(char *)      pointer to the db prefix
 *  recName	(char *)      pointer to the CAD record name 
 *				associated with 
 *                            the task
 *  priority    (int)	      priority to assign to the task
 *  options	(int)	      options to give to send to taskSpawn
 *  stack       (int)	      stack size to allow for the task
 *  func	(FUNCPTR)     pointer to the starting address of 
 *				the task
 *
 * FUNCTION VALUE:
 * int	- the ID of the created task
 *
 * PURPOSE:
 * To spawn a VxWorks control task
 *
 * DESCRIPTION:
 * This function creates a binary semaphore for the task, determines
 *     	the address of its associated CAD record.  The CAD address 
 *	is passed as the first (and only used) paramater of the 
 *	control task.  This function then spawns the task using the 
 *	supplied input parameters.
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
 * 20-Jan-1998  Original version.    Ken Ramey - adapted from JT's
 *
 *-
 */


 SEM_ID *abvc;

int initTask ( SEM_ID  *semId, SEM_B_STATE semState, char *recTop, char *recName,int priority, int options, int stack,  FUNCPTR func)
{
  char   taskName[MAX_STRING_SIZE];
  char   dbRec[MAX_STRING_SIZE];
  char   errMsg[80];
  struct dbAddr addr;
  long   ret;
  int    taskId;
  int    iAddr;
  
  /* Create a semaphore for the task */
  *semId = semBCreate(SEM_Q_FIFO,semState);
  if (semId == NULL)
    {
      DPRINT(DPdebug,0,"Error creating semaphore.\n");
      taskId = ERROR;
      return taskId;
    }
  
  /* Get the address of the data structure containing command
   *	 arguments */
  sprintf(dbRec,"%s%s.OUTA",recTop,recName);
  ret=dbNameToAddr (dbRec,&addr);
 
  
  /* If successful, then create a task name and spawn it */
  if(ret == 0)
    {
  iAddr = (int) addr.precord;
      sprintf(taskName,"%c%s",'t',recName);
      taskId = taskSpawn(taskName,priority,options,stack,func,iAddr,
			 0,0,0,0,0,0,0,0,0);
      if (taskId == ERROR)
	{
	    
	    semDelete(*semId);
	    semId = NULL;
	    sprintf(errMsg, "Unable to spawn %s\n", taskName);
	    DPRINT(DPdebug,0,errMsg);
	}
    }
  
  /* Otherwise, print an error message and return ERROR */
  else
    {
	semDelete(*semId);
	DPRINT(DPdebug,0,"Unable to access record\n");
	cicsLogLong(0, "failed wit	h dbNameToAddr error = %ld", ret);
	cicsLogString(0, "dbName = ", dbRec);
	taskId = ERROR;
    }
  
  return taskId;
  
}



