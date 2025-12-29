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
*/
/* *INDENT-OFF* */
/*
 * $Log: cicsConst.h,v $
 * Revision 1.2  2009/05/27 19:35:02  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.3  1999/11/13 06:35:07  yamada
 * Release 1.00 alpha 06.
 *
 * Revision 1.2  1999/08/27 10:13:09  yamada
 * Fixed a serious buffer overrun problem involving sprintf.  Also,
 * shortened status messages so that the entire message can be seen.
 *
 * Revision 1.1  1997/07/26 01:30:28  yamada
 * Initial release
 *
 */
/* *INDENT-ON* */

#ifndef CICSCONST
#define CICSCONST

/*
 * This constant is used to prevent a sequence command from executing
 * when the instrument is busy.
 */

#define CICS_BUSY      1

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

/* This is the size of the buffer that is used for sprintf statements.
 * Earlier versions of the software used MAX_STRING_SIZE, which is
 * too small to be safe.  Memory was getting clobbered.  Unfortunately,
 * with sprintf, there is no way to prevent a buffer overflow, but
 * this should be fairly safe.
 */

#define STRBUF (1024 + 5 * MAX_STRING_SIZE)

#endif
