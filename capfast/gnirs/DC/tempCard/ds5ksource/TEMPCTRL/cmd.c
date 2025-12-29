#include <stdio.h>
#include <8051.h>
#include "temp.h"
#include "../prot/prot.h"

/* Decode the received command and execute its' function */
/* NOTE : Read = read DS5000 register and transmit it to the console */
/*         Set = receive a valuefrom the console and update DS5000 register */
gocmd()
{
     switch (xxcmd)
       {
     case 'N':		/* Read an analog mux channel */
		errno = RdMux1((xxd1 & 0x000F) | 0x0010);
	 	break;

     case 'h':		/* Set the heater DACs */
	 	SetDACs();
	 	break;

     case 'T':		/* Read the temperature DACs */
	 	RdTemp();
	 	break;

     case 't':		/* Set the temperature DACs */
	 	SetTemp();
	 	break;

     case 'F':		/* Read heater current feed back */
	 	RdHtrFB();
	 	break;

     case 'H':		/* Read high gain input of sensor */
	 	RdHG();
	 	break;

     case 'L':		/* Read low gain input of sensor */
	 	RdLG();
	 	break;

     case 'R':		/* Read sensor reference (threshold) voltage */
	 	RdRef();
	 	break;

     case 'V':		/* Read integral servo error */
	 	servDev();
	 	break;

     case 'C':		/* Read servo command */
	 	servCmd();
	 	break;

     case 's':		/* En/disable servo loop calculations */
     	errno = servoff = xxd1;
	 	break;

     case 'G':		/* Read proportional gain */
     	RdPGain();
	 	break;

     case 'g':		/* Set proportinal gain */
     	SetPGain();
	 	break;

     case 'D':		/* Read integral gain */
     	RdIGain();
	 	break;

     case 'd':		/* Set integral gain */
     	SetIGain();
	 	break;

     default:
     	errno = INVCMD;
     }
     rperr();
     return(0);
}

SetDACs()    /* Command to set the DACs to a given value */
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = DacCmd[FOT0] = xxd2 & 0xFFF;
     if (xxd1 == MNT0) errno = DacCmd[MNT0] = xxd2 & 0xFFF;
	 if (errno != INVAXIS) updateHeat = PENDING;
}

SetTemp()	/* Set the temperature DAC's */
{
     errno = INVAXIS;
	 if (xxd1 == FOT0) errno = tempDac[FOT0] = xxd2 & 0xFFF;
	 if (xxd1 == MNT0) errno = tempDac[MNT0] = xxd2 & 0xFFF;
	 if (errno != INVAXIS) updateTemp = PENDING;
}

RdTemp()	/* Read the temperature DACs */
{
	 errno = INVAXIS;
	 if (xxd1 == FOT0) errno = tempDac[FOT0];
	 if (xxd1 == MNT0) errno = tempDac[MNT0];
}

servCmd()           /* Read servo command */
{
     errno = INVAXIS;
     asm(" CLR ET0");
     if (xxd1 == FOT0) errno = (int)(10*cmdserv[FOT0]);
     if (xxd1 == MNT0) errno = (int)(10*cmdserv[MNT0]);
     asm(" SETB ET0");
}

servDev()           /* Read integral servo error */
{
     errno = INVAXIS;
     asm(" CLR ET0");
     if (xxd1 == FOT0) errno = (int)(2048+45*intserv[FOT0]);
     if (xxd1 == MNT0) errno = (int)(2048+45*intserv[MNT0]);
     asm(" SETB ET0");
}

RdRef()             /* Reference temperatures */
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = RdMux1(MUX1_REF_FOT0);
     if (xxd1 == MNT0) errno = RdMux1(MUX1_REF_MNT0);
}

RdLG()              /* Low gain, absolute */
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = RdMux1(MUX1_LG_FOT0);
     if (xxd1 == MNT0) errno = RdMux1(MUX1_LG_MNT0);
}

RdHG()              /* High gain */
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = chanHG[FOT0];
     if (xxd1 == MNT0) errno = chanHG[MNT0];
}

RdHtrFB()           /* Heater feedback */
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = RdMux1(MUX1_HFB_FOT0);
     if (xxd1 == MNT0) errno = RdMux1(MUX1_HFB_MNT0);
}

RdPGain()
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = PGain[FOT0];
     if (xxd1 == MNT0) errno = PGain[MNT0];
}

SetPGain()
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = PGain[FOT0] = xxd2;
     if (xxd1 == MNT0) errno = PGain[MNT0] = xxd2;
}

RdIGain()
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = IGain[FOT0];
     if (xxd1 == MNT0) errno = IGain[MNT0];
}

SetIGain()
{
     errno = INVAXIS;
     if (xxd1 == FOT0) errno = IGain[FOT0] = xxd2;
     if (xxd1 == MNT0) errno = IGain[MNT0] = xxd2;
}

