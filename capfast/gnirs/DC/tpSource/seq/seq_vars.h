#ifdef MAIN
char seqSrcID[] = { PWD };

int trace_flag;

Process *Preader, *Pwriter;
Channel *Reader_to_Writer, *Control_to_Reader, *Writer_to_Control;

#else
extern int trace_flag;
extern Channel *Reader_to_Writer, *Control_to_Reader, *Writer_to_Control;
#endif

void myExecuteProc(), set_int(), read_int(), startMsg();
