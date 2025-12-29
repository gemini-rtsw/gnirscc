/* $Id: drvSoftHS.h,v 1.2 2009/05/27 19:34:44 fkraemer Exp $ */

#if !defined(DRV_SOFTHS_H)
#define DRV_SOFTHS_H

#include <dbDefs.h>

#include "ifaErrors.h"

/*****************************************************************
 * The simulator data structures
 *****************************************************************/

typedef struct {
	double val;
} ShsSensor;

/*
 * The first block of fields are initialized statically.  Don't
 * rearrange them.
 */

typedef struct shs_parms_ {
	const char *id;                     /* Identification string */
	void (*updateSig)(struct shs_parms_ *); /* Function to update position */
	const double width;                 /* Approx. width of sensor peak */
	const int nMagnet;                  /* Number of (primary) magnets */
	const int firstMagnet;              /* Position of first magnet */
	const int incMagnet;                /* Spacing between magnets */
	const int modulus;                  /* Number of steps (wheels only) */

	int event;
	long (*callback)(void *);
	void *callbackArg;

	int motorBaseSpeed;
	int motorSpeed;
	int motorDest;
	int motorMoving;
	int motorPos;
	int motorDir;

	ShsSensor hs1p;
	ShsSensor hs1b;
	ShsSensor hs2p;
	ShsSensor hs2b;
} ShsParms;

/*****************************************************************
 * The simulator functions
 *****************************************************************/

/*
 * Simulator functions
 */

long shsDoCallback(ShsParms *);
long shsMotorCommand(ShsParms *, int, int);
long shsMotorInit(void *, const char [], void **);
long shsSensorInit(void *, const char [], void **);
long shsSensorRead(ShsSensor *, double *);
long shsSetPos(ShsParms *, int);
long shsSetSpeed(ShsParms *, int);
long shsSetDest(ShsParms *, int, int);
long shsStart(ShsParms *);
long shsStop(ShsParms *);
void shsSetCallback(ShsParms *, long (*)(void *), void *);

/*
 * Simulation mode: full or fast
 */

extern long shsFast;

#endif /* DRV_SOFTHS_H */
