/* configuration variables for ARRAY CONTROL. */
/******************************************************************************
 * Program:	ARRAY CONTROL software
 * File:        cnfg_NAAC.c     
 * Purpose:     routine to set up configuration of instrument controller  
 * Author:      Nick C Buchholz
 *
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *      06-Mar-1996 - created orginal file - ncb
 *
 *****************************************************************************/
/* descriptors for arrays */
struct inst_cfg inst =
{
    1,					/* number of arrays */
    NUMSERVOS,				/* number of servos */
    32,					/* number of A2D's */
    {					/*struct ar_desc array[] =  */
    {     
	32,				/* analog outputs per array */
	1024,				/* number of rows in array */
	1024,				/* number of columns in array */
	NUM_D2A,			/* number of D 2 A converters  */
	ENBL_DET_BIAS,			/* SCB bit which activates array */
	7,				/* dettemp hkchan for activate check */
	1.000,				/* min activation readout voltage  */
	/* initialize D2A structures */
        {   /* numingrp  BAddr,    Cen#,    hkchnl,  startv,   hmax,   hmin */ 
            {    4,      -1,   C_DODAC_Ld,  25,     0.0,     5.0,    0.0,
	         /* normv,  maximum, minimum,   bits/v,    method */
		     0.0,   5.0,     0.0,       819.,     SET0  },
            /*NU NuminGrp  BAddr,   Cen#,   hkchnl,  startv,   hmax,   hmin */ 
	    {    4,      -1,   C_DODAC_Ld, 113,     0.0,     5.0,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		     0.0,   5.0,     0.0,       819.,     SET0  },
/* Vdet     NuminGrp  BAddr,   Cen#,     hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,  81,      0.0,     1.35,    0.0, 
	     /* normv,  maximum, minimum,   bits/v,    method */
		1.0,      1.35,     0.0,     3681.1,      SET12  },
/* Vset     NuminGrp  BAddr,   Cen#,     hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,  76,      0.0,     4.64,    0.0, 
	     /* normv,  maximum, minimum,   bits/v,    method */
		1.0,      4.64,     0.0,     880.91,      SET11  },
 /* VgCL_1   NuminGrp  BAddr,   Cen#,      hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,   64,       0.0,     5.84,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		    .5,      5.84,      0.0,     700.00,      SET11  },
 /* VgCL_2  NuminGrp BAddr,   Cen#,      hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,   75,       0.0,     5.85,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		    1.0,      5.85,     0.0,     699.13,      SET11  },
 /* VdCl_1   NuminGrp BAddr,   Cen#,      hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,   61,       0.0,     4.63,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		    .5,      4.63,      0.0,    882.33,      SET11  },
 /* VdCl_2   NuminGrp BAddr,   Cen#,      hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,   78,       0.0,     4.63,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		    .5,      4.63,      0.0,    882.89,      SET11  },
/* NOT USED   NuminGrp BAddr,   Cen#,      hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,   14,       0.0,     2.5,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		    .0,      2.43,      0.0,    1638.,      SET11  },
/* VOff1     NuminGrp BAddr,   Cen#,      hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,   14,       0.0,     2.5,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		    .0,      2.43,      0.0,    1681.7,      SET11  },
/* VOff2     NuminGrp BAddr,   Cen#,      hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,   15,       0.0,     2.5,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		    .0,      2.43,      0.0,    1679.6,      SET11  },
/* NOT USED   NuminGrp BAddr,   Cen#,      hkchnl,  startv,   hmax,   hmin */ 
	    {   1,       3,   C_PRVolts_Ld,   17,       0.0,     2.5,    0.0, 
		 /* normv,  maximum, minimum,   bits/v,    method */
		    0.,      2.5,      0.0,    1638.,      SET11  },
	},
        {	/* D2A text names */
	    "NA", "NA", "Detector Bias", "VSet",
	    "VggCl_1", "VggCl_2", "VddCl_1", "VddCl_2", "NA", "VOff_1", 
	    "VOff_2", "NA",
	},
	{				/* hk chan number of rail voltages */
	    18,			/* N_1Fast_Upper   */
	    22,			/* N_2Fast_Upper   */
	    27,			/* N_FastS_Upper   */
	    19,			/* N_1Fast_Lower   */
	    23,			/* N_2Fast_Lower   */
	    28,			/* N_FastS_Lower   */
	    20,			/* N_1Slow_Upper   */
	    25,			/* N_2Slow_Upper   */
	    29,			/* N_SlowS_Upper   */
	    21,			/* N_1Slow_Lower   */
	    26,			/* N_2Slow_Lower   */
	    30,			/* N_SlowS_Lower   */
	    31,			/* N_Reset_Upper   */
	    32,			/* N_Reset_Lower   */
	    33,			/* N_VddOut_Upper  */
	    34,			/* N_VddOut_Lower  */
	    35,			/* N_VggUC_Upper   */
	    36			/* N_VggUC_Lower   */
	},
	{
	    7.270,			/* N_1Fast_Upper  */
	    7.270,			/* N_2Fast_Upper  */
	    4.973,			/* N_FastS_Upper  */
	    -0.010,			/* N_1Fast_Lower  */
	    -0.010,			/* N_2Fast_Lower  */
	    -0.010,			/* N_FastS_Lower  */
	    7.270,			/* N_1Slow_Upper  */
	    7.270,			/* N_2Slow_Upper  */
	    4.973,			/* N_SlowS_Upper  */
	    -0.010,			/* N_1Slow_Lower  */
	    -0.010,			/* N_2Slow_Lower  */
	    -0.010,			/* N_SlowS_Lower  */
	    7.220,			/* N_Reset_Upper  */
	    -0.001,			/* N_Reset_Lower  */
	    5.024,			/* N_VddOut_Upper */
	    0.003,			/* N_VddOut_Lower */
	    4.973,			/* N_VggUC_Upper  */
	    -0.001,			/* N_VggUC_Lower  */
	},
        {				/* rail name gotten from SUN  */
	    "1Fast Upper", "2Fast Upper", "Fast Sync Upper",
	    "1Fast Lower", "2Fast Lower", "Fast Sync Lower",
	    "1Slow Upper", "2Slow Upper", "Slow Sync Upper",
	    "1Slow Lower", "2Slow Lower", "Slow Sync Lower",
	    "Reset Upper", "Reset Lower",
	    "VddOut Upper", "VddOut Lower",
	    "VggUC Upper", "VggUC Lower"
	 },
		/* spad descriptors */
	 { /* board address,   initial mode */
	     {      0x00,		0        },
	     {      0x00,		0        },
	     {      0x00,		0        },
	     {      0x00,		0        },
	     {      0x01,		0        },
	     {      0x01,		0        },
	     {      0x01,		0        },
	     {      0x01,		0        },
	 },
    }					/* end of first array */
    },					/* end of array descriptors */
    { /* motor descriptors */
/* 1/2 wave plate -mjrdev mnrdev    data_scb        reset_scb      type */
                {   13,      2,   SEL_MOTOR0_DATA, ENBL_MOTOR_RST, T_MOTOR_ENC,
                  /*  cd_delay  dlyovhd  minval maxval dlypstp  curval */
		        20,      700,     0,   8191,     2,      0
		},

    }
};

