#include "vxWorks.h"
#include "string.h"
#include "stdlib.h"

#include "cad.h"
#include "cadRecord.h"
#include "logLib.h"

#include "stdio.h"
#include "NIRSfocusProc.h"

char focprocFilter1[MAX_FILTERS_PER_GRISM]   = "undef";
char focprocGrating[MAX_FILTERS_PER_GRISM]   = "undef";
char focprocCamera[MAX_FILTERS_PER_GRISM]    = "undef";
char focprocPrism[MAX_FILTERS_PER_GRISM]     = "undef";
char focprocFilter2[MAX_FILTERS_PER_GRISM]   = "undef";
char focprocAcqPos[MAX_FILTERS_PER_GRISM]    = "undef";
int focprocBypass      = -1;
int focprocFocusResult = -1;
int focprocTableRow    = -1;
int focprocTableCol    = -1;
char focprocTable[MAX_FILTERS_PER_GRISM] = "undef";


void focprocHelp() {
  printf("focprocReport --Shows current inputs used to find Focus Demand.\n");
  printf("printAllFocusConfArrays -- Prints all 12 tables as follows: \n");
  printf("\tFocus Config 0 --> SBspectral\n");
  printf("\tFocus Config 1 --> SRspectral\n");
  printf("\tFocus Config 2 --> LBspectral\n");
  printf("\tFocus Config 3 --> LRspectral\n");
  printf("\tFocus Config 4 --> SBspatial\n");
  printf("\tFocus Config 5 --> SRspatial\n");
  printf("\tFocus Config 6 --> LBspatial\n");
  printf("\tFocus Config 7 --> LRspatial\n");
  printf("\tFocus Config 8 --> SBimaging\n");
  printf("\tFocus Config 9 --> SRimaging\n");
  printf("\tFocus Config 10 --> LBimaging\n");
  printf("\tFocus Config 11 --> LRimaging\n");
}

void focprocReport() {

  printf("-----Focus Process Report-----\n");
  printf("Filter2:\t>%s<\n", focprocFilter2);
  printf("Grating:\t>%s<\n", focprocGrating);
  printf("Camera:\t\t>%s<\n", focprocCamera);
  printf("Prism:\t\t>%s<\n", focprocPrism);
  printf("Filter1:\t>%s<\n", focprocFilter1);
  printf("AcqMirPos:\t>%s<\n", focprocAcqPos);
  printf("Bypass?: \t>%d<\n", focprocBypass);
  printf("Focus val:\t>%d<\n", focprocFocusResult);
  printf("focprocTable:\t>%s<\n", focprocTable);
  printf("focprocTableRow: >%d<\n", focprocTableRow);
  printf("focprocTableCol: >%d<\n", focprocTableCol);
}


