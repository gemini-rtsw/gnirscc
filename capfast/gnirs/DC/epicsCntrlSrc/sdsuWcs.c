static struct
  {
      void *v;
      char *c;
  }
rcsid =
{
    &rcsid,
        "$Id: sdsuWcs.c,v 1.8 2011/08/04 21:42:49 gemvx Exp $"
};
#define DEBUG
/*
 * Copyright 1999 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME:
 * Wcs.c
 *
 * FUNCTION NAME(S):
 *  getWcsPoints   -   Reads WCS data from a file into memory 
 *  CalcWcs   -   Generates header values for current observation and current WCS 
 *  InitWcs   -   Sets WCS information from input files 
 *  WcsContext   -   Records WCS context information from EPICS layer 
 *  wcsCalibrate   -   Generate WCS data from data points and WCS EPICS information 
 *
 *INDENT-OFF*
 * $Log: sdsuWcs.c,v $
 * Revision 1.8  2011/08/04 21:42:49  gemvx
 * removed unused variables
 *
 * Revision 1.7  2010/11/27 03:25:01  mrippa
 * Added globalWcsnpoints to be used in selectWcs cad.
 *
 * Revision 1.6  2010/11/17 00:57:53  mrippa
 * Remove some verbose debug statements.
 *
 * Revision 1.5  2010/09/02 00:38:00  mrippa
 * Current version disables TCS which was causing CAC to
 * crash. Test by setting myDisableTCS=1 on the DC console,
 * then running getWcsParams().
 *
 * Revision 1.4  2010/08/16 20:55:56  mrippa
 * New util routines to show WCS xform and header.
 *
 * Revision 1.3  2010/08/02 05:20:06  mrippa
 * New WCS routines look at Camera and AG port
 * for WCS file definition.
 *
 * Revision 1.2  2009/05/27 19:32:22  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 *INDENT-ON*
 *-
 */

#include <vxWorks.h>
#include <stdio.h>
#include <fcntl.h>
#include <ioLib.h>
#include <vme.h>
#include <memLib.h>
#include <usrLib.h>             /* Debugging */
#include <cacheLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <intLib.h>
#include <logLib.h>
#include <iv.h>
#include <vxLib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "epCommon.h"
#include "gnDCADefs.h"
#include "localWcs.h"

#include "timeLib.h"
#include "slalib.h"

#include "astLib.h"


/* Define global constants */

#define  NPOINTS            100 /* Number of measured points             */
                                 /* (must be at least 3).                 */
#define  MATRIXSIZE         6   /* Size of transformation matrix         */
                                 /* (always 6).                           */
gmosChip wcsInfo[NUM_CHIPS];
/* Define global variables */

/* wcs holds information obtained from telescope context and time.
 * It is used to transform the cij into the local coordinate system.
 */
static struct WCS wcs;               /* Basic TCS World Coordinate System.  */

static FRAMETYPE trackFrame;         /* TCS track frame.                    */
static struct EPOCH trackEquinox;    /* TCS track equinox.                  */
static double timeTAI;               /* International atomic time in secs.  */

/* the output of the wcs calculations consists of the following 13 items */
static char ctype1[81];              /* World Coordinate System projection  */
                                     /* type for axis 1.                    */
static double crpix1;                /* Pixel coordinate reference for      */
                                     /* axis 1.                             */
static double crval1;                /* World coordinate reference for      */
                                     /* axis 1.                             */
static char ctype2[81];              /* World Coordinate System projection  */
                                     /* type for axis 2.                    */
static double crpix2;                /* Pixel coordinate reference for      */
                                     /* axis 2.                             */
static double crval2;                /* World coordinate reference for      */
                                     /* axis 2.                             */
static double cd1_1;                 /* xi rotation/skew matrix element.    */
static double cd1_2;                 /* xj rotation/skew matrix element.    */
static double cd2_1;                 /* yi rotation/skew matrix element.    */
static double cd2_2;                 /* yj rotation/skew matrix element.    */
static char radecsys[81];            /* Type of RA/Dec (for celestial       */
                                     /* coordinates).                       */
static double equinox;               /* Epoch of mean equator & equinox     */
                                     /* (celestial  coords).                */
static double mjdobs;                /* Modified Julian Date.               */
long globalWcsnPts = 0;

/* external variables*/
extern char *dbTop;
extern long TCS;
extern char ErrorMessage[120];

/*function prototypes*/
long printctx(struct WCS_CTX ctx);
STATUS getROI(roiParams *roi);
int InitWcs(int, char *, char *, char *);
int WcsContext(struct WCS *, int, struct EPOCH *, double);
/*int CalcWcs(int, int);*/
int getWcsPoints(int, char *);
int wcsCalibrate(int, int, int, int, int);
long printTel(struct TELP tel);
/*long getWcsParams();*/
int mytimeOffline ( double tai,
                  double elong, double phi, double hm,
                  double dleap, double dat, double dut );
