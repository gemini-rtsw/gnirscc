static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: clrError.c,v 1.2 2009/05/27 19:32:20 fkraemer Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * clrError.c
 *
 * DESCRIPTION 
 *
 * 
 * FUNCTION NAME(S)
 * ClrError()
 *   
 * DEPENDENCIES

 *
 *INDENT-OFF*
 * $Log: clrError.c,v $
 * Revision 1.2  2009/05/27 19:32:20  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:50  buchholz
 * Imported gnaacSrc into CVS
 *
 * Revision 1.2  1998/11/20 17:13:39  pruckle
 * speed up saver, log messages
 *
 * Revision 1.1.1.1  1998/09/30 16:40:29  pruckle
 * Initial Release
 *
 *INDENT-ON* 
 */


/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>
extern char *dbTop;
extern char *dbSadTop;
/* NAAC specific include files */
#include <epCommon.h>
#include <car.h>


/*
 *+
 * FUNCTION NAME:
 * clrError
 *
 * INVOCATION:
 * clrError();
 *
 * PARAMETERS: (">" input, "!" modified, "<" output)
 * None
 *
 * FUNCTION VALUE:
 * None
 *
 * PURPOSE:
 *  This function is used to clear the car and cad records.
 *
 * DESCRIPTION:
 * 
 *
 * EXTERNAL VARIABLES:
 * None
 *
 * PRIOR REQUIREMENTS:
 * 
 *
 * DEFICIENCIES:
 * None known
 *
 * HISTORY (optional):
 * 17-Mar-1997  Original version.			 Peter Ruckle
 *
 *-
 */
extern int epdebug;
/* #define DEBUG */
long clrError()
{
    char buf[80];


    cicsLogMessage(3," clear errors\n");
    /*  pvload(CLEAR_CARS,"top=" TOP ",sadtop=" TOP);*/

    setCar(APPLY_CAR,CAR_IDLE,OK,"",buf);
    setCar(ARSETUP_CAR,CAR_IDLE,OK,"",buf);
    setCar(DATUM_CAR,CAR_IDLE,OK,"",buf);
    setCar(DRROISET_CAR,CAR_IDLE,OK,"",buf);
    setCar(END_GUIDE_CAR,CAR_IDLE,OK,"",buf);
    setCar(END_OBSERVE_CAR,CAR_IDLE,OK,"",buf);
    setCar(END_VERIFY_CAR,CAR_IDLE,OK,"",buf);
    setCar(GSYS_CAR,CAR_IDLE,OK,"",buf);
    setCar(GUIDE_CAR,CAR_IDLE,OK,"",buf);
    setCar(NOOP_C,CAR_IDLE,OK,"",buf);
    setCar(OBSSETUP_CAR,CAR_IDLE,OK,"",buf);
    setCar(OBSERVE_CAR,CAR_IDLE,OK,"",buf);
    /* observedone*/
    setCar(PARK_CAR,CAR_IDLE,OK,"",buf);
    setCar(TEST_CAR,CAR_IDLE,OK,"",buf);
    setCar(VERIFY_CAR,CAR_IDLE,OK,"",buf);
    sprintf(buf,"top=%s,sadtop=%s",dbTop,dbSadTop);
    pvload(CLEAR_CADS,buf);
    return OK;
}
