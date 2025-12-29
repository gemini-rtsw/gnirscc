static char rcid[]="$Id: dev_funcs.c,v 1.2 2009/05/27 19:33:25 fkraemer Exp $";
/******************************************************************************
 * Program:	INSTRUMENT CONTROL software
 * File:        dev_funcs.c     
 * Purpose:     Motor and Lab device controlused by the  INSTRUMENT CONTROL
 *		transputer. 
 *
 * Author:      Nick C Buchholz 
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	25-Jun-1992 - created file - ncb
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
 
#include "config_st.h"	       /* description of configuration information */
#include <instdefs.h>          /* definitions specific to instrument */

#define NOINSTVARS
#include <instvars.h>          /* global variables specific to instrument */ 
#define NOINSTPROCS
#include <instprocs.h>		/* process descriptors & channel assignments*/
#include "prototypes.h"
#include "devdefs.h"			/* device variables and structures */
#include <ierrorno.h>
/******************************************************************************
 * Program: Inst_Control - control software for Wildfire based instruments
 * Routine: safe_devs
 * Purpose: Puts all servos into safe states.
 * Parameters: none
 * Returns: int - 0 if OK,  error_code if command failed.
 *
 *****************************************************************************/
int safe_devs()
{
    int i, res, err;
    struct srvo_desc *sr;		/* pointer to servo descriptor */

    
#if !defined(SQIID) && !defined(BARACUDA) && !defined(NAAC)
     if ((err = me_servo_cmd(SRVO_ALL_OFF, 0, &res, 0)) != INST_OK)
    {
	send_debug("Error: servo %d returned error %d, data=0x%8.8x",i ,err, res);
    }
    ProcWait(10);
    if ((err = me_servo_cmd(SRVO_ALL_ON, 0, &res, 0)) != INST_OK)
    {

	send_debug("Error: servo %d returned error %d, data=0x%8.8x",i ,err, res);
    }	
    ProcWait(9375);

    for (i=0; i<inst.maxservos; i++)
    {
	sr = &(inst.servos[i]);
	if (sr->type != T_MOTOR_ENC)
	{
	    wheel_pos[i] = (float) -1;
	    continue;
	}
	if ((err = me_servo_cmd(SRVO_READV, i, &res, 0)) != INST_OK)
	{
	    send_debug("Error: sd: servo %d returned error %d, data=0x%8.8x",i ,err, res);
	    continue;
	}

	sr->v.motor.curval = res;
	wheel_pos[i] = (float) res;
    }
#endif
	     
    return INST_OK;
}


int updpos()
{
    int i, res, err;
    struct srvo_desc *sr;		/* pointer to servo descriptor */
    
#if !defined(SQIID) && !defined(BARACUDA) && !defined(NAAC)
    for (i=0; i<inst.maxservos; i++)
    {
	sr = &(inst.servos[i]);
	if (sr->type != T_MOTOR_ENC)
	{
	    wheel_pos[i] = (float) -1;
	    continue;
	}
	if ((err = me_servo_cmd(SRVO_READV, i, &res, 0)) != INST_OK)
	{
	    send_debug("Error: up: servo %d returned error %d, data=0x%8.8x",i , err, res);
	    continue;
	}

	sr->v.motor.curval = res;
	wheel_pos[i] = (float) res;
    }
#endif
	     
    return INST_OK;
}

int servo_delay(sr, srvonum, data, tout)
struct srvo_desc *sr;		/* pointer to servo descriptor */
int srvonum;
int *data, *tout;
{
    int i = 0;

    if (trace_flag >40 )
    {
	send_debug("**servo_delay: sr=0x%8.8x, srvonum=%d, data=0x%8.8x, tout=0x%8.8x", sr, srvonum, *data, *tout);
    }

    ProcWait(sr->cd_delay);
    do
    {
	mod_scb_reg(NONE, DATA_BUS);
	mod_scb_reg(inst.servos[srvonum].data_scb, NONE);
	*data = (*(RD_DBUS_ADDR) ) & 0xFFFF ;
	ProcWait(10);
	++i;
    }
    while ( (i <= SERVOTIMEOUT) && ((unsigned int)*data != SERVO_NOT_DONE) );
    *tout = i;
   
}

/******************************************************************************
 * Program: Inst_Control - control software for Wildfire based instruments
 * Routine: servo_cmd
 * Purpose: Handles servo commands for upper level devices
 * Parameters:
 * Returns: int - 0 if OK,  error_code if command failed.
 *
 *****************************************************************************/
