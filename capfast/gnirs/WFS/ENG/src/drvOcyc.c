static char rcsid[] = "$Id: drvOcyc.c,v 1.2 2009/05/27 19:34:43 fkraemer Exp $";

/*
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * EPICS Device support software simulation system for NIRI mechanism. 
 *
 * FILENAME
 *     drvOcyc.c
 *
 * FUNCTION NAME(S)
 */

#include "drvOcyc.h"

#include <vxWorks.h>
#include <stdioLib.h>
#include <sysLib.h>             /* library for task support */
#include <taskLib.h>
#include <rngLib.h>             /* library for ring buffer support */
#include <semLib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <tyLib.h>
#include <usrLib.h>
#include <ioLib.h>
#include <tyLib.h>
#include <usrLib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <logLib.h>

#include <dbDefs.h>
#include <drvSup.h>
#include <devSup.h>
#include <recSup.h>
#include <module_types.h>
#include <taskwd.h>

/*
 * If XY490 is defined, communications takes place using the XY490 
 * serial board support of the built-in serial port.
 *
 * DANGER: There may be a bug in this driver which clobbers memory and
 * causes the task to stop updating intermittantly (roughly once every
 * two or three days).  This might have been due to a bug in the
 * OMS driver which caused it to overwrite memory if the motor counters
 * were too high, but I don't want to re-enable this to test it.
 */

/* #define XY490 */

#if defined(XY490)
#include "xy490.h"
#endif

#if !defined(DEBUG)
#	define DEBUG (0)
#endif

#define BAUD_RATE (1200) /* Must be set to match the hardware */

typedef struct { /* derived from struct drvet */
	long number;         /* !REQ! number of support routines */
	DRVSUPFUN report;    /* !REQ! print report */
	DRVSUPFUN init;      /* !REQ! *init support */
	DEVSUPFUN reboot;    /* !REQ! init support for particular record */
/* End of standard fields */
} DrvOcyc;

static long init();
#define report ((DEVSUPFUN)0)
#define reboot ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
DrvOcyc drvOcyc = {
	4,
	report,
	init,
	reboot,
};

/*
 * How long a timeout for serial communications?
 *
 * There really is no need for a timeout.  However, the system will
 * occasionally hang (XY490 mode only) and need to be reset.  This
 * is a bug that should be isolated, but I haven't been able to find
 * it.
 */

#define SERIAL_TIMEOUT_S  (2)
#define SERIAL_TIMEOUT_US (0)

/*
 * How long can a command or a command response be?
 */

#define BUFSZ 32

/*
 * How many times to retry a failed command?
 */

#define RETRY (3)

/*****************************************************************
 * The temperature-controller data structures
 *****************************************************************/

typedef struct parms_ {
	char tty[MAX_STRING_SIZE];  /* Which tty? */
	SEM_ID semFd;               /* Currently writing */
	int fd;                     /* File descriptor */

	int units;                  /* Units */

	int delay;                  /* Don't read hardware, until delay = 0 */

	SEM_ID semUpdate;           /* Protect update field */
	OcycUpdate update;          /* New values to set */

	OcycValues values;          /* Parameter values */
} Parms;

#define NPARM 5 /* Maximum number of controllers */
static Parms parms[NPARM];

static SEM_ID semParms; /* Prevent access while a parms entry is half created */

/*
 * Nothing about the temperature control-loop will change very rapidly,
 * so don't poll very often.  It will actually poll more slowly than
 * this, because of serial line delays.
 */

#define UPDATE_RATE (1000) /* Update rate, ms */
#define UPDATE_RATE_S (UPDATE_RATE / 1000.0)
#define UPDATE_RATE_NS ((UPDATE_RATE % 1000) * 1000000)

/*
 * How often do we reread all parameters.  It takes nearly a second to
 * read everything, at 1200 baud, and most of it isn't interesting
 * anyway, so we only reread it occasionally.
 */

#define UPDATE_SKIP (10) /* In units of UPDATE_RATE */

/*
 * Debugging flag
 */

/*
 * Function declarations
 */

static void ocycTask(int i);
static int sendCmd(Parms *pp, const char cmd[], char *rpy, int rpysz);



