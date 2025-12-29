
/* -------------------------------------------------------------- */
/*                                                                */
/*             WFDEW module software support                      */
/*                                                                */
/*  These wfdew.[ch] files contain the source for accessing       */
/* the WFDEW board located in the EPICS module of the array       */
/* controller and/or in the instrument controller module.         */
/*                                                                */
/* Setup:                                                         */
/*  The address jumpers need to be setup, as well as the          */
/* #define BASEADD and #define SHORTOFF lines.                    */
/*  The address jumpers determine the base address. The board     */
/* resides in the VME short space address, 0xffff**** on the 162  */
/*                                                                */
/* Initialization:                                                */
/*  The routine _dewinit()_ must be called once after bootup.     */
/*  The routines _wfdewdbgon()_ and _wfdewdbgoff()_ can be called */
/* to turn on and off the real time debugging information.        */ 
/*                                                                */
/* Usage:                                                         */
/*  A given channel (0-31) is accessed with routine _wradd(ch)_   */
/*  The ADC is clocked with routine _adcctc()_                    */
/*  The ADC is readout with routine _rdadc()_                     */
/*  The 3 previous steps are done in sequence with proper timing  */
/* in routines _rddewint(ch)_ and _rddewvolt()_ which return      */
/* either an ADC count (int) or voltage (float).                  */
/*  The routine _dspdew()_ displays the 32 voltages.              */
/*                                                                */
/* Special channels:                                              */
/*   Gnd: 15 and 31                                               */
/*   1 volt reference: Ch 30                                      */
/*   100K current ref: Ch 29                                      */
/*   On board diode: Ch 28                                        */
/*                                                                */
/*  The external bus is readout with routine _extbus()_           */
/*                                                                */
/* Nick Roddier, 14-MAY-1997                                      */
/* -------------------------------------------------------------- */

/* wfdew control routines */
int wfdewdbgon();     /* Turn on debugging information */       
int wfdewdbgoff();    /* Turn off debugging information */
int dewinit();        /* Module initialization */

/* dewar and ADC routines */
int wradd(unsigned char myadd);        /* Select a given channel */
int adcctc();                          /* Clock the ADC */
int rdadc();                       /* Read the ADC */
int rddewint(unsigned char ch);    /* Sequence of previous 3, return an int */
float rddewvolt(unsigned char ch); /* Previous routine converted to volts */
int dspdew();                          /* Display the 32 channels in volts */

/* External bus */
int extbus();          /* Read the external 16 bit bus */

