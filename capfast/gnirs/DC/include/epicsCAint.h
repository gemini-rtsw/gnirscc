
long updateState( char *);
long updateHealth( char *);
STATUS getEpicsCA (char* name, int count, void *val);
STATUS putEpicsCA (char* name, void *val);
STATUS getEpics (char* name, void *val,int count,void **ch);
STATUS putEpics (char* name, void *val,void **ch);