int me_servo_cmd(cmd, srvonum, fval, dir)
int cmd;				/* desired command to execute */
int srvonum;				/* number of device to use */
int *fval;				/* desired final value & return*/
int dir;				/* direction to move 0==CCW, 1==CW */
{
    struct srvo_desc *sr;		/* pointer to servo descriptor */
    int *servo_addr;			/* pointer to servo hardware */
    int data, res;			/* data return and result of set */
    int i;				/* count of tries to read valid data */
    int mv_delay;			/* time to wait for move to complete */

#if defined(SQIID) || defined(BARACUDA) || defined(ALADDIN) || defined (ALAD1024) && !defined(NAAC)
    return (INST_OK);
#endif
    if (srvonum > inst.maxservos )
	return (INVALID_SERVO);

    sr = &(inst.servos[srvonum]);

    if (sr->type != T_MOTOR_ENC)
    {
	send_debug("Error: Servo %d is not a motor/encoder pair.");
	return (INVALID_TYPE);
    }

    if (cmd == SRVO_UPDPOS)
    {
	if (trace_flag >30)
	    send_debug("** me_servo_cmd: addr %8.8x, board %d, dev %d, cmd %8.8x, fval %d",
		       servo_addr, sr->mjrdev, sr->mnrdev, cmd, *fval);
	updpos();
	return (INST_OK);
    }

    mv_delay = sr->dlyovhd + (abs(sr->v.motor.curval - *fval) *
			      			sr->v.motor.dlypstp);
    servo_addr = SERVO_ADDR(sr->mjrdev);
    if (trace_flag >30)
	send_debug("** me_servo_cmd: addr %8.8x, board %d, dev %d, cmd %8.8x, fval %d",
		   servo_addr, sr->mjrdev, sr->mnrdev, cmd, *fval);

    switch (cmd)
    {
      case SRVO_SET:
	SemP(dbus_lock);
	write_dbus(sr->mjrdev,  (int)SERVO_CMD(sr->mnrdev, MTRMOV));
	servo_delay(sr, srvonum, &data, &i);
	if (i >= SERVOTIMEOUT)
	{
	    SemV(dbus_lock);
	    return (SERVO_TOUT);
	} 
	write_dbus(sr->mjrdev,	*fval);
	ProcWait(((int)((float)mv_delay / WAIT_SIZ) + 1));
	break;
	
      case SRVO_READV:
	SemP(dbus_lock);
	write_dbus(sr->mjrdev, (int)SERVO_CMD(sr->mnrdev, MTRREADPOS));
	servo_delay(sr, srvonum, &data, &i);
	if (i >= SERVOTIMEOUT)
	{
	    SemV(dbus_lock);
	    return (SERVO_TOUT);
	} 
	write_dbus(sr->mjrdev, (int)SERVO_CMD(sr->mnrdev, MTRREADPOS));
	ProcWait(sr->cd_delay);
	break;
	
      case SRVO_ALL_RESET:
	/* check if srnum == 0 */
	mod_scb_reg(inst.servos[srvonum].reset_scb, NONE);
	ProcWait(sr->cd_delay + sr->dlyovhd);
	mod_scb_reg(NONE, inst.servos[srvonum].reset_scb);
	ProcWait(sr->cd_delay + sr->dlyovhd);
	return (INST_OK);

      case SRVO_SW_RESET:	/* initiate a software reset */
	SemP(dbus_lock);
	write_dbus(sr->mjrdev, 0xFFFF);
	servo_delay(sr, srvonum, &data, &i);
	if (i >= SERVOTIMEOUT)
	{
	    SemV(dbus_lock);
	    return (SERVO_TOUT);
	} 
	write_dbus(sr->mjrdev, 0xFFFF);
	ProcWait(sr->cd_delay + sr->dlyovhd);
	SemV(dbus_lock);
	return (INST_OK);

      case SRVO_ALL_OFF:
	/* check if srvonum == 0 */
	mod_scb_reg(inst.servos[srvonum].reset_scb, NONE);
	ProcWait(sr->cd_delay);
	return (INST_OK);
	
      case SRVO_ALL_ON:
	/* check if srvonum == 0 */
	mod_scb_reg(NONE, inst.servos[srvonum].reset_scb);
	ProcWait(sr->cd_delay + sr->dlyovhd);
	return (INST_OK);
	
      case SRVO_RD_LIMITS:
	SemP(dbus_lock);
	write_dbus(sr->mjrdev, (int)SERVO_CMD(sr->mnrdev, READLIMITS));
	servo_delay(sr, srvonum, &data, &i);
	if (i >= SERVOTIMEOUT)
	{
	    SemV(dbus_lock);
	    return (SERVO_TOUT);
	} 
	write_dbus(sr->mjrdev, data);
	ProcWait(sr->cd_delay);
	break;
       default:
	break;
    }
    data = 0;
    i = 0;
    do
    {
	mod_scb_reg(NONE, DATA_BUS);
	mod_scb_reg(inst.servos[srvonum].data_scb, NONE);
	data = (*(RD_DBUS_ADDR) ) & 0xFFFF ;
	ProcWait(10);
	++i;
	mod_scb_reg(NONE, DATA_BUS);
    }
    while ((i <= SERVOTIMEOUT) && ((unsigned int)data == SERVO_NOT_DONE));

    SemV(dbus_lock);
    if (trace_flag >30)
	send_debug("** me_servo_cmd: mjr %d, dev %d, cmd %8.8x, data 0x%08x ",
		   sr->mjrdev, sr->mnrdev, cmd, data);
    if (trace_flag >30)
	send_debug("** me_servo_cmd: error %d, num %d, dev %8.8x, data 0x%08x ",
		   SERVO_ERROR(data), SERVO_NUMBER(data), sr->mnrdev &0x03, data);

    if (SERVO_ERROR(data) && (SERVO_NUMBER(data) == ((uint)sr->mnrdev & 0x3)))
	return (data & 0xFFFF);
    else if (i > SERVOTIMEOUT)
    {
	*fval =  SERVO_TOUT;
	return (SERVO_TOUT);
    }
    else if (SERVO_NUMBER(data) != ((uint)sr->mnrdev & 0x3))
	return (SERVO_FUP);

    if (cmd == SRVO_SET || cmd == SRVO_READV)
    {
	wheel_pos[srvonum] = (float) (data & 0x1fff); 
	sr->v.motor.curval = (data & 0x1fff);
    }

    *fval = (data & 0x1FFF);

    if (SERVO_NUMBER(data) != ((uint)sr->mnrdev & 0x3))
	return (SERVO_FUP);
    else 
	return (INST_OK);
}

