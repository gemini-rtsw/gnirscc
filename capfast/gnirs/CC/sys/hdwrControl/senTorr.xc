
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc. 
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * senTorr.c
 *
 * DESCRIPTION
 * This file contains the EPICS device support files for the senTorr
 *   pressure monitoring hardware.
 * 
 * FUNCTION NAME(S)
 * initCCG - initialize the hardware for the cold cathode gauge
 * initTC - initialize the hardware for the senTorr or thermocouple gauge
 * readPort - Read the serial port connected to the pressure sensors
 * openSenTorrPort - open the serial port connected to the pressure sensors
 * readTC - read the senTorr or thermocouple gauge
 * readCCG - read the cold cathode gauge
 *
 * 
 * DEPENDENCIES
 * 
 *
 * $Log $
 * 
 */






#include <senTorr.h>

/* global variables*/
char dbTop[40], dbSadTop[40];
SEM_ID semSenTorr;
long pressSwitch = 0;
long serialPort;
/*
 *+
 * FUNCTION NAME:
 * initCCG
 *
 * INVOCATION:
 * static long initCCG(pai)
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  !pai - pointer to ai record
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *   Initialiaze the record that reads the varian senTorr Gauge controller
 *
 *
 * DESCRIPTION:
 *     This routine creates a mutual exclusion semaphore, Opens the serial port
 *   to the varian senTorr Gauge controller , and initializes the pai structure
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
 * October 1998  Original version adapted from CICS alpha 1.0  
 *
 * Author P. Ruckle (serial port routines from N. Roddier
 *-
 */
long initCCG(struct aiRecord *pai)
{
 char *dst;
 char str[20],response[10];



  /* open serial port*/
  if (serialPort <= 0)
  {
      if((serialPort = openSenTorrPort()) == ERROR)
	  return ERROR;
  } 
 taskDelay(2);
  /* create semaphore*/ 
  if (semSenTorr == NULL)
    semSenTorr = semBCreate(SEM_Q_PRIORITY,SEM_FULL);

  /**/
  if( pai->inp.type != INST_IO ) 
    {
      recGblRecordError(S_db_badField,(void *)pai,
			"devaoTempPort2 (init_record) Illegal out.type");  
      return(S_db_badField);
    }
  
  
  /* copy format string*/
  dst = (char *)malloc (INSTIO_FLD_SZ);
  strncpy( dst, pai->inp.value.instio.string,INSTIO_FLD_SZ );
  dst[INSTIO_FLD_SZ-1] = NULL; 
  dst[7] = 13;
 dst[8] = NULL;
  pai->dpvt = dst ; 
  
  /* send any initialization strings necessary to controller*/
  /* first there characters*/
   str[0] = dst[0]; 
   str[1] = dst[1]; 
   str[2] = dst[2]; 
  
 /* lock keypad*/
 str[3] = '2';
 str[4] = '1';  
 str[5] = 13;   
 write(serialPort,str,6);
 if( readPort(serialPort,response,2,0)!= OK)
 { 	
     pai->val = -1;
     strcpy(pai->desc,"Couldn't lock keypad");
     ioctl(serialPort,FIOFLUSH,NULL);
  /*    return ERROR; */
     
 }
 
  strcpy(pai->desc,NOTHING);
  return(OK);

}

/*
 *+
 * FUNCTION NAME:
 * initTC
 *
 * INVOCATION:
 * static long initTC(pai)
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  !pai - pointer to ai record
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *   Initialiaze the record that reads ConvectTorr sensor on the varian 
 *    senTorr Gauge controller
 *
 *
 * DESCRIPTION:
 *     This routine initializes the pai structure
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
 * October 1998  Original version adapted from CICS alpha 1.0  
 *
 * Author P. Ruckle (serial port routines from N. Roddier
 *-
 */
long initTC(struct aiRecord *pai)
{
    char *dst;
    if( pai->inp.type != INST_IO ) 
    {
	recGblRecordError(S_db_badField,(void *)pai,
			  "devaoTempPort2 (init_record) Illegal out.type");  
	return(S_db_badField	);
    }	
    
  
    /* copy format string*/
    dst = (char *)malloc (INSTIO_FLD_SZ);
    strncpy( dst, pai->inp.value.instio.string,INSTIO_FLD_SZ );
    dst[7] = 13;
    dst[8] = NULL;
    dst[INSTIO_FLD_SZ-1] = NULL; 
  
    pai->dpvt = dst ; 
    return OK;
}


/*
 *+
 * FUNCTION NAME:
 * readPort
 *
 * INVOCATION:
 * static long readPort(intport,char *msg,int len,int time)
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  >int port - port to read
 *  <char *msg - strint that was read
 *  >int len - desired number or characters
 *  >int time - time to wait for response
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *     Read the return message from the serial port
 *   
 *
 *
 * DESCRIPTION:
 *    Read the serial port.  If less than len characters are read before
 *   a time out is reached, an error is returned.  Or if a ? is read as the
 *   first character, an error is returned.  
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
 * October 1998  Original version adapted from CICS alpha 1.0  
 *
 * Author P. Ruckle (serial port routines from N. Roddier
 *-
 */