STATUS getEpics (char* name, void *val,int count,void **tmp);


/*
 *+
 * FUNCTION NAME:
 * InitWcs
 *
 * INVOCATION:
 * InitWcs(file1, file2, file3)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) file1 (char *)
 * (>) file2 (char *)
 * (>) file3 (char *)
 *
 * FUNCTION VALUE:
 * (int) Returns OK (0) or ERROR (1) as status
 *
 * PURPOSE:
 * Sets WCS information from input files
 * * DESCRIPTION:
 * Get the three calibration files for the wcs and extract the data.
 * part of the API
 *
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
int InitWcs(int numChips,char *file1, char *file2, char *file3)
{
/*printf("!!!!!!!!!!!! Starting InitWcs\n"); */
    if (getWcsPoints(1, file1) != OK)
	{
		printf("getWcsPoints returned error\n");
        return ERROR;
	}
    
    if ((numChips >1)&&(getWcsPoints(2, file2) != OK))
        return ERROR;
    if ((numChips>2)&&(getWcsPoints(3, file3) != OK))
        return ERROR;
/*printf("Ending InitWcs\n"); */
    return OK;
}

/*
 *+
 * FUNCTION NAME:
 * getWcsPoints
 *
 * INVOCATION:
 * getWcsPoints(chip, file)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) chip (int)
 * (>) file (char *)
 *
 * FUNCTION VALUE:
 * (int) Returns OK (0) or ERROR (1) as status
 *
 * PURPOSE:
 * Reads WCS data from a file into memory
 *
 * DESCRIPTION:
 * Get the wcs information from a file.  If there are more than NPOINTS
 * lines in the file, they are ignored.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
int getWcsPoints(int chip, char *file)
{
   int n,
       count;
   int c;
   char buffer[81];
   char *p;
   FILE *fp;
   char MyDebugMessage[120];

   /*printf("2.... Starting getWcsPoints\n");*/
   c = chip - 1;
   wcsInfo[c].numPoints = globalWcsnPts = 0;
   if ((strncmp(file, "NONE", 4) == 0) || (*file == '\0')) {
      sprintf(ErrorMessage,"No wcs data for chip %d", chip);
      printf("No wcs data for chip %d", chip);
      DPRINT(1,ErrorMessage);
      return OK;
   }
   fp = fopen(file, "r");
   if (fp == NULL) {
      sprintf(ErrorMessage, "Cannot open wcs file '%s'", file);
      printf("Cannot open wcs file '%s'", file);
      DPRINT(1,ErrorMessage);
      return ERROR;
   }

   printf("Found wcs file =  '%s'\n", file);
   for (n = 0; n < NPOINTS; n++) {
      p = fgets(buffer, 80, fp);
      if (p == NULL) {
	 if (feof(fp))
	    break;
	 else {
	    sprintf(ErrorMessage,"Error reading wcs file '%s'", file);
	    printf("Error reading wcs file '%s'", file);
	    DPRINT(1,ErrorMessage);
	    fclose(fp);
	    return ERROR;
	 }
      }
      count = sscanf(buffer, "%lf %lf %lf %lf",
	    &(wcsInfo[c].pixij[n][0]), &(wcsInfo[c].pixij[n][1]),
	    &(wcsInfo[c].fpxy[n][0]), &(wcsInfo[c].fpxy[n][1]));

      sprintf(MyDebugMessage, "wcs file buffer = %f %f %f %f\n",
	    (wcsInfo[c].pixij[n][0]),(wcsInfo[c].pixij[n][1]),
	    (wcsInfo[c].fpxy[n][0]), (wcsInfo[c].fpxy[n][1]));
      DPRINT(0, MyDebugMessage);

      if (count != 4) {
	 n--;
	 continue;
      }
   } 
   fclose(fp);
   wcsInfo[c].numPoints = globalWcsnPts = n;
   /*printf("2...... Ending getWcsPoints\n"); */
   return OK;
}