/*
 *+
 * FUNCTION NAME: init
 *
 * INVOCATION: init()
 *
 * PARAMETERS:
 *
 *    None
 *
 * FUNCTION VALUE: (long) Returns status (0 = okay, non-zero = error)
 *
 * PURPOSE: Initialize driver
 *
 * DESCRIPTION:
 *
 *     This function is called by the standard EPICS driver initialization
 *     code.  It is not intended to be called directly.  It initializes
 *     static data structures that are needed by the driver. 
 *
 * EXTERNAL VARIABLES:
 *
 *     semParms
 *
 * PRIOR REQUIREMENTS:
 *
 *     none
 *
 * DEFICIENCIES:
 *
 *     Should only be called once.
 *-
 */

static long
init()
{
	long status = 0;

	semParms = semBCreate(SEM_Q_FIFO, SEM_FULL);

	return status;
}



/*
 *+
 * FUNCTION NAME: ocycRecInit
 *
 * INVOCATION: ocycRecInit(pRec, id, pDpvt)
 *
 * PARAMETERS:
 *
 *     (>) pRec  (void *)         EPICS record
 *     (>) tty   (const char [])  Serial port or xycom port to monitor
 *     (<) pDpvt (void **)        Private data for this record instance
 *
 * FUNCTION VALUE: (long) status (0 = okay, non-zero = error)
 *
 * PURPOSE:  Driver-specific initialization of an EPICS record.
 *
 * DESCRIPTION:
 *
 *     This routine is called by EPICS record initialization, once
 *     for each epics record which uses this driver.  It allocates
 *     and initializes a data structure for each new port (tyLib 
 *     serial port or xycom serial port), and stores the address of
 *     the data structure in the pDpvt variable (which is intended
 *     to be the dpvt field of an EPICS record).  It then sets the
 *     parameters of the serial port to appropriate values.
 *
 *     If the port has already been initialized, the corresponding data
 *     structure is stored in pDpvt, and no further initialization takes
 *     place.
 *
 * EXTERNAL VARIABLES:
 *
 *     semParms (static)
 *     parms (static)
 *
 * PRIOR REQUIREMENTS:
 * 
 *     Must be called by standard EPICS initialization.
 *
 * DEFICIENCIES:
 *
 *     There can be only NPARM devices.
 *-
 */

long
ocycRecInit(void *pRec, const char *tty, void **pDpvt)
{
	int i;
	long status = 0;

#if DEBUG > 1
	fprintf(stderr, "ocycRecInit(%p,\"%s\",%p)\n", pRec, tty, pDpvt);
#endif

	semTake(semParms, WAIT_FOREVER);

	if (!tty || !*tty) {
		status = S_dev_badSignal;
#if DEBUG > 1
		fprintf(stderr, 
			__FILE__ "(%d): ocycRecordInit No serial port specified.",
			__LINE__);
#endif
		recGblRecordError(status, (void *)pRec,
			__FILE__ "::ocycRecordInit No serial port specified.");
	}

	/*
	 * First, see if this tty is already in use
	 */

	if (!status) {
		for (i = 0; i < NPARM && *pDpvt == NULL && !status; i++) {
			if (strncmp(parms[i].tty, tty, MAX_STRING_SIZE - 1) == 0)
				*pDpvt = &parms[i];
		}
	}

	/* 
	 * If the record is not already active, see if we have an available
	 * slot, then try to open it.
	 */

	if (!*pDpvt && !status) {
		int fd;
		char buf[BUFSZ];

#if DEBUG > 1
		fprintf(stderr, "Opening tty \"%s\"\n", tty);
#endif

		/*
		 * Look for unused port
		 */

		for (i = 0; i < NPARM; i++) {
			if (*parms[i].tty == '\0')
				break;
		}

		if (i == NPARM) {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badInpType, pRec,
				"drvOcyc (ocycRecInit) Too many serial ports specified.");
		}

		/*
		 * Open the file
		 */

		if (!status) {
#if defined(XY490)
			fd = atoi(tty);
#if DEBUG > 1
			fprintf(stderr, "XY490 port %d\n", fd);
#endif
#else /* Not XY490, assume tyLib */
#if DEBUG > 1
			fprintf(stderr, "Opening \"%s\"\n", tty);
#endif
			fd = open(tty, O_RDWR, 0);
#if DEBUG > 1
			fprintf(stderr, "Opened: Returned %d\n", fd);
#endif
			if (fd == ERROR) {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badInpType, pRec,
					"drvOcyc (ocycRecInit) Cannot open input.");
			}
#endif /* XY490 or tyLib */
		}

		/*
		 * Set the baud rate
		 */

		if (!status) {
#if defined(XY490)
#if DEBUG > 1
			fprintf(stderr, "XY490/%d: Setting baud rate to %d.\n",
				fd, BAUD_RATE);
#endif
			if (xy490SetBaud(fd, BAUD_RATE) == -1) {
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, pRec,
					"drvOcyc (ocycRecInit) Cannot set baud rate.");
#if DEBUG > 1
			} else {
				fprintf(stderr, "Baud rate set\n");
#endif
			}
