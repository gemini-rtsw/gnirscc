static char rcsid[]="$Id: cicsCarHealth.c,v 1.2 2009/05/27 19:34:55 fkraemer Exp $";

/*
 *   FILENAME
 *   -------- 
 *   cicsCarHealth.c
 *
 *   PURPOSE
 *   -------
 *   This file contains the source for all the functions used to
 *   combine together multiple CAR events or multiple health
 *   values through "genSub" records.
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *   cicsCarValCombine  - Combine together multiple CAR states and client IDs.
 *   cicsHealthCombine  - Combine together multiple health values.
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
 * $Log: cicsCarHealth.c,v $
 * Revision 1.2  2009/05/27 19:34:55  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.3  1999/11/14 02:05:38  yamada
 * Reformatted logs.
 *
 *
 *INDENT-ON*
 *
 *   24-Jan-1997: Original version based on tcsCarCombine.         (smb)
 */

#include  <vxWorks.h>
#include  <types.h>
#include  <stdlib.h>
#include  <stdioLib.h>
#include  <string.h>

#include  <dbDefs.h>
#include  <genSubRecord.h>
#include  <car.h>
#include  <dbCommon.h>
#include  <recSup.h>

#include  <cicsConst.h>
#include  <cicsLib.h>


/* ===================================================================== */

/*+
 *   Function name:
 *    cicsCarValCombine -  Combine together multiple CAR states.
 *
 *   Purpose:
 *   This function reads the states of up to five CAR records
 *   and generates an event based on those states.
 *
 *   - An IDLE event is generated when all CAR records are idle.
 *   - A PAUSED event is generated when one of the CAR records is PAUSED
 *     and all the others are IDLE.
 *   - A BUSY event is generated when any of the CAR records is BUSY
 *     but none are in the ERROR state.
 *   - An ERROR event is generated when any of the CAR records are in
 *     the ERROR state (regardless of the other states).
 *
 *   The client ID associated with the winning CAR record is passed to
 *   the output.
 *
 *   The function is designed to be used with the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = cicsCarValCombine( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    long     Event delivered to CAR record 1
 *      > pgensub->b    long     Client ID of CAR record 1
 *      > pgensub->c    long     Event delivered to CAR record 2
 *      > pgensub->d    long     Client ID of CAR record 2
 *      > pgensub->e    long     Event delivered to CAR record 3
 *      > pgensub->f    long     Client ID of CAR record 3
 *      > pgensub->g    long     Event delivered to CAR record 4
 *      > pgensub->h    long     Client ID of CAR record 4
 *      > pgensub->i    long     Event delivered to CAR record 5
 *      > pgensub->j    long     Client ID of CAR record 5
 *
 *   Parameters out:
 *      < pgensub->vala long     Resulting CAR event
 *      < pgensub->valb long     Winning client ID
 *      < pgensub->valc long     Index of winning CAR event (1-5)
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *   This function works best if the inputs come from CAR events and not
 *   CAR states. This means connecting the A,C,E,G and I inputs of the
 *   genSub record to the .IVAL fields of the individual CAR records
 *   and not to the .VAL fields
 *
 *   Limitations:
 *   This function does not deal with CAR error status values and messages.
 *   However, the index output to VALC can be used to extract any error
 *   status and message from the winning CAR record by using a "selection"
 *   record.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   24-Jan-1997: Original version, based on tcsCarValCombine,
 *                bit with client ID included.                     (smb)
 *   25-Jan-1997: Generate index of winning CAR event.             (smb)
 *-
 */

