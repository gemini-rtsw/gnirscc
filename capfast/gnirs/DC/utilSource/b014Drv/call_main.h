/* VxWorks does not like the shell task to exit. */
#define ex_or_ret(arg)  if(taskIdSelf()==taskNameToId("tShell"))\
			    return(arg);\
			else \
			    exit(arg)

/* A macro for calling a UNIX style main program from the VxWorks shell. 
 * The first argument is the name used in the VxWorks shell and the second
 * is the name of the program called.
 */
#define	call_main( name, execute ) \
int name( a1, a2, a3, a4, a5, a6, a7, a8, a9, aa ) \
    int a1, a2, a3, a4, a5, a6, a7, a8, a9, aa ; \
{ \
	extern	int execute() ; \
	extern	int _call_main() ; \
	return( _call_main( (char *)execute, \
		a1, a2, a3, a4, a5, a6, a7, a8, a9, aa ) ) ; \
}
