# 43 "H:\PCS\AVCASE51\include\intrpt.h"
 defseg c_vectors,class=code,overlaid,start=0 ;#
# 20 "prot.h"
CMDAV EQU $7F ;#
RBYTE EQU $7E ;#
BCNT EQU $7D ;#
	global	stack_external
	global	restra,savera,lcsv,scsv
	defseg	c_text,class=CODE
	seg	c_text
	global	_decod
	signat	_decod,26
	global	sp_dp
	global	_sscanf
	signat	_sscanf,4122
	global	_bbuf
	global	sp_dp
	global	psh_WR2
	global	sp_dp
	global	psh_WR2
	global	sp_dp
	global	psh_WR2
	global	sp_dp
	global	psh_WR2
	global	_xxcmd
	global	sp_dp
	global	_xxd1
	global	sp_dp
	global	_xxd2
	global	sp_dp
	global	_xxd3
	global	sp_dp
_decod:
	mov	a,#-7
	lcall	scsv
	pop	0
	pop	1
	lcall	savera
;prot.c: 8: ui t1, t2, t3;
;prot.c: 9: uc tm;
;prot.c: 11: t1 = t2 = t3 = tm = 999;
	mov	r5,#231
	mov	dptr,#2
	lcall	sp_dp
	mov	a,r5
	movx	@dptr,a
	mov	r4,#0
	mov	dptr,#3
	lcall	sp_dp
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	mov	dptr,#5
	lcall	sp_dp
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	mov	dptr,#7
	lcall	sp_dp
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;prot.c: 12: sscanf((char *)(bbuf), "%c %u %u %u", &tm, &t1, &t2, &t3);
	mov	dptr,#3
	lcall	sp_dp
	mov	r4,dph
	mov	r5,dpl
	call	psh_WR2
	mov	dptr,#7
	lcall	sp_dp
	mov	r4,dph
	mov	r5,dpl
	call	psh_WR2
	mov	dptr,#11
	lcall	sp_dp
	mov	r4,dph
	mov	r5,dpl
	call	psh_WR2
	mov	dptr,#8
	lcall	sp_dp
	mov	r4,dph
	mov	r5,dpl
	call	psh_WR2
	mov	r4,#high u19
	mov	r5,#low u19
	call	psh_WR2
	mov	dptr,#_bbuf
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	lcall	_sscanf
	mov	acc,#10
	lcall	scsv
;prot.c: 13: xxcmd = tm;
	mov	dptr,#2
	lcall	sp_dp
	movx	a,@dptr
	mov	dptr,#_xxcmd
	movx	@dptr,a
;prot.c: 14: xxd1 = t1;
	mov	dptr,#7
	lcall	sp_dp
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#_xxd1
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;prot.c: 15: xxd2 = t2;
	mov	dptr,#5
	lcall	sp_dp
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#_xxd2
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;prot.c: 16: xxd3 = t3;
	mov	dptr,#3
	lcall	sp_dp
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#_xxd3
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
;prot.c: 17: }
	lcall	restra
	push	1
	push	0
	mov	a,#7
	jmp	scsv
	global	_rstprot
	signat	_rstprot,26
_rstprot:
;prot.c: 21: bbuf = (uc *)(0x7200);
	mov	dptr,#_bbuf
	mov	a,#114
	movx	@dptr,a
	inc	dptr
	clr	a
	movx	@dptr,a
;prot.c: 22: asm(" MOV CMDAV, #$00");
# 22 "prot.c"
 MOV CMDAV, #$00 ;#
;prot.c: 23: }
	ret
	global	_rstbuf
	signat	_rstbuf,26
_rstbuf:
;prot.c: 27: asm(" CALL RSTBUF");
# 27 "prot.c"
 CALL RSTBUF ;#
;prot.c: 28: }
	ret
	global	_myprintf
	signat	_myprintf,4154
	global	sp_dp
	global	ldx_r5
	global	sp_dp
	global	dec_dptr
	global	sp_dp
	global	ldx_byte
