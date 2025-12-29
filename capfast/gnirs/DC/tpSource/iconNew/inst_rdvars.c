static char rcid[]="$Id: inst_rdvars.c,v 1.2 2009/05/27 19:33:26 fkraemer Exp $";
#define INST_CTRLR 1
/* Command and control process for INSTRUMENT CONTROL. */
/******************************************************************************
 * Program:	INSTRUMENT CONTROL software
 * File:        inst_rd_vars..c     
 * Purpose:     Read all variables, d2a and a2d converters in an
 *		instrument.  
 *
 * Author:      Nick C Buchholz - revised from Dick Fredericksen's sqidctrl.c
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	22-Apr-1991 - created file - dhf
 *      06-Nov-1991 - revised orginal file - ncb
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
#include <common.h>

#include "config_st.h"	       /* description of configuration information */
#include <instdefs.h>          /* definitions specific to INST */
#define NOINSTVARS
#include <instvars.h>          /* global variables specific to INST */ 
#define NOINSTPROCS              /* already defined in sqidmain */
#include <instprocs.h>	       /* process descriptors & channel assignments*/
#include "devdefs.h"
#include <ierrorno.h>
#include "prototypes.h"

#if 0  /* removed 1-Mar-93 - ncb - not needed */
/******************************************************************************
 * Routine: 	read_creg_var()
 * Purpose: 	routine used to read inst control register values 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_creg_var(header, buf)
int header;
int *buf;
{
    int *pbuf;

    if (trace_flag > 30)
	send_debug("**read_creg_var: header=0x%8.8x, buf=0x%8.8x, SCBreg_val=0x%8.8x", header, buf, SCBreg_val);

    header=TO_NODE(buf[REPLY_TO]) | MESSAGE(VAR_READ) | OF_LENGTH(3);

    /* always a single element	 */
    pbuf = mem_alloc(sizeof(int) * 3);

    pbuf[RETURN_VAL] = SCBreg_val;
   
    pbuf[VAR_NUM] = buf[VAR_NUM];
    pbuf[FROM] = _node_number;/* send message with value of */

    SEND(header, (char *) pbuf, Control_to_Reader); /* var[VAR_NUM] to REPLY_TO */

}
#endif

/******************************************************************************
 * Routine: 	read_camera_pwr()
 * Purpose: 	routine used to read inst camera power value 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_camera_pwr(header, buf)
int header;
int *buf;
{
    int *pbuf;

    if (trace_flag > 30)
	send_debug("**read_camera_pwr: header=0x%8.8x, buf=0x%8.8x, camera_pwr is %s", header, buf, ((SCBreg_val & ENBL_DET_BIAS) ? "on" : "off"));

    header=TO_NODE(buf[REPLY_TO]) | MESSAGE(VAR_READ) | OF_LENGTH(3);

    /* always a single element	 */
    pbuf = mem_alloc((int) (sizeof(int) * 3));

    pbuf[RETURN_VAL] = SCBreg_val & ENBL_DET_BIAS;
   
    pbuf[VAR_NUM] = buf[VAR_NUM];
    pbuf[FROM] = _node_number;/* send message with value of */

    SEND(header, (char *) pbuf, Control_to_Reader); /* var[VAR_NUM] to REPLY_TO */

}

