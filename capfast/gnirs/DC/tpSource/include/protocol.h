/******************************************************************************
 * File:	protocol.h
 * Purpose:	header file to describe and define the protocol used by the
 *		dsp_worker controllers
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		1-15-91	created					dak
 *              1-31-91 added TOP_DSP and BOTM_DSP   		dhf
 *		2-14-91	BEGIN_XMIT propogates down		dak
 *              2-19-91 added ECHO_ME                           dhf 
 *              2-21-91 added STATUS_REPORT                     dhf 
 *		2-27-91	added some processor specific vars	dak
 *		2-27-91	changed details of
 *                      set_var/read_var/var_read		dak
 *		2-28-91	support for automatic updates of varnum
 *			added.					dak
 *		3-26-91	array indices added (transparent)	dak
 *		5-30-91 MEMORY variable added			dak
 *
 *****************************************************************************/

#if 0
protocol specifications:
a packet shall consist of the following:
int header
int arr[0..length-1]

this shall be transmitted sequentially starting with address
and ending with arr[length-1].  length shall be larger
or equal to zero. the addr is composed of a one byte address
(node_number) and a one byte length.  The masks below will decode
these.

messages shall be encoded in the header and accesed:
MESSAGE(header);
they should be written as header | msg.

messages shall be passed:
by a process which will determine the recipient.  if
the recipient is equal to the _node_number, then
that node is the recipient.  Otherwise, pass the entire
packet along in the proper direction.

The message structure is as follows:
struct message {
    struct header {
	byte destnode;
	byte length;
	byte fromnode;
	byte message;
    } header;
    union buffer {			/* each message type is in the union */
	int buf[head.length];	/* actual c description */
	struct image_done {		/* message type ALL_LOWER_DONE nada */ };
	struct begin_xmit {		/* begin transmitting picture nothing */ };
	struct set_var {            /* for standard variable setting */
	    short array_index;
	    short varnum;
	    union var {
		switch (VARNUM)
		    default:
    		int val[head.length - 1];
		case ARRAY_D2A:
/* for various instrument hardware variables */
		struct arr_d2a {	/* for ARRAY_D2A */
		    int which_d2a;	/* which D2A to change */
		    float d2a_val;	/* floating pt value to set d2a to */
		}; 
		case SERVO_CTRL:
		struct servo_ctrl {	/* for SERVO_CTRL */
		    short servo_num;	/* which servo to move  */
		    short servoindex;
		    int servo_val;	/* value to set or time to leave on */
		};
		case DEV_CTRL:
		struct dev_ctrl {	/* for DEV_CTRL */
		    int dev_num;	/* which device to modify */
		    int dev_val;	/* value to set */
		};
		case SPAD_MODE:
		struct spad_mode {
		    int card_num;
		    int mode_val;
		};

	    };

	    struct read_var {
		short array;		/* array index 1 through n, 0 means all */
		short varnum;		/* variable number */
		int reply_to;
		union length {
		    int length;	/* 0 == use strlen, otherwise # of ints */
		    void nada;
		};
	    };

	    struct var_read {
		short array;		/* array index 1 through n, 0 means all */
		short varnum;		/* variable number */
		int from;
		int val[head.length - 2];
	    };