long cicsCarValCombine( struct genSubRecord *pgensub ) 
{
  long status;         /* return status */
  long outVal;         /* Output CAR event value */
  long outClid;        /* Output client ID */
  long outIndex;       /* Index of winning CAR event */

/* Initialise the status */

  status = PASS;

/*
 * The default output value is IDLE and the default client ID is the maximum
 * of the input client IDs. The default index is whichever of the input CAR
 * CAR records has the largest client ID.
 */

  outVal = CAR_IDLE;

  outClid = *(long *)pgensub->b;
  outIndex = 1;

  if ( *(long *)pgensub->d > outClid )
  {
    outClid = *(long *)pgensub->d;
    outIndex = 2;
  }

  if ( *(long *)pgensub->f > outClid )
  {
    outClid = *(long *)pgensub->f;
    outIndex = 3;
  }

  if ( *(long *)pgensub->h > outClid )
  {
    outClid = *(long *)pgensub->h;
    outIndex = 4;
  }

  if ( *(long *)pgensub->j > outClid )
  {
    outClid = *(long *)pgensub->j;
    outIndex = 5;
  }

/*
 * If any CAR is PAUSED this will override the IDLE state and result in a
 * PAUSED output value.
 */

  if ( *(long *)pgensub->a == CAR_PAUSED )
  {
    outVal = CAR_PAUSED;
    outClid = *(long *)pgensub->b;
    outIndex = 1;
  }

  if ( *(long *)pgensub->c == CAR_PAUSED )
  {
    outVal = CAR_PAUSED;
    outClid = *(long *)pgensub->d;
    outIndex = 2;
  }

  if ( *(long *)pgensub->e == CAR_PAUSED )
  {
    outVal = CAR_PAUSED;
    outClid = *(long *)pgensub->f;
    outIndex = 3;
  }

  if ( *(long *)pgensub->g == CAR_PAUSED )
  {
    outVal = CAR_PAUSED;
    outClid = *(long *)pgensub->h;
    outIndex = 4;
  }

  if ( *(long *)pgensub->i == CAR_PAUSED )
  {
    outVal = CAR_PAUSED;
    outClid = *(long *)pgensub->j;
    outIndex = 5;
  }

/*
 * If any CAR is BUSY this will override the IDLE or PAUSED states and result
 * in a BUSY output value.
 */

  if ( *(long *)pgensub->a == CAR_BUSY )
  {
    outVal = CAR_BUSY;
    outClid = *(long *)pgensub->b;
    outIndex = 1;
  }

  if ( *(long *)pgensub->c == CAR_BUSY )
  {
    outVal = CAR_BUSY;
    outClid = *(long *)pgensub->d;
    outIndex = 2;
  }

  if ( *(long *)pgensub->e == CAR_BUSY )
  {
    outVal = CAR_BUSY;
    outClid = *(long *)pgensub->f;
    outIndex = 3;
  }

  if ( *(long *)pgensub->g == CAR_BUSY )
  {
    outVal = CAR_BUSY;
    outClid = *(long *)pgensub->h;
    outIndex = 4;
  }

  if ( *(long *)pgensub->i == CAR_BUSY )
  {
    outVal = CAR_BUSY;
    outClid = *(long *)pgensub->j;
    outIndex = 5;
  }

/*
 * If any CAR is in the ERROR state this will override the IDLE, PAUSED
 * or BUSY states and result in an ERROR output value.
 */

  if ( *(long *)pgensub->a == CAR_ERROR )
  {
    outVal = CAR_ERROR;
    outClid = *(long *)pgensub->b;
    outIndex = 1;
  }

  if ( *(long *)pgensub->c == CAR_ERROR )
  {
    outVal = CAR_ERROR;
    outClid = *(long *)pgensub->d;
    outIndex = 2;
  }

  if ( *(long *)pgensub->e == CAR_ERROR )
  {
    outVal = CAR_ERROR;
    outClid = *(long *)pgensub->f;
    outIndex = 3;
  }

  if ( *(long *)pgensub->g == CAR_ERROR )
  {
    outVal = CAR_ERROR;
    outClid = *(long *)pgensub->h;
    outIndex = 4;
  }

  if ( *(long *)pgensub->i == CAR_ERROR )
  {
    outVal = CAR_ERROR;
    outClid = *(long *)pgensub->j;
    outIndex = 5;
  }

/* Output the resulting value, client ID and index. */

   *(long *)pgensub->vala = outVal;
   *(long *)pgensub->valb = outClid;
   *(long *)pgensub->valc = outIndex;

  return status;
}


/* ===================================================================== */

/*+
 *   Function name:
 *    cicsHealthCombine -  Combine together multiple health states.
 *
 *   Purpose:
 *   This function reads the states of up to five health records
 *   and generates a combined health.
 *
 *   - The result will be GOOD if none of the inputs are WARNING or BAD.
 *   - The result will be WARNING if any of the inputs are WARNING
 *     but none of the inputs are BAD.
 *   - The result will be BAD if any of the inputs are BAD.
 *
 *   The message contained in the winning health record is passed to
 *   the output.
 *
 *   The function is designed to be used with the EPICS "genSub" record.
 *
 *   Invocation:
 *   status = cicsCarValCombine( struct genSubRecord *pgensub );
 *
 *   Parameters in:
 *      > pgensub->a    string*  Value of health record 1
 *      > pgensub->b    string*  Message contained in health record 1
 *      > pgensub->c    string*  Value of health record 2
 *      > pgensub->d    string*  Message contained in health record 2
 *      > pgensub->e    string*  Value of health record 3
 *      > pgensub->f    string*  Message contained in health record 3
 *      > pgensub->g    string*  Value of health record 4
 *      > pgensub->h    string*  Message contained in health record 4
 *      > pgensub->i    string*  Value of health record 5
 *      > pgensub->j    string*  Message contained in health record 5
 *
 *   Parameters out:
 *      < pgensub->vala string*  Resulting health
 *      < pgensub->valb string*  Winning message
 *      < pgensub->valc long     Index of winning health input
 *
 *   Return value:
 *      < status      long        Status value
 *
 *   Globals:
 *      External functions:
 *      None
 *
 *      External variables:
 *      None
 *
 *   Requirements:
 *
 *   Limitations:
 *   If any or all the input health values are silly the result is still
 *   GOOD. Only the strings "WARNING" and "BAD" are recognised.
 *
 *   I expect this function will be very inefficient, since it deals with
 *   lots of character strings. It would be much more efficient to combine
 *   health values as integers by taking the maximum. It should be ok if
 *   the function is executed only when the health changes and is not
 *   executed continuously.
 *
 *   Author:
 *   Steven Beard  (smb@roe.ac.uk)
 *
 *   History:
 *   24-Jan-1997: Original version.                                (smb)
 *   25-Jan-1997: Generate index of winning health input.          (smb)
 *-
 */

