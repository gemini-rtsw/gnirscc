/*
 * TCF.H - Header file describing CCD-TCP data block.  Based on D'Anne's
 * TCP code.
 * 
 * This header file describes the size and format of the encoded response
 * message that the TCP system send in response to a 'gettcf' command.
 * See the forth tcp system source, file tcio, for more information.
 * 
 * Common data types are:
 *   s - short integer, no scaling.
 *   l - long integer, no scaling.
 *   f - long integer, scaled by 100.
 *   h - long integer, scaled by 3600. (to get hours/degrees)
 */

/* Telescope independent. */
#define	BNUM		0
#define	VNUM		2		/* s, vdu page # */
#define	FPOSNUM		4		/* s, focal position # */
#define	DAY		6		/* s, day of month */
#define	MONTH		8		/* s, month of year */
#define	YEAR		10		/* s, year */
#define	JDNUM		36		/* s, julian day # */
#define	UT		40		/* h, ut (hours) */
#define	LST		44		/* h, local sidereal time (hours) */
#define	HA		48		/* h, ha (degrees) */
#define	RA		52		/* h, ra (hours) */
#define	DEC		56		/* h, dec (degrees) */
#define	DAZ		60		/* f, dome azimuth */
#define	ERA		64		/* h, error in ra */
#define	EDEC		68		/* h, error in dec */
#define	DMER		72		/* f, dome error */
#define	PRA		76		/* h, preset ra */
#define	PDEC		80		/* h, preset dec */
#define	DTOL		84		/* f, dome tolerance */
#define	DEPCH		88		/* f, alternate vdu epoch */
#define	TZD		92		/* f, telescope zenith distance */
#define	TAZ		96		/* f, telescope azimuth */
#define	AIR		100		/* f, airmass */
#define	DAYNUM		104		/* s, day of year */

/* Telescope dependent. */
#define GLAMBDA		580		/* l, grating angle in 10ths deg */
#define	FILTPOS09	577		/* b, 0.3 meter filter (ascii char) */
#define	FILTPOS21	901		/* b, 1.1 meter filter (ascii char) */
#define	FILTPOS4	578		/* s, 4 meter filter */