long readPort(int port,char *msg,int len,int time)
{
  int n = 0;
 
  if(time == 0)
      time = tickGet() + 20*sysClkRateGet();
  /* check number of bytes to be read until there are len or a timeout occurs*/
  while ((n<len) && (tickGet()<time))
    {
      taskDelay(1);
      ioctl(port, FIONREAD, (int)&n);
    }
  if(n>0) /* something was read*/
    {
      if (n<len) /* parital message*/
	{
	  read(port,msg,n);
	  if (msg[1] == '?') /* error*/
	    cicsLogMessage(0,"Pressure sensor returned error");
	  return ERROR;
	}
      else /* full message*/
	{
	  /*read response*/
	  read(port,msg,len);
	  msg[len] = NULL;
	}
    }
  else /*no characters read time out*/ 
    {
      cicsLogMessage(0,"no response from pressure sensor\n");
      return ERROR;
    }
  return OK;
}
/*
 *+
 * FUNCTION NAME:
 * openSenTorrPort
 *
 * INVOCATION:
 * static long openSenTorrPort()
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  none
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *    Open the serial port to the senTorr gauge controller
 *
 *
 * DESCRIPTION:
 *    Open the serial port on the 68040 that is connected to the senTorr
 *     gauge controller and set its baud rate.
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
 * October 1998  Original version adapted from CICS alpha 1.0  
 *
 * Author P. Ruckle (serial port routines from N. Roddier
 *-
 */

