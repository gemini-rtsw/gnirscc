static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: obsSetupChk.c,v 1.5 2016/05/04 21:33:28 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * obsSetupChk.c
 *
 * DESCRIPTION
 * This file contains the functions for the user subroutine
 * of the CAD record obsSetup.
 * 
 * FUNCTION NAME(S)
 * obsSetupChk - validates the input parameters to the CAD record
 * 
 * DEPENDENCIES
 * Changes to the CAD record properties must be reflected
 * in this software.  
 *
 *INDENT-OFF*
 * $Log: obsSetupChk.c,v $
 * Revision 1.5  2016/05/04 21:33:28  gemvx
 * Fix indentation
 *
 * Revision 1.4  2010/08/17 02:09:57  mrippa
 * *** empty log message ***
 *
 * Revision 1.3  2010/08/16 20:51:01  mrippa
 * formatting
 *
 * Revision 1.2  2009/05/27 19:32:21  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.2  1998/11/20 17:13:51  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:29  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */



#include <epCommon.h>
#include <naacTasks.h>
#include <car.h>

extern char *dbTop;
double calcMinInt(long arSizeVal, long DAvgs, long Lnrs);
/*
 *+
 * FUNCTION NAME:
 * obsSetupChk
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = obsSetupChk( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "obsSetup" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the obsSetup CAD record is processed.
 * obsSetup is the command for setting up an observation.  The command has 
 * has the following arguments as CAD inputs.  All CAD inputs are
 * restricted to strings.
 * (a) seqRoiSize: size of square to use centered on the array
 * (b) detState: whether the detector is active or not
 * (c) numDAvgs: number of digital averages
 * (d) numLNRs: number of low noise reads
 * (e) numcoAdds: number of co-adds
 * (f) numPics: number of pictures
 * (g) reqIntTime: requested integration time
 * (h) hdrDetail: header detail
 * (i) title: title to include with the data
 * (j) seqNum: sequence number to include with the data
 * (k) comment: comment to include with the data
 * (l) hkState: state of housekeeping data generation
 * (m) procMode: processing mode to use
 * (n) hdrTiming: time(s) at which header data should be collected
 *
 * This routine will produce the following CAD outputs.
 * (vala) seqRoiSize: size of square to use centered on the array
 * (valb) detState: whether the detector is active or not
 * (valc) numDAvgs: number of digital averages
 * (vald) numLNRs: number of low noise reads
 * (vale) numcoAdds: number of co-adds
 * (valf) numPics: number of pictures
 * (valg) reqIntTime: requested integration time
 * (valh) hdrDetail: header detail
 * (vali) title: title to include with the data
 * (valj) seqNum: sequence number to include with the data
 * (valk) comment: comment to include with the data
 * (vall) hkState: state of housekeeping data generation
 * (valm) procMode: processing mode to use
 * (valn) hdrTiming: time(s) at which header data should be collected
 * (valo) seqFDly: delay between frames in the sequencer ?
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * It is assumed the CAD record has already been initialized and the
 * directive and any arguments have already been assembled into the cadRecord
 * data structure.
 *
 * DEFICIENCIES:
 * CAD inputs are restricted to the maximum string size within EPICS.
 *
 * HISTORY (optional):
 * 24-Mar-1997  Original version adapted from CICS alpha 1.0   Janet Tvedt
 *
 *-
 */
