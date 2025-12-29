static char rcid[]="$Id: inst_hdw.c,v 1.2 2009/05/27 19:33:25 fkraemer Exp $";
#define INST_CTRLR 1
/* Command and control process for INSTRUMENT CONTROL. */
/******************************************************************************
 * Program:	INSTRUMENT CONTROL software
 * File:        inst_hdw.c     
 * Purpose:     Control hardware of instrument. d2a converters, motors and 
 *		SCB regioster
 *
 * Author:      Nick C Buchholz
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	22-Apr-1991 - created file - ncb
 *      06-Nov-1991 - revised orginal file - ncb
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conc.h>
#include <math.h>

#include "irstd.h"
#include <protdefs.h>
#include <protocol.h>

#include "config_st.h"	       /* description of configuration information */
#include <instdefs.h>          /* definitions specific to INST */
#define NOINSTVARS
#include <instvars.h>          /* global variables specific to INST */ 
#define NOINSTPROCS              /* already defined in sqidmain */
#include <instprocs.h>	       /* process descriptors & channel assignments*/
#include "prototypes.h"

/******************************************************************************
 * Routine:	write_dbus
 * Purpose:	writes a data word to the standard databus using an appropriate
 *		CEN to 	select destination.
 * Parameters:  cen - int - clock enable bit to use as selector
 *		dta - int - 16 bit value to write
 * Returns:     OK - 
 * Notes: Caller should insure exclusive access to bus with dbus_lock semaphore
 *
 *****************************************************************************/
int write_dbus(cen, data)
int cen;
int data;
{
    int *cenaddr = (int *)((CEN_BASE) + (cen));
    int *dbusaddr = HKC_REG_ADDR; 

    if ((trace_flag > 40) && (cen != HK_PA_BEN) && (cen != HK_ADC_BEN))
	send_debug("**write_dbus:ENTER  cen = %8.8x, data = %8.8x, cenaddr = %8.8x", cen, data, cenaddr);
    if ((trace_flag > 50) && a2d_freeze)
	send_debug("**write_dbus:ENTER  cen = %8.8x, data = %8.8x, cenaddr = %8.8x", cen, data, cenaddr);

    mod_scb_reg(NONE, DATA_BUS);
    mod_scb_reg(SEL_DBUS, NONE);
    *dbusaddr = data;		/* load dbus registers */
    *cenaddr = data;		/* trigger clock pulse */

    return OK;
}

/******************************************************************************
 * Routine:	read_dbus
 * Purpose:	reads a data word from the databus using an appropriate SCB to
 * 		select destination.
 * Parameters:  scb - int - Static control bit to use as selector
 * Returns:     OK - 
 *
 *****************************************************************************/
int read_dbus(scb)
int scb;
{
    int *dbusaddr = RD_DBUS_ADDR;
    int data;

    mod_scb_reg(NONE, DATA_BUS);	/* turn off all data bus drviers */
    mod_scb_reg(scb, NONE);		/* enable databus drivers requested */
    data = *dbusaddr;			/* read the data bus */

    mod_scb_reg(NONE, scb);		/* turn the requested drivers off */

    if (trace_flag>50)
	send_debug("**read_dbus:LEAVE  scb = %8.8x, data = %8.8x", scb, data);

    return (data);			/* return the data */
}

/* these are duplicates of the above for use in CRSP IRIM & IRS */
#if defined(CRSP) || defined(IRIM) || defined(IRS) || defined(CIRIM)
/******************************************************************************
 * Routine:	write_adbus
 * Purpose:	writes a data word to the analog box databus in CRSP IRIM IRS
 *		using an appropriate SCB to select destination
 *
 * Parameters:  cen - int - clock enable bit to use as selector
 *		dta - int - 16 bit value to write
 * Returns:     OK - 
 * Notes: Caller should insure exclusive access to bus with dbus_lock semaphore
 *
 *****************************************************************************/
