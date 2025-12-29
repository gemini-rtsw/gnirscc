/* stuff because these headers SUCK! */

double atof(char *s);
int atoi(char *s);
int tolower(int c);

void printf(char *format, ...);
void bcopy(char *src, char *dst, int len);
int read(int fd, unsigned char *buf, int len);

#ifdef FILE
void fprintf(FILE *fp, char *format, ...);
#endif