#else /* Not XY490, assume tyLib */
#if DEBUG > 0
			fprintf(stderr, "%s: Setting baud rate to %d.\n",
				tty, BAUD_RATE);
#endif
			if (ioctl(fd, FIOBAUDRATE, BAUD_RATE) == -1) {
				printErrno(errno);
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, pRec,
					"drvOcyc (ocycRecInit) Cannot set baud rate.");
#if DEBUG > 0
			} else {
				fprintf(stderr, "Baud rate set\n");
#endif
			}
#endif
		}

		/*
		 * Set the number of data bits
		 */

#if !defined(XY490)
		if (!status) {
#if DEBUG > 0
			fprintf(stderr, "%s: Setting data bits to 7.\n", tty);
#endif
			if (ioctl(fd, FIOSETOPTIONS, OPT_7_BIT) == -1) {
				printErrno(errno);
				status = S_dev_badSignal;
				recGblRecordError(S_dev_badSignal, pRec,
					"drvOcyc (ocycRecInit) Cannot set data bits.");
#if DEBUG > 0
			} else {
				fprintf(stderr, "Data bits set\n");
#endif
			}
		}
#endif

		/*
		 * After all other initialization has taken place succesfully,
		 * create semaphores, start the task to update the temperature
		 * reading, and set the return value.
		 */

		if (!status) {
			strncpy(parms[i].tty, tty, MAX_STRING_SIZE);
			parms[i].tty[MAX_STRING_SIZE - 1] = '\0';

			parms[i].fd = fd;

			parms[i].update.flags = 0;
			parms[i].delay = 0; /* Read hardware immediately */

			parms[i].semFd = semBCreate(SEM_Q_FIFO, SEM_FULL);

			parms[i].semUpdate = semBCreate(SEM_Q_FIFO, SEM_FULL);

			parms[i].values.status = S_ifa_ocyc_Uninitialized;
			parms[i].values.pending = 0; /* No pending operations */

			sprintf(buf, "ocycTask%d", i);
			taskSpawn(buf, 42, VX_FP_TASK, 10000, (FUNCPTR)ocycTask,
				i, 0, 0, 0, 0, 0, 0, 0, 0, 0);

			*pDpvt = &parms[i];
		}
	}

	semGive(semParms);

	return status;
}



/*
 *+
 * FUNCTION NAME: ocycSet
 *
 * INVOCATION: ocycSet(pArg, pUpdate)
 *
 * PARAMETERS:
 *
 *     (!) pArg    (void *)              Driver infomation for port
 *     (>) pUpdate (const OcydUpdate *)  New parameter values
 *
 * FUNCTION VALUE: (long) status (0 = okay, non-zero = error)
 *
 * PURPOSE:  Update driver parameters.
 *
 * DESCRIPTION:
 *
 *     Changes driver parameter values.
 *
 * EXTERNAL VARIABLES:
 * 
 *     None.
 *
 * PRIOR REQUIREMENTS:
 *
 *     This should only be called after all standard EPICS record
 *     and driver initialization has completed.
 *
 * DEFICIENCIES:
 *
 *     Warning: Do not pass in a NULL pointer
 *-
 */