long cicsHealthCombine( struct genSubRecord *pgensub ) 
{
  long status;                       /* return status */
  char outHealth[MAX_STRING_SIZE];   /* Output health value */
  char inMess[MAX_STRING_SIZE];      /* Input message */
  char outMess[MAX_STRING_SIZE];     /* Output message */
  long outIndex;                     /* Index of winning health input */

/* Initialise the status */

  status = PASS;

/*
 * The default output value is GOOD and the default message is the first
 * non-null and non-blank message (or a null message if there are no non-null
 * and non-blank messages). The default index corresponds to the message
 * chosen (or is 1 if there are no messages found).
 */

  strcpy( outHealth, "GOOD" );

  strcpy( outMess, "" );

  outIndex = 1;

  strcpy( inMess, (char *)pgensub->j );
  if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
  {
    strcpy( outMess, inMess );
    outIndex = 5;
  }

  strcpy( inMess, (char *)pgensub->h );
  if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
  {
    strcpy( outMess, inMess );
    outIndex = 4;
  }

  strcpy( inMess, (char *)pgensub->f );
  if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
  {
    strcpy( outMess, inMess );
    outIndex = 3;
  }

  strcpy( inMess, (char *)pgensub->d );
  if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
  {
    strcpy( outMess, inMess );
    outIndex = 2;
  }

  strcpy( inMess, (char *)pgensub->b );
  if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
  {
    strcpy( outMess, inMess );
    outIndex = 1;
  }

/*
 * If any health value is WARNING this will override the GOOD value and result 
 * in a WARNING output value. If the message associated with this WARNING
 * value is non-null and non-blank it will override the default message.
 */

  if ( (strcmp( (char *)pgensub->a, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->a, "bad") == 0) )
  {
    strcpy( outHealth, "WARNING" );
    outIndex = 1;

    strcpy( inMess, (char *)pgensub->b );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

  if ( (strcmp( (char *)pgensub->c, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->c, "bad") == 0) )
  {
    strcpy( outHealth, "WARNING" );
    outIndex = 2;

    strcpy( inMess, (char *)pgensub->d );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

  if ( (strcmp( (char *)pgensub->e, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->e, "bad") == 0) )
  {
    strcpy( outHealth, "WARNING" );
    outIndex = 3;

    strcpy( inMess, (char *)pgensub->f );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

  if ( (strcmp( (char *)pgensub->g, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->g, "bad") == 0) )
  {
    strcpy( outHealth, "WARNING" );
    outIndex = 4;

    strcpy( inMess, (char *)pgensub->h );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

  if ( (strcmp( (char *)pgensub->i, "WARNING") == 0) ||
       (strcmp( (char *)pgensub->i, "bad") == 0) )
  {
    strcpy( outHealth, "WARNING" );
    outIndex = 5;

    strcpy( inMess, (char *)pgensub->j );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

/*
 * If any health value is BAD this will override any GOOD or WARNING values
 * and result in a BAD output value. If the message associated with this BAD
 * value is non-null and non-blank it will override the default message.
 */

  if ( (strcmp( (char *)pgensub->a, "BAD") == 0) ||
       (strcmp( (char *)pgensub->a, "bad") == 0) )
  {
    strcpy( outHealth, "BAD" );
    outIndex = 1;

    strcpy( inMess, (char *)pgensub->b );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

  if ( (strcmp( (char *)pgensub->c, "BAD") == 0) ||
       (strcmp( (char *)pgensub->c, "bad") == 0) )
  {
    strcpy( outHealth, "BAD" );
    outIndex = 2;

    strcpy( inMess, (char *)pgensub->d );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

  if ( (strcmp( (char *)pgensub->e, "BAD") == 0) ||
       (strcmp( (char *)pgensub->e, "bad") == 0) )
  {
    strcpy( outHealth, "BAD" );
    outIndex = 3;

    strcpy( inMess, (char *)pgensub->f );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

  if ( (strcmp( (char *)pgensub->g, "BAD") == 0) ||
       (strcmp( (char *)pgensub->g, "bad") == 0) )
  {
    strcpy( outHealth, "BAD" );
    outIndex = 4;

    strcpy( inMess, (char *)pgensub->h );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

  if ( (strcmp( (char *)pgensub->i, "BAD") == 0) ||
       (strcmp( (char *)pgensub->i, "bad") == 0) )
  {
    strcpy( outHealth, "BAD" );
    outIndex = 5;

    strcpy( inMess, (char *)pgensub->j );
    if ( (strlen(inMess) != 0) && (strspn(inMess," ") != strlen(inMess)) )
    {
      strcpy( outMess, inMess );
    }
  }

/* Output the resulting health value, message and index. */

   strcpy( (char *)pgensub->vala, outHealth );
   strcpy( (char *)pgensub->valb, outMess );
   *(long *)pgensub->valc = outIndex;

  return status;
}
