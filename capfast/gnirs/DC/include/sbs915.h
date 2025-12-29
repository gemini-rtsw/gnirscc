

struct sbs915 {
	u_char SEMA[8];		/* 8 Semephore registers */
	u_char PADD[8];		/* hole 		 */
	u_char GP[2];		/* 2 General Purpose Bytes */
	u_short MAP[7];		/* 7 SBus to VME Map Control */
	u_char RMWC;		/* Read Modify Write Control */
	u_char SB2VMC;		/* SBus to VME Control */
	u_short VMEbase;	/* VME Base Address	*/
	u_short SBbase;		/* SBus Virtual Address base */
	u_char VM2SBC;		/* VME to SBus Control */
	u_char FPctrl;		/* Front Panel Control Reg */
	u_char VMEirqstat;	/* VMEbus Interrupt Request Status */
	u_char VMEintvec[7];	/* VMEbus Interrupt Level Vector Array */
	u_char VMEirqctrl;	/* VMEbus Interrupt Request Control */
	u_char SBintlev;	/* SBus Interrupt Level Select */
	u_char SBctrl;		/* General SBus Control Byte */
};
