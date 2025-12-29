/* motor.h */
/* supported command set to be used by record support */

#define HMOTOR_MOVE_ABS		0
#define HMOTOR_MOVE_REL		1
#define HMOTOR_HOME_FOR		2
#define HMOTOR_HOME_REV		3
#define HMOTOR_HOME_ENC		4
#define HMOTOR_LOAD_POS		5
#define HMOTOR_SET_VEL_BASE	6
#define HMOTOR_SET_VELOCITY	7
#define HMOTOR_SET_ACCEL		8
#define HMOTOR_GO				9
#define HMOTOR_SET_ENC_RATIO	10
#define HMOTOR_GET_MOTOR_POS	11
#define HMOTOR_GET_ENCODER_POS	12
#define HMOTOR_GET_INFO		13
#define HMOTOR_STOP_AXIS		14
#define HMOTOR_JOG				15
#define HMOTOR_POWER_ON			16
#define HMOTOR_POWER_OFF			17

struct hmotor_table {
	unsigned char	type;
	char	*command;
	int	num_parms;
	};

/* -------------------------------------------------- */

/* driver and device support parameters */
#define HMOTOR_SCAN_RATE	6		/* 60=once a second */
#define HMOTOR_CNULL		(char *)NULL
#define HMOTOR_MAX_COUNT	50000 /*19000*/		/* timeout value */
#define HMOTOR_MESS_SIZE	300		/* maximum message size - guess */
#define HMOTOR_MAX_AXIS	10		/* max number of axis per board */

#define HMOTOR_NOTHING_DONE	0
#define HMOTOR_CALLBACK_DATA 	1

#define HMOTOR_NO		0
#define HMOTOR_YES		1

/* -------------------------------------------------- */
/* axis and encoder status for return to requester */
#define HMOTOR_RA_DIRECTION		0x01	/* (last) 0=Negative, 1=Positive */
#define HMOTOR_RA_DONE			0x02	/* a motion is complete */
#define HMOTOR_RA_OVERTRAVEL		0x04	/* a limit switch has been hit */
#define HMOTOR_RA_HOME			0x08	/* The home signal is on */
#define HMOTOR_EA_SLIP			0x10	/* encoder slip enabled */
#define HMOTOR_EA_POSITION		0x20	/* position maintenence enabled */
#define HMOTOR_EA_SLIP_STALL		0x40	/* slip/stall detected */
#define HMOTOR_EA_HOME			0x80	/* encoder home signal on */
#define HMOTOR_EA_PRESENT		0x100	/* encoder is present */
#define HMOTOR_RA_PROBLEM		0x200	/* driver stopped polling */
#define HMOTOR_RA_MOVING		0x400	/* non-zero velocity present */

/*
   The HMOTOR_RA_PROBLEM status bit indicates that the driver has stopped the
   1/10 second polling because it believes that the motor is really not
   moving.  Under normal operation, the OMS board will tell the driver
   when the motor is done moving.  This is safety precaution and should
   normally be treated like a HMOTOR_RA_DONE.
*/

