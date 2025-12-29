static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: cicsLib2.c,v 1.1 2009/06/10 15:05:09 gemvx Exp $"
};
/*
*   FILENAME
*   -------- 
*   cicsLib2.c
*
*   PURPOSE
*   -------
*   This file contains the source for all the channel access functions used
*   by the Core Instrument Control System
*
*
*   FUNCTION NAME(S)
*   ----------------
*   cicsCaGet         - Get a value from an EPICS field using channel access
*   cicsCaPut         - Put a value to an EPICS field using channel access
*
*   DEPENDENCIES
*   ------------
*
*   LIMITATIONS
*   ------------
*
*   AUTHOR
*   ------
*   Steven Beard  (smb@roe.ac.uk)
*   Janet Tvedt   (tvedt@noao.edu)
*
*   HISTORY
*   -------
*   04-Dec-1996: Original version, split off from cicsLib.c because
*                it was not possible to include "dbAccess.h" and
*                "cadefs.h" in the same file.                         (smb)
*/
/* *INDENT-OFF* */
/*
 * $Log: cicsLib2.c,v $
 * Revision 1.1  2009/06/10 15:05:09  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 */
/* *INDENT-ON* */


/* Global Constants */

#include  <vxWorks.h>
#include  <types.h>
#include  <math.h>
#include  <time.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>

#include  <cadef.h>

#include  <cicsConst.h>
#include  <cicsLib.h>


/* Global Variables */

/* ===================================================================== */


/*
 *+
 * FUNCTION NAME:
 * cicsCaGet
 *
 * INVOCATION:
 * long status;
 * char *fieldName;  
 * char *errMess;
 * unsigned short type;
 * double *outVal;
 *
 * status = cicsCaGet(fieldName, errMess, type, &outVal);
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > fieldName (char *)               pointer to reocord_name.field_name
 * ! errMess   (char *)               pointer to string
 * > type      (unsigned short)       data type to convert output to 
 *                                    (DBF_DOUBLE, DBF_LONG, etc.)
 * < outval    (void *)               pointer to output value
 *
 * FUNCTION VALUE:
 * long  - status value returned to calling routine, a non-zero value
 *         indicates an error
 *
 * PURPOSE:
 * Function to get a value from an EPICS field by database access.
 *
 * DESCRIPTION:
 * This routine is called to obtain the value of the specified field 
 * within an EPICS database record.  It handles potential errors by
 * sending messages to the CICS logging functions and returning an
 * error emssage and status value to the calling routine.
 *
 * EXTERNAL VARIABLES:
 * ca_search       - EPICS channel access routine for finding channel
 * ca_get          - EPICS channel access routine for retrieving the data
 * cicsLogMessage  - CICS logging function for a message
 * cicsLogString   - CICS logging function for a message + a string
 *
 * PRIOR REQUIREMENTS:
 * None.
 * 
 * DEFICIENCIES:
 * The function takes ages (several seconds) to execute. I suspect I
 * am not using ca_pend_io and ca_pend_event correctly. When I try
 * reduce the size of the arguments to the "pend" functions this function
 * hangs up forever. I don't understand what is going on, and cannot
 * find any suitable documentation.
 *
 * HISTORY (optional):
 * 13-Mar-1997  Original version as getDbInfo.			Janet Tvedt
 * 17-Jun-1997  Imported into cicsLib.				Steven Beard
 * 26-Jun-1997  Channel access version.                         Steven Beard
 *-
 */

long cicsCaGet(char *fieldName, char *errMess, unsigned short type, void *outVal)
{
    chid   chanId;
    long   ret;
    long   status;


    status = PASS;

/* Search for the named field and return a channel ID */

    ret = ca_search( fieldName, &chanId );
    SEVCHK(ret,NULL);
    ret = ca_pend_io(1.0);

    if ( ret == ECA_NORMAL )
    {

/* Read the value from the field. */

        ret = ca_get( type, chanId, outVal );
        SEVCHK(ret,NULL);
        ca_pend_event(2.0);

    }
    else
    {
	status = FAIL;
	sprintf(errMess, "Channel not found error = %ld", ret);
	cicsLogMessage( CICS_DB_ERROR, errMess);
	cicsLogString( CICS_DB_MIN, "Channel = ", fieldName);
    }

    /* Return error status */
    return status;
}

/* ===================================================================== */


/*
 *+
 * FUNCTION NAME:
 * cicsCaPut
 *
 * INVOCATION:
 * char *fieldName;
 * char *errMess;
 * unsigned short type;
 * double outVal;
 * long status;
 *
 * status = cicsCaPut(fieldName, errMess, DBF_DOUBLE, &outVal)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * > fieldName (char *)           pointer to record+field name
 * ! errMess   (char *)           pointer to string
 * > type      (unsigned short)   data type of value to put
 * > outVal    (void *)           pointer of data to put
 *
 * FUNCTION VALUE:
 * long  Status value returned to calling routine, a non-zero value indicates
 *       an error
 *
 * PURPOSE:
 * Function to put a value into an EPICS field by database access
 *
 * DESCRIPTION:
 * This routine may be called by a user subroutine to put a value
 * into an EPICS databaser record field.  The complete field name must
 * be specified (for example: sytem:subsystem:recordx.FIELDY).  This function
 * will also handle errors by logging them through the CICS logging functions
 * and by returning an error message and status value to the calling routine.
 *
 * EXTERNAL VARIABLES:
 * ca_search       - EPICS channel access routine for finding channel
 * ca_put          - EPICS channel access routine for putting the data
 * cicsLogMessage  - CICS logging function for a message
 * cicsLogString   - CICS logging function for a message + a string
 *
 * PRIOR REQUIREMENTS:
 * None
 *
 * DEFICIENCIES:
 * The function takes ages (several seconds) to execute. I suspect I
 * am not using ca_pend_io and ca_pend_event correctly. When I try
 * reduce the size of the arguments to the "pend" functions this function
 * hangs up forever. I don't understand what is going on, and cannot
 * find any suitable documentation.
 *
 * HISTORY (optional):
 * 19-Mar-1997  Original version as putDbInfo.			Janet Tvedt
 * 17-Jun-1997  Imported into cicsLib				Steven Beard
 * 26-Jun-1997  Channel access version.                         Steven Beard
 *-
 */


long cicsCaPut(char *fieldName, char *errMess, unsigned short type, void *outVal)
{
    chid   chanId;
    long   ret;
    long   status;


    status = PASS;

/* Search for the named field and return a channel ID */

    ret = ca_search( fieldName, &chanId );
    SEVCHK(ret,NULL);
    ret = ca_pend_io(1.0);

    if ( ret == ECA_NORMAL )
    {

/* Write the value to the field. */

        ret = ca_put( type, chanId, outVal );
        SEVCHK(ret,NULL);
        ca_pend_event(2.0);

    }
    else
    {
	status = FAIL;
	sprintf(errMess, "Channel not found error = %ld", ret);
	cicsLogMessage( CICS_DB_ERROR, errMess);
	cicsLogString( CICS_DB_MIN, "Channel = ", fieldName);
    }

    /* Return error status */
    return status;
}
