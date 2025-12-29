static char rcid[]="$Id: ctio_devs.c,v 1.2 2009/05/27 19:33:25 fkraemer Exp $";
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
    int i, res=0, err=0;
    struct srvo_desc *sr;		/* pointer to servo descriptor */

    
#if !defined(SQIID) && !defined(BNSB)
     if ((err = me_servo_cmd(SRVO_ALL_OFF, 0, &res, 0)) != INST_OK)
    {
	send_debug("sd1: servo %d returned error %d, data=0x%8.8x",i ,err, res);
    }
    ProcWait(10);
    if ((err = me_servo_cmd(SRVO_ALL_ON, 0, &res, 0)) != INST_OK)
    {

	send_debug("sd2: servo %d returned error %d, data=0x%8.8x",i ,err, res);
    }	
    ProcWait(9375);

    res = err = 0;
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
	    send_debug("sd3: servo %d returned error %d, data=0x%8.8x",i ,err, res);
	    continue;
	}

	sr->v.motor.curval = res;
	wheel_pos[i] = (float) res;
    }
#endif
	     
    return INST_OK;
}

/*****************************************************************************
 * Routine: updpos()
 * Purpose: updates the memory array holding the positions of all motors and 
 *		devices.
 *
 ****************************************************************************/
int updpos()
{
    int i, res, err;
    struct srvo_desc *sr;		/* pointer to servo descriptor */
    
#if !defined(SQIID) && !defined(BNSB)
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
	    send_debug("up: servo %d returned error %d, data=0x%8.8x",i , err, res);
	    continue;
	}

	sr->v.motor.curval = res;
	wheel_pos[i] = (float) res;
    }
#endif
	     
    return INST_OK;
}


/*****************************************************************************
 * Routine: servo_delay()
 * Purpose: waits for the embedded controler to output data or run motors
 *
 ****************************************************************************/
