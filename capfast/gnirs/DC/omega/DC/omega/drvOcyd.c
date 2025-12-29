static char rcsid[] = "$Id: drvOcyd.c,v 1.2 2009/05/27 19:32:35 fkraemer Exp $";

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
 *     drvOcyd.c
 *
 * FUNCTION NAME(S)
 */

#include "drvOcyd.h"

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

#include "ifaErrors.h"

#if !defined(DEBUG)
#	define DEBUG (0)
#endif

#define BAUD_RATE (300) /* Must be set to match the hardware */

/*
 * If XY490 is defined, communications takes place using the XY490 
 * serial board support of the built-in serial port.
 */

#define XY490

#if defined(XY490)
#	include "xy490.h"
#endif

typedef struct { /* derived from struct drvet */
	long number;         /* !REQ! number of support routines */
	DRVSUPFUN report;    /* !REQ! print report */
	DRVSUPFUN init;      /* !REQ! *init support */
	DEVSUPFUN reboot;    /* !REQ! init support for particular record */
/* End of standard fields */
} DrvOcyd;

static long init();
#define report ((DEVSUPFUN)0)
#define reboot ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)
DrvOcyd drvOcyd = {
	4,
	report,
	init,
	reboot,
};

/*
 * How long a timeout for serial communications?
 *
 * 2 seconds seems rather long, but the hardware doesn't seem to respond
 * for a little while after a parameter has been changed.
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

	SEM_ID semUpdate;           /* Protect update field */
	OcydUpdate update;          /* New values to set */

	OcydValues values;          /* Parameter values */
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
 * Debugging flag
 */

/*
 * Function declarations
 */

static void ocydTask(int i);
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
 * FUNCTION VALUE: (long) Returns status (0 = OK, non-zero = error)
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
 *     semParms (static)
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
 * FUNCTION NAME: ocydRecInit
 *
 * INVOCATION: ocydRecInit(pRec, id, pDpvt)
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
ocydRecInit(void *pRec, const char tty[], void **pDpvt)
{
	int i;
	long status = 0;

#if DEBUG > 1
	fprintf(stderr, __FILE__ " (%d): ocydRecInit(%p,\"%s\",%p)\n",
		__LINE__, pRec, tty, pDpvt);
#endif

	semTake(semParms, WAIT_FOREVER);

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
#if DEBUG > 1
		fprintf(stderr, "Opening tty \"%s\"\n", tty);
#endif

		for (i = 0; i < NPARM; i++) {
			if (*parms[i].tty == '\0')
				break;
		}

		if (i == NPARM) {
			status = S_dev_badSignal;
			recGblRecordError(S_dev_badInpType, pRec,
				"drvOcyd (ocydRecInit) Too many serial ports specified.");
		} else {
			char buf[BUFSZ];

			strncpy(parms[i].tty, tty, MAX_STRING_SIZE);
			parms[i].tty[MAX_STRING_SIZE - 1] = '\0';

			parms[i].semFd = semBCreate(SEM_Q_FIFO, SEM_FULL);

			parms[i].semUpdate = semBCreate(SEM_Q_FIFO, SEM_FULL);
			parms[i].update.flags = OCYD_SET_INIT;

			if (!status) {
#if DEBUG > 1
				fprintf(stderr, "Opening \"%s\"\n", parms[i].tty);
#endif
#if defined(XY490)
				parms[i].fd = atoi(parms[i].tty);
#else
				parms[i].fd = open(parms[i].tty, O_RDWR, 0);
#endif
#if DEBUG > 1
				fprintf(stderr, "Opened: Returned %d\n", parms[i].fd);
#endif

				if (parms[i].fd == ERROR) {
					status = S_dev_badSignal;
					recGblRecordError(S_dev_badInpType, pRec,
						"drvOcyd (ocydRecInit) Cannot open input.");
				}
			}

			if (!status) {
#if DEBUG > 1
				fprintf(stderr, "%s: Setting baud rate to %d.\n",
					tty, BAUD_RATE);
#endif
#if defined(XY490)
				if (xy490SetBaud(parms[i].fd, BAUD_RATE) == -1) {
					status = S_dev_badSignal;
					recGblRecordError(S_dev_badSignal, pRec,
						"drvOcyd (ocydRecInit) Cannot set baud rate.");
#if DEBUG > 1
				} else {
					fprintf(stderr, "Baud rate set\n");
#endif
				}
#else
				if (ioctl(parms[i].fd, FIOBAUDRATE, BAUD_RATE) == -1) {
					printErrno(errno);
					status = S_dev_badSignal;
					recGblRecordError(S_dev_badSignal, pRec,
						"drvOcyd (ocydRecInit) Cannot set baud rate.");
#if DEBUG > 0
				} else {
					fprintf(stderr, "Baud rate set\n");
#endif
				}
#endif
			}

#if !defined(XY490)
			if (!status) {
#if DEBUG > 0
				fprintf(stderr, "%s: Setting data bits to 7.\n", tty);
#endif
				if (ioctl(parms[i].fd, FIOSETOPTIONS, OPT_7_BIT) == -1) {
					printErrno(errno);
					status = S_dev_badSignal;
					recGblRecordError(S_dev_badSignal, pRec,
						"drvOcyd (ocydRecInit) Cannot set data bits.");
#if DEBUG > 0
				} else {
					fprintf(stderr, "Data bits set\n");
#endif
				}
			}
#endif

			/*
			 * After all other initialization has taken place,
			 * start the task to update the temperature reading.
			 */

			if (sprintf(buf, "ocydTask%d", i) > BUFSZ) {
				/* 
				 * We can't prevent a failure, so we try to detect
				 * it, and warn the programmer.
				 */
				fprintf(stderr, "Buffer overflow in " __FILE__ ":%d\n",
					__LINE__);
			}
			taskSpawn(buf, 42, VX_FP_TASK, 8000, (FUNCPTR)ocydTask,
				i, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		}

		*pDpvt = &parms[i];
	}

	semGive(semParms);

	return status;
}



