/*******************************************************************************
 * Program:     naacdrvr
 * File:	fchanio.h
 * Purpose:	Defines structures and routine prototypes for VxWorks naaclink 
 *		protocol 
 * Author:	Diana Kennedy
 * History:
 *	11-Jan-1996 - created file - djk
 *
 ******************************************************************************/

#ifndef NAACCHANIO
#define NAACCHANIO

/* chanio  hardcoded stuff based on the speed of the hardware. */

#define HZ	25	/* # cycles per microsecond for mv162 */
#define LOOPR	62	/* # cycles to process loop (unsuccessful "peek") mv162 */
#define LOOPW	62	/* # cycles to process loop (unsuccessful "poke") mv162 */

/* TIME is calculated from the clock speed of the machine
 * HZ, times the number of microseconds we want to wait X,
 * divided by the number of cycles the loop will take LOOP[RW].
 */
#define	TIMEREAD(X)		(X * HZ / LOOPR)
#define	TIMEWRITE(X)		(X * HZ / LOOPW)

/* min and max select delay time,  in microseconds.  At its worst, select
 * will be timing out every SELDELAY2 us (currently 200 ms, .2 second).
 */
#define	SELDELAY1	2000
#define SELDELAY2	128000

#endif				/* NAACCHANIO */
