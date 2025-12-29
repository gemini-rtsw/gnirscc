/* $Id: drvOcyc.h,v 1.2 2009/05/27 19:32:35 fkraemer Exp $ */

#if !defined(DRV_OCYC_H)
#define DRV_OCYC_H

#include <dbDefs.h>

#include "ifaErrors.h"

/*
 * Which fields need updating (binary flag bits)
 *
 * Note:  After setting a parameter, it can take several seconds
 * for the setting to be updated.
 */

#define OCYC_UPDATE_SETPOINT    (0x0002) /* Set point */
#define OCYC_UPDATE_GAIN        (0x0004) /* Gain */
#define OCYC_UPDATE_RESET       (0x0008) /* Reset */
#define OCYC_UPDATE_RATE        (0x0010) /* Rate */
#define OCYC_UPDATE_RANGE       (0x0020) /* Heater range */
#define OCYC_UPDATE_TUNE        (0x0040) /* Autotuning */

#define OCYC_RANGE_OFF     (0)
#define OCYC_RANGE_LOW     (2)
#define OCYC_RANGE_HIGH    (3)

#define OCYC_TUNE_MANUAL   (0)
#define OCYC_TUNE_P        (1)
#define OCYC_TUNE_PI       (2)
#define OCYC_TUNE_PID      (3)

#define OCYC_UNITS_C       (0)
#define OCYC_UNITS_K       (1)
#define OCYC_UNITS_F       (2)

typedef struct ocyc_update_ {
	long flags;                 /* Which fields to change */
	double setPoint;            /* Temperature setpoint (C) */
	double gain;
	double reset;
	double rate;
	int range;
	int tune;
} OcycUpdate;

/*
 * These parameters can change at any time, without warning; do not
 * include any fields in this array which need more careful handling.
 */

typedef struct ocyc_values_ {
	int status;                 /* Set if there is a communications problem */
	int pending;                /* Hardware settings are being updated */
	double temperature;         /* Current temperature */
	double heat;
	double setPoint;            /* Temperature setpoint (C) */
	double gain;
	double reset;
	double rate;
	int range;
	int tune;
} OcycValues;

/*
 * Temperature control functions
 */

long ocycRecInit(void *, const char[], void **);
long ocycSet(void *, const OcycUpdate *);
void ocycSetUnits(void *, int);
const OcycValues *ocycGet(void *);

/*
 * Error codes
 */

#define S_ifa_ocyc_BadCmd        (M_ifa_ocyc | IFA_ERR | 1)
#define S_ifa_ocyc_Comm          (M_ifa_ocyc | IFA_ERR | 2)
#define S_ifa_ocyc_Uninitialized (M_ifa_ocyc | IFA_ERR | 3)
#define S_ifa_ocyc_BufOverflow   (M_ifa_ocyc | IFA_ERR | 4)
#define S_ifa_ocyc_ErrorMsg      (M_ifa_ocyc | IFA_ERR | 5)

#endif /* DRV_OCYC_H */