/*
 *+
 * FUNCTION NAME: ocydSet
 *
 * INVOCATION: ocydSet(pArg, pUpdate)
 *
 * PARAMETERS:
 *
 *     (!) pArg    (void *)              Driver infomation for port
 *     (>) pUpdate (const OcydUpdate *)  New parameter values
 *
 * FUNCTION VALUE: (long) Returns status (0 = OK, non-zero = error)
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
ocydSet(void *pArg, const OcydUpdate *pUpdate)
{
	Parms *const pParms = (Parms *)pArg;
	long status = 0;
	int i;

#if DEBUG > 5
	fprintf(stderr, __FILE__ " (%d): ocydSet(%p,%p)\n",
		__LINE__, pArg, pUpdate);
#endif

	semTake(pParms->semUpdate, WAIT_FOREVER);
	for (i = 1; i <= OCYD_NCHAN; i++) {
		if (pUpdate->flags & OCYD_SET_ACTIVE(i))
			pParms->values.active[i] = pUpdate->active[i];
	}
	semGive(pParms->semUpdate);

#if DEBUG > 5
	fprintf(stderr, __FILE__ " (%d): ocydSet() return %ld\n", __LINE__, status);
#endif

	return status;
}



/*
 *+
 * FUNCTION NAME: ocydGet
 *
 * INVOCATION: ocydGet(pParms)
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
 *     current values for the temperture controller.
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

const OcydValues *
ocydGet(void *pArg)
{
	return &((Parms *)pArg)->values;
}



/*
 *+
 * FUNCTION NAME: ocydTask
 *
 * INVOCATION: ocydTask(i)
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
 *     This takes the new hardware parameters (set by ocydSet) and sets
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
ocydTask(int i)
{
	Parms *const pp = &parms[i];

	for (;;) {
		int j;
		struct timespec ts;
		char buf[BUFSZ];
		OcydUpdate update;
		long status;

		ts.tv_sec = UPDATE_RATE_S;
		ts.tv_nsec = UPDATE_RATE_NS;
		nanosleep(&ts, NULL);

		semTake(pp->semUpdate, WAIT_FOREVER);
		update = pp->update;
		pp->update.flags = 0;
		semGive(pp->semUpdate);

		status = 0;

		/*
		 * Set parameters.
		 *
		 * Order is, of course, important.  If we are going to
		 * set new parameters, we want to set the desired value,
		 * before reading the hardware values.
		 */

		/*
		 * Just in case someone has been playing around with
		 * the front panel, set the units appropriately.  This
		 * command can be combined on a line with the next
		 * command, but the unit change doesn't seem to take
		 * effect soon enough if that is done.
		 */

		if (!status)
			switch (pp->units) {
			case OCYD_UNITS_K:
				status = sendCmd(pp, "F0K", buf, sizeof(buf));
				break;

			case OCYD_UNITS_F:
				status = sendCmd(pp, "F0F", buf, sizeof(buf));
				break;

			default: /* OCYD_UNITS_C */
				status = sendCmd(pp, "F0C", buf, sizeof(buf));
				break;
			}
#if DEBUG > 0 && defined(XY490)
			if (status) {
				fprintf(stderr, __FILE__ " (%d): %s\n",
					__LINE__, xy490GetError(pp->fd));
				xy490PrintfKludge();
			}
