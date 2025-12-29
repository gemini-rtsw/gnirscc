/******************************************************************************
 * Program:	Instrument Configuration Software
 * File:        config_st.h
 * Purpose:     describe the configuration structure for all instruments.  This 
 *		defines all the configuration option needed for an instrument.
 *		(maybe).
 *
 * Author:      Nick C Buchholz
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	07-Nov-1991 - created file - ncb
 *
 *****************************************************************************/

#ifndef CONFIG_DEFS
#define CONFIG_DEFS

/******************************************************************************
 * Standard defines which can change from one array type to the next 
 *****************************************************************************/
#define NUM_ARRAYS	1		/* number of arrays in instrument */
#define NUM_D2A		12		/* number of D2A channels per array */
#define NUM_A2D         32		/* number of A2D channels per array */

#include "DACmethods.h"

#define SUBVAL1		2		/* Vdduc D2A hk chan index */

#define HK_CHANS	256		/* number of house keeping channels */
#define HK_BUFS	 	2		/* number of HK buffers */

#define MAXSERVOS	1		/* maxm number of servos etc allowed */

#define MAX_DVARS	10

/* device related stuff */
#define NUM_WHEELS	MAXSERVOS	/* number of filter wheels in system */
#define MAX_FILTS       1		/* max number of filters per wheel */
#define NUMSERVOS       0		/* mnumber actually in instrument */

/* rail voltages are stored in array[n].rails[] indexed by #defined constants */
#define N_1Fast_Upper   	0
#define N_2Fast_Upper   	1
#define N_FastS_Upper   	2
#define N_1Fast_Lower   	3
#define N_2Fast_Lower   	4
#define N_FastS_Lower   	5
#define N_1Slow_Upper   	6
#define N_2Slow_Upper   	7
#define N_SlowS_Upper   	8
#define N_1Slow_Lower   	9
#define N_2Slow_Lower   	10
#define N_SlowS_Lower   	11
#define N_Reset_Upper   	12
#define N_Reset_Lower   	13
#define N_VddOut_Upper  	14
#define N_VddOut_Lower  	15
#define N_VggUC_Upper   	16
#define N_VggUC_Lower   	17

#define NUM_RAILS	        18	/* number of constant rail voltages */

/* register bit layouts for instrument controller,  
   lower 16 bits are positive logic upper 16 are negative logic, some are named */
#define SCBREG_ADDR	(int *) 0xA0000000
#define BIT_0		0x0001
#define BIT_1		0x0002
#define BIT_2		0x0004
#define BIT_3		0x0008
#define BIT_4		0x0010
#define BIT_5		0x0020
#define BIT_6		0x0040
#define BIT_7		0x0080
#define BIT_8   	0x0100
#define BIT_9   	0x0200
#define BIT_10  	0x0400
#define BIT_11		0x0800
#define BIT_12		0x1000
#define BIT_13		0x2000
#define BIT_14		0x4000
#define BIT_15		0x8000
#define BIT_16		0x00010000
#define BIT_17		0x00020000
#define BIT_18		0x00040000
#define BIT_19		0x00080000
#define BIT_20		0x00100000
#define BIT_21		0x00200000
#define BIT_22		0x00400000
#define BIT_23		0x00800000
#define BIT_24   	0x01000000
#define BIT_25   	0x02000000
#define BIT_26  	0x04000000
#define BIT_27		0x08000000
#define BIT_28		0x10000000
#define BIT_29		0x20000000
#define BIT_30		0x40000000
#define BIT_31		0x80000000
#define ALL		0xFFFFffff
#define NONE		0x00000000

/* Control register bit names */ 

#define SEL_MOTOR0_DATA BIT_2       /* Changed to SCB#2, unused here */
#define SEL_MOTOR1_DATA BIT_2
#define SEL_TCL_DATA    BIT_2
#define ENBL_MOTOR_RST	BIT_2

#define SEL_HK_DATA	BIT_2
#define SEL_HK_ADDR	BIT_10
#define SEL_DBUS        BIT_10

