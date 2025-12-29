/* 
   headers for test functions in coaddTest.c 
   ASP 8/1/2000
*/

int setdmasize (int size);
int setup (void);
void setflags(void);
void first (int first);
void last (int last);
void both (void);
void neither (void);
void add (void);
void subtract (void);
int  teststart(void);
int  teststop (void);
int setcbase (int addr);
void setcoaddbufsize (int size);
int setdbase (int addr);
void setdsnumrows (int rows);
void setdsnumcols (int cols);
int setxbase (int addr);
void setxnumrows (int rows);
void setxnumcols (int cols);
int zerobase (void);
int dmago (void);
int setdmasize (int size);
void readbuffer (int numwords, int firstrow);
int mempoke(int row, int col);
