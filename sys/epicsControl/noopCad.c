static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: noopCad.c,v 1.1 2009/06/10 15:05:13 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * noopCad.c
 *
 * DESCRIPTION
 * This file contains a function which essentially ignores the folloing
 * commands:
 *              datum
 *              guide
 *              endGuide
 *              verify
 *              endVerify
 *              endObserve 
 *
 * FUNCTION NAME(S)
 * noopCad - subroutine for all commands considered noops
 * 
 * DEPENDENCIES		
 * The name of the subroutine in this file should be identical to those
 * declared in the SNAM field of each CAD record considered to be a noop.
 *
 *INDENT-OFF*
 * $Log: noopCad.c,v $
 * Revision 1.1  2009/06/10 15:05:13  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */
#include <car.h>
#include "epCommon.h"
#include "epicsNames.h"

/*
 *+
 * FUNCTION NAME:
 * noopCad
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = noopCad( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for noop CAD records
 *
 * DESCRIPTION:
 * This routine is called whenever a noop CAD record is processed.
 * On a START directive, this function forces processing of the
 * related car record. 
 *
 * The function always accepts the command unless there is an error
 * on the START.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized.
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 20-Jan-1999  Original version.     Janet Tvedt
 *
 *-
 */

long noopCad( struct cadRecord *pCad ) 
{
   long status = CAD_ACCEPT;
   char rec[80];
   char name[80];

   /* Create name of record to process car from name of this record*/
   strcpy(rec,pCad->name);
   strtok(rec,":");
   strtok(NULL,":");
   strcpy(name,strtok(NULL,":"));
   strcat(name,"Toggle");
   strcat(name,0);

   LOG_MSG(DEBUG2_MSG,"\n\nRoutine: noopCad \n");
   if(DIRECTIVE == CAD_START)
   {
#if 0
       if(setCar(INIT_CAR,CAR_BUSY,OK,"",dummy)!= OK)
		DPRINT(DPdebug,ERROR_MSG,dummy);
       /* process record, interface record sets car to busy and then idle after
	a few seconds*/
    /*    if(processRec(dbTop, name) != OK) */
/*       { */
/*          strncpy(MESSAGE, "Error toggling noopC",MAX_STRING_SIZE - 1); */
/* 	 LOG_MSG(ERROR_MSG, pCad->mess); */
/* 	 status = CAD_REJECT; */
/*       } */
   /*     setCar(NOOP_CAR,CAR_BUSY,OK,"",MESSAGE); */
/*        setCar(NOOP_CAR,CAR_IDLE,OK,"",MESSAGE); */
        if(setCar(INIT_CAR,CAR_IDLE,OK,"",dummy)!= OK)
		DPRINT(DPdebug,ERROR_MSG,dummy);
#endif
       
   }
   else if (DIRECTIVE == CAD_STOP)
   {
       strncpy(MESSAGE, "Cannot stop",MAX_STRING_SIZE - 1);
       LOG_MSG(ERROR_MSG, MESSAGE);
       status = CAD_REJECT;
   }
   return status;

}