long obsSetupChk( struct cadRecord *pCad)
{
	int frames;
    int framesPerCycle;
  int n;
    long status;         /* return status */
    int ret;
    char name[MAX_STRING_SIZE], buf2[80];
   static  long seqRoiSize=0,numDAvgs=0,numLNRs=0,numcoAdds=0,numPics=0,seqNum=0;
    static int hdrDetail, detState, hkState, procMode, hdrTiming;
    static double reqIntTime=0, seqFDly=0, seqIntTime=0, minInt=0;
    unsigned short usVal=0;
    long arSizeVal = 0;
    long lVal;
    int error;
    char dummy[80];
    short flag;

    /* Initialise CAD status */
    status = CAD_ACCEPT;
	strcpy (pCad->mess,"");
    /* Switch according to the CAD directive in DIR field */
    switch (pCad->dir)
    {
       /* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	  cicsLogMessage( 2, "obsSetupChk - MARK directive.");
	  break;

	  /* CAD PRESET directive detected.  Check the input argument. 
	     If it is acceptable then copy it to the output.       */
      case CAD_PRESET: 
	  cicsLogMessage( 2, "obsSetupChk - PRESET directive.");
	 
	   
	    

	  /* Check seqRoiSize - should be 128,256,384,512,768,1024 */
	  cicsLogString(3, "obsSetupChk:Requested seqRoiSize = ", pCad->a);
	  ret = sscanf(pCad->a,"%ld",&seqRoiSize);
	  if(ret != 1)
	  {
	      status = CAD_REJECT;
	      strcpy(pCad->mess,"obsSetupChk:Invalid seqRoiSize - need integer");
	      cicsLogMessage(2, pCad->mess);
	  }
	  switch(seqRoiSize)
	  {
	    case 256:
	      arSizeVal = 256;
	      break;
	    case 512:
	      arSizeVal = 512;
	      break;
	    case 1024:
	      arSizeVal = 1024;
	      break;
	    case 768:
	      arSizeVal = 768;
	      break;
	    case 128:
	    case 384:
	    default:
	      status = CAD_REJECT;
	      sprintf(pCad->mess,"obsSetupChk:Invalid value for seqRoiSize");
	      cicsLogMessage(2, pCad->mess);
	      cicsLogMessage(2, "obsSetupChk:Need 256, 512 or 1024");
	  }
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk: Requested procMode = ",pCad->m);
	      if( strcmp(pCad->m, "STARE") == 0)
		  procMode = STARE;  
	      else if (strcmp(pCad->m, "SEP") == 0) 
		  procMode = SEP;
	      else if (strcmp(pCad->m, "CHOP") == 0)
		  procMode = CHOP;
	      else if (strcmp(pCad->m, "CHOP3") == 0)
		  procMode = CHOP3;
	      else if (strcmp(pCad->m, "TEST") == 0)
		  procMode = TEST;
	      else
	      {
		  status = CAD_REJECT;
		  sprintf( pCad->mess, "obsSetupChk: Invalid procmode %s\n",pCad->m);
		 /*  cicsLogMessage(0, pCad->mess); */
/* 		  cicsLogMessage(0,"obsSetupChk: Expecting: stare, sep, chop, chop3, or test"); */
	      }	  
	  }
	  /* Check detState - should be "0" or "1" */
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString(3, "obsSetupChk:Requested detState = ", pCad->b);
	      if(strcmp(pCad->b,"0") == 0) 
	      {
		  detState = DEACTIVATED;
	      }
	      else if(strcmp(pCad->b,"1") == 0) 
	      {
		  detState = ACTIVATED;
	      }
	      else
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess,"obsSetupChk:Invalid value for detState-need 0 or 1");
		  cicsLogMessage(2, pCad->mess);
	      }
	  }


	  /*check numDAvgs - should be power of 2 up to 16 */  
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk:Requested numDAvgs = ",pCad->c);
	      ret = sscanf(pCad->c,"%ld",&numDAvgs);
	      if(ret != 1)
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess,"obsSetupChk:Invalid numDAvgs - need integer");
		  cicsLogMessage(2, pCad->mess);
	      }
	      if(!pow2(numDAvgs,&n) || (numDAvgs < 1) || (numDAvgs > 16))
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess, "obsSetupChk:Invalid value for numDAvgs");
		  cicsLogMessage(2,pCad->mess);
		  cicsLogMessage(2,"obsSetupChk:Expecting power of 2 upto 16");
	      }
	  }

	  /*check numLNRs - should be 2 upto 3128*/  
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk:Requested numLNRs = ",pCad->d);
	      ret = sscanf(pCad->d,"%ld",&numLNRs);
	      if(ret != 1)
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess,"obsSetupChk:Invalid numLNRs - need integer");
		  cicsLogMessage(2, pCad->mess);
	      }
	      if( (numLNRs < 1) || (numLNRs > 128))
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess, "obsSetupChk:Invalid value for numLNRs");
		  cicsLogMessage(2,pCad->mess);
		  cicsLogMessage(2,"obsSetupChk:Expecting power of 2 upto 32");
	      }
	    
	  }

	  /*check numcoAdds - should be greater than or equal to 1 */  
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk:Requested numcoAdds = ",pCad->e);
	      ret = sscanf(pCad->e,"%ld",&numcoAdds);
	      if(ret != 1)
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess,"obsSetupChk:Invalid numcoAdds - need integer");
		  cicsLogMessage(2, pCad->mess);
	      }
	      if(numcoAdds < 1)
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess, "obsSetupChk:Invalid value for numcoAdds");
		  cicsLogMessage(2,pCad->mess);
		  cicsLogMessage(2,"obsSetupChk:Expecting value >= 1");
	      }
	  }
	  if(status == CAD_ACCEPT)
	    {
/* 	      status = getDbInfoT(dbTop, ROW_HI ".VAL", buf2, DBF_LONG,  */
/* 				  &arSizeVal); */
	      minInt = calcMinInt(arSizeVal, numDAvgs, numLNRs);	
	    }
	  /*check numPics - should be greater than or equal to 1 */  
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk:Requested numPics = ",pCad->f);
	      ret = sscanf(pCad->f,"%ld",&numPics);
	      if(ret != 1)
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess,"obsSetupChk:Invalid numPics - need integer");
		  cicsLogMessage(2, pCad->mess);
	      }
	      
	      if(numPics < 1)
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess, "obsSetupChk:Invalid value for numPics");
 		  cicsLogMessage(0,pCad->mess); 
 		  cicsLogMessage(0,"obsSetupChk:Expecting value >= 1");
	      }
	  }

	  /*check reqIntTIme - criteria TBD */  
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk:Requested intTime = ",pCad->g);
	      ret = sscanf(pCad->g,"%lf",&reqIntTime);
	   
	      if(ret != 1)
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess,"obsSetupChk:Invalid reqIntTime - need float");
		  cicsLogMessage(2, pCad->mess);
	      }
	      else /* check against minInt*/
	      {
		  if (minInt > reqIntTime + .001)
		  {  
		      error = -1;
		       status = putDbInfoT(dbTop,"intOK", pCad->mess, DBF_LONG, &error);
		       status = CAD_REJECT; 
		      sprintf( pCad->mess, "obsSetupChk:Desired integration must be greater than required integration time");
		       cicsLogMessage(2, pCad->mess);
		     /*  seqIntTime = 0; */
		  }
		  else 
		  {
		      error = -0;
		       status = putDbInfoT(dbTop,"intOK", pCad->mess, DBF_LONG, &error);
		      seqIntTime = reqIntTime +.001 - minInt;     
		  }
  
	      }
	      /* Calculate seqFDly */
	      if(status == CAD_ACCEPT)
		seqFDly = 0.0;

	      if(0) /* criteria for rejection needs to go here */
	      {
		  status = CAD_REJECT;
		  strcpy(pCad->mess, "Invalid value for reqIntTime");
		  cicsLogMessage(2,pCad->mess);
		  cicsLogMessage(2,"Expecting value ???");
	      }
	  }


	  /* check header detail */
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk:Requested hdrDetail = ",pCad->h);
	      if( strcmp(pCad->h, "NORMAL") == 0)
		  hdrDetail = STDHEAD;   /*  STDHEAD = 0 */
	      else if (strcmp(pCad->h, "PARTIAL") == 0) 
		  hdrDetail = MIDHEAD;    /*  MIDHEAD  = 2 */
	      else if (strcmp(pCad->h, "FULL") == 0) 
		  hdrDetail = MAXHEAD;    /*  MAXHEAD  = 3 */
	      else
	      {
		  status = CAD_REJECT;
		  sprintf( pCad->mess, "obsSetupChk:Invalid header detail level");
		  cicsLogMessage(2, pCad->mess);
		  cicsLogMessage(2,"obsSetupChk:Expecting: normal, partial, full");
	      }
	  }

	  /* Accept any title */
	  if(status == CAD_ACCEPT)
	      cicsLogString(3, "Requested Title = ", pCad->i);

	  /* Allow anything for sequence number */
	  if(status == CAD_ACCEPT)
          {
	      cicsLogString(3, "obsSetupChk:Requested Sequence Number = ", 
			    pCad->j);
	      sscanf(pCad->j,"%ld",&seqNum);
	  }

	  /* Allow any comment */
	  if(status == CAD_ACCEPT)
	      cicsLogString(3, "obsSetupChk:Requested Comment = ", pCad->k);

		
          /* Check hkState - should be "freeze" or "unfreeze" */
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk:Requested hkState = ",pCad->l);
	      if( strcmp(pCad->l, "OFF") == 0)
	      {
		  hkState = FREEZE;   
	      }
	      else if (strcmp(pCad->l, "ON") == 0) 
	      {
		  hkState = UNFREEZE; 
	      }
	      else
	      {
		  status = CAD_REJECT;
		  sprintf( pCad->mess, "obsSetupChk: Invalid housekeeping state");
		  cicsLogMessage(2, pCad->mess);
		  cicsLogMessage(2,"obsSetupChk: Expecting: OFF, ON");
	      }	  
	  }
	  status |= getDbInfoT(dbTop, UC_FRMSPCYCLE ".VAL",
			       buf2,DBF_LONG, &framesPerCycle);

          /* Check procMode - should be stare, sep, chop, chop3, or test */
	  switch(arSizeVal)
	  {
		case 1024:
		  frames = 16; 
		  break;
		case 768:
		  frames = 20;
		  break;
		case 512:
		  frames = 32;
		  break;
		case 256:
		  frames = 64;
		  break;
	  }
	  printf("lnr = %d, fpc = %d, coads = %d, total = %d\n",numLNRs, framesPerCycle, numcoAdds, numLNRs*framesPerCycle*numcoAdds);
	  if((procMode == SEP)&&(numLNRs*framesPerCycle*numcoAdds > frames))
		{
		  status = CAD_REJECT;
		  printf("too many frames\n");
		  sprintf(pCad->mess,"obsSetupChk: frames<%d for  %d\n",frames,arSizeVal);
		 
		}


		/* Check hdrTiming - should be "before", "after" or "both" */
	  if(status == CAD_ACCEPT)
	  {
	      cicsLogString (3,"obsSetupChk: Requested hdrTiming = ",pCad->n) ;
	      if( strcmp(pCad->n, "BEFORE") == 0)
	      {
		  hdrTiming = BEFORE;  
	      }
	      else if (strcmp(pCad->n, "AFTER") == 0) 
	      {
		  hdrTiming = AFTER;
	      }
	      else if (strcmp(pCad->n, "BOTH") == 0) 
	      {
		  hdrTiming = BOTH;
	      }
	      else
	      {
		  status = CAD_REJECT;
		  sprintf( pCad->mess, "obsSetupChk: Invalid header timing");
		  cicsLogMessage(2, pCad->mess);
		  cicsLogMessage(2,"obsSetupChk: Expecting: BEFORE, AFTER or BOTH");
	      }	  
	  }
