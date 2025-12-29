# 43 "H:\PCS\AVCASE51\include\intrpt.h"
 defseg c_vectors,class=code,overlaid,start=0 ;#
# 11 "prot.h"
MSGAV EQU $7F ;#
MSGDT EQU $7E ;#
CMDAV EQU $7D ;#
RBYTE EQU $7C ;#
	global	stack_external
	global	restra,savera,lcsv,scsv
	defseg	c_text,class=CODE
	seg	c_text
	global	_rstprot
	signat	_rstprot,26
_rstprot:
# 9 "prot.c"
 MOV MSGAV, #$00 ;#
 MOV CMDAV, #$00 ;#
;prot.c: 12: }
	ret
	global	_rstbuf
	signat	_rstbuf,26
	global	_bcnt
	global	_bbuf
_rstbuf:
;prot.c: 16: for (bcnt=0; bcnt<20; bcnt++) bbuf[bcnt]=0;
	clr	a
	mov	dptr,#_bcnt
	jmp	A1

l5:
	mov	dptr,#_bcnt
	movx	a,@dptr
	add	a,#low _bbuf
	mov	dpl,a
	clr	a
	addc	a,#high _bbuf
	mov	dph,a
	clr	a
	movx	@dptr,a
	mov	dptr,#_bcnt
	movx	a,@dptr
	inc	a
A1:
	movx	@dptr,a
	mov	dptr,#_bcnt
	movx	a,@dptr
	add	a,#-20
	bnc	l5
;prot.c: 17: bcnt = 0;
	clr	a
	movx	@dptr,a
;prot.c: 18: }
	ret
	global	_myprintf
	signat	_myprintf,4154
	global	sp_dp
	global	ldx_r5
	global	sp_dp
	global	dec_dptr
	global	sp_dp
	global	ldx_byte
;prot.c: 20: myprintf(char *mystr)
;prot.c: 21: {
;	param _mystr assigned to r4/r5 on entry
_myprintf:
	mov	a,#-4
	lcall	scsv
;prot.c: 22: int i=0;
	mov	dptr,#2
	lcall	sp_dp
	clr	a
	movx	@dptr,a
	inc	dptr
	movx	@dptr,a
;prot.c: 24: while (mystr[i] != 0)
;_mystr stored from WR2
	mov	dptr,#0
	lcall	sp_dp
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	jmp	A2

l11:
;prot.c: 25: {
;prot.c: 26: SBUF = mystr[i];
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
;prot.c: 27: asm(" JNB TI, $");
# 27 "prot.c"
 JNB TI, $ ;#
;prot.c: 28: asm(" CLR TI");
 CLR TI ;#
;prot.c: 29: i++;
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
A2:
	movx	@dptr,a
;prot.c: 30: }
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
	bnz	l11
;prot.c: 31: }
	mov	a,#4
	jmp	scsv
	global	_myrp
	signat	_myrp,26
	global	sp_dp
	global	_sprintf
	signat	_sprintf,4122
	global	sp_dp
	global	psh_WR2
	global	sp_dp
