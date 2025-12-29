
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "NIRSfocusCommon.h"
#include  <dbAccess.h>
#include  <logLib.h>

NISfocusConfDef *focusSBspectral=NULL;
NISfocusConfDef *focusSRspectral=NULL;
NISfocusConfDef *focusLBspectral=NULL;
NISfocusConfDef *focusLRspectral=NULL;

NISfocusConfDef *focusSBspatial=NULL;
NISfocusConfDef *focusSRspatial=NULL;
NISfocusConfDef *focusLBspatial=NULL;
NISfocusConfDef *focusLRspatial=NULL;

NISfocusConfDef *focusSBimaging=NULL;
NISfocusConfDef *focusSRimaging=NULL;
NISfocusConfDef *focusLBimaging=NULL;
NISfocusConfDef *focusLRimaging=NULL;

/* Assumes that focusStruct is has been created, but *NOT* filled out - ie. memory for filter should not have been allocated yet. */

void deleteFocusConfArray(NISfocusConfDef **ppFocusConf)
{
        int i;

	for (i = 0; (*ppFocusConf)[i].filter1; ++i) {
            free((*ppFocusConf)[i].filter1);
            free((*ppFocusConf)[i].filter2);
        }
	free(*ppFocusConf);
	*ppFocusConf = NULL;
}

/* Returns the number of items that were parsed or -1 if encountered an error. */

int createFocusConfArray(const char *fileName, NISfocusConfDef **ppFocusConf)
{
        int i;
	FILE *pFile;
	char line[LINE_LEN];
        int items  = 0;					/* Number of entries found	*/
        struct tmpGp {					/* Can't do this as an autovar	*/
	    char fN[33];				/* This is unused!	*/
	    char f1N[33];				/* Size must match sscanf below	*/
	    char f2N[33];				/* Size must match sscanf below	*/
            int a[12];					/* Number must match sscanf	*/
        } *tmpGp;

        tmpGp = (struct tmpGp *)malloc(sizeof(struct tmpGp) * MAX_FILTERS_PER_GRISM);

	if (!(pFile = fopen(fileName, "r" ))) {
		logMsg("Error opening file '%s'.\n", (int) (char *)fileName, 0, 0, 0, 0, 0);
                free(tmpGp);
		return NIRS_FOCUS_ERROR;
	}
        for (i = 0; i < MAX_FILTERS_PER_GRISM - 1; ) {
                struct tmpGp *tGp = tmpGp + i;
		if (feof(pFile)) break;

		if (fgets(line, LINE_LEN, pFile)!=NULL) {
			/* Ignore comments and empty lines. */
			if (line[0] == '#'|| strlen(line) < 5) continue;
                        if (sscanf(line, "\"%32[^\"]\" \"%32[^\"]\" \"%32[^\"]\" %d %d %d %d %d %d %d %d %d %d %d %d",
				tGp->fN, tGp->f1N, tGp->f2N,
		                &tGp->a[0], &tGp->a[1], &tGp->a[2],
		                &tGp->a[3], &tGp->a[4], &tGp->a[5],
		                &tGp->a[6], &tGp->a[7], &tGp->a[8],
		                &tGp->a[9], &tGp->a[10], &tGp->a[11]) != 15) {
			    logMsg("Line: \"%s\": Bad focus format...\n", (int) (char *)line, 0, 0, 0, 0, 0);
	                    fclose(pFile);
                            free(tmpGp);
			    return NIRS_FOCUS_ERROR;
			}
                        ++i; 	/* Successfully parsed focus values, count it.	*/
		}
	}

	/* Cleanup.*/
	fclose(pFile);
        items = i;

        if (items == MAX_FILTERS_PER_GRISM) {
	    logMsg("File: \"%s\": Too many entries.\n", (int) fileName, 0, 0, 0, 0, 0);
            free(tmpGp);
	    return NIRS_FOCUS_ERROR;
        }

        if (items == 0) {
	    logMsg("File: \"%s\": Found NO entries.\n", (int) fileName, 0, 0, 0, 0, 0);
            free(tmpGp);
	    return NIRS_FOCUS_ERROR;
        }

	if (*ppFocusConf != NULL) {
	    logMsg("Configuration already exists. Cleaning up before updating.\n", 0, 0, 0, 0, 0, 0);
	    deleteFocusConfArray((NISfocusConfDef **) ppFocusConf);
	}

        *ppFocusConf = (NISfocusConfDef *)malloc(sizeof(NISfocusConfDef) * (items + 1));
        memset(*ppFocusConf, 0, sizeof(NISfocusConfDef) * (items + 1));	/* This will make last item NULL */
        for (i = 0; i < items; ++i) {
            struct tmpGp *tGp = tmpGp + i;
            int j;
	    (*ppFocusConf)[i].filter1 = (char *)malloc(strlen(tGp->f1N + 1)); strcpy((*ppFocusConf)[i].filter1, tGp->f1N);
	    (*ppFocusConf)[i].filter2 = (char *)malloc(strlen(tGp->f2N + 1)); strcpy((*ppFocusConf)[i].filter2, tGp->f2N);
            for (j = 0; j < PRISM_GRATING_ID_MAX; ++j) (*ppFocusConf)[i].focus[j] = tGp->a[j];
	}
        free(tmpGp);
	return items;
}

