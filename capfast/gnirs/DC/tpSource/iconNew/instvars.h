/******************************************************************************
 * Program:	INST CONTROL Processes
 * File:     	instvars.h
 * Purpose:  	Define global variables for tasks in INST CONTROL node.
 * Author:   	Dick H. Fredericksen
 *		revised and generalized by Nick C. Buchholz
 * Copyright:   Aura Inc.  All rights reserved.
 * History:  
 *	22-Apr-1991 - created file - dhf
 *	12-Nov-1991 - revised file to eliminate sqiid particulars
 ****************************************************************************/

#ifndef NOINSTVARS
#define NOINSTVARS
char iconSrcID[] = { PWD };
int my_unit = INST;         /* This is the INST CONTROL node */

/* channel stuff used by communications in instrument controller */
Channel	*Reader_to_Writer,	/* Channel between reader and writer */
        *Writer_to_Control,	/* Channel to control on this node */
        *Control_to_Reader,	/* Channel to reader for outgoing messages */
        *Cntrl_to_HK_A2D,	/* Channel to housekeeping reader */
	*Cntrl_to_Pic_Ctrl,	/* Channel to picture controller */
        *Pic_Ctrl_to_Reader,	/* Channel to reader */
	*HK_A2D_to_Reader; 


/* standard buffer for diagnostic messages */
char stdbuf[MAXLINE];

/* locks for hardware control registers and data bus */
Semaphore SCBreg_lock = SEMAPHOREINIT;	/* SCB register lock */
Semaphore dbus_lock = SEMAPHOREINIT;	/* data bus lock */
Semaphore var_lock = SEMAPHOREINIT;	/* variable space lock */
Semaphore idxlock = SEMAPHOREINIT;

/* HARDWARE VARIABLES */
int *SCBreg_ptr = SCBREG_ADDR;		/* addr of SCB register */
/* maintain record of control register value (it's a write-only register) */
int SCBreg_val  = (unsigned) 0x0000;	/* starting value of register */
int spad_mode[NUM_ARRAYS][NUM_A2D];     /* spad card mode flags */

/* CONFIGURATION VARIABLES  */
int num_arrays;			/* number of arrays in instrument */
int protection = 0;		/* level of user protection */
int deactivate;			/* true if will deactivate on shutdown */
int cp_state;			/* last setting for camera power */
int a2d_freeze = 0;		/* squelch reading of hkdata  when true */
int data_simul = 0;		/* setting of data simulation in DQ tranfer*/

/* Buffer space for housekeeping variables managed by INST CONTROL */
/* housekeeping channels viewed as ints */
int A2D_array[HK_CHANS];

/* converted housekeeping values as floats */
float A2D_vals[HK_CHANS];

/* standard values for housekeeping variables */
float hk_norms[HK_CHANS];

/* buffers for hk data */
float hk_buf[HK_BUFS][HK_CHANS];
int hkbufix = -1;			/* index of current hkbuf */

/* dummy variable for testing integrity of communications
   (set and read this from another transputer if unsure) */
int echo_me = 913442915;
float echo_float = 3.5;
int status_report = 0;		/* status report value */
int debug = 0;			/* debug flag */
int trace_flag=0;			/* true if trace messages are to be sent */
char debug_line[DEBUG_LINES][MAXLINE];	/* debug buffer lines for sprintf  */

/* names of housekeeping variables: */
char *hk_names[HK_CHANS];

int filter_number[NUM_WHEELS][MAX_FILTS];

int srvo_control[MAXSERVOS][MAX_DVARS];
float wheel_pos[MAXSERVOS];  
#else

extern int my_unit;

extern Channel	*Reader_to_Writer, *Writer_to_Control, *Control_to_Reader,
    *Cntrl_to_HK_A2D, *HK_A2D_to_Reader, *Cntrl_to_Pic_Ctrl, 
    *Pic_Ctrl_to_Reader;

extern char stdbuf[];

/* start of configuration variables */
extern Semaphore SCBreg_lock;
extern Semaphore dbus_lock;	/* data bus lock */
extern Semaphore var_lock;	/* variable space lock */
extern Semaphore idxlock;

extern int *SCBreg_ptr;
extern int SCBreg_val;
extern int spad_mode[NUM_ARRAYS][NUM_A2D];
extern int num_arrays;			/* number of arrays in instrument */
extern int protection;			/* level of user protection */
extern int deactivate;			/* true if will deactivate on shutdown */
extern int cp_state;			/* last setting for camera power */
extern int a2d_freeze;
extern int data_simul;		/* setting of data simulation in DQ tranfer*/

extern int *poweroff;
extern int *scb_reg;
/* true if trace messages are to be sent */
extern int A2D_array[];
extern float A2D_vals[];
extern float hk_norms[];
extern float hk_buf[HK_BUFS][HK_CHANS];
extern int hkbufix;

extern int echo_me, status_report;
extern float echo_float;
/* extern int debug; */
extern int trace_flag;
extern char debug_line[DEBUG_LINES][MAXLINE];

extern char *hk_names[];

extern int filter_number[NUM_WHEELS][MAX_FILTS];

extern int srvo_control[MAXSERVOS][MAX_DVARS];
extern float wheel_pos[MAXSERVOS];

#endif

/* configuration defined variables */
extern char ucode_def[];
extern char ucode_cur[];

extern int data_order;
extern int proc_def;

extern int num_dev;

extern int chdesc[];
extern struct hkc_desc hkcdesc[]; 

extern float tolerance;
/* end of configuration variables */

/* variables created in inst_cmnds.c */
extern command_form command[];
extern num_command;
extern command_form setv_commands[];
extern command_form readv_commands[];
extern Channel *chan_list[];

extern float array_d2a[];

/* pointers to protocol-designated variables defined in inst_cmnds.c*/
extern int *var_ptr[NUM_OF_VARS];
extern struct inst_cfg inst;		/* instrument descriptor variable */

extern int bad_msg();
extern int abort_img();
extern int start_picture();
extern int read_hk();
extern int kill_proc(); 
extern int execute_proc(); 
extern int check_hk();

extern int set_SCBreg_var();
extern int set_camera_pwr();
extern int set_d2a_var();
extern int set_servo_var();
extern int setDataSimul();
extern int no_set();

extern int read_SCBreg_var();
extern int read_camera_pwr();
extern int read_d2a_var();
extern int read_servo_var();
extern int readDataSimul();
extern int read_onehk();
extern int no_read();

extern int lcd_ctrl();

/* array controler routines */
int arr_d2a_set(int anum, int d2agrp, int d2aidx, float value, char stdbuf[]);

/* hardware routines */
 int setd2a_12b(int mjr_num, int cen_num, int bval,  char stdbuf[]);
int set_spad_mode(int header, int *buf);
int read_spad_mode(int header, int *buf);
int read_spmode(int baddr, int *value);
int write_spad_mode( int baddr, int value);
float ReadOneHK(int chan, float *ftpr);
int servo_read_data(int srvonum, int *data);
int servo_write_cmnd(struct srvo_desc *sr, int srvonum, int cmnd, int *data);











