static char rcsid[] = "$Id: tm.c,v 1.2 2009/05/27 19:34:46 fkraemer Exp $";
 
/*
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada
 *
 * Standalone time stamp utility 
 *
 * FILENAME
 *     tm.c
 *
 * FUNCTION NAME(S)
 */

#include <sys/types.h>
#include <time.h>
#include <stdio.h>

/*
 *+
 * FUNCTION NAME: init_record
 *
 * INVOCATION: init_record(phs, pass)
 *
 * PARAMETERS:
 *
 *     (!) phs    (HallStepRecord *)  EPICS record
 *     (>) pass   (int)               Initialization pass
 *
 * FUNCTION VALUE:
 *
 *     (long) Error status (nonzero indicates an error)
 *
 * PURPOSE: Standalone function to return the current time.
 *
 *     This function prints the current time, as a numeric value, 
 *     for timestamping temperature curves.
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES: 
 *
 * PRIOR REQUIREMENTS: none
 *
 * DEFICIENCIES:
 *
 *     None known.
 *-
 */

int
main()
{
	printf("%d\n", time(NULL));

	return 0;
}