/*******************************************************************************
 * Program: Inst_Control - control software for Wildfire based instruments
 * Routine: sol_servo_cmd
 * Purpose: Handles servo commands for upper level devices
 * Parameters:
 * Returns: int - 0 if OK,  error_code if command failed.
 *
  ****************************************************************************/
int sol_servo_cmd(cmd, srvonum, fval)
int cmd;			/* desired command to execute */
int srvonum;			/* number of device to use */
int *fval;			/* desired final value or value return */
{
    struct srvo_desc *sr;		/* pointer to servo descriptor */
    int *servo_addr;			/* pointer to servo hardware */
    int data, res;			/* data return and result of set */
    int i;				/* count of tries to read valid data */
    int enrg_delay;			/* time to wait for move to complete */

    if (srvonum > inst.maxservos )
	return (INVALID_SERVO);

    sr = &(inst.servos[srvonum]);
    if (sr->type != T_SOLENOID)
    {
	send_debug("Error: Servo %d is not a solenoid.");
	return (INVALID_TYPE);
    }

    if (cmd == SRVO_UPDPOS)
    {
	safe_devs();
	return (INST_OK);
    }

    enrg_delay = sr->dlyovhd + (*fval * TICKSP10MS);
    servo_addr = SERVO_ADDR(sr->mjrdev);

    switch (cmd)
    {
      case SRVO_SET:
        if (trace_flag >30)
	    send_debug("** sol_servo_cmd: addr %8.8x, board %d, dev %d, cmd %8.8x, fval %d",
		       servo_addr, sr->mjrdev, sr->mnrdev, cmd, *fval);
 	SemP(dbus_lock);
	write_dbus(sr->mjrdev,  (int)SERVO_CMD(sr->mnrdev, SOLENERGZ));
	ProcWait(sr->cd_delay);
	write_dbus(sr->mjrdev,	*fval);
	ProcWait(sr->cd_delay);
	ProcWait((int)(enrg_delay / WAIT_SIZ) + 1);
	break;
	
      case SRVO_READV:		/* not used by solenoids */
	return (INST_OK);
	break;
	
      case SRVO_ALL_RESET:
	/* check if srnum == 0 */
	mod_scb_reg(inst.servos[srvonum].reset_scb, NONE);
	ProcWait(sr->cd_delay);
	mod_scb_reg(NONE, inst.servos[srvonum].reset_scb);
	ProcWait(sr->cd_delay + sr->dlyovhd);
	return (INST_OK);
	
      case SRVO_ALL_OFF:
	/* check if srvonum == 0 */
	mod_scb_reg(inst.servos[srvonum].reset_scb, NONE);
	ProcWait(sr->cd_delay);
	return (INST_OK);
	
      case SRVO_ALL_ON:
	/* check if srvonum == 0 */
	mod_scb_reg(NONE, inst.servos[srvonum].reset_scb);
	ProcWait(sr->cd_delay + sr->dlyovhd);
	return (INST_OK);

      default:	
	break;
    }
    
    i = 0;
    do
    {
	mod_scb_reg(NONE, DATA_BUS);
	mod_scb_reg(inst.servos[srvonum].data_scb, NONE);
	data = *(RD_DBUS_ADDR) & 0xFFFF;
	ProcWait(10);
	++i;
    }
    while (((unsigned int) data == SERVO_NOT_DONE) && (i <= SERVO_TOUT));

    SemV(dbus_lock); 
    if (SERVO_ERROR(data) && (SERVO_NUMBER(data) == (uint)sr->mnrdev))
	return (data && 0xFFFF);
    or (i > SERVOTIMEOUT)
	return (SERVO_TOUT);
    else if (SERVO_ERROR(data))
	return (SERVO_FUP);
    
    *fval = (data && 0x1FFF);
    if (SERVO_NUMBER(data) != (uint)sr->mnrdev)
	return (SERVO_FUP);
    else 
	return (INST_OK);
}













