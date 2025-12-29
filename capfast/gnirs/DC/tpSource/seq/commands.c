static char rcid[]="$Id: commands.c,v 1.2 2009/05/27 19:33:28 fkraemer Exp $";
/******************************************************************************
 * Program:	dspw
 * File:	commands.c (was config.c)
 * Purpose:	to give the configuration info
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		19Nov91	created						dak
 *		23Dec91	separated into files for each group		dak
 *
 *****************************************************************************/

#define NO_MEM

#include <conc.h>
#include "common.h"

#include "seq_defs.h"
#include "seq_vars.h"

#include "seqhdw.h"
#include "seq.h"

#define MEGS	(1024*1024)
#define KILO	(1024)

_MEMORY(32 * 1024);

char *memory = (char *) (0x80000000 + 256 * 1024 - 32 * 1024);

command_form command[] = {
		NULL,		/* IMAGE_DONE */	
		NULL,		/* BEGIN_XMIT */
		var,		/* SET_VAR */
		var,		/* READ_VAR */
		NULL,		/* DESBUG_MSG */
		NULL,		/* ABORT_MSG */
		NULL,		/* STOP_MSG */
		startMsg,	/* START_MSG */
		NULL,		/* PAUSE */
		NULL,		/* RESUME */
		NULL,		/* VAR_READ */
		NULL,		/* READ_HK */
 		NULL,		/* SET_VAR_AKK */
		NULL,		/* AKK */
		NULL,		/* AKK_FAIL */
		kill_proc,	/* KILL_PROC */
		myExecuteProc,	/* EXECUTE_PROC */
		NULL,		/* CHECK_HK */
		NULL		/* ERROR */
};

int num_command = 18;

int *var_ptr[32] = {      /* VarNum  Purpose */
    &trace_flag,      /*  0     trace level var for the Xputer node programs */
    prog,	      /*  1     a pointer to the ucode process code array */
    &frames,              /*2*/
    NULL,                 /*3*/
    &cntrl_reg,           /*4*/
    &lnr,                 /*5*/
    &coadds,              /*6*/
    &Int_Time_Seconds,    /*7*/
    &FInt_Time_Seconds,   /*8*/
    &Spad_Filter,         /*9*/
    &gINT(LastHdwVar),    /*10*/
    &quadrant,            /*11*/
    &ndavg ,              /*12*/
    &roisize,             /*13*/
    &var1,                /*14*/
    &var2,                /*15*/
    &var3,                /*16*/
    &var4                /*17*/
  
};

command_form setv_cmnds[32] = {
		set_var,	/* TRACE_FLAG       0*/
		set_var,	/* PROG             1*/
		set_var,        /* number of frames 2*/
		NULL,           /*                  3*/
		set_var,        /*cntrl_reg         4*/
		set_var,        /*lnr               5*/
		set_var,        /*coadds            6*/
		set_int,        /*Int_Time_Second   7*/
		set_int,        /*FInt_Time_Seconds 8*/
		set_var,        /*Spad_Filter       9*/
		set_var,        /*gINT(LastHdwVar)  10*/
		set_var,        /*quadrant          11*/
		set_var,        /*ndavg             12*/
		set_var,        /*roisize           13*/
		set_var,        /*var1              14*/
		set_var,        /*var2              15*/
		set_var,        /*var3              16*/ 
		set_var,        /*var4              17*/
	    
};

command_form readv_cmnds[32] = {
		read_var,	/* TRACE_FLAG        0  */
		read_var,	/* PROG              1 */
		read_var,           /*                   2*/
		NULL,           /*                   3*/
		read_var,       /* cntrl_reg         4*/
		read_var,       /* lnr               5*/
		read_var,       /* coadds            6*/
		read_int,       /* Int_Time_Second   7 */
		read_int,       /* FInt_Time_Seconds 8*/
		read_var,       /* Spad_Filter       9*/
		read_var,       /*gINT(LastHdwVar    10*/
		read_var,       /*quadrant           11*/
		read_var,        /*ndavg             12*/
		read_var,        /*roisize           13*/
		read_var,        /*var1              14*/
		read_var,        /*var2              15*/
		read_var,        /*var3              16*/
		read_var,        /*var4              17*/
	
		/* add new variables here pr?*/
};

Channel *chan_list[] = {
	NULL,			/* place holders */
	NULL,
	LINK0IN,
	LINK1IN,
	&DEBUG_CHAN,
	NULL,
	NULL
	};

