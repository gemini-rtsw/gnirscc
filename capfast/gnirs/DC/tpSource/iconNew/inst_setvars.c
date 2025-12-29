static char rcid[]="$Id: inst_setvars.c,v 1.2 2009/05/27 19:33:27 fkraemer Exp $";
#define INST_CTRLR 1
/* Command and control process for INSTRUMENT CONTROL. */
/******************************************************************************
 * Program:	INSTRUMENT CONTROL software
 * File:        inst_setvars..c     
 * Purpose:     Set all variables, d2a converters in an
 *		instrument.  Also handles setting lab device control.
 *
 * Author:      Nick C Buchholz 
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	28-Apr-1993 - created file - ncb
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conc.h>
#include <math.h>
#include <string.h>

#include "irstd.h"
#include <protdefs.h>
#include <protocol.h>

#include "config_st.h"	       /* description of configuration information */
#include <instdefs.h>          /* definitions specific to INST */
#define NOINSTVARS
#include <instvars.h>          /* global variables specific to INST */ 
#define NOINSTPROCS            /* already defined in sqidmain */
#include <instprocs.h>	       /* process descriptors & channel assignments*/
#include "devdefs.h"
#include <ierrorno.h>
#include "prototypes.h"

/******************************************************************************
 * Routine: 	no_set()
 * Purpose: 	error routine used when upper level attempts to set a read only
 *		variable
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int no_set(header, buf)
int header;
int *buf;
{
    char pbuf[MAXLINE];

    send_debug("ERROR Variable is read only no set possible.");
    return(ERROR);

}

/******************************************************************************
 * Routine: 	set_SCBreg_var()
 * Purpose: 	routine used to set inst control register values 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int set_SCBreg_var(header, buf)
int header;
int *buf;
{
    int on, off, err;
 
    on = buf[VAR_VAL];		/* get value to set into SCB */
    off = ~ on;
    if (trace_flag>20)
	send_debug("**set_SCBreg_var: on = %8.8x, off = %8.8x", on, off);

    err = mod_scb_reg(on, off);
    send_debug("SCB Register set to %8.8x", SCBreg_val);
    return(OK);

}

/******************************************************************************
 * Routine: 	set_camera_pwr()
 * Purpose: 	routine used to activate the arrays 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int set_camera_pwr(header, buf)
int header;
int *buf;
{
    int which_var, mina, maxa;
    int index, err, i;
    char pbuf[MAXLINE];
    char pbuf2[MAXLINE];
    pbuf[0] = '\0';
    which_var = VAR_NUMVAL(buf);
    index = ARR_INDEX(buf) - 1;

    if (index == -1)		/* set all camera power bits on */
    {
	mina = 0; maxa = NUM_ARRAYS;
    }
    else
    {
	mina = index; maxa = index + 1;
    }

    if (trace_flag>20)
	send_debug("**set_camera_power: index %d, mina=%8.8x, maxa=%8.8x,val = %d",
		   	index, mina, maxa,buf[VAR_VAL]);
	
    for(i = mina; i < maxa; i++)
    {
	err |= arr_power(i, buf[VAR_VAL], pbuf);
        if (err)
          break;
    }
	    
    if (!err)  /* on err send_debug called from arr_power */
    {
        if (mina < maxa -1)
           send_debug ("Vars %d - %d set", mina, maxa);
        else if (mina+1 == maxa)
           send_debug (pbuf);
        else
           send_debug ("No variables to set");
    } 
    return(err);
}


