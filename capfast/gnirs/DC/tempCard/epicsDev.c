
static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: epicsDev.c,v 1.2 2009/05/27 19:33:20 fkraemer Exp $"
};
#define DEBUG 
 
int tempdebug;

#include <netinet/in.h> 
#include "sockutil.h"

double cnvtLKS(double x);
#include "epicsDev.h"
extern char *dbTop;
#define BYTESPERMSG 8
char tmp[80];
/* debugging routine*/
void dPrint(int val,char *str)
{
    if (val > 0)
    {
	fputs(str,stderr);
	fflush(stderr);
    }
  
}
/*
 *+
 * FUNCTION NAME:
 * init_aoTempPort
 *
 * INVOCATION:
 * static long init_aoTempPort(pao)
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  !pao - pointer to ao record
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *   Initialiaze the record that writes to the temp port
 *
 *
 * DESCRIPTION:
 *     This routine creates a mutual exclusion semaphore, Opens the serial port
 *   to the temperature card, and initializes the pao structure
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  
 *
 *-
 */
static long init_aoTempPort(pao)
    struct aoRecord *pao;
{
    char dst[INSTIO_FLD_SZ];
    cicsLogMessage(3, "***********init_aoTempPort2():\n" ) ; 
    
    if (semTempPort == NULL)
	semTempPort = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
    
    if (Port2 == 0)
	if (openport2() == ERROR)
	    return ERROR;
    
    if( pao->out.type != INST_IO ) 
    {
	recGblRecordError(S_db_badField,(void *)pao,
			  "devaoTempPort2 (init_record) Illegal out.type");  
	return(S_db_badField);
    }

  
    (void)strncpy( dst, pao->out.value.instio.string,INSTIO_FLD_SZ );
    dst[INSTIO_FLD_SZ-1] = NULL; 
  
    pao->dpvt = dst ; /* this is used later in write */

      
    return( OK ) ;
}
/*
 *+
 * FUNCTION NAME:
 * write_aoTempPort
 *
 * INVOCATION:
 * static long write_aoTempPort(pao)
 * initiated by processing a temp card ao record
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  >pao - pointer to ao record 
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   Send a message to the temperature card and wait for a reply
 *
 * DESCRIPTION:
 *    
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */
static long write_aoTempPort(pao)
struct aoRecord	*pao;
{
    long time;
    long nBytes;
    char  *pvals;
    long outputVal;
    char outString[20],junk[20],inString[20],rinString[20];
    char str[80];
  
    pvals = pao->dpvt;
    dPrint(serOn, "***********write_aoTempPort2():");
   
    /* value to send*/
    outputVal = (long)pao->val;
    sprintf (outString,"%s %-4d%c",pao->out.value.instio.string,outputVal,10);
    dPrint(serOn,outString); 
  
    /* get the receive Message Queue semaphore here */
    if (semTake(semTempPort , SemTakeTimeout*10) == OK)
    {
      /* there should be nothing on the serial port, if there is get rid of it*/
	ioctl(Port2, FIONREAD, (int)junk);
	if (junk != 0)
	{
	    ioctl(Port2, FIOFLUSH,NULL);
	  
	}
	dPrint(serOn,outString );
	write(Port2,outString,strlen(outString));

	dPrint(serOn,	 "***********write_aoTempPort2(): waiting for reply\n" ) ;  

	/* Wait for the reply or timeout. */
	nBytes = 0;
	time = tickGet() + 4*sysClkRateGet();
	while ((nBytes<BYTESPERMSG) && (tickGet()<time))
	{
	    taskDelay(1);
	    ioctl(Port2, FIONREAD, (int)(&nBytes));
	}

	if (nBytes == BYTESPERMSG) 	
	{ 	  
	    read(Port2, inString, BYTESPERMSG); 	
	    if (inString[0] != 'R') 
	    {
		sprintf (str,"output = %c%c%c%c%c%c %0x %0x\n",inString[0],inString[1],inString[2],inString[3],inString[4],inString[5],inString[6],inString[7]); 

		dPrint(serOn,str);
	
		sprintf(str,"\nReply from port2  has wrong code %s\n",inString); 
		dPrint(serOn,str);
		(void)semGive( semTempPort);

		return(ERROR);
	    }
	    sprintf (str,"output = %c%c%c%c%c%c %0x %0x\n",inString[0],inString[1],inString[2],inString[3],inString[4],inString[5],inString[6],inString[7]); 

	    dPrint(serOn,str);
	    rinString[0] = inString[2];
	    rinString[1] = inString[3];
	    rinString[2] = inString[4];
	    rinString[3] = inString[5];
	    rinString[4] = 0;
	    sscanf(rinString, "%x", &outputVal);
	}
	else if (nBytes >0)/* nBytes != BYTESPERMSG*/
	{
	    read(Port2, inString, nBytes); 
	    sprintf(tmp, "tempCard port read error %s - %d\n",outString, nBytes); 
	    cicsLogMessage(0,tmp);
	    sprintf (str, " read <%s> \n", inString);
	    DPRINT(serOn, str);
	    ioctl(Port2, FIOFLUSH,NULL); 
	}
	else
	{ 
	    ioctl(Port2, FIOFLUSH,NULL); 
	    /* 	  cicsLogMessage(1,"Port2 read error"); */
	    sprintf(tmp,"Port2 read error %s\n",outString); 
	    cicsLogMessage(0,tmp);
	    (void)semGive( semTempPort);
	    return(ERROR); 
	} 
     	
	/* Give the receive Message Queue semaphore here */
	(void)semGive( semTempPort);
      

    }   
    else	
    {
	printf ("Couldn't take semaphore %s \n",outString);
	return ERROR;
    }
  