long openSenTorrPort()
{ /*open serial port*/
  long port;
  if ((port = open("/tyCo/1", UPDATE, 0)) == 0)
    { 
      cicsLogMessage(0,  "Opening Temp PORT failed");
      
      serialPort = 0;
      return(ERROR);
    }
  /*set baud rate*/
  if (ioctl(port, FIOBAUDRATE, BAUD) == ERROR)
    { 
      cicsLogMessage(0, "Setting baud rate for Temp PORT failed");
      port = 0;
      return(ERROR);
    }
  return port;
}
#if 0
int setdtr(int dtr,long port)
{
  unsigned char *Padd;
  char tmp;


  Padd = (unsigned char *)(0xFFF45001);
  *Padd = SCC_WR0_SEL_WR5;       
  tmp = SCC_WR5_TX_EN | SCC_WR5_TX_8_BITS | SCC_WR5_RTS;
  if (dtr == 1)
    {
      *Padd = SCC_WR5_DTR | tmp;
      tmp = 0x0D;
      write(port, &tmp, 1);
    }
  else
    *Padd = tmp;
     

  
  return(0);
}
#endif/*
 *+
 * FUNCTION NAME:
 * readTC
 *
 * INVOCATION:
 * static long readTC(struct aiRecord *pai)
 * !struct aiRecord *pai
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *   device support for the convectTorr or tc sensors
 *
 *
 * DESCRIPTION:
 *    Send a request across the serial port and wait for a response
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
 * October 1998  Original version adapted from CICS alpha 1.0  
 *
 * Author P. Ruckle (serial port routines from N. Roddier
 *-
 */

long readTC(struct aiRecord *pai)
{
  char result[20];
  double P;

   if(semTake(semSenTorr,SEM_TIMEOUT) == OK)
   {
       if(write(serialPort,pai->dpvt,STR_LEN_OUT) != STR_LEN_OUT)
       {
	   cicsLogMessage(0,"write error to thermocouple\n");
	   semGive(semSenTorr);
	   return ERROR;
       }
  
       strcpy(pai->desc,NOTHING);
       if(readPort(serialPort,result,STR_LEN_IN,0)!= OK)
       {
	   pai->val = -6;
	   strcpy(pai->desc,"Couldn't read thermocouple");
	   ioctl(serialPort,FIOFLUSH,NULL);
	   cicsLogMessage(0,"read error to thermocouple\n");
	   semGive(semSenTorr);
	   return ERROR;
       }

       P = (double)atof(&result[1]);/* skip first character, it is non numeric*/
       semGive(semSenTorr);	
   }
   else
    { 
      cicsLogMessage(0,"tc Couldn't take semaphore\n");
      return ERROR;
    }
  
 pai->val = P;
 return 2;
}/*
 *+
 * FUNCTION NAME:
 *  readCCG
 *
 * INVOCATION:
 * static long readCCG(struct aiRecord *pai)
 * 
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *  !struct aiRecord *pai
 *
 * FUNCTION VALUE:
 *   status
 *
 * PURPOSE:
 *   Device support for reading the ccg sensor
 *
 *
 * DESCRIPTION:
 *    Check the pressures on the convectTorr or tc sensors.  If the pressure
 *  is low enough, check the pressure on the ccg.
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
 * October 1998  Original version adapted from CICS alpha 1.0  
 *
 * Author P. Ruckle (serial port routines from N. Roddier
 *-
 */

long readCCG(struct aiRecord *pai)
{
    int emis;
  long status = OK;
  double p1=760,p2=760,p3 =760;
  char message[80];
  char errMess[80];
  char result[20];


  /* read thermocouble sensors*/
   processRec("", THERMOCOUPLE1); 
  if(getDbInfo( THERMOCOUPLE1,errMess,DBF_DOUBLE,&p1) == ERROR)
  { 
    cicsLogMessage(0,"tc1 returned error\n");
    return ERROR;
  }
  processRec("", THERMOCOUPLE2); 
  if(getDbInfo(THERMOCOUPLE2,errMess,DBF_DOUBLE,&p2)== ERROR)
    { 
      cicsLogMessage(0,"tc2 returned error\n");
      return ERROR;
    }
 


  if(semTake(semSenTorr,SEM_TIMEOUT) == OK)
    {
      switch (pressSwitch)
	{
	case 1: /* ignore sensor 1*/
	  break;
	case 2:/* ignore sensor 2*/
	  break;
	case 3:/* average sensors*/
	  break;
	case 4:/* use highest value*/
	  break;
	case 5:/* use lowest value*/
	  break;
	case 0:
	default:   /* return error if high pressure sensors > EPSILON difference*/
	  if(fabs(p1-p2)>EPSILON)
	    {
	      cicsLogMessage(0,"pressure values are different\n");
	      status = ERROR;
	    }
	  
	  if((p1 > THRESHOLD)||(p2 > THRESHOLD)||(status != OK)
	     ||(p1==0)||(p2==0))
	    {
	      p3 = (p1+p2)/2;
	    }
	  else
	    {
  

	      /* check emission status*/
	      write(serialPort,message,STR_LEN_OUT); 
	      if(readPort(serialPort,result,4,0) != OK)
		{
		  pai->val = -5;
		  strcpy(pai->desc,"emissions status error");
		  ioctl(serialPort,FIOFLUSH,NULL);
		  semGive(semSenTorr);
		  return ERROR;
		}	
	      printf(" emis status  result = %s\n",result);
	      emis = atoi(&result[2]);


	      /* if emissions are off turn it on and wait for it to 
		 come on*/
	      if(!emis)
		{
		  /* turn on emision*/  
		  strcpy(message,pai->dpvt); 	
		  message[3] = '3' ; message[4] = '1';
		  message[5] = 'I';  message[6] = '1';
		  message[7] = 13;
		  
		  write(serialPort,message,STR_LEN_OUT); 
		  if(readPort(serialPort,result,2,0) != OK)
		    {
		      pai->val = -5;
		      strcpy(pai->desc,"couldn't set emissions on");
		      ioctl(serialPort,FIOFLUSH,NULL);
		      semGive(semSenTorr);
		      return ERROR;
		    }	


		  /*  emision status*/  
		  strcpy(message,pai->dpvt); 	
		  message[3] = '3' ; message[4] = '2';
		  message[5] = 'I';  message[6] = '1';
		  message[7] = 13;
		  emis = 0;
		  /* loop until emission status is on*/
		  while (!emis )
		    {
		      taskDelay(500);
		      write(serialPort,message,STR_LEN_OUT); 
		      if(readPort(serialPort,result,4,0) != OK)
			{
			   pai->val = -5;
			   strcpy(pai->desc,"emissions status error");
			   ioctl(serialPort,FIOFLUSH,NULL);
			   semGive(semSenTorr);
			   return ERROR;
			}	
		      printf(" emis status  result = %s\n",result);
		      emis = atoi(&result[2]);
		    }

		}



		/* read ccg sensor*/
		strcpy(message,pai->dpvt);
		message[7] = 13; 
		write(serialPort,message,STR_LEN_OUT);
	    
		strcpy(pai->desc,NOTHING);
		if(readPort(serialPort,result,STR_LEN_IN,0) != OK)
		  {
		    pai->val = -5;
		    strcpy(pai->desc,"Couldn't read ccg");
		    ioctl(serialPort,FIOFLUSH,NULL);
		    status = ERROR;
		  }  
		p3 = (double)atof(&result[1]);



		/* set emission off*/
		strcpy(message,pai->dpvt); 
		message[3] = '3' ;message[4] = '0';
		message[5] = 'I';message[6] = '1';
		message[7] = 13;
	 	write(serialPort,message,STR_LEN_OUT); 
		if(readPort(serialPort,result,2,0) != OK)
		  {
		    pai->val = -5;
		    strcpy(pai->desc,"couldn't set emissions off");
		    ioctl(serialPort,FIOFLUSH,NULL);
		    semGive(semSenTorr);
		    return ERROR;
		  }
	     
	    }
	   

	  
	}
      semGive(semSenTorr);
    }
  else
    { 
      cicsLogMessage(0,"ccg Couldn't take semaphore\n");
      return ERROR;
    }
  
  pai->val = p3;
  if (status == ERROR)
    {
      ioctl(serialPort,FIOFLUSH,NULL);
      return ERROR;
    }
  return 2;	
  
}
