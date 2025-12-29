static char rcsid[]="$Id: engLib.c,v 1.2 2009/05/27 19:34:57 fkraemer Exp $";

/*
 * This module provides an interface to the health and status of the 
 * engineering database.
 *
 * Author: Hubert Yamada
 * Original Version: 1999-07-30
 * 
 * Revision History:
 *INDENT-OFF*
 *
 * $Log: engLib.c,v $
 * Revision 1.2  2009/05/27 19:34:57  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.6  2000/04/25 02:25:23  yamada
 * Added support for current motor wiring and ignores flakey sensor.
 *
 * Revision 1.5  1999/11/23 23:24:30  yamada
 * ICD's are completely revised to fix record names, add missing
 * records, add record descriptions, change out-of-date information, etc.
 * Increased size of temperture description fields and shorted descriptions.
 *
 * Revision 1.4  1999/11/14 02:05:42  yamada
 * Reformatted logs.
 *
 *
 *INDENT-ON*
 */

#include <devSup.h>
#include <rec/genSubRecord.h>
#include <rec/sirRecord.h>
#include <recHallStep.h>
#include <cicsConst.h>
#include <stdlib.h>
#include <string.h>

/*
 * Input:
 *
 * B:  Hall sensor enable (1p)
 * D:  Hall sensor enable (1b)
 * F:  Hall sensor enable (2p)
 * H:  Hall sensor enable (2b)
 *
 * Output:
 *
 * E:  Hall sensor status (GOOD/WARN/BAD)
 * F:  Hall sensor message
 *
 * Processing routine for a gensub record.  This is not intended to
 * be called directly.
 */

long
engHallHealth(struct genSubRecord *pGenSub)
{
	/******************************************************************
	 * Convert the input values into more human friendly form.  These
	 * should be listed in decreasing order of importance.
	 ******************************************************************/

	/*
	 * Convert the sensor enables into a warning, and output it.
	 */

	if ((!*(long *)pGenSub->b && !*(long *)pGenSub->d)
			|| (!*(long *)pGenSub->f && !*(long *)pGenSub->h)) {
		strcpy(pGenSub->vala, "BAD");
		strcpy(pGenSub->valb, "Sensor pair disabled");
	} else if (!*(long *)pGenSub->b || !*(long *)pGenSub->d 
			|| (!*(long *)pGenSub->f || !*(long *)pGenSub->h)) {
		strcpy(pGenSub->vala, "WARNING");
		strcpy(pGenSub->valb, "Sensor disabled");
	} else {
		strcpy(pGenSub->vala, "GOOD");
		strcpy(pGenSub->valb, "");
	}

	return PASS;
}

/*
 * Convert the motor driver module status into a health value
 */

int
engModuleHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "BAD");
		strcpy(pSir->imss, "Module fault");
	} else {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

/*
 * Convert the Datum Health into a health value.
 */

int
engDatumHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	} else { 
		strcpy(pSir->val, "WARNING");
		strcpy(pSir->imss, "Gnirs OIWFS Not datumed");
	}

	return PASS;
}

/*
 * Convert the Backlash Health into a health value.
 */

