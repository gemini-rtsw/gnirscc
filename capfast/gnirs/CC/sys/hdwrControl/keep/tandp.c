static struct
  {
      void *v;
      char *c;
  }
sccsid =
{
    &sccsid,
        "%W% %G%"
};

#include <vxWorks.h>
#include <stdio.h>
#include <fcntl.h>
#include <ioLib.h>
#include <vme.h>
#include <memLib.h>
#include <usrLib.h>             /* Debugging */
#include <cacheLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <intLib.h>
#include <logLib.h>
#include <iv.h>
#include <vxLib.h>
#include <ctype.h>
#include "stdarg.h"
#include "gnirsCC.h"
#include "timeLib.h"
#include "epicsTypes.h"

/*
 *
 * TEMPERATURE MONITORING
 *
 */


/*     
 *      Dewar Temperature readback board
 *
 *  This file contains the source for accessing the temperature
 * monitoring board located in the EPICS module of the array
 * controller and/or in the instrument controller module.
 *
 * Setup:
 *  The address jumpers need to be setup, as well as the
 * #define BASEADD and #define SHORTOFF lines.
 *  The address jumpers determine the base address. The board
 * resides in the VME short space address, 0xffff****.
 *
 * Initialization:
 *  The routine initTempBoard must be called once after bootup.
 *
 * Usage:
 *  A given channel (0-31) is accessed br writing to the mux address.
 *  The ADC is clocked with by writing to the strobe address, adcstr.
 *  The ADC is readout with by reading at address adcrd.
 *  The 3 previous steps are done in sequence with proper timing
 * in routine rddewint(ch), which returns an ADC count (int).
 * The routine printTempVolts displays the 32 voltages.
 *
 * Special channels:
 *   Gnd: 15 and 31
 *   1 volt reference: Ch 30
 *   100K current ref: Ch 29
 *   On board diode: Ch 28
 *
 * Thus there are 27 inputs available to the user.
 *
 *  The external bus is readout with routine extbus()
 *
 * Nick Roddier, 14-MAY-1997
 * Comments modified and code consolidated by R Wolff March 2000.
 */


#define ANLG_GAIN  6.0
#define ADC_RANGE 10.0
#define ADC_CNTS  4096
#define ADC_GAIN  (ADC_RANGE/(float)(ADC_CNTS)/ANLG_GAIN)

#define GND_A    15
#define T_DIODE  28
#define T_100K   29
#define T_1V     30
#define GND_B    31

struct tempMonReg {
    char pad1;
    unsigned char muxadd;
    char pad2;
    unsigned char adcstr;
    unsigned short adcrd;
    unsigned short extcon;
} tempMonReg;

struct tempMonReg *pTm = NULL;

int noTempMon = 0;
unsigned char tempMonDbg = 0;
SEM_ID semTemps = NULL;

int testTempBoards() {
    int i, offset;
    double val;
    int rc;
    
    if (noTempMon || gnirsG.simulation)
        return VME_OK;
    

    rc = VME_OK;
    /* NOTE  test limits are first guesses  (rjw 2/15/01) XXX */
    for (i = 0; i < NUM_T_BOARDS; i++) {
        offset = i * NUM_T_PER_BOARD;
        /* check the grounds */
        val = rddewVolt(GND_A + offset);
        if ( fabs(val) > 0.01 ) {
            gnirsLogMessage(CICS_DB_ERROR, "Ch %d err: gnd reads %6.3f",
                    GND_A + offset, val);
            rc = VME_ERROR;
        }
        val = rddewVolt(GND_B + offset);
        if ( fabs(val) > 0.01 ) {
            gnirsLogMessage(CICS_DB_ERROR, "Ch %d err: gnd reads %6.3f",
                    GND_A + offset, val);
            rc = VME_ERROR;
        }
        /* 1 Volt reference */
        val = rddewVolt(T_1V + offset);
        if ( fabs(val - 1.0) > 0.01 ) {
            gnirsLogMessage(CICS_DB_ERROR, "Ch %d err: 1V reads %6.3f",
                    T_1V + offset, val);
            rc = VME_ERROR;
        }
        /* 100 kohm */
        val = rddewVolt(T_100K + offset);
        if ( fabs(val - 1.1) > 0.2 ) {
            gnirsLogMessage(CICS_DB_ERROR, "Ch %d err: 100k reads %6.3f",
                    T_100K + offset, val);
            rc = VME_ERROR;
        }
        /* will be affected by temp in crate */
        /*
         * val = rddewVolt(T_DIODE + offset);
         * if ( fabs(val) > 0.02 ) {
         *    gnirsLogMessage(CICS_DB_ERROR, "Ch %d error, diode reads %5.2f",
         *            T_DIODE + offset, val);
         *    rc = VME_ERROR;
         * }
         */
    }
    return rc;
}

