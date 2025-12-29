#define ARR_CTRLR 1
/* Command and control process for Array CONTROL. */
/******************************************************************************
 * Program:	INSTRUMENT CONTROL software
 * File:        arr_crtrl.c     
 * Purpose:     Control array related stuff this is the part of the code which
 * 		knows about the array and is recoded for new arrays.  It uses 
 *		The routines in inst_hdw.c to control and set up the array(s).
 *
 * Author:      Nick C Buchholz
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	12-Aug-1992 - created file - ncb
 *	06-Mar-1996 - Modified file to support single instrument/array - ncb
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conc.h>
#include <math.h>

#include <irstd.h>
#include <protdefs.h>
#include <protocol.h>
#include <common.h>

#include "config_st.h"			/* description of config structs */
#include "instdefs.h"			/* definitions specific to INST */

#define NOINSTVARS
#include "instvars.h"			/* global variables specific to INST */ 
#define NOINSTPROCS			/* already defined in instmain */
#include "instprocs.h"			/* process descriptors & channels */
#include "devdefs.h"
#include "prototypes.h"
#include "cnfg_inst.c"		/* code to describe a particular array */

#define ON 1
#define OFF 0

/******************************************************************************
 * Routine: arr_power()
 * Purpose: turn on and off the dbias enable
 * Parameters: header - int - header of message which started this off
 *	       buf - int * - rest of message
 *	       state - int - turn on or off based on this value
 * Returns:       int - error status
 *	    stdbuf - char * - filled with error message
 * Comments: this is included here by tradition It should be the same for all 
 *          arrays/instruments
 *****************************************************************************/
int arr_power(anum, state, stdbuf)
int anum;
int state;
char stdbuf[];
{
    float act_valid;
    int err = OK;
    float junk;
    int actchan =  inst.array[anum].activ_tch;
    float activ_lvl = inst.array[anum].activ_lvl;

    if (trace_flag>10)
	send_debug("**arr_power:ENTER: array=%d, state=%d, actchan=%d, activ_lvl=%f",
		   	anum, state, actchan, (double)activ_lvl);

    if (protection && state == ON)	/* are we being safe? */
    {
	act_valid = ReadOneHK(actchan, &junk );
	if (trace_flag>30)
	    send_debug("**arr_power: protection=%d, act_valid=%f",
		       protection, (double)act_valid);
	if (act_valid < activ_lvl)
	{
	    sprintf(stdbuf, "Array %d is too warm to activate, ", anum + 1);
	    err = ERROR;
	}
    }

    if (err == OK)
	switch (state)
	{
	  case ON:
	    err = mod_scb_reg(inst.array[anum].activ_bit, NONE);
	    sprintf(stdbuf, "Array %d activated, ", anum + 1);
	    send_debug("%s",stdbuf);
	    break;
	    
	  case OFF:
	    err = mod_scb_reg(NONE, inst.array[anum].activ_bit);
	    sprintf(stdbuf, "Array %d deactivated, ", anum + 1);
	    send_debug("%s",stdbuf);
	    break;
	}

    if (trace_flag>10)
	send_debug("**arr_power:LEAVE: ");

    if (err)
	send_debug(stdbuf);

    return (err);
}


/******************************************************************************
 * Routine: arr_d2a_set
 * Purpose: set the varius D2a converters in the instrument
 * Parameters: header - int - header of message which started this off
 *	       buf - int * - rest of message
 * Returns:       int - error status
 *	    stdbuf - char * - filled with error message
 * Comments: This is the main routine which changes from instrument to 
 *	     instrument each new method for accessing the DAC's in the 
 *	     analog electronics will create a new set method here and in
 *	     DacMethods.h
 *****************************************************************************/