#endif


		/*
		 * Read the current temperatures.
		 */

		if (!status) {
			struct timespec ts;

			/*
			 * Give the units setting time to respond
			 */

			ts.tv_sec = 1;
			ts.tv_nsec = 0;
			nanosleep(&ts, 0);

			for (j = 1; j <= OCYD_NCHAN; j++) {
				if (!pp->values.active[j])
					continue;

				if (sprintf(buf, "Y%d5", j) > BUFSZ) {
					/* 
					 * We can't prevent a failure, so we try to detect
					 * it, and warn the programmer.
					 */
					fprintf(stderr, __FILE__ " (%d): Buffer overflow\n",
						__LINE__);
				}
				sendCmd(pp, buf, buf, sizeof(buf));
#if DEBUG > 0 && defined(XY490)
				if (status) {
					fprintf(stderr, __FILE__ " (%d): %s\n",
						__LINE__, xy490GetError(pp->fd));
					xy490PrintfKludge();
				}
#endif

				if (sprintf(buf, "YC%d", j) > BUFSZ) {
					/* 
					 * We can't prevent a failure, so we try to detect
					 * it, and warn the programmer.
					 */
					fprintf(stderr, __FILE__ " (%d): Buffer overflow\n",
						__LINE__);
				}
				sendCmd(pp, buf, buf, sizeof(buf));
#if DEBUG > 0 && defined(XY490)
				if (status) {
					fprintf(stderr, __FILE__ " (%d): %s\n",
						__LINE__, xy490GetError(pp->fd));
					xy490PrintfKludge();
				}
#endif

				{
					struct timespec ts;
					ts.tv_sec = 6; /* Takes 5 seconds to register temperature */
					ts.tv_nsec = 0;
					nanosleep(&ts, 0);
				}

				if (!status) {
					status = sendCmd(pp, "WS", buf, sizeof(buf));
#if DEBUG > 0 && defined(XY490)
					if (status) { 
						fprintf(stderr, __FILE__ " (%d): %s\n",
							__LINE__, xy490GetError(pp->fd));
						xy490PrintfKludge();
					}
#endif
					if (!status) {
#if DEBUG > 2
						fprintf(stderr, __FILE__ "(%d): buf=\"%s\"\n",
							__LINE__, buf);
#if defined(XY490)
						xy490PrintfKludge();
#endif
#endif
						/*
						 * I think Err10 is a parity or other RS-232 error;
						 * It appears to be harmless.
						 */

						if (strlen(buf) > 7 && strncmp(buf, "Err10", 4) == 0)
							pp->values.tmp[j] = atof(buf + 7);
						else
							pp->values.tmp[j] = atof(buf + 2);
#if DEBUG > 2
						fprintf(stderr, __FILE__ "(%d): T%d=%g\n",
							__LINE__, j, pp->values.tmp[j]);
#if defined(XY490)
						xy490PrintfKludge();
#endif
#endif
					}
				}
			}
		}

		/*
		 * If there was a communications error, we want to make sure
		 * that any pending operations are completed, whenever the
		 * link starts to work again.  We should also reread the
		 * hardware parameters, just in case something was changed by
		 * a power-cycle.
		 */

		if (status) {
			semTake(pp->semUpdate, WAIT_FOREVER);
			pp->update.flags |= update.flags | OCYD_SET_INIT;
			semGive(pp->semUpdate);
		}

		ioctl(pp->fd, FIOFLUSH, 0);

		pp->values.status = status;
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
 * PURPOSE: Issue a command to the temperature sensor.
 *
 * DESCRIPTION:
 *
 *     Write a command to the temperature sensor serial port, and read
 *     the response back.  (Note:  Only commands that begin with a 'W'
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
	fprintf(stderr, __FILE__ " (%d): sendCmd(%p,\"%s\",%p,%d)\n",
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
			nanosleep(&ts, NULL); /* A little extra time, to be safe */

#if DEBUG > 0
			fprintf(stderr, __FILE__ " (%d): retry %d\n",
				__LINE__, RETRY - retry);
#endif
		}

		status = 0;

		/*
		 * Write the command, appending appropriate CR/LF.
		 */

#if DEBUG > 5
		fprintf(stderr, __FILE__ " (%d): Write: fd=%d cmd=\"%s\"\n",
			__LINE__, pp->fd, cmd);
#endif

		if (!status) {
			if (xy490WriteString(pp->fd, cmd) == ERROR
					|| xy490WriteString(pp->fd, "\r\n") == ERROR) {
				xy490Reinit(pp->fd);
				(void)xy490SetBaud(pp->fd, BAUD_RATE);
				status = S_ifa_ocyd_Comm;
			}
		}

		/*
		 * If the command returns a response, read it, discarding the
		 * terminating \r\n
		 */

		if (!status && *cmd == 'W') {
			char *cp;

#if DEBUG > 5
			fprintf(stderr, __FILE__ "(%d): Reading into %p[%d]\n",
				__LINE__, rpy, rpysz);
#endif

			if (xy490Read(pp->fd, rpy, rpysz) == ERROR) {
				xy490FlushInput(pp->fd);
				xy490Reinit(pp->fd);
				(void)xy490SetBaud(pp->fd, BAUD_RATE);
				status = S_ifa_ocyd_Comm;
			}

			/*
			 * Get rid of trailing \r and/or \n
			 */

			if (!status && (cp = strchr(rpy, '\r')) != NULL)
				*cp = '\0';
			else if (!status && (cp = strchr(rpy, '\n')) != NULL)
				*cp = '\0';

#if DEBUG > 5
			fprintf(stderr, __FILE__ "(%d): Read \"%s\"\n",
				__LINE__, rpy);
#endif
		}
	} while (status && retry-- > 0);

	/*
	 * We're done with the tty
	 */

	semGive(pp->semFd);