int initTempBoards() {
    int i, offset;
    tempDescriptor *td;

    
    if (noTempMon || gnirsG.simulation) {
        pTm = (struct tempMonReg *) NULL;
        return VME_OK;
    }

    if (semTemps)
        semDelete(semTemps);
    semTemps = semBCreate(SEM_Q_FIFO, SEM_FULL);
    if (semTemps == NULL) {
        gnirsLogMessage(CICS_DB_ERROR, "No semaphore for Temperatures");
        return VME_ERROR;
    }

    for (i = 0, td = &temperatures[0]; i < NUM_TEMPS; i++, td++ ) {
        td->type = T_NORMAL;
    }

    for (i = 0; i < NUM_T_BOARDS; i++) {
        offset = i * NUM_T_PER_BOARD;
        strcpy(temperatures[T_1V + offset].name, "1 Volt Reference");
        temperatures[T_1V + offset].type = T_REF;
        strcpy(temperatures[T_100K + offset].name, "100K Current Reference");
        temperatures[T_100K + offset].type = T_REF;
        strcpy(temperatures[T_DIODE + offset].name, "On board diode");
        temperatures[T_DIODE + offset].type = T_NORMAL;
        strcpy(temperatures[GND_A + offset].name, "Ground (A)");
        temperatures[GND_A + offset].type = T_REF;
        strcpy(temperatures[GND_B + offset].name, "Ground (B)");
        temperatures[GND_B + offset].type = T_REF;
    }

    pTm = (struct tempMonReg *)TEMP_P_ADDR;

    return VME_OK;
}

int extbus() {

    /* Fix this, if needed, for two board operation XXX */
    return (int)pTm->extcon;
}

int rddewint(unsigned char ch) {
    int mydat;
    int rc;

    if (ch > (NUM_TEMPS - 1)) {
      gnirsLogMessage(CICS_DB_ERROR, " rddewint address %d out of range\n", ch);
      return(-1);
    }
    if (pTm == (struct tempMonReg *) NULL)
        return 0;

    rc = semTake(semTemps, WAIT_FOREVER);
    if (rc == ERROR) {
        gnirsLogMessage(CICS_DB_ERROR, "semTake error in rddewint");
        return -1;
    }

    /* Only good for two boards! */
    if ( ch < NUM_T_PER_BOARD)
        pTm = (struct tempMonReg *)TEMP_P_ADDR;
    else
        pTm = (struct tempMonReg *)TEMP_D_ADDR;
    ch = ch % NUM_T_PER_BOARD;
    
    /* Tell card which channel to read */
    pTm->muxadd = ch;
    taskDelay(1);
    /* Strobe adc control to start conversion */
    pTm->adcstr = 0;
    taskDelay(1);
    /* Read the data */
    mydat = pTm->adcrd;
    /* Cause board to reset adc (?) */
    pTm->muxadd = GND_A;
    pTm->muxadd = GND_B;
    semGive(semTemps);
    if (tempMonDbg == 1) {
      gnirsLogMessage(CICS_DB_FULL, "wfdew ch %d = (int) %d\n", ch, mydat);
    }
    return(mydat);
}

double rddewVolt(unsigned char ch) {
    double myval;

    if (ch > (NUM_TEMPS - 1)) {
      gnirsLogMessage(CICS_DB_ERROR,"rdVolt address %d out of range\n", ch);
      return(TEMP_ERROR);
    }
    myval = (double)(rddewint(ch)) * ADC_GAIN;

    if (tempMonDbg == 1) {
      gnirsLogMessage(CICS_DB_FULL, "wfdew _ch_ %d = (volts) %f\n", ch, myval);
    }

    return(myval);
}

