static struct {
	void *v;
	char *c;
} rcsid = {
	&rcsid,
	"$Id: epicsCA.c,v 1.1 2009/06/10 15:05:11 gemvx Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * epicsCA.c
 *
 * DESCRIPTION 
 *	Functions that set sir records with data from detector controller
 * 
 * FUNCTION NAME(S)
 *	updateAstCtx
 *
 * DEPENDENCIES
 * SDSULib and EPICS support functions.
 *
 *
 *INDENT-OFF*
 * $Log: epicsCA.c,v $
 * Revision 1.1  2009/06/10 15:05:11  gemvx
 * Added Files:
 * epicsControl diectory populated.
 *
 *INDENT-ON* 
 */
#define NODBACCESS
#include "epCommon.h"
#include <genSubRecord.h> 
#include "gnirsCC.h"
#include "epicsDefines.h"
#include "epicsNames.h"
#include "epicsCA.h"
#include "astLib.h"
long Dynamic = 0;
long sadwatch = 1;
extern long TCS;

/*
 *+
 * FUNCTION NAME:
 *	updateAstCtx
 *
 * INVOCATION:
 *	called when "astCtx"  gensub is processed
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 *	none
 * FUNCTION VALUE:
 *	status
 *
 * PURPOSE:
 *	Get context from TCS.
 *
 * DESCRIPTION:
 *	Tries to get ast context from TCS.  If it fails, it just sets a 
 *		global variable.  
 *
 * EXTERNAL VARIABLES:
 *	TCS
 *
 * PRIOR REQUIREMENTS:
 *
 * DEFICIENCIES:
 *
 * HISTORY (optional):
 *	4 Sept 99		Initial version ( from icd 1.9.3/1.9.d.5) 
 *	26 Oct 99		Changed to getEpics 
 *
 *-
 */
#include "alarm.h"
long updateAstCtx(struct genSubRecord *pgsub)
{
 
    if(pgsub->sevr != INVALID_ALARM)
    { 
	TCS = 1; 
	astSetctx(pgsub->a); 
    } 
    else 
    { 
	TCS = 0 ;
	return ERROR; 
    } 

   /*  if(getEpics("tcs:ak:astCtx.VALA",ctxa,AST_CTXA_SIZE,&ch) == OK) */
/*     { */
/* 	TCS = 1; */
/* 	printf("TCS found\n"); */
/* 	astSetctx(ctxa); */
/*     } */
/*     else */
/*     { */
/* 	SDSU_LOG(SDSU_ERROR,"ERROR - TCS disconnected\n"); */
/* 	TCS = 0; */
/* 	return ERROR; */
/*     } */

    return OK;
}
