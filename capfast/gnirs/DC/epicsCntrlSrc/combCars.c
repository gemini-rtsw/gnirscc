static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: combCars.c,v 1.2 2009/05/27 19:32:20 fkraemer Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * combCars.c
 *
 * FUNCTION NAME(S)
 * combCars - function for hierachically combining CAR status values
 * 
 *
 *INDENT-OFF*
 * $Log: combCars.c,v $
 * Revision 1.2  2009/05/27 19:32:20  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:50  buchholz
 * Imported gnaacSrc into CVS
 *
 *INDENT-ON* 
 */


#include <epCommon.h>
#include <genSubRecord.h>


/*
 *+
 * FUNCTION NAME:
 * combCars
 *
 * INVOCATION:
 * struct genSubRecord *pGenSub;
 * long status;
 *
 * status = combCars( *pGenSub )
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pGenSub   (struct genSubRecord *)   pointer to genSub data structure
 *
 * FUNCTION VALUE:
 * long  Status value, 0 for success
 *
 * PURPOSE:
 * User defined function for several genSub records used for hierarchically
 * combining CAR status values.
 *
 * DESCRIPTION:
 * This routine is called whenever the appropriate genSub records are processed.
 * It takes the CAR VAL, OERR and OMSS fields of three CAR records.  The highest
 * CAR VAL is determined.  The highest CAR value is placed on the genSub output 
 * along with its error code (OERR) and error message (OMSS) values.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * The genSub record needing this routine must have its SNAM property set to
 * combCars. 
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 * 17-Apr-1997  Original version.				  J.E. Tvedt
 *
 *-
 */


long combCars( struct genSubRecord *pGenSub )
{
    long ival;
    long tmp;
    long ierr;
    char imss[MAX_STRING_SIZE];
    char errMess[MAX_STRING_SIZE];
    long status;


    /* Assume the first CAR has the maximum value */
    ival = *(long *) pGenSub->a;
    ierr = *(long *) pGenSub->b;
    strcpy(imss,pGenSub->c);

    /* If there is a second CAR attached to the genSub, compare
       it to the first.  If it is larger, make it the maximum.
    */
    if(pGenSub->d != NULL)
    {
	tmp = *(long *) pGenSub->d;
	if(tmp > ival) 
	{
	    ival = tmp;
	    ierr = *(long *) pGenSub->e;
	    strcpy(imss,pGenSub->f);
	}

    }

    /* If there is a third CAR attached to the genSub, compare it
       to the current maximum.  If it is larger, make it the maximum.
    */
    if(pGenSub->g != NULL)
    {
	tmp = *(long *) pGenSub->g;
	if(tmp > ival) 
	{
	    ival = tmp;
	    ierr = *(long *) pGenSub->h;
	    strcpy(imss,pGenSub->i);
	}
    }

    /* Put the maximum CAR status and its error message and error code
       on the outputs of the genSub
    */
    status = assignVal(pGenSub->fta,&ival,pGenSub->vala,errMess);
    if(status == OK)
	status = assignVal(pGenSub->ftc,imss,pGenSub->valc,errMess);
    if(status == OK)
	status = assignVal(pGenSub->ftb,&ierr,pGenSub->valb,errMess);
    
    /* Print an error message if unsuccessful */
    if(status != OK)
    {
	cicsLogMessage(2,"Error combining CAR record status");
	cicsLogMessage(2,errMess);
    }
 

    return status;
}

