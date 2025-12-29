static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: drRoiChk.c,v 1.4 2016/05/04 21:32:50 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * drRoiChk.c
 *
 * FUNCTION NAME(S)
 * drRoiChk - CAD user subroutine for the drRoiSet CAD record
 *
 *INDENT-OFF*
 * $Log: drRoiChk.c,v $
 * Revision 1.4  2016/05/04 21:32:50  gemvx
 * Use:
 * >         getDbInfoT(dbSadTop, OBSERVE_CAR ".VAL", dummy, DBF_STRING, carVAL);
 * instead of:
 * <         getDbInfoT(dbTop, OBSERVE_CAR ".IVAL", dummy,DBF_LONG,&lVal);
 *
 * .. to fix REL-2470.
 *
 * Revision 1.3  2010/08/16 20:30:15  mrippa
 * More debug level statements for drRoiChk
 *
 * Revision 1.2  2009/05/27 19:32:20  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/09/30 16:40:28  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */


/*
 *+
 * FUNCTION NAME:
 * drRoiChk
 *
 * INVOCATION:
 * struct cadRecord *pCad;
 * status = drRoiChk( pCad );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * ! pCad   (struct cadRecord *)   pointer to CAD data structure
 *
 * FUNCTION VALUE:
 * long  Status value written to CAD VAL field
 *
 * PURPOSE:
 * User defined function for "defineROI1" CAD record
 *
 * DESCRIPTION:
 * This routine is called whenever the drRoiSet CAD record is processed.
 * This is the command for defining a data reduction ROI (Region Of Interest)
 * in the data coadder software.  
 * The command has 17 arguments:
  *      numRois (a) - the number of ROIs to be defined
 *	rowLow1 (b) - the low row for ROI1
 *	colLow1 (c) - the low column for ROI1
 *	rowHigh1(d) - the high row for ROI1
 *	colHigh1(e) - the high column for ROI1
 *	rowLow2 (f) - the low row for ROI2
 *	colLow2 (g) - the low column for ROI2
 *	rowHigh2(h) - the high row for ROI2
 *	colHigh2(i) - the high column for ROI2
 *	rowLow3 (j) - the low row for ROI3
 *	colLow3 (k) - the low column for ROI3
 *	rowHigh3(l) - the high row for ROI3
 *	colHigh3(m) - the high column for ROI3
 *	rowLow4 (n) - the low row for ROI4
 *	colLow4 (o) - the low column for ROI4
 *	rowHigh5(p) - the high row for ROI4
 *	colHigh5(q) - the high column for ROI4
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
 * None
 *
 * HISTORY:
 * 17-Apr-1997  Original version adapted from CICS alpha 1.0   J.E. Tvedt
 *
 *-
 */

#include <epCommon.h>
#include <naacTasks.h>
#include <car.h>
#include "gnDCADefs.h"
extern char *dbTop;