    dPrint(serOn, "***********write_aoTempPort2(): done\n" ) ; 
    return( OK ) ;
}

/*
 *+
 * FUNCTION NAME:
 * init_aiTempPort
 *
 * INVOCATION:
 * static long init_aiTempPort(pai)
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  !pai - pointer to ai record
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   Initialize the record that reads values from the temp card.
 *
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  
 *
 *-
 */

static long init_aiTempPort(pai)
    struct aiRecord *pai;
{
   
    char dst[INSTIO_FLD_SZ];
   
    
    if (semTempPort == NULL)
	semTempPort = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
    
    if (Port2 == 0)
	if (openport2() == ERROR) 
	    return ERROR;
    
    if( pai->inp.type != INST_IO ) 
    {
	recGblRecordError(S_db_badField,(void *)pai,
			  "devaoTempPort2 (init_record) Illegal out.type");  
	return(S_db_badField);
    }
  /* calculate linear conversion */
  if(pai->aoff == -1)
    {
      /* the constants correspond to a 16 bit range of the input*/
      /* eguf and eguf are the upper and lower limits of the output*/
      pai->eslo = (pai->eguf - pai->egul)/65535.0;
      pai->roff = 32768.0;
    }
  else
    {
      pai->roff = 0.0;
      pai->eslo = 1.0;
    }
  
    strncpy( dst, pai->inp.value.instio.string,INSTIO_FLD_SZ );
    dst[INSTIO_FLD_SZ-1] = NULL; 

    pai->dpvt = dst ; /* this is used later in read */
 
   
    return( OK ) ;
}

/*
 *+
 * FUNCTION NAME:
 * read_aiTempPort
 *
 * INVOCATION:
 * static long read_aiTempPort(pai)
 * initiated by processing a temp card ai record
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  !pai - pointer to ai record
 *
 * FUNCTION VALUE:
 *   none
 *
 * PURPOSE:
 *   Read a value from the temp card
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * June-1997  Original version adapted from CICS alpha 1.0  Jan Schwitters
 *
 *-
 */


static long read_aiTempPort(pai)
    struct aiRecord	*pai;
{
  int swVal;
    long time;
    short out1,out2;
    long nBytes;
    long outputVal;
    char outString[20],junk[20],inString[20],rinString[20];
    char str[80];
  
    dPrint(serOn,"***read_aiTempPort2():  " ) ;  
   
    outputVal = pai->val ;  