#define SEL_SPAD_DTA	BIT_2

#define ENBL_DET_BIAS	BIT_15   /* Changed from 30 to 15 */

#define DATA_BUS	(SEL_MOTOR0_DATA | SEL_MOTOR1_DATA | SEL_SPAD_DTA | \
			 SEL_TCL_DATA | SEL_HK_DATA | SEL_HK_ADDR)  

#define HK_PA_C0        BIT_0
#define HK_PA_C1        BIT_1
#define HK_ADC_C1       BIT_3
#define HK_ADC_C0       BIT_4
#define RX_ADC          BIT_6
#define RX_PA           BIT_7

#define WR_DBUS_ADDR	(int *)0xB0000000 /* write to the databus DMAO */
#define HKC_REG_ADDR	(int *)0xB0000000 /* alias for WR_DBUS_ADDR */
#define DSTRB_ADDR	(int *)0xD0000000 /* send out a data strobe */
#define HK_A2D_CTC	(int *)0xD0000000 /* alias for DSTRB_ADDR */
#define RD_DBUS_ADDR 	(int *)0xC0000000 /* read from the data bus DMAI */

/* /cen bit addresses */
#define CEN_BASE (int *)0x90000000
 /* /cen bit names */
#define C_Brd_Addr_Ld	2       /* Changed from 0 to 2 */
#define C_Spad_Dac_Ld	1       /* Unused gor GC */
#define C_Spad_Mode_Ld	2       /* Unused gor GC */
#define C_DODAC_Ld	4       /* Changed from 2 to 4 */
#define C_PRDAC_Sel     3	/* Left at 3 */
#define C_DBias_Ld	3       /* Unused for GC */
#define C_PRVolts_Ld	4       /* Unused for GC */
#define C_VOff_Ld	4       /* Unused for GC */
#define C_VggHi_Ld	5       /* Unused for GC */
#define C_VggLo_Ld	6       /* Unused for GC */
#define C_CEN_7		7       /* Unused for GC */
#define C_CEN_8		8       /* Unused for GC */
#define C_CEN_9		9       /* Unused for GC */
#define C_CEN_10	10       /* Unused for GC */
#define C_CEN_11	11       /* Unused for GC */
#define C_CEN_12	12       /* Unused for GC */
#define C_CEN_13	13       /* Unused for GC */
#define C_CEN_14	14       /* Unused for GC */
#define C_Temp_Ctrl	15       /* Unused for GC */

#define SIMU            7
#define XM1_BEN         9
#define XM2_BEN         10
#define HK_PA_BEN       11
#define HK_ADC_BEN      12

/******************************************************************************
 *****************************************************************************/

#define MAX_I_VARS        6	/* max internal vars in LCD controllers */

/* preliminary methods for getting and processing data */ 
/* values for proc_def */
#define RDD		10		/* Reset,  bias Data, signal Data */
#define SRB		20		/* (Signal, Reset, Bias) per pixel */
#define LNRS		30		/* Reset, n * bias Data, n * signal Data */
#define LNCF		40		/* Reset, bias Data, n * (integ, sig Data) */
#define RD		50		/* Reset, Int, signal Read */

/* values for data_desc */
#define SMB		10		/* signal minus bias */
#define BMS		20		/* bias minus signal */
#define CA_SMB		30		/* coadd bias & signal then S_M_B and avg  */
#define CA_BMS	        40		/* coadd bias & signal then B_M_S and avg  */
#define LN_BMS		50		/* avg n biases & n signals then BMS */
#define LN_SMB		60		/* avg n biases & n signals then SMB */
#define CA_RD		70		/* avg n signals */

/* structures used in configurations */