/******************************************************************************
 * Routine: 	set_d2a_var()
 * Purpose: 	routine used to set a D2A converter value 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int set_d2a_var(header, buf)
int header;
int *buf;
{
    float fval;
    int anum, min, max;
    int d2a, i, err = 0;
    int d2a_idx, d2a_grp;
    int ecnt = 0;
    struct d2a_cfg *d2adesc;
    char pbuf[MAXLINE];

    anum = ARR_INDEX(buf);

    pbuf[0] = '\0';
    err = ecnt = 0;
    
    d2a_idx = D2A_INDEX(buf);
    d2a_idx = (d2a_idx & 0x8000) ? (d2a_idx | 0xFFFF0000) : d2a_idx ;

    d2a_grp = D2A_GROUP(buf);
    fval = D2A_VAL(buf);

    if (trace_flag>20)
	send_debug("**set_d2a_var: anum=%d, d2agrp=%d, d2aidx=%d fval=%f",
		   	anum, d2a_grp, d2a_idx, (double)fval);

    d2adesc = &(inst.array[anum].voltage[d2a_grp]);
 
    if (d2a_idx == -1)
    {
	min = 0; 
	max = d2adesc->num_in_grp - 1;
    }
    else
    {
	min = max = d2a_idx;
    }

    for (i=min; i<=max; i++)
    {
	if (err = arr_d2a_set(anum, d2a_grp, i, fval,pbuf))
	    ecnt++;
    }
	
    return (err);
}


/******************************************************************************
 * Routine: 	set_servo_var()
 * Purpose: 	routine used to set a filter position value 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int set_servo_var(header, buf)
int header;
int *buf;
{
    int array, servo, spclcmd, value;
    int err = 0;

    array = ARR_INDEX(buf);

    servo = SERVO_NUM(buf);
    spclcmd = SERVO_SPCMD(buf);
    value = OBJ_VAL(buf);
    if (trace_flag>20)
	send_debug("**set_servo_var1: array %d, servo %d cmd=%8.8x, value %d",
		   	array, servo, spclcmd, value);

#if defined(SQIID)
    send_debug("ERROR No controllable devices in Sqiid.");
    return (-1);
#else
    switch (inst.servos[servo].type)
    {
      case T_MOTOR_ENC:
    if (trace_flag>20)
	send_debug("**set_servo_var2: array %d, servo %d cmd=%8.8x, value %d",
		   	array, servo, spclcmd, value);

	err = me_servo_cmd(spclcmd, servo, &value, 0);
	if (err == INST_OK && spclcmd == SRVO_SET)
#if !defined(IRS)
	    send_debug("%d", (value & 0x1FFF));
#else
	    send_debug("%d", (value & 0xFFFFFF));
#endif
	else if  (err == SERVO_TOUT)
	    send_debug("Servo movement timed out");
	else if  (err == SERVO_FUP)
	    send_debug("ERROR Wrong servo responded,(mtr = %d, error = %d)", 
		       ((uint)((err)>>13) & 0x3), err & 0x03);
	else if (spclcmd == SRVO_SET)
	{
	    err &= 0xF;
	    switch (err)
	    {
	      case 1:
		send_debug("ERROR Servo %d failed: JAMMED.", servo); 
		break;
	      case 3:
		send_debug("ERROR Servo %d failed: POSITION TOO LOW.",servo); 
		break;
	      case 2:
		send_debug("ERROR Servo %d failed: POSITION TOO HIGH.",servo); 
		break;
	      case 4:
		send_debug("ERROR Servo %d failed: NO POSITION RECEIVED.",servo); 
		break;
	      case 5:
		send_debug("ERROR Servo %d failed: GRATING NOT IN DETENT POSITION.",servo); 
		break;
	      case 6:
		send_debug("ERROR Servo %d failed: GRATING IN SHORT WAVE LENGTH STOP.",servo); 
		break;
	      case 7:
		send_debug("ERROR Servo %d failed: GRATING IN LONG WAVE LENGTH STOP.",servo); 
		break;
	      default:
		send_debug("ERROR Servo %d failed: error %8.8x.", servo,err); 
		break;
	    }
	}
	else if (err == INST_OK)
	    send_debug("Servo command completed.");
	else 
	    send_debug("ERROR: Servo command failed with error %8.8x!", err);
	break;

      case T_SOLENOID:
	err = sol_servo_cmd(spclcmd, servo, &value);
	if (err == INST_OK && spclcmd == SRVO_SET)
	    send_debug("Solenoid %d energized successfully for %d ms.",
		       		servo, value );
	else  if (spclcmd == SRVO_SET)
	    send_debug("ERROR: Solenoid %d failed with error %8.8x.", servo, err);
	else  if (err == INST_OK && spclcmd == SRVO_LAMP_SET)
	    send_debug("Lamp %d turned %s.", 
		       		inst.servos[servo].mnrdev, 
		       			((value) ? "on" : "off"));
	else  if (spclcmd == SRVO_LAMP_SET)
	    send_debug("ERROR: lamp %d failed with error %8.8x.", 
		       		inst.servos[servo].mnrdev, err);
	else if (err == INST_OK)
	    send_debug("Solenoid Servo command completed.");
	else 
	    send_debug("ERROR: Servo command failed! with error %8.8x ",err);
	break;

      case T_SWITCH:
	send_debug("ERROR: Switches are not implemented!");break;

	break;
    }

    return(err);
#endif    
}

/******************************************************************************
 * Routine: 	set_spad_mode()
 * Purpose: 	routine used to set a spad card mode register
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int set_spad_mode(int header, int *buf)
{
    int anum, card, value;
    int err = 0;

    anum = ARR_INDEX(buf);
    card = CARD_NUM(buf);
    value = MODE_VAL(buf);

    if (trace_flag>20)
	send_debug("**set_spad_mode: array %d, servo %d value %d",
		   	anum, card, value);

#if defined(SQIID) || defined(CRSP) || defined(IRIM) || defined(IRS) || defined(CIRIM)
    /* SQIID, CRSP & IRIM have no mode registers the array number is all
       that's needed to control the spad_mode */
    err = write_spad_mode(card, value);
#else
    err = write_spad_mode(inst.array[anum].spad[card].mjr_num,  value );
#endif

    return(err);
}

int setDataSimul(header, buf)
int header;
int *buf;
{


}

