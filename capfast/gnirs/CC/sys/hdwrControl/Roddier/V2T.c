#include <vxWorks.h>
#include <types.h>
#include <stdioLib.h>
#include <semLib.h>

#include <dbDefs.h>
#include <subRecord.h>
#include <dbCommon.h>
#include <recSup.h>

double CnvT(double), CnvTlks(double);

/*
long V2T0(psub)
struct subRecord *psub;
{
}
*/

long V2TDlks(psub)
struct subRecord *psub;
{
     double tdat, tref;

     tdat = CnvTlks(psub->b + psub->a/120.0);
     tref = CnvTlks(psub->b);

     if (psub->a > 4.5)
       {
       psub->val = -999.0;
       return(0);
       }
     if (psub->a < -4.5)
       {
       psub->val = 999.0;
       return(0);
       }
     psub->val = tdat - tref;
     return(0);
}

long V2Tlks(psub)
struct subRecord *psub;
{
     psub->val = CnvTlks(psub->a);
     return(0);
}

long V2TD(psub)
struct subRecord *psub;
{
     double tdat, tref;

     tdat = CnvT(psub->b + psub->a/120.0);
     tref = CnvT(psub->b);

     if (psub->a > 4.5)
       {
       psub->val = -999.0;
       return(0);
       }
     if (psub->a < -4.5)
       {
       psub->val = 999.0;
       return(0);
       }
     psub->val = tdat - tref;
     return(0);
}

long V2T(psub)
struct subRecord *psub;
{
     psub->val = CnvT(psub->a);
     return(0);
}

double CnvT(x)
double x;
{
     if (x<0.388) return(300.0);
     if (x<0.993) return(300.0 - 223.0*(x-0.388)/0.6050);
     if (x<1.020) return(77.0 - 11.0*(x-0.993)/0.027);
     if (x<1.040) return(66.0 - 9.0*(x-1.020)/0.020);
     if (x<1.060) return(57.0 - 8.0*(x-1.040)/0.020);
     if (x<1.080) return(49.0 - 6.0*(x-1.060)/0.020);
     if (x<1.100) return(43.0 - 5.0*(x-1.080)/0.020);
     if (x<1.120) return(38.0 - 3.0*(x-1.100)/0.020);
     if (x<1.140) return(35.0 - 3.0*(x-1.120)/0.020);
     if (x<1.160) return(32.0 - (x-1.140)/0.020);
     if (x<1.180) return(31.0 - (x-1.160)/0.020);
     return(30.0);
}

double CnvTlks(x)
double x;
{
     if (x<0.51892) return(300.0);
     if (x<0.97550) return(300.0 - 200.0*(x-0.51892)/0.4566);
     if (x<1.05267) return(100.0 -  40.0*(x-0.97550)/0.0772);
     if (x<1.06700) return( 60.0 -   8.0*(x-1.05267)/0.0143);
     if (x<1.07748) return( 52.0 -   6.0*(x-1.06700)/0.0105);
     if (x<1.08781) return( 46.0 -   6.0*(x-1.07748)/0.0103);
     if (x<1.09310) return( 40.0 -   3.0*(x-1.08781)/0.0053);
     if (x<1.09864) return( 37.0 -   3.0*(x-1.09310)/0.0055);
     if (x<1.10482) return( 34.0 -   3.0*(x-1.09864)/0.0062);
     if (x<1.11212) return( 31.0 -   3.0*(x-1.10482)/0.0073);
     if (x<1.12463) return( 28.0 -   3.0*(x-1.11212)/0.0125);
     if (x<1.17705) return( 25.0 -   3.0*(x-1.12463)/0.0524);
     return(22.0);
}