char ucode_def[MAXLINE];		/* file name of default ucode to use */
char ucode_cur[MAXLINE];		/* file name of current ucode  */

float  array_d2a[NUM_D2A];		/* values for d2a conversions */
int data_order = RDD;			/* order data arrives in */
int proc_def = CA_BMS;			/* processing description */

/* index of channel descriptor structure for each housekeeping channel. These
   descriptors hold the coefficents of the linear adu to volt conversion used
   by the housekeeping channels */ 
int chdesc [] = 
{
    4, 4, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 
    0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 
    0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 4, 4, 0, 4, 4, 0, 
    3, 4, 0, 3, 4, 0, 7, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 4, 4, 
    0, 4, 4, 4, 4, 4, 4, 4, 2, 2, 4, 2, 2, 4, 4, 4, 0, 0, 0, 0, 
    0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 4, 2, 1, 1, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 6, 
    6, 6, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* array of unique hk channel descriptors */
struct hkc_desc hkcdesc[] = 
{
    {   0, 1.0000000, 0.0, 0.0    },	 /* desc 0 - raw readout */
    {   0, 0.0021365, 0.0, 0.0    },     /* desc 1 - 2.1365 mV/ADU */
    {   0, 0.0010683, 0.0, 0.0    },     /* desc 2 - 1.0683 mV/ADU */
    {   0, 0.0006677, 0.0, 0.0    },     /* desc 3 - 0.6677 mV/ADU */
    {   0, 0.0005341, 0.0, 0.0    },     /* desc 4 - 0.5341 mV/ADU */
    {   0,-0.0534100, -273.0, 0.0 },     /* desc 5 - 0.05341 K/ADU */
    {   0, 0.0013297, 0.0, 0.0    },     /* desc 6 - 1.3297 mV/ADU */
    {   0, 0.005341, 0.0, 0.0      },    /* desc 7 - 0.005341 uA/ADU */
};

/* tolerated difference between hk_norms and observed hk_buf values */
float tolerance = 100.0E-3;