long NIRSfocusProc (struct cadRecord *pcad) 
{
   char msg[40];
   long status = CAD_REJECT;
   int i = 0;
   GnirsFocusMode gnirsFocusMode = SPECTRAL;
   GnirsPrismGratingPair prismGratingType;
   NISfocusConfDef *fs = NULL;
   char *ft = NULL;

   switch(pcad->dir) {

      case CAD_PRESET:
	 logMsg("NIRS - PRESET focus for filter2=%s grating=%s camera=%s prism=%s filter1=%s acqPos=%s\n",
	       (int) (char *)pcad->a,		/* FW2 */
	       (int) (char *)pcad->b,
	       (int) (char *)pcad->c, 
	       (int) (char *)pcad->f, 
	       (int) (char *)pcad->g, 		/* FW1 */
	       (int) (char *)pcad->h);

	 strcpy(focprocFilter2,  (char *)pcad->a);	/* FW2 */
	 strcpy(focprocGrating,  (char *)pcad->b);
	 strcpy(focprocCamera,   (char *)pcad->c);
	 strcpy(focprocPrism,    (char *)pcad->f);
	 strcpy(focprocFilter1,  (char *)pcad->g);	/* FW1 */
	 strcpy(focprocAcqPos,   (char *)pcad->h);

	 if(! pcad->h[0]) {
	    printf("BEST focus says not datumed\n");
	    break;	
	 }

	 else if ( !strcmp(pcad->h, ACQ_POS_OUT)) {
	    gnirsFocusMode = SPECTRAL;
	 }
	 else if ( !strcmp(pcad->h, ACQ_POS_IN)) {
	    gnirsFocusMode = IMAGING;
	    prismGratingType = 0;  /*Use first column in table*/
	 }
	 /* NO SPATIAL MODE SUPPORT FOR NOW
	    else if ( focusModeIn == SPATIAL) {
	    gnirsFocusMode = SPATIAL;
	 } */
	 else {
	    strcpy(msg,"Focus needs spectral, spatial or imaging");
	    printf("%s\n", msg);
	    break;
	 }

	 if((! pcad->a[0]) || (! pcad->b[0]) || (! pcad->c[0]) || (! pcad->f[0])) {
	    strcpy(msg,"Need filt, grat, camera or prism.");
	    printf("%s\n", msg); 
	    break;
	 }

	 /* Need to find column into table if NOT in imaging mode.*/
	 if (gnirsFocusMode != IMAGING) {
	    /* Get prism grating combo */
	    /* Check for G10 */
	    if ((strstr(pcad->b,FOCUS_G10SB) != NULL)  ||  
		(strstr(pcad->b,FOCUS_G10LB) != NULL)  ||  
		(strstr(pcad->b,FOCUS_G10LR) != NULL)  ||  
		(strstr(pcad->b,FOCUS_G10LXD) != NULL) ||  
		(strstr(pcad->b,FOCUS_G10SXD) != NULL) ) 
	    {
	       /* Found G10 */
	       if (strstr(pcad->f,PRISM_MIR) != NULL)
		  prismGratingType = G10MIR;

	       else if (strstr(pcad->f,PRISM_LXD) != NULL)
		  prismGratingType = G10LXD;

	       else if (strstr(pcad->f,PRISM_SXD) != NULL)
		  prismGratingType = G10SXD;

	       else if (strstr(pcad->f,PRISM_WOLL) != NULL)
		  prismGratingType = G10WOLL;

	       else {
		  strcpy(msg,"Failed to locate low-res prism index");
	          printf("%s\n", msg);
		  break;
	       }
	    } 

	    /* Check for G32 */
	    else if ((strstr(pcad->b,FOCUS_G32SB) != NULL) ||  
		  (strstr(pcad->b,FOCUS_G32SR) != NULL) ||  
		  (strstr(pcad->b,FOCUS_G32LB) != NULL) ||  
		  (strstr(pcad->b,FOCUS_G32LR) != NULL) ) 
	    {
	       /* Found G32*/
	       if (strstr(pcad->f,PRISM_MIR) != NULL)
		  prismGratingType = G32MIR;

	       else if (strstr(pcad->f,PRISM_LXD) != NULL)
		  prismGratingType = G32LXD;

	       else if (strstr(pcad->f,PRISM_SXD) != NULL)
		  prismGratingType = G32SXD;

	       else if (strstr(pcad->f,PRISM_WOLL) != NULL)
		  prismGratingType = G32WOLL;

	       else {
		  strcpy(msg,"Failed to locate med-res prism index");
	          printf("%s\n", msg);
		  break;
	       }
	    }

	    /* Check for G111 */
	    else if ((strstr(pcad->b,FOCUS_G111SB) != NULL) ||  
		  (strstr(pcad->b,FOCUS_G111SR) != NULL) ||  
		  (strstr(pcad->b,FOCUS_G111LB) != NULL) ||  
		  (strstr(pcad->b,FOCUS_G111LR) != NULL) ) 
	    {
	       /* Found G111*/
	       if (strstr(pcad->f,PRISM_MIR) != NULL)
		  prismGratingType = G111MIR;

	       else if (strstr(pcad->f,PRISM_LXD) != NULL)
		  prismGratingType = G111LXD;

	       else if (strstr(pcad->f,PRISM_SXD) != NULL)
		  prismGratingType = G111SXD;

	       else if (strstr(pcad->f,PRISM_WOLL) != NULL)
		  prismGratingType = G111WOLL;

	       else {
		  strcpy(msg,"Failed to locate hi-res prism index");
	          printf("%s\n", msg);
		  break; 
	       }
	    }

	    /* Grating not found */
	    else 
	    {
	       /* should never happen */
	       strcpy(msg,"NISfocusProc grating name mismatch");
	       printf("%s\n", msg);
	       break;
	    }

	 } /*End if ! IMAGING */

	 /* Check whether to set the focus manually, or use "best focus" (ie, values from the luts). */
	 if (strcmp(pcad->e, BEST_FOCUS_DIRECTIVE))
	 {
	    /* Don't use the 'best focus'. Take the value of pcad->e to be the focus. */
	    status =  CAD_ACCEPT;
	    strcpy(pcad->valb, pcad->e);		 	
            break;
	 }
	 else if (!strcmp(pcad->b,FOCUS_BLANK)) 
	 {
	    /* same focus */
	    status =  CAD_ACCEPT;
	    strcpy(pcad->valb,pcad->d);
            break;
	 }

         if (!strcmp(pcad->c, FOCUS_SB)) {	/* SHORT_BLUE    */
             switch(gnirsFocusMode) {
                 case SPECTRAL: ft = "SBSpectral"; fs = focusSBspectral; break;
                 case SPATIAL:  ft = "SBSpatial";  fs = focusSBspatial;  break;
                 case IMAGING:  ft = "SBImaging";  fs = focusSBimaging;  break;
             }
         }
	 else if (!strcmp(pcad->c, FOCUS_SR)) {	/* SHORT RED     */
             switch(gnirsFocusMode) {
                 case SPECTRAL: ft = "SRSpectral"; fs = focusSRspectral; break;
	         case SPATIAL:  ft = "SRSpatial";  fs = focusSRspatial;  break;
	         case IMAGING:  ft = "SRImaging";  fs = focusSRimaging;  break;
             }
         }
	 else if (!strcmp(pcad->c, FOCUS_LB)) {	/* LONG BLUE     */
             switch(gnirsFocusMode) {
                 case SPECTRAL: ft = "LBSpectral"; fs = focusLBspectral; break;
	         case SPATIAL:  ft = "LBSpatial";  fs = focusLBspatial;  break;
	         case IMAGING:  ft = "LBImaging";  fs = focusLBimaging;  break;
             }
         }
	 else if (!strcmp(pcad->c, FOCUS_LR)) {	/* LONG RED      */
             switch(gnirsFocusMode) {
                 case SPECTRAL: ft = "LRSpectral"; fs = focusLRspectral; break;
	         case SPATIAL:  ft = "LRSpatial";  fs = focusLRspatial;  break;
	         case IMAGING:  ft = "LRImaging";  fs = focusLRimaging;  break;
             }
         }
         else {
	     strcpy(msg,"NIRSfocusProc unknown camera");
	     printf("%s\n", msg);
             break;
         }

         if (fs == NULL) {
             strcpy(msg,"Focus has not been initialized.");
	     printf("%s\n", msg);
             break;
         }
         else {
	     memcpy(focprocTable, fs, sizeof(focprocTable));
	     printf("Using %s focus table.\n", ft);
             for(i = 0; fs[i].filter1 != NULL; ++i) {
                 printf("Does fw1: %s == %s and fw2: %s == %s ?\n", fs[i].filter1, pcad->g, fs[i].filter2, pcad->a);
                 if (((fs[i].filter1[0] == '*') || !strcmp(fs[i].filter1, pcad->g)) && 
                     ((fs[i].filter2[0] == '*') || !strcmp(fs[i].filter2, pcad->a))) {
	             printf("Match: focus %d at table coordinates row=%d, col=%d\n", fs[i].focus[prismGratingType], i, prismGratingType);
                     status = CAD_ACCEPT;
                     sprintf(pcad->valb,"%d", fs[i].focus[prismGratingType]);
	             break;
                 }
             }
             if (fs[i].filter1 == NULL) {
	         strcpy(msg,"No filter found in LUT.");
	         printf("%s\n", msg);
	         break;
             }
         }

         /* disable if already in place */
         if (status == CAD_ACCEPT) {
	      if (atol(pcad->d) == atol(pcad->valb)) {
	          *(long *) pcad->vala = 1;
	          focprocBypass = 1;	
	      }
	      else {
	          *(long *) pcad->vala = 0;
	          focprocBypass = 0;	
	      }
          }

	 logMsg("focus=%s prev=%s disabled=%ld icounter=%d\n", (int) (char *) pcad->valb, (int) (char *)pcad->d, *(long *) pcad->vala, i, 0, 0);
	 focprocTableRow = i;
	 focprocTableCol = prismGratingType;
	 focprocFocusResult = atoi(pcad->valb); 
	 break;			

      case CAD_MARK:	
      case CAD_CLEAR:	
      case CAD_START:	
      case CAD_STOP:	
	 status = CAD_ACCEPT;
	 break;

      default:
	 strcpy(msg,"NISfocusProc invalid DIR");	
	 printf("%s\n", msg);
	 break;
   }

   if (status == CAD_REJECT) 
   {
      strcpy(focprocTable, "REJECT");
      focprocTableRow = -99;
      focprocTableCol = -99;
      strcpy(pcad->mess,msg);
   }
   return status;
}