/* This is a "public" function that should be mostly called by epics/user code. */
int updateFocusConfArray(const char *name)
{
	int status=SUCCESS;
	int recNum=0;
	/*Need to account for extension ".dat". and prefix DAT_FILE_DIR */
	char *fileName = (char *)malloc(strlen(name)+strlen(DAT_FILE_DIR)+5);

	/* Build the file name to where the lookup table is. */ 
	strcpy(fileName, DAT_FILE_DIR); 
	/*strcpy(fileName, "./"); */
	strcat(fileName, name);
	strcat(fileName, ".dat");

	if (!strcmp(name,"focusSB-spectral"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusSBspectral); 
	}

	else if (!strcmp(name,"focusSR-spectral"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusSRspectral); 

	}   
	else if (!strcmp(name,"focusLB-spectral"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusLBspectral); 

	}   
	else if (!strcmp(name,"focusLR-spectral"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusLRspectral); 

	}   
   
   /*  Focus Spatial Group*/
	else if (!strcmp(name,"focusSB-spatial"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusSBspatial); 
	}

	else if (!strcmp(name,"focusSR-spatial"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusSRspatial); 

	}   
	else if (!strcmp(name,"focusLB-spatial"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusLBspatial); 

	}   
	else if (!strcmp(name,"focusLR-spatial"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusLRspatial); 

	}   

   /*  Focus Imaging Group*/
	else if (!strcmp(name,"focusSB-imaging"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusSBimaging); 
	}

	else if (!strcmp(name,"focusSR-imaging"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusSRimaging); 

	}   
	else if (!strcmp(name,"focusLB-imaging"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusLBimaging); 

	}   
	else if (!strcmp(name,"focusLR-imaging"))
	{
		logMsg("Filling out '%s' from file '%s'..\n", (int) (char *)name, (int) (char *)fileName, 0, 0, 0, 0);
		recNum=createFocusConfArray(fileName, (NISfocusConfDef **) &focusLRimaging); 

	} 	else
	{
		logMsg("Name '%s' not understood.\n", (int) (char *)name, 0, 0, 0, 0, 0);
		status=NIRS_FOCUS_ERROR;
	}


	if (recNum <0)
	{
		logMsg("Something went wrong during parsing...\n", 0, 0, 0, 0, 0, 0);
		status=NIRS_FOCUS_ERROR;
	}

	free(fileName);

	return status;
}

