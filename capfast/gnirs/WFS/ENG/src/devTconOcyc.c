static char rcsid[] = "$Id: devTconOcyc.c,v 1.2 2009/05/27 19:34:42 fkraemer Exp $";

/*
 * Copyright 1997 University of Hawaii, Institute for Astronomy.  All
 * rights reserved.
 *
 * The University of Hawaii grants AURA a non-exclusive license to 
 * use this software, as stated in [contract].
 *
 * Author: Hubert Yamada, University of Hawaii, Institute for Astronomy
 *
 * Device Support Routines for Hall-Effect/Stepper-motor control record,
 * wheel-like component.
 *
 * FILENAME
 *     devTconOcyc.c
 *
 * FUNCTION NAME(S)
 */

#include <sys/types.h>
#include <dbCommon.h>
#include <dbEvent.h>
#include <recSup.h>
#include <devSup.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <logLib.h>

#include <tconRecord.h>
#include <choiceTcon.h>
#include "recTcon.h"
#include "drvOcyc.h"

#if !defined(DEBUG)
#	define DEBUG (0)
#endif

typedef struct {
	long number;
	DEVSUPFUN report;
	DEVSUPFUN init;
	DEVSUPFUN tcon_init_record;
	DEVSUPFUN get_ioint_info;
	DEVSUPFUN read_tcon;
	DEVSUPFUN set_units;
} DevTconOcyc;

static long read_tcon(struct tconRecord *);
static long tcon_init_record(struct tconRecord *);
static long set_units(struct tconRecord *);
#define init ((DEVSUPFUN)0)
#define report ((DEVSUPFUN)0)
#define get_ioint_info ((DEVSUPFUN)0)

DevTconOcyc devTconOcyc = {
	7,
	report,
	init,
	tcon_init_record,
	get_ioint_info,
	read_tcon,
	set_units,
};



/*
 *+
 * FUNCTION NAME: read_tcon
 *
 * INVOCATION: read_tcon(pTcon)
 *
 * PARAMETERS:
 *
 *     (!) pTcon (struct tconRecord *)
 *
 * FUNCTION VALUE:
 *
 * PURPOSE:
 *
 * DESCRIPTION:
 *
 * EXTERNAL VARIABLES:
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *-
 */