long drRoiChk( struct cadRecord *pCad ) 
{
    static long collow1,colhigh1,rowlow1,rowhigh1;
    static long collow2,colhigh2,rowlow2,rowhigh2;
    static long collow3,colhigh3,rowlow3,rowhigh3;
    static long collow4,colhigh4,rowlow4,rowhigh4;
    char dummy[80];
    long lmin,lmax;
    long ret;
    long status = CAD_ACCEPT;         /* return status */
    static long numRoi;
    unsigned short usVal;
    short flag;
    char name[MAX_STRING_SIZE];
    
    /* Initialise CAD status */
    status = CAD_ACCEPT;

    /* Switch according to the CAD directive in DIR field */
     switch (pCad->dir)
    {
	/* CAD MARK directive detected. Nothing needs to be done.*/
      case CAD_MARK:
	  cicsLogMessage( 2, "drRoiChk - MARK directive.");
	  break;

	  /* CAD PRESET directive detected.  Check the input argument. 
	     If it is acceptable then copy it to the output.       */
      case CAD_PRESET: 
	  cicsLogMessage( 2, "drRoiChk - PRESET directive.");
	
	  /* get current arraysize from epics*/
	  lmin = 0;
	  getDbInfoT(dbTop, CUR_MAX_COL ".VAL", dummy,DBF_LONG,&lmax);
	  lmax = lmax -1;
	  

	  numRoi = atoi(pCad->a);
	  if(numRoi <0)
	    {
	      sprintf( pCad->mess, "numRois must be positive" );
	      puts(pCad->mess);
	      cicsLogMessage(2, pCad->mess);
		  status =  CAD_REJECT;
	      
	      
	    }
	  else if (numRoi>MAXROIS)
	    {

	      sprintf( pCad->mess, "numRois must be < %d",MAXROIS );
	      puts(pCad->mess);
	      cicsLogMessage(2, pCad->mess);
		  status =  CAD_REJECT;
	    }
	  else if ((numRoi >=1)&&(status == CAD_ACCEPT))
	  {
	      /*check rowLow(b) for roi1*/  
	      cicsLogString (3,"Requested rowLow1 = ",pCad->b);
	      if ((ret = check_input (pCad->ftvb,pCad->b,&lmin,&lmax,&rowlow1))!=OK)
	      {   
	          if (ret == ERROR_TYPE)
	          {
		      sprintf( pCad->mess, "1 rowLow should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "1 rowLow range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      /*check colLow(c)*/  
	      cicsLogString (3,"Requested colLow1 = ",pCad->c);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvc,pCad->c,&lmin,&lmax, &collow1))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "1 colLow should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "1 colLow range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }


	      /*check rowHigh(d)*/  
	      cicsLogString (3,"Requested rowHigh1 = ",pCad->d);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvd,pCad->d,&lmin,&lmax, &rowhigh1))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "1 rowHigh should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "1 rowHigh range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      /*check colHigh(e)*/  
	      cicsLogString (3,"Requested colHigh1 = ",pCad->e);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftve,pCad->e,&lmin,&lmax, &colhigh1))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "1 colHigh should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "1 colHigh range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      if((status == CAD_ACCEPT)&&((rowlow1>=rowhigh1)||(collow1>=colhigh1)))
	      {   
		printf("rowlow = %d, rowhigh = %d, collo = %d, colhi = %d",rowlow1,rowhigh1, collow1,colhigh1);
		  sprintf( pCad->mess, " 1 low must be < high");
		  status = CAD_REJECT;
		  cicsLogMessage(2, pCad->mess);
              } 
	      if((colhigh1 - collow1 +1 )%8 != 0)
	      {
		  printf("colhigh %d, collow %d\n",colhigh1,collow1);
		  sprintf (pCad->mess,"Number of columns must be divisible by 8\n");
		  status = CAD_REJECT;
		  cicsLogMessage(1,pCad->mess);
	      }

	  }
	  

	  if ((numRoi >= 2)&&(status == CAD_ACCEPT))
	  {
	      /*check rowLow(f) for roi2*/  
	      cicsLogString (3,"Requested rowLow2 = ",pCad->f);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvf,pCad->f,&lmin,&lmax,&rowlow2))!=OK))
	      {   
	          if (ret == ERROR_TYPE)
	          {
		      sprintf( pCad->mess, "2 rowLow should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, " 2 rowLow range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      /*check colLow(g)*/  
	      cicsLogString (3,"Requested colLow2 = ",pCad->g);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvg,pCad->g,&lmin,&lmax, &collow2))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "2 colLow should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "2 colLow range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }


	      /*check rowHigh(h)*/  
	      cicsLogString (3,"Requested rowHigh2 = ",pCad->h);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvh,pCad->h,&lmin,&lmax, &rowhigh2))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "2 rowHigh should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "2 rowHigh range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      /*check colHigh(i)*/  
	      cicsLogString (3,"Requested colHigh2 = ",pCad->i);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvi,pCad->i,&lmin,&lmax, &colhigh2))!=OK))
	      {   
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "2 colHigh should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "2 colHigh range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      if((status == CAD_ACCEPT)&&(((rowlow2>=rowhigh2)||(collow2>=colhigh2))))
	      {   
		  sprintf( pCad->mess, "2 low must be < high");
		      puts(pCad->mess);
		  status = CAD_REJECT;
		  cicsLogMessage(2, pCad->mess);
              }
	      if((colhigh2 - collow2 +1)%8 != 0)
	      {
		  sprintf (pCad->mess,"Number of columns must be divisible by 8\n");
		  status = CAD_REJECT;
		  cicsLogMessage(1,pCad->mess);
	      }
	  }


	  if ((numRoi >=3)&&(status == CAD_ACCEPT))
	  {
	      /*check rowLow(j) for roi3*/  
	      cicsLogString (3,"Requested rowLow3 = ",pCad->j);
	      if ((ret = check_input (pCad->ftvj,pCad->j,&lmin,&lmax,&rowlow3))!=OK)
	      {   
	          if (ret == ERROR_TYPE)
	          {
		      sprintf( pCad->mess, "3 rowLow should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "3 rowLow range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      /*check colLow(k)*/  
	      cicsLogString (3,"Requested colLow3 = ",pCad->k);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvk,pCad->k,&lmin,&lmax, &collow3))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "3 colLow should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "3 colLow range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }


	      /*check rowHigh(l)*/  
	      cicsLogString (3,"Requested rowHigh3 = ",pCad->l);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvl,pCad->l,&lmin,&lmax, &rowhigh3))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "3 rowHigh should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "3 rowHigh range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      /*check colHigh(m)*/  
	      cicsLogString (3,"Requested colHigh3 = ",pCad->m);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvm,pCad->m,&lmin,&lmax, &colhigh3))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "3 colHigh should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {	
		      sprintf( pCad->mess, "3 colHigh range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }
	      if((status == CAD_ACCEPT)&&(((rowlow3>=rowhigh3)||(collow3>=colhigh3))))
	      {   
		  sprintf( pCad->mess, "3 low must be < high");
		  status = CAD_REJECT;
		  cicsLogMessage(2, pCad->mess);
              } 
	      if((colhigh3 - collow3 +1)%8 != 0)
	      {
		  sprintf (pCad->mess,"Number of columns must be divisible by 8\n");
		  status = CAD_REJECT;
		  cicsLogMessage(1,pCad->mess);
	      }

	  }

	  if ((status == CAD_ACCEPT)&&((numRoi >= 4)&&(status == CAD_ACCEPT)))
	  {
	      /*check rowLow(n) for roi4*/  
	      cicsLogString (3,"Requested rowLow4 = ",pCad->n);
	      if ((ret = check_input (pCad->ftvn,pCad->n,&lmin,&lmax,&rowlow4))!=OK)
	      {   
	          if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "4 rowLow should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "4 rowLow range %4ld- %4ld",lmin,lmax );
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      /*check colLow(o)*/  
	      cicsLogString (3,"Requested colLow4 = ",pCad->o);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvo,pCad->o,&lmin,&lmax, &collow4))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "4 colLow should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "4 colLow range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }


	      /*check rowHigh(p)*/  
	      cicsLogString (3,"Requested rowHigh4 = ",pCad->p);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvp,pCad->p,&lmin,&lmax, &rowhigh4))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, " 4 rowHigh should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "4 rowHigh range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }

	      /*check colHigh(q)*/  
	      cicsLogString (3,"Requested colHigh4 = ",pCad->q);
	      if ((status == CAD_ACCEPT)&&((ret = check_input (pCad->ftvq,pCad->q,&lmin,&lmax, &colhigh4))!=OK))
	      {
		  if (ret == ERROR_TYPE)
		  {
		      sprintf( pCad->mess, "4 colHigh should be a long" );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  else 
		  {
		      sprintf( pCad->mess, "4 colHigh range %4ld- %4ld",lmin,lmax );
		      puts(pCad->mess);
		      cicsLogMessage(2, pCad->mess);
		  }
		  status =  CAD_REJECT;
	      }
	      if((status == CAD_ACCEPT)&&((rowlow4>=rowhigh4)||(collow4>=colhigh4)))
	      {   
		  sprintf( pCad->mess, "4 low must be < high");
		  status = CAD_REJECT;
		  cicsLogMessage(2, pCad->mess);
              }
	      if((colhigh4 - collow4 +1)%8 != 0)
	      {
		  sprintf (pCad->mess,"Number of columns must be divisible by 8\n");
		  status = CAD_REJECT;
		  cicsLogMessage(1,pCad->mess);
	      }
	  }

          
	
	  break;


	  /* CAD CLEAR directive detected. Nothing needs to be done. */
      case CAD_CLEAR:
	  cicsLogMessage( 2, "drRoiChk - CLEAR directive.");
	  break;

	  /* CAD START directive detected. */
      case CAD_START:
          char carVAL[32];	// WARNING: guessed at size.
 
	  cicsLogMessage( 2, "drRoiChk - START directive."); 
          getDbInfoT(dbSadTop, OBSERVE_CAR ".VAL", dummy, DBF_STRING, carVAL);
	  
	  if (carStatus(carVAL) != CAR_IDLE)
	  {
	      strcpy(MSG,"Cannot start drRoiChk while Observe is busy");
	      cicsLogMessage( 2, "!!!Cannot start drRoiChk while Observe is busy");
	      return CAD_REJECT;
	  }
	  getDbInfoT(dbTop, ARSETUP_DONE ".VAL", dummy, DBF_ENUM, &usVal);
	  if (usVal != NAAC_DONE)
	  {
	      
	      strcpy(MSG,"drRoiChk: array setup not done");
	      return  CAD_REJECT;
	  }
	  getDbInfoT(dbTop, OBSSETUP_DONE ".VAL", dummy, DBF_ENUM, &usVal);
	  if (usVal != NAAC_DONE)
	  {
	      strcpy(MSG,"drRoiChk:  observe setup not done");
	      return CAD_REJECT;
	  }
	  status=assignVal( pCad->ftva, &numRoi, pCad->vala, pCad->mess);

	  if ((numRoi>=1)&&(status == CAD_ACCEPT)) 
	  {
	      *(long *)pCad->valb = rowlow1;
	      *(long *)pCad->vald = rowhigh1;		  
	      *(long *)pCad->valc = collow1;
	      *(long *)pCad->vale = colhigh1;
	  }

	  if ((numRoi>=2)&&(status == CAD_ACCEPT)) 
	  {	  
	      *(long *)pCad->valf = rowlow2;
	      *(long *) pCad->valh = rowhigh2;
	      *(long *)pCad->valg = collow2;
	      *(long *)pCad->vali = colhigh2;
	  }

	  if ((numRoi>=3)&&(status == CAD_ACCEPT)) 
	  {	  
	      *(long *)pCad->valj = rowlow3;
	      *(long *) pCad->vall = rowhigh3;
	      *(long *)pCad->valk = collow3;
	      *(long *)pCad->valm = colhigh3;
	  }

	  if ((numRoi>=4)&&(status == CAD_ACCEPT)) 
	  {
	      *(long *)pCad->valn = rowlow4;
	      *(long *) pCad->valp = rowhigh4;
	      *(long *)pCad->valo = collow4;
	      *(long *)pCad->valq = colhigh4;
	  }
	  /* Check the status of the obsSetup command */
	  flag = naacStatus(dbTop,OBSSETUP_DONE);

	  /* If it has been started or is done, then set the drRoiSetDone
	     and drRoiSetC records to BUSY and start this task
	  */
	  if((flag == NAAC_BUSY) || (flag == NAAC_DONE))
	  {
	      sprintf(name,"%s%s.VAL",dbTop,DRROISET_DONE);
	      usVal = NAAC_BUSY;
	      status = putDbInfo(name, pCad->mess, DBF_ENUM, &usVal);

	      if(status == OK)
		  semGive(semDrRoiSet);
	      else
		  status = CAD_REJECT;
	  }

	  /* Otherwise, reject the start */
	  else
	  {
	      strcpy(pCad->mess,"obsSetup must be started or done first");
	      cicsLogMessage(1,pCad->mess);
	      cicsLogLong(1,"obsSetupDone = ", (long) flag);
	      status = CAD_REJECT;
	  }
	  /* start wcs cad*/
	usVal = 3; /* for start*/
	putDbInfoT(dbTop,"setWcsCad.DIR",dummy,DBF_ENUM,&usVal);
	  break;

	  /* CAD STOP directive detected.      */
      case CAD_STOP:
	  cicsLogMessage( 2, "drRoiChk - STOP directive. Cannot be stopped."); 
	  strcpy( pCad->mess, "Cannot be stopped.");
	  status = CAD_REJECT;
	  break;

	  /* Unrecognised CAD directive detected. This is regarded as an error. */
      default:
	  sprintf( pCad->mess, "Unrecognized CAD directive" );
	  status = CAD_REJECT;
	  break;
    }
   /*  if(status != CAD_ACCEPT) */
/*     { */
/* 	sprintf(name,"%s%s.IVAL",dbTop,DRROISET_CAR); */
/* 	lVal = CAR_ERROR; */
/* 	putDbInfo(name, dummy, DBF_LONG, &lVal); */

/* 	sprintf(name,"%s%s.IERR",dbTop,DRROISET_CAR); */
/* 	lVal = error; */
/* 	putDbInfo(name, dummy, DBF_LONG, &lVal); */
	    
/* 	sprintf(name,"%s%s.IMSS",dbTop,DRROISET_CAR); */
/* 	putDbInfo(name, dummy, DBF_STRING, errMess); */

/* 	sprintf(name,"%s%s.VAL",dbTop,DRROISET_DONE); */
/* 	lVal = NAAC_ERROR; */
/* 	putDbInfo(name, dummy, DBF_ENUM, &lVal);  */

	

/*     } */
    return status;
}








