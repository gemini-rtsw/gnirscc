/* $Id: drvOcyd.h,v 1.2 2009/05/27 19:34:44 fkraemer Exp $ */

#if !defined(DRV_OCYD_H)
#define DRV_OCYD_H

#include <dbDefs.h>

#include "ifaErrors.h"

#define OCYD_NCHAN (8)

#define OCYD_UNITS_C       (0)
#define OCYD_UNITS_K       (1)
#define OCYD_UNITS_F       (2)

/*
 * Which fields need updating (binary flag bits)
 *
 * If OCYC_SET_INIT is set, all driver parameters are assumed to be
 * wrong, and are reread from the hardware.  This is rather
 * time-consuming, so should not be done too frequently.
 */

#define OCYD_SET_INIT     (0x0001) /* Re-read hardware parameters */

/*
 * Make channel active (fields 0x0100-0x8000).  Warning: the
 * field number must be between 1 and 8, inclusive!
 */

#define OCYD_SET_ACTIVE(i)  (0x0100 << ((i) - 1)) 

typedef struct ocyd_update_ {
	long flags;                 /* Which fields to change */
	int active[OCYD_NCHAN + 1];
} OcydUpdate;

/*
 * These parameters can change at any time, without warning; do not
 * include any fields in this array which need more careful handling.
 */

typedef struct ocyd_values_ {
	double setPoint;               /* Temperature setpoint (C) */
	double tmp[OCYD_NCHAN + 1];    /* Current temperature */
	int active[OCYD_NCHAN + 1];    /* Channel enabled */
	int status;                    /* Set if communications problem */
} OcydValues;

/*
 * Temperature control functions
 */

long ocydRecInit(void *, const char [], void **);
long ocydSet(void *, const OcydUpdate *);
void ocydSetUnits(void *, int);
const OcydValues *ocydGet(void *);

/*
 * Error codes
 */

#define S_ifa_ocyd_BadCmd       (M_ifa_ocyd | IFA_ERR | 1)
#define S_ifa_ocyd_Comm         (M_ifa_ocyd | IFA_ERR | 2)
#define S_ifa_ocyd_BufOverflow  (M_ifa_ocyd | IFA_ERR | 3)

#endif /* DRV_OCYD_H */