double rddewDegK(unsigned char ch) {
    double volts;
    tempDescriptor *td;
    double T;

    if (ch > (NUM_TEMPS - 1)) {
      gnirsLogMessage(CICS_DB_ERROR,"redDegK: address %d out of range\n", ch);
      return(TEMP_ERROR);
    }
    volts = rddewVolt(ch);
    td = &temperatures[ch];

    switch(td->type) {
        case T_NORMAL:
            T = (((((tempCoeff[5] * volts) + tempCoeff[4] ) * volts +
                tempCoeff[3] ) * volts + tempCoeff[2] ) * volts +
                tempCoeff[1] ) * volts + tempCoeff[0] + td->offset;
            break;
        case T_REF:
            T = volts;
            break;
        case T_COLD_HEAD:
            T = volts;
            break;
    }
    return T;
}

double rddewDegC(unsigned char ch) {
    double T;

    if (ch > (NUM_TEMPS - 1)) {
      gnirsLogMessage(CICS_DB_ERROR,"redDegC: address %d out of range\n", ch);
      return(TEMP_ERROR);
    }
    T = rddewDegK(ch);
    return (T - 273.);
}


/*
 *
 * PRESSURE MONITORING
 *
 * Varian SenTorr Pressure Monitor
 *
 * Code adapted from that written by Peter Ruckle in 1998, which, in turn
 * was derived from that supplied by N. Roddier.
 * Extensively modified by R. Wolff   3/2000
 */



/* defines*/
#define SEM_TIMEOUT (sysClkRateGet())
#define SENTORR_ID       0
/* Timeout is in ticks, which, at 9600 baud, is 16 chars per tick. */
#define SENTORR_TIMEOUT 10
/* Ticks to wait for emission command to be acted upon */
#define SENTORR_EMISSION_ON     50
/* Ticks to wait for IG to stabilize */
#define SENTORR_STABLE          120

#define SENTORR_BAUD        9600
#define SENTORR_EPSILON     .1
#define THRESHOLD           .0015


/* 'global' variables*/
SEM_ID semSenTorr = NULL;
int serialPort;
int noSenTorr = 0;


/* function prototypes that the world doesn't need to know */
int readPort(char *, int);

int initSenTorr() {

    if (semSenTorr)
        semDelete(semSenTorr);
    semSenTorr = semBCreate(SEM_Q_FIFO, SEM_FULL);
    if (semSenTorr == NULL) {
        gnirsLogMessage(CICS_DB_ERROR, "No semaphore for SenTorr");
        return VME_ERROR;
    }

    if (serialPort)
        close(serialPort);
    if ((serialPort = open("/tyCo/1", O_RDWR, 0)) == 0) { 
        gnirsLogMessage(CICS_DB_ERROR,  "Opening SenTorr port failed");
        return(VME_ERROR);
    }
    /*set baud rate*/
    if (ioctl(serialPort, FIOBAUDRATE, SENTORR_BAUD) == ERROR) { 
        gnirsLogMessage(CICS_DB_ERROR, "Setting baud rate for Temp PORT failed");
        return(VME_ERROR);
    }
    taskDelay(2);
    return VME_OK;
}

int testSenTorr() {
    char response[SENTORR_LEN];
    int rc;
    char old, new;

    if ( noSenTorr || gnirsG.simulation)
	return VME_OK;
    rc = sendSenTorr("13", response, 2);
    if (rc != VME_OK)
        return rc;
    old = response[1];
    rc = sendSenTorr( (old == '0' ? "11" : "10"), response,0 );
    if (rc != VME_OK)
        return rc;
    rc = sendSenTorr("13", response, 2);
    if (rc != VME_OK)
        return rc;
    new = (old == '0' ? '1' : '0');
    if (new != response[1])
        return VME_ERROR;
    rc = sendSenTorr( (old == '0' ? "10" : "11"), response,0 );
    if (rc != VME_OK)
        return rc;
    return VME_OK;
}

