static struct
  {
      void *v;
      char *c;
  }
rcsid =
{
    &rcsid,
        "$Id: gnirsTandP.c,v 1.2 2009/05/27 19:32:08 fkraemer Exp $"
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
#include "gnirs.h"
#include "timeLib.h"
#include "epicsTypes.h"

/*
 *
 * TEMPERATURE MONITORING
 *
 */


/*             WFDEW module software support
 *      WildFire Dewar Temperature readback board
 *
 *  This file contains the source for accessing
 * the WFDEW board located in the EPICS module of the array
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

struct wfdewReg {
    char pad1;
    unsigned char muxadd;
    char pad2;
    unsigned char adcstr;
    unsigned short adcrd;
    unsigned short extcon;
} wfdewReg;

struct wfdewReg *pWf = NULL;

int noWFDEW = 0;
unsigned char wfdewdbg = 0;
SEM_ID semTemps = NULL;

/* Perhaps this is best done by checking for rational values of
 * the reference channels.
 */
int testTempBoard() {
    
    if (noWFDEW)
        return VME_OK;
    return VME_OK;
}

/* NOT TESTED for more than one board XXX */

int initTempBoard() {
    int i, offset;
    tempDescriptor *td;

    
    if (noWFDEW) {
        pWf = (struct wfdewReg *) NULL;
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
        temperatures[T_DIODE + offset].type = T_REF;
        strcpy(temperatures[GND_A + offset].name, "Ground (A)");
        temperatures[GND_A + offset].type = T_REF;
        strcpy(temperatures[GND_B + offset].name, "Ground (B)");
        temperatures[GND_B + offset].type = T_REF;
    }

    pWf = (struct wfdewReg *)TEMP_P_ADDR;

    return VME_OK;
}

int extbus() {

    /* Fix this, if needed, for two board operation XXX */
    return (int)pWf->extcon;
}

int rddewint(unsigned char ch) {
    int mydat;
    int rc;

    if (ch > (NUM_TEMPS - 1)) {
      gnirsLogMessage(CICS_DB_ERROR, " rddewint address %d out of range\n", ch);
      return(-1);
    }
    if (pWf == (struct wfdewReg *) NULL)
        return 0;

    /* Only good for two boards! */
    if ( ch < NUM_T_PER_BOARD)
        pWf = (struct wfdewReg *)TEMP_P_ADDR;
    else
        pWf = (struct wfdewReg *)TEMP_D_ADDR;
    ch = ch % NUM_T_PER_BOARD;

    rc = semTake(semTemps, WAIT_FOREVER);
    if (rc == ERROR) {
        gnirsLogMessage(CICS_DB_ERROR, "semTake error in rddewint");
        return -1;
    }
    /* Tell card which channel to read */
    pWf->muxadd = ch;
    taskDelay(1);
    /* Strobe adc control to start conversion */
    pWf->adcstr = 0;
    taskDelay(1);
    /* Read the data */
    mydat = pWf->adcrd;
    /* Cause board to reset adc (?) */
    pWf->muxadd = GND_A;
    pWf->muxadd = GND_B;
    semGive(semTemps);
    if (wfdewdbg == 1) {
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

    if (wfdewdbg == 1) {
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
            break;
        case T_COLD_HEAD:
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

/* Test that the unit will respond:
 *   Get the current units
 *   Set to the "other" units and verify
 *   Restore original
 */ 
int testSenTorr() {
    char response[SENTORR_LEN];
    int rc;
    char old, new;

    if ( noSenTorr)
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

/* Send a message to the SenTorr box and wait for a response of length 'len'.
 * Len only includes the wanted characters, not the response delimiters.
 * A semaphore is used to insure that the write/read is atomic.
 */
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
    m = write(serialPort, buffer, strlen(buffer));
    if (m != n) {
        gnirsLogMessage(CICS_DB_ERROR, "SenTorr write failure");
        return VME_ERROR;
    }
    rc = readPort(response, len);
    semGive(semSenTorr);
    return rc;
}


/*
 *+
 * FUNCTION NAME:
 * readPort
 *
 * INVOCATION:
 * int readPort(char *msg, int len)
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  < char *msg  - string that was read
 *  > int wanted - desired number of characters (not including delimiters)
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *   Read the return message from the serial port
 *
 * DESCRIPTION:
 *   Read the serial port.  If fewer than len characters are read before
 *   a time out is reached, an error is returned.  Similarly, if a ? is read
 *   as the first character, an error is returned.  
 *    
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * October 1998  Original version adapted from CICS alpha 1.0  
 *
 * Author P. Ruckle (serial port routines from N. Roddier)
 * Extensively modified, R. Wolff   3/2000
 *-
 */

int readPort(char *msg, int wanted) {
    int n, ticks, len;
    char buffer[2 * SENTORR_LEN];
    int rc;
 
    len = wanted + 2;
    n = ticks = 0;
    *msg = '\0';
    rc = VME_OK;
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

/* Read all three sensors */
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
    SenTorrIg = atof(buffer);
    /* Errors are negative small numbers */
    SenTorrTc1 = (tc1Error ? -1. * atof(t1+1) : atof(t1));
    SenTorrTc2 = (tc2Error ? -1. * atof(t2+1) : atof(t2));
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


/*
 *+
 * FUNCTION NAME:
 *  readCCG
 *
 * INVOCATION:
 * int readCCG()
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *   Read the ccg sensor
 *
 *
 * DESCRIPTION:
 *  Check the pressures on the convectTorr or tc sensors.  If the pressure
 *  is low enough, check the pressure on the ccg.
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * October 1998  Original version adapted from CICS alpha 1.0  
 *
 * Author P. Ruckle (serial port routines from N. Roddier)
 * Extensively modified by R. Wolff  3/2000
 *-
 */

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
    if ((SenTorrTc1 < 0.) && (SenTorrTc2 < 0.)) {
        /* XXX HEALTH is bad */
        return VME_ERROR;
    }
    tc1 = SenTorrTc1;
    tc2 = SenTorrTc2;

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
	  
    if((tc1 > THRESHOLD) || (tc2 > THRESHOLD) ||
            (rc != OK) || (tc1 == 0.0)||(tc2 == 0.0)) {
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
                gnirsLogMessage(CICS_DB_FULL, " emission result = %s\n", response);
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
    return VME_OK;
}
