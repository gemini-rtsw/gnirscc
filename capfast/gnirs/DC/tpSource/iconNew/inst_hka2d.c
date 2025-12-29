static char rcid[]="$Id: inst_hka2d.c,v 1.2 2009/05/27 19:33:26 fkraemer Exp $";
#define INST_HKCAPT 1
#define INST_CTRL 1
#define DEBUG_HK 1
/* Housekeeping variables a/d process for INST CONTROL. */
/******************************************************************************
 * Program:	INST CONTROL software
 * File:        inst_hka2d.c     
 * Purpose:     Periodically read all a/d channels into memory and transfer
 *              (with scaling) to a floating point array.
 * Author:      Dick H. Fredericksen
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	24-Apr-1991 - created file - dhf
 *	08-Aug-1992 - Modified for Wildfire - ncb
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conc.h>

#include "irstd.h"
#include <protdefs.h>
#include <protocol.h>
#include <common.h>

#include "config_st.h"
#include <instdefs.h>          /* definitions specific to B011 */
#define NOINSTVARS
#include <instvars.h>          /* global variables specific to B011 */ 
#define NOINSTPROCS            /* already defined in inst_main */
#include <instprocs.h>	       /* process descriptors & channel assignments*/
#include "prototypes.h"

static int ME = INST_HKCAPT;      /* self identifier for debugging messages */

/******************************************************************************
 * Program: inst_ctrl
 * Routine: hka2d()
 * Purpose: to read a/d converters for housekeeping variables.
 *          
 * Inputs:  none (but action doesn't start until a go-ahead signal
 *          is received from controller).
 * 
 *****************************************************************************/
void Hka2d()
{
    int ii;				/* all-purpose counter */
    int a2dval;				/* temp for value read */
    int bufix = 0;			/* index of cur float buffer */
    float *fptr;			/* pointer to float buffer */
    float slope, interc;		/* slope & intercept for conversions*/
    int request;			/* request code from controller */
    int badreq;				/* place to stash trash */
    char pbuf[81];			/* if needed for debugging */

    /* Await go-ahead signal from control process: */
    request = 0;
    forever                  /* until we get a valid go-ahead signal */
    {
	request = ChanInInt(Cntrl_to_HK_A2D);
	if (request != 1)		/* bad request */
	    continue;			/* ignore it until we get a good one */
	else
	    break;			/* it was a good request */
    }

    forever
    {
	
	/* Read the a/d converters:  */
  	for (ii = 0; ii < HK_CHANS; ii++) 
        {
	    if (a2d_freeze) break;
	    ReadOneHK(ii, &(hk_buf[bufix][ii]));
	}
	    
	/* now turn off channel number selector and hk data selector */
	mod_scb_reg(NONE, SEL_HK_ADDR | SEL_HK_DATA );

	SemP(idxlock);
	VAR_MOD(HKBUF, (int *)hk_buf[bufix]);
	bufix = (bufix+1) % HK_BUFS;    /* cycle through buffers */
	hkbufix = bufix;
	SemV(idxlock);

	ProcWait(31250);                  /* don't tie up the transputer all
					   the time */
	while (a2d_freeze)
	    ProcWait(1000);              /* moreover, freeze upon demand */
    }
}

/*****************************************************************************
 * Routine: ReadOneHK
 * Purpose: read one housekeeping channel and return the floating point
 *          value of it
 * Parameters: chan - int - channel number
 *             fptr - float
 * Returns:    float - value read from channel.
 *****************************************************************************/
