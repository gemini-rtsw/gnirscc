static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: naacGlobals.c,v 1.2 2009/05/27 19:32:21 fkraemer Exp $"
};
/*
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * naacGlobals.c
 *
 * DESCRIPTION
 * This file contains the definitions of global variables used in the
 * GNAAC software system.  It was necessary to place all these variables
 * in one file and load them prior to other files which use them.
 *
 * FUNCTION NAME(S)
 * None
 *
 *INDENT-OFF*
 * $Log: naacGlobals.c,v $
 * Revision 1.2  2009/05/27 19:32:21  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:18:51  buchholz
 * Imported gnaacSrc into CVS
 *
 *INDENT-ON* 
 */


#include <semLib.h>

SEM_ID semArSetup = NULL;
SEM_ID semObsSetup = NULL;
SEM_ID semDrRoiSet = NULL;
SEM_ID semTest = NULL;
SEM_ID semInit = NULL;
SEM_ID semSetWcs = NULL;
SEM_ID semSetDhsInfo = NULL;
SEM_ID semObserve = NULL;
SEM_ID semPark = NULL;
SEM_ID semAbort = NULL;
SEM_ID semStop = NULL;

char tldFile[256];
char cmdFile[256];