int servo_delay(sr, srvonum, data, tout)
struct srvo_desc *sr;		/* pointer to servo descriptor */
int srvonum;
int *data, *tout;
{
    int i = 0;

    if (trace_flag >40 )
    {	send_debug("**servo_delay: sr=0x%8.8x, srvonum=%d, data=0x%8.8x, tout=0x%8.8x", sr, srvonum, *data, *tout);
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
 * Routine:	servo_read_data
 * Purpose:	reads a data word from the servo board trys for .32 seconds for
 *		data other than 0x7fff.  times out if not recieved
 * Parameters:  srvonum - int - number of the servo to use
 *		data - int * - data retrieved
 * Returns:     OK - if data other than 0x7fff recieved 
 * 		SERVO_TOUT - if 0x7fff only data for SERVOTIMEOUT * 640 us 
 *				(500 * 640) about .320 seconds
 * Notes: Caller should insure exclusive access to bus with dbus_lock semaphore
 *****************************************************************************/
int servo_read_data(int srvonum, int *data)
{
    int i=0;
    if (trace_flag > 30)
	send_debug("**servo_read_data: srvonum=%d,, data=%8.8x", srvonum, *data);
    do
    {
	mod_scb_reg(NONE, DATA_BUS);
	mod_scb_reg(inst.servos[srvonum].data_scb, NONE);
	*data = (*(RD_DBUS_ADDR) ) & 0xFFFF ;
	ProcWait(10);
	++i;
	mod_scb_reg(NONE, DATA_BUS);
    }
    while ((i <= SERVOTIMEOUT) && ((unsigned int)(*data) == SERVO_NOT_DONE));
    if (i >= SERVOTIMEOUT)  {  return (SERVO_TOUT);  } 
    if (trace_flag > 45)
	send_debug("**servo_read_data: srvonum=%d,, data=%8.8x, i=%d", srvonum, *data, i);
    return (OK);

}

/*******************************************************************************
 * Routine:	servo_write_data
 * Purpose:	writes a data word to the servo board waits for response
 *		0x7fff times out if not recieved
 * Parameters:  sr - struct srvo_desc * - pointer to servo descriptor
 *		srvonum - int - number of the servo to use
 *		data - int * - data retrieved
 * Returns:     OK - if 0x7fff recieved 
 * 		SERVO_TOUT - if no 0x7fff in SERVOTIMEOUT * 640 us (500 * 640)
 *				about 320 ms
 * Notes: Caller should insure exclusive access to bus with dbus_lock semaphore
 *
 *****************************************************************************/
int servo_write_cmnd(sr, srvonum, cmnd, data) 
struct srvo_desc *sr;
int srvonum;
int cmnd;
int *data;
{
    int i=0;

    if (trace_flag > 30)
	send_debug("**servo_write_cmnd: sr=%8.8x, srvonum=%d,, data=%8.8x, i=%8.8x", sr, srvonum, *data, i);
    write_dbus(sr->mjrdev,  cmnd );
    ProcWait(sr->cd_delay);
    write_dbus(sr->mjrdev,  *data );
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
    if (i >= SERVOTIMEOUT)  {  return (SERVO_TOUT);  } 
    if (trace_flag >44)
	send_debug("**servo_write_cmd: data %8.8x, i %d", *data, i);
    return (OK);
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
    int cmnd;				/* hardware level command */
    int lowv=0, hiv=0;			/* storage for high and low portions 
					   of data for setting and reading 
					   CTIO encoders */

    if (trace_flag >30)
	send_debug("**me_servo_cmd1: cmd %8.8x, srvonum %8.8x, fval %d, dir %d",
		   cmd, srvonum, *fval, dir);

#if defined(SQIID) || defined(BNSB) || defined(ALADDIN) || defined(ALAD1024)
    return (INST_OK);
#endif
    if (srvonum > inst.maxservos )
	return (INVALID_SERVO);

    sr = &(inst.servos[srvonum]);

    if (sr->type != T_MOTOR_ENC)
    {
	send_debug("Servo %d is not a motor/encoder pair.");
	return (INVALID_TYPE);
    }

    if (cmd == SRVO_UPDPOS)
    {
	if (trace_flag >34)
	    send_debug("**me_servo_cmd2: addr %8.8x, board %d, dev %d, cmd %8.8x, fval %d",
		       servo_addr, sr->mjrdev, sr->mnrdev, cmd, *fval);
	updpos();
	return (INST_OK);
    }

    mv_delay = sr->dlyovhd + (abs(sr->v.motor.curval - *fval) *
			      			sr->v.motor.dlypstp);
    servo_addr = SERVO_ADDR(sr->mjrdev);

    if (trace_flag >30)
	send_debug("**me_servo_cmd3: addr %8.8x, board %d, dev %d, cmd %8.8x, fval %d",
		   servo_addr, sr->mjrdev, sr->mnrdev, cmd, *fval);

    switch (cmd)
    {
      case SRVO_SET:
      case SRVO_SETD:
	SemP(dbus_lock);
	lowv = ((*fval) & 0x00000FFF);
	hiv = (((*fval) & 0x00FFF000) >> 12);
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd: SERVO_SET(D): cmd=%8.8x, lowv,hiv=%8.8x,%8.8x, fval=%8.8x", cmd, lowv, hiv, *fval);

	cmnd = SERVO_CMD(sr->mnrdev, SETD_LOWV); 
	res = servo_write_cmnd(sr,  srvonum, cmnd, &lowv);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	res = servo_read_data(srvonum, &data);
	if (trace_flag > 34) 
	    send_debug("**me_servo_cmnd: SERVO_SET(D):  after SETD_LOWV data %8.8x", data);

	cmnd = SERVO_CMD(sr->mnrdev, SETD_HIV); 
	res = servo_write_cmnd(sr, srvonum, cmnd, &hiv);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	res = servo_read_data(srvonum, &data);
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  SERVO_SET(D):  after SETD_HIV data %8.8x", data);

	if (cmd == SRVO_SET)
	{
	    cmnd = SERVO_CMD(sr->mnrdev, MOVEMTR);
	    res = servo_write_cmnd(sr, srvonum, cmnd, &data);
	    if (res == SERVO_TOUT) {  SemV(dbus_lock);  return (SERVO_TOUT); } 
	    if (trace_flag > 34)
		send_debug("**me_servo_cmnd:  SERVO_SET:  after MOVEMTR");
 	    ProcWait(((float)mv_delay / WAIT_SIZ) + 1);
	}
	else if (cmd == SRVO_SETD)
	{
	    cmnd = SERVO_CMD(sr->mnrdev, SETMTR);
	    servo_write_cmnd(sr, srvonum, cmnd, &data);
	    if (res == SERVO_TOUT)  {  SemV(dbus_lock);  return (SERVO_TOUT); } 
	    if (trace_flag > 34)
		send_debug("**me_servo_cmnd:  SERVO_SET:  after SETMTR");
 	    ProcWait(((float)10 / WAIT_SIZ) + 1);
	}
	res = servo_read_data(srvonum, &data);
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  after move data %8.8x", data);
	data = ((data & 0xE000)<<16) | (*fval & 0xffffff);
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  after move data %8.8x", data);
	break;

      case SRVO_READV:
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  SERVO_READV:  cmd=%8.8x, lowv,hiv=%8.8x,%8.8x, fval=%8.8x", cmd, lowv, hiv, *fval);
	SemP(dbus_lock);
	cmnd = SERVO_CMD(sr->mnrdev, RD_LOWV);
	res = servo_write_cmnd(sr, srvonum, cmnd, &lowv);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	res = servo_read_data(srvonum, &lowv);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  SERVO_READV:  lowv=%d (%8.8x)", lowv,lowv);

	cmnd = SERVO_CMD(sr->mnrdev, RD_HIV);
	res = servo_write_cmnd(sr, srvonum, cmnd, &hiv);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	res = servo_read_data(srvonum, &hiv);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  SERVO_READV:  hiv=%d (%8.8x)", hiv, hiv);
	data = (lowv & 0xFFF) | ((hiv & 0xFFF) << 12) | ((hiv & 0xE000) << 16) ;
	break;

      case SRVO_ALL_RESET:		/* hardware reset */
	/* check if srnum == 0 */
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  SERVO_ALL_RESET:  cmd=%8.8x, fval=%8.8x", cmd, *fval);
	mod_scb_reg(inst.servos[srvonum].reset_scb, NONE);
	ProcWait(sr->cd_delay + sr->dlyovhd);
	mod_scb_reg(NONE, inst.servos[srvonum].reset_scb);
	ProcWait(10 * (sr->cd_delay + sr->dlyovhd));
	return (INST_OK);

      case SRVO_SW_RESET:	/* initiate a software reset */
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  SERVO_SW_RESET:  cmd=%8.8x, fval=%8.8x", cmd, *fval);
	SemP(dbus_lock);
	write_dbus(sr->mjrdev, 0xFFFF);
	ProcWait(sr->cd_delay);
	write_dbus(sr->mjrdev, 0xFFFF);
	ProcWait(10 * (sr->cd_delay + sr->dlyovhd));
	SemV(dbus_lock);
	return (INST_OK);

      case SRVO_ALL_OFF:
	/* check if srvonum == 0 */
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  SERVO_ALL_OFF:  cmd=%8.8x, fval=%8.8x", cmd, *fval);
	mod_scb_reg(inst.servos[srvonum].reset_scb, NONE);
	ProcWait(sr->cd_delay);
	return (INST_OK);
	
      case SRVO_ALL_ON:
	/* check if srvonum == 0 */
	if (trace_flag > 34)
	    send_debug("**me_servo_cmnd:  SERVO_ALL_ON:  cmd=%8.8x, fval=%8.8x", cmd, *fval);
	mod_scb_reg(NONE, inst.servos[srvonum].reset_scb);
	ProcWait(10 * (sr->cd_delay + sr->dlyovhd));
	return (INST_OK);

#if 0	
      case SRVO_FIND_RPOS:
	mv_delay = sr->dlyovhd + ((sr->v.motor.maxval-sr->v.motor.minval) *
			  				sr->v.motor.dlypstp);
	SemP(dbus_lock); /* send servop command */
	write_dbus(sr->mjrdev, (SERVO_CMD(sr->mnrdev, FINDRESET))); 
	servo_delay(sr, srvonum, &data, &i);
	if (i >= SERVOTIMEOUT)
	{
	    SemV(dbus_lock);
	    return (SERVO_TOUT);
	} 
	write_dbus(sr->mjrdev, (SERVO_DATA(*fval)));
	ProcWait((mv_delay / WAIT_SIZ) + 1);
	break;
#endif
	
      case SRVO_RD_LIMITS:
	SemP(dbus_lock);
	write_dbus(sr->mjrdev, (SERVO_CMD(sr->mnrdev, READLIMITS)));
	servo_delay(sr, srvonum, &data, &i);
	if (i >= SERVOTIMEOUT)
	{
	    SemV(dbus_lock);
	    return (SERVO_TOUT);
	} 
	write_dbus(sr->mjrdev, data);
	ProcWait(sr->cd_delay);
	data = 0;
	res = servo_read_data(srvonum, &data);
	data = ((uint)(data & 0xE000) << 16) | (data & 0x1FFF);
	break;

      default:
	break;
    }

    SemV(dbus_lock);
    if (trace_flag >30)
	send_debug("**me_servo_cmd: mjr %d, dev %d, cmd %8.8x, data 0x%08x ",
		   sr->mjrdev, sr->mnrdev, cmd, data);

    if (SERVO_ERROR(data) && (SERVO_NUMBER(data) == ((uint)sr->mnrdev & 0x3)))
	return (data & 0xFFFFFFFF);
    else if (res == SERVO_TOUT)
    {
	*fval =  SERVO_TOUT;
	return (SERVO_TOUT);
    }
    else if (SERVO_NUMBER(data) != ((uint)sr->mnrdev & 0x3))
	return (SERVO_FUP);

    if (cmd == SRVO_SET || cmd == SRVO_READV)
    {
	wheel_pos[srvonum] = (float) (data & 0xFFFFFF); 
	sr->v.motor.curval = (data & 0xFFFFFF);
    }

    *fval = (data & 0xFFFFFF);

    if (SERVO_NUMBER(data) != ((uint)sr->mnrdev & 0x3))
	return (SERVO_FUP);
    else 
	return (INST_OK);
}

/******************************************************************************
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
    int cmnd;				/* hardware command time */

    if (trace_flag >30)
	send_debug("**sol_servo_cmd: cmnd %8.8x, srvonum %d, fval %d", 
		       cmd, srvonum, *fval);

    if (srvonum > inst.maxservos )
	return (INVALID_SERVO);

    sr = &(inst.servos[srvonum]);
    if (sr->type != T_SOLENOID)
    {
	send_debug("Servo %d is not a solenoid.");
	return (INVALID_TYPE);
    }

    if (cmd == SRVO_UPDPOS)
    {
	safe_devs();
	return (INST_OK);
    }

    enrg_delay = sr->dlyovhd + (*fval * TICKSP10MS);
    servo_addr = SERVO_ADDR(sr->mjrdev);
    data = *fval;

    switch (cmd)
    {
      case SRVO_SET:
        if (trace_flag >34)
	    send_debug("**sol_servo_cmd: SOL_SET: addr %8.8x, board %d, dev %d, cmd %8.8x, fval %d",
		       servo_addr, sr->mjrdev, sr->mnrdev, cmd, *fval);
 	SemP(dbus_lock);
	cmnd = SERVO_CMD(sr->mnrdev, SOLENERGZ);
	res = servo_write_cmnd(sr, srvonum, cmnd, &data);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	break;
	
      case SRVO_LAMP_SET:
        if (trace_flag >34)
	    send_debug("**sol_servo_cmd: LAMP_SET: addr %8.8x, board %d, dev %d, cmd %8.8x, fval %d",
		       servo_addr, sr->mjrdev, sr->mnrdev, cmd, *fval);
 	SemP(dbus_lock);
	cmnd =  SERVO_CMD(sr->mnrdev, LAMP_SET);
	res = servo_write_cmnd(sr, srvonum, cmnd, &data );
        if (trace_flag >34)
	    send_debug("**sol_servo_cmd: LAMP_SET: data %8.8x", data);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	res = servo_read_data(srvonum, &data );
        if (trace_flag >34)
	    send_debug("**sol_servo_cmd: LAMP_SET: data %8.8x", data);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	SemV(dbus_lock); 
	return (INST_OK);
	break;

      case SRVO_LAMP_READ:		/* not used by solenoids */
        if (trace_flag >34)
	    send_debug("**sol_servo_cmd: LAMP_READ: addr %8.8x, board %d, dev %d, cmd %8.8x, fval %d",
		       servo_addr, sr->mjrdev, sr->mnrdev, cmd, *fval);
 	SemP(dbus_lock);
	cmnd =  SERVO_CMD(sr->mnrdev, LAMP_READ);
	res = servo_write_cmnd(sr, srvonum, cmnd, &data );
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	res = servo_read_data(srvonum, &data);
	if (res == SERVO_TOUT) 	{  SemV(dbus_lock);  return (SERVO_TOUT); } 
	SemV(dbus_lock);
	*fval = data;
	return (INST_OK);
	break;
	
      case SRVO_ALL_RESET:
	/* check if srnum == 0 */
	mod_scb_reg(inst.servos[srvonum].reset_scb, NONE);
	ProcWait(sr->cd_delay);
	mod_scb_reg(NONE, inst.servos[srvonum].reset_scb);
	ProcWait(sr->cd_delay + sr->dlyovhd);
	return (INST_OK);

      case SRVO_SW_RESET:	/* initiate a software reset */
	SemP(dbus_lock);
	write_dbus(sr->mjrdev, 0xFFFF);
	ProcWait(sr->cd_delay);
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

      default:	
	break;
    }
    
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













