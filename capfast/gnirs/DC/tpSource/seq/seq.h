/******************************************************************************
 * Program:	ValleyOaks01
 * File:	GoldFish.h
 * Purpose:	constants for the GoldFish sequencer
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		5-20-91	created						dak
 *		6-27-91	made static location globals			dak
 *		7-25-91	completed global vars & sequencer procs		dak
 *
 ******************************************************************************/

/* LastHdwVar is number of last global variable defined for all hardware */
#define lnr			gINT(LastHdwVar + 1)
#define coadds			gINT(LastHdwVar + 2)
#define inst_state		gINT(LastHdwVar + 3)
#define quadrant		gINT(LastHdwVar + 4)
#define ndavg			gINT(LastHdwVar + 5)
#define var1			gINT(LastHdwVar + 6)
#define var2			gINT(LastHdwVar + 7)
#define var3			gINT(LastHdwVar + 8)
#define var4			gINT(LastHdwVar + 9)
#define roisize			gINT(LastHdwVar + 10)
#define frames			gINT(LastHdwVar + 11)

#define RESETING	0
#define READ1		1
#define READ2		2


#define ONE		0
#define SEVEN		1
#define FIFTEEN		2