# 31 "prot.c"
RSTBUF: ;#
 PUSH ACC ;#
 PUSH DPH ;#
 PUSH DPL ;#
 MOV BCNT, #$00 ;#
 MOV DPH, #$72 ;#
 MOV A, #$00 ;#
 MOV DPL, #$00 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$01 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$02 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$03 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$04 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$05 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$06 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$07 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$08 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$09 ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$0A ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$0B ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$0C ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$0D ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$0E ;#
 MOVX @DPTR, A ;#
 MOV DPL, #$0F ;#
 MOVX @DPTR, A ;#
 POP DPL ;#
 POP DPH ;#
 POP ACC ;#
 RET ;#
;prot.c: 78: {
;	param _mystr assigned to r4/r5 on entry
_myprintf:
	mov	a,#-4
	lcall	scsv
;prot.c: 79: int i=0;
	mov	dptr,#2
	lcall	sp_dp
	clr	a
	movx	@dptr,a
	inc	dptr
	movx	@dptr,a
;prot.c: 81: while (mystr[i] != 0)
;_mystr stored from WR2
	mov	dptr,#0
	lcall	sp_dp
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	jmp	A1

l8:
;prot.c: 82: {
;prot.c: 83: SBUF = mystr[i];
	mov	dptr,#0
	lcall	sp_dp
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#2
	lcall	sp_dp
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	add	a,r3
	mov	dpl,a
	mov	a,r4
	addc	a,r2
	mov	dph,a
	call	ldx_r5
	mov	sbuf,r5
;prot.c: 84: asm(" JNB TI, $");
# 84 "prot.c"
 JNB TI, $ ;#
;prot.c: 85: asm(" CLR TI");
 CLR TI ;#
;prot.c: 86: i++;
	mov	dptr,#2
	lcall	sp_dp
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	add	a,#1
	mov	r5,a
	mov	a,r4
	addc	a,#0
	mov	r4,a
	mov	a,r5
	movx	@dptr,a
	call	dec_dptr
	mov	a,r4
A1:
	movx	@dptr,a
;prot.c: 87: }
	mov	dptr,#0
	lcall	sp_dp
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#2
	lcall	sp_dp
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	add	a,r3
	mov	dpl,a
	mov	a,r4
	addc	a,r2
	mov	dph,a
	call	ldx_byte
	bnz	l8
;prot.c: 88: }
	mov	a,#4
	jmp	scsv
	global	_myrp
	signat	_myrp,8250
	global	sp_dp
	global	_sprintf
	signat	_sprintf,4122
	global	sp_dp
	global	psh_WR2
	global	sp_dp
;prot.c: 90: myrp(rtyp, rdat)
;prot.c: 91: ui rtyp, rdat;
;prot.c: 92: {
;	param _rtyp assigned to r4/r5 on entry
;	param _rdat assigned to r2/r3 on entry
_myrp:
	mov	a,#-19
	lcall	scsv
	pop	0
	pop	1
	lcall	savera
;prot.c: 93: char rstr[10], rx[5];
;prot.c: 95: rstr[0] = 10;
	mov	dptr,#11
	lcall	sp_dp
	mov	a,#10
	movx	@dptr,a
;prot.c: 96: rstr[1] = 13;
	mov	dptr,#12
	lcall	sp_dp
	mov	a,#13
	movx	@dptr,a
;prot.c: 97: rstr[2] = 'E';
	mov	dptr,#13
	lcall	sp_dp
	mov	a,#69
	movx	@dptr,a
;prot.c: 98: if (rtyp == 1) rstr[2] = 'R';
;_rdat stored from WR1
	mov	dptr,#4
	lcall	sp_dp
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
;_rtyp stored from WR2
	mov	dptr,#2
	lcall	sp_dp
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	mov	dptr,#2
	lcall	sp_dp
	movx	a,@dptr
	cbne	a,#0,l11
	inc	dptr
	movx	a,@dptr
	cbne	a,#1,l11
	mov	dptr,#13
	lcall	sp_dp
	mov	a,#82
	movx	@dptr,a
;prot.c: 99: sprintf(rx, "%04X", rdat);
l11:
	mov	dptr,#4
	lcall	sp_dp
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	call	psh_WR2
	mov	r4,#high u29
	mov	r5,#low u29
	call	psh_WR2
	mov	dptr,#10
	lcall	sp_dp
	mov	r4,dph
	mov	r5,dpl
	lcall	_sprintf
	mov	acc,#4
	lcall	scsv
;prot.c: 100: rstr[3] = rx[0];
	mov	dptr,#6
	lcall	sp_dp
	movx	a,@dptr
	push	acc
	mov	dptr,#14
	lcall	sp_dp
	pop	acc
	movx	@dptr,a
;prot.c: 101: rstr[4] = rx[1];
	mov	dptr,#7
	lcall	sp_dp
	movx	a,@dptr
	push	acc
	mov	dptr,#15
	lcall	sp_dp
	pop	acc
	movx	@dptr,a
;prot.c: 102: rstr[5] = rx[2];
	mov	dptr,#8
	lcall	sp_dp
	movx	a,@dptr
	push	acc
	mov	dptr,#16
	lcall	sp_dp
	pop	acc
	movx	@dptr,a
;prot.c: 103: rstr[6] = rx[3];
	mov	dptr,#9
	lcall	sp_dp
	movx	a,@dptr
	push	acc
	mov	dptr,#17
	lcall	sp_dp
	pop	acc
	movx	@dptr,a
;prot.c: 104: rstr[7] = 10;
	mov	dptr,#18
	lcall	sp_dp
	mov	a,#10
	movx	@dptr,a
;prot.c: 105: rstr[8] = 13;
	mov	dptr,#19
	lcall	sp_dp
	mov	a,#13
	movx	@dptr,a
;prot.c: 106: rstr[9] = 0;
	mov	dptr,#20
	lcall	sp_dp
	clr	a
	movx	@dptr,a
;prot.c: 107: myprintf(rstr);
	mov	dptr,#11
	lcall	sp_dp
	mov	r4,dph
	mov	r5,dpl
	lcall	_myprintf
;prot.c: 108: }
	lcall	restra
	push	1
	push	0
	mov	a,#19
	jmp	scsv
	global	_rperr
	signat	_rperr,26
	global	_errno