int arr_d2a_set(int anum, int d2agrp, int d2aidx, float value, char stdbuf[])
{
    float hkval1, hkval2, rvalue, junk;
    float slope, interc;
    int d2achan;
    struct d2a_cfg *d2adesc = &(inst.array[anum].voltage[d2agrp]);
    int hkrdv, bval, chkchan;
    int spval, d2anum;

    if (trace_flag>10)
	send_debug("**arr_d2a_set:ENTER  anum = %d, grp = %d, idx = %d, val = %f",
		   anum, d2agrp, d2aidx, (double)value);

    if (anum < 0 || anum >= NUM_ARRAYS ||
	d2agrp < 0 || d2agrp >= NUM_D2A ||
	d2aidx < 0 || d2aidx >= d2adesc->num_in_grp)  /* invalid d2a to set */
    { 
	send_debug("ERROR: Invalid array or D/A, ");
	return (ERROR);
    }
    
    d2achan = d2adesc->d2a_chan + d2aidx;
    
    if (trace_flag>10)
	send_debug("**arr_d2a_set:  mthd=%d", d2adesc->set_mthd);
    /* Determine the correct voltage to set */
    /* remove the switch statement once this is working */
    switch (d2adesc->set_mthd)
    {
      case SET11: case SET12:
	rvalue = value;
	if (trace_flag>30)
	    send_debug("**arr_d2a_set: val = %f, rval = %f",
		       (double)value,(double) rvalue );
	
	break;

      case SET0: 
	  /* null method does nothing, used for filler Dac descriptors */
      default: break;
    }


    /* check whether the desired voltage is valid */
    switch (d2adesc->set_mthd)
    {
      case SET11: case SET12:
	if (trace_flag>30)
	    send_debug("**arr_d2a_set:  method=%d, min=%f, max=%f",
		       		d2adesc->set_mthd, (double)d2adesc->min,
		       		    (double)d2adesc->max);
	if (rvalue < d2adesc->min || rvalue > d2adesc->max)
	{
	    send_debug("ERROR: Value %6.3f outside range for %s %d"
		       " (%6.3f to %6.3f allowed)",
		       (double) rvalue, 
		       inst.array[anum].D2A_names[d2agrp], d2aidx,
		       (double)d2adesc->min, (double)d2adesc->max);
	    return ERROR;
	}
	break;
      case SET0:
	  /* null method does nothing, used for filler Dac descriptors */
      default: break;
    }
	
    /* calculate the bit pattern to set the desired voltage */
    switch (d2adesc->set_mthd)
    {
      case SET11: case SET12:	/* used by new aladdin pr card in goldfish */
	bval = ((int)(value * d2adesc->bits_per_v)) & 0xfff;
	if (trace_flag>30)
	    send_debug("**arr_d2a_set:  method=%d, bval=0x%8.8x, hkchannel=%d",
		       		d2adesc->set_mthd, bval, d2achan);
	break;

      case SET0: 
	  /* null method does nothing, used for filler Dac descriptors */
      default: break;
    }

    /*set the value into the DAC */
    switch (d2adesc->set_mthd)
    {
      case SET11: case SET12:  /* setup Dac Addr reg to load the right DAC */
	SemP(dbus_lock);
	write_dbus(C_Brd_Addr_Ld, d2adesc->mjr_num); /* select board */
 	write_dbus( C_PRDAC_Sel, d2agrp - 1);/* select DAC */ 
	SemV(dbus_lock);
	if (trace_flag>30)
	    send_debug("**arr_d2a_set:  mthd=%d, spval=0x%8.8x",
		       d2adesc->set_mthd, spval);
	setd2a_12b(d2adesc->mjr_num,
			       d2adesc->cen_num, bval,stdbuf);
	SemP(dbus_lock);
	write_dbus(C_Brd_Addr_Ld, 0); /* deselect the select board */
	if (trace_flag>30)
	    send_debug("**arr_d2a_set:  mthd=%d, deselect", d2adesc->set_mthd);
	SemV(dbus_lock);

	break;      
      case SET0:
	  /* null method does nothing, used for filler Dac descriptors */
      default: break;
    }
    
    ProcWait(16000);
    chkchan = d2adesc->d2a_chan + d2aidx;
	
    /* check to see if the correct voltage got set */
    switch (d2adesc->set_mthd)
    {
      case SET11:
	hkval1 = ReadOneHK(chkchan, &junk);
	if (trace_flag>30)
	    send_debug("**arr_d2a_set:  mthd=%d, hkChan=%f",
		       d2adesc->set_mthd, (double)hkval1);
	d2anum = (d2adesc->set_mthd == SET10) ? 
	    			       ((d2agrp == 0) ? d2aidx : d2aidx + 4) :
					   d2aidx ;
	if (fabs((double)(fabs((double)hkval1)-fabs((double)value)))>(double)tolerance)
	{
	    send_debug("ERROR: Array %d, %s %d FAILED, requested %6.3f got %6.3f",
		       anum, inst.array[anum].D2A_names[d2agrp],
		         d2anum, (double)value, (double)hkval1);
	    return ERROR;
	}
	else
	    send_debug(" Array %d, %s %d set to %6.3f", 
			   anum, inst.array[anum].D2A_names[d2agrp], d2anum,
			   (double)hkval1);
        break;

      case SET12:   /* used in ALADDIN and BARACUDA for setbias */
	hkval1 = ReadOneHK(d2adesc->d2a_chan + d2aidx, &junk );	
	hkval2 = ReadOneHK((SUBVAL1 + anum), &junk); /* get VDDUC for array */
	if (trace_flag>30)
	{
	    send_debug("**arr_d2a_set:  mthd=%d, hkChan1=%f, hkChan2=%f",
		       d2adesc->set_mthd, (double)hkval1, (double)hkval1);
	}

	if ((fabs(fabs((double)hkval2 + rvalue)
		       		- fabs((double)hkval1))) > (double)tolerance)
	{
	    send_debug("ERROR: Array %d, %s %d did not set. Requested %6.3f got %6.3f", 
		       	anum, inst.array[anum].D2A_names[d2agrp], d2aidx, (double)value,
		       	(double)fabs((double)hkval2 - hkval1));
	    return ERROR;
	}
        else
	    send_debug("Array %d, %s %d set to %6.3f", 
		       anum, inst.array[anum].D2A_names[d2agrp], d2aidx,
		       (double)fabs((double)hkval2 - hkval1));
	break;
      case SET0:
            /* null method does nothing, used for filler Dac descriptors */
      default: 
            send_debug("No Array set");
            break;
    }

    if (trace_flag>10)
	send_debug("**arr_d2a_set:LEAVE:");
    return OK;

}