/*
 *+
 * FUNCTION NAME:
 * WcsContext
 *
 * INVOCATION:
 * WcsContext(wp, f, ep, t)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) wp (struct WCS *)
 * (>) f (int)
 * (>) ep (struct EPOCH *)
 * (>) t (double)
 *
 * FUNCTION VALUE:
 * (int) Returns OK (0) or ERROR (1) as status
 *
 * PURPOSE:
 * Records WCS context information from EPICS layer
 *
 * DESCRIPTION:
 * Input data from the EPICS world is just copied to local storage
 * part of the API
 * 
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
int WcsContext(struct WCS *wp, int f, struct EPOCH *ep, double t)
{

    struct WCS *localWp;
    struct EPOCH *localEp;

    localWp = &wcs;
    *localWp = *wp;

    trackFrame = f;

    localEp = &trackEquinox;
    *localEp = *ep;

    timeTAI = t;
    return OK;
}


/*
 *+
 * FUNCTION NAME:
 * wcsCalibrate
 *
 * INVOCATION:
 * wcsCalibrate(chip, ibin, jbin, iskip, jskip)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) chip (int)
 * (>) ibin (int)
 * (>) jbin (int)
 * (>) iskip (int)
 * (>) jskip (int)
 *
 * FUNCTION VALUE:
 * (int) Returns OK (0) or ERROR (1) as status
 *
 * PURPOSE:
 * Generate WCS data from data points and WCS EPICS information
 *
 * DESCRIPTION:
 * Function wcsCalibrate() defines a pixel coordinate (i,j) to (x,y)
 * transformation.
 *
 * In practise this will be done by executing the following steps:
 *
 * Step 1 - calibration
 *
 *    For each calibration point, p
 *        Command the TCS to put a star at position (x,y) in the focal plane,
 *           (fpxy[p][0]), fpxy[p][1]).
 *        Measure the position of that star on the detector in pixels (i,j),
 *           (pixij[p][0], pixij[p][1]).
 *    Next point (at least 3 points are needed).
 *
 *    In this example the points are defined arbitrarily. In the real world
 *    the (x,y) and (i,j) measurements will be stored in a file and read when
 *    needed. 4 points are used merely as an example and a more reliable fit 
 *    would be obtained using more points. A fit such as this is valid for any
 *    imaging instrument, not just the HRWFS.
 *
 * Step 2 - transform the (i,j) measurements to match the current configuration
 *
 *    It is assumed the calibration at step (1) will have been done with the
 *    detector generating unbinned full frames. If the detector is reconfigured
 *    to bin the data or generate a smaller region of interest, the (i,j) 
 *    values need to be transformed to match the detector coordinates in the 
 *    new frame of reference.
 *
 *    For example, if the detector is skipping the first "iskip" pixels and 
 *    binning the remaining pixels by a factor "ibin" each i measurement
 *    becomes
 *
 *        i_new = ((i_old - 0.5 - iskip) / ibin) + 0.5;
 *
 * Step 2 - fit
 *
 *    Function astFitij() is used to generate an (i,j) to (x,y) transformation.
 *    This transformation will be valid as long as the parameters used for
 *    steps (1) and (2) remain the same.
 *
 *    If the detector is reprogrammed (e.g. different binning or window)
 *    go back to step (2).
 *    If the instrument is reconfigured (e.g. different camera used)
 *    go back to step (1).
 *
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * From code supplied by Steven Beard
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
int wcsCalibrate(int chip, int ibin, int jbin, int iskip, int jskip)
{
   char buffer[120];
   /* Detector i and j binning factors: ibin, jbin
    * Detector will skip this number of pixels: iskip, jskip    
    */

   int wcsStat;                /* WCS status.    */
   int p;                      /* Point counter. */


   double pixis;               /* x to i scale factor.     */
   double pixjs;               /* y to j scale factor.     */
   double perp;                /* Non-perpendicularity of i and j axes in
				* radians.
				*/
   double orient;              /* Orientation of (i,j) axes with respect to
				*  (x,y) in radians.
				*/
   gmosChip *wp;

   /*printf("#### Starting wcsCalibrate\n"); */
   wp = &wcsInfo[chip];
   /*printf("bin = %d, %d, skip = %d, %d\n",ibin,jbin,iskip,jskip); */
   for (p = 0; p < wp->numPoints; p++) {
      wp->detij[p][0] = ((wp->pixij[p][0] - 0.5 - (double) iskip) /
	    (double) ibin) + 0.5;

      wp->detij[p][1] = ((wp->pixij[p][1] - 0.5 - (double) jskip) /
	    (double) jbin) + 0.5;
      /*printf("det[%d][0] = %f, det[%d][1] = %f\n",p,wp->detij[p][0],p,wp->detij[p][1]); */
   }


   /*
    * Define the (i,j) to (X,Y) transformation. The transformation is written
    * to matrix cij.
    */
   /*printf("numPoints = %d\n",wp->numPoints); */

   wcsStat = astFitij(wp->numPoints, wp->fpxy, wp->detij, wp->cij,
	 &pixis, &pixjs, &perp, &orient);
   if (wcsStat != 0) {
      sprintf(buffer,
	    "astFitij: Failed to define transformation: %d", wcsStat);
      DPRINT(0,buffer);
      return ERROR;
   }
   /*
    * printf ("Best fit scale is %f X units per i pixel and "
    * "%f Y units per j pixel\n", pixis, pixjs);
    * printf ("i/j non-perpendicularity is %f radians.\n", perp);
    * printf ("i/j is rotated by %f radians with respect to x/y axis.\n",
    * orient);
    */

   sprintf(buffer, "Cij matrix contains %f %f %f %f %f %f\n",
	 wp->cij[0], wp->cij[1], wp->cij[2], wp->cij[3], wp->cij[4], wp->cij[5]);
   /*printf("Cij matrix contains %f %f %f %f %f %f\n",
     wp->cij[0], wp->cij[1], wp->cij[2], wp->cij[3], wp->cij[4], wp->cij[5]);*/
   /*	DPRINT(1,buffer); */

   /*printf("##### Ending wcsCalibrate\n");*/
   return OK;
}