;prot.c: 110: rperr()
;prot.c: 111: {
_rperr:
	pop	0
	pop	1
	lcall	savera
;prot.c: 112: if (errno < 0)
	mov	dptr,#_errno
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	a,r4
;prot.c: 113: myrp(0, -errno);
	mov	dptr,#_errno
	bnb	acc.7,A2

	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	clr	a
	clr	c
	subb	a,r5
	mov	r3,a
	clr	a
	subb	a,r4
	mov	r2,a
	mov	r4,#0
	mov	r5,#0
	jmp	A3

;prot.c: 114: else
;prot.c: 115: myrp(1, errno);
A2:
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	r4,#0
	mov	r5,#1
A3:
	lcall	_myrp
;prot.c: 116: }
	lcall	restra
	push	1
	push	0
	ret
	global	_sio
	signat	_sio,24
;prot.c: 118: void interrupt sio()
;prot.c: 119: {
_sio:
	push	psw
	push	acc
# 121 "prot.c"
 JNB RI, ENDSIO ;#
 MOV RBYTE, SBUF ;#
 MOV A, RBYTE ;#
 CJNE A, #'^', NO_RSTBUF ;#
 CALL RSTBUF ;#
 JMP ENDSIO ;#
NO_RSTBUF: ;#
 CJNE A, #10, NO_LF ;#
 MOV CMDAV, #$01 ;#
 JMP ENDSIO ;#
NO_LF: ;#
 CJNE A, #13, NO_CR ;#
 MOV CMDAV, #$01 ;#
 JMP ENDSIO ;#
NO_CR: ;#
 PUSH DPH ;#
 PUSH DPL ;#
 MOV DPH, #$72 ;#
 MOV DPL, BCNT ;#
 MOV A, RBYTE ;#
 MOVX @DPTR, A ;#
 POP DPL ;#
 POP DPH ;#
 INC BCNT ;#
ENDSIO: ;#
 CLR RI ;#
;prot.c: 148: }
	pop	acc
	pop	psw
	reti
	defseg	c_strings,class=XDATA
	seg	c_strings
u19:
	db	"%c %u %u %u",0
u29:
	db	"%04X",0
	defseg	c_bss,class=XDATA
	seg	c_bss
	global	_bbuf
_bbuf:
	ds	2
	global	_errno
_errno:
	ds	2
	global	_xxcmd
_xxcmd:
	ds	1
	global	_xxd1
_xxd1:
	ds	2
	global	_xxd2
_xxd2:
	ds	2
	global	_xxd3
_xxd3:
	ds	2
	end
