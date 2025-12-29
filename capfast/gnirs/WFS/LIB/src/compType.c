static char rcsid[]="$Id: compType.c,v 1.2 2009/05/27 19:34:56 fkraemer Exp $";

/*
 *   FILENAME
 *   -------- 
 *   compType.c
 *
 *   PURPOSE
 *   -------
 *
 *   FUNCTION NAME(S)
 *   ----------------
 *
 *   DEPENDENCIES
 *   ------------
 *
 *   LIMITATIONS
 *   ------------
 *
 *   AUTHOR
 *   ------
 *   Hubert Yamada (yamada@newton.ifa.hawaii.edu)
 *
 *   HISTORY
 *   -------
 *INDENT-OFF*
 *
 * $Log: compType.c,v $
 * Revision 1.2  2009/05/27 19:34:56  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.3  1999/11/14 02:05:42  yamada
 * Reformatted logs.
 *
 *
 *INDENT-ON*
 */

/* Global Constants */

#include <vxWorks.h>
#include <semLib.h>
#include <types.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdioLib.h>
#include <string.h>

#include <dbDefs.h>
#include <dbCommon.h>
#include <recSup.h>
#include <dbAccess.h>
#include <ctype.h>

#include <cicsConst.h>
#include <cicsLib.h>
#include <cad.h>
#include <compLib.h>

/*
 * Given a string, convert it to a double.  If the string is not a
 * valid double, then return CAD_REJECT.  If the string is a valid
 * double, return CAD_ACCEPT.
 */

long
compDouble(const char* pStr, double *pDouble)
{
    char *pEnd;
	long status;

    status = CAD_ACCEPT;

    if (status == CAD_ACCEPT && pStr == NULL)
        status = CAD_REJECT;

    if (status == CAD_ACCEPT)
        *pDouble = strtod(pStr, &pEnd);

	/*
	 * String was not convertable.
	 */

	if (status == CAD_ACCEPT && *pDouble == 0.0 && pEnd == pStr)
		status = CAD_REJECT;

	/*
	 * If there are any trailing characters, then this is not a legal
	 * double value.
	 */

	if (status == CAD_ACCEPT) {
		while (*pEnd != '\0') { 
			if (isascii(*pEnd) && isspace(*pEnd))
				pEnd++;
			else
				break;
		}

		if (*pEnd != '\0') status = CAD_REJECT;
	}

	return status;
}

/*
 * Given a string, convert it to a long.  If the string is not a
 * valid long, then return CAD_REJECT.  If the string is a valid
 * long, return CAD_ACCEPT.
 */

long
compLong(const char* pStr, long *pLong)
{
    char *pEnd;
	long status = CAD_ACCEPT;

    if (status == CAD_ACCEPT && pStr == NULL)
        status = CAD_REJECT;

    if (status == CAD_ACCEPT)
        *pLong = strtol(pStr, &pEnd, 10);

	/*
	 * String was not convertable.
	 */

	if (status == CAD_ACCEPT && *pLong == 0 && pEnd == pStr)
		status = CAD_REJECT;

	/*
	 * If there are any trailing characters, then this is not a legal
	 * double value.
	 */

	if (status == CAD_ACCEPT) {
		while (*pEnd != '\0') { 
			if (isascii(*pEnd) && isspace(*pEnd))
				pEnd++;
			else
				break;
		}

		if (*pEnd != '\0')
			status = CAD_REJECT;
	}

	return status;
}