    sprintf(outString,"%s%c",pai->inp.value.instio.string,10);
   
  
    /* get the receive Message Queue semaphore here */
    if (semTake(semTempPort , SemTakeTimeout*15) == OK)
    {
	ioctl(Port2, FIONREAD, (int)junk);
	if (junk != 0)
	{
	    ioctl(Port2, FIOFLUSH,NULL);
	  
	}
	dPrint(serOn,outString);
	write(Port2,outString,strlen(outString));

	/* Wait for the reply or timeout. */
	nBytes = 0;
	time = tickGet() + 8*sysClkRateGet();
	while ((nBytes<BYTESPERMSG) && (tickGet()<time))
	{
	    taskDelay(1);
	    ioctl(Port2, FIONREAD, (int)(&nBytes));
	}
  
	if (nBytes == BYTESPERMSG) 	
	{ 
	    read(Port2, inString, BYTESPERMSG);
	    sprintf (str,"return = %c%c%c%c%c%c %0x %0x\n",inString[0],inString[1],
		     inString[2],inString[3],inString[4],inString[5],inString[6],inString[7]); 
	    dPrint(serOn,str);
	    if (inString[0] != 'R') 
	    {      
	
		(void)semGive( semTempPort);
		return(ERROR);
	    }
	  
	    rinString[0] = inString[2];
	    rinString[1] = inString[3];
	    rinString[2] = inString[4];
	    rinString[3] = inString[5];
	    rinString[4] = 0;

	    sscanf(rinString,"%x",(unsigned int  *)&out1);
	    sscanf(rinString, "%x", &outputVal);
	    sprintf (str,"<%s> -> 0x%x \n",rinString, outputVal);
	    dPrint(serOn,str);
	    
 	} 
	else if (nBytes >0) /* nBytes != BYTESPERMSG*/
	{ 
	    read(Port2, inString, nBytes); 
	    sprintf(tmp,"Port2 read error out =<%s> Bytes = %d \n",outString, nBytes); 
	    cicsLogMessage(0,tmp);
	    printf ( "    read <%s> \n", inString);
	   
	  
	}
	else /* nBytes <= 0*/
 	{ 
	    ioctl(Port2, FIOFLUSH,NULL); 
	  
	    sprintf(tmp,"Port2 read error num bytes = %d %s\n",nBytes,outString);
	    cicsLogMessage(0,tmp);
	    (void)semGive( semTempPort);
	    return(ERROR); 
 	} 
     	
	/* Give the receive Message Queue semaphore here */
	(void)semGive( semTempPort);
	
	swVal = pai->aoff; 

      	switch (swVal)
	  {
	  case -1:
	    if(outputVal > 0x7fff)
	      out1 = outputVal - 0xffff;
	    else
	      out1 = outputVal ;
	    out2 = (out1 + pai->roff)*pai->eslo + pai->egul;
	    pai->val = out2;
	  
	    break;
	  case 0:
	    pai->val = outputVal;
	    break;
	  default:
	    pai->val = outputVal;
	  }

	/* We got the message. */
    }
    else	
    {
	printf ("Couldn't take semaphore %s\n",outString);
	return ERROR;
    }
  
    sprintf( str,"***********read_aiTempPort2(): done %s\n\n\n",outString ) ; 
    dPrint(serOn,str);
    return( DO_NOT_CONVERT ) ;
}

/*
 *+
 * FUNCTION NAME:
 * cnvtLKS
 *
 * INVOCATION:
 * double cnvtLKS(double x)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  >double x = voltage read from sensor 
 *
 * FUNCTION VALUE:
 *   temperature corresponding to inputted voltage
 *
 * PURPOSE:
 *  linear aproximation of the voltage to temperature  curve for a laeshore
 * temperature sensor
 *
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 
 *
 *-
 */
double cnvtLKS(double x)
{
    /* this was taken from temperature control routine written by nick rodier*/
    if (x<0.51892) 
	return(300.0);
    if (x<0.97550) 
	return(300.0 - 200.0*(x-0.51892)/0.4566);
    if (x<1.05267) 
	return(100.0 -  40.0*(x-0.97550)/0.0772);
    if (x<1.06700) 
	return( 60.0 -   8.0*(x-1.05267)/0.0143);
    if (x<1.07748) 
	return( 52.0 -   6.0*(x-1.06700)/0.0105);
    if (x<1.08781) 
	return( 46.0 -   6.0*(x-1.07748)/0.0103);
    if (x<1.09310) 
	return( 40.0 -   3.0*(x-1.08781)/0.0053);
    if (x<1.09864) 
	return( 37.0 -   3.0*(x-1.09310)/0.0055);
    if (x<1.10482) 
	return( 34.0 -   3.0*(x-1.09864)/0.0062);
    if (x<1.11212) 
	return( 31.0 -   3.0*(x-1.10482)/0.0073);
    if (x<1.12463) 
	return( 28.0 -   3.0*(x-1.11212)/0.0125);
    if (x<1.17705) 
	return( 25.0 -   3.0*(x-1.12463)/0.0524);
    return(22.0);
}
/*
 *+
 * FUNCTION NAME:
 * 
 *
 * INVOCATION:
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 
 *
 * FUNCTION VALUE:
 *  
 *
 * PURPOSE:
 * 
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 
 *
 *-
 */

