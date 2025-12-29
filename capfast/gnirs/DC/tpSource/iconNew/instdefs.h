/******************************************************************************
 * Program:	header file
 * File:	instdefs.h
 * Purpose:	to provide constant, type, and structure definitions which
 *		are common to all instruments for INSTRUMENT CONTROL
 * Author:	Dick Fredericksen
 *		
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		04-22-91 created	      dhf
 *		12-Nov-91 - revised by Nick C Buchholz
 *****************************************************************************/
#ifndef CONFIG_DEFS
#include "config_st.h"
#endif

#define TO_SUN           LINK0OUT       /* via DSP CONTROL and B011 */
#define FROM_SUN         LINK0IN        /* via B011 and DSP CONTROL */
#define TO_DSP_CONTROL   LINK0OUT       /* 1st step enroute to SUN */       
#define FROM_DSP_CONTROL LINK0IN        /* last previous step from SUN */
#define TO_B0BD          LINK0OUT       /* via DSP CONTROL */
#define FROM_B0BD        LINK0IN        /* via DSP CONTROL */
#define TO_SEQUENCER     LINK1OUT       /* SEQuencers */
#define TO_AUX_CTRL      LINK2OUT       /* Auxillary control */
#define FROM_SEQUENCER   LINK1IN        /* SEQuencers */
#define FROM_AUX_CTRL    LINK2IN        /* Auxillary control */ 

#define MAX_CHANS 8             /* maximum number of channels to watch */
                                /* names for indexs of channels in waitlist: */
#define SUN_IX     0
#define DBG_IX     1
#define SEQ_IX     2
#define ARRAY_IX   3
#define XM_IX      4
#define A2D_IX     5 
#define DEVCTRL_IX 6
#define AUXCTRL_IX 7

/******************************************************************************
 * Program: 	inst_ctrl
 * Macro: 	VAR_MOD()
 * Purpose: 	handles the modification of the control Variables when its 
 *              important
 * Parameters:  vno - int - which control variable to modify
 * 		val - int - value to set in variable
 * Returns:	none
 *
 *****************************************************************************/
#define VAR_MOD(vno, val)	SemP(var_lock); \
    				*(var_ptr + vno) = val; \
    				SemV(var_lock); /* release lock */


/******************************************************************************
 * Program: 	inst_ctrl
 * Macro: 	mod_SCBreg()
 * Purpose: 	handles the modification of the control register for the
 *		instrument controller
 * Parameters:  on - int - an int mask with ones in the bits to turn on
 *		off - int - an int mask with ones in the bits to turn off
 * Returns:	none
 *
 *****************************************************************************/
#define mod_SCBreg(on, off)	SemP(SCBreg_lock); \
    				SCBreg_val = (SCBreg_val | on) & (~ off); \
    				*SCBreg_ptr = SCBreg_val; /* update register */ \
    				SemV(SCBreg_lock); /* release lock */


/******************************************************************************
 * Program: 	inst_ctrl
 * Macro: 	TURNON_SCB()
 * Purpose: 	Turns on the static control bit given by the mask
 * Parameters:  msk - int - which static control bit to turn on
 * Returns:	none
 *
 *****************************************************************************/
#define TURNON_SCB(msk)		SemP(SCBreg_lock); \
    				SCBreg_val = (SCBreg_val | msk); \
    				*SCBreg_ptr = SCBreg_val; /* update register */ \
    				SemV(SCBreg_lock); /* release lock */


/******************************************************************************
 * Program: 	inst_ctrl
 * Macro: 	TURNOFF_SCB()
 * Purpose: 	Turns off the static control bit given by the mask
 * Parameters:  msk - int - which static control bit to turn off
 * Returns:	none
 *
 *****************************************************************************/
#define TURNOFF_SCB(msk)	SemP(SCBreg_lock); \
    				SCBreg_val = (SCBreg_val & (~ msk)); \
    				*SCBreg_ptr = SCBreg_val; /* update register */ \
    				SemV(SCBreg_lock); /* release lock */


/* number of buffers for debugging */
#define DEBUG_LINES 32

/* maximum number of sequencer chains and max length of chains */
#define MAX_S_CHAINS	2
#define MAX_CHAIN_LEN   2

#define EVEN 0
#define ODD  1
#define TOP  0
#define BOT  1

#define METHOD		0
#define SLOPE		1
#define INTERCPT	2
#define SCALEF		3







