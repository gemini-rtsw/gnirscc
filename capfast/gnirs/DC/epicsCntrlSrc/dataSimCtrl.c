static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: dataSimCtrl.c,v 1.2 2009/05/27 19:32:20 fkraemer Exp $"
};
 /*INDENT-OFF*
 * $Log: dataSimCtrl.c,v $
 * Revision 1.2  2009/05/27 19:32:20  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:50  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:13:42  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:28  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */
#include "epCommon.h"
extern char *dbTop;
char tmp[80];
/*
 *+
 * FUNCTION NAME:
 * dataSimCalc
 *
 * INVOCATION:
 *   status = dataSimCalc( struct genSubRecord *pgenSub );

 *   Parameters in:
 *      pgenSub->a    	long     pattern selector from mbbi record
 *      pgenSub->j	long     simulation enable from bi record
 *
 *   Parameters out:
 *      pgenSub->vala	long - pattern selector to hardware
 *      pgenSub->valb	long - simulation enable to hardware
 *

 * PURPOSE:
 *   This function enables proper simulation data patterns, based on the
 *   value of inpa, which selects a pattern, and inpb, which determines
 *   whether or not to enable ADC simulation.
 *
 * DESCRIPTION: 
 * This routine is called when the dataSimCalc GENSUB record is processed.
 * The data pattern is set to one of seven legal values, iff the enable
 * simulation value is set to true.  Otherwise, the pattern is ignored.
 *
 *  RETURN VALUE:
 *      < status      long        Status value
 *
 *   GLOBALS:
 *      External functions:
 *      None
 *

 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS: 
 *
 * DEFICIENCIES:
 * None
 *
 *   Author:
 *   Ken Ramey  (kramey@noao.edu)
 *
 *   History:
 *   06-Aug-1997: Original version.                           (kjr)
 *-

 * HISTORY:
 * 06-Aug-1997  Template ripped off from Nick Buchholz's arSizeCalc 
 *
 *-
 */

long dataSimCalc(struct genSubRecord *pgenSub)
{
    long pattern;		/* pattern selector from mbbi */
    long simEnable;		/* simulation enable from bi  */
    char cRegName[80];          /* name of record for control */
    char scbRegName[80];        /* scb register record        */

    char errMess[80];		/* error message ????	      */

    long creg;                  /* current control reg value  */
    long scbReg;                /* current scb register value */

    long status;                /* return codes               */

    long simPattern;		/* pattern to write to SCB    */
  
    pattern = *((long *)pgenSub->a);
    simEnable = *((long *)pgenSub->j);
    sprintf(tmp,"pat = %d, enb = %d \n", pattern,simEnable);
    cicsLogMessage(3,tmp);
    
    /* get current control register value                     */

    sprintf(cRegName, "%s%s.VAL",dbTop,SEQREG);
    creg = 0;
    status = putDbInfo(cRegName, errMess, DBF_LONG, &creg);
    status = getDbInfo(cRegName, errMess, DBF_LONG, &creg);
    sprintf(tmp,"creg = %8.8x\n", creg);
    cicsLogMessage(3,tmp);
    /* get current scb register value                         */
    sprintf(scbRegName, "%s%s.VAL",dbTop,SCBACC);
    scbReg = 0;
    status = putDbInfo(scbRegName, errMess, DBF_LONG, &scbReg);
    status = getDbInfo(scbRegName, errMess, DBF_LONG, &scbReg);
    sprintf(tmp,"scbreg = %8.8x\n",scbReg);
    cicsLogMessage(3,tmp);
    /* first need to see if simulation is enabled at all      */
    if (simEnable == 0)		/* means simulation not on    */
    {
      cicsLogMessage(3,"Sim off\n");
	scbReg = (scbReg & ~0x2b00);
	status = putDbInfoT(dbTop, SCBSET ".VAL", errMess, DBF_LONG, &scbReg);
	scbReg = scbReg | 0x0800;
	status = putDbInfoT(dbTop, SCBSET ".VAL", errMess, DBF_LONG, &scbReg);
	scbReg = scbReg & ~0x0800;
	status = putDbInfoT(dbTop, SCBSET ".VAL", errMess, DBF_LONG, &scbReg);
	creg =  (creg & ~0x0008);
	status = putDbInfoT(dbTop, SEQREGSET ".VAL", errMess, DBF_LONG, &creg);
    }
    else			/* simulation is on           */
    {
      cicsLogMessage(3,"Sim on\n");
	switch (pattern)
	{
	  case 0:
	  case 1:
	      simPattern = 0x0800;
	      break;

	  case 2:
	      simPattern = 0x0900;
	      break;

	  case 3:
	      simPattern = 0x2900;
	      break;

	  case 4:
	      simPattern = 0x0a00;
	      break;

	  case 5:
	      simPattern = 0x2a00;
	      break;

	  case 6:
	      simPattern = 0x0b00;
	      break;

	  case 7:
	      simPattern = 0x2b00;
	      break;

	}
	scbReg = (scbReg & ~0x2b00);
	status = putDbInfoT(dbTop, SCBSET ".VAL", errMess, DBF_LONG, &scbReg);
	scbReg = (scbReg | simPattern);
	status = putDbInfoT(dbTop, SCBSET ".VAL", errMess, DBF_LONG, &scbReg);
	scbReg = (scbReg & ~0x0800);
	status = putDbInfoT(dbTop, SCBSET ".VAL", errMess, DBF_LONG, &scbReg);
	creg = (creg | 0x0008);
	status = putDbInfoT(dbTop, SEQREGSET ".VAL", errMess, DBF_LONG, &creg);

    }
    return status;
}

long dataSimInit(struct genSubRecord *pgenSub)
{
    return (OK);
}