int write_adbus(scb, data)
int scb;
int data;
{
    int *dbusaddr = WR_DBUS_ADDR; 
    int *cicr_addr = (int *)((CEN_BASE) + (C_CIC_REG));
    int *dstrb = DSTRB_ADDR;

    if (trace_flag>40)
	send_debug("**write_adbus:  SCB = %8.8x, data = %8.8x", scb, data);

    mod_scb_reg(NONE, DATA_BUS);	/* turn off All Dbus Drivers */
    mod_scb_reg(SEL_DBUS, NONE);	/* turn on ICON DBus Drivers */

    *dbusaddr = data;			/* load dbus registers */
    *cicr_addr = data;			/* write to CIC register */
    mod_scb_reg(scb, NONE);		/* Select the appropriate Dest w SCB */

    *dstrb = data;			/* load data register on ADU side*/
    mod_scb_reg(NONE, scb);		/* Select the appropriate Dest w SCB */

    return OK;
}

/******************************************************************************
 * Routine:	read_adbus
 * Purpose:	reads a data word from the analog databus using an appropriate
 *		SCB to select destination.  does and OR of the SCB and the
 *		SEL_CIC_OUTPUT SCB in CRSP IRIM IRS
 * Parameters:  scb - int - Static control bit to use as selector
 * Returns:     OK - 
 *
 *****************************************************************************/
int read_adbus(scb)
int scb;
{
    int *dbusaddr = RD_DBUS_ADDR;
    int data;

    mod_scb_reg(NONE, DATA_BUS);	/* turn off all data bus drviers */

    scb |= SEL_CIC_OUTPUT;		/* also select the CIC DMAI drivers */
    mod_scb_reg(scb, NONE);		/* enable databus drivers requested */
    data = *dbusaddr;			/* read the data bus */

    mod_scb_reg(NONE, scb);		/* turn the requested drivers off */

    if (trace_flag>50)
	send_debug("**read_adbus:  scb = %8.8x, data = %8.8x", scb, data);


    return (data);			/* return the data */
}

#endif /* functions for CRSP, IRIM, CIRIM & IRS*/

/******************************************************************************
 * Routine:	write_a2daddr
 * Purpose:	writes an a2d address for the housekeeping board to
 * 		select channel.
 * Parameters:  scb - int - Static control bit to use as selector
 * Returns:     OK - 
 *
 *****************************************************************************/
int write_a2daddr(addr)
int addr;
{
    int *dbusaddr = WR_DBUS_ADDR;
#if defined(IRIM) || defined(CRSP) || defined(IRS) || defined(CIRIM)
    int *cenaddr = (int *)((CEN_BASE) + (C_CIC_REG));
    int *dstrb = DSTRB_ADDR;
#endif

    if (trace_flag>50)
	send_debug("**write_a2daddr:ENTER addr = %8.8x", addr);

#if defined(IRIM) || defined(CRSP) || defined(IRS) || defined(CIRIM)

    mod_scb_reg(NONE, DATA_BUS);	/* turn off All Dbus Drivers */
    mod_scb_reg(SEL_HK_ADDR, NONE);	/* turn on ICON DBus Drivers */
    *dbusaddr = addr;			/* load ICON dbus registers */
    *cenaddr = addr;			/* write to CIC register */

    *dstrb = addr;			/* load data register on ADU side*/

#else /* this code for COB or PHOENIX */

    mod_scb_reg(NONE, DATA_BUS);	/* turn off All Dbus Drivers  */
    mod_scb_reg(SEL_HK_ADDR, NONE);	/* turn on ICON DBus Drivers */
    *dbusaddr = addr;			/* write data to HK card register */

#endif
    ProcWait(1);			/* wait for voltage to settle */

    return OK;
}

/******************************************************************************
 * Routine:	strobe_a2d
 * Purpose:	strobes the housekeeping board a2d converter
 * Parameters:  none 
 * Returns:     OK - 
 *
 *****************************************************************************/
