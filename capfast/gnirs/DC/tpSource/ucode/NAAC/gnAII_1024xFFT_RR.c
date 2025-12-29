/*****************************************************************************
 * Copyright 2000 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	gnAII_256xFFT_RR.c
 *
 * DESCRIPTION
 * This file contains the wave form definitions for the gnaac aladdin array
 *	controller. This ucode creates a 256x256 output file which reads a 
 *	single pixel set 2048 times. the upper level software must be set to 
 *	capture a 256x256 image in SEP mode.
 * The number of digital averages selects a sampling frequency
 *	1 = 500 kHz, 2 = 400 kHz, 4 = 250 kHz, 8 = 200 kHz, 16 = 100 kHz
 * The Int_time seconds gives the number of rows to skip 1-512
 * The FInt time seconds gives the number of 16 column groups to skip 1-32
 * The code delays exactly 500ms between the two reads of the image
 *
 * DEPENDENCIES
 *  This file requires sdc v2.3 or later. and an appropriate gnaacWFire.h file
 *
 * Author:
 *	Nick C. Buchholz
 *
 * History:
 *	2-May-2001 - Created file from gnAII_1024xA05_6RR.c - ncb
 *	
 *
 *****************************************************************************/
#include "gnaacWFire.h"

extern void *_heapend;

#define TRUE	1
#define FALSE	0

#define STARTROW	0
#define ENDROW		(STARTROW + 512)

#define ONE_DA		1
#define TWO_DA 		2
#define FOUR_DA		4
#define EIGHT_DA	8
#define SIXTEEN_DA	16

int firstpx;
int rows, rows2skip, cols2skip;
GLOBAL_CLK(1);			/* gnaac speed 20 MHz - 50 ns clock time */
/*****************************************************************************
 * Routine: start - this is the entry point for the transputer process which
 *	actually runs the waveform timing. main is defined by SDC to perform
 *	memory loading and hardware setup.
 *
 ****************************************************************************/
