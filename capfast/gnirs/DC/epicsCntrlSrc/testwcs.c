/* program to print an example context.  taken from "World Coordinates, Part I
Astrometry" P.T. Wallace*/
#include "vxWorks.h"
#include <fioLib.h>
#include <stdio.h>
#include <timeLib.h>
#include <slalib.h>
#include "astLib.h"
#include "epCommon.h"
#include "localWcs.h"
typedef struct {
        int chipId;             /* which chip; 0 - 2 */
        char ctype1[81];
        double crpix1, crval1;
        char ctype2[81];
        double crpix2, crval2;
        double cd1_1, cd1_2, cd2_1, cd2_2;
        char radecsys[81];
        double equinox, mjdobs;
} SDSU_WCS_INFO;

static double timeTAI;
extern long TCS;
extern gmosChip wcsInfo[NUM_CHIPS];
static SDSU_WCS_INFO wcsHdr[NUM_CHIPS];

long printTel(struct TELP tel);
long printctx(struct WCS_CTX ctx);
int mytimeOffline ( double tai,
                  double elong, double phi, double hm,
                  double dleap, double dat, double dut );

#define AS2R 4.84813681109536e-6
#define D2R  0.0174532925199433

#define IPIX 2220.0
#define JPIX 1280.0

long testwcs()
{
    int n = 0;
    double m2xy[3][2] = {{0.0,0.0},
			  {0.0,0.0},
			  {0.0,0.0}
    };
 
    double pai = 270.0*D2R;
 
    double aia;
    struct TELP tel;
    double track_ra, track_dec, track_wl;
    FRAMETYPE track_frame;
    double timestamp;
    struct WCS wcs;
    struct WCS wcsij;
    struct WCS_CTX ctx;
    struct EPOCH track_equinox;
    double tai,dleap,dat,dut,elongm,phim,hm,xp,yp,tdk,pmb,rh;
    double tlr,elong,phi,daz,ut,r,d,xi,eta;
    int j,i;

/* wcsInfo set by file selection eleswhere */
/*
    wcsInfo[n]. pixij[0][0] = 51.3;
    wcsInfo[n].pixij[0][1] = 49.5;
    wcsInfo[n].fpxy[0][0] = -19.15;
    wcsInfo[n].fpxy[0][1] = 12.31;
    wcsInfo[n].pixij[1][0] = 50.7;
    wcsInfo[n].pixij[1][1] = 1227.8;
    wcsInfo[n].fpxy[1][0] = -23.77;
    wcsInfo[n].fpxy[1][1] = -13.95;
    wcsInfo[n].pixij[2][0] = 2179.6;
    wcsInfo[n].pixij[2][1] = 1230.4;
    wcsInfo[n].fpxy[2][0] =  23.36;
    wcsInfo[n].fpxy[2][1] =  -22.26;
    wcsInfo[n].pixij[3][0] = 2182.3;
    wcsInfo[n].pixij[3][1] = 53.1;
    wcsInfo[n].fpxy[3][0] =  28.08;
    wcsInfo[n].fpxy[3][1] =  3.9;

        wcsInfo[n].numPoints =  4;
  */  
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


    track_wl = .55; 
    track_frame = FK5;
    track_equinox.type = 'J'; 
    track_equinox.year = 2000.0;
    track_ra = 36.0 * D2R;
    track_dec = 45.0 * D2R;

    tai = 49560.643391203703;
    if(j = mytimeOffline(tai,elong,phi,hm,dleap,dat,dut))
    {
	printf("bad status from time Offline: %d\n",j);
	return -1;
    }

    timestamp = timeTai2raw (tai);
    printf("timestamp = %f\n",timestamp);
    if(j = timeThenD(timestamp,TT,&timeTAI))/*XXX  timeTAI*/
    {
	printf("bad status from timeThend: %d \n",j);
	return -1;
    }
    
    if (j = timeThenD(timestamp,UT1,&ut))
    {
	printf("bad status from timeThend: %d \n",j);
	return -1;
    }
    for (i=0;i<4;i++)
    {
	printf("%f	%f 	%f	%f\n",
	wcsInfo[n].fpxy[i][0],
	wcsInfo[n].pixij[i][0],
	wcsInfo[n].fpxy[i][1],
	wcsInfo[n].pixij[i][1]);
    }

    if(j = astFitij(4,wcsInfo[n].fpxy,
		    wcsInfo[n].pixij,wcsInfo[n].cij,
		    &wcsInfo[n].pixis,&wcsInfo[n].pixjs,
		    &wcsInfo[n].perp,&wcsInfo[n].orient))
    {
	printf("bad status from astFitijxy: %d\n",j);
	return -1;
    }

    for (i=0;i< 6;i++)
    {
	printf("cij[%d] = %f   ",i,wcsInfo[n].cij[i]);
    }
    printf("\n");

    aia = slaDranrm (wcsInfo[n].orient -90.0*D2R);
    
    slaXy2xy((IPIX+1.0)/2.0,(JPIX+1.0)/2.0,wcsInfo[n].cij,&tel.pox,&tel.poy);
    
	printf("tai = %f, elongm = %f, phim = %f\n",tai,elongm,phim);
	printf("hm = %f, xp = %f yp = %f, tdk = %f\n",hm,xp,yp,tdk);
	printf("pmb = %f,rh = %f, tlr = %f, trackWavelength = %f\n",pmb,rh,tlr,track_wl);
	printf("aia = %f,pai = %f,trackFrame = %d\n",aia,pai,track_frame);
	printf("year = %f, type = %c\n",track_equinox.year,track_equinox.type);
	printTel(tel);
	printf("trackRA = %f, trackDec = %f\n",track_ra,track_dec);

    if(j = astSimctx_r(tai, elongm, phim, hm, xp, yp, tdk, pmb, rh, 
		       tlr, track_wl, tel, m2xy, track_ra, track_dec, 
		       track_frame, track_equinox, aia, pai,track_frame,
		       track_equinox, &ctx))    /*XXX track_frame*/
    {
	printf("bad status from astSimctx_r: %d\n",j);
	return -1;
    }

    printctx(ctx);

    if (j = astCtx2tr(ctx, track_frame, track_equinox,
		      track_wl, 0, &wcs, &timestamp))
    {
	printf("bad status from astCtx2tr: %d\n",j);
	return -1;
    }

    
    if(j = astXtndtr(wcsInfo[n].cij,wcs,&wcsij))
    {
	printf("bad status from astXtndtr: %d\n",j);
	return -1;
    }

	
  
    if(j = astXy2sq(.5,.5,wcsij,&r,&d))
    {
	printf("bad status from astXy2sq: %d\n",j);
	return -1;
    }
    printf("RA/Dec of BLC = %15.10f, %+15.10f deg\n",r/D2R,d/D2R);
    if(astXy2sq(IPIX+.5,JPIX+.5,wcsij,&r,&d))
    {
	printf("bad status from astXy2sq: %d\n",j);
	return -1;
    }

    printf("RA/Dec of TRC = %15.10f, %+15.10f deg\n",r/D2R,d/D2R);
    slaDs2tp(r,d,track_ra,track_dec,&xi,&eta,&j);
    if(j)
    {
	printf("bad status from slaDs2tp: %d\n",j);
	return -1;
    }
    

    if(j = astFITSv(wcsij,track_frame,track_equinox,
		    timeTAI,wcsHdr[n].ctype1,&wcsHdr[n].crpix1,&wcsHdr[n].crval1,
		    wcsHdr[n].ctype2,&wcsHdr[n].crpix2,&wcsHdr[n].crval2,
		    &wcsHdr[n].cd1_1,&wcsHdr[n].cd1_2,&wcsHdr[n].cd2_1,&wcsHdr[n].cd2_2,
		    wcsHdr[n].radecsys,&wcsHdr[n].equinox,&wcsHdr[n].mjdobs))
    {
	printf("bad status from astFITSs: %d\n",j);
	return -1;
    }

    printf("chip id = %s\n",wcsHdr[n].ctype1);
    printf("%s\n",wcsHdr[n].ctype1);
    printf("%f\n",wcsHdr[n].crpix1);
    printf("%f\n",wcsHdr[n].crval1);
    printf("%s\n",wcsHdr[n].ctype2);
    printf("%f\n",wcsHdr[n].crpix2);
    printf("%f\n",wcsHdr[n].crval2);
    printf("%f\n",wcsHdr[n].cd1_1);
    printf("%f\n",wcsHdr[n].cd1_2);
    printf("%f\n",wcsHdr[n].cd2_1);
    printf("%f\n",wcsHdr[n].cd2_2);
    printf("%s\n",wcsHdr[n].radecsys);
    printf("%f\n",wcsHdr[n].equinox);
    printf("%f\n",wcsHdr[n].mjdobs);

return 0;


}
#include "timesys.h" 

