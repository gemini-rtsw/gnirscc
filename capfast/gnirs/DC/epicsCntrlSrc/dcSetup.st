program dcSetup                    

/* The following option is needed to make the code reentrant   */
/* not needed here                                             */
/* option +r;                                                  */

%%#include "string.h"

%%#include "cad.h"
%%#include "car.h"
%%#include "gemLogMsg.h"

short entering_state;
short step;

#define ARSETUP   0
#define OBSSETUP  1
#define ROISETUP  2
#define ALLSETUP  3
string  stepString[ALLSETUP];

#define CLEAR_ENTRY \
   entering_state = 0
   
#define MARK_ENTRY \
   entering_state = 1

#define ON_ENTRY when(entering_state)

short  directive;     /* CAD directive */
short  car_val;       /* CAR state     */
string car_msg;       /* CAR message   */
short  car_err;       /* CAR error     */


short  subSetup_DIR  [ALLSETUP];
short  subSetup_MARK [ALLSETUP];
short  subSetup_VAL  [ALLSETUP];
string subSetup_MESS [ALLSETUP];
short  subSetupC_VAL [ALLSETUP];
string subSetupC_OMSS[ALLSETUP];
short  subSetupDone  [ALLSETUP];
double subSetup_to   [ALLSETUP];

assign  subSetup_DIR  [ARSETUP ]  to "{dctop}:arSetup.DIR"     ;          
assign  subSetup_MARK [ARSETUP ]  to "{dctop}:arSetup.MARK"    ;          
assign  subSetup_VAL  [ARSETUP ]  to "{dctop}:arSetup.VAL"     ;          
assign  subSetup_MESS [ARSETUP ]  to "{dctop}:arSetup.MESS"    ;          
assign  subSetupC_VAL [ARSETUP ]  to "{dctop}:arSetupC.VAL"    ;
assign  subSetupC_OMSS[ARSETUP ]  to "{dctop}:arSetupC.OMSS"   ;
assign  subSetupDone  [ARSETUP ]  to "{dctop}:arSetupDone.VAL" ;          

assign  subSetup_DIR  [OBSSETUP]  to "{dctop}:obsSetup.DIR"    ;            
assign  subSetup_MARK [OBSSETUP]  to "{dctop}:obsSetup.MARK"   ;            
assign  subSetup_VAL  [OBSSETUP]  to "{dctop}:obsSetup.VAL"    ;            
assign  subSetup_MESS [OBSSETUP]  to "{dctop}:obsSetup.MESS"    ;          
assign  subSetupC_VAL [OBSSETUP]  to "{dctop}:obsSetupC.VAL"   ;
assign  subSetupC_OMSS[OBSSETUP]  to "{dctop}:obsSetupC.OMSS"  ;
assign  subSetupDone  [OBSSETUP]  to "{dctop}:obsSetupDone.VAL";            

assign  subSetup_DIR  [ROISETUP]  to "{dctop}:drRoiSet.DIR"    ;          
assign  subSetup_MARK [ROISETUP]  to "{dctop}:drRoiSet.MARK"   ;          
assign  subSetup_VAL  [ROISETUP]  to "{dctop}:drRoiSet.VAL"    ;          
assign  subSetup_MESS [ROISETUP]  to "{dctop}:drRoiSet.MESS"    ;          
assign  subSetupC_VAL [ROISETUP]  to "{dctop}:drRoiSetC.VAL"   ;
assign  subSetupC_OMSS[ROISETUP]  to "{dctop}:drRoiSetC.OMSS"  ;
assign  subSetupDone  [ROISETUP]  to "{dctop}:drRoiSetDone.VAL";          

assign  directive        to "{dctop}:dcSetup.DIR"     ;
assign  car_val          to "{dctop}:dcSetupC.IVAL"   ;
assign  car_msg          to "{dctop}:dcSetupC.IMSS"   ;
assign  car_err          to "{dctop}:dcSetupC.IERR"   ;

monitor directive;
evflag  dir_flag;         /* Event flag to detect new directives */
sync directive dir_flag;  /* Syncronize variable with flag       */

monitor subSetup_VAL [ARSETUP ];
monitor subSetupC_VAL[ARSETUP ];
monitor subSetup_VAL [OBSSETUP];
monitor subSetupC_VAL[OBSSETUP];
monitor subSetup_VAL [ROISETUP];
monitor subSetupC_VAL[ROISETUP];

