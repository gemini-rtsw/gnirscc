/*****************************************************************
 * $Id: recHallStep.h,v 1.2 2009/05/27 19:34:45 fkraemer Exp $
 *
 * Author: Hubert Yamada
 * Date:   1997 March 22
 *
 * Include file for the hallStep (hs) Record
 *****************************************************************/

#if !defined(REC_HALLSTEP_H)
#define REC_HALLSTEP_H

#include "ifaErrors.h"

/* 
 * Limit switches.
 */

#define HS_LIMIT_NONE (0)
#define HS_LIMIT_HIGH (1)
#define HS_LIMIT_LOW  (2)

/*
 * Operation codes (Do not change the numerical value of these constants! 
 * These constants must match hard-coded constants in capfast diagrams and 
 * in edd/dm displays.)
 */

#define HS_OPNONE      (0) /* Must be 0 */
#define HS_OPMOVE      (1)
#define HS_OPDATUM     (2)
#define HS_OPREDATUM   (3)
#define HS_OPCYCLE     (4)
#define HS_OPDIAGNOSE  (5)
#define HS_OPDATUMDIAG (6)
#define HS_OPNOP       (7)
#define HS_OPMOVEVER   (8)
#define HS_OPUPDATE    (9)

/*
 * For convenience, define a HallStepRecord data type
 */

typedef struct hallStepRecord HallStepRecord;

/*
 * Fields that are marked !REQ! are defined by EPICS and must be
 * present, in exactly the specified order.
 */

#define HS_DEVNUM (12) /* Number of fields in DevHallStep */

typedef struct { /* derived from struct dset */
	long number;              /* !REQ! number of support routines */
	DEVSUPFUN report;         /* !REQ! print report */
	DEVSUPFUN init;           /* !REQ! init support */
	DEVSUPFUN init_record;    /* !REQ! init support for particular record */
	DEVSUPFUN get_ioint_info; /* !REQ! get io interrupt information */
/* End of dset fields */
	long (*move)(HallStepRecord *);
	long (*datum)(HallStepRecord *);
	long (*redatum)(HallStepRecord *);
	long (*cycleN)(HallStepRecord *, int);
	long (*update)(HallStepRecord *);
	long (*verify)(HallStepRecord *);
	long (*directMove)(HallStepRecord *);
} DevHallStep;

/* HallStep record support functions */

long hsCheckLimits(HallStepRecord *, int *);
long hsCycleN(HallStepRecord *, int, int);
long hsGetLinkValueDouble(HallStepRecord *, struct link *, double *);
long hsGetLinkValueDouble3(HallStepRecord *, struct link *, double *);
long hsGetLinkValueLong(HallStepRecord *, struct link *, long *);
void hsMSPause(HallStepRecord *, int);
long hsMax(HallStepRecord *, struct link *, long *, long, int, long, long, int);
long hsMoveTo(HallStepRecord *, long, int);
long hsPutLinkValueDouble(HallStepRecord *, struct link *, double);
long hsPutLinkValueLong(HallStepRecord *, struct link *, long);
long hsSeekHome(HallStepRecord *phs, int);
long hsSetHome(HallStepRecord *phs, long);
long hsSetOffset(HallStepRecord *phs, long);
void hsStatus(HallStepRecord *, int sev, const char [], ...);

/* 
 * Severity of warning
 */

#define HS_MESS   (-1)
#define HS_ERROR  (-2)
#define HS_STATUS (-3)

/*
 * Interlocks
 */

extern volatile int hsLockCfg;
extern volatile int hsLockObs;
extern volatile int hsLockGen;
extern volatile int hsLockInit;
extern volatile int hsLockTmp;
extern volatile int hsForceLockTmp;
extern volatile int hsLockTmpHB;
extern volatile int hsActive;
extern volatile int hsMoving;
extern volatile int hsMaxMoving;

/*
 * Temperature status (values for hsLockTmp)
 *
 * (Do not change the numerical value of these constants! These 
 * constants must match hard-coded constants in capfast diagrams and 
 * in edd/dm displays.)
 */

#define HS_TMP_INVALID  (-1) /* Used to flag invalid temperature state */
#define HS_TMP_CHANGING (0)  /* Default */
#define HS_TMP_COLD     (1)
#define HS_TMP_WARM     (2)

/*
 * Axis (values for AXIS field)
 *
 * (Do not change the numerical value of these constants! These 
 * constants must match hard-coded constants in capfast diagrams and 
 * in edd/dm displays.)
 */

#define HS_AXIS_NONE (0) /* Default */
#define HS_AXIS_WFS_X    (1)
#define HS_AXIS_WFS_Y    (2)

/*
 * Follow mode enabled?  (values for hsWfsFollow)
 */

#define HS_DISABLE (0)
#define HS_ENABLE (1)

/*
 * If the step count is out of range, strange things can happen.  The
 * actual legal range seems to be around 15000000 (2^24?).  The
 * limit is set a little smaller (by about 5000000, which guarantees
 * that we can always travel a little further than this limit without
 * problem).
 */

#define HS_MAX_STEP 10000000

/*
 * Error codes
 */

#define S_ifa_hs_Stop          (M_ifa_hs | IFA_EWARN | 1)

#define S_ifa_hs_Busy          (M_ifa_hs | IFA_ERR | 1)
#define S_ifa_hs_MoveFailed    (M_ifa_hs | IFA_ERR | 2)
#define S_ifa_hs_CycleFailed   (M_ifa_hs | IFA_ERR | 3)
#define S_ifa_hs_VerifyFailed  (M_ifa_hs | IFA_ERR | 4)
#define S_ifa_hs_MissingSensor (M_ifa_hs | IFA_ERR | 5)
#define S_ifa_hs_MissingHome   (M_ifa_hs | IFA_ERR | 6)
#define S_ifa_hs_MathError     (M_ifa_hs | IFA_ERR | 7)
#define S_ifa_hs_OutOfRange    (M_ifa_hs | IFA_ERR | 8)
#define S_ifa_hs_BadOp         (M_ifa_hs | IFA_ERR | 9)
#define S_ifa_hs_Locked        (M_ifa_hs | IFA_ERR | 10)
#define S_ifa_hs_TimeOut       (M_ifa_hs | IFA_ERR | 11)

#endif /* REC_HALLSTEP_H */
