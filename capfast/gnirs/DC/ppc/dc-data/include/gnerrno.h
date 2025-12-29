/*****************************************************************************
 * Program:	GNAAC Detector controller Software
 * File:        gnerrno.h
 * Purpose:     define error numbers used by the GNAAC controller
 *
 * Author:      Nick C Buchholz
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *	16Sep1997 - created file - ncb
 *
 ****************************************************************************/

#ifndef GNERRORS
#define GNERRORS

#define GNAAC_OK		 0
#define BAD_TYPE		-1
#define BAD_RANGE		-2
#define BIASFAILED		-3
#define ARRAY_NOT_SETUP	 	-4
#define OBS_NOT_SETUP	 	-5		
#define DROI_NOT_SETUP		-6
#define VOLT_SET_FAILED		-7
#define DQ_GETPARAM_ERR		-10
#define BAD_DQ_PARAM		-11
#define CAPT_OBJ_NOT_SET	-12
#define CAPT_BUF_NOT_SET	-13
#define ACQ2DES_NOT_SET		-14
#define UNSCRMBL_NOT_SET	-15
#define ACQMV_NOT_SET		-16
#define ADDPAT_NOT_SET		-17
#define SUBPAT_NOT_SET		-17			
#define OBS_OUTOFORDER		-18
#define INVALID_UCODE		-19
#define RDD_NOT_SETUP		-20
#define RRD_NOT_SETUP		-21		
#define RDD_RUNTIME		-22
#define RRD_RUNTIME		-23
#define DQNOTREADY		-24
#define DQ_RPC_FAILED		-25
#define ABORTFAILED		-26		
#define OBSFAILED		-27		
#define PARKFAILED		-28		
#define CA_ERROR	 	-30
#define COADD_ERROR		-31
#define LNR_ERROR		-32
#define NDAVGS_ERROR		-33
#define NPICS_ERROR		-34
#define DISPOSE_FAILED		-35		
#define REBOOTFAILED		-36

#define SEP_NOT_SETUP		-80
#define STARE_NOT_SETUP		-81		
#define TEST_NOT_SETUP		-82

#define SEP_RUNTIME		-90
#define TEST_RUNTIME		-91
#define TPROC_NOT_SET		-92
#define MEMORY_ERROR		-99
#define UNK_ERROR		-999


#endif