ss dcSetup
{
   /*--------------------------------------------------------------------------
    * startup
    *--------------------------------------------------------------------------*/
   state dcSetup_startup
   {   
   ON_ENTRY {
      gemLogMsg(GEMLOG_DEBUG,"dcSetup_startup\n");
         CLEAR_ENTRY;
      } state dcSetup_startup
   
   when() {
      gemLogMsg(GEMLOG_DEBUG,"DC setup sequencer: Startup... \n");
      
      /* ignore any lingering monitors raised on connection */
      efClear(dir_flag)       ;
      subSetup_to[ARSETUP ]=20.0;
      subSetup_to[OBSSETUP]=10.0;
      subSetup_to[ROISETUP]= 2.0;
      strcpy(stepString[ARSETUP ], "ARSETUP" );
      strcpy(stepString[OBSSETUP], "OBSSETUP");
      strcpy(stepString[ROISETUP], "ROISETUP");

      gemLogMsg(GEMLOG_DEBUG,"DC setup sequencer: waiting for new directive\n");
      
      MARK_ENTRY;
      } state dcSetup_idle
   }
   
   
   /*--------------------------------------------------------------------------
    * idle
    *--------------------------------------------------------------------------*/
   state dcSetup_idle
   {
   
   ON_ENTRY {
      gemLogMsg(GEMLOG_DEBUG,"dcSetup_idle\n");
         CLEAR_ENTRY;
      } state dcSetup_idle

   /* only process marked CADs */
   when(efTest(dir_flag) && (directive==CAD_START))
      {
         /* go busy */
         car_err=OK;
         car_val=CAR_BUSY;
         strcpy(car_msg,"");
         pvPut(car_msg);         
         pvPut(car_err);         
         pvPut(car_val);

         gemLogMsg(GEMLOG_DEBUG,"DC setup sequencer: Received START directive\n");
         efClear(dir_flag);                   
         step=ARSETUP;
         MARK_ENTRY;
      } state dcSetup_seq
   
   when(efTest(dir_flag) && (directive==CAD_CLEAR))
      {
         gemLogMsg(GEMLOG_DEBUG,"DC setup sequencer: Received CLEAR directive\n");
         efClear(dir_flag);
         strcpy (car_msg,"");
         car_err=OK;
         car_val=CAR_IDLE;
         
         /* clear all sub setup dir */
         subSetup_DIR[ARSETUP ]=CAD_CLEAR;
         subSetup_DIR[OBSSETUP]=CAD_CLEAR;
         subSetup_DIR[ROISETUP]=CAD_CLEAR;
         pvPut(subSetup_DIR[ARSETUP ]);
         pvPut(subSetup_DIR[OBSSETUP]);
         pvPut(subSetup_DIR[ROISETUP]);
         MARK_ENTRY;
      } state dcSetup_end
   

   /* ignore anything else */
   when(efTest(dir_flag))
      {
      efClear(dir_flag);
      } state dcSetup_idle


   }

   /*--------------------------------------------------------------------------
    * seq
    *--------------------------------------------------------------------------*/
   state dcSetup_seq
   {
   
   ON_ENTRY {
      gemLogMsg(GEMLOG_DEBUG,"dcSetup_seq\n");
      CLEAR_ENTRY;
      } state dcSetup_seq
   
   /* get the mark value associted with the current step */
   when(step<ALLSETUP)
      {
         gemLogMsg(GEMLOG_DEBUG,"dcSetup_seq step %s\n",stepString[step]);
         pvGet(subSetup_MARK[step]);
         pvGet(subSetupDone[step]);
       MARK_ENTRY;
      } state dcSetup_trig
      
   /* last step goes back idle and over */
   when() {
         gemLogMsg(GEMLOG_DEBUG,"DC setup sequencer: over and out\n");
         
         /* go back idle */
         strcpy (car_msg,"");
         car_err=OK;
         car_val=CAR_IDLE;
         
      MARK_ENTRY;
      } state dcSetup_end
   }

   /*--------------------------------------------------------------------------
    * dcSetup_trig
    *--------------------------------------------------------------------------*/
   state dcSetup_trig
   {
   ON_ENTRY {
      gemLogMsg(GEMLOG_DEBUG,"dcSetup_trig\n");
      CLEAR_ENTRY;
      } state dcSetup_trig

   /* if the step is marked or not done (reverse logic=1) , start it */
   when(subSetup_MARK[step] | subSetupDone[step]) {
         gemLogMsg(GEMLOG_DEBUG,"DC setup sequencer: step %s triggered now\n", stepString[step]);
         /* mark it if not */
         if (! subSetup_MARK[step]) {
            subSetup_DIR[step]=CAD_MARK;
            pvPut(subSetup_DIR[step]);
            taskDelay(10);
            }
         subSetup_DIR[step]=CAD_START;
         pvPut(subSetup_DIR[step]);
      MARK_ENTRY;
      } state dcSetup_start
            
   /* otherwise proceed to next one */
   when(step<ALLSETUP) {
         gemLogMsg(GEMLOG_DEBUG,"DC setup sequencer: step %s not marked and done\n",stepString[step]);
         step++;
      MARK_ENTRY;
      } state dcSetup_seq

   }

   /*--------------------------------------------------------------------------
    * dcSetup_start
    *--------------------------------------------------------------------------*/
   state dcSetup_start
   {
      ON_ENTRY {
         gemLogMsg(GEMLOG_DEBUG,"dcSetup_start\n");
         CLEAR_ENTRY;
         } state dcSetup_start
      
      /* step got busy - fine */
      when(subSetupC_VAL[step]==CAR_BUSY) {
         gemLogMsg(GEMLOG_DEBUG,"dcSetup step %s got busy\n",stepString[step]);
         MARK_ENTRY;
         } state dcSetup_busy

      /* step got rejected - final */
      when(subSetup_VAL[step]==CAD_REJECT) {
         gemLogMsg(GEMLOG_ERR,"dcSetup step %s got rejected\n",stepString[step]);
         /* go error */
         car_err=ERROR;
         car_val=CAR_ERROR;
         /* grab the message from the relevant rejected CAD */
         pvGet(subSetup_MESS[step]);
         sprintf(car_msg,"%s",subSetup_MESS[step]);
         MARK_ENTRY;
         } state dcSetup_end

      /* handle timeout - could be we missed a monitor (eg. ROI is too fast to see the transition) */
      when(delay(subSetup_to[step])) {
         gemLogMsg(GEMLOG_ERR,"dcSetup didn't get busy\n");
         MARK_ENTRY;
         } state dcSetup_timeout
   }

   /*--------------------------------------------------------------------------
    * dcSetup_busy
    *--------------------------------------------------------------------------*/
   state dcSetup_busy
   {
      ON_ENTRY {
         gemLogMsg(GEMLOG_DEBUG,"dcSetup_busy\n");
         CLEAR_ENTRY;
      } state dcSetup_busy
      
      /* error */
      when(subSetupC_VAL[step]==CAR_ERROR) {
         gemLogMsg(GEMLOG_ERR,"dcSetup got error for step %s\n",stepString[step]);
         sprintf (car_msg,"arSetup step %s failed",stepString[step]);
         car_err=ERROR;
         car_val=CAR_ERROR;
         MARK_ENTRY;
         } state dcSetup_end
         
      /* idle */
      when(subSetupC_VAL[step]==CAR_IDLE) {
         gemLogMsg(GEMLOG_DEBUG,"dcSetup completed step %s\n",stepString[step]);
         step++;
         MARK_ENTRY;
         } state dcSetup_seq
      
      /* handle timeout */
      when(delay(subSetup_to[step])) {
         gemLogMsg(GEMLOG_ERR,"dcSetup busy timed out step %s\n",stepString[step]);
         MARK_ENTRY;
         } state dcSetup_timeout
   }

   /*--------------------------------------------------------------------------
    * dcSetup_timeout
    *--------------------------------------------------------------------------*/
   state dcSetup_timeout
   {
      ON_ENTRY {
         gemLogMsg(GEMLOG_DEBUG,"dcSetup_timeout\n");
         pvGet(subSetupDone[step]);
         pvGet(subSetupC_VAL[step]);
         CLEAR_ENTRY;
      } state dcSetup_timeout
      
      /* timeout + done - continue */
      when(subSetupDone[step]==0 && subSetupC_VAL[step]==CAR_IDLE) {
         gemLogMsg(GEMLOG_INFO,"dcSetup timeout but is done for step %s\n",stepString[step]);
         step++;
         MARK_ENTRY;
         } state dcSetup_seq
         
      /* timeout + not done - final */
      when() {
         gemLogMsg(GEMLOG_ERR,"dcSetup timed out NOT done step %s\n",stepString[step]);
         sprintf (car_msg,"dcSetup timeout NOT done step %s",stepString[step]);
         car_err=ERROR;
         car_val=CAR_ERROR;
         MARK_ENTRY;
         } state dcSetup_end
   }

   /*--------------------------------------------------------------------------
    * End
    *--------------------------------------------------------------------------*/
   state dcSetup_end
   {
   ON_ENTRY {
      gemLogMsg(GEMLOG_DEBUG,"dcSetup_end\n");
         CLEAR_ENTRY;
      } state dcSetup_end
      
      when() {         
         gemLogMsg(GEMLOG_DEBUG,"DC setup sequencer: ready for new directive\n");
         pvPut(car_msg);
         pvPut(car_err);
         pvPut(car_val);
         MARK_ENTRY;
         } state dcSetup_idle
   }
}
