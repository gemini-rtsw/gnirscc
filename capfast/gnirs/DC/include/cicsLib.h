/*
*   FILENAME
*   --------
*   cicslib.h
*
*   PURPOSE
*   -------
*   This file defines the functions contained in the cicslib library.
*   It should be included by any routine wishing to use the library
*
*   REQUIREMENTS
*   ------------
*   If the definitions of the functions in cicslib are changed, or if
*   more functions are added, this file must be kept up to date
*
*   LIMITATIONS
*   -----------
*   If this file is included in an EPICS state notation program, the state
*   notation compiler will issue a warning message claiming that the
*   functions are being used without being declared. This is a "feature"
*   of the compiler and can be ignored
*
*   AUTHOR
*   ------
*   Steven Beard  (smb@roe.ac.uk)
*
*   HISTORY
*   -------
*   04-Dec-1996: Original version.                           (smb)
*   16-Dec-1996: cicsLogFloat added.                         (smb)
*   10-Jan-1997: cicsLogDouble added.                        (smb)
*   14-Jan-1997: cicsSetDebug added.                         (smb)
*   16-Jan-1997: Header modified and Functions renamed to
*                conform to new SPS.                         (smb)
*   09-Jun-1997: More debug level constants added.           (smb)
*   17-Jun-1997: cicsInitLogging added.                      (smb)
*/
/* *INDENT-OFF* */
/*
 * $Log: cicsLib.h,v $
 * Revision 1.2  2009/05/27 19:32:26  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:19  buchholz
 * Imported gnaacSrc into CVS
 *
 */
/* *INDENT-ON* */

#ifndef CICSLIB
#define CICSLIB

/* Declare the constants used by functions in the cicslib library. */

/*
 * These constants define the numerical codes used to store the current
 * debugging level. The levels are:
 *
 * NOLOG - only error messages;
 * NONE  - only error messages and log messages;
 * MIN   - minimal debugging (log messages plus a few others);
 * FULL  - full debugging (messages giving a full running comentary);
 */

#define CICS_DB_NOLOG 0
#define CICS_DB_NONE  1
#define CICS_DB_MIN   2
#define CICS_DB_FULL  3

/*
 * The above constants may also be used to define the level of each
 * message logged to the system. The following constants are used as
 * synonyms.
 */

#define CICS_DB_ERROR 0
#define CICS_DB_LOG   1


/* Declare the functions in the cicslib library. */

long cicsDbGet( char *, char *, unsigned short, void * );
long cicsDbPut( char *, char *, unsigned short, void * );
void cicsInitLogging( void );
void cicsLogMessage( const long, const char * );
void cicsLogLong( const long, const char *, const long );
void cicsLogFloat( const long, const char *, const float );
void cicsLogDouble( const long, const char *, const double );
void cicsLogString( const long, const char *, const char * );
void cicsSetDebug( const long );

#endif