int
engBacklashHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "WARNING");
		strcpy(pSir->imss, "Backlash");
	} else { 
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

/*
 * Initialization routine for a gensub record.  This is not intended to
 * be called directly.
 */

int
engParkInit(struct genSubRecord *pGenSub)
{
	return PASS;
}

/*
 * Processing routine for a gensub record.  This is not intended to
 * be called directly.
 *
 * Convert the current position into a PARK value.
 *
 * Input:
 *     A: Current position (string)
 *     B: Currently datumed? (long)
 *     C: Park string 0
 *     D: Park string 1
 *     E: Park string 2
 *     F: Park string 3
 *
 * Output:
 *     A: 0/1 (Not parked/Parked)
 */

int
engParkSub(struct genSubRecord *pGenSub)
{
	if (!*(long *)pGenSub->b) { /* Not datumed, so might not be parked */
		*(long *)pGenSub->vala = EPICS_FALSE;
	} else if (strcmp((char *)pGenSub->a, (char *)pGenSub->c) == 0
			|| strcmp((char *)pGenSub->a, (char *)pGenSub->d) == 0
			|| strcmp((char *)pGenSub->a, (char *)pGenSub->e) == 0
			|| strcmp((char *)pGenSub->a, (char *)pGenSub->f) == 0) {
		*(long *)pGenSub->vala = EPICS_TRUE;
	} else {
		*(long *)pGenSub->vala = EPICS_FALSE;
	}

	return PASS;
}

/*
 * Initialization routine for a gensub record.  This is not intended to
 * be called directly.
 */

int
engTmpInit(struct genSubRecord *pGenSub)
{
	return PASS;
}

/*
 * Processing routine for a gensub record.  This is not intended to
 * be called directly.
 *
 * Convert the current position into a PARK value.
 *
 * Input:
 *     A: Temperature Input
 *     B: Temperature Input
 *     C: Temperature Input
 *     D: Temperature Input
 *     E: Temperature Input
 *     F: Temperature Input
 *     G: Temperature Input
 *     H: (Reserved for Temperature Input)
 *     I: (Reserved for Temperature Input)
 *     J: (Reserved for Temperature Input)
 *     K: (Reserved for Temperature Input)
 *     M: Temperature state (WARM, COLD, CHANGING)
 *     N: Cooling motors (Off, Low, High)
 *
 * Output:
 *     A: Health: Temperature 
 *     B: Message: Temperature 
 *     C: Health: Cooling
 *     D: Message: Cooling
 */

#define NTMP (6)

/*
 * These values should really be in some global configuration file,
 * but they are unlikely to need to change.
 */

#define HIGHTEMP (250.0) /* Threshold for low temperature warning */
#define LOWTEMP  (90.0)  /* Threshold for high temperature warning */

int
engTmpSub(struct genSubRecord *pGenSub)
{
	double tmps[NTMP];
	double minTmp, maxTmp;
	int i;

	tmps[0] = *(double *)pGenSub->a;
	tmps[1] = *(double *)pGenSub->b;
	tmps[2] = *(double *)pGenSub->c;
	tmps[3] = *(double *)pGenSub->d;
	tmps[4] = *(double *)pGenSub->e;
	tmps[5] = *(double *)pGenSub->f; /* index Must be NTMP - 1 */

	minTmp = tmps[0];
	maxTmp = tmps[0];
	for (i = 0; i < NTMP; i++) {
		if (tmps[i] < minTmp)
			minTmp = tmps[i];

		if (tmps[i] > maxTmp)
			maxTmp = tmps[i];
	}

	*(double *)pGenSub->vala = minTmp;
	*(double *)pGenSub->valc = maxTmp;

	/*
	 * This insures that the user is warned if the temperature state is
	 * inconsistent with the actual temperature.  It would be pretty
	 * easy to set the temperature state here, but then the state would
	 * start doing unexpected things if the temperature sensor is
	 * removed.
	 */

	if (strcmp((char *)pGenSub->m, "COLD") == 0 && maxTmp > LOWTEMP) {
		strcpy((char *)pGenSub->vala, "BAD");
		strcpy((char *)pGenSub->valb, "Overheating");
	} else if (strcmp((char *)pGenSub->m, "WARM") == 0 && minTmp < HIGHTEMP) {
		strcpy((char *)pGenSub->vala, "BAD");
		strcpy((char *)pGenSub->valb, "Too cold");
	} else {
		strcpy((char *)pGenSub->vala, "GOOD");
		strcpy((char *)pGenSub->valb, "");
	}

	/* 
	 * If the temperature is WARM, then the cooling motors should be
	 * off.  If the temperature is COLD, then the cooling motors
	 * should be on.  If the temperature is CHANGING, then we are
	 * either heating or cooling.
	 */

	if (strcmp(pGenSub->m, "COLD") == 0 && strcmp(pGenSub->n, "Off") == 0) {
		strcpy(pGenSub->valc, "BAD");
		strcpy(pGenSub->vald, "Coolers are OFF");
	} else if (strcmp(pGenSub->m, "WARM") == 0 
			&& strcmp(pGenSub->n, "Off") != 0) {
		strcpy(pGenSub->valc, "BAD");
		strcpy(pGenSub->vald, "Coolers are ON");
	} else {
		strcpy(pGenSub->valc, "GOOD");
		strcpy(pGenSub->vald, "");
	}

	return PASS;
}

int
engLockObsHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "WARNING");
		strcpy(pSir->imss, "Locked for Observation");
	} else {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

int
engLockGenHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "WARNING");
		strcpy(pSir->imss, "Unspecified Interlock");
	} else {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

int
engLockCfgHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "WARNING");
		strcpy(pSir->imss, "Locked for Configuration");
	} else {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

int
engLockInitHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "BAD");
		strcpy(pSir->imss, "Init incomplete");
	} else {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

int
engLockTmpHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval) == HS_TMP_CHANGING) {
		strcpy(pSir->val, "BAD");
		strcpy(pSir->imss, "Temperature changing");
	} else {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

int
engLockTmpHBHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "BAD");
		strcpy(pSir->imss, "Communications failure");
	} else {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

int
engLockTmpBusyHealth(struct sirRecord *pSir)
{
	if (atoi(pSir->rval)) {
		strcpy(pSir->val, "WARNING");
		strcpy(pSir->imss, "Updating temperature");
	} else {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	}

	return PASS;
}

int
engMode(struct sirRecord *pSir)
{
	if (strcmp("Normal", pSir->rval) == 0) {
		strcpy(pSir->val, "GOOD");
		strcpy(pSir->imss, "");
	} else if (strcmp("Acc Warm", pSir->rval) == 0) {
		strcpy(pSir->val, "BAD");
		strcpy(pSir->imss, "Warm-Up Mode");
	} else if (strcmp("ERROR", pSir->rval) == 0){
		strcpy(pSir->val, "BAD");
		strcpy(pSir->imss, "Error Mode");
	} else {
		strcpy(pSir->val, "BAD");
		strcpy(pSir->imss, "Invalid Mode");
	}

	return PASS;
}

/*
 * Action subroutine for a gensub record.  Examines the current state of
 * a record.
 *
 * Returns EPICS_TRUE in VALA if the mechanism is in follow mode, and
 * EPICS_FALSE otherwise.  Assumes that all states which are part of
 * follow mode begin with "FOLLOW+", and that no other states begin
 * with that string.
 *
 * Returns EPICS_TRUE in VALB if the mechanism is in an idle mode, and
 * EPICS_FALSE otherwise.  Assumes that all idle states end with "+IDLE",
 * and no other states end with that string.
 */

int
engAgModeSub(struct genSubRecord *pGenSub)
{
	const int len1 = strlen(pGenSub->a);
	const char *const end1 = strchr(pGenSub->a, '\0');
	const int len2 = strlen(pGenSub->b);
	const char *const end2 = strchr(pGenSub->b, '\0');
	const int len3 = strlen(pGenSub->c);
	const char *const end3 = strchr(pGenSub->c, '\0');

	if (strncmp("FOLLOW+", pGenSub->a, 7) == 0
			&& strncmp("FOLLOW+", pGenSub->b, 7) == 0
			&& strncmp("FOLLOW+", pGenSub->c, 7) == 0) {
		*(long *)pGenSub->vala = EPICS_TRUE;
	} else {
		*(long *)pGenSub->vala = EPICS_FALSE;
	}

	if (len1 > 5 && strcmp(end1 - 5, "+IDLE") == 0
			&& len2 > 5 && strcmp(end2 - 5, "+IDLE") == 0
			&& len3 > 5 && strcmp(end3 - 5, "+IDLE") == 0) {
		*(long *)pGenSub->valb = EPICS_TRUE;
	} else {
		*(long *)pGenSub->valb = EPICS_FALSE;
	}

	return PASS;
}

int
SIRengSensHealth(struct sirRecord *psir)
{
	if (atoi(psir->rval)) {
		strcpy(psir->val, "BAD");
		strcpy(psir->imss, "Temp sensor failure");
	} else {
		strcpy(psir->val, "GOOD");
		strcpy(psir->imss, "");
	}

	return 0;
}

int
SIRengCtlrHealth(struct sirRecord *psir)
{
	if (atoi(psir->rval)) {
		strcpy(psir->val, "BAD");
		strcpy(psir->imss, "Temp controller failure");
	} else {
		strcpy(psir->val, "GOOD");
		strcpy(psir->imss, "");
	}

	return 0;
}
