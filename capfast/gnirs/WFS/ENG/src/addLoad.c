static char rcsid[] = "$Id: addLoad.c,v 1.2 2009/05/27 19:34:40 fkraemer Exp $";

/*
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * VxWorks utility to test the system under increased load.
 *
 * FILENAME
 *
 *     addLoad.c
 *
 * FUNCTION NAME(S)
 *     addLoad
 */

#include <vxWorks.h>
#include <time.h>
#include <math.h>



/*
 *+
 * FUNCTION NAME: addLoad
 *
 * INVOCATION: 
 *
 *     From the VxWorks prompt:
 *
 *         ld < addLoad.o
 *         sp addLoad
 *
 *     This function is not intended to be called as a subroutine.
 *
 * PARAMETERS: none
 *
 * FUNCTION VALUE: (void) This function will never return.
 *
 * SCOPE: global
 *
 * PURPOSE: 
 *
 *      Adds a load (about 20% on my cpu) to the system, to insure 
 *      that the software will work correctly if the system is 
 *      loaded down by other processes.
 *
 * DESCRIPTION:
 *
 *     Once this routine is called, it enters into an infinite loop,
 *     and pauses.  Periodically it performs a series of calculations,
 *     to load the system down, then goes back to sleep.
 *
 * EXTERNAL VARIABLES: none
 *
 * PRIOR REQUIREMENTS: none
 *
 * DEFICIENCIES:
 *
 *     Impossible to stop, once it is started.  Timing values depend
 *     on the cpu-clock speed.  Frequency of operation is hard-coded.
 *-
 */

void
addLoad() 
{
	int i;
	const msec = 20;

	for (;;) {
		struct timespec ts;

		ts.tv_sec = msec / 1000;
		ts.tv_nsec = (msec % 1000) * 1000000;
		nanosleep(&ts, NULL);

		for (i = 0; i < 200; i++)
			sin(0.0);
	}
}
