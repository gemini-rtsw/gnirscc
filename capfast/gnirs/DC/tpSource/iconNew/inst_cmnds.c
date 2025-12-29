static char rcid[]="$Id: inst_cmnds.c,v 1.2 2009/05/27 19:33:25 fkraemer Exp $";
/******************************************************************************
 * Program:	INSTRUMENT CONTROL software
 * File:        inst_cmnds.c     
 * Purpose:     Command and control function array used by INSTRUMENT CONTROL
 *		transputer. 
 *
 * Author:      Nick C Buchholz 
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	10-Dec-1991 - created file - ncb
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <conc.h>
#include <math.h>

#include <irstd.h>
#include <protdefs.h>
#include <protocol.h>

#include "config_st.h"	       /* description of configuration information */

#define NO_MEM
#include <common.h>
#include "instdefs.h"          /* definitions specific to instrument */
#define NOINSTVARS
#include "instvars.h"          /* global variables specific to instrument */ 

_MEMORY(128*0x400);

char *memory = (char *) (0x80000000 + 380 * 1024);
int *poweroff =  (int *) (0x80000000 + (512 * 1024) - 16);
int *scb_reg = (int *)  (0x80000000 + (512 * 1024) - 12);

command_form command[] = {
    		bad_msg,		/* IMAGE_DONE */	
		bad_msg,		/* BEGIN_XMIT */
		var,			/* SET_VAR */
		var,			/* READ_VAR */
		bad_msg,		/* DEBUG_MSG */
		abort_img,		/* ABORT_MSG */
		abort_img,		/* STOP_MSG */
		start_picture,		/* START_MSG */
		bad_msg,		/* PAUSE */
		bad_msg,		/* RESUME */
		bad_msg,		/* VAR_READ */
		read_hk,		/* READ_HK */
		var,			/* SET_VAR_AKK */
		bad_msg,		/* AKK */
		bad_msg,		/* AKK_FAIL */
		bad_msg,		/* KILL_PROC */
		bad_msg,		/* EXECUTE_PROC */
		check_hk,		/* CHECK_HK */
		bad_msg,		/* ERROR */
};

int num_command = 19;

int *var_ptr[] = {  /* VarNum  Purpose */
    &trace_flag,    /*  0     trace level var for the Xputer node programs */
    &num_arrays,    /*  1     number of arrays in this controller */
    &echo_me,	    /*  2     a large number used in debugging */
    &status_report, /*  3     error number for process tracking */
    &deactivate,    /*  4     true if arrays are deactivated before shutdown */
    hk_buf[0],	    /*  5     address of first HK buffer */
    &protection,    /*  6     true if array temp is checked before activation*/
    &SCBreg_val,    /*  7     software copy of the SCB register on the ICON */
    &cp_state,	    /*  8     software copy of activation state of array */
    array_d2a,	    /*  9     array of voltages set into DACs on PR/CD cards */
    filter_number,  /*  A     array of filter wheel settings NOT USED */
    &data_simul,    /*  B     software copy of data simulation state */
    &a2d_freeze,    /*  C     true if houseKeeping data generator is shutdown*/
    hk_buf[0],	    /*  D     used to read a single HK channel */
    wheel_pos,	    /*  E     NOT USED positions of servo encoders */
};

command_form setv_cmnds[] = {
    		set_var,		/* TRACE_FLAG      0    */
		set_var,		/* NUM_ARRAYS      1    */
		set_var,		/* ECHO_ME         2    */
		set_var,		/* STATUS_REPORT   3    */
		set_var,		/* DEACTIVATE      4    */
		no_set,			/* HK_BUF          5    */
		set_var,		/* PROTECTION      6    */
		set_SCBreg_var,		/* CREG_VAL        7    */
		set_camera_pwr,		/* CAMERA_POWER    8    */
		set_d2a_var,		/* ARRAY_D2A       9    */
		no_set,			/* SERVO_CTRL      A   */
		setDataSimul,		/* LCD_CTRL        B   */
		set_var,                /* A2DFREEZE       C    */
		no_set,	        	/* read one hkChan D    */
		no_set,			/* encoder positions  E */
};                                   

command_form readv_cmnds[] = {
    		read_var,		/* TRACE_FLAG      0    */
		read_var,		/* NUM_ARRAYS      1    */
		read_var,		/* ECHO_ME         2    */
		read_var,		/* STATUS_REPORT   3    */
		read_var,		/* DEACTIVATE      4    */
		read_var,		/* HK_BUF          5    */
		read_var,		/* PROTECTION      6    */
		read_var,		/* CREG_VAL        7    */
		read_camera_pwr,	/* CAMERA_POWER    8    */
		read_d2a_var,		/* ARRAY_D2A       9    */
		read_servo_var,	        /* SERVO_CTRL      A    */
		readDataSimul,		/* LCD_CTRL        B    */
		read_var,	        /* A2DFREEZE       C    */
		read_onehk,             /* read 1 HKChan   D    */
		read_var,		/* NOT USED        E    */
};                                   

Channel *chan_list[] = {
	NULL,
	NULL,
	&DEBUG_CHAN,
	LINK0IN,
	LINK1IN,
	LINK2IN,
	LINK3IN,
	NULL,
	NULL,
	NULL,
};