	    struct debug_msg { 
		int msg[head.length];	/* this is filled with chars */
	    };

	} message;

#endif
/*
    messages:
    ALL_LOWER_DONE:
    	arguments: none
	 purpose:  to signal that all dsps lower on the chain than
	   the recipient are ready to transmit data
    BEGIN_XMIT:
	 arguments: none
	 purpose:  to signal the controller that processors above are ready
	   to recieve the results.  Note-- the processor that recieves
		   this message will send it down to the next processor.
     SET_VAR:
	 arguments: arr[VAR_NUM]=variable_number (see below)
		    arr[VAR_VAL]=new_value
	 purpose:  to set a variable on the dsp workers, such as LNR.  Strings
		   can also be set by changing the LENGTH field of the header.
		   In this case, the new value will be in arr[VAR_VAL ..

		   VAR_VAL+LENGTH-2].
			See the instrument variable description for the
		    structure of mesages to set the special instrument
		    level variables. 
     SET_VAR_AK:
	 arguments: same as SET_VAR
	 purpose:   same as SET_VAR.  This should only be sent out to the
		    sqiid controller, all other transputers will ignore it.  
		    The b011 will wait until it gets a reply (AKK) from sqidc &
		    relay this to the sun.
			See the instrument variable description for the
		    structure of mesages to set the special instrument
		    level variables. 
     AKK:
	 arguments:  like a debug msg
	 purpose:    acknowledge a SET_VAR_AK
     AKK_FAIL:
	 arguments:  like a debug msg
	 purpose:    say the akk fails
     READ_VAR:
	 arguments: arr[VAR_NUM]=variable_number (see below)
		    arr[REPLY_TO]=address of recipient
		    arr[LENGTH_TO_SEND]=number of words to send (optional)
	 purpose:  to read a value on one of the worker dsps and get
		   a VAR_READ back.  If the LENGTH(header) == 2, a single word
		   will be returned. If LENGTH(header) == 3, then
		   arr[LENGTH_TO_SEND] will contain the number of words
		   to send, except that if arr[LENGTH_TO_SEND] == 0, then send
		   a string of the proper length.
     VAR_READ:
	 arguments: arr[VAR_NUM]=variable number value
		    arr[RETURN_VAL]=value read
		    arr[FROM]=_node_number of sender
	 purpose:  to return values read by READ_VAR.  The variable value
		   will be in arr[RETURN_VAL .. RETURN_VAL+LENGTH-2].
     READ_HK:
	 arguments: none
	 purpose:   to signal that housekeeping data capture should start
		    taking place.
     DEBUG_MSG:
	 arguments: arr[1..length-1]=text
	 purpose:  to allow dsp workers to send information to other
		   dsps, especially the main controller. (text)
     ABORT_MSG:
	 arguments:
	 purpose:  to abort the current data taking procedure and discard
		   the results
     STOP_MSG:
	 arguments:
	 purpose: to stop the current data taking procedure and return the
		  results
     START_MSG:
	 arguments: none
	 purpose:  top signal that picture data capture should start taking
		   place
     PAUSE:
	 arguments: none
	 purpose:  to halt the processing of data (it will still be collected
		   and discarded).  This will take place after the current
		   "picture" has been dealt with.
     RESUME:
	 arguments: none
	 purpose:  to restart the processing of data after being PAUSED.
		   possible problem:  where are we when we get back?
     KILL_PROCS:
	 arguments: none
	 purpose:  to kill all processes on a transputer.  This message should
		   only go to the sequencers, as they are the only ones that
		   will handle it.
     EXECUTE_PROC:
	 arguments: none.
	 purpose:  this will start executing another process.  This other
		   process's code is in the prog array.  (see hd(1))
     CONFIG:
	 arguments: arr = new configuration structure value
	 purpose:  to setup the variable parts of the system. Used at the startup
		   to reconfigure hardware.

     NEXT_MSG:

*/
/* HARDWARE LEVEL INSTRUMENT VARIABLES
   These variables require special handling.  The structure for the SET_VAR
    and SET_VAR_AKK message is enhanced for these variable types.
    The base packet will still look like
	     int header
	     int arr[0..length-1]
    However the argument structure is enhanced.  The variable which require this are
    array_d2a - this sets the D2A's associated with each array.
    filters - moves a filter on a wheel into the optical path.
    ldc_control - changes the state of some lsb or instrument device.
    Spad_mode - changes spad card operating parameters

    Normally the arguments to a SET_VAR are: 
		 arr[VAR_NUM]=variable_number
		 arr[VAR_VAL]=new_value
    These variables have arguments as follows:
 	ARRAYD2A:  arr[VARNUM] & 0xffff = ARRAYD2A 
		   arr[VARNUM]>>16 & 0xffff = array_index
		   arr[D2A_INDEX] & 0xffff = dac_group_number
		   arr[D2A_INDEX]>>16 & 0xffff = number in group
	           arr[D2A_VAL] = value to set
	SERVO_CTRL:arr[VARNUM] & 0xffff = SERVO_CTRL
	           arr[VARNUM]>>16 & 0xffff = array_index	   
	           arr[SERVO_NUM] = servo number
		   arr[OBJ_VALUE] = value to set or time to leave on 
	LCD_CTRL:  arr[VARNUM] & 0xffff = LCD_CNTRL
		   arr[VARNUM]>>16 & 0xffff = array_index	   
		   arr[DEVICE_NUM] = device to control
		   arr[ENC_VALUE] = encoder value to go to
	SPAD_MODE: arr[VARNUM] & 0xffff = SPAD_MODE
		   arr[VARNUM]>>16 & 0xffff = array_index	   
		   arr[CARD_NUM] = spad card to control
		   arr[MODE_VALUE] & 0xff = mode value to set 


*/
#define NUM_OF_VARS	32		/* number of variables in the table */
/*
     variables: (currently these are numbered 0..31)
	 These are the variables that can be examined and altered.
	 The high word of the varnum may be used as an array index.  No
	 checking is done to make sure that a legal value is contained
	 here.

	 TRACE_FLAG:  this flag has a bit for each process.  If the bit is set,
		      the process should generate trace information (see
		      debug.h)

	 FRAMES_DEMANDED:  this is the current number of frames to go.
			   warning:  should not be altered!!!
	 FRAMES:  this is the value that is passed to DC as frames_demanded
	 LNR:  this is the number of low_noise reads that is passed to QA
	 TOP_DSP: node number of top DSP in local chain (the one which
		  reports directly to B011)
	 BOTM_DSP: node number of DSP at bottom of local chain
	 REMOTE_PRESENT: TRUE if a remote source of data (as opposed to
			 a local emulator) is present in network.
	 ECHO_ME: scratch variable for testing integrity of communications.
	 STATUS_REPORT: contains a value reporting any error condition known
			to the controller on the addressed DSP.
	 SEP_MODE: variable that turns on sep mode

	 IMAGE_NAME: string floowed by a space.  This is the name for the image
		     to be saved.
	 IMAGE_SAVED: flag (same values as IMAGE_MASK (see shared.h)) which tells
		      which of the images have been saved so far.
	 IMAGE_MASK: mask to and with READY (allows for saving only specific
		     channels).
	 HK_READY_VAL: HK_READY variable contents
	 HEADER_NAME: array of header names (see shared.h)
	 HEADER_COMMENTS: array of header comments
	 HEADER_VALS: array of header values
	 HEADER_TYPES: array of header_vals types

	 CAMERA_POWER: variable to set for toggling camera power
	 LOW_FILTER: variable to toggle electrical signal filter 
	 CAMERA_POWER: variable to toggle detector bias

	 LIGHT_FILTER: current light filter number

	 DETBIAS_n: voltage for vgate[n]
	 EDO_n: ditto
	 ODO_n: ditto
*/

#define SEQ 10
#define ICON 2

#define VAR_NUM		0	/* offset for var number (set,read) */
#define VAR_VAL		1	/* offset for new var value (set) */
#define ISUBVNUM        1
#define ISUBVAR_VAL     2

#define RETURN_VAL	2	/* offset for VAR_READ return */
#define REPLY_TO	1	/* offset for addr of reply_to (read) */
#define FROM		1	/* offset for from number */
#define LENGTH_TO_SEND	2	/* offset for number of ints to send */

#define IMAGE_DONE	0
#define BEGIN_XMIT	1
#define SET_VAR		2
#define READ_VAR	3
#define DEBUG_MSG	4
#define ABORT_MSG	5
#define STOP_MSG	6
#define START_MSG	7
#define PAUSE		8
#define RESUME		9
#define VAR_READ	10
#define READ_HK         11
#define SET_VAR_AK	12
#define AKK		13
#define AKK_FAIL	14
#define KILL_PROC	15
#define EXECUTE_PROC	16
#define CHECK_HK        17
#define MSG_ERROR	18
#define NEXT_MSG	19

/* START VARNUMS */

/* START dspw */
#define TRACE_FLAG	0		/* int */
/* #define FRAMES		1 */		/* int */
#define RECEIVED	2		/* int */
/* #define PROG		3 */		/* int */
#define IMAGE_SIZE	4		/* int */
/* #define LNR		5 */		/* int */
/* #define COADDS		6 */		/* int */
#define MODE		7		/* int */

/* START b011 */
/* #define TRACE_FLAG	0 */		/* int */
#define	H_NAME		1		/* int */
#define H_FORM		2		/* int */
#define H_DATA		3		/* int */
#define H_COMM		4		/* int */
#define HK_DATA		5		/* int */
#define P_FILENAME      6               /* char */
#define P_PATH          7               /* char */
#define P_NUM           8               /* int */
#define P_PIXEL_DIR	9		/* char */

/* START Inst */
/* #define TRACE_FLAG      0  */      	/* int */
#define NUMARRAYS       1               /* int */
#define ECHO_ME         2       	/* int */
#define STATUS_REPORT   3       	/* int */
#define DEACTIVATE      4       	/* int */
#define HKBUF           5  
#define PROTECTION      6       	/* int */
#define SCBREG_VAL      7       	/* int */
#define CPSTATE         8     
#define ARRAYD2A        9     
#define SERVO_CTRL     10 
#define LCD_CTRL       11  
#define A2DFREEZE      12 
#define SPAD_MODE      13               /* int [] */
#define WHEEL_POS      14	        /* int [] */

/* START seq */
#ifdef SEQUENCER
/* #define	TRACE_FLAG	0 */		/* int */

#define	PROG		1 		/* int */
#define FRAMES		2		/* int */
#define	CTRL_REG	4		/* int */
#define LNR		5		/* int */
#define COADDS		6 		/* int */
#define INT_S		7		/* int */
#define FINT_S          8               /**/
#define SPAD_FILTER     9               /**/
#define QUADRANT        11
#define N_DAVG          12
#define ROI_SIZE        13
#define VAR1            14
#define VAR2            15
#define VAR3            16
#define VAR4            17
#define PTR		29		/* int */
#define MEMORY		30		/* int */

#endif

/* END VARNUMS */

#define NODE(x)		((x & 0xff000000)>>24)	/* get node number */
#define TO_NODE(x)	((x)<<24)		/* for setting node # */
#define LENGTH(x)	((x & 0xff0000)>>16)	/* get length */
#define OF_LENGTH(x)	((x)<<16)		/* set length */
#define MESSAGE(x)	(x & 0xff)		/* get message from header */
#define FROM_NODE(x)	((x)<<8)		/* set from field */
#define FROM_WHERE(x)	((x)>>8 & 0xff)		/* get from field */

#define ABOVE(x)	(((x == 3) || (x == 4)) ? (1) : (x-2))
				/* node # of above node */
#define BELOW(x)	(x+2)			/* node # of node below */


#define VAR_NUMVAL(x)	(*(x + VAR_NUM) & 0xffff)     /* get varnum form buf */
#define ARR_INDEX(x)	(*(x + VAR_NUM)>>16 & 0xffff) /* get array index from buf */
#define D2A_GROUP(x)    (*(x + ISUBVNUM) & 0xffff) /* get d2a group from buf */
#define D2A_INDEX(x)    (*(x + ISUBVNUM)>>16 & 0xffff) /* get d2a index from buf */
#define SERVO_NUM(x)    (*(x + ISUBVNUM)>>16 & 0xffff) /* get servo num from buf */
#define SERVO_SPCMD(x)  (*(x + ISUBVNUM) & 0xffff)  /* get servo cmnd from buf */

#define DEVICE_NUM(x)   (*(x + ISUBVNUM) & 0xffff)      /* get device num from buf */
#define OBJECT_NUM(x)   (*(x + ISUBVNUM) & 0xffff)      /* get device num from buf */
#define CARD_NUM(x)	(*(x + ISUBVNUM) & 0xffff)      /* get spad num from buf */

/* get d2a value from buf */
#define D2A_VAL(x)	(*((float *)(x + ISUBVAR_VAL )))  
#define DEV_VAL(x)	(*(x + ISUBVAR_VAL) & 0xffff)  /* get device val from buf */
#if !defined(IRS)
#define OBJ_VAL(x)	(*(x + ISUBVAR_VAL) & 0xffff)  /* get val from buf */
#define ENC_VAL(x)	(*(x + ISUBVAR_VAL) & 0xffff)  /* get val from buf */
#else
#define OBJ_VAL(x)	(*(x + ISUBVAR_VAL) & 0xFFFFFF)  /* get val from buf */
#define ENC_VAL(x)	(*(x + ISUBVAR_VAL) & 0xFFFFFF)  /* get val from buf */
#endif
#define MODE_VAL(x)	(*(x + ISUBVAR_VAL) & 0xffff)  /* get device val from buf */

/*
     example:
	 if (NODE(header) == _node_number)
	ChanIn(chan, buf, LENGTH(header))
	header |= OF_LENGTH(10);
	header |= TO_NODE(0);
	switch (MESSAGE(header))
	TO_NODE(ABOVE(_node_number));
	TO_NODE(BELOW(_NODE_NUMBER));
	*(var_ptr[VAR_NUMVAL(buf)] + ARR_INDEX(buf)) = 10;
*/


#define MAXBUFLEN	256		/* maximum bufferlength (# of ints) */

#define TIMEOUT		1000		/* timeout length in ms */




	