long
ocycSet(void *pArg, const OcycUpdate *pUpdate)
{
	Parms *const pParms = (Parms *)pArg;
	long status = 0;

#if DEBUG > 1
	fprintf(stderr, __FILE__ "(%d): ocycSet(%p,%p)\n",
		__LINE__, pArg, pUpdate);
#endif

	semTake(pParms->semUpdate, WAIT_FOREVER);

	if (pUpdate->flags & OCYC_UPDATE_SETPOINT) {
#if DEBUG > 0
		fprintf(stderr, __FILE__ "(%d): ocycSet() setting setPoint to %f\n",
			__LINE__, pUpdate->setPoint);
#endif
		pParms->values.pending = 1;
		pParms->update.setPoint = pUpdate->setPoint;
		pParms->update.flags |= OCYC_UPDATE_SETPOINT;
	}

	if (pUpdate->flags & OCYC_UPDATE_GAIN) {
#if DEBUG > 0
		fprintf(stderr, __FILE__ "(%d): ocycSet() setting gain to %f\n",
			__LINE__, pUpdate->gain);
#endif
		pParms->values.pending = 1;
		pParms->update.gain = pUpdate->gain;
		pParms->update.flags |= OCYC_UPDATE_GAIN;
	}

	if (pUpdate->flags & OCYC_UPDATE_RESET) {
#if DEBUG > 0
		fprintf(stderr, __FILE__ "(%d): ocycSet() setting reset to %f\n",
			__LINE__, pUpdate->reset);
#endif
		pParms->values.pending = 1;
		pParms->update.reset = pUpdate->reset;
		pParms->update.flags |= OCYC_UPDATE_RESET;
	}

	if (pUpdate->flags & OCYC_UPDATE_RATE) {
#if DEBUG > 0
		fprintf(stderr, __FILE__ "(%d): ocycSet() setting rate to %f\n",
			__LINE__, pUpdate->rate);
#endif
		pParms->values.pending = 1;
		pParms->update.rate = pUpdate->rate;
		pParms->update.flags |= OCYC_UPDATE_RATE;
	}

	if (pUpdate->flags & OCYC_UPDATE_RANGE) {
#if DEBUG > 0
		fprintf(stderr, __FILE__ "(%d): ocycSet() setting range to %d\n",
			__LINE__, pUpdate->range);
#endif
		pParms->values.pending = 1;
		pParms->update.range = pUpdate->range;
		pParms->update.flags |= OCYC_UPDATE_RANGE;
	}

	if (pUpdate->flags & OCYC_UPDATE_TUNE) {
#if DEBUG > 0
		fprintf(stderr, __FILE__ "(%d): ocycSet() setting tune to %d\n",
			__LINE__, pUpdate->tune);
#endif
		pParms->values.pending = 1;
		pParms->update.tune = pUpdate->tune;
		pParms->update.flags |= OCYC_UPDATE_TUNE;
	}

	semGive(pParms->semUpdate);

#if DEBUG > 1
	fprintf(stderr, __FILE__ "(%d): ocycSet() return %ld\n",
		__LINE__, status);
#endif

	return status;
}



/*
 *+
 * FUNCTION NAME: ocycGet
 *
 * INVOCATION: ocycGet(pParms)
 *
 * PARAMETERS:
 *
 * FUNCTION VALUE: (long) Returns status (0 = OK, non-zero = error)
 *
 * PURPOSE:  Read back parameters and status values.
 *
 * DESCRIPTION:
 *
 *     Returns a data structure that contains various parameters and
 *     current values for the temperature sensor.
 *
 * EXTERNAL VARIABLES:
 *
 *     None.
 *
 * PRIOR REQUIREMENTS:
 *
 *     This should only be called after all standard EPICS record
 *     and driver initialization has completed.
 *
 * DEFICIENCIES:
 *
 *     Warning: Do not pass in a NULL pointer.  The returned data
 *     structure must not be modified.
 *-
 */

const OcycValues *
ocycGet(void *pArg)
{
	return &((Parms *)pArg)->values;
}



