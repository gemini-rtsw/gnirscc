/* header to declare all the link stuff that inmos didnt */

int OpenLink(char *name);
int ReadLink(int lid, void *buffer, int size, int something);
int WriteLink(int lid, void *buffer, int length, int something);

int b011_map(unsigned long *addr, int *size);