static long
read_tcon(struct tconRecord *pTcon)
{
	long status = 0;

#if DEBUG > 0
	fprintf(stderr, __FILE__ "(%d): read_tcon(%p)\n", __LINE__, pTcon);
#endif

	if (pTcon && pTcon->dpvt) {
		const OcycValues *pValues;

		pValues = ocycGet(pTcon->dpvt);
		pTcon->val = !!pValues->status;

		if (!pValues->status) {
			OcycUpdate update;
			update.flags = 0;

			pTcon->udf = FALSE;

			/*
			 * Update the gain, rate, reset, and set point.  They values can
			 * be changed via channel access, in which case the last value
			 * will not agree with the current value.  Also, the hardware 
			 * can spontanously change some values (i.e., the gain, in
			 * PID autotune mode).
			 */

			if (pTcon->rate != pTcon->lrte) { /* Changed via CA */
				update.flags |= OCYC_UPDATE_RATE;
				update.rate = pTcon->rate;
				pTcon->lrte = pTcon->rate;
			} else if (pValues->rate != pTcon->rate && !pValues->pending) {
				pTcon->lrte = pTcon->rate = pValues->rate; 
				pTcon->mon |= TCON_MONITOR_RATE;
			}

			if (pTcon->setp != pTcon->lstp) { /* Changed via CA */
				update.flags |= OCYC_UPDATE_SETPOINT;
				update.setPoint = pTcon->setp;
				pTcon->lstp = pTcon->setp;
			} else if (pValues->setPoint != pTcon->setp && !pValues->pending) {
				pTcon->lstp = pTcon->setp = pValues->setPoint;
				pTcon->mon |= TCON_MONITOR_SETP;
			}

			if (pTcon->gain != pTcon->lgan) { /* Changed via CA */
				update.flags |= OCYC_UPDATE_GAIN;
				update.gain = pTcon->gain;
				pTcon->lgan = pTcon->gain;
			} else if (pValues->gain != pTcon->gain && !pValues->pending) {
				pTcon->lgan = pTcon->gain = pValues->gain;
				pTcon->mon |= TCON_MONITOR_GAIN;
			}

			if (pTcon->rst != pTcon->lrst) { /* Changed via CA */
				update.flags |= OCYC_UPDATE_RESET;
				update.reset = pTcon->rst;
				pTcon->lrst = pTcon->rst;
			} else if (pValues->reset != pTcon->rst && !pValues->pending) {
				pTcon->lrst = pTcon->rst = pValues->reset;
				pTcon->mon |= TCON_MONITOR_RST;
			}

			if (pTcon->rang != pTcon->lrng) { /* Changed via CA */
				switch (pTcon->rang) {
				case TCON_RANG_OFF:
					update.flags |= OCYC_UPDATE_RANGE;
					update.range = OCYC_RANGE_OFF;
					pTcon->lrng = pTcon->rang;
					break;

				case TCON_RANG_LOW:
					update.flags |= OCYC_UPDATE_RANGE;
					update.range = OCYC_RANGE_LOW;
					pTcon->lrng = pTcon->rang;
					break;

				case TCON_RANG_HIGH:
					update.flags |= OCYC_UPDATE_RANGE;
					update.range = OCYC_RANGE_HIGH;
					pTcon->lrng = pTcon->rang;
					break;
				}
			} else if (!pValues->pending) { /* HW _might_ have changed */
				switch (pValues->range) {
				case OCYC_RANGE_OFF:
					if (pTcon->rang != TCON_RANG_OFF) {
						pTcon->lrng = pTcon->rang = TCON_RANG_OFF;
						pTcon->mon |= TCON_MONITOR_RANG;
					}
					break;

				case OCYC_RANGE_LOW:
					if (pTcon->rang != TCON_RANG_LOW) {
						pTcon->lrng = pTcon->rang = TCON_RANG_LOW;
						pTcon->mon |= TCON_MONITOR_RANG;
					}
					break;

				case OCYC_RANGE_HIGH:
					if (pTcon->rang != TCON_RANG_HIGH) {
						pTcon->lrng = pTcon->rang = TCON_RANG_HIGH;
						pTcon->mon |= TCON_MONITOR_RANG;
					}
					break;
				}
			}

			if (pTcon->tune != pTcon->ltun) { /* Changed via CA */
				switch (pTcon->tune) {
				case TCON_TUNE_MANUAL:
					update.flags |= OCYC_UPDATE_TUNE;
					update.tune = OCYC_TUNE_MANUAL;
					pTcon->ltun = pTcon->tune;
					break;

				case TCON_TUNE_P:
					update.flags |= OCYC_UPDATE_TUNE;
					update.tune = OCYC_TUNE_P;
					pTcon->ltun = pTcon->tune;
					break;

				case TCON_TUNE_PI:
					update.flags |= OCYC_UPDATE_TUNE;
					update.tune = OCYC_TUNE_PI;
					pTcon->ltun = pTcon->tune;
					break;

				case TCON_TUNE_PID:
					update.flags |= OCYC_UPDATE_TUNE;
					update.tune = OCYC_TUNE_PID;
					pTcon->ltun = pTcon->tune;
					break;
				}
			} else if (!pValues->pending) {
				switch (pValues->tune) {
				case OCYC_TUNE_MANUAL:
					pTcon->ltun = pTcon->tune = TCON_TUNE_MANUAL;
					pTcon->mon |= TCON_MONITOR_TUNE;
					break;

				case OCYC_TUNE_P:
					pTcon->ltun = pTcon->tune = TCON_TUNE_P;
					pTcon->mon |= TCON_MONITOR_TUNE;
					break;

				case OCYC_TUNE_PI:
					pTcon->ltun = pTcon->tune = TCON_TUNE_PI;
					pTcon->mon |= TCON_MONITOR_TUNE;
					break;

				case OCYC_TUNE_PID:
					pTcon->ltun = pTcon->tune = TCON_TUNE_PID;
					pTcon->mon |= TCON_MONITOR_TUNE;
					break;
				}
			}

			if (!status && update.flags)
				status = ocycSet(pTcon->dpvt, &update);
		}
	}

	return 0;
}

static long
tcon_init_record(struct tconRecord *pTcon)
{
	long status = 0;

#if DEBUG > 0
	fprintf(stderr, __FILE__ "(%d): tcon_init_record(%p)\n", __LINE__, pTcon);
#endif

	pTcon->udf = TRUE;

	/**************************************************************
	 * Find the data field
	 **************************************************************/

	/*
	 * ai.out must be an INST_IO
	 */

	if (!status && pTcon->out.type != INST_IO) {
		status = S_dev_badInpType;
		recGblRecordError(S_dev_badInpType, (void *)pTcon,
			__FILE__ "::tcon_init_record Illegal INP field type.");
	}

	if (!status) {
		status = ocycRecInit(pTcon, pTcon->out.value.instio.string, 
			&pTcon->dpvt);
	}

	return status;
}

static long
set_units(struct tconRecord *pTcon)
{
	switch (pTcon->egu) {
	case TCON_UNITS_K:
		ocycSetUnits(pTcon->dpvt, OCYC_UNITS_K);
		break;

	case TCON_UNITS_F:
		ocycSetUnits(pTcon->dpvt, OCYC_UNITS_F);
		break;

	default:
		ocycSetUnits(pTcon->dpvt, OCYC_UNITS_C);
		break;
	}

	return 0;
}
