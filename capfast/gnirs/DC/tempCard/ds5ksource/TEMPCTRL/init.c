#include "temp.h"
#include <8051.h>

asmreg() 
{ 
#asm 
   	MOV SCON, #$50 
   	ORL PCON, #$80     /* PCON and Timer1 set for 19.2 KBd */
   	MOV TH1, #$FD 
   	MOV TH0, #$FC      /* Timer0 FC66 set for 1ms @ 11.0592 MHz */
   	MOV TL0, #$66 
   	MOV TMOD, #$21 
   	SETB TR0 
   	SETB TR1           /* Start counters 0 and 1 */
   	SETB ES 		   /* Turn on SERIAL port interrupt */
   	SETB ET0 		   /* Turn on TIMER0 interrupt */
	SETB IT0		   /* External interrupt 0 is falling edge triggered */
	SETB EX0		   /* Turn on ext.int0 */
   	SETB PS			   /* Serial port has highest priority! */
   	SETB EA            /* Enable interrupts */
#endasm 
} 

/* Initialize variables */
initch()
{
       chanHG[FOT0] = 0; 
       chanHG[MNT0] = 0; 
       HGl[FOT0] = 0; 
       HGl[MNT0] = 0; 
       cmdserv[FOT0] = 7.7;
       cmdserv[MNT0] = 4.1;
       DacCmd[FOT0] = (int)(409.6 * cmdserv[FOT0]);
       DacCmd[MNT0] = (int)(409.6 * cmdserv[MNT0]);
	   tempDac[FOT0] = 0;
       tempDac[MNT0] = 0;
       intserv[FOT0] = 0.0;
       intserv[MNT0] = 0.0;
       PGain[FOT0] = 50;
       PGain[MNT0] = 50;
       IGain[FOT0] = 100;
       IGain[MNT0] = 300;
}
