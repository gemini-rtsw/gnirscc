/*
*   FILENAME
*   --------
*   dataHandling.h
*
*   PURPOSE
*   -------
*   This file defines the functions contained in the dataHandling library.
*   It should be included by any routine wishing to use the library
*
*   REQUIREMENTS
*   ------------
*   If the definitions of the functions in dataHandling are changed, or if
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
*   18-Jun-1997: Original version.                           (smb)
*   26-Jun-1997: dataSetDhsInfo added.                       (smb)
*/
/* *INDENT-OFF* */
/*
 * $Log: dataHandling.h,v $
 * Revision 1.2  2009/05/27 19:32:26  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:19  buchholz
 * Imported gnaacSrc into CVS
 *
 */
/* *INDENT-ON* */

#ifndef DATAHANDLING
#define DATAHANDLING

/* Declare the constants used by functions in the dataHandling library. */


#define DATA_MAX_DIMS   7
#define DATA_MAX_XSIZE  1024
#define DATA_MAX_YSIZE  1024

/* Declare the functions in the dataHandling library. */

long dataInit( const char *, const long, const char *, const char * );
long dataDisconnect( );
long dataObserveStart( const char *, float **, float **, short ** );
long dataObserveEnd( const char * );
long dataSetDhsInfo( const char * );

void   dataErrorCallBack();
void   dataGetCallBack();
void   dataPutCallBack();

#endif
