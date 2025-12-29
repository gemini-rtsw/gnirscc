#include <8051.h>
#include "temp.h"

/* Update the FOT0 and MNT0 heater DACs */
heaters()
{
    writeport_int(DACHI, (DacCmd[FOT0] >> 8) & 0x0F);
    writeport_int(DACCS, DacCmd[FOT0] & 0xFF);
    writeport_int(DACHI, ((DacCmd[MNT0] >> 8) & 0x0F) | 0x80);
    writeport_int(DACCS, DacCmd[MNT0] & 0xFF);
    writeport_int(DACLD, 0);
	updateHeat = DONE;
}

/* Update the FOT0 and MNT0 temperature set DACs */
tempSet()
{
	writeport_int(DACHI, ((tempDac[FOT0] >> 8) & 0x0F) | 0x40);
	writeport_int(DACCS, tempDac[FOT0] & 0xFF);
	writeport_int(DACHI, ((tempDac[MNT0] >> 8) & 0x0F) | 0xC0);
	writeport_int(DACCS, tempDac[MNT0] & 0xFF);
	updateTemp = DONE;
}

/* Read a value from a hardware port */
readport(pdat, port)
uc *port, *pdat;
{
   *pdat = *port;
}

/* Write a value to a hardware port */
writeport(port, pdat)
uc *port, pdat;
{
   *port = pdat; 
}

/* Write a value to a hardware port, disabling any TIMER0 interrupts during write */
writeport_int(port, pdat)
uc *port, pdat;
{
   asm(" CLR ET0");
   *port = pdat; 
   asm(" SETB ET0");
}

/* Set analog mux to (ch), start an ADC conversion and read, return value */
RdMux1(ch)
uc ch;
{
   ui  result_mux1;
   uc  adchi, adclo;

   asm(" CLR ET0");
   writeport(MUX1, ch);

   mydly();
   mydly();

   ADCA0 = 0;
   ADCRC = 0;
   writeport(ADCCS, 0);
   ADCRC = 1;

   while (ADCST);

   readport(&adchi, ADCCS);
   ADCA0 = 1;
   readport(&adclo, ADCCS);

   result_mux1 = 256*adchi + adclo;

   writeport(MUX1, MUX1_REF_FOT0);

   asm(" SETB ET0");
   return(4095-((result_mux1 >> 4) & 0x0FFF));
}

/* Software delay, ~0.55mSec with a 11.0592MHz xtal */
mydly()
{
#asm
   MOV $70, #$00
   DJNZ $70, $
#endasm
}

/* Calculate new heater PID servo loop values, disable TIMER0 interrupt during calc. */
servo()
{
   float servtmp;

   asm(" CLR ET0");

/* Servo for the foot */

   servtmp = 20.3*((float)(chanHG[FOT0]) - 2048.0); /* in uV */
   intserv[FOT0] += servtmp*(float)(IGain[FOT0])/(float)(1000000)/intserv[FOT0];

   if (intserv[FOT0] > 9.95) intserv[FOT0] = 9.95;
   if (intserv[FOT0] < 0.0) intserv[FOT0] = 0.0;
   
   cmdserv[FOT0] = intserv[FOT0] + (float)(PGain[FOT0])*servtmp/100000.0;

   if (cmdserv[FOT0] > 9.95) cmdserv[FOT0] = 9.95;
   if (cmdserv[FOT0] < 0.0) cmdserv[FOT0] = 0.0;

/* Servo for the mount */
	
	if (mntBw++ == 3)
	{
	mntBw = 0;
   	servtmp = 20.3*((float)(chanHG[MNT0]) - 2048.0); /* in uV */
   	intserv[MNT0] += servtmp*(float)(IGain[MNT0])/(float)(5000000)/intserv[MNT0];

   	if (intserv[MNT0] > 9.95) intserv[MNT0] = 9.95;
   	if (intserv[MNT0] < 0.0) intserv[MNT0] = 0.0;

   	cmdserv[MNT0] = intserv[MNT0] + (float)(PGain[MNT0])*servtmp/100000.0;

   	if (cmdserv[MNT0] > 9.95) cmdserv[MNT0] = 9.95;
   	if (cmdserv[MNT0] < 0.0) cmdserv[MNT0] = 0.0;
	}

/* Update DAC commands */

   DacCmd[FOT0] = (int)(cmdserv[FOT0] * 409.6);
   DacCmd[MNT0] = (int)(cmdserv[MNT0] * 409.6);

   asm(" SETB ET0");
}

/* Reset TIMER0 interrupt roll-over value, read ADC and sum with previous read */
void interrupt intXms()
{
     TEST = 0;			/* Tell outside world we're in ADC sample task */

#asm
     CLR TR0            /* CDA1 is 14ms for 11.0592 MHz */
     MOV TH0, #$DC      /* DC05 is 10ms for 11.0592 MHz */
     MOV TL0, #$05      /* FC66 is  1ms for 11.0592 MHz */
     SETB TR0
#endasm
     clk++;

     if (!SYNC1 || !SYNC2) syncwf = 0; 

     syncwf++;

     if (syncwf > SYNCWAIT)
       {
       syncwf = 250;

       HGl[FOT0] = ( 128*(long)(RdMux1(MUX1_HG_FOT0)) + 63*HGl[FOT0] ) >> 6;
       HGl[MNT0] = ( 128*(long)(RdMux1(MUX1_HG_MNT0)) + 63*HGl[MNT0] ) >> 6;

       chanHG[FOT0] = (int)(HGl[FOT0] / 128);
       chanHG[MNT0] = (int)(HGl[MNT0] / 128);
       }
}

/* Ext. interrupt 0, wait here until cleared */
void interrupt intSync()
{
	while (!SYNCWF);
	clk = 0;
}