/* code taken from timelib modified to run under vxworks*/
int mytimeOffline ( double tai,
                  double elong, double phi, double hm,
                  double dleap, double dat, double dut )
#define AUKM  1.4959787066e8   /* AU to km */
{
   double tt, r, z;


/* Offset (zeroed). */
   biass = 0.0;

/* TT-TAI (fixed). */
   dttd = 32.184 / 86400.0;          /* ttmtai from TCS */

/* MJD following latest known UTC leap second (given). */
   djmls = dleap <= 0.0 ? 1e10 : dleap;

/* TAI-UTC before latest known UTC leap second (given). */
   datlsd = dat / 86400.0;

/* UT1-UTC before latest known UTC leap second (given). */
   dutd = dut / 86400.0;

/* Current TDB-TT (computed from TAI and site location). */
   tt = tai + dttd;
   slaGeoc ( phi, hm, &r, &z );
   deltdbd = slaRcc ( tt, fmod ( tai - dat + dut, 1.0 ), -elong,
                      r * AUKM, z * AUKM ) / 86400.0;

/* LAST minus GMST. */
   delstr = slaDranrm ( elong + slaEqeqx ( tt + deltdbd ) );

/* Set the "initialized" flag. */
   initd = 1;

/* Return the status. */
   return 0;
}