float ReadOneHK(chan, fptr )
int chan;
float *fptr;
{
    int a2dval;				/* temp for value read */
    float slope, interc;		/* slope & intercept for conversions*/
    float hkoffset;                     /* ground offset for HK */
    static float hkoffsetpa, hkoffsetadc;
    int *cen_hk;

    if (chdesc[chan] == 0)      /* Make sure the channel is valid */
    {
	*fptr = 0.0;
	return (*fptr);
    }

    SemP(dbus_lock);		/* make sure noone else uses the bus */

    if (trace_flag>50 && a2d_freeze)
	send_debug("**ReadOneHK: chan %d", chan);

/* Write the ground channel address to the muxes */
#if 0
    if (chan < 128)    /* PreAmp HK */
    {
	mod_scb_reg(HK_PA_C1, HK_PA_C0);
	write_dbus(HK_PA_BEN, 1);
    }
    else              /* ADC HK */
    {
	mod_scb_reg(HK_ADC_C1, HK_ADC_C0);
	write_dbus(HK_ADC_BEN, 83);
    }
    mod_scb_reg(NONE, DATA_BUS);        /* Turn off the bus for noise */
    ProcWait(400);
#endif

/* Write the channel address to the muxes */
    if (chan < 128)    /* PreAmp HK */
    {
	mod_scb_reg(HK_PA_C1, HK_PA_C0);
	write_dbus(HK_PA_BEN, chan & 0x7F);
    }
    else              /* ADC HK */
    {
	mod_scb_reg(HK_ADC_C1, HK_ADC_C0);
	write_dbus(HK_ADC_BEN, chan & 0x7F);
    }
    mod_scb_reg(NONE, DATA_BUS);        /* Turn off the bus for noise */
    ProcWait(61);

/* Strobe the ADCs */
    if (chan < 128)    /* PreAmp HK */
    {
	mod_scb_reg(HK_PA_C0, HK_PA_C1);
	cen_hk = (int *)(CEN_BASE + HK_PA_BEN);
	*cen_hk = 0;
    }
    else              /* ADC HK */
    {
	mod_scb_reg(HK_ADC_C0, HK_ADC_C1);
	cen_hk = (int *)(CEN_BASE + HK_ADC_BEN);
	*cen_hk = 0;
    }
    ProcWait(2);

/* Read the ADCs */

    if (chan < 128)    /* PreAmp HK */
	a2dval = 0xFFFF & read_dbus(RX_PA | HK_PA_C0 | HK_PA_C1);
    else              /* ADC HK */
	a2dval = 0xFFFF & read_dbus(RX_ADC | HK_ADC_C0 | HK_ADC_C1);

/* Reset all the HK SCBs and release the bus */
    mod_scb_reg(NONE, DATA_BUS);   
    mod_scb_reg(NONE, RX_PA | HK_PA_C0 | HK_PA_C1);
    mod_scb_reg(NONE, RX_ADC | HK_ADC_C0 | HK_ADC_C1);
    SemV(dbus_lock);

/* Now convert the observed values to floating point and scale them: */
    if (a2dval & 0x00008000) a2dval |= 0xffff0000;
    slope = hkcdesc[chdesc[chan]].slope;
    interc = hkcdesc[chdesc[chan]].intercept;
    if (chan == 1) hkoffsetpa = ((float)a2dval * slope) + interc;
    if (chan == 211) hkoffsetadc = ((float)a2dval * slope) + interc;
    if (chan < 128) 
	hkoffset = hkoffsetpa;
    else
	hkoffset = hkoffsetadc;
    if (chan == 1)
	*fptr = hkoffsetpa;
    else if (chan == 211)
	*fptr = hkoffsetadc;
    else
    *fptr = ((float)a2dval * slope) + interc - hkoffset;

    if (trace_flag >50 && a2d_freeze)
	send_debug("**a2dval %8.8x, slope %f, interc %f, fptr %f",
		   a2dval, (double)slope, (double)interc, (double)*fptr);

    return (*fptr);
}

int read_onehk(header, buf, var_ptr)
    int header;
    int *buf;
    int *var_ptr[];
{
    float fval;
    int anum = 0;
    int hkChan, i, err = 0;
    int *pbuf;

   /*  anum = ARR_INDEX(buf); */
   /*  hkChan = D2A_GROUP(buf); */
    hkChan = ARR_INDEX(buf);
    ReadOneHK(hkChan, &fval);

    if (trace_flag>20)
	send_debug("**read_onehk: chan %d, fval %f", hkChan, (double)fval);

    header=TO_NODE(0) | MESSAGE(VAR_READ) | OF_LENGTH(3);

    /* always a single element	 */
    pbuf = mem_alloc(sizeof(int) * 3);
    *((float *) &pbuf[RETURN_VAL]) = fval;
    pbuf[VAR_NUM] = buf[VAR_NUM];
    pbuf[FROM] = _node_number;

    SEND(header, (char *)pbuf, Control_to_Reader);

}