STATUS getROI(roiParams *proi)
{
    int i;
    char dummy[80];
    char name[80];
    
    getDbInfoT(dbTop,DRROISET_CAD ".VALA",dummy, DBF_LONG,&proi->numRois);
    for (i=0;i<proi->numRois;i++)
    {
    /*printf("getRoi primer IF\n");*/
	sprintf(name,"%s%s%d.VAL",dbTop,LOWCOL,i+1);
	getDbInfo(name,dummy, DBF_LONG, &proi->roi[i].lowX);
	sprintf(name,"%s%s%d.VAL",dbTop,LOWROW,i+1);
	getDbInfo(name,dummy, DBF_LONG, &proi->roi[i].lowY);
	sprintf(name,"%s%s%d.VAL",dbTop,HICOL,i+1);
	getDbInfo(name,dummy, DBF_LONG, &proi->roi[i].hiX);
	sprintf(name,"%s%s%d.VAL",dbTop,HIROW,i+1);
	getDbInfo(name,dummy, DBF_LONG, &proi->roi[i].hiY);  

    }
  
    if(proi->numRois == 0)
    {
    /*printf("getRoi segundo IF\n");*/
	proi->numRois = 1;
	proi->roi[0].lowX = 0;
	proi->roi[0].lowY = 0;
	proi->roi[0].hiX = ARRAY_WIDTH-1;
	proi->roi[0].hiY = ARRAY_LENGTH - 1;
	proi->roi[0].rows = ARRAY_LENGTH;
	proi->roi[0].cols = ARRAY_WIDTH;
    }
    return OK;
}
/*
*+
 * FUNCTION NAME:
 * CalcWcs
 *
 * INVOCATION:
 * CalcWcs(chip,  roi, bdFrame)
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * (>) chip (int)   valid values: 0-2
 * (>) roi (int)    valid values: 0-3
 * (>) bdFrame (DHS_BD_FRAME)
 *
 * FUNCTION VALUE:
 * (int) Returns OK (0) or ERROR (1) as status
 *
 * PURPOSE:
 * Generates header values for current observation and current WCS
 *
 * DESCRIPTION:
 * Function wcsCalc() generates FITS header values describing the current World
 * Coordinate System. It assumes that function wcsCalibrate() has been executed
 * and a (i,j) to (X,Y) transformation matrix has been defined in global
 * variable cij.
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Nov 2, 1999      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */
int CalcWcs(int chip, int roiNum) {
#ifdef DEBUG
   int t1 = 1;
#endif
   char dummy[80];
   char name[80];
   int wcsStat;                /* WCS status.                         */
   struct WCS wcsij;           /* World Coordinate System transformed
				* into detector frame of reference.     */
   roiParams roi;
   int xbin=1,ybin=1;
   tRect *p;
   char buffer[80];

   /*printf("IN CALCWCS !!!\n"); */
   /* if no wcs data, return */
   if (wcsInfo[chip].numPoints == 0)
   {
      DPRINT(t1,"CalcWcs: no points \n");
      return OK;
   }


   /*printf("timeTAI = %f\n",timeTAI);   */ 
   /*printf("To getWcsParams \n");   */ 
   wcsStat = getWcsParams();
   /*printf("from getWcsParams \n");    
     printf("timeTAI after getWcsParams = %f\n",timeTAI); */


   getROI(&roi);
   p = &roi.roi[roiNum];       /* requested row values, unmodified */

   /*printf("1. In CALCWCS lowx = %d, lowy = %d\n",p->lowX,p->lowY);
     printf("1. In CALCWCS hiX = %d, hiY = %d\n",p->hiX,p->hiY);
     printf("1. In CALCWCS rows = %d, cols = %d\n",p->rows,p->cols);
     printf("!!!. In CALCWCS chip = %d xbin = %d, ybin = %d\n",chip,xbin,ybin);
     printf("to wcsCalibrate \n");*/
   wcsStat = wcsCalibrate(chip, xbin, ybin, p->lowX, p->lowY );
   /*printf("from wcsCalibrate \n");*/
   if (wcsStat != OK)
   {
      DPRINT(t1,"CalcWcs: wcscalibrate error \n");
      return wcsStat;
   }

   /*printf("4. to astXtndtr \n"); */
   wcsStat = astXtndtr(wcsInfo[chip].cij, wcs, &wcsij);
   if (wcsStat != 0) {
      sprintf(buffer, "astXtndtr failed : %d", wcsStat);
      DPRINT(1,buffer);
      DPRINT(t1,"CalcWcs: astxtndtr error \n");
      return (ERROR);
   }
   /*
    * Calculate the FITS header values.
    *
    */

   /*printf("to astFITSv\n");*/
   /*printf("ab0[0] = %f \n",wcsij.ab0[0]);
     printf("ab0[1] = %f \n",wcsij.ab0[1]);
     printf("coeffs[0] = %f \n",wcsij.coeffs[0]);
     printf("coeffs[1] = %f \n",wcsij.coeffs[1]);
     printf("coeffs[2] = %f \n",wcsij.coeffs[2]);
     printf("coeffs[3] = %f \n",wcsij.coeffs[3]);
     printf("coeffs[4] = %f \n",wcsij.coeffs[4]);
     printf("coeffs[5] = %f \n",wcsij.coeffs[5]);
     printf("trackFrame = %d \n",trackFrame);
     printf("trackEquinox YEAR = %f \n",trackEquinox.year);
     printf("trackEquinox TYPE = %c \n",trackEquinox.type);
     printf("timeTAI = %f \n",timeTAI); */

   wcsStat = astFITSv(wcsij, trackFrame, trackEquinox, timeTAI,
	 ctype1, &crpix1, &crval1,
	 ctype2, &crpix2, &crval2,
	 &cd1_1, &cd1_2, &cd2_1, &cd2_2,
	 radecsys, &equinox, &mjdobs);
   if (wcsStat != 0) {
      sprintf(buffer,
	    "astFITSv failed to calculate FITS header: %d", wcsStat);
      DPRINT(1,buffer);
      return (ERROR);
   }

   /*printf("CalcWcs: setting values \n");*/
   DPRINT(t1,"CalcWcs: setting values \n");
   sprintf(name,"%s%s.VAL",dbTop,WCS_CTYPE1);
   /*DPRINT(t1,name); */
   putDbInfo(name,dummy,DBF_STRING,ctype1);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CRPIX1);
   putDbInfo(name,dummy,DBF_DOUBLE,&crpix1);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CRVAL1);
   putDbInfo(name,dummy,DBF_DOUBLE,&crval1);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CTYPE2);
   putDbInfo(name,dummy,DBF_STRING,ctype2);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CRPIX2);
   putDbInfo(name,dummy,DBF_DOUBLE,&crpix2);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CRVAL2);
   putDbInfo(name,dummy,DBF_DOUBLE,&crval2);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CD1_1);
   putDbInfo(name,dummy,DBF_DOUBLE,&cd1_1);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CD1_2);
   putDbInfo(name,dummy,DBF_DOUBLE,&cd1_2);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CD2_1);
   putDbInfo(name,dummy,DBF_DOUBLE,&cd2_1);
   sprintf(name,"%s%s.VAL",dbTop,WCS_CD2_2);
   putDbInfo(name,dummy,DBF_DOUBLE,&cd2_2);
   sprintf(name,"%s%s.VAL",dbTop,WCS_RADECSYS);
   putDbInfo(name,dummy,DBF_STRING,radecsys);
   sprintf(name,"%s%s.VAL",dbTop,WCS_EQUINOX);
   putDbInfo(name,dummy,DBF_DOUBLE,&equinox);
   sprintf(name,"%s%s.VAL",dbTop,WCS_MJDOBS);
   putDbInfo(name,dummy,DBF_DOUBLE,&mjdobs);
   /* write out the values into the headers */
   /*    if (D.dataOp != DOWRITE) */
   /*         return OK; */


   return (OK);
}