;prot.c: 33: myrp(rtyp, rdat)
;prot.c: 34: unsigned char rtyp, rdat;
;prot.c: 35: {
_myrp:
	mov	a,#-11
	lcall	scsv
	pop	0
	pop	1
	lcall	savera
;prot.c: 36: char rstr[8], rx[3];
;prot.c: 38: rstr[0] = 10;
	mov	dptr,#5
	lcall	sp_dp
	mov	a,#10
	movx	@dptr,a
;prot.c: 39: rstr[1] = 13;
	mov	dptr,#6
	lcall	sp_dp
	mov	a,#13
	movx	@dptr,a
;prot.c: 40: rstr[2] = 'E';
	mov	dptr,#7
	lcall	sp_dp
	mov	a,#69
	movx	@dptr,a
;prot.c: 41: if (rtyp == 1) rstr[2] = 'R';
	mov	dptr,#14
	lcall	sp_dp
	movx	a,@dptr
	cbne	a,#1,l14
	mov	dptr,#7
	lcall	sp_dp
	mov	a,#82
	movx	@dptr,a
;prot.c: 42: sprintf(rx, "%02X", rdat);
l14:
	mov	dptr,#16
	lcall	sp_dp
	movx	a,@dptr
	mov	r5,a
	mov	r4,#0
	call	psh_WR2
	mov	r4,#high u19
	mov	r5,#low u19
	call	psh_WR2
	mov	dptr,#6
	lcall	sp_dp
	mov	r4,dph
	mov	r5,dpl
	lcall	_sprintf
	mov	acc,#4
	lcall	scsv
;prot.c: 43: rstr[3] = rx[0];
	mov	dptr,#2
	lcall	sp_dp
	movx	a,@dptr
	push	acc
	mov	dptr,#8
	lcall	sp_dp
	pop	acc
	movx	@dptr,a
;prot.c: 44: rstr[4] = rx[1];
	mov	dptr,#3
	lcall	sp_dp
	movx	a,@dptr
	push	acc
	mov	dptr,#9
	lcall	sp_dp
	pop	acc
	movx	@dptr,a
;prot.c: 45: rstr[5] = 10;
	mov	dptr,#10
	lcall	sp_dp
	mov	a,#10
	movx	@dptr,a
;prot.c: 46: rstr[6] = 13;
	mov	dptr,#11
	lcall	sp_dp
	mov	a,#13
	movx	@dptr,a
;prot.c: 47: rstr[7] = 0;
	mov	dptr,#12
	lcall	sp_dp
	clr	a
	movx	@dptr,a
;prot.c: 48: myprintf(rstr);
	mov	dptr,#5
	lcall	sp_dp
	mov	r4,dph
	mov	r5,dpl
	lcall	_myprintf
;prot.c: 49: }
	lcall	restra
	push	1
	push	0
	mov	a,#11
	jmp	scsv
	global	_sio
	signat	_sio,24
;prot.c: 51: void interrupt sio()
;prot.c: 52: {
_sio:
	push	psw
	push	acc
	push	dpl
	push	dph
	push	1
	push	0
;prot.c: 53: if (RI)
	bnb	ri,l15
;prot.c: 54: {
;prot.c: 55: asm(" MOV RBYTE, SBUF");
# 55 "prot.c"
 MOV RBYTE, SBUF ;#
;prot.c: 56: switch (RBYTE)
	mov	a,124
	add	a,#246
	bz	l21
	add	a,#253
	bz	l21
	add	a,#175
	bnz	l22
;prot.c: 57: {
;prot.c: 58: case '^':
;prot.c: 59: rstbuf();
	lcall	_rstbuf
;prot.c: 60: break;
	jmp	l17
;prot.c: 62: case 10:
;prot.c: 63: case 13:
l21:
;prot.c: 64: asm(" MOV CMDAV, #$01");
# 64 "prot.c"
 MOV CMDAV, #$01 ;#
;prot.c: 65: break;
	jmp	l17
;prot.c: 67: default:
l22:
;prot.c: 68: bbuf[bcnt] = RBYTE;
	mov	dptr,#_bcnt
	movx	a,@dptr
	add	a,#low _bbuf
	mov	dpl,a
	clr	a
	addc	a,#high _bbuf
	mov	dph,a
	mov	a,124
	movx	@dptr,a
;prot.c: 69: bcnt++;
	mov	dptr,#_bcnt
	movx	a,@dptr
	inc	a
	movx	@dptr,a
;prot.c: 70: if (bcnt > 19)
	movx	a,@dptr
	add	a,#-20
	bnc	l17
;prot.c: 71: {
;prot.c: 72: rstbuf();
	lcall	_rstbuf
;prot.c: 73: asm(" MOV MSGDT, #$01");
# 73 "prot.c"
 MOV MSGDT, #$01 ;#
;prot.c: 74: asm(" MOV MSGAV, #$01");
 MOV MSGAV, #$01 ;#
;prot.c: 75: }
;prot.c: 76: }
l17:
;prot.c: 77: asm(" CLR RI");
# 77 "prot.c"
 CLR RI ;#
;prot.c: 78: }
;prot.c: 79: }
l15:
	pop	0
	pop	1
	pop	dph
	pop	dpl
	pop	acc
	pop	psw
	reti
	defseg	c_strings,class=XDATA
	seg	c_strings
u19:
	db	"%02X",0
	end
