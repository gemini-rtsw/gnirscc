#define SIOSRC 
#include <stdio.h> 
#include <8051.h> 
#include "prot.h" 
 
decod()
{
ui t1, t2, t3;
uc tm;

  t1 = t2 = t3 = tm = 999;
  sscanf((char *)(bbuf), "%c %u %u %u", &tm, &t1, &t2, &t3);
  xxcmd = tm;
  xxd1 = t1;
  xxd2 = t2;
  xxd3 = t3;
}

rstprot() 
{ 
  bbuf = (uc *)(0x7200);
  asm(" MOV CMDAV, #$00");
} 

rstbuf()
{
   asm(" CALL RSTBUF");
}
 
#asm
RSTBUF:
   PUSH ACC
   PUSH DPH
   PUSH DPL
   MOV BCNT, #$00
   MOV DPH, #$72
   MOV A, #$00
   MOV DPL, #$00
   MOVX @DPTR, A
   MOV DPL, #$01
   MOVX @DPTR, A
   MOV DPL, #$02
   MOVX @DPTR, A
   MOV DPL, #$03
   MOVX @DPTR, A
   MOV DPL, #$04
   MOVX @DPTR, A
   MOV DPL, #$05
   MOVX @DPTR, A
   MOV DPL, #$06
   MOVX @DPTR, A
   MOV DPL, #$07
   MOVX @DPTR, A
   MOV DPL, #$08
   MOVX @DPTR, A
   MOV DPL, #$09
   MOVX @DPTR, A
   MOV DPL, #$0A
   MOVX @DPTR, A
   MOV DPL, #$0B
   MOVX @DPTR, A
   MOV DPL, #$0C
   MOVX @DPTR, A
   MOV DPL, #$0D
   MOVX @DPTR, A
   MOV DPL, #$0E
   MOVX @DPTR, A
   MOV DPL, #$0F
   MOVX @DPTR, A
   POP DPL
   POP DPH
   POP ACC
   RET
#endasm
 
myprintf(mystr)  
char *mystr;
{  
     int i=0;  
  
     while (mystr[i] != 0)  
       {  
       SBUF = mystr[i];  
       asm(" JNB TI, $");  
       asm(" CLR TI");  
       i++;  
       }  
}  
  
myrp(rtyp, rdat) 
ui rtyp, rdat;
{ 
    char rstr[10], rx[5]; 
 
    rstr[0] = 10; 
    rstr[1] = 13; 
    rstr[2] = 'E'; 
    if (rtyp == 1) rstr[2] = 'R'; 
    sprintf(rx, "%04X", rdat); 
    rstr[3] = rx[0]; 
    rstr[4] = rx[1]; 
    rstr[5] = rx[2]; 
    rstr[6] = rx[3]; 
    rstr[7] = 10; 
    rstr[8] = 13; 
    rstr[9] = 0; 
    myprintf(rstr); 
} 

rperr()
{
    if (errno < 0)
      myrp(0, -errno);
     else
      myrp(1,  errno);
}

void interrupt sio()  
{  
#asm
  JNB RI, ENDSIO
    MOV RBYTE, SBUF
    MOV A, RBYTE
    CJNE A, #'^', NO_RSTBUF
      CALL RSTBUF
      JMP ENDSIO
NO_RSTBUF:
    CJNE A, #10, NO_LF
      MOV CMDAV, #$01
      JMP ENDSIO
NO_LF:
    CJNE A, #13, NO_CR
      MOV CMDAV, #$01
      JMP ENDSIO
NO_CR:
    PUSH DPH
    PUSH DPL
    MOV DPH, #$72
    MOV DPL, BCNT
    MOV A, RBYTE
    MOVX @DPTR, A
    POP DPL
    POP DPH
    INC BCNT
ENDSIO:
  CLR RI
#endasm
}