/*
 *+
 * FUNCTION NAME:
 *              getWcsParams
 *
 * INVOCATION:
 * 
 * status = getWcsParams( );
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *      none
 *
 * FUNCTION VALUE:
 * > long status
 *
 * PURPOSE:
 *      Get the tcs related parameters for an observe.
 *
 * DESCRIPTION:
 *      real mode:  gets the time, tcs context, trackFrame, and trackEquinox fro
m the tcs for an observation.
 *
 *      simulation mode: use default values from "World Coordinates, Part1: Astr
ometry" to simulate
 *                              a tcs context.  The values computed should match
 the ones in the paper
 *
 *
 * EXTERNAL VARIABLES:
 *      gmosChip wcsInfo
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY (optional):
 *      code taken from "World Coordinates, Part1: Astrometry" P.T. Wallace
 *
 *
 *-
 */
/* defines for testing wcs calculations.  */
#define IPIX 2220.0
#define JPIX 1280.0
#define AS2R 4.84813681109536e-6
#define D2R  0.0174532925199433
int myEnableTCS=1;

long getWcsParams()
{     
   char equinox[30];
   int n = 0;
   double m2xy[3][2] = {{0.0,0.0},
      {0.0,0.0},
      {0.0,0.0}
   };
   char err[80];
   char buf[80];
   char errMess[80];
   double timeTAI;
   struct WCS wcs;
   struct WCS_CTX ctx;
   double pai = 270.0*D2R;
   double aia;
   struct TELP tel;
   double track_ra, track_dec, trackWavelength;
   FRAMETYPE trackFrame;
   struct EPOCH trackEquinox;
   double rawTime, timeStamp;
   double tai,dleap,dat,dut,elongm,phim,hm,xp,yp,tdk,pmb,rh;
   double tlr,elong,phi,daz;
   int j;


   /* default or test values*/
   dleap = 50083.0;
   dat = 29.0;
   dut = .0746;

   elongm = -2.71349248271422;
   phim = 0.346040618846507;
   hm = 4145.0;
   xp = 0.25*AS2R;
   yp = .4 * AS2R;
   tdk = 275.0;
   pmb = 605.0;
   rh = 0.8;
   tlr = 0.0065;
   slaPolmo (elongm,phim,xp,yp,&elong,&phi,&daz);

   tel.fl = 128000.0;;
   tel.rma = 30.0*D2R;
   tel.an = -12.0*AS2R;
   tel.aw = -5.0*AS2R;
   tel.pnpae = 8.0*AS2R;
   tel.ca = -110.0*AS2R;
   tel.ce = 22.0*AS2R;
   track_ra = 36.0 * D2R;
   track_dec = 45.0 * D2R;

   /*Test CAC failures to TCS which result in total Channel Access crash*/
   TCS = myEnableTCS;
   if(TCS)
   {
      /* get time from tcs and convert it to TAI */
      /*printf("*** 1. in getWcsparams \n Getting time from TCS\n");*/
      if(timeNow(&timeStamp) != OK)
      {
	 DPRINT(1,"Failed to get time stamp\n");
	 return ERROR;
      }
   }

   else /* fake time if tcs not available */
   {  
      DPRINT(1,"TCS disconntected\n");
      tai = 49560.643391203703;
      if(j = mytimeOffline(tai,elong,phi,hm,dleap,dat,dut)) 
      { 
	 printf("bad status from time Offline: %d\n",j); 
	 return -1; 
      } 

      timeStamp = timeTai2raw(tai);
   }

   DPRINT(1,"getWcsParams got time\n");
   /*printf("2. getWcsParams got time\n");*/
   if(timeThenD(timeStamp,TT,&timeTAI) != OK)
   {
      DPRINT(1,"Failed to convert time stamp to TAI\n");
      printf("Failed to convert time stamp to TAI\n");
      return ERROR;
   } 

   printf("TimeTAI in WCS  = %f\n",timeTAI);
   DPRINT(1,"getWcsParams convert time\n");

   if(getDbInfo(CURRENT_FRAME,errMess,DBR_STRING,buf) != OK) {
      printf("name = %s, buf = %s\n",CURRENT_FRAME,buf);
      DPRINT(1,"Error getting trackFrame from tcs\n");
      return ERROR;
   }
   if(strcmp(buf,"FK4") == 0)
      trackFrame = FK4;
   else if(strcmp(buf,"FK5") == 0)
      trackFrame = FK5;
   else if(strcmp(buf,"Apparent") == 0)
      trackFrame = APPT;
   else if(strcmp(buf,"Observer Altaz") == 0)
      trackFrame = AZEL_TOPO;
   else if(strcmp(buf,"Mount Altaz") == 0)
      trackFrame = AZEL_MNT;
   else
   {
      sprintf(err,"Error %s value = %s ",CURRENT_FRAME, buf);
      DPRINT(1,err);
      return ERROR;
   }

   if(getDbInfo(CURRENT_WAVELENGTH,errMess,DBR_DOUBLE,&trackWavelength) != OK) {
      DPRINT(1,"Error getting trackWavelength from tcs\n");
      return ERROR;
   }

   /* convert wavelength from angstroms to microns*/
   trackWavelength = trackWavelength / 10000.0;

   if(getDbInfo(CURRENT_EQUINOX,errMess,DBR_STRING,equinox) != OK) {
      printf("Error getting trackEquinox from tcs\n");
      return ERROR;
   }

   trackEquinox.year = atof(&(equinox[1]));
   trackEquinox.type = equinox[0]; 

   DPRINT(1,"getWcsParams set defaults\n");
   /* get ctx context from tcs or simulate it it with astSimctx_r*/

   if (TCS) {

      printf("@@@@@@@@@@@@@@@@ astGetctx\n");
      if(astGetctx(&ctx) != OK)
      {
	 DPRINT(1,"Failed to get current wcs context\n");
	 printf("Failed to get current wcs context\n");
	 return ERROR;
      }
   }
   else /* simulate wcs context*/
   { 
      /*printf("SIMULATING PIXIJ with wcs_short parameters...\n");*/
      wcsInfo[n].pixij[0][0] = 513.710000;
      wcsInfo[n].pixij[0][1] = 458.070000;
      wcsInfo[n].fpxy[0][0] = 2.916646;
      wcsInfo[n].fpxy[0][1] = -0.80052624;
      wcsInfo[n].pixij[1][0] = 758.47;
      wcsInfo[n].pixij[1][1] = 443.46;
      wcsInfo[n].fpxy[1][0] = 25.995383;
      wcsInfo[n].fpxy[1][1] = -2.0912972;
      wcsInfo[n].pixij[2][0] = 600.76;
      wcsInfo[n].pixij[2][1] = 461.34;
      wcsInfo[n].fpxy[2][0] =  11.157722;
      wcsInfo[n].fpxy[2][1] =  -0.45301097;
      wcsInfo[n].pixij[3][0] = 513.44;
      wcsInfo[n].pixij[3][1] = 457.19;
      wcsInfo[n].fpxy[3][0] =  2.916646;
      wcsInfo[n].fpxy[3][1] =  -0.80052624;
      wcsInfo[n].pixij[4][0] = 480.47;
      wcsInfo[n].pixij[4][1] = 468.50;
      wcsInfo[n].fpxy[4][0] =  -0.21719704;
      wcsInfo[n].fpxy[4][1] =  0.19858015;
      wcsInfo[n].pixij[5][0] = 663.12;
      wcsInfo[n].pixij[5][1] = 454.07;
      wcsInfo[n].fpxy[5][0] =  17.009631;
      wcsInfo[n].fpxy[5][1] =  -1.0735739;
      wcsInfo[n].pixij[6][0] = 513.93;
      wcsInfo[n].pixij[6][1] = 457.29;
      wcsInfo[n].fpxy[6][0] =  2.916646;
      wcsInfo[n].fpxy[6][1] =  -0.80052624;
      wcsInfo[n].pixij[7][0] = 714.49;
      wcsInfo[n].pixij[7][1] = 471.93;
      wcsInfo[n].fpxy[7][0] =  21.8252;
      wcsInfo[n].fpxy[7][1] =  0.57712357;
      wcsInfo[n].pixij[8][0] = 403.46;
      wcsInfo[n].pixij[8][1] = 468.79;
      wcsInfo[n].fpxy[8][0] =  -7.4777838;
      wcsInfo[n].fpxy[8][1] =  0.26063645;
      wcsInfo[n].pixij[9][0] = 747.48;
      wcsInfo[n].pixij[9][1] = 459.67;
      wcsInfo[n].fpxy[9][0] =  24.940426;
      wcsInfo[n].fpxy[9][1] =  -0.53368416;
      wcsInfo[n].pixij[10][0] = 513.79;
      wcsInfo[n].pixij[10][1] = 457.28;
      wcsInfo[n].fpxy[10][0] =  2.916646;
      wcsInfo[n].fpxy[10][1] =  -0.80052624;
      wcsInfo[n].numPoints =  globalWcsnPts = 11;
      wcsCalibrate(n,1,1,0,0);   
      /* 	for (i=0;i< 6;i++) */
      /* 	{ */
      /* 	    printf("cij[%d] = %f   ",i,wcsInfo[n].cij[i]); */
      /* 	} */
      /* 	printf("\n"); */

      aia = slaDranrm (wcsInfo[n].orient -90.0*D2R);

      slaXy2xy((IPIX+1.0)/2.0,(JPIX+1.0)/2.0,wcsInfo[n].cij,&tel.pox,&tel.poy); 
      /* 	printf("tai = %f, elongm = %f, phim = %f\n",tai,elongm,phim); */
      /* 	printf("hm = %f, xp = %f yp = %f, tdk = %f\n",hm,xp,yp,tdk); */
      /* 	printf("pmb = %f,rh = %f, tlr = %f, trackWavelength = %f\n",pmb,rh,tlr,trackWavelength); 
		printf("aia = %f,pai = %f,trackFrame = %d\n",aia,pai,trackFrame); 
		printf("year = %f, type = %c\n",trackEquinox.year,trackEquinox.type); */
      /* 	printTel(tel); */
      /* 	printf("trackRA = %f, trackDec = %f\n",track_ra,track_dec); */

      if((j = astSimctx_r(tai, elongm, phim, hm, xp, yp, tdk, pmb, rh, 
		  tlr, trackWavelength, tel, m2xy, track_ra, track_dec, 
		  trackFrame, trackEquinox, aia, pai,trackFrame,
		  trackEquinox, &ctx) != OK))    
      {
	 DPRINT(1,"bad status from astSimctx_r: \n");
	 return ERROR;
      } 

      /* 	printf("getwcsparams 4\n"); */
      /* 	sleep(1,0); */

   } /*end sim: TCS=0 */
   /*     printctx(ctx); */
   /*     DPRINT(1,"getWcsParams got context\n"); */
   /*     sleep(3,0); */
   /*extract the current focal plane to sky WCS transformationfrom the tcs cont
     ext*/

   if ( (j=astCtx2tr(ctx, trackFrame, trackEquinox,
	       trackWavelength, 0, &wcs, &rawTime))!= OK)
   {
      DPRINT(1,"bad status from astCtx2tr: \n");
      return ERROR;
   }

   /*printf("WCS information extracted from TCS context is valid at time %f\n",rawTime);*/ 
   /*     DPRINT(1,"getWcsParams got wcs\n"); */
   /*printf("getWcsParams got wcs\n"); */
   /*     sleep(1,0); */

   /* send values to controller*/
   if(WcsContext(&wcs, trackFrame,&trackEquinox,timeTAI) != OK)
   {   
      /*         ErrorMessage[39]= 0; */
      /*      	   DPRINT(1,ErrorMessage); */
      printf("Error in WcsContext\n");
      return ERROR;
   }

   DPRINT(1,"getWcsParams done\n");
   /*printf("getWcsParams done\n");*/
   return OK;
}

