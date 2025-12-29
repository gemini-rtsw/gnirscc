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
 ******************************************************************************/

/*
    protocol specifications:

	a packet shall consist of the following:
	    int header
	    int arr[0..length-1]

	    this shall be transmitted sequentially starting with address
	    and ending with arr[length-1].  length shall be larger
	    or equal to zero.

	    the addr is composed of a one byte address (node_number)
	    and a one byte length.  The masks below will decode these.

	messages shall be encoded in the header and accesed:
	    MESSAGE(header);

	    they should be written as header | msg.

	messages shall be passed:
	    by a process which will determine the recipient.  if
	    the recipient is equal to the _node_number, then
	    that node is the recipient.  Otherwise, pass the entire
	    packet along in the proper direction.

*/

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
	notes:    some variables have multiple instances one for each array or 
	          several for an instrument. additional information is needed in
		  the message as follows
		  DAC's :  arr[VARNUM] & 0xffff = ARRAYD2A 
		           arr[VARNUM]>>16 & 0xffff = array_index
			   arr[D2A_INDEX] = dac_number
			   arr[D2A_VAL] = value to set
		  FILTERS: arr[VARNUM] & 0xffff = FILTERS
		           arr[VARNUM]>>16 & 0xffff = array_index	   
			   arr[DEVICE_NUM] = wheel or slider number
			   arr[OBJECT_NUM] = filter on wheel to goto 
			   arr[OBJ_VALUE] = final encoder reading (optional)
		  SERVOS:   arr[VARNUM] & 0xffff = SERVO_CNTRL
		           arr[VARNUM]>>16 & 0xffff = array_index	   
			   arr[DEVICE_NUM] = device to control
			   arr[ENC_VALUE] = 


    SET_VAR_AK:
	arguments: same as SET_VAR
	purpose:   same as SET_VAR.  This should only be sent out to the
		   sqiid controller, all other transputers will ignore it.  
		   The b011 will wait until it gets a reply (AKK) from sqidc &
		   relay this to the sun.
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


#define VAR_NUM		0		/* offset for var number (set,read) */
#define VAR_VAL		1		/* offset for new var value (set) */
#define RETURN_VAL	2		/* offset for VAR_READ return */
#define REPLY_TO	1		/* offset for addr of reply_to (read) */
#define FROM		1		/* offset for from number */
#define LENGTH_TO_SEND	2		/* offset for number of ints to send */
#define D2A_INDEX	1               /* offset for d2a device number */  
#define D2A_VAL		2               /* offset for new D2A value  */         
#define DEVICE_NUM	1               /* offset for Instrument device number */  
#define	OBJECT_NUM	2               /* offset for object on device number  */ 
#define	OBJ_VALUE	3               /* offset for encoder value for device */  
#define	ENC_VALUE	2               /* offset for encoder value for servo */  
#define MODE_VALUE      2		/* offset for spad mode value */


#define IMAGE_DONE      0
#define BEGIN_XMIT      1
#define SET_VAR         2
#define READ_VAR        3
#define DEBUG_MSG       4
#define ABORT_MSG       5
#define STOP_MSG        6
#define START_MSG       7
#define PAUSE           8
#define RESUME          9
#define VAR_READ        10
#define READ_HK         11
#define SET_VAR_AK      12
#define AKK             13
#define AKK_FAIL        14
#define KILL_PROC       15
#define EXECUTE_PROC    16
#define CHECK_HK        17
#define MSG_ERROR       18

/* START VARNUMS */

/* START dspw */
#ifdef DSP_WORKER
#define FRAMES_DEMANDED	0		/* int */
#define FRAMES		1		/* int */
#define LNR		2		/* int */
#define TOP_DSP         3		/* int */
#define BOTM_DSP        4		/* int */
#define REMOTE_PRESENT  5		/* int */
#define ECHO_ME         6		/* int */
#define STATUS_REPORT   7		/* int */
#define BIAS		9		/* int */
#define SIGNAL		10		/* int */
#define SEP_MODE	23		/* int */
#define PTR		29		/* int */
#define MEMORY		30		/* int */
#define	TRACE_FLAG	31		/* int */
#endif

/* START b011 */
#ifdef B016_CODE
#define CHAIN_LENGTH    3               /* int */
#define SEQS_PRESENT    4               /* int */
#define SQIID_PRESENT   5               /* int */
#define ECHO_ME         6		/* int */
#define WHAT_STAGE      7               /* int */
#define IMAGE_NAME	8		/* str */
#define IMAGE_SAVED	9		/* int */
#define IMAGE_MASK	10		/* int */
#define HK_READY_VAL	11		/* int */
#define HK_SCREEN_NUM	12		/* int */
#define HK_ARR		18		/* int */
#define HEADER_NAMES	13		/* str */
#define HEADER_TYPES	14		/* int */
#define HEADER_COMMENTS	15		/* str */
#define HEADER_VALS	16		/* int */
#define NUM_OF_HEADERS	17		/* int */
#define P_ZSCALE	19		/* fp */
#define P_MOVIE		20		/* int */
#define P_FRAMES	22		/* int */
#define SEP_MODE	23		/* int */
#define USE_HK		24		/* int */
#define F_TIMES		25		/* int */
#define P_COADDS	26		/* int */
#define P_PATH		27		/* str */
#define P_PIC_NUMS	28		/* int */
#define PTR		29		/* int */
#define MEMORY		30		/* int */
#define	TRACE_FLAG	31		/* int */
#endif

/* START Instrument */
#ifdef INST_CTRL
#define	TRACE_FLAG	0		/* int */
#define NUMARRAYS	1		/* int */
#define ECHO_ME		2		/* int */
#define STATUS_REPORT   3		/* int */
#define DEACTIVATE	4		/* int */
#define HKBUF           5		/* float [][]  */
#define PROTECTION	6		/* int */
#define SCBREG_VAL	7		/* int */
#define CPSTATE		8		/* int */
#define ARRAYD2A	9		/* int []  place holder */
#define FILTERS		10		/* int [][] place holder */
#define SERVO_CNTRL	11		/* int [] place holder */

#endif

/* START seq */
#ifdef SEQUENCER
#define FRAMES		1		/* int */
#define LNR		2		/* int */
#define ECHO_ME         6		/* int */
#define STATUS_REPORT   7		/* int */
#define	CTRL_REG	3		/* int */
#define INT_MS		4		/* int */
#define INT_S		5		/* int */
#define	PROG		8		/* int */
#define JHK_COADDS	9
#define PTR		29		/* int */
#define MEMORY		30		/* int */
#define	TRACE_FLAG	31		/* int */
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

#define VAR_NUMVAL(x)	(*(x + VAR_NUM) & 0xffff)
				/* get varnum */
#define ARR_INDEX(x)	(*(x + VAR_NUM)>>16 & 0xffff)
				/* get array index */

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




