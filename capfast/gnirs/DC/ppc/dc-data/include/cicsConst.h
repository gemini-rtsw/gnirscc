/*
*   FILENAME
*   --------
*   cicsconst.h
*
*   PURPOSE
*   -------
*   This file contains the symbolic constants used by the CICS. It should be
*   included by any routine requiring them
*
*   REQUIREMENTS
*   ------------
*   If the contents of this file are changed then all the CICS source 
*   code which includes this file should be recompiled. This operation
*   should to be done automatically by "gmake", provided this file is 
*   identified as a dependency in the Makefile
*
*   LIMITATIONS
*   ------------
*   The PASS and FAIL constants ought to go in a Gemini-supplied
*   file rather than here.
*
*   If this file is included in an EPICS state notation program, the state
*   notation compiler will issue a warning message claiming that the
*   variables are being used without being declared. This is a "feature"
*   of the compiler and can be ignored
*
*   AUTHOR
*   ------
*   Steven Beard  (smb@roe.ac.uk)
*
*   HISTORY
*   -------
*   29-Mar-1996: Original version.                          (smb)
*   03-May-1996: Add CAD acceptance or rejection values.    (smb)
*   07-May-1996: Add EPICS_FALSE and EPICS_TRUE.            (smb)
*   19-Jun-1996: Header modified to meet Gemini standards.  (smb)
*   16-Jan-1997: Header modified again to meet changed
*                Gemini standards. PASS and FAIL included
*                instead of STATUS_OK and STATUS_ERROR.     (smb)
*   03-Feb-1997: Gem4 version. Removed CAD_ACCEPT and
*                CAD_REJECT.                                (smb)
*   16-Apr-1997: CICS_BUSY added.                           (smb)
*   11-Jul-1997: CICS_DATUM_ON and CICS_DATUM_OFF included. (smb)
*   08-Aug-1997: CICS_BUSY removed (no longer needed).      (smb)
*/
/* *INDENT-OFF* */
/*
 * $Log: cicsConst.h,v $
 * Revision 1.2  2009/05/27 19:32:41  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  2001/10/30 21:15:25  mbec
 * checking in
 *
 * Revision 1.1.1.1  2001/10/30 20:25:59  mbec
 *
 *
 * Revision 1.1.1.1  2001/07/18 03:17:04  mbec
 * checkin
 *
 * Revision 1.1.1.1  2001/03/28 07:36:03  gemvx
 * checking in split gn-4
 *
 * Revision 1.1  2000/12/19 01:55:00  mbec
 * adding all missing headers
 * 
 */
/* *INDENT-ON* */

#ifndef CICSCONST
#define CICSCONST

/*
 * This is the default message string side used by the CICS. It is larger
 * than the largest EPICS string size (MAX_STRING_SIZE) and care must therefore
 * be taken whenever a CICS message is copied to an EPICS buffer.
 */

#define CICS_MESSAGE_LENGTH 80

/*
 * These constants are used to switch the CICS mechanism simulator into
 * and out of datum search mode.
 */

#define CICS_DATUM_ON  1
#define CICS_DATUM_OFF 0

/* Note that, unlike C, EPICS uses zero to represent a logical value 
 * of TRUE. This is used particularly in binary input and output records.
 */

#define EPICS_TRUE     0
#define EPICS_FALSE    1

/* These constants are used for signalling the success or failure of
 * a function call.
 */

#define PASS           0
#define FAIL           1

#endif