long printctx(struct WCS_CTX ctx)
{
    int i;
    printf("ab0[%d] = %f",0,ctx.ab0[0]);
    printf("ab0[%d] = %f",1,ctx.ab0[1]);
    printTel(ctx.tel);
    printf("aoprms\n");
    for (i = 0; i<15; i++)
        printf(" %f ",ctx.aoprms[i]);
    printf("\n");
    printf("m2xy\n");
   for (i = 0;i <3; i++)
        printf("%f %f \n",ctx.m2xy[i][0],ctx.m2xy[i][1]);
    printf("time = %f\n\n\n",ctx.time);
    return OK;
}

long printTel(struct TELP tel)
{
    printf("focal length = %f\n",tel.fl);
    printf("rotator orientation = %f\n",tel.rma);
    printf("azimuth ns = %f\n",tel.an);
    printf("azimuth ew = %f\n",tel.aw);
    printf("Az/el = %f\n",tel.pnpae);
    printf("lr = %f\n",tel.ca);
    printf("ud = %f\n",tel.ce);
    printf("pointing origin x = %f\n",tel.pox);
    printf("pointing origin y = %f\n",tel.poy);
return OK;
}


void showwcsxform(void) {

int n=0;

   int points = 0;
   points = wcsInfo[0].numPoints;

   if (points <= 0)  {
	printf("showwcsxform error: wcsInfo not populated. Re-initialize WCS selection?\n");
	return;
   }

   for (n=0; n<points; n++) {
      printf("wcs file buffer = %f %f %f %f\n",
	    (wcsInfo[0].pixij[n][0]),(wcsInfo[0].pixij[n][1]),
	    (wcsInfo[0].fpxy[n][0]), (wcsInfo[0].fpxy[n][1]));
   }
}

void showwcsheader(void) {
int status;

    status = CalcWcs(0,0);

     printf("------------------------------\n");
     printf("ctype1   = %s\n", ctype1);
     printf("crpix1   = %f pixels\n", crpix1);
     printf("crval1   = %f degrees = %f hours\n", crval1, (crval1 / (double) 15.0));
     printf("ctype2   = %s\n", ctype2);
     printf("crpix2   = %f pixels\n", crpix2);
     printf("crval2   = %f degrees\n", crval2);
     printf("cd1_1    = %g\n", cd1_1);
     printf("cd1_2    = %g\n", cd1_2);
     printf("cd2_1    = %g\n", cd2_1);
     printf("cd2_2    = %g\n", cd2_2);
     printf("radecsys = %s\n", radecsys);
     printf("equinox  = %f\n", equinox);
     printf("mjd-obs  = %f\n", mjdobs);

}