/******************************************************************************
 * Routine: 	read_d2a_var()
 * Purpose: 	routine used to read a D2A converter value 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_d2a_var(header, buf)
int header;
int *buf;
{
    float fval;
    int anum;
    int *pbuf;
    int i;
    int d2a_idx,d2a_grp;
    int first, last;
    char stdbuf[MAXLINE];

  
   /*  index = ARR_INDEX(buf); */
    anum = ARR_INDEX(buf);
    d2a_idx = D2A_INDEX(buf);
    d2a_grp = D2A_GROUP(buf);	
    stdbuf[0] = '\0';

 

  /*   send_debug("**buf = %8.8x %8.8x %8.8x ", buf[0], buf[1], buf[2]); */
  
	
    if (trace_flag > 30)
	send_debug("**read_d2a_var: header=0x%8.8x, buf=0x%8.8x, d2a_idx=%d, d2a_grp = %d d2a_val=%f", header, buf, d2a_idx,d2a_grp,(double) array_d2a[d2a_idx]);

   arr_d2a_read(anum, d2a_grp, d2a_idx, array_d2a, stdbuf);

    header=TO_NODE( 0 ) | MESSAGE(VAR_READ) | OF_LENGTH(3);

 
 /* always a single element	 */
    pbuf = mem_alloc((int) (sizeof(int) * 3));
   
    (float) pbuf[RETURN_VAL] = array_d2a[d2a_grp]; 
    
    pbuf[VAR_NUM] = buf[VAR_NUM];
    pbuf[FROM] = buf[2];/* send message with value of */

/* 	send_debug("**read_d2a_var: header=0x%8.8x,  d2a_val=%f%%%%%",  */
/* 		   header,(double) array_d2a[d2a_grp]); */

    SEND(header, (char *) pbuf, Control_to_Reader); /* var[VAR_NUM] to REPLY_TO */
}

/******************************************************************************
 * Routine: 	read_filter_var()
 * Purpose: 	routine used to read a filter position value 
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_servo_var(header, buf)
int header;
int *buf;
{
    int i;
    int anum, servo, baddr, err = 0;
    int *pbuf;
    int rval;
    int spclcmd;

    anum = ARR_INDEX(buf);
    header=TO_NODE( 0 ) | MESSAGE(VAR_READ) | OF_LENGTH(3);
    servo = SERVO_NUM(buf);
    spclcmd = SERVO_SPCMD(buf);
 
    /* always a single element	 */
    pbuf = mem_alloc((int) (sizeof(int) * 3));
    if (trace_flag > 30)
	send_debug("**read_servo_var1: anum=%d, servo=%d", anum, servo);

    switch (inst.servos[servo].type)
    {
      case T_MOTOR_ENC:  /* read the value */
	err = me_servo_cmd(spclcmd, servo, &rval, 0);
        if (err == INST_OK)
#if !defined(IRS)
	    pbuf[RETURN_VAL] = (rval & 0x1fff );
#else
	    pbuf[RETURN_VAL] = (rval & 0xffffff );
#endif
	else
	{
	    send_debug("Error %d in servo read back.", err);
	    pbuf[RETURN_VAL] = err;
	}
	break;
      case T_SOLENOID:
	err = sol_servo_cmd(spclcmd, servo, &rval);
        if (trace_flag > 30)
	    send_debug("**read_servo_var???: err=%d, rval=%d", err, rval);
        if (err == INST_OK)
#if !defined(IRS)
	    pbuf[RETURN_VAL] = (rval & 0x1fff );
#else
	    pbuf[RETURN_VAL] = (rval & 0xffffff );
#endif
	else
	{
	    send_debug("Error %d in servo read back.", err);
	    pbuf[RETURN_VAL] = err;
	}
	break;

      case T_SWITCH:
	pbuf[RETURN_VAL] = inst.servos[servo].v.sswitch.curpos;
	break;
    };

    if (trace_flag > 30)
	send_debug("**read_servo_var: err=%d, rval=%d", err, rval);
	
    pbuf[VAR_NUM] = buf[VAR_NUM];
    pbuf[FROM] = _node_number;/* send message with value of */
    SEND(header, (char *)pbuf, Control_to_Reader); /* var[VAR_NUM] to REPLY_TO*/

}