/******************************************************************************
 * Routine: srr_d2a_read
 * Purpose: set the varius D2a converters in the instrument
 * Parameters: header - int - header of message which started this off
 *	       buf - int * - rest of message
 * Returns:       int - error status
 *	    stdbuf - char * - filled with error message
 * Comments: This is the main routine which changes from instrument to 
 *	     instrument each new method for accessing the DAC's in the 
 *	     analog electronics will create a new set method here and in
 *	     DacMethods.h
 *****************************************************************************/
int arr_d2a_read(int anum, int d2agrp, int d2aidx, float *value, char stdbuf[])
{
    float hkval1, hkval2, rvalue, junk;
    float slope, interc;
    int d2achan;
    struct d2a_cfg *d2adesc = &(inst.array[anum].voltage[d2agrp]);
    int hkrdv, bval, chkchan;
    int spval, d2anum;

    if (trace_flag>10)
	send_debug("**arr_d2a_read:ENTER  anum = %d, grp = %d, idx = %d, val = %f",
		   anum, d2agrp, d2aidx, (double)value);

    if (anum < 0 || anum >= NUM_ARRAYS ||
	d2agrp < 0 || d2agrp >= NUM_D2A ||
	d2aidx < 0 || d2aidx >= d2adesc->num_in_grp)  /* invalid d2a to set */
    { 
	send_debug("ERROR: Invalid array or D/A, ");
	return (ERROR);
    }
    
    d2achan = d2adesc->d2a_chan + d2aidx;
    
    if (trace_flag>10)
	send_debug("**arr_d2a_read:  mthd=%d",
		       d2adesc->set_mthd);
  
    chkchan = d2adesc->d2a_chan + d2aidx;
	
    /* check to see if the correct voltage got set */
    switch (d2adesc->set_mthd)
    {
      case SET11:
      {
	  hkval1 = ReadOneHK(chkchan, &junk);
	  value[d2agrp] = hkval1;
	  if (trace_flag>30)
	      send_debug("**arr_d2a_read:  mthd=%d, hkChan=%f, val = %f" ,
			 d2adesc->set_mthd, (double)hkval1, (double) value[d2agrp]);
	  d2anum = (d2adesc->set_mthd == SET10) ? 
	      ((d2agrp == 0) ? d2aidx : d2aidx + 4) :
	      d2aidx ;

	/*   send_debug(" **Array %d, %s %d set to %6.3f, val = %f",  */
/* 			   anum, inst.array[anum].D2A_names[d2agrp], d2anum, */
/* 			   (double)hkval1, (double )value[d2agrp]); */
       break;
    }
      case SET12:   /* used in ALADDIN and BARACUDA for setbias */
	hkval1 = ReadOneHK(d2adesc->d2a_chan + d2aidx, &junk );	
	hkval2 = ReadOneHK((SUBVAL1 + anum), &junk); /* get VDDUC for array */

	value[d2agrp] = fabs(hkval2 - hkval1); 
	if (trace_flag>30)
	    send_debug("**arr_d2a_read:  mthd=%d, hkChan1=%f, hkChan2=%f",
		       d2adesc->set_mthd, (double)hkval1, (double)hkval2);
/* 	send_debug("**Array %d, %s %d set to %6.3f",  */
/* 		   anum, inst.array[anum].D2A_names[d2agrp], d2aidx, */
/* 		   (double)fabs((double)hkval2 - hkval1)); */
	break;
      case SET0:
            /* null method does nothing, used for filler Dac descriptors */
      default: 
            send_debug("No Array read");
            break;
    }

    if (trace_flag>10)
	send_debug("**arr_d2a_read:LEAVE:");
    return OK;

}

/******************************************************************************
 * Routine: arr_dconfig
 * Purpose: set values into the lab device controllers
 * Parameters: anum - int - array number to do the set for
 *	       device - int - number of the device to setup
 *	       value - int - value to set into device
 *	       stdbuf - char * - buffer for error messages
 * Returns:    int - error status
 *	       stdbuf - char * - filled with error message
 *****************************************************************************/
int arr_dconfig(anum, device, value, stdbuf)
int anum;
int device;
int value;
char stdbuf[];
{
    /* Not yet implemented */
    return (ERROR);
}

/******************************************************************************
 * Routine: arr_set_fconfig
 * Purpose: setup the filter wheels for the instrument
 * Parameters: anum - int - array number to do the set for
 *	       wheel - int - number of the wheel to setup
 *	       filt - int - number of the filter to move to
 *	       stdbuf - char * - buffer for error messages
 * Returns:       int - error status
 *	    stdbuf - char * - filled with error message
 *****************************************************************************/
int arr_set_fconfig(anum, wheel, value, stdbuf)
int anum;
int wheel;
float value;
char stdbuf[];
{
    /* Not yet implemented */
    return (ERROR);
}