int strobe_a2d()
{
    int *strbaddr = HK_A2D_CTC;

    *strbaddr = 0;			/* data written is unimportant */
    ProcWait(1);			/* wait for conversion to complete */
    return OK;
}

/******************************************************************************
 * Routine: 	mod_scb_reg()
 * Purpose: 	routine used to set inst control register values 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * 		int *[] - var_ptr - pointer to the array of variable pointers
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int mod_scb_reg(on, off)
int on;
int off;
{
    if (trace_flag>45 & a2d_freeze)
	send_debug("**mod_scb_reg:ENTER addr = 0x%8.8x, scbval = 0x%8.8x, on=0x%8.8x, off=0x%8.8x", SCBreg_ptr, SCBreg_val, on, off);

    SemP(SCBreg_lock);			/* set lock on register */
    SCBreg_val = (SCBreg_val | on) & (~ off);/* change register values */
    *scb_reg = SCBreg_val;		/* fill in powerup register  */
    *SCBreg_ptr = SCBreg_val;		/* update register */ 
    SemV(SCBreg_lock);			/* release lock */

    if (trace_flag>45 & a2d_freeze)
	send_debug("**mod_scb_reg:LEAVE addr = 0x%8.8x, scbval = 0x%8.8x, on=0x%8.8x, off=0x%8.8x", SCBreg_ptr, SCBreg_val, on, off);

    return OK;
}


/******************************************************************************
 * Routine:	setd2a_12b()
 * Purpose:	set a value into a 12 bit hardware dac
 * Parameters:	mjr_num - int - major device number ie board address
 *		cen_num - int - clock enable bit to use
 *		bval - int - value to set 0 to 0xFFF 
 * Returns:	int - 0 if OK, ERROR if error occurred
 *****************************************************************************/
int setd2a_12b(mjr_num, cen_num, bval,stdbuf)
int mjr_num;
int cen_num;
int bval;
char stdbuf[];
{

    if (trace_flag > 35)
	send_debug("**setd2a_12b:ENTER cen %8.8x, addr %8.8x, bval %8.8x", cen_num, mjr_num, bval);
    SemP(dbus_lock);			/* lock out data bus use */
#if defined(SQIID)
    ;					/* no board addresses in SQIID */
#elif defined(CRSP) || defined(IRIM) || defined (IRS) || defined(CIRIM)
    write_adbus(cen_num,  bval);	/* CRSP/IRIM use a SCB not a CEN */
    SemV(dbus_lock);			/* free up data bus */
    return OK;
#else
    write_dbus(C_Brd_Addr_Ld, mjr_num);
    ProcWait(1);
#endif
    write_dbus( cen_num,  bval);
    SemV(dbus_lock);			/* free up data bus */

    if (trace_flag > 35)
	send_debug("**setd2a_12b:LEAVE ");
    return OK;
}

#if defined(SQIID)
/*****************************************************************************
 * SQIID has no mode register on the A2D cards the functions are simulated
 * using two SCB's on each card SEL_LO_GAIN_n and SEL_LO_FILT_n. we set these 
 * and fill the values into a variable.  ALl A2D's on an array are always set
 * to the same mode.
 ****************************************************************************/
int write_spad_mode( int baddr, int value)
{
    int on, off;

    if (trace_flag > 30)
	send_debug("**write_spad_mode(S): baddr = %8.8x, value = %8.8x", baddr, value);

    switch (value & 0x3)
    {
      case 0:				/* LO_GAIN off, LO_FILT off */
	on =  (0x11 << baddr);	off = NONE;		break;

      case 1:				/* LO_FILT on LO_GAIN off */
	on = (0x01 << baddr);	off = (0x10 << baddr);		break;

      case 2:				/* LO_FILT off, LO_GAIN on   */
	on = (0x10 << baddr);	off = (0x01 << baddr);		break;

      case 3:				/* LO_FILT on, LO_GAIN on */
	on = NONE;	        off = (0x11 << baddr);			break;
      default:
	send_debug("Error: Invalid spad_mode value %d.",value);
	return (-1);
    }

    mod_scb_reg(on, off);

    /* this is bogus, we know that sqiid only has two spads per array  and 
       they are always in the same mode */
    spad_mode[baddr][0] = spad_mode[baddr][1] = value;

    return OK;
}
 