double cnvtLKSInv(double t)
{
    if (t>=300)
	return (.51892);
    else if (t >= 100)
	return (-.4566*(t - 100)/200+.51892);
    else if (t >= 60)
	return (-.0772 * (t - 60) / 40 + 1.05267);
    else if (t >= 52)
	return (-.0143 * (t - 52) / 8 + 1.05267);
    else if (t >= 46)
	return (-.0105 * (t - 60) / 8 + 1.067);
    else if (t >= 40)
	return (-.0103 * (t-46) / 6 + 1.07748);
    else if (t >= 37)
	return (-.0053 * (t-40) / 3 + 1.08781);
    else if (t >= 34)
	return (-.0055 * (t-37) / 3 + 1.0931);
    else if (t >= 31)
	return (-.0062 * (t-34) / 3 + 1.09864);
    else if (t >= 28)
	return (-.0073 * (t-31) / 3 + 1.10482);
    else if (t >= 25)
	return (-.0125 * (t-28) / 3 + 1.11212);
    else if ( t >= 22)
	return (-.0524 * (t - 25) / 3 + 1.12463);
    else
	return (1.7703);/* value that corresponds to 22 degrees*/
}
 /*
 *+
 * FUNCTION NAME:
 * 
 *
 * INVOCATION:
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * 
 *
 * FUNCTION VALUE:
 *  
 *
 * PURPOSE:
 * 
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 
 *
 *-
 */

#if 0
double v2TDLKS(double a,double b)
{
    double tdat, tref;
    double ret;

    if (a > 4.5)
	ret = -999.0;
    else if (a < -4.5) 
	ret = 999.0;
    else
    { 
	tdat =  cnvtLKS(b + a/120.0);
	/* 	 printf ("tdat = %.3f\n",tdat); */
	 
	tref = cnvtLKS(b);
	/* 	 printf ("tref = %.3f\n",tref); */
	ret = tdat - tref;
    }
    return(ret);
}
#endif
/*
 *+
 * FUNCTION NAME:
 * initConvertTemp
 *
 * INVOCATION:
 * long initConvertTemp(struct subRecord *psub)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * struct subRecord *psub
 *
 * FUNCTION VALUE:
 *  
 *
 * PURPOSE:
 * EPICS wants an initialization routine for subroutine records even if it 
 *   doesn't do anything.
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 
 *
 *-
 */

long initConvertTemp(struct subRecord *psub)
{
    return(0);
}
/*
 *+
 * FUNCTION NAME:
 * servoCntrl
 *
 * INVOCATION:
 * long servoCntrl(struct subRecord *pSub)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * struct subRecord *pSub
 *
 * FUNCTION VALUE:
 *  
 *
 * PURPOSE:
 * turn the servo control loop on or off
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 
 *
 *-
 */

/* This sub is called when the servoCntrl record gets modified */
long servoCntrl(struct subRecord *pSub)
{
    long a;
    char buf[128];

    a = pSub->a;
    dPrint(serOn,"servoCntrl \n");
    if(a>0)
    {
	processRec(dbTop,"enableTempServo");
	sprintf(buf,"db name = %s%s\n",dbTop,"enableTempServo");
    }

    else
    {
	processRec(dbTop, "disableTempServo"); 
	sprintf(buf,"db name = %s%s\n",dbTop,"disableTempServo");
    }
    dPrint(serOn, buf);
    return OK;
}
/*
 *+
 * FUNCTION NAME:
 * cnvrtTempAo
 *
 * INVOCATION:
 * long cnvrtTempAo(struct subRecord *pSub)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * struct subRecord *pSub - pointer to subroutine record
 *
 * FUNCTION VALUE:
 *  status
 *
 * PURPOSE:
 * convert values to send to the temperature card
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 
 *
 *-
 */

