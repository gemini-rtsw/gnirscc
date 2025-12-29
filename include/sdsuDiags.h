/*
	Header for SDSU diagnostic commands implemented in C code, which
	calls lower-level SDSU boot commands.
 */

void testDataLinks(void);
void dumpStats(void);
void clrStats(void);
int  sdsuD(int, char, uint32, int);
void clearMem(char*, int);
void varSizes(void);