#if DEBUG > 5
	fprintf(stderr, __FILE__ " (%d): sendCmd() return %d\n",
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
	fprintf(stderr, __FILE__ " (%d): sendCmd(%p,\"%s\",%p,%d)\n",
		__LINE__, pp, cmd, rpy, rpysz);
#endif

	/*
	 * If two threads try to write to the same tty at the same time,
	 * they will probably end up confusing each other.  This semaphore
	 * prevents that.
	 */

	semTake(pp->semFd, WAIT_FOREVER);

	/*
	 * Write the command, appending appropriate CR/LF.
	 */

	if (!status) {
		write(pp->fd, (char *)cmd, strlen(cmd));
		write(pp->fd, "\r\n", 2);
	}

	/*
	 * If the command returns a response, read it, discarding the terminating
	 * \r\n
	 */

	if (!status && *cmd == 'W') {
		char *cp;

#if DEBUG > 5
		fprintf(stderr, __FILE__ " (%d): Reading into %p[%d]\n",
			__LINE__, buf, sizeof(buf));
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
					__FILE__ "(%d): Comm failure on \"%s\" (\"%s\" failed).\n",
					__LINE__, pp->tty, cmd);
				status = S_ifa_ocyd_Comm;
				break;
			}

			read(pp->fd, cp, 1);
			if (*cp == '\n') /* Input is terminated by cr/lf */
				break;
			else if (*cp != '\r') /* Ignore carriage return */
				cp++;
		}

		/*
		 * Check for buffer overflow.
		 */

		if (!status && cp == buf + rpysz) {
			fprintf(stderr, __FILE__ " (%d): sendCmd() Buffer overflow\n",
				__LINE__);
			status = S_ifa_ocyd_BufOverflow;
			*buf = '\0';
		} else {
			*cp = '\0';
		}

#if DEBUG > 5
		fprintf(stderr, __FILE__ " (%d): Read \"%s\"\n", __LINE__, buf);
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
	fprintf(stderr, __FILE__ " (%d): sendCmd() return %d\n", __LINE__, status);
#endif

	return status;
}

#endif

/*
 *+
 * FUNCTION NAME: ocydCmd
 *
 * INVOCATION: ocydCmd(id, cmd)
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

/*
 * Command line interface--for debugging only.  Warning: this can confuse
 * the software, by setting hardware parameters to an unexpected
 * value; use with caution.
 */

int
ocydCmd(int i, const char cmd[])
{
	int status = 0;
	char buf[BUFSZ];

	if (semParms == 0) {
		fprintf(stderr, __FILE__ " (%d): semParms is not initialized!\n",
			__LINE__);
	} else {
		semTake(semParms, WAIT_FOREVER);

		if (cmd == 0 || i < 0 || i > NPARM || *parms[i].tty == '\0') {
			for (i = 0; i < NPARM; i++) {
				if (*parms[i].tty != '\0')
					fprintf(stderr, __FILE__ "(%d): %2d: \"%s\"\n",
						__LINE__, i, parms[i].tty);
			}
		} else {
			status = sendCmd(&parms[i], cmd, buf, sizeof(buf));
			if (!status)
				fprintf(stderr, __FILE__ "(%d): Read \"%s\"\n",
					__LINE__, buf);
		}

		semGive(semParms);
	}

	return status;
}

void
ocydSetUnits(void *pArg, int units)
{
	Parms *const pParms = (Parms *)pArg;

	pParms->units = units;
}