/*
 *+
 * FUNCTION NAME: ocycTask
 *
 * INVOCATION: ocycTask(i)
 *
 * PARAMETERS:
 *
 *     (>) i (int) Which device to update
 *
 * FUNCTION VALUE: (long) Returns status (0 = okay, non-zero = error)
 *
 * PURPOSE: Update hardware values.
 *
 * DESCRIPTION:
 *
 *     This takes the new hardware parameters (set by ocycSet) and sets
 *     the hardware values.  It also reads the current hardware values.
 *
 * EXTERNAL VARIABLES:
 *     
 *     None.
 *
 * PRIOR REQUIREMENTS:
 *
 *     This is called by the driver initialization routine, and should
 *     not be called directly.
 *
 * DEFICIENCIES:
 *
 *     Units must be C (for now).
 *-
 */

/*
 * Note that fields in pp can be set _while_ this is processing a command,
 * so be careful to lock the parms data structure before accessing it,
 * be careful not to leave the parms data structure locked for too
 * long, and don't assume that it will remain unchanged.
 */

static void
ocycTask(int i)
{
	Parms *const pp = &parms[i];

	for (;;) {
		struct timespec ts;
		char buf[BUFSZ];
		OcycUpdate update;
		long status;
		int reread;

#if DEBUG > 1
		fprintf(stderr, "ocycTask(%d)\n", i);
#endif

		/* 
		 * Read flags and values periodically.  If new parameters were
		 * sent, reread all hardware values.  Also, if the current
		 * values are uninitialized or if there was an error, reread
		 * the hardware.
		 *
		 * The OCYC_UPDATE_PENDING flag is a bit of a kludge.  It's
		 * set only so that the hardware values will not be read
		 * into the record fields if a new value is about to be
		 * written to the hardware.
		 */

		semTake(pp->semUpdate, WAIT_FOREVER);
		update = pp->update;
		reread = pp->delay-- <= 0 || pp->update.flags || pp->values.status;
		if (pp->update.flags)
			pp->update.flags = 0;
		if (reread)
			pp->delay = UPDATE_SKIP; 
		semGive(pp->semUpdate);

		status = 0;

		/*
		 * Set parameters.
		 *
		 * Order is, of course, important.  If we are going to
		 * set new parameters, we want to set the desired value, 
		 * before reading the hardware values.  We also want to
		 * set the units before setting any values.
		 */

		/*
		 * Just in case someone has been playing around with
		 * the front panel, set the units appropriately.  This  
		 * command can be combined on a line with the next
		 * command, but the unit change doesn't seem to take
		 * effect soon enough if that is done.
		 */

		if (!status) {
			switch (pp->units) {
			case OCYC_UNITS_F:
				status = sendCmd(pp, "CUNI F", NULL, 0);
				break;

			case OCYC_UNITS_K:
				status = sendCmd(pp, "CUNI K", NULL, 0);
				break;

			default: /* OCYC_UNITS_C */
				status = sendCmd(pp, "CUNI C", NULL, 0);
			}

#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * Set the set-point temperature.
		 *
		 * Warning: This can affect the gain (and other parameters?
		 * depending on PID/PI/P/Manual mode?).  Do this early, so
		 * that we can read the new values back. 
		 */

		if (!status && (update.flags & OCYC_UPDATE_SETPOINT)) {
			sprintf(buf, "SETP %f", update.setPoint);
			status = sendCmd(pp, buf, buf, strlen(buf));
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif

			/*
			 * The SETP command takes a while to complete, which
			 * causes following commands to timeout.
			 */

			ts.tv_sec = 2;
			ts.tv_nsec = 0;
			nanosleep(&ts, NULL);
		}

		if (!status && reread) {
			status = sendCmd(pp, "SETP?", buf, sizeof(buf));
			if (!status)
				pp->values.setPoint = atof(buf);
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * Set the gain.
		 */

		if (!status && (update.flags & OCYC_UPDATE_GAIN)) {
			sprintf(buf, "GAIN %f", update.gain);
			status = sendCmd(pp, buf, buf, strlen(buf));
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		if (!status && reread) {
			status = sendCmd(pp, "GAIN?", buf, sizeof(buf));
			if (!status)
				pp->values.gain = atof(buf);
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * Set the reset.
		 */

		if (!status && (update.flags & OCYC_UPDATE_RESET)) {
			sprintf(buf, "RSET %f", update.reset);
			status = sendCmd(pp, buf, buf, strlen(buf));
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		if (!status && reread) {
			status = sendCmd(pp, "RSET?", buf, sizeof(buf));
			if (!status)
				pp->values.reset = atof(buf);
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * Set the rate.
		 */

		if (!status && (update.flags & OCYC_UPDATE_RATE)) {
			sprintf(buf, "RATE %f", update.rate);
			status = sendCmd(pp, buf, buf, strlen(buf));
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		if (!status && reread) {
			status = sendCmd(pp, "RATE?", buf, sizeof(buf));
			if (!status)
				pp->values.rate = atof(buf);
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * Set the range.
		 */

		if (!status && (update.flags & OCYC_UPDATE_RANGE)) {
			sprintf(buf, "RANG %d", update.range);
			status = sendCmd(pp, buf, buf, strlen(buf));
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		if (!status && reread) {
			status = sendCmd(pp, "RANG?", buf, sizeof(buf));
			if (!status)
				pp->values.range = atoi(buf);
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * Set the tuning.
		 */

		if (!status && (update.flags & OCYC_UPDATE_TUNE)) {
			sprintf(buf, "TUNE %d", update.tune);
			status = sendCmd(pp, buf, buf, strlen(buf));
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		if (!status && reread) {
			status = sendCmd(pp, "TUNE?", buf, sizeof(buf));
			if (!status)
				pp->values.tune = atoi(buf);
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * Read the current heater power.
		 */

		if (!status) {
			status = sendCmd(pp, "HEAT?", buf, sizeof(buf));
			if (!status)
				pp->values.heat = atof(buf);
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * Read the current temperature.
		 */

		if (!status) {
			status = sendCmd(pp, "CDAT?", buf, sizeof(buf));

			if (!status && strncmp(buf, " ER", 3) == 0) {
				status = S_ifa_ocyc_ErrorMsg;
#if defined(XY490)
				xy490SetError(pp->fd, "Controller error");
#endif
#if DEBUG > 0
				fprintf(stderr, __FILE__ "(%d): Controller error: %s\n",
					__LINE__, buf);
#endif
			}

			if (!status)
				pp->values.temperature = atof(buf);

#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ "(%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif
		}

		/*
		 * If there was a communications error, we want to make sure
		 * that any pending operations are completed, whenever the
		 * link starts to work again.  We should also reread the
		 * hardware parameters, just in case something was changed by
		 * a power-cycle.
		 */

		semTake(pp->semUpdate, WAIT_FOREVER);
		if (status)
			pp->update.flags |= update.flags;
		if (!pp->update.flags)
			pp->values.pending = 0;
		semGive(pp->semUpdate);

		/*
		 * The very last action.  We don't want to clear any existing 
		 * error status, until we're sure that all parameters have
		 * been properly initialized.
		 */

		pp->values.status = status;

		/*
		 * Don't update too frequently
		 */

		ts.tv_sec = UPDATE_RATE_S;
		ts.tv_nsec = UPDATE_RATE_NS;
		nanosleep(&ts, NULL);
	}
}

/*
 *+
 * FUNCTION NAME: sendCmd
 *
 * INVOCATION: sendCmd(pp, cmd, rpy, rpysz)
 *
 * PARAMETERS:
 *
 *     (>) pp    (Parms *) Which device to update
 *     (>) cmd   (const char []) Command to send
 *     (<) rpy   (char *) Reply value
 *     (>) rpysz (int) Size of reply buffer
 *
 * FUNCTION VALUE: (long) Returns status (0 = okay, non-zero = error)
 *
 * PURPOSE: Issue a command to the temperature controller.
 *
 * DESCRIPTION:
 *
 *     Write a command to the temperature controller serial port, and read
 *     the response back.  (Note:  Only commands that end with a '?'
 *     send a response.)
 *
 * EXTERNAL VARIABLES:
 *     
 *     None.
 *
 * PRIOR REQUIREMENTS:
 *
 *     The port must have been initialized already, by init().
 *
 * DEFICIENCIES:
 *-
 */

#if defined(XY490)

/*
 * Note: rpy must be non-null, and rpysz must be large enough to hold
 * the full reply.  This doesn't do a very good job of error trapping
 * if the reply is truncated, and can get out of sync.
 */

static int
sendCmd(Parms *pp, const char cmd[], char *rpy, int rpysz)
{
	long status;
	int retry = RETRY;
	struct timespec ts;

#if DEBUG > 5
	fprintf(stderr, __FILE__ "(%d): sendCmd(%p,\"%s\",%p,%d)\n",
		__LINE__, pp, cmd, rpy, rpysz);
#endif

	/*
	 * If two threads try to write to the same tty at the same time,
	 * they will probably end up confusing each other.  This semaphore
	 * prevents that.
	 */

	semTake(pp->semFd, WAIT_FOREVER);
	
	do {
		if (retry != RETRY) {
			ts.tv_sec = 0;
			ts.tv_nsec = 100000000;
			nanosleep(&ts, NULL); /* A little extra time, just to be safe */
#if DEBUG > 0 
			fprintf(stderr, __FILE__ "(%d): retry %d\n",
				__LINE__, RETRY - retry);
#endif
		}

		status = 0;

		/*
		 * Write the command, appending appropriate CR/LF
		 */

#if DEBUG > 5
		fprintf(stderr, "Write: fd=%d cmd=\"%s\"\n", pp->fd, cmd);
#endif

		if (!status) {
			if (xy490WriteString(pp->fd, cmd) == ERROR
					|| xy490WriteString(pp->fd, "\r\n") == ERROR) {
				xy490Reinit(pp->fd);
				(void)xy490SetBaud(pp->fd, BAUD_RATE);
				status = S_ifa_ocyc_Comm;
			}
		}

		/*
		 * If the command returns a response, read it, discarding the
		 * terminating \r\n
		 */

		if (!status && strchr(cmd, '?') != 0) {
			char *cp;

#if DEBUG > 5
			fprintf(stderr, "Reading into %p[%d]\n", rpy, rpysz);
#endif

			if (xy490Read(pp->fd, rpy, rpysz) == ERROR) {
				xy490FlushInput(pp->fd);
				xy490Reinit(pp->fd);
				(void)xy490SetBaud(pp->fd, BAUD_RATE);
				status = S_ifa_ocyc_Comm;
			}

			/*
			 * Get rid of trailing \r and/or \n
			 */

			if (!status && (cp = strchr(rpy, '\r')) != NULL)
				*cp = '\0';
			else if (!status && (cp = strchr(rpy, '\n')) != NULL)
				*cp = '\0';

#if DEBUG > 5
			fprintf(stderr, "Read \"%s\"\n", rpy);
#endif
		}
	} while (status && retry-- > 0);

	/*
	 * We're done with the tty
	 */

	semGive(pp->semFd);

#if DEBUG > 5
	fprintf(stderr, __FILE__ "(%d): sendCmd() return %d\n",
		__LINE__, status);
#endif

	return status;
}

#else /* Not XY490, assume tyLib */

static int
sendCmd(Parms *pp, const char cmd[], char *rpy, int rpysz)
{
	char buf[BUFSZ];
	long status = 0;

#if DEBUG > 5
	fprintf(stderr, __FILE__ "(%d): sendCmd(%p,\"%s\",%p,%d)\n",
		__LINE__, pp, cmd, rpy, rpysz);
#endif

	/*
	 * If two threads try to write to the same tty at the same time,
	 * they will probably end up confusing each other.  This semaphore
	 * prevents that.
	 */

	semTake(pp->semFd, WAIT_FOREVER);


	/*
	 * Write the command, appending a carriage return and line feed to the
	 * command
	 */

	if (!status) {
		if (write(pp->fd, (char *)cmd, strlen(cmd)) == ERROR
				|| write(pp->fd, "\r\n", 2) == ERROR)
			status = S_ifa_ocyc_Comm;
	}

	/*
	 * If the command returns a response, read it, discarding the terminating
	 * \r\n
	 */

	if (!status && strchr(cmd, '?') != 0) {
		char *cp;

#if DEBUG > 5
		fprintf(stderr, "Reading into %p[%d]\n", buf, sizeof(buf));
#endif

		for (cp = buf; cp < buf + rpysz;) {
			fd_set rfds;
			struct timeval timeout;
			int s;

			FD_ZERO(&rfds);
			FD_SET(pp->fd, &rfds);
			timeout.tv_sec = SERIAL_TIMEOUT_S;
			timeout.tv_usec = SERIAL_TIMEOUT_US;
			if ((s = select(FD_SETSIZE, &rfds, NULL, NULL, &timeout)) != 1) {
				fprintf(stderr, 
					"Communications failure on \"%s\" (\"%s\" failed).\n",
					pp->tty, cmd);
				status = S_ifa_ocyc_Comm;
				break;
			}

			read(pp->fd, cp, 1);
			if (*cp == '\n') /* Input is terminated by cr/lf */
				break;
			else if (*cp != '\r') /* Ignore carriage return */
				cp++;
		}

		/*
		 * Check for buffer overflow
		 */

		if (!status && cp == buf + rpysz) {
			fprintf(stderr, __FILE__ "(%d): sendCmd() Buffer overflow\n",
				__LINE__);
			status = S_ifa_ocyc_BufOverflow;
			*buf = '\0';
		} else {
			*cp = '\0';
		}

#if DEBUG > 5
		fprintf(stderr, "Read \"%s\"\n", buf);
#endif
	}

	if (rpy) {
		if (status) {
			*rpy = '\0';
		} else {
			strncpy(rpy, buf, rpysz);
			rpy[rpysz - 1] = '\0';
		}
	}

	/*
	 * We're done with the tty
	 */

	semGive(pp->semFd);

#if DEBUG > 5
	fprintf(stderr, __FILE__ "(%d): sendCmd() return %d\n", __LINE__, status);
#endif

	return status;
}

#endif /* XY490 or tyLib */

/*
 *+
 * FUNCTION NAME: ocycCmd
 *
 * INVOCATION: ocycCmd(id, cmd)
 *
 * PARAMETERS:
 *
 *     (>) id    (int)           Which device to update (index to parms array)
 *     (>) cmd   (const char []) Command to send
 *
 * FUNCTION VALUE: (long) Returns status (0 = okay, non-zero = error)
 *
 * PURPOSE: Issue a command to the temperature controller (vxWorks).
 *
 * DESCRIPTION:
 *
 *     This is intended to be used from the vxWorks prompt, not called
 *     like a subroutine.  It is a debugging tool that accepts commands,
 *     and writes them to the temperature-controller serial port.  If
 *     there is a response, it is displayed.
 *
 * EXTERNAL VARIABLES:
 *     
 *     semParms
 *     parms
 *
 * PRIOR REQUIREMENTS:
 *
 *     The port must have been initialized already, by init().
 *
 * DEFICIENCIES:
 *
 *     This is rather dangerous, and can put the temperature
 *     controller into an unexpected state.
 *-
 */

int
ocycCmd(int i, const char cmd[])
{
	int status = 0;
	char buf[BUFSZ];

	if (semParms == 0) {
		fprintf(stderr, "semParms is not initialized!\n");
	} else {
		semTake(semParms, WAIT_FOREVER);

		if (cmd == 0 || i < 0 || i > NPARM || *parms[i].tty == '\0') {
			for (i = 0; i < NPARM; i++) {
				if (*parms[i].tty != '\0')
					fprintf(stderr, "%2d: \"%s\"\n", i, parms[i].tty);
			}
		} else {
			status = sendCmd(&parms[i], cmd, buf, sizeof(buf));
			if (!status)
				fprintf(stderr, "Read \"%s\"\n", buf);
		}

		semGive(semParms);
	}

	return status;
}

void
ocycSetUnits(void *pArg, int units)
{
	Parms *const pParms = (Parms *)pArg;

	pParms->units = units;
}

#if defined(XY490) && (DEBUG > 0)

/*
 * For debugging only.  Try to respawn the task when it dies.
 */

void
ocycRespawn(int i)
{
	char buf[32];

	sprintf(buf, "ocycTask%d", i);
	taskSpawn(buf, 42, VX_FP_TASK, 10000, (FUNCPTR)ocycTask,
		i, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

#endif /* defined(XY490) && (DEBUG > 0) */