int sendSenTorr(char *msg, char *response, int len) {
    char buffer[SENTORR_LEN];
    int n, m;
    int rc;

    n = strlen(msg);
    if (n > (SENTORR_LEN - 4)) {
        gnirsLogMessage(CICS_DB_ERROR, "SenTorr msg too long <%s>", msg);
        return VME_ERROR;
    }

    sprintf(buffer, "#%02d%s\r", SENTORR_ID, msg);
    n += 4;
    rc = semTake(semSenTorr, WAIT_FOREVER);
    if ( rc != OK) {
        gnirsLogMessage(CICS_DB_ERROR, "SenTorr Semaphore failed");
        return VME_ERROR;
    }
    if (noSenTorr || gnirsG.simulation)
	m = n;
    else
	m = write(serialPort, buffer, strlen(buffer));
    if (m != n) {
        gnirsLogMessage(CICS_DB_ERROR, "SenTorr write failure");
	semGive(semSenTorr);
        return VME_ERROR;
    }
    if (noSenTorr || gnirsG.simulation) {
	rc = VME_OK;
	*response = '\0';
    } else
	rc = readPort(response, len);
    semGive(semSenTorr);
    return rc;
}



int readPort(char *msg, int wanted) {
    int n, ticks, len;
    char buffer[2 * SENTORR_LEN];
    int rc;
 
    len = wanted + 2;
    n = ticks = 0;
    *msg = '\0';
    rc = VME_OK;
    if (noSenTorr || gnirsG.simulation)
        return rc;
    /* check number of bytes to be read until there are len or a timeout occurs */
    while ((n < len) && (ticks++ < SENTORR_TIMEOUT)) {
        taskDelay(1);
        ioctl(serialPort, FIONREAD, (int)&n);
    }
    if (n > 0) {                        /* something was read*/
        read(serialPort, buffer, n);
        if (buffer[0] == '?') {         /* error*/
            gnirsLogMessage(CICS_DB_ERROR, "SenTorr read gave error");
            rc = VME_ERROR;
        } else if (buffer[0] != '>') {    /* Synchronization error */
            gnirsLogMessage(CICS_DB_ERROR, "Missing '>' from SenTorr");
            rc = VME_ERROR;
        }
    } else {                            /*no characters read time out*/ 
        gnirsLogMessage(CICS_DB_ERROR, "no response from SenTorr");
        rc = VME_ERROR;
    }
    if (rc == VME_ERROR) {
        ioctl(serialPort, FIOFLUSH, (int)NULL);
        return VME_ERROR;
    }
    if (buffer[n-1] == '\r') {
        buffer[n-1] = '\0';
    } else {
        gnirsLogMessage(CICS_DB_ERROR, "SenTorr terminator not found");
        return VME_ERROR;
    }
    strcpy(msg, &buffer[1]);    /* Don't return the '>' at buffer[0] */
  return VME_OK;
}

int readSenTorr() {
    int rc;
    char buffer[2 * SENTORR_LEN];
    char *p, *t1, *t2;
    BOOL tc1Error, tc2Error;
    int commas;

    rc = sendSenTorr("0F", buffer, 29);
    if (rc != VME_OK)
        return rc;
    tc1Error = tc2Error = FALSE;
    commas = 0;
    /* Look for 'E' as leading char for a reading ... flags an error */
    if (buffer[0] == 'E') {
        gnirsLogMessage(CICS_DB_FULL, "SenTorr IG error %s", buffer);
        return VME_SENTORR;
    }
    for (p = buffer; *p; p++) {
        if (*p == ',') {
            commas++;
            if (*(p+1) == 'E') {
                if (commas == 1)
                    tc1Error = TRUE;
                else if (commas == 2)
                    tc2Error = TRUE;
            }
            if (commas == 1)
                t1 = p+1;
            else if (commas == 2)
                t2 = p+1;
            *p = '\0';
        }
    }
    if (noSenTorr || gnirsG.simulation) {
	senTorrIg = 0.0;
	senTorrTc1 = 0.0;
	senTorrTc2 = 0.0;
    } else {
	senTorrIg = atof(buffer);
	/* Errors are negative small numbers */
	senTorrTc1 = (tc1Error ? -1. * atof(t1+1) : atof(t1));
	senTorrTc2 = (tc2Error ? -1. * atof(t2+1) : atof(t2));
    }
    return VME_OK;
}