/* convert from Temperature (K) or Power (mw) to DAC values (Bits)*/
long cnvrtTempAo(struct subRecord *pSub)
{
    double c,d,e,f,rawVal,v1,v2;
    long b;
    char str[80];
    
    b = pSub->b;
    rawVal = pSub->a;
    dPrint(subOn,"cnvrtTempAO \n");
    switch (b)
    {
      case 0:/* convert from foot or mount temperature to bit pattern for DAC  */
	  dPrint(subOn,"cnvrtTempAO foot\n");
	  c = pSub->c;
	  v1 = cnvtLKSInv(rawVal);
	  sprintf(str,"v1 = %.3f, rawVal = %.3f\n",v1,rawVal);
	  dPrint(subOn,str);
	  v2 = v1 * c;
	  pSub->val = v2;
	  sprintf (str,"c = %.3f, v2 = %.3f %.3f\n",c, v2, pSub->val);
	  dPrint(subOn,str);
	  break;

      case 1:/* convert from foot or mount heater Power (mw) to bit pattern for DAC */
	  dPrint(subOn,"cnvrtTempAO type 1\n");
	  c = pSub->c;/* heater resistance */
	  d = pSub->d;/* Sensor resistance */
	  e = pSub->e;/* mw->w */
	  f = pSub->f;/* bits per volt */ 
	  v1 = sqrt(((rawVal / e)* (d * d)) / c);
	  v2 = v1 * f;
	  pSub->val = v2;
	  sprintf (str,"c = %.3f, d = %.3f e = %.3f f = %.3f %.3f %.3f\n",c,d,e,f, v2, pSub->val);
	  dPrint(subOn, str);
	  break;
    }
    return OK;
}
/*
 *+
 * FUNCTION NAME:
 * convertTemp
 *
 * INVOCATION:
 * long convertTemp(struct subRecord *pSub)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * struct subRecord *pSub - pointer to subroutine record
 *
 * FUNCTION VALUE:
 *  status
 *
 * PURPOSE:
 * convert values read from temperature card to what they correspond to. (power
 *       temperature, voltage etc.)
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 
 *
 *-
 */

