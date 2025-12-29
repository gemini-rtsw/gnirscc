/*
 * chanio.h
 *
 * hardcoded stuff based on the speed
 * of the hardware.
 *
 */

#ifdef mc68020
#define HZ	16	/* # cycles per microsecond */
#define LOOPR	62	/* # cycles to process loop (unsuccessful "peek") */
#define LOOPW	62	/* # cycles to process loop (unsuccessful "poke") */
#endif

/*
 * TIME is calculated from the clock speed of the machine
 * HZ, times the number of microseconds we want to wait X,
 * divided by the number of cycles the loop will take LOOP[RW].
 */
#define	TIMEREAD(X)		(X * HZ / LOOPR)
#define	TIMEWRITE(X)		(X * HZ / LOOPW)

/*
 * min and max select delay time,
 * in microseconds.  At its worst, select
 * will be timing out every SELDELAY2
 * microseconds (currently 200 milliseconds,
 * or 2/10 second).
 */
#define	SELDELAY1	2000
#define SELDELAY2	128000
