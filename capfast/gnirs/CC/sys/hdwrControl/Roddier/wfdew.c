#include <stdioLib.h>
#include <taskLib.h>
#include "wfdew.h"

#define SHORTOFF 0xffff0000
#define BASEADD  0x3300
#define OFFMUX   0x01
#define OFFCTC   0x03
#define OFFRD    0x04
#define OFFEXT   0x06

#define ANLG_GAIN  6.0
#define ADC_RANGE 10.0
#define ADC_CNTS  4096
#define ADC_GAIN  ADC_RANGE/(float)(ADC_CNTS)/ANLG_GAIN

#define GND_A    15
#define GND_B    31

unsigned char *muxadd, *adcstr;
unsigned short *adcrd, *extcon;
unsigned char wfdewdbg = 0, wfdewinit = 0;

int wfdewdbgon()
{
   wfdewdbg = 1;
   return(0);
}

int wfdewdbgoff()
{
   wfdewdbg = 0;
   return(0);
}

int dewinit()
{
   muxadd = (unsigned char  *)(SHORTOFF + BASEADD + OFFMUX);
   adcstr = (unsigned char  *)(SHORTOFF + BASEADD + OFFCTC);
   adcrd  = (unsigned short *)(SHORTOFF + BASEADD + OFFRD);
   extcon = (unsigned short *)(SHORTOFF + BASEADD + OFFEXT);
   wfdewinit = 1;

   if (wfdewdbg == 1)
     {
     printf("Initialized memory pointer\n");
     printf("Pointers are initialized to these addresses:\n");
     printf("*muxadd = 0x%08X\n", (unsigned int) (muxadd));
     printf("*adcstr = 0x%08X\n", (unsigned int) (adcstr));
     printf("*adcrd  = 0x%08X\n", (unsigned int) (adcrd));
     printf("*extcon = 0x%08X\n", (unsigned int) (extcon));
     printf("wfdewinit = %d\n", (wfdewinit));
     }
   return(0);
}

int wradd(unsigned char myadd)
{
    if (wfdewinit != 1)
      {
      printf("Routine _wradd_\n");
      printf("Wildfire/Dewar module needs first to be initialized\n");
      return(-1);
      }
    if (myadd > 31)
      {
      printf("Routine _wradd_\n");
      printf("Channel address %d out of range\n", myadd);
      return(-1);
      }
    *muxadd = myadd;
    if (wfdewdbg == 1)
      {
      printf("Routine _wradd_\n");
      printf("Argument _myadd_ = %d\n", myadd);
      printf("pointer *muxadd = 0x%08X\n", (unsigned int)(muxadd));
      }
    return(0);
}

int adcctc()
{
    if (wfdewinit != 1)
      {
      printf("Routine _adcctc_\n");
      printf("Wildfire/Dewar module needs first to be initialized\n");
      return(-1);
      }
    *adcstr = 0;
    if (wfdewdbg == 1)
      {
      printf("Routine _adcctc_\n");
      printf("pointer *adcstr = 0x%08X\n", (unsigned int)(adcstr));
      }
    return(0);
}

int rdadc()
{
    int mydat;

    if (wfdewinit != 1)
      {
      printf("Routine _rdadc_\n");
      printf("Wildfire/Dewar module needs first to be initialized\n");
      return(-1);
      }
    mydat = *adcrd;
    if (wfdewdbg == 1)
      {
      printf("Routine _rdadc_\n");
      printf("pointer *adcrd = 0x%08X\n", (unsigned int)(adcrd));
      printf("Read result _mydat_ = %d\n", mydat);
      }
    return(mydat);
}

int extbus()
{
    int mydat;

    if (wfdewinit != 1)
      {
      printf("Routine _extbus_\n");
      printf("Wildfire/Dewar module needs first to be initialized\n");
      return(-1);
      }
    mydat = *extcon;
    if (wfdewdbg == 1)
      {
      printf("Routine _extbus_\n");
      printf("pointer *extcon = 0x%08X\n", (unsigned int)(extcon));
      printf("Read result _mydat_ = %d\n", mydat);
      }
    return(mydat);
}

int rddewint(unsigned char ch)
{
    int mydat;

    if (wfdewinit != 1)
      {
      printf("Routine _rddewint_\n");
      printf("Wildfire/Dewar module needs first to be initialized\n");
      return(-1);
      }
    if (ch > 31)
      {
      printf("Routine _rddewint_\n");
      printf("Channel address %d out of range\n", ch);
      return(-1);
      }
    wradd(ch);
    taskDelay(1);
    adcctc();
    taskDelay(1);
    mydat = rdadc();
    wradd(GND_A);
    wradd(GND_B);
    if (wfdewdbg == 1)
      {
      printf("Routine _rddewint_\n");
      printf("Argument _ch_ = %d\n", ch);
      printf("Read result _mydat_ = %d\n", mydat);
      }
    return(mydat);
}

float rddewvolt(unsigned char ch)
{
    float myval;

    if (wfdewinit != 1)
      {
      printf("Routine _rddewvolt_\n");
      printf("Wildfire/Dewar module needs first to be initialized\n");
      return(-1);
      }
    if (ch > 31)
      {
      printf("Routine _rddewvolt_\n");
      printf("Channel address %d out of range\n", ch);
      return(-1);
      }
    myval = (float)(rddewint(ch)) * ADC_GAIN;
    if (wfdewdbg == 1)
      {
      printf("Routine _rddewvolt_\n");
      printf("Argument _ch_ = %d\n", ch);
      printf("Read result _myval_ = %f\n", myval);
      }
    return(myval);
}

int dspdew()
{
    short i;
    float mydewdat[32];

    printf("The 32 sensor voltages are:\n");
    for (i=0; i<32; i++)
      mydewdat[i] = rddewvolt(i);

    printf("ch 0-7:   ");
    for (i=0; i<8;   i++) printf("%6.3f ", mydewdat[i]);
    printf("\n");

    printf("ch 8-15:  ");
    for (i=8; i<16;  i++) printf("%6.3f ", mydewdat[i]);
    printf("\n");

    printf("ch 16-23: ");
    for (i=16; i<24; i++) printf("%6.3f ", mydewdat[i]);
    printf("\n");

    printf("ch 24-31: ");
    for (i=24; i<32; i++) printf("%6.3f ", mydewdat[i]);
    printf("\n");

    return(0);
}

