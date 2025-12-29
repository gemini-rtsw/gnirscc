/******************************************************************************
 * Program:	Instrument Configuration Software
 * File:        devdef.h
 * Purpose:     define stuff local to the servos, motors and other devices.
 *		defines all the configuration options needed for a servo.
 *		(maybe).
 *
 * Author:      Nick C Buchholz
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	25-Jun-1992 - created file - ncb
 *
 ****************************************************************************/

#ifndef CONFIG_DEFS
#include "config_st.h"
#endif
#define INST_OK		0
/* servo types */
#define T_MOTOR_ENC	0		/* motor encoder pair */
#define T_SOLENOID	1		/* solenoid device */
#define T_SWITCH	2		/* switch device */

#define SERVO_NOT_DONE		((unsigned int) 0x7fff)
#define SERVO_TIME_OUT		((int) 0x0010)

#define SERVO_ADDR(dnum)	(int *)((CEN_BASE) + (dnum))

#define SERVO_DATA(dd)		(dd)

#define SERVO_CMD(mnrd, cmd)	((uint)(((mnrd & 0xf) | (cmd << 8 )) & 0xFFFF))

#if !defined(IRS)
#define SERVO_ERROR(dta)	((uint)(dta & 0x8000))
#define SERVO_NUMBER(dta)	((uint)(((dta)>>13) & 0x3)) 	
#else
#define SERVO_ERROR(dta)	((uint)(dta & 0x80000000))
#define SERVO_NUMBER(dta)	((uint)(((dta)>>29) & 0x3)) 	
#endif

#define SERVOTIMEOUT	        0x500	/* count for timeout loop */

#define TICKSP10MS	157	/* number of procwait ticks in 10 millisecs */

/* servo commands */
#define SRVO_SET		0
#define SRVO_READV		1
#define SRVO_ALL_RESET		2
#define SRVO_ALL_OFF		3
#define SRVO_ALL_ON		4
#define SRVO_RD_LIMITS		5
#define SRVO_SW_RESET		7
#define SRVO_UPDPOS		8
#define SRVO_LAMP_SET		9
#define SRVO_LAMP_READ		10
#define SRVO_SETD		11

#define WAIT_SIZ	.064

/* motor control board commands */
#define READRESETS	0x01
#define MTRMOV		0x02
#define MTRREADPOS	0x03
#define SOLENERGZ	0x04
#define READLIMITS	0x05
#define LAMP_SET	0x11
#define LAMP_READ	0x12
#define SETD_LOWV	0x13
#define SETD_HIV	0x14
#define MOVEMTR		0x15
#define SETMTR		0x16
#define RD_LOWV		0x17
#define RD_HIV		0x18


#define REINITSW	0xFFFF


