long convertTemp(struct subRecord *pSub)
{
    long type;
    double rawVal,c,d,e,f, g;
    double v2,v1,temp, temp1, temp2;
    char err[80];
    char str[120];
    
    type = pSub->b;
    rawVal = pSub->a;
    /*   printf("convertTemp\n"); */
  
    switch (type)
    {
      case 0:/* conversion for heater feedback power*/
	  dPrint(subOn, "convertTemp type 0\n"); 
	  c = pSub->c; /* Bits for 5.0 volts */
	  d = pSub->d; /* bits per volt */
	  e = pSub->e; /* gain term */
	  f = pSub->f; /* heater resistance*/
	  g = pSub->g; /*emitter resistance*/

	  v1 = (((rawVal-c)/d) / e); /* conversion from 4095 scale to +-5*/
	  v2 = ((v1 * v1) / (g * g)) * f * 1000.0;
	  pSub->val = v2;
	  sprintf (str,"c=%.3f, d=%.3f e=%.3f f=%.3f g=%.3f %.4f %.4f %.4f\n",c,d,e,f,g, v1, v2,pSub->val);
	  dPrint(subOn, str);
	  break;
	
      case 1:/* conversion for value loaded into DAC */
	  dPrint(subOn, "convertTemp type 1\n"); 
	  c = pSub->c; /* Bits for 5.0 volts */
	  d = pSub->d; /* bits per volt */
	  e = pSub->e; /* gain term */

	  v1 = (((rawVal-c)/d) * e);
	  pSub->val = v1;
	  sprintf (str,"c=%.3f, d=%.3f e=%.3f %.4f %.4f\n",c,d,e, v1, pSub->val);
	  dPrint(subOn, str);
	  break;
	
      case 3:/* conversion for value loaded into DAC */
	  dPrint(subOn, "convertTemp type 1\n"); 
	  c = pSub->c; /* Bits for 5.0 volts */
	  d = pSub->d; /* bits per volt */
	  e = pSub->e; /* gain term */

	  v1 = (((rawVal-c)/d) * e);
	  pSub->val = v1;
	  sprintf (str,"c=%.3f, d=%.3f e=%.3f %.4f %.4f \n",c,d,e, v1, pSub->val);
	  dPrint(subOn, str);
	  break;

      case 2:/*conversion to temperature for reference temp and low gain*/
	  /*   printf("convertTemp\n"); */
	  dPrint(subOn, "convertTemp type 2\n");
	  c = pSub->c;
	  d = pSub->d;
	  e = pSub->e;
	
	  v1 = (((rawVal-c)/d)/e);
	  temp = cnvtLKS(v1);
	  pSub->val = temp;
	  sprintf (str,"c=%.3f, d=%.3f e=%.3f %.4f %.4f %.4f\n",c,d,e, v1, temp, pSub->val);
	  dPrint(subOn, str);
	  break;
	
      case 4:/* convert to temperature for foot high gain */
	  dPrint(subOn, "convertTemp type 4\n");
	  c = pSub->c;
	  d = pSub->d;
	  e = pSub->e;
	  g = 1.0;
	
	  v1 = ((rawVal - c) / d) / e;
	  getDbInfoT(dbTop, "footRef.VAL", err, DBF_DOUBLE, &g);
	  v2 = ((g - 2047.0)/ 409.5) / 3.00;
	  
	  temp1 = cnvtLKS(v2 - v1);
	  temp2 = cnvtLKS(v2);
	  temp = (temp1 - temp2) * 1000.0;
	  /* 	printf ("temp = %.3f\n",temp); */
	  pSub->val = temp;
	  sprintf (str,"rawVal=%.3f, temp1=%.3f temp2=%.3f v1=%.3f v2=%.4f %.4f %.4f\n",rawVal ,temp1, temp2, v1, v2, temp, pSub->val);
	  dPrint(subOn, str);
	  break;
	
      case 5:/* convert to temperature for mount high gain */
 	  dPrint(subOn, "convertTemp type 5\n");
	  c = pSub->c;
	  d = pSub->d;
	  e = pSub->e;
	  g = 1.0;
	  	
	  v1 = ((rawVal - c) / d) / e;
	  getDbInfoT(dbTop, "mntRef.VAL", err, DBF_DOUBLE, &g);	
	  v2 = ((g - 2047.0)/ 409.5) / 3.0;

	  temp1 = cnvtLKS(v2 - v1);
	  temp2 = cnvtLKS(v2);
	  temp = (temp1 - temp2) * 1000.0;
	  pSub->val = temp;
	  sprintf (str,"c=%.3f, d=%.3f e=%.3f g=%.3f  %.4f %.4f %.4f %.4f\n",c,d,e, g, v1, v2, temp, pSub->val);
	  dPrint(subOn, str);

	  break;
      case 6:/**/
 	  dPrint(subOn, "convertTemp type 6\n");
	  c = pSub->c;
	  d = pSub->d;
	  v1 = (rawVal-c)/d;
	  pSub->val = v1;
	  sprintf (str,"c=%.3f, d=%.3f %.4f %.4f\n",c,d, v1, pSub->val);
	  dPrint(subOn, str);
    }
    return OK;
  
}
long iLogTemps(struct subRecord *pSub)
{
return OK;
}
/*
 *+
 * FUNCTION NAME:
 * logTemps
 *
 * INVOCATION:
 * long logTemps(struct subRecord *pSub)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * struct subRecord *pSub - pointer to subroutine record
 *
 * FUNCTION VALUE:
 *  status
 *
 * PURPOSE:
 * log current temperature values to a file
 *
 * DESCRIPTION:
 *  
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * none?
 *
 * DEFICIENCIES:
 * None
 *
 * HISTORY:
 * 
 *
 *-
 */

int pLog;
long logTemps(struct subRecord *pSub)
{
    static FILE *fileBuf = NULL;
    double a, b, c, d, e, f; /*temps to hold channel values */
    char err[80];
    
    /*   printf("convertTemp\n"); */
    
    if (fileBuf == NULL)
	if ((fileBuf = fopen("temptest", "w")) == NULL)

	{
	    cicsLogMessage(0, "fopen temptest Failed\n");
	    return ERROR;
	}

    getDbInfoT(dbTop, "footRef.VAL", err, DBF_DOUBLE, &a);
    getDbInfoT(dbTop, "footLGain.VAL", err, DBF_DOUBLE, &b);
    getDbInfoT(dbTop, "rdFootHGain.VAL", err, DBF_DOUBLE, &c);
    getDbInfoT(dbTop, "footHtrFB.VAL", err, DBF_DOUBLE, &d);
    getDbInfoT(dbTop, "mntLGain.VAL", err, DBF_DOUBLE, &e);
    getDbInfoT(dbTop, "mntHtrFB.VAL", err, DBF_DOUBLE, &f);

    fprintf(fileBuf,"fRef %.2f fLGain %.2f fHGain %.2f fHtrFB %.2f mLGain %f mHtrFB %f\n",a,b,c,d,e,f);
    fflush(fileBuf);
    if (pLog==1)
	fprintf(stderr,"fRef %.2f fLGain %.2f fHGain %.2f fHtrFB %.2f mLGain %.2f mHtrFB %.2f\n",a,b,c,d,e,f);

    return (OK);
}




