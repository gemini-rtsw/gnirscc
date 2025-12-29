#include <cadef.h> 
#define NODBACCESS
#include "epicsDefines.h"
#include "epicsCAint.h"
void putEpicsVar(char *Var, dbr_int_t Value);
STATUS putEpicsIntCA(char *Var, dbr_int_t Value);
STATUS getEpicsChanArrayCA (char **name, chid **chID, int n);
STATUS getEepicsTypeArrayCA(chid *chID, int **ctype, int n);
STATUS getEpicsValArrayCA(int *ctype, chid *chID, void **val, int n);
STATUS putEpicsValArrayCA(int *ctype, chid *chID, void **val, int n);
void changeNotifyCA(struct event_handler_args args);
STATUS getEpicsEnumCA(char *name,dbr_int_t *val);
STATUS putEpicsEnumCA(char *name,dbr_int_t *val);

