static char rcsid[]="$Id: cicsMiscSub.c,v 1.2 2009/05/27 19:34:55 fkraemer Exp $";

/*
 *   FILENAME
 *   -------- 
 *   cicsMiscSub.c
 *
 *   PURPOSE
 *   -------
 *   This file contains the source for all the miscellaneous functions
 *   called by the CICS through subroutine and genSub records.
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *   cicsNullInit - Null initialization function for subroutine records.
 *   cicsReboot   - Reboot the CICS.
 *
 *   DEPENDENCIES
 *   ------------
 *
 *   LIMITATIONS
 *   -----------
 *
 *   AUTHOR
 *   ------
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   HISTORY
 *   -------
 *INDENT-OFF*
 *
 * $Log: cicsMiscSub.c,v $
 * Revision 1.2  2009/05/27 19:34:55  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.3  1999/11/14 02:05:40  yamada
 * Reformatted logs.
 *
 *
 *INDENT-ON*
 *
 *   21-Mar-1997: Original version.                                (smb)
 *   14-Apr-1997: Call to reboot() commented out, since it didn't
 *                work and resulted in load errors.                (smb)
 *   17-Apr-1997: cicsNullInit added.                              (smb)
 */

#include  <vxWorks.h>
#include  <types.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>

#include  <dbDefs.h>
#include  <genSubRecord.h>
#include  <subRecord.h>
#include  <dbCommon.h>
#include  <recSup.h>

#include  <cicsConst.h>
#include  <cicsLib.h>

/* NOTE: Where can I get this definition from ? */

/* void reboot(); */

/* ===================================================================== */


/*+
 *   Function name:
 *   cicsNullInit
 *
 *   Purpose:
 *   Null initialization function for subroutine records.
 *
 *   Purpose:
 *   Error messages wil be generated if a subroutine record does not have
 *   an initialization function. This function exists as a placeholder
 *   when no initialization is required.
 *
 *   Invocation:
 *   status = cicsNullInit( struct subRecord *psub );
 *
 *   Parameters in:
 * 
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *      cicsLogMessage   CICS function for cicsLogging a message.
 *      reboot           VxWorks function for rebooting the IOC
 *
 *      External variables:
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   17-Apr-1997: Original version.                  (smb)
 *-
 */

long cicsNullInit ( struct subRecord *psub )
{

    return 0;
}


/* ===================================================================== */


/*+
 *   Function name:
 *   cicsReboot
 *
 *   Purpose:
 *   Reboot the IOC.
 *
 *   Purpose:
 *   This routine is called when a "reboot" command has been confirmed
 *   and does whatever is necessary to reboot the IOC.
 *
 *   Invocation:
 *   status = cicsReboot( struct subRecord *psub );
 *
 *   Parameters in:
 * 
 *   Parameters out:
 *
 *   Return value:
 *      This is a void function. It has no status value.
 *
 *   Globals:
 *      External functions:
 *      cicsLogMessage   CICS function for cicsLogging a message.
 *      reboot           VxWorks function for rebooting the IOC
 *
 *      External variables:
 *
 *   Requirements:
 *
 *   Limitations:
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   21-Mar-1997: Original version.                  (smb)
 *-
 */

long cicsReboot ( struct subRecord *psub )
{
    long status;

    status = 0;

    cicsLogMessage( CICS_DB_ERROR,
      "\n\n    *** REBOOTING (doesn't work) ***\n\n" );

/*    reboot(); */

    return status;
}
