/* VxWorks include files */
#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>

/* EPICS specific include files */
#include "epCommon.h"
#include "epicsNames.h"
#include "gnirsTasks.h"

/* Include file needed for control tasks */
#include <sysLib.h>
#include <car.h>
#include <genSubRecord.h>
#include <genSub.h>

int tcSet(struct cadRecord *pCad) {

  long status = CAD_ACCEPT;

  static double setpoint;
  static char pid[15];
 
  switch (DIRECTIVE) {

    case CAD_MARK:
      status = CAD_ACCEPT;
      break;
      
    case CAD_CLEAR:
      status = CAD_ACCEPT;
      break;
      
    case CAD_PRESET:

      printf("Setpoint 2 input: %s\n", pCad->a);
      setpoint = atof(pCad->a);

      printf("PID gain input: %s, %s, %s\n", pCad->b, pCad->c, pCad->d );
      sprintf(pid, "%s, %s, %s", pCad->b, pCad->c, pCad->d);

      printf("PID output: %s\n", pid);
      
      status = CAD_ACCEPT;
      break;

    case CAD_START:
      *(double *)pCad->vala = setpoint;
      strcpy( (char *)pCad->valb, pid) ;
      status = CAD_ACCEPT;
      break;

    case CAD_STOP:
      status = CAD_ACCEPT;
      break;
  }
  return status;
}

int CADcsetin(struct cadRecord *pCad) {

  long status = CAD_ACCEPT;
  static char csetinstr[20]; 
  static long units, pwrup, curpwr;
 
  switch (DIRECTIVE) {

    case CAD_MARK:
      status = CAD_ACCEPT;
      break;
      
    case CAD_CLEAR:
      status = CAD_ACCEPT;
      break;
      
    case CAD_PRESET:

	/*
      if ( strcmp(pCad->b, "SelectUnits") == 0) {
	sprintf(MESSAGE, "Select units first");		
	LOG_MSG(ERROR_MSG, MESSAGE);
	status = CAD_REJECT;
	break;
      }
      
      else if ( strcmp(pCad->b, "kelvin") == 0) units = 1;
      else if ( strcmp(pCad->b, "Celsius") == 0) units = 2;
      else if ( strcmp(pCad->b, "sensor units") == 0) units = 3;
      else {
	sprintf(MESSAGE, "TC -Cset: No unit string match");		
	LOG_MSG(ERROR_MSG, MESSAGE);
	status = CAD_REJECT;
	break;
      }

      if ( strcmp(pCad->c, "OFF") == 0) pwrup = 0;
      else if ( strcmp(pCad->c, "ON") == 0) pwrup = 1;
      else {
	sprintf(MESSAGE, "TC -Cset: No pwrup string match");		
	LOG_MSG(ERROR_MSG, MESSAGE);
	status = CAD_REJECT;
	break;
      }

      if ( strcmp(pCad->d, "current") == 0) curpwr = 1;
      else if ( strcmp(pCad->d, "power") == 0) curpwr = 2;
      else {
	sprintf(MESSAGE, "TC -Cset: No curpwr string match");		
	LOG_MSG(ERROR_MSG, MESSAGE);
	status = CAD_REJECT;
	break;
      }
*/

      sscanf(pCad->b, "%ld", &units);
      sscanf(pCad->c, "%ld", &pwrup);
      sscanf(pCad->d, "%ld", &curpwr);
      sprintf(csetinstr, "%s, %ld, %ld, %ld", pCad->a, units, pwrup, curpwr);

      printf("csetinstr output: %s\n", csetinstr);
      
      status = CAD_ACCEPT;
      break;

    case CAD_START:
      strcpy( (char *)pCad->vala, csetinstr) ;
      status = CAD_ACCEPT;
      break;

    case CAD_STOP:
      status = CAD_ACCEPT;
      break;
  }
  return status;
}

int parseCset(genSubRecord *sub) {

  long status = OK;
  static char input;
  static long units, powerupEnable, currentOrPower;

  sscanf(sub->a, "%c, %ld, %ld, %ld", &input, &units, &powerupEnable, &currentOrPower);
    
  printf("Parsed CSET: %c:%ld:%ld:%ld\n", input, units, powerupEnable, currentOrPower); 

  /* Show which input to control from and also post flags to 
   * valf and valg for a PV flag representing which control is active.
   * */
  *(char *) sub->vala = input;
  if (input == 'A') {
    *(long *) sub->valf = 1; /*A is control*/
    *(long *) sub->valg = 0; 
  }
  else { /* switch PV flags for control*/
    *(long *) sub->valf = 0;
    *(long *) sub->valg = 1; /*B is control*/
  }

  /* Parse input arg 2 to show appropriate units*/
  switch (units) {

    case 1:
      strncpy((char *) sub->valb, "K",1);
      break;

    case 2:
      strncpy((char *) sub->valb, "Celsius",7);
      break;

    case 3:
      strncpy((char *) sub->valb, "sensor units",12);
      break;

    default:
      strncpy((char *) sub->vale, "CSET: Invalid units", 16);
      status = ERROR;

      break;

  }

  /* Power up enable message*/
  if (powerupEnable == 1)
    strncpy((char *) sub->valc, "ON",4);
  else
    strncpy((char *) sub->valc, "OFF",4);
    
  /* Show if heater output displays current or power */
  if (currentOrPower == 1)
    strncpy ((char *) sub->vald, "current", 7);
  else
    strncpy ((char *) sub->vald, "power", 7);

  return status;
}


int parseGain(genSubRecord *sub) {

  long status = OK;
  static double gain, reset, rate;

  sscanf(sub->a, "%lf, %lf, %lf", &gain, &reset, &rate);
    
  printf("Parsed PID: %f:%f:%f", gain, reset, rate); 

  *(double *) sub->vala = gain;
  *(double *) sub->valb = reset;
  *(double *) sub->valc = rate;
  return status;
}


int setRamp(genSubRecord *sub) {

  long status = OK;
  
  long rampswitch = 0;
  double rampval = 0.0;
  
  rampswitch =  *(long *)sub->a; 
  rampval = *(double *)sub->b;

  printf("rampswitch: %d, rampval: %f\n", rampswitch, rampval);
  sprintf((char *)sub->vala, "%d, %f", rampswitch, rampval);
  return status;
}



