#include <cadef.h> 
#include "common.h" 
/* #include <time.h> */
void putEpicsVar(char *Var, dbr_int_t Value);
STATUS getEpics (char* name, void *val);
STATUS putEpics (char* name, void *val);
STATUS putEpicsInt(char *Var, dbr_int_t Value);
STATUS getEpicsChanArray (char **name, chid **chID, int n);
STATUS getEepicsTypeArray(chid *chID, int **ctype, int n);
STATUS getEpicsValArray(int *ctype, chid *chID, void **val, int n);
STATUS putEpicsValArray(int *ctype, chid *chID, void **val, int n);
void changeNotify(struct event_handler_args args);
STATUS getEpicsEnum(char *name,dbr_int_t *val);
STATUS putEpicsEnum(char *name,dbr_int_t *val);
