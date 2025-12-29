typedef union valUnion
{
  char s[80];
  double d;
  long l;
  long us;
}valUnion;

typedef struct 
{
	long op;
	long id;
	long status;
	char name[80];
	long type;
	union valUnion val;
	
}dcaMsgStruct;