/* structure for d2a addr, channel, voltage defaults and limits */
struct d2a_cfg {
    int num_in_grp;		/* number of d2a's with this Cen */
    int mjr_num;			/* lowest Board address to use */
    int cen_num;			/* clock enable line to use */
    int d2a_chan;			/* lowest hk chan num of d2a readback */
    float startv;			/* value to set at startup */
    float hmax, hmin;			/* max and min possible values */
    float norm;				/* value to use when reading out */
    float max;				/* maximum allowed value */
    float min;				/* minimum allowed value */
    float bits_per_v;			/* converts volts to  bits */
    int set_mthd;			/* number of set routine */
};

/* structure for d2a addr, channel, voltage defaults and limits */
struct a2d_cfg {
    int mjr_num;			/* Board address to use */
    int spad_mode;			/* initial mode register copy */
};

struct ar_desc {
    int num_outputs,			/* number of outputs in array */
        rows, cols,			/* number of rows and cols in array */
        num_d2a,			/* number of D to A channels */
        activ_bit,			/* SCB used to activate array */
        activ_tch;			/* channel for activation check */
    float activ_lvl;			/* voltage at which we can activate */
    struct d2a_cfg voltage[NUM_D2A];	/* voltage value structure for D2A's */
    char *D2A_names[NUM_D2A];		/* names of d2a converters */
    int rail_chan[NUM_RAILS];		/* hk chan num of the rail voltage */
    float rails[NUM_RAILS];		/* normal values for rail voltages */
    char *rail_names[NUM_RAILS];	/* names of the rail voltages */
    struct a2d_cfg spad[NUM_A2D];	/* config reg copies for a2d boards */
};
/* rail voltages are stored in the array rails indexed by #defined constants */

struct hkc_desc {
    int cnvrt_mthd;			/* which conversion routine to use */
    float slope, intercept;		/* values of slope & intercept */
    float norm;				/* normal value for channel */
};

/* structure to describe things on wheels ie filters, lvf's etc. */
struct lcd_obj {
    char *objname;			/* text object name */
    int start_pos;			/* starting postion of object */
    int end_pos;			/* ending position of object
					   contains -1 if object is discrete */
    int (*cnvrt)();			/* routine to convert hlv to pos */
    int vars[MAX_DVARS];		/* internal variables for object */
    struct lcd_obj *prev, *next;	/* pointers for object list */
};

/* structure to decsribe wheels, sliders etc. */
struct lcd_desc {
    int mtr_num;			/* motor number for this device */
    int num_obs;			/* number of objects on device */
    int (*cntrl)();			/* pointer to the control function */
    int vars[MAX_I_VARS];		/* pointer to internal variables  */
    struct lcd_obj  *head, *tail;	/* list of objects on the device */
};

struct motor {
    int minval, maxval;			/* max and min values */
    int dlypstp;			/* time to do one bit value change */
    int curval;				/* current value */
};

struct solenoid {
    int enrg_time;			/* time to energize the solenoid */
};

struct sswitch {
    int sw_time;			/* time to do one positon change */
    int positions;			/* number of positions */
    int curpos;
};

struct srvo_desc {			/* hardware descriptor for motors  */
    int mjrdev, mnrdev;			/* major & minor device nums */
    int data_scb;			/* mask for SCB to use for data */
    int reset_scb;			/* mask for SCB to use to reset */
    int type;				/* type of servo motor=0, solenoid=1 */
    int cd_delay;			/* command to data delay time */
    int dlyovhd;			/* overhead in movement routine */
    union {				/* union of servo type structures */
	struct motor motor;
	struct solenoid solenoid;
	struct sswitch sswitch;
    } v;
};

/* structure for instrument description */
struct inst_cfg {
    int num_arrays;		/* number of arrays in instrument */
    int maxservos;		/* number of controllable devices in inst */
    int numspads;		/* number of spad boards in instrument */
    struct ar_desc array[NUM_ARRAYS];   /* space for array descriptors */
    struct srvo_desc servos[MAXSERVOS]; /* space for servo descriptors */ 
#if 0
    struct lcd_desc lcds[NUM_LCDS];	/* space for lcd descriptor */
#endif
};

typedef void (*command_form)();

#endif