/******************************************************************************
 * Routine: 	read_spad_mode()
 * Purpose: 	routine used to read a spad card mode register
 * Parameters:  int - baddr - address of card to be written
 *		*int - value - value to be stored in register
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_spmode( int baddr, int *value)
{
    int err=0;

    if (trace_flag > 30 )
	send_debug("**read_spad_mode(CI): baddr = %8.8x", baddr);
    *value = spad_mode[baddr][0];

    return (err);
}
#elif defined(CRSP) || defined(IRIM) || defined(IRS) || defined(CIRIM)
/*****************************************************************************
 * CRSP and IRIM have no mode registers on the A2D cards the functions are 
 * simulated using a single SCB - SEL_LO_FILT_n. is set in the SCB register
 * and into a variable.  ALL A2D's are always set to the same mode.
 ****************************************************************************/
int write_spad_mode( int baddr, int value)
{

    if (trace_flag > 30)
	send_debug("**write_spad_mode(CI): baddr = %8.8x, value = %8.8x", baddr, value);
    if (value < 0 || value > 1)
	return (OK);

    if (value == 0x01)			/* turn on LO_FILT */
	mod_scb_reg(SEL_LO_FILT, NONE);
    else
	mod_scb_reg(NONE, SEL_LO_FILT);

    /* this is bogus, we know that the old instruments have only two spads 
	and they are always in the same mode */
    spad_mode[0][0] = spad_mode[0][1] = value & 0x01;

    return (OK);
    
}

/******************************************************************************
 * Routine: 	read_spad_mode()
 * Purpose: 	routine used to read a spad card mode register
 * Parameters:  int - baddr - address of card to be written
 *		*int - value - value to be stored in register
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_spmode( int baddr, int *value)
{
    int err=0;

    if (trace_flag > 30 )
	send_debug("**read_spad_mode(CI): baddr = %8.8x", baddr);
    *value = spad_mode[0][0];

    return (err);
}
#else /* this code for Goldfish, COB or PHOENIX */
/******************************************************************************
 * Routine: 	write_spad_mode()
 * Purpose: 	routine used to set a spad card mode register
 * Parameters:  int - baddr - address of card to be written
 *		int - value - value to be stored in register
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int write_spad_mode( int baddr, int value)
{
    int err=0;

    if (trace_flag > 30 )
	send_debug("**write_spad_mode(GCP): baddr = %8.8x, value = %8.8x", baddr, value);
    SemP(dbus_lock);
    err = write_dbus( C_Brd_Addr_Ld, baddr );	/* select correct spad Board */
    if (!err)
	err = write_dbus( C_Spad_Mode_Ld, value);
    SemV(dbus_lock);

    return (err);
}

/******************************************************************************
 * Routine: 	read_spad_mode()
 * Purpose: 	routine used to read a spad card mode register
 * Parameters:  int - baddr - address of card to be written
 *		*int - value - value to be stored in register
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_spmode( int baddr, int *value)
{
    int err=0;

    if (trace_flag > 30 )
	send_debug("**read_spmode(GCP): baddr = %8.8x", baddr);
    SemP(dbus_lock);
    err = write_dbus( C_Brd_Addr_Ld, baddr );	/* select correct spad Board */
    if (!err)
	*value = read_dbus( SEL_SPAD_DTA );
    SemV(dbus_lock);

    return (err);
}

#endif






























