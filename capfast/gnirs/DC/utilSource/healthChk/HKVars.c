static char rcid[]="$Id: HKVars.c,v 1.2 2009/05/27 19:33:33 fkraemer Exp $";
#include "stdio.h"
#include "errno.h"

#define OK	0

double HKc0[16];
double HKc1[16];
double HKc2[16];
double HKc3[16];
double HKc4[16];
double HKc5[16];
double HKc6[16];
double HKc7[16];
double HKc8[16];
double HKc9[16];
double HKc10[16];
double HKc11[16];
double HKc12[16];
double HKc13[16];
double HKc14[16];
double HKc15[16];

double *HKc[] = { HKc0, HKc1, HKc2, HKc3, HKc4, HKc5, HKc6, HKc7, HKc8,
		  HKc9, HKc10, HKc11, HKc12, HKc13, HKc14, HKc15
};


int
putHK(int chan, double val)
{
    int fld, no;

    if (chan < 0 || chan > 255)
    {
		errno = EINVAL;
		return (ERROR);
    }
	
    fld = chan / 16;
    no = chan % 16;

    HKc[fld][no] = val;
    return (OK);
}

int

chgHK(double val)
{
    int i;
    
    for (i=0;i<256;i++)
	putHK(i, val + i);

    return (OK);

}

int
prtHK(int chan)
{
    int  no;

    if (chan < 0 || chan > 16)
    {
		errno = EINVAL;
		return (ERROR);
    }
	
    for(no=0;no<16;no++)
    {	
		switch (chan)
		{
		  case 0: printf(" %d %d %f \n", chan, no, (double)HKc0[no]); break;
		  case 1: printf(" %d %d %f \n", chan, no, (double)HKc1[no]); break;
		  case 2: printf(" %d %d %f \n", chan, no, (double)HKc2[no]); break;
		  case 3: printf(" %d %d %f \n", chan, no, (double)HKc3[no]); break;
		  case 4: printf(" %d %d %f \n", chan, no, (double)HKc4[no]); break;
		  case 5: printf(" %d %d %f \n", chan, no, (double)HKc5[no]); break;
		  case 6: printf(" %d %d %f \n", chan, no, (double)HKc6[no]); break;
		  case 7: printf(" %d %d %f \n", chan, no, (double)HKc7[no]); break;
		  case 8: printf(" %d %d %f \n", chan, no, (double)HKc8[no]); break;
		  case 9: printf(" %d %d %f \n", chan, no, (double)HKc9[no]); break;
		  case 10: printf(" %d %d %f \n", chan, no, (double)HKc10[no]); break;
		  case 11: printf(" %d %d %f \n", chan, no, (double)HKc11[no]); break;
		  case 12: printf(" %d %d %f \n", chan, no, (double)HKc12[no]); break;
		  case 13: printf(" %d %d %f \n", chan, no, (double)HKc13[no]); break;
		  case 14: printf(" %d %d %f \n", chan, no, (double)HKc14[no]); break;
		  case 15: printf(" %d %d %f \n", chan, no, (double)HKc15[no]); break;
		}
    }

    return (OK);
}