/******************************************************************************
 * Routine: 	read_spad_mode()
 * Purpose: 	routine used to read back a spad mode register
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_spad_mode(header, buf)
int header;
int *buf;
{
    int i;
    int anum, card, baddr, err = 0;
    int *pbuf;
    int rval;

    anum = ARR_INDEX(buf);
    header=TO_NODE( 0 ) | MESSAGE(VAR_READ) | OF_LENGTH(3);
    card = CARD_NUM(buf);

    if (trace_flag > 30)
	send_debug("**read_spad_mode: anum=%d, card=%d", anum, card);

    if (card == -1)			/* want to read all values */
    {
        header=TO_NODE( 0 ) | MESSAGE(VAR_READ) | OF_LENGTH(2 + NUM_A2D);
	pbuf = mem_alloc((int) (sizeof(int) * (NUM_A2D + 2)));	/* NUM_A2D values returned */
	for (i = 0; i < NUM_A2D; i++)
#if  defined(SQIID) 
	{
	    pbuf[RETURN_VAL + i] = spad_mode[anum][i];
        }
#elif defined(CRSP) || defined(IRIM) || defined(IRS) || defined(CIRIM)
        {
	    err = -1;
	    pbuf[RETURN_VAL + i] = spad_mode[anum][i];
	}
#else
        {
	    err = write_dbus(C_Brd_Addr_Ld,
			     	inst.array[anum].spad[i].mjr_num );
	    if (!err)
		pbuf[RETURN_VAL + i] = read_dbus ( SEL_SPAD_DTA ) & 0x1ff;
	    else
		pbuf[RETURN_VAL + i] = -1;
	}
#endif
    }
    else
    {
	pbuf = mem_alloc((int) (sizeof(int) * 3));	/* one value returned */
#if defined(SQIID) || defined(CRSP) || defined(IRIM) || defined(IRS) || defined(CIRIM)
	pbuf[RETURN_VAL] = spad_mode[anum][card];
    }
#else
	err = write_dbus( C_Brd_Addr_Ld, inst.array[anum].spad[card].mjr_num );
	if (!err)
	{
	    rval =  read_dbus ( SEL_SPAD_DTA ) & 0x1ff;
	    pbuf[RETURN_VAL] = rval;
	    if (trace_flag > 2)
		send_debug("**read_spad_mode: card %d, rval %8.8x, pbuf %8.8x", card,
			   	rval, pbuf[RETURN_VAL]);
	}
	else
	    pbuf[RETURN_VAL] = -1;
    }
    write_dbus(C_Brd_Addr_Ld, 0xFFFF);

#endif

    pbuf[VAR_NUM] = buf[VAR_NUM];
    pbuf[FROM] = _node_number;/* send message with value of */
    SEND(header, (char *) pbuf, Control_to_Reader); /* var[VAR_NUM] to REPLY_TO */
   
}

/******************************************************************************
 * Routine: 	read_hk()
 * Purpose: 	routine used to read a HouseKeeping channel value
 * Parameters:  int - header - header of message sent to start this command
 *		int * - buf - pointer to the body of the message
 * 		int *[] - var_ptr - pointer to the array of variable pointers
 * Returns:     int - error - OK or ERROR depending on outcome
 *****************************************************************************/
int read_hk(header, buf)
int header;
int *buf;
{
    int h;
    int i;
    int *p;

/* 148 -> 250 ; remove NUMSERVOS line, sizeof(int) -> sizeof(float)  rjw */
/* node: from 1 to 0 */
    h = MESSAGE(SET_VAR) | TO_NODE(0) | OF_LENGTH(250 + 1);
    p = mem_alloc((int) (250 * sizeof(float) + 4));

    p[0] = 10;
    if (LENGTH(header) == 1)
	p[0] = buf[0];
    bcopy(var_ptr[HKBUF], p + 1, 250 * sizeof(float));
/* rjw 5/12/97
    bcopy(var_ptr[WHEEL_POS], p + 1 + HK_CHANS, NUMSERVOS * sizeof(int));
 */
 
    SEND(h, p, Control_to_Reader);

    return 0;
}


int readDataSimul(header, buf)
int header;
int *buf;
{
}