/* log temperatures from console*/
int sFd = ERROR;
int LOG;
long logtemps(int time,int S,char *name)
{
    int i = 0;
  int status;
    static FILE *fileBuf = NULL;
    double a, b, c, d,b1,d1,e,e1; /*temps to hold channel values */
    char err[80],str[80];
    int port=5546; 
    struct sockaddr_in clientAddr;
    int clientFd;
    double convc,convd,conve;
    char name1[80];
    int totalTime = 0;
    LOG = 1;
       printf("convertTemp\n"); 
    while (1)
    {
	if (S)
	{
	    while (sFd == ERROR)
	    {
		sFd = sockCreate(&port);
	    }
	    printf ("sfd = %d\n",sFd);
	    sockAccept(sFd, &clientAddr, &clientFd);
	    printf("got connection\n");
	}
	totalTime = 0;
	LOG = 1;
	sprintf(name1,"%s%d.dat",name,i);
/* 	if (fileBuf == NULL) */
	    if ((fileBuf = fopen(name1, "w")) == NULL)
	    
	    {
		cicsLogMessage(0, "fopen temptest Failed\n");
		return ERROR;
	    }
    
	while (LOG == 1)
	{
	    getDbInfoT(dbTop, "footRef.VAL", err, DBF_DOUBLE, &a);
	    getDbInfoT(dbTop, "footLGain.VAL", err, DBF_DOUBLE, &b);
	    getDbInfoT(dbTop, "mntLGain.VAL", err, DBF_DOUBLE, &b1);
	    getDbInfoT(dbTop, "rdFootHGain.VAL", err, DBF_DOUBLE, &c);
	    getDbInfoT(dbTop, "footHtrFB.VAL", err, DBF_DOUBLE, &d);
	    getDbInfoT(dbTop, "mntHtrFB.VAL", err, DBF_DOUBLE, &d1);

	    getDbInfoT(dbTop, "rdMntLGain.VAL", err, DBF_DOUBLE, &e);
	    getDbInfoT(dbTop, "rdFootLGain.VAL", err, DBF_DOUBLE, &e1);
	    getDbInfoT(dbTop, "footLGainSub.C", err, DBF_DOUBLE, &convc);
	    getDbInfoT(dbTop, "footLGainSub.D", err, DBF_DOUBLE, &convd);
	    getDbInfoT(dbTop, "footLGainSub.E", err, DBF_DOUBLE, &conve);
/* 	sprintf(str,"fRef =%.2f, fLGain =%.2f, fHGain =%.2f, fHtrFB =%.2f, mLGain =%f, mHtrFB %f\n",a,b,c,d,b1,d1); */

	    e = (e-convc)/convd/conve;
	    e1 = (e1-convc)/convd/conve;
	    sprintf(str,"%d,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,%8.3f,%8.4f,%8.4f,\n",totalTime,a,b,b1,c,d,d1,e1,e);
	    if(S)
		status = sockWrite(clientFd, str, strlen(str));
	
	    fprintf(fileBuf,str);   
	    fflush(fileBuf);
	
	    if (pLog==1)
		/*  fprintf(stderr,"fRef %.2f fLGain %.2f fHGain %.2f fHtrFB %.2f mLGain %.2f mHtrFB =%.2f\n",a,b,c,d,e,f); */
		fprintf(stderr,"%d,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f\n",totalTime,a,b,b1,c,d,d1);
	    sleep(time,0);
	    totalTime +=time;
	}
      
	if(S)
	{	
	    status = sockWrite(clientFd, "end", strlen("end"));
	    sockClose(&clientFd); 
	}
 
	fclose(fileBuf);
	i++;
    }
    return (OK);
}
