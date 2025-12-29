#include "vxWorks.h"
#include "string.h"
#include "stdlib.h"

#include "cad.h"
#include "cadRecord.h"

#include "db_access.h"     /* for stuff like DBF_LONG */

#include "stdio.h"

#define DIRECTIVE pcad->dir
#include "NIRSfocusLoadConfProc.h"

long seqfocusLoadConfProc (struct cadRecord *pcad) 
{
  char msg[40]="";
  long status = CAD_ACCEPT;

  switch(DIRECTIVE) 
  {
    case CAD_MARK:	
      break;
    case CAD_CLEAR:	
      break;
    case CAD_PRESET:
      break;
    case CAD_START:	

      printf("NISRfocusLoadLutProc: START - trying to load %s.\n",pcad->a);
      /* Maybe should check the integrity of pcad->a here ?! */   


      if (!strcmp("All-spectral", pcad->a) || !strcmp("ALL-spectral", pcad->a) || !strcmp("all-spectral", pcad->a))
      {
	/* Want to update all the config arrays in one go. */
	status=updateAllSpectralFocusConfArrays(); 
      }
      else if (!strcmp("All-spatial", pcad->a) || !strcmp("ALL-spatial", pcad->a) || !strcmp("all-spatial", pcad->a))
      {
	/* Want to update all the config arrays in one go. */
	status=updateAllSpatialFocusConfArrays(); 
      }
      else if (!strcmp("All-imaging", pcad->a) || !strcmp("ALL-imaging", pcad->a) || !strcmp("all-imaging", pcad->a))
      {
	/* Want to update all the config arrays in one go. */
	status=updateAllImagingFocusConfArrays(); 
      }
      else
      {
	status=updateFocusConfArray(pcad->a);    
      }

      if (status<0)
      {
	sprintf(msg, "Error loading '%s'",pcad->a);
	status = CAD_REJECT;
      }
      else
      {
	sprintf(msg, "Successfully loaded '%s'",pcad->a);
	printf("NISfocusLoadLutProc: START - Successfully updated '%s'.\n",pcad->a);
      }    

      break;	

    case CAD_STOP:	
      break;
    default:
      status = CAD_REJECT;
      strcpy(msg,"NIRSfocusLoadLutProc: invalid DIR");	
      break;
  }

  if (status == CAD_ACCEPT)
  {
    strcpy(pcad->mess,msg);
  }

  else if (status == CAD_REJECT) 
  {
    strcpy(pcad->mess,msg);
  }
  return status;
}
