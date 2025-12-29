typedef union valUnion
{
  char s[80];
  double d;
  long l;
  unsigned short us;
}valUnion;

typedef struct 
{
  short op;
  unsigned short type;
  int status;
  char name[80];
  union valUnion val;
  
}dcaMsgStruct;