start()
{
    int i, j, c;
    int fs, fms;

/* startup defaults for startup */
cntrl_reg = 0;
Int_Time_MilliSecs = 0;
Int_Time_Seconds = 0;
FInt_Time_MilliSecs = 0;
FInt_Time_Seconds = 0;
lnr = 1;
coadds = 1;
quadrant = 0;
ndavgs = 1;	      /* valid values are 1,2,4,8 or 16 avgs */
colcnt = 32;	      /* valid values are 32,   24,  16,  8,  for 
		       *		  1024, 768, 512, 256 square */
 
    ProcToHigh();

    TITLE("GNAAC FFT Microcode based from gnAII_1024xSU01_6RR"); 

    /* reset the FIFO high and low water marks and fill factor */ 
    reset_fifo(0x0000);

    /* default starting point for waves is all bitf in off state */
    DEFAULT( );

    CLK_SPEED( 1 );   /* 50ns */

    /* one millisecond delay wave form define and run it here to give us
       time to load the FIFO */
    wave(onems,100)
    {
	wait(200);
    };

    /* Initialize the Row SR to reset all the row disable FF's */
    /* this will probably be more complex in actual chips since only some rows in
     * certain quadrants will need to be turned off 
     */
    wave(row_sr_init)
    {
	wait( 1 );
	set(SlowClks, 0x11);
	wait( 1 );
	toggle_hi(MClk, 2);
	wait( 1 );
	set(SlowClks, 0x00);
	set(RowDis, 0x1F);
	toggle_hi( MClk, 2);
	wait(250);
	set(RowDis, 0x01);
	toggle_hi( MClk, 2);
	set(SlowClks, 0x01);
	wait( 1 );
	toggle_hi(MClk, 2);
	wait( 1 );
	set(SlowClks, 0x00);
	wait( 1 );
    };

#if 1
    wave(tServoOn)
    {
	wait( 1 );
	on(Tswtch, 2);
	wait( 1 );
	toggle_hi(TdClk, 2);
	wait( 1 );
    };
#endif
    
    /* start the FIFO reading out, this also starts the waveform generator */
    start_fifo();

    /* loop forever doing the correct things */
    while (TRUE)
    {
	cols2skip = (int)0;
	rows2skip = (int)0;
	if ( cntrl_reg & DIE )	/* we've been asked to Die do it */
	    suicide();		/* probably reloading ucode */

	if ( cntrl_reg & IDLE_FLAG ) /* idle state no waveforms generated */
	    continue;		/* not used much with current arrays */

	/* need to set up here for whether or not simulation is being done */
	if ( cntrl_reg & SIM_FLAG ) /* put the ADC into Simulation mode */
	{
	    /* The ADC's go into simulation mode if 12 XCLKs are sent to the 
	     * XLINIX chip with reset on */
	    wave ( simulon )
	    {
		wait( 1 );
		on ( ADCRst & XRst, 2 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 2 );
		off ( ADCRst & XRst, 2 );
		wait ( 1 );
	    };
	}
	else
	{
	    /* The ADC's leave simulation mode if >=17 XCLKs are sent to the 
	     * XLINIX chip with reset on */
	    wave(simuloff)
	    {
		wait( 1 );
		on ( ADCRst & XRst, 2 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		on ( XClk, 1 );
		off ( XClk, 1 );
		wait ( 1 );
		off ( ADCRst & XRst, 2 );
		wait ( 1 );
	    };
	}

	/* standard readout techniques here */
	if ( cntrl_reg & Read_Flag ) /* we want to read out the array */
	{
#if 1
	    wave(tServoOff)
	    {
		wait( 1 );
		off(Tswtch, 2);
		toggle_hi(TdClk, 2);
		wait( 1 );
	    };
#endif		
	    inst_state = RESETING;
	    access_array();	/* OK reset the array */
	    wave(onems);	/* wait one milisecond needed to avoid
				  overrunning the data capture routines */

	    firstpx = 1;
	    inst_state = READ1;
	    access_array(); /* OK read the entire array once */
	    wave(onems);
		
	    for( i=0; i<100; i++) /* delay between frames 500 ms */
	    {
		wave(onems);	/* wait one milisecond needed to avoid */
	    }
		
	    firstpx = 1;
	    inst_state = READ2;
	    access_array();
	    wave(onems);
#if 1
	    wave(tServoOn);		
#endif
	    wave(onems);

	    
	    if (!(cntrl_reg & SDT_MODE)) /* are we in sdt Mode? */
	    {
		cntrl_reg &= ~ Read_Flag; /* if not reset read flag after
					     each observation */
	    }

	    if ((cntrl_reg & ABORT_INT)) /* are we Aborting? */
	    {
		cntrl_reg &= ~ ABORT_INT ; /* if so reset abort flag after
					     each observation */
	    }
#if 0
		
	    /* wait at least 60 ms for DQ to setup Acq pipe for second 
	     * frame. This needs to be tuned to minimum possible time */
	    for(j=0; j<10; j++) 
	    {
		wave(onems);	/* wait one milisecond */
	    }
#endif
	}
	else
	{   /* we aren't reading out so just reset the array  */
	    inst_state = RESETING;
	    access_array();
	    for(j=0; j<10; j++) 
	    {
		wave(onems);	/* wait one milisecond */
	    }
	    wave(onems);
	}
    } /* end forever loop */
}

/*****************************************************************************
 * Routine: access_array - handle setup of the array for reset or readout and
 *		generates appropriate waveforms for each read of the array
 *
 ****************************************************************************/
access_array()
{

    int i;

    DEFAULT( );

    wave( Clamp_and_Reset_setup )
    {
	wait( 1 );
	set( CandR, 0x01);
	set(RowEnbl, 0x1);
	wait( 1 );
	toggle_hi( MClk, 2 );
	wait( 1 );
	set( CandR, 0x0);
	set(RowEnbl, 0x0);
	wait( 1 );
    };

    switch (inst_state )
    {
      case RESETING:
	  wave( rsarray )
	  {
	      toggle_hi( Actv & RstLoopSync, 2 );
	      wait ( 2 );
	  };

	  wave( ResetArray )
	  {
	      wait ( 1);
	      set(CandR, 0x9);
	      set(RowEnbl, 0x1);
	      wait( 1 );
	      toggle_hi( MClk, 1 );
	      wait( 1 );
	      set(CandR, 0x0);
	      set(RowEnbl, 0x0);
	      wait( 400 );
	      set(CandR, 0x01);
	      wait( 1 );
	      toggle_hi( MClk, 1 );
	      wait( 1 );
	      set(CandR, 0x0);
	      wait( 73 );
	      wait( 1 );
	  };
	  return (0);

      case READ1:
	wave( v1Sync )
	{
	    wait( 1 );
	    toggle_hi( Read1Sync & ACB53 , 2 );
	    wait(1);
	};

	wave( vSync0 )
	{
	    wait ( 1 );
	    on ( Vsync );
	    wait(100);
	    off (Vsync);
	    wait ( 1 );
	};
	break;

      case READ2:
	wave( v2Sync )
	{
	    wait( 1 );
	    toggle_hi( Read2Sync & ACB54, 1 );
	    wait(1);
	};
	wave (vSync0);
	break;

    }

    DEFAULT( );
    wave (ar_start)
    {
	wait(1);
	set(SlowClks, 0x11);
	set(FastClks, 0x9);
	wait( 1 );
	toggle_hi(MClk, 2);
	wait( 5 );
	set(SlowClks, 0x01 );
	set(FastClks, 0x1);
	wait( 1 );	
	toggle_hi(MClk, 2);
	wait( 1 );
	set(SlowClks, 0x03);
	set(FastClks, 0x0);
	wait ( 1 );
	toggle_hi( MClk , 2);
	wait ( 5 );
	set(SlowClks, 0x01);
	wait ( 1 );
	toggle_hi(MClk, 2);
	wait( 1 );
	set(SlowClks, 0x00);
        set( CandR, 0x3);
	set(RowEnbl, 0x1);
        wait( 1 );
	toggle_hi( MClk, 2 );
	wait( 1 );
	set( CandR, 0x0);
	set(RowEnbl, 0x0);
	wait ( 1 );
    };


    for(i=0; i<1; i++)
    {
	Do_row( i );
    }

    if (inst_state == READ1)
    {
	wave ( lastpx1 )
	{
	    wait( 1 );
	    set(CandR, 0x1);
	    set(RowEnbl, 0x1);
	    wait( 1 );
	    toggle_hi(MClk,2);
	    wait( 1 );
	    set( CandR, 0x0);
	    set(RowEnbl, 0x1);
    	    wait( 1 );
	};
    }
    else if(inst_state == READ2)
    {
	wave ( lastpx2 )
	{
	    wait( 1 );
	    set(CandR, 0x1);
	    set(RowEnbl, 0x1);
	    wait( 1 );
	    toggle_hi(MClk,2);
	    wait( 1 );
	    set( CandR, 0x0);
	    set(RowEnbl, 0x0);
	    wait( 1 );
	};
    }

    wave(ar_end)
    {
        wait ( 1 );
	set(SlowClks, 0x11);
	set(FastClks, 0x09);
	wait ( 1 );
	toggle_hi(MClk,2);
	wait ( 1 );
	set(SlowClks, 0x01);
	set(FastClks, 0x01);
	wait ( 7 );
	toggle_hi(MClk,2);
	set(SlowClks, 0x00);
	set(FastClks, 0x00);
	wait ( 1 );
    };
    
}


Do_row(row)
int row;
{
    int i, j;
    int *fifo_read = (int *) 0xe0000000;
    int *fifo_write = (int *) 0xa0000000;

    i = row % 4;
    DEFAULT( );

    switch (i)
    {
      case 0: /* Odd Row Pair, Odd Row */
        wave( rowOO )
	{
	    wait( 1 );
	    set(SlowClks, 5);
	    wait ( 1 );
	    toggle_hi(SPARE52 & MClk , 2);
	    wait ( 1 );
	    set(SlowClks, 0);
	    set(CandR, 0x7);
	    set(RowEnbl, 0x3);
	    wait ( 1 );
	    toggle_hi(MClk, 2);
	    wait( 1 );
	    set(CandR, 0x00);
	    set(RowEnbl, 0x0);
	    wait( 250 );
	    wait( 1 );
	};
	break;
      case 1: /* Odd Row Pair, Even Row */
	wave( rowOE )
	{
	    wait( 1 );
	    set(SlowClks, 0x0D);
	    wait ( 1 );
	    toggle_hi(SPARE52 &  MClk , 2);
	    wait ( 1 );
	    set(SlowClks, 0x0);
	    set(CandR, 0x7);
	    set(RowEnbl, 0x3);
	    wait ( 1 );
	    toggle_hi(MClk, 2);
	    wait( 1 );
	    set(CandR, 0x00);
	    set(RowEnbl, 0x0);
	    wait ( 250 );
	    wait( 1 );
	};
	break;
      case 2: /* Even Row Pair, Odd Row */
	wave( rowEO )
	{
	    wait( 1 );
	    set(SlowClks, 3);
	    wait ( 1 );
	    toggle_hi(SPARE52 & MClk , 2);
	    wait ( 1 );
	    set(SlowClks, 0x0);
	    set(CandR, 0x7);
	    set(RowEnbl, 0x3);
	    wait ( 1 );
	    toggle_hi(MClk, 2);
	    wait( 1 );
	    set(CandR, 0x00);
	    set(RowEnbl, 0x0);
	    wait ( 250 );
	    wait( 1 );
	};
	break;
      case 3: /* Even Row Pair, Even Row */
        wave( rowEE )
	{
	    wait( 1 );
	    set(SlowClks, 0xB);
	    wait ( 1 );
	    toggle_hi(SPARE52 & MClk , 2);
	    wait ( 1 );
	    set(SlowClks, 0x0);
	    set(CandR, 0x7);
	    set(RowEnbl, 0x3);
	    wait ( 1 );
	    toggle_hi(MClk, 2);
	    wait( 1 );
	    set(CandR, 0x00);
	    set(RowEnbl, 0x0);
	    wait ( 250 );
	    wait( 1 );
	};
	break;
     }

    if (row < rows2skip)
    {
	return (0);
    }

    switch (inst_state)
    {
      case READ1:
      case READ2:
	    wave( InjctBit )
	    {
		wait( 1 );
		set( FastClks, 9);
		wait( 1 );
		toggle_hi( MClk, 2 );
		wait( 1 );
		set( FastClks, 1);
		wait( 1 );
		toggle_hi( MClk, 2 );
		wait( 1 );
		set( FastClks, 3);
		wait( 1 );
		toggle_hi( MClk, 2 );
		set( FastClks, 1);
		wait( 1 );
		toggle_hi( MClk, 2 );
		set(FastClks, 0x0);
		wait( 10);
	    };

	    for (i=0; i<cols2skip; i++)
	    {

		wave(skip64Pxls)
		{
		    wait( 1 );
		    set( FastClks, 5); /* First 8 pixels */
		    wait( 1 );
		    toggle_hi( PxlSync & MClk, 2 );
		    wait( 5 ) ;
		    set( FastClks, 1);
		    wait( 1 );
		    toggle_hi( MClk, 2 );
		    set( FastClks, 3); /* Second 8 pixels */
		    wait( 3 );
		    toggle_hi( MClk, 2 );
		    wait( 2 );
		    set( FastClks, 1);
		    wait( 2 );
		    toggle_hi( MClk, 2 );
		    wait( 6 );
		};
	    }		    		    

	    wave(settlePxl)
	    {
	      wait( 1 );
	      set( FastClks, 5); /* First 8 pixels */
	      wait( 1 );
	      toggle_hi(MClk, 2 );
	      wait( 5 ) ;
	      set( FastClks, 1);
	      wait( 1 );
	      toggle_hi( MClk, 2 );
	      wait( 1 );
	      wait( 60 );
	      wait( 60 );
	      wait( 1 );
	    };

	    for (i=0; i<2048; i++)
	    {
	        switch (ndavgs)
		{
		  case ONE_DA:
		      wave (rd32x4_2us) /* 40 tics (2us) per sample */
		      {
			  wait( 1 );
			  toggle_hi(PxlSync & CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 24 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 24 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 24 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 24 );
			  wait(1);			
		      };
		      wave (rd32x4_2us);
		      wave (rd32x4_2us);
		      wave (rd32x4_2us);
		      break;

		case TWO_DA: 
		      wave (rd32x4_2hus) /* 50tics (2.5us per sample */
		      {
			  wait( 1 );
			  toggle_hi(PxlSync & CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 34 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 34 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 34 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 34 );
			  wait( 1 );	
		      };
		      wave (rd32x4_2hus);
		      wave (rd32x4_2hus);
		      wave (rd32x4_2hus);
		      break;

		  case FOUR_DA:
		      wave (rd32x4_4us) /* 80 tics (4 Us) per Sample */
		      {
			  wait( 1 );
			  toggle_hi(PxlSync & CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 64 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 64 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 64 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 64 );
			  wait( 1 );	
		      };
		      wave (rd32x4_4us);
		      wave (rd32x4_4us);
		      wave (rd32x4_4us);
		      break;

		  case EIGHT_DA:
		      wave (rd32x4_5us)	/* 100 tics (5us) per sample */
		      {
			  wait( 1 );
			  toggle_hi(PxlSync & CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 84 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 84 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 84 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 84 );
			  wait( 1 );	
		      };
		      wave (rd32x4_5us);
		      wave (rd32x4_5us);
		      wave (rd32x4_5us);
		      break;

		  case SIXTEEN_DA:
		      wave (rd32x4_10us) /* 200 tics (10us) per sample */
		      {
			  wait( 1 );
			  toggle_hi(PxlSync & CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 184 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 184 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 184 );
			  toggle_hi(CTC, 2);
			  wait( 7 );
			  on(ADCRst & XRst);
			  wait( 1 );
			  on( XClk );
			  wait( 1 );
			  off(ADCRst & XRst & XClk);
			  wait( 1 );
			  toggle_hi(XClk, 2);
			  wait( 1 );
			  toggle_hi( Dxfr & Hsync, 1 );	
			  wait( 184 );
			  wait( 1 );				  
		      };
		      wave (rd32x4_10us);
		      wave (rd32x4_10us);
		      wave (rd32x4_10us);
		      break;
		}		
	    }
	    
    }

}

/******************************************************************************
 * Integration Time Routines for sequences.
 * Author: Nick C. Buchholz
 * Copyright:	26-Apr-1991 - Aura Inc. - All Rights Reserved
 *
 *****************************************************************************/

integrate()
{
    int i;
    int secs, msecs;
    DEFAULT( );

    secs = 0;
    msecs = 500;

    while (secs--)
    {
	one_second();
	if ( cntrl_reg & DIE )
	    suicide();
 	if (cntrl_reg & ABORT_INT)
	    break;	    
    }

    wave(onems);

    while (msecs--)
    {
	wave(onems);
    }

}

one_second()
{
    int i;

    for(i=0; i<1000; i++)
	wave(onems);
}

fdelay()
{
    int i;
    int secs, msecs;
    DEFAULT( );

    secs = 0;
    msecs = 500;

    while (secs--)
    {
	one_second();
    }

    wave(onems);
    while ( msecs-- )
    {
	wave(onems);
    }

}