#if 0
	  if (status != CAD_ACCEPT)
	  {
	      setCar();
 	      lVal = CAR_ERROR;
 	      putDbInfoT(dbTop, OBSSETUP_CAR ".IVAL", dummy, DBF_LONG, &lVal);

 	      lVal = CAR_ERROR;
 	      putDbInfoT(dbTop, OBSSETUP_CAR ".IERR", dummy, DBF_LONG, &lVal);
		      

	      usVal = NAAC_ERROR;
	      putDbInfoT(dbTop, OBSSETUP_DONE ".VAL", dummy, DBF_ENUM, &usVal);
	  }
#endif
	  break;
	  
	  /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	cicsLogMessage( 2, "obsSetupChk - CLEAR directive.");
	break;

	  /* CAD START directive detected. Do nothing. The directive is being
	     monitored by the sequence code, which will start the appropriate
	     action.       */
      case CAD_START:
	cicsLogMessage( 2, "obsSetupChk - START directive.");
	getDbInfoT(dbTop, OBSERVE_CAR ".VAL", dummy,DBF_LONG,&lVal);
	if (lVal != CAR_IDLE)
	{
	  sprintf(pCad->mess,"Can't start observeChk command while observing");
	  return CAD_REJECT;
	}


	getDbInfoT(dbTop, ARSETUP_DONE ".VAL", dummy, DBF_ENUM, &usVal);
	if (usVal != NAAC_DONE)
	{
	  printf("arSetupDone = %d\n",usVal);
	  sprintf(pCad->mess,"Array setup is not done yet.");
	  return  CAD_REJECT;
	}
	if(status == CAD_ACCEPT)
	{
	  status = assignVal(pCad->ftva, &seqRoiSize, pCad->vala,pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftvb,&detState, pCad->valb, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftvc,&numDAvgs, pCad->valc, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status =  assignVal(pCad->ftvd, &numLNRs, pCad->vald, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftve,&numcoAdds, pCad->vale, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftvf,&numPics, pCad->valf, 
			  pCad->mess);
	  if(status == CAD_ACCEPT)
		 status = assignVal(pCad->ftvg,&seqIntTime, pCad->valg, 
			  pCad->mess);
	  if(status == CAD_ACCEPT)
		 status = assignVal(pCad->ftvh,&hdrDetail, pCad->valh, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftvi,pCad->i, pCad->vali, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftvj,&seqNum, pCad->valj, 
			  pCad->mess);
	  if(status == CAD_ACCEPT)
		 status = assignVal(pCad->ftvk,pCad->k, pCad->valk, 
			  pCad->mess);
	  if(status == CAD_ACCEPT)
		 status = assignVal(pCad->ftvl,&hkState, pCad->vall, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftvm,&procMode, pCad->valm, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftvn,&hdrTiming, pCad->valn, 
			  pCad->mess);
	  if(status == CAD_ACCEPT) 
		 status = assignVal(pCad->ftvo,&seqFDly, pCad->valo, 
			  pCad->mess);
	}


	/* Check the status of the arSetup command */
	flag = naacStatus(dbTop,ARSETUP_DONE);

	/* If it has been started or is done, then set the DONE record
		to BUSY and start this task 	  */
	if((flag == NAAC_BUSY) || (flag == NAAC_DONE))
	{
	  sprintf(name,"%s%s.VAL",dbTop,OBSSETUP_DONE);
	  usVal = NAAC_BUSY;
	  status = putDbInfo(name, pCad->mess, DBF_ENUM, &usVal);
	  if(status == OK)
		 semGive(semObsSetup);
	  else
		 status = CAD_REJECT;
	}

	/* Otherwise, reject the start */
	else
	{
	  strcpy(pCad->mess,"obsSetupChk: arSetup must be started or done first");
	  cicsLogMessage(1,pCad->mess);
	  cicsLogLong(1,"obsSetupChk: arSetupDone = ", (long) flag);
	  status = CAD_REJECT;
	}

	break;
	/* CAD STOP directive detected.      */
		case CAD_STOP:
	cicsLogMessage( 1, "obsSetupChk - STOP directive. Cannot be stopped.");
	strcpy(pCad->mess,"obsSetupChk: Cannot be stopped");
	status = CAD_REJECT;
	break;

	/* Unrecognised CAD directive detected. This is regarded 
		as an error. */
		default:
	sprintf( pCad->mess, "obsSetupChk:  Unrecognized CAD directive" );
	status = CAD_REJECT;
	break;
	 }

    return status;
}



