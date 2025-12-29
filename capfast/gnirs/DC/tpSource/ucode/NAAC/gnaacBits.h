/*****************************************************************************
 * Copyright 1997 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME
 * 	gnaacBits.h
 *
 * DESCRIPTION
 * This file contains the bit definitions for the gnaac aladdin array
 *	controller. It is used by the SDC waveform compiler in conjunction
 *	with a ucode .c file to create a ucode .tld file capable of running
 *	an aladdin I or II array. 

 * DEPENDENCIES
 * The names of the bits depend on the hardware configuration of the array and
 *	controller the ucode is destined for changes to bit usage should be
 *	reflected in changes to this file.  This file requires sdc v2.3 or
 *	later.
 *
 * Author:
 *	Nick C. Buchholz
 *
 * History:
 *	29-May-1997 - Created file from nroddier's description file for gnaac
 *		controller - ncb
 *	
 *INDENT-OFF*
 * $Log: gnaacBits.h,v $
 * Revision 1.2  2009/05/27 19:33:31  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 * Revision 1.1.1.1  1998/12/15 16:17:31  buchholz
 * Imported gnaacSrc into CVS
 *
 *INDENT-ON* 
 *****************************************************************************/

BIT(CTC, 1);			/* command to convert for all ADC's */
BIT(XM2_CTC, 2);		/* spare command to convert for 2nd Xmtr */
BIT(Dxfr, 3);			/* start Data transfer bit */
BIT(XM2_Dxfr, 4);		/* spare Dxfr bit for 2nd Xmtr */
BIT(XClk, 5);			/* clk 1 digital add on ADC Xlinix chips */
BIT(XM2_XClk, 6);		/* spare XClk bit for 2nd Xmtr  */
BIT(XRst, 7);			/* signal to reset ADC Xlinix chips */
BIT(XM2_XRst, 8);		/* spare XRst bit for 2nd Xmtr */
BIT(Hsync, 9);			/* horizontal sync signal for Datacube */
BIT(XM2_Hsync, 10);		/* spare HSYNC bit for 2nd Xmtr */
BIT(Vsync, 11);			/* vertical sync signal for Datacube */
BIT(XM2_Vsync, 12);		/* spare VSYNC bit for 2nd Xmtr */
BIT(PxlGate, 13);		/* enable pixel clock during blanking */
BIT(XM2_PxlGate, 14);		/* spare PxlGate bit for 2nd Xmtr */
BIT(Actv, 15);			/* Activate Enable Clock */
BIT(RstLoopSync, 16);		/* Reset loop scope trigger */
BIT(Read1Sync, 17);		/* First read scope trigger */
BIT(Read2Sync, 18);		/* Second read scope trigger */
BIT(PxlSync, 19);		/* Reset loop scope trigger */
BIT(MEN1, 20);			/* Preamp Mux Enable Bit 1 */
BIT(MEN2, 21);			/* Preamp Mux Enable Bit 2 */
BIT(MEN4, 22);			/* Preamp Mux Enable Bit 3 */
BIT(MEN3, 23);			/* Preamp Mux Enable Bit 4 */
BIT(M0, 24);			/* Mux Select Bit 0 */
BIT(M1, 25);			/* Mux Select Bit 1 */
BIT(M2, 26);			/* Mux Select Bit 2 */
BIT(M3, 27);			/* Mux Select Bit 3 */
BIT(M4, 28);			/* Mux Select Bit 4 */
BIT(FEN, 29);			/* Fast Clk Enable Bit */
BIT(F1, 30);			/* Fast Clk 1 bit */
BIT(F2, 31);			/* Fast Clk 2 bit */
BIT(FS, 32);			/* Fast Clk Sync Bit */
BIT(SEN, 33);			/* Slow Clk Enable Bit */
BIT(S1, 34);			/* Slow Clk 1 bit */
BIT(S2, 35);			/* Slow Clk 2 bit */
BIT(SOE, 36);			/* Slow Clk Odd/Even bit */
BIT(SS, 37);			/* Slow Clk Sync bit */
BIT(RowEnEn, 38);		/* Row Enable Enable */
BIT(VRowEnbl, 39);		/* Row Enable */
BIT(CREN, 40);			/* Clamp/Reset Enable Bit */
BIT(Clamp, 41);			/* Clamp Bit */
BIT(RstR, 42);			/* Reset G bit */
BIT(RstG, 43);			/* Reset R bit */
BIT(DEN, 44);			/* Row Disable Enable Bit */
BIT(DES1, 45);			/* Quadrant 1 Row Disable bit */
BIT(DES2, 46);			/* Quadrant 2 Row Disable bit */
BIT(DES3, 47);			/* Quadrant 3 Row Disable bit */
BIT(DES4, 48);			/* Quadrant 4 Row Disable bit */
BIT(MClk, 49);			/* clock to lock in values on Enabled Regs */
BIT(Tswtch, 50);		/* temperature servo on/off switch */
BIT(TdClk, 51);			/* clock for servo switch */
BIT(ADCRst, 52);		/* spare */
BIT(SPARE52, 53);		/* spare */
BIT(ACB53, 54);			/* spare */
BIT(ACB54, 55);			/* spare */
BIT(ACB55, 56);			/* spare */


/* Field Control Bits */
BITS(ScpTrigs, 16, 4);		/* scope triggers */
BITS(MuxEn, 20, 4);		/* MuxEnable Bits as a Bit Field */
BITS(MuxSel, 24, 5);		/* MuxSelect Control Bits */
BITS(FastClks, 29, 4);		/* Fast clocks as a bit field */
BITS(SlowClks, 33, 5);		/* Slow clocks as a bit field */
BITS(CandR, 40, 4);		/* Clamp and Reset Control Bits */
BITS(RowDis, 44, 5);		/* Row Disable control bits */
BITS(RowEnbl,38,2);