/* XXX why?? */
int initCCG() {
    char response[SENTORR_LEN];
    int rc;

    /* lock keypad! */
    rc = sendSenTorr("21", response, 2);
    if (rc != VME_OK) {
        gnirsLogMessage(CICS_DB_ERROR, "Couldn't lock keypad");
        return rc;
    }
    return VME_OK;
}



int readCCG() {
    int emission;
    double tc1, tc2, value;
    char response[SENTORR_LEN];
    int rc;

    /* read thermocouple sensors*/
    rc = readSenTorr();
    if (rc != VME_OK)
        return rc;

    /* If both are in error, can't continue */
    if ((senTorrTc1 < 0.) && (senTorrTc2 < 0.)) {
        /* XXX HEALTH is bad */
        return VME_ERROR;
    }
    tc1 = senTorrTc1;
    tc2 = senTorrTc2;

    /* If one is in error, set it equal the other, so average below isn't
     * changed.
     * */
    if (tc1 < 0.)
        tc1 = tc2;
    else if (tc2 < 0.)
        tc2 = tc1;

    rc = VME_OK;
    if (fabs(tc1 - tc2) > SENTORR_EPSILON) {
	      gnirsLogMessage(CICS_DB_ERROR, "pressure values are different");
	      rc =  VME_ERROR;
    }
	  
    /* if noSenTorr or simulation, tc1 == tc2 == 0.0 so the following
     * conditional holds.
     */
    if((tc1 > THRESHOLD) || (tc2 > THRESHOLD) ||
            (rc != VME_OK) || (tc1 == 0.0)||(tc2 == 0.0)) {
	      value = (tc1 + tc2)/2.;
    } else {
        /* check emission status*/
        rc = sendSenTorr("32I1", response, 2);
        if (rc != VME_OK)
            return rc;

        gnirsLogMessage(CICS_DB_FULL, " emission status = %s", response);
        emission = atoi(response);


        /* if emission is off,  turn it on and wait for it to come on*/
        if(!emission) {
            /* turn on emission*/  
            rc = sendSenTorr("31I1", response, 0);
            if (rc != VME_OK)
                return rc;
              
            /* loop until emission status is on */
            while (!emission ) {
                taskDelay(SENTORR_EMISSION_ON);
                rc = sendSenTorr("32I1", response, 2);
                if (rc != VME_OK)
                    return rc;
                gnirsLogMessage(CICS_DB_FULL,
                        " emission result = %s\n", response);
                emission = atoi(response);
            }
        }

        taskDelay(SENTORR_STABLE);
        /* read ccg sensor now that it's on*/
        /* XXX readSenTorr(); ??? */
        /*
        rc = sendSenTorr(XXX, response, XXX);
        */
        if (rc != VME_OK)
            return rc;
        value = (double)atof(response);

        /* Turn emission off*/
        rc = sendSenTorr("30I1", response, 0);
        if (rc != VME_OK)
            return rc;
    }
    senTorrCcg = value;
    return VME_OK;
}


/*******************
 * Cryohead Control
 *******************/

void cryoHead(int state) {
    /* Set, or 'on', is * +5V; off is 0V; function is suitable for
     * "manual" control  as well.
     */
    if (state)
        setBit(cryoSwitches[COMPUTER_CONTROL]);
    else
        clearBit(cryoSwitches[COMPUTER_CONTROL]);
    gnirsG.cryoCpuState = state;
}

void computerMode() {
    int state;

    gnirsG.cryoSelect = COMPUTER;
    state = isSet(cryoSwitches[ON_OFF_SWITCH]);
    cryoHead(state);
}

void manualMode() {
    gnirsG.cryoSelect = MANUAL;
    cryoHead(CRYO_OFF);  /* might as well though electronics disables this */
}

void finishCryoConfig() {
    /* Read the switches, set gnirsG items */
    if (isSet(cryoSwitches[COMPUTER_MANUAL]))
        manualMode();
    else
        computerMode();

    /* Set the interrupt handler and unmask the interrupt for each bit  */
    xycomSetInterrupt(0, computerMode);
    xycomSetInterrupt(1, manualMode);
}