static void printFocusConf (NISfocusConfDef *pFocusConf)
{
	int i;

	printf("%s\t%s", (*pFocusConf).filter1, (*pFocusConf).filter2);
	for (i = 0; i < PRISM_GRATING_ID_MAX; ++i)
	    printf("\t%d", (*pFocusConf).focus[i]);
}


/* This is a "public" function that should be mostly called by epics/user code. */
void printAllFocusConfArrays()
{
	int i=0;
	int i2=0;
	NISfocusConfDef **focConfPointerArray[] = {  
		&focusSBspectral, &focusSRspectral, &focusLBspectral, &focusLRspectral, 
		&focusSBspatial,  &focusSRspatial,  &focusLBspatial,  &focusLRspatial,
		&focusSBimaging,  &focusSRimaging,  &focusLBimaging,  &focusLRimaging 
	};


	for (i = 0; i < (sizeof(focConfPointerArray) / sizeof(NISfocusConfDef*)); i++) {
		printf("\n------------------------------------------------\n");
		if (*focConfPointerArray[i] == NULL) {
			printf("Focus Configuration %d is NULL\n", i); 
		}
		else {
			printf("Focus Configuration %d\n", i); 
			printf("FW1\tFW2|\tG10MIR|G32MIR|G111MIR	|G10LXD|G32LXD|G111LXD	|G10SXD|G32SXD|G111SXD	|G10WOLL|G32WOLL|G111WOLL\n");
                        for (i2 = 0; (*focConfPointerArray[i])[i2].filter1 != NULL; ++i2) {  
				printFocusConf(&(*focConfPointerArray[i])[i2]);	
				printf("\n");  
			}
		}
	}
	printf("\n");

} 

/* Spectral update: This is a "public" function that should be mostly called by epics/user code. */
int updateAllSpectralFocusConfArrays()
{
	int status=SUCCESS;
	int i=0;

	const char *cameras[]={
		"focusSB-spectral",  
		"focusSR-spectral",
		"focusLB-spectral",
		"focusLR-spectral"
	};  
	for (i=0; i<sizeof(cameras)/sizeof(char *); i++)
	{
		status=updateFocusConfArray(cameras[i]);
		if (status!=SUCCESS)
		{  
			printf("Failed to update focus LUT: %s\n",cameras[i]);
			break;
		}
	}
	return status;
}


/* Spatial update: This is a "public" function that should be mostly called by epics/user code. */
int updateAllSpatialFocusConfArrays()
{
	int status=SUCCESS;
	int i=0;

	const char *cameras[]={
		"focusSB-spatial",  
		"focusSR-spatial",
		"focusLB-spatial",
		"focusLR-spatial"
	};  
	for (i=0; i<sizeof(cameras)/sizeof(char *); i++)
	{
		status=updateFocusConfArray(cameras[i]);
		if (status!=SUCCESS)
		{  
			printf("Failed to update focus LUT: %s\n", cameras[i]);
			break;
		}
	}

	return status;
}

/* Imaging update: This is a "public" function that should be mostly called by epics/user code. */
int updateAllImagingFocusConfArrays()
{
	int status=SUCCESS;
	int i=0;

	const char *cameras[]={
		"focusSB-imaging",  
		"focusSR-imaging",
		"focusLB-imaging",
		"focusLR-imaging"
	};  
	for (i=0; i<sizeof(cameras)/sizeof(char *); i++)
	{
		status=updateFocusConfArray(cameras[i]);
		if (status!=SUCCESS)
		{  
			printf("Failed to update focus LUT: %s\n",cameras[i]);
			break;
		}
	}

	return status;
}



/**********************************************************
  Stuff below only used for testing.
 **********************************************************/

#ifdef NIS_FOC_TESTING
int main(int argc , char *argv[])
{
	int status;

	status=updateFocusConfArray("focusSB-spectral");
	status=updateFocusConfArray("focusSR-spectral");
	status=updateFocusConfArray("focusLB-spectral");
	status=updateFocusConfArray("focusLR-spectral");
        printAllFocusConfArrays();
	return status;
} 
#endif

