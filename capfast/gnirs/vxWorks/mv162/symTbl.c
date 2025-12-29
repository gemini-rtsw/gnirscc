/* makeSymTbl.c - stand-alone system symbol table */

/* CREATED BY makeSymTbl
 *       FROM tmp.o
 *         ON Thu May 18 17:55:18 PDT 1995
 */

#include "vxWorks.h"
#include "symbol.h"

#include "a_out.h"

#undef READ
#undef WRITE

IMPORT int _Randseed;
IMPORT __assert ();
IMPORT int __clocale;
IMPORT int __costate;
IMPORT int __ctype;
IMPORT __daysSinceEpoch ();
IMPORT __errno ();
IMPORT __exp10 ();
IMPORT __fixunsdfsi ();
IMPORT __fixunssfsi ();
IMPORT __getDstInfo ();
IMPORT __getTime ();
IMPORT __getZoneInfo ();
IMPORT __julday ();
IMPORT int __locale;
IMPORT int __loctime;
IMPORT __sclose ();
IMPORT __sflags ();
IMPORT __sflush ();
IMPORT __sfvwrite ();
IMPORT __smakebuf ();
IMPORT __sread ();
IMPORT __srefill ();
IMPORT __srget ();
IMPORT __sseek ();
IMPORT __stderr ();
IMPORT __stdin ();
IMPORT __stdout ();
IMPORT __strxfrm ();
IMPORT __swbuf ();
IMPORT __swrite ();
IMPORT __swsetup ();
IMPORT int _archHelp_msg;
IMPORT int _clockRealtime;
IMPORT int _dbgAdrsChkRtn;
IMPORT _dbgArchInit ();
IMPORT int _dbgDsmInstRtn;
IMPORT _dbgFuncCallCheck ();
IMPORT _dbgInfoPCGet ();
IMPORT _dbgInstPtrAlign ();
IMPORT _dbgInstSizeGet ();
IMPORT _dbgIntrInfoRestore ();
IMPORT _dbgIntrInfoSave ();
IMPORT _dbgRegsAdjust ();
IMPORT _dbgRetAdrsGet ();
IMPORT _dbgSStepClear ();
IMPORT _dbgSStepSet ();
IMPORT _dbgTaskBPModeClear ();
IMPORT _dbgTaskBPModeSet ();
IMPORT _dbgTaskPCGet ();
IMPORT _dbgTaskPCSet ();
IMPORT _dbgTaskSStepSet ();
IMPORT _dbgTraceDisable ();
IMPORT _dbgVecInit ();
IMPORT int _func_bdall;
IMPORT int _func_evtBufferCheck;
IMPORT int _func_evtLogM0;
IMPORT int _func_evtLogM1;
IMPORT int _func_evtLogM2;
IMPORT int _func_evtLogM3;
IMPORT int _func_evtLogO;
IMPORT int _func_evtLogOIntLock;
IMPORT int _func_evtLogPoint;
IMPORT int _func_evtLogString;
IMPORT int _func_evtLogT0;
IMPORT int _func_evtLogT1;
IMPORT int _func_evtLogTSched;
IMPORT int _func_excBaseHook;
IMPORT int _func_excInfoShow;
IMPORT int _func_excIntHook;
IMPORT int _func_excJobAdd;
IMPORT int _func_excPanicHook;
IMPORT int _func_fclose;
IMPORT int _func_fppTaskRegsShow;
IMPORT int _func_ftpLs;
IMPORT int _func_logMsg;
IMPORT int _func_memalign;
IMPORT int _func_netLsByName;
IMPORT int _func_remCurIdGet;
IMPORT int _func_remCurIdSet;
IMPORT int _func_scrPadToBuffer;
IMPORT int _func_selTyAdd;
IMPORT int _func_selTyDelete;
IMPORT int _func_selWakeupAll;
IMPORT int _func_selWakeupListInit;
IMPORT int _func_sigExcKill;
IMPORT int _func_sigTimeoutRecalc;
IMPORT int _func_sigprocmask;
IMPORT int _func_smObjObjShow;
IMPORT int _func_symFindByValueAndType;
IMPORT int _func_taskRegsShowRtn;
IMPORT int _func_tmrConnect;
IMPORT int _func_tmrDisable;
IMPORT int _func_tmrEnable;
IMPORT int _func_tmrFreq;
IMPORT int _func_tmrPeriod;
IMPORT int _func_tmrStamp;
IMPORT int _func_tmrStampLock;
IMPORT int _func_valloc;
IMPORT _insque ();
IMPORT int _l_PITBL;
IMPORT _l_denorm ();
IMPORT _l_dnrm_lp ();
IMPORT _l_dst_nan ();
IMPORT _l_fabsd ();
IMPORT _l_fabss ();
IMPORT _l_fabsx ();
IMPORT _l_facosd ();
IMPORT _l_facoss ();
IMPORT _l_facosx ();
IMPORT _l_faddd ();
IMPORT _l_fadds ();
IMPORT _l_faddx ();
IMPORT _l_fasind ();
IMPORT _l_fasins ();
IMPORT _l_fasinx ();
IMPORT _l_fatand ();
IMPORT _l_fatanhd ();
IMPORT _l_fatanhs ();
IMPORT _l_fatanhx ();
IMPORT _l_fatans ();
IMPORT _l_fatanx ();
IMPORT _l_fcosd ();
IMPORT _l_fcoshd ();
IMPORT _l_fcoshs ();
IMPORT _l_fcoshx ();
IMPORT _l_fcoss ();
IMPORT _l_fcosx ();
IMPORT _l_fdivd ();
IMPORT _l_fdivs ();
IMPORT _l_fdivx ();
IMPORT _l_fetoxd ();
IMPORT _l_fetoxm1d ();
IMPORT _l_fetoxm1s ();
IMPORT _l_fetoxm1x ();
IMPORT _l_fetoxs ();
IMPORT _l_fetoxx ();
IMPORT _l_fgetexpd ();
IMPORT _l_fgetexps ();
IMPORT _l_fgetexpx ();
IMPORT _l_fgetmand ();
IMPORT _l_fgetmans ();
IMPORT _l_fgetmanx ();
IMPORT _l_fintd ();
IMPORT _l_fintrzd ();
IMPORT _l_fintrzs ();
IMPORT _l_fintrzx ();
IMPORT _l_fints ();
IMPORT _l_fintx ();
IMPORT _l_flog10d ();
IMPORT _l_flog10s ();
IMPORT _l_flog10x ();
IMPORT _l_flog2d ();
IMPORT _l_flog2s ();
IMPORT _l_flog2x ();
IMPORT _l_flognd ();
IMPORT _l_flognp1d ();
IMPORT _l_flognp1s ();
IMPORT _l_flognp1x ();
IMPORT _l_flogns ();
IMPORT _l_flognx ();
IMPORT _l_fmodd ();
IMPORT _l_fmods ();
IMPORT _l_fmodx ();
IMPORT _l_fmuld ();
IMPORT _l_fmuls ();
IMPORT _l_fmulx ();
IMPORT _l_fnegd ();
IMPORT _l_fnegs ();
IMPORT _l_fnegx ();
IMPORT _l_fremd ();
IMPORT _l_frems ();
IMPORT _l_fremx ();
IMPORT _l_fscaled ();
IMPORT _l_fscales ();
IMPORT _l_fscalex ();
IMPORT _l_fsind ();
IMPORT _l_fsinhd ();
IMPORT _l_fsinhs ();
IMPORT _l_fsinhx ();
IMPORT _l_fsins ();
IMPORT _l_fsinx ();
IMPORT _l_fsqrtd ();
IMPORT _l_fsqrts ();
IMPORT _l_fsqrtx ();
IMPORT _l_fsubd ();
IMPORT _l_fsubs ();
IMPORT _l_fsubx ();
IMPORT _l_ftand ();
IMPORT _l_ftanhd ();
IMPORT _l_ftanhs ();
IMPORT _l_ftanhx ();
IMPORT _l_ftans ();
IMPORT _l_ftanx ();
IMPORT _l_ftentoxd ();
IMPORT _l_ftentoxs ();
IMPORT _l_ftentoxx ();
IMPORT _l_ftwotoxd ();
IMPORT _l_ftwotoxs ();
IMPORT _l_ftwotoxx ();
IMPORT _l_l_sint ();
IMPORT _l_l_sintd ();
IMPORT _l_l_sintrz ();
IMPORT _l_ld_minf ();
IMPORT _l_ld_mone ();
IMPORT _l_ld_mpi2 ();
IMPORT _l_ld_mzero ();
IMPORT _l_ld_pinf ();
IMPORT _l_ld_pone ();
IMPORT _l_ld_ppi2 ();
IMPORT _l_ld_pzero ();
IMPORT _l_mon_nan ();
IMPORT _l_nrm_set ();
IMPORT _l_nrm_zero ();
IMPORT _l_pmod ();
IMPORT _l_prem ();
IMPORT _l_pscale ();
IMPORT _l_round ();
IMPORT _l_sabs ();
IMPORT _l_sacos ();
IMPORT _l_sacosd ();
IMPORT _l_sadd ();
IMPORT _l_sasin ();
IMPORT _l_sasind ();
IMPORT _l_satan ();
IMPORT _l_satand ();
IMPORT _l_satanh ();
IMPORT _l_satanhd ();
IMPORT _l_scos ();
IMPORT _l_scosd ();
IMPORT _l_scosh ();
IMPORT _l_scoshd ();
IMPORT _l_sdiv ();
IMPORT _l_setox ();
IMPORT _l_setoxd ();
IMPORT _l_setoxm1 ();
IMPORT _l_setoxm1d ();
IMPORT _l_setoxm1i ();
IMPORT _l_sgetexp ();
IMPORT _l_sgetexpd ();
IMPORT _l_sgetman ();
IMPORT _l_sgetmand ();
IMPORT _l_sinf ();
IMPORT _l_sint ();
IMPORT _l_sintd ();
IMPORT _l_sintdo ();
IMPORT _l_sintrz ();
IMPORT _l_slog10 ();
IMPORT _l_slog10d ();
IMPORT _l_slog2 ();
IMPORT _l_slog2d ();
IMPORT _l_slogn ();
IMPORT _l_slognd ();
IMPORT _l_slognp1 ();
IMPORT _l_slognp1d ();
IMPORT _l_smod ();
IMPORT _l_smul ();
IMPORT _l_sneg ();
IMPORT _l_snzrinx ();
IMPORT _l_sone ();
IMPORT _l_sopr_inf ();
IMPORT _l_spi_2 ();
IMPORT _l_src_nan ();
IMPORT _l_srem ();
IMPORT _l_sscale ();
IMPORT _l_ssin ();
IMPORT _l_ssincos ();
IMPORT _l_ssincosd ();
IMPORT _l_ssincosi ();
IMPORT _l_ssincosnan ();
IMPORT _l_ssincosz ();
IMPORT _l_ssind ();
IMPORT _l_ssinh ();
IMPORT _l_ssinhd ();
IMPORT _l_sslog10 ();
IMPORT _l_sslog10d ();
IMPORT _l_sslog2 ();
IMPORT _l_sslog2d ();
IMPORT _l_sslogn ();
IMPORT _l_sslognd ();
IMPORT _l_sslognp1 ();
IMPORT _l_ssqrt ();
IMPORT _l_ssub ();
IMPORT _l_stan ();
IMPORT _l_stand ();
IMPORT _l_stanh ();
IMPORT _l_stanhd ();
IMPORT _l_stentox ();
IMPORT _l_stentoxd ();
IMPORT _l_sto_cos ();
IMPORT _l_stwotox ();
IMPORT _l_stwotoxd ();
IMPORT _l_szero ();
IMPORT _l_szr_inf ();
IMPORT _l_t_avoid_unsupp ();
IMPORT _l_t_dz ();
IMPORT _l_t_dz2 ();
IMPORT _l_t_extdnrm ();
IMPORT _l_t_frcinx ();
IMPORT _l_t_inx2 ();
IMPORT _l_t_operr ();
IMPORT _l_t_ovfl ();
IMPORT _l_t_ovfl2 ();
IMPORT _l_t_resdnrm ();
IMPORT _l_t_unfl ();
IMPORT _l_tag ();
IMPORT int _pSigQueueFreeHead;
IMPORT int _procNumWasSet;
IMPORT _remque ();
IMPORT _setjmpSetup ();
IMPORT _sigCtxLoad ();
IMPORT _sigCtxRtnValSet ();
IMPORT _sigCtxSave ();
IMPORT _sigCtxSetup ();
IMPORT _sigCtxStackEnd ();
IMPORT int _sigfaulttable;
IMPORT int _x_BIGRN;
IMPORT int _x_BIGRP;
IMPORT int _x_BIGRZRM;
IMPORT int _x_PIRN;
IMPORT int _x_PIRP;
IMPORT int _x_PIRZRM;
IMPORT int _x_PITBL;
IMPORT int _x_PTENRM;
IMPORT int _x_PTENRN;
IMPORT int _x_PTENRP;
IMPORT int _x_SMALRN;
IMPORT int _x_SMALRP;
IMPORT int _x_SMALRZRM;
IMPORT _x_ap_st_n ();
IMPORT _x_ap_st_z ();
IMPORT _x_b1238_fix ();
IMPORT _x_bindec ();
IMPORT _x_binstr ();
IMPORT _x_calc_e ();
IMPORT _x_calc_m ();
IMPORT _x_decbin ();
IMPORT _x_denorm ();
IMPORT _x_dest_dbl ();
IMPORT _x_dest_ext ();
IMPORT _x_dest_sgl ();
IMPORT _x_dnrm_lp ();
IMPORT _x_do_func ();
IMPORT _x_dst_nan ();
IMPORT _x_fpsp_bsun ();
IMPORT _x_fpsp_done ();
IMPORT _x_fpsp_dz ();
IMPORT _x_fpsp_fline ();
IMPORT _x_fpsp_fmt_error ();
IMPORT _x_fpsp_ill_inst ();
IMPORT _x_fpsp_inex ();
IMPORT _x_fpsp_operr ();
IMPORT _x_fpsp_ovfl ();
IMPORT _x_fpsp_snan ();
IMPORT _x_fpsp_unfl ();
IMPORT _x_fpsp_unsupp ();
IMPORT _x_g_dfmtou ();
IMPORT _x_g_opcls ();
IMPORT _x_g_rndpr ();
IMPORT _x_gen_except ();
IMPORT _x_get_fline ();
IMPORT _x_get_op ();
IMPORT _x_ld_minf ();
IMPORT _x_ld_mone ();
IMPORT _x_ld_mpi2 ();
IMPORT _x_ld_mzero ();
IMPORT _x_ld_pinf ();
IMPORT _x_ld_pone ();
IMPORT _x_ld_ppi2 ();
IMPORT _x_ld_pzero ();
IMPORT _x_mem_read ();
IMPORT _x_mem_write ();
IMPORT _x_norm ();
IMPORT _x_nrm_set ();
IMPORT _x_nrm_zero ();
IMPORT _x_ovf_r_k ();
IMPORT _x_ovf_r_x2 ();
IMPORT _x_ovf_r_x3 ();
IMPORT _x_ovf_res ();
IMPORT _x_p_move ();
IMPORT _x_pmod ();
IMPORT _x_prem ();
IMPORT _x_pscale ();
IMPORT _x_pwrten ();
IMPORT _x_real_bsun ();
IMPORT _x_real_dz ();
IMPORT _x_real_inex ();
IMPORT _x_real_operr ();
IMPORT _x_real_ovfl ();
IMPORT _x_real_snan ();
IMPORT _x_real_trace ();
IMPORT _x_real_unfl ();
IMPORT _x_real_unsupp ();
IMPORT _x_reg_dest ();
IMPORT _x_res_func ();
IMPORT _x_round ();
IMPORT _x_sacos ();
IMPORT _x_sacosd ();
IMPORT _x_sasin ();
IMPORT _x_sasind ();
IMPORT _x_satan ();
IMPORT _x_satand ();
IMPORT _x_satanh ();
IMPORT _x_satanhd ();
IMPORT _x_sc_mul ();
IMPORT _x_scos ();
IMPORT _x_scosd ();
IMPORT _x_scosh ();
IMPORT _x_scoshd ();
IMPORT _x_serror ();
IMPORT _x_setox ();
IMPORT _x_setoxd ();
IMPORT _x_setoxm1 ();
IMPORT _x_setoxm1d ();
IMPORT _x_setoxm1i ();
IMPORT _x_sgetexp ();
IMPORT _x_sgetexpd ();
IMPORT _x_sgetman ();
IMPORT _x_sgetmand ();
IMPORT _x_sinf ();
IMPORT _x_sint ();
IMPORT _x_sintd ();
IMPORT _x_sintdo ();
IMPORT _x_sintrz ();
IMPORT _x_slog10 ();
IMPORT _x_slog10d ();
IMPORT _x_slog2 ();
IMPORT _x_slog2d ();
IMPORT _x_slogn ();
IMPORT _x_slognd ();
IMPORT _x_slognp1 ();
IMPORT _x_slognp1d ();
IMPORT _x_smod ();
IMPORT _x_smovcr ();
IMPORT _x_snzrinx ();
IMPORT _x_sone ();
IMPORT _x_sopr_inf ();
IMPORT _x_spi_2 ();
IMPORT _x_src_nan ();
IMPORT _x_srem ();
IMPORT _x_sscale ();
IMPORT _x_ssin ();
IMPORT _x_ssincos ();
IMPORT _x_ssincosd ();
IMPORT _x_ssincosi ();
IMPORT _x_ssincosnan ();
IMPORT _x_ssincosz ();
IMPORT _x_ssind ();
IMPORT _x_ssinh ();
IMPORT _x_ssinhd ();
IMPORT _x_sslog10 ();
IMPORT _x_sslog10d ();
IMPORT _x_sslog2 ();
IMPORT _x_sslog2d ();
IMPORT _x_sslogn ();
IMPORT _x_sslognd ();
IMPORT _x_sslognp1 ();
IMPORT _x_stan ();
IMPORT _x_stand ();
IMPORT _x_stanh ();
IMPORT _x_stanhd ();
IMPORT _x_stentox ();
IMPORT _x_stentoxd ();
IMPORT _x_sto_cos ();
IMPORT _x_sto_res ();
IMPORT _x_store ();
IMPORT _x_stwotox ();
IMPORT _x_stwotoxd ();
IMPORT _x_szero ();
IMPORT _x_szr_inf ();
IMPORT _x_t_avoid_unsupp ();
IMPORT _x_t_dz ();
IMPORT _x_t_dz2 ();
IMPORT _x_t_extdnrm ();
IMPORT _x_t_frcinx ();
IMPORT _x_t_inx2 ();
IMPORT _x_t_operr ();
IMPORT _x_t_ovfl ();
IMPORT _x_t_ovfl2 ();
IMPORT _x_t_resdnrm ();
IMPORT _x_t_unfl ();
IMPORT int _x_tblpre;
IMPORT _x_unf_sub ();
IMPORT _x_uni_getop ();
IMPORT _x_uns_getop ();
IMPORT a0 ();
IMPORT a1 ();
IMPORT a2 ();
IMPORT a3 ();
IMPORT a4 ();
IMPORT a5 ();
IMPORT a6 ();
IMPORT a7 ();
IMPORT abort ();
IMPORT abs ();
IMPORT accept ();
IMPORT acos ();
IMPORT acosf ();
IMPORT int activeQHead;
IMPORT addSegNames ();
IMPORT int afswitch;
IMPORT arpAdd ();
IMPORT arpCmd ();
IMPORT arpDelete ();
IMPORT arpFlush ();
IMPORT arpShow ();
IMPORT arpinput ();
IMPORT arpioctl ();
IMPORT arpresolve ();
IMPORT int arptab;
IMPORT arptabShow ();
IMPORT int arptab_size;
IMPORT arptfree ();
IMPORT arptimer ();
IMPORT arptnew ();
IMPORT arpwhohas ();
IMPORT asctime ();
IMPORT asctime_r ();
IMPORT asin ();
IMPORT asinf ();
IMPORT int assertFiles;
IMPORT atan ();
IMPORT atan2 ();
IMPORT atan2f ();
IMPORT atanf ();
IMPORT atexit ();
IMPORT atof ();
IMPORT atoi ();
IMPORT atol ();
IMPORT b ();
IMPORT bcmp ();
IMPORT bcopy ();
IMPORT bcopyBytes ();
IMPORT bcopyLongs ();
IMPORT bcopyWords ();
IMPORT bcopy_to_mbufs ();
IMPORT bd ();
IMPORT bdTask ();
IMPORT bdall ();
IMPORT bfill ();
IMPORT bfillBytes ();
IMPORT bind ();
IMPORT bindresvport ();
IMPORT binvert ();
IMPORT bootBpAnchorExtract ();
IMPORT bootChange ();
IMPORT bootNetmaskExtract ();
IMPORT bootParamsErrorPrint ();
IMPORT bootParamsPrompt ();
IMPORT bootParamsShow ();
IMPORT bootScanNum ();
IMPORT bootStringToStruct ();
IMPORT bootStructToString ();
IMPORT bsearch ();
IMPORT bspVersion ();
IMPORT bswap ();
IMPORT int bufferAddress;
IMPORT build_cluster ();
IMPORT bzero ();
IMPORT c ();
IMPORT cache040DataDisable ();
IMPORT cache040WriteBufferFlush ();
IMPORT cacheArchClear ();
IMPORT cacheArchClearEntry ();
IMPORT cacheArchDisable ();
IMPORT cacheArchDmaFree ();
IMPORT cacheArchDmaMalloc ();
IMPORT cacheArchEnable ();
IMPORT cacheArchInvalidate ();
IMPORT cacheArchLibInit ();
IMPORT cacheArchLock ();
IMPORT cacheArchTextUpdate ();
IMPORT cacheArchUnlock ();
IMPORT cacheCACRGet ();
IMPORT cacheCACRSet ();
IMPORT cacheCINV ();
IMPORT cacheCPUSH ();
IMPORT cacheClear ();
IMPORT cacheDTTR0Get ();
IMPORT cacheDTTR0ModeSet ();
IMPORT cacheDTTR0Set ();
IMPORT int cacheDataEnabled;
IMPORT int cacheDataMode;
IMPORT cacheDisable ();
IMPORT cacheDmaFree ();
IMPORT int cacheDmaFreeRtn;
IMPORT int cacheDmaFuncs;
IMPORT cacheDmaMalloc ();
IMPORT int cacheDmaMallocRtn;
IMPORT cacheDrvFlush ();
IMPORT cacheDrvInvalidate ();
IMPORT cacheDrvPhysToVirt ();
IMPORT cacheDrvVirtToPhys ();
IMPORT cacheEnable ();
IMPORT cacheFlush ();
IMPORT cacheFuncsSet ();
IMPORT cacheInvalidate ();
IMPORT int cacheLib;
IMPORT cacheLibInit ();
IMPORT cacheLock ();
IMPORT int cacheMmuAvailable;
IMPORT int cacheNullFuncs;
IMPORT cachePipeFlush ();
IMPORT cacheTextUpdate ();
IMPORT cacheUnlock ();
IMPORT int cacheUserFuncs;
IMPORT calloc ();
IMPORT cbrt ();
IMPORT cbrtf ();
IMPORT cd ();
IMPORT ceil ();
IMPORT ceilf ();
IMPORT cfree ();
IMPORT changeReg ();
IMPORT chdir ();
IMPORT checkInetAddrField ();
IMPORT checkStack ();
IMPORT check_trailer ();
IMPORT checksum ();
IMPORT cksum ();
IMPORT int classClassId;
IMPORT classCreate ();
IMPORT classDestroy ();
IMPORT classHelpConnect ();
IMPORT classInit ();
IMPORT classLibInit ();
IMPORT classMemPartIdSet ();
IMPORT classShow ();
IMPORT classShowConnect ();
IMPORT classShowInit ();
IMPORT clearerr ();
IMPORT clock ();
IMPORT clockLibInit ();
IMPORT clock_getres ();
IMPORT clock_gettime ();
IMPORT clock_setres ();
IMPORT clock_settime ();
IMPORT clock_show ();
IMPORT close ();
IMPORT closedir ();
IMPORT int clusterConfig;
IMPORT connect ();
IMPORT connectWithTimeout ();
IMPORT int consoleFd;
IMPORT int consoleName;
IMPORT copy ();
IMPORT copyStreams ();
IMPORT int copyright_wind_river;
IMPORT copysign ();
IMPORT cos ();
IMPORT cosf ();
IMPORT cosh ();
IMPORT coshf ();
IMPORT cplusDemangle ();
IMPORT cplusLibInit ();
IMPORT cplusLoadFixup ();
IMPORT cplusMatchMangled ();
IMPORT cplusUnloadFixup ();
IMPORT creat ();
IMPORT int creationDate;
IMPORT int creationDateStr;
IMPORT cret ();
IMPORT ctime ();
IMPORT ctime_r ();
IMPORT int ctypeFiles;
IMPORT int currentContext;
IMPORT d ();
IMPORT d0 ();
IMPORT d1 ();
IMPORT d2 ();
IMPORT d3 ();
IMPORT d4 ();
IMPORT d5 ();
IMPORT d6 ();
IMPORT d7 ();
IMPORT dbgBpStub ();
IMPORT dbgBreakNotifyInstall ();
IMPORT dbgBreakpoint ();
IMPORT dbgBrkAdd ();
IMPORT dbgBrkDelete ();
IMPORT dbgBrkExists ();
IMPORT dbgBrkGet ();
IMPORT dbgBrkIgnoreDefault ();
IMPORT int dbgBrkIgnoreRtn;
IMPORT dbgHelp ();
IMPORT dbgInit ();
IMPORT int dbgLockUnbreakable;
IMPORT dbgPrintCall ();
IMPORT int dbgSafeUnbreakable;
IMPORT dbgStepQuiet ();
IMPORT dbgTrace ();
IMPORT dbgTraceStub ();
IMPORT devs ();
IMPORT difftime ();
IMPORT diskFormat ();
IMPORT diskInit ();
IMPORT div ();
IMPORT div_r ();
IMPORT dllAdd ();
IMPORT dllCount ();
IMPORT dllCreate ();
IMPORT dllDelete ();
IMPORT dllEach ();
IMPORT dllGet ();
IMPORT dllInit ();
IMPORT dllInsert ();
IMPORT dllRemove ();
IMPORT dllTerminate ();
IMPORT do_protocol ();
IMPORT do_protocol_with_type ();
IMPORT domaininit ();
IMPORT int domains;
IMPORT drem ();
IMPORT int drvTable;
IMPORT dsmData ();
IMPORT dsmInst ();
IMPORT dsmNbytes ();
IMPORT int eiEnetAddr;
IMPORT eiattach ();
IMPORT int enpNewROM;
IMPORT enpShow ();
IMPORT enpattach ();
IMPORT envLibInit ();
IMPORT envPrivateCreate ();
IMPORT envPrivateDestroy ();
IMPORT envShow ();
IMPORT int errno;
IMPORT errnoGet ();
IMPORT errnoOfTaskGet ();
IMPORT errnoOfTaskSet ();
IMPORT errnoSet ();
IMPORT etherAddrResolve ();
IMPORT etherInputHookAdd ();
IMPORT etherInputHookDelete ();
IMPORT int etherInputHookRtn;
IMPORT etherOutput ();
IMPORT etherOutputHookAdd ();
IMPORT etherOutputHookDelete ();
IMPORT int etherOutputHookRtn;
IMPORT ether_attach ();
IMPORT ether_output ();
IMPORT ether_sprintf ();
IMPORT int etherbroadcastaddr;
IMPORT int evtBufIsEmpty;
IMPORT int evtBufOverflow;
IMPORT int evtBufPostMortem;
IMPORT int evtBufSem;
IMPORT int evtLogOIsOn;
IMPORT int evtLogTIsOn;
IMPORT exattach ();
IMPORT excExcHandle ();
IMPORT int excExcepHook;
IMPORT excHookAdd ();
IMPORT excInit ();
IMPORT excIntHandle ();
IMPORT excIntStub ();
IMPORT excJobAdd ();
IMPORT int excMsgQId;
IMPORT excShowInit ();
IMPORT excStub ();
IMPORT excTask ();
IMPORT int excTaskId;
IMPORT int excTaskOptions;
IMPORT int excTaskPriority;
IMPORT int excTaskStackSize;
IMPORT excVecInit ();
IMPORT execute ();
IMPORT exit ();
IMPORT exp ();
IMPORT exp__E ();
IMPORT expf ();
IMPORT fabs ();
IMPORT fabsf ();
IMPORT fclose ();
IMPORT int fdTable;
IMPORT fdopen ();
IMPORT fdprintf ();
IMPORT feof ();
IMPORT ferror ();
IMPORT fflush ();
IMPORT ffsMsb ();
IMPORT fgetc ();
IMPORT fgetpos ();
IMPORT fgets ();
IMPORT int fieldSzIncludeSign;
IMPORT fileno ();
IMPORT finite ();
IMPORT fioFltInstall ();
IMPORT fioFormatV ();
IMPORT fioRdString ();
IMPORT fioRead ();
IMPORT fioScanV ();
IMPORT floatInit ();
IMPORT floor ();
IMPORT floorf ();
IMPORT fmod ();
IMPORT fmodf ();
IMPORT fopen ();
IMPORT int fpClassId;
IMPORT int fpCtlRegName;
IMPORT int fpRegName;
IMPORT fpTypeGet ();
IMPORT fppArchInit ();
IMPORT fppArchTaskCreateInit ();
IMPORT int fppCreateHookRtn;
IMPORT int fppDisplayHookRtn;
IMPORT fppDtoDx ();
IMPORT fppDxtoD ();
IMPORT fppInit ();
IMPORT fppProbe ();
IMPORT fppProbeSup ();
IMPORT fppProbeTrap ();
IMPORT fppReset ();
IMPORT fppRestore ();
IMPORT fppSave ();
IMPORT fppShowInit ();
IMPORT int fppTaskRegsCFmt;
IMPORT int fppTaskRegsDFmt;
IMPORT fppTaskRegsGet ();
IMPORT fppTaskRegsSet ();
IMPORT fppTaskRegsShow ();
IMPORT fprintf ();
IMPORT fputc ();
IMPORT fputs ();
IMPORT fread ();
IMPORT free ();
IMPORT freopen ();
IMPORT frexp ();
IMPORT fscanf ();
IMPORT fseek ();
IMPORT fsetpos ();
IMPORT fstat ();
IMPORT fstatfs ();
IMPORT ftell ();
IMPORT ftpCommand ();
IMPORT ftpDataConnGet ();
IMPORT ftpDataConnInit ();
IMPORT int ftpDebug;
IMPORT int ftpErrorSuppress;
IMPORT ftpHookup ();
IMPORT ftpLogin ();
IMPORT ftpLs ();
IMPORT ftpReplyGet ();
IMPORT int ftpVerbose;
IMPORT ftpXfer ();
IMPORT int ftpdDebug;
IMPORT ftpdDelete ();
IMPORT ftpdInit ();
IMPORT ftpdTask ();
IMPORT int ftpdTaskPriority;
IMPORT int ftpdWorkTaskOptions;
IMPORT int ftpdWorkTaskPriority;
IMPORT int ftpdWorkTaskStackSize;
IMPORT fwrite ();
IMPORT gccUss040Init ();
IMPORT getc ();
IMPORT getchar ();
IMPORT getcwd ();
IMPORT getenv ();
IMPORT gethostname ();
IMPORT getpeername ();
IMPORT gets ();
IMPORT getsockname ();
IMPORT getsockopt ();
IMPORT getw ();
IMPORT getwd ();
IMPORT gmtime ();
IMPORT gmtime_r ();
IMPORT h ();
IMPORT int hashClassId;
IMPORT hashFuncIterScale ();
IMPORT hashFuncModulo ();
IMPORT hashFuncMultiply ();
IMPORT hashKeyCmp ();
IMPORT hashKeyStrCmp ();
IMPORT hashLibInit ();
IMPORT hashTblCreate ();
IMPORT hashTblDelete ();
IMPORT hashTblDestroy ();
IMPORT hashTblEach ();
IMPORT hashTblFind ();
IMPORT hashTblInit ();
IMPORT hashTblPut ();
IMPORT hashTblRemove ();
IMPORT hashTblTerminate ();
IMPORT help ();
IMPORT hostAdd ();
IMPORT hostDelete ();
IMPORT hostGetByAddr ();
IMPORT hostGetByName ();
IMPORT int hostList;
IMPORT int hostListSem;
IMPORT hostShow ();
IMPORT hostTblInit ();
IMPORT hypot ();
IMPORT hypotf ();
IMPORT i ();
IMPORT iam ();
IMPORT icmp_error ();
IMPORT icmp_input ();
IMPORT icmp_reflect ();
IMPORT icmp_send ();
IMPORT int icmpstat;
IMPORT icmpstatShow ();
IMPORT ifAddrGet ();
IMPORT ifAddrSet ();
IMPORT int ifAttachChange;
IMPORT ifBroadcastGet ();
IMPORT ifBroadcastSet ();
IMPORT ifDstAddrGet ();
IMPORT ifDstAddrSet ();
IMPORT ifFlagChange ();
IMPORT ifFlagGet ();
IMPORT ifFlagSet ();
IMPORT ifMaskGet ();
IMPORT ifMaskSet ();
IMPORT ifMetricGet ();
IMPORT ifMetricSet ();
IMPORT ifRouteDelete ();
IMPORT ifShow ();
IMPORT if_attach ();
IMPORT if_dettach ();
IMPORT if_down ();
IMPORT if_qflush ();
IMPORT if_slowtimo ();
IMPORT ifa_ifwithaddr ();
IMPORT ifa_ifwithdstaddr ();
IMPORT ifa_ifwithnet ();
IMPORT ifconf ();
IMPORT ifinit ();
IMPORT ifioctl ();
IMPORT int ifnet;
IMPORT ifptoia ();
IMPORT int ifqmaxlen;
IMPORT ifreset ();
IMPORT ifunit ();
IMPORT in_arpinput ();
IMPORT in_broadcast ();
IMPORT in_canforward ();
IMPORT in_cksum ();
IMPORT in_control ();
IMPORT in_iaonnetof ();
IMPORT int in_ifaddr;
IMPORT in_ifaddr_remove ();
IMPORT in_ifinit ();
IMPORT int in_interfaces;
IMPORT in_lnaof ();
IMPORT in_localaddr ();
IMPORT in_losing ();
IMPORT in_makeaddr_b ();
IMPORT in_netof ();
IMPORT in_pcballoc ();
IMPORT in_pcbbind ();
IMPORT in_pcbconnect ();
IMPORT in_pcbdetach ();
IMPORT in_pcbdisconnect ();
IMPORT in_pcblookup ();
IMPORT in_pcbnotify ();
IMPORT in_rtchange ();
IMPORT in_setpeeraddr ();
IMPORT in_setsockaddr ();
IMPORT index ();
IMPORT inet_addr ();
IMPORT inet_hash ();
IMPORT inet_lnaof ();
IMPORT inet_makeaddr ();
IMPORT inet_makeaddr_b ();
IMPORT inet_netmatch ();
IMPORT inet_netof ();
IMPORT inet_netof_string ();
IMPORT inet_network ();
IMPORT inet_ntoa ();
IMPORT inet_ntoa_b ();
IMPORT int inetctlerrmap;
IMPORT int inetdomain;
IMPORT inetstatShow ();
IMPORT int inetsw;
IMPORT infinity ();
IMPORT infinityf ();
IMPORT int intCnt;
IMPORT intConnect ();
IMPORT intContext ();
IMPORT intCount ();
IMPORT intEnt ();
IMPORT intExit ();
IMPORT intHandlerCreate ();
IMPORT intLevelSet ();
IMPORT intLock ();
IMPORT int intLockIntSR;
IMPORT intLockLevelGet ();
IMPORT intLockLevelSet ();
IMPORT int intLockMask;
IMPORT int intLockTaskSR;
IMPORT intRestrict ();
IMPORT intUnlock ();
IMPORT intVBRSet ();
IMPORT intVecBaseGet ();
IMPORT intVecBaseSet ();
IMPORT intVecGet ();
IMPORT intVecSet ();
IMPORT intVecTableWriteProtect ();
IMPORT ioDefDevGet ();
IMPORT ioDefDirGet ();
IMPORT int ioDefPath;
IMPORT ioDefPathCat ();
IMPORT ioDefPathGet ();
IMPORT ioDefPathSet ();
IMPORT ioFullFileNameGet ();
IMPORT ioGlobalStdGet ();
IMPORT ioGlobalStdSet ();
IMPORT int ioMaxLinkLevels;
IMPORT ioTaskStdGet ();
IMPORT ioTaskStdSet ();
IMPORT ioctl ();
IMPORT iosClose ();
IMPORT iosCreate ();
IMPORT iosDelete ();
IMPORT iosDevAdd ();
IMPORT iosDevDelete ();
IMPORT iosDevFind ();
IMPORT iosDevShow ();
IMPORT iosDrvInstall ();
IMPORT iosDrvRemove ();
IMPORT iosDrvShow ();
IMPORT int iosDvList;
IMPORT iosFdDevFind ();
IMPORT iosFdFree ();
IMPORT int iosFdFreeHookRtn;
IMPORT iosFdNew ();
IMPORT int iosFdNewHookRtn;
IMPORT iosFdSet ();
IMPORT iosFdShow ();
IMPORT iosFdValue ();
IMPORT iosInit ();
IMPORT iosIoctl ();
IMPORT int iosLibInitialized;
IMPORT iosNextDevGet ();
IMPORT iosOpen ();
IMPORT iosRead ();
IMPORT iosShowInit ();
IMPORT iosWrite ();
IMPORT int ipFragCreates;
IMPORT int ipFragFails;
IMPORT int ipFragOKs;
IMPORT int ipInDelivers;
IMPORT int ipInUnknownProtos;
IMPORT int ipOutDatagrams;
IMPORT int ipOutDiscards;
IMPORT int ipOutNoRoutes;
IMPORT int ipReasmOKs;
IMPORT int ipReasmReqds;
IMPORT int ipTimeToLive;
IMPORT ip_ctloutput ();
IMPORT ip_deq ();
IMPORT ip_dooptions ();
IMPORT ip_drain ();
IMPORT ip_enq ();
IMPORT ip_forward ();
IMPORT ip_freef ();
IMPORT int ip_id;
IMPORT ip_init ();
IMPORT ip_insertoptions ();
IMPORT int ip_nhops;
IMPORT ip_optcopy ();
IMPORT ip_output ();
IMPORT ip_pcbopts ();
IMPORT int ip_protox;
IMPORT ip_reass ();
IMPORT ip_rtaddr ();
IMPORT ip_slowtimo ();
IMPORT ip_srcroute ();
IMPORT ip_stripoptions ();
IMPORT int ipaddr;
IMPORT int ipcksum;
IMPORT int ipforward_rt;
IMPORT int ipforwarding;
IMPORT ipintr ();
IMPORT int ipintrq;
IMPORT int ipprintfs;
IMPORT int ipqmaxlen;
IMPORT int ipsendredirects;
IMPORT int ipstat;
IMPORT ipstatShow ();
IMPORT iptime ();
IMPORT irint ();
IMPORT irintf ();
IMPORT iround ();
IMPORT iroundf ();
IMPORT isalnum ();
IMPORT isalpha ();
IMPORT isatty ();
IMPORT iscntrl ();
IMPORT isdigit ();
IMPORT isgraph ();
IMPORT islower ();
IMPORT isprint ();
IMPORT ispunct ();
IMPORT isspace ();
IMPORT isupper ();
IMPORT isxdigit ();
IMPORT kernelInit ();
IMPORT int kernelIsIdle;
IMPORT int kernelState;
IMPORT kernelTimeSlice ();
IMPORT kernelVersion ();
IMPORT kill ();
IMPORT ksleep ();
IMPORT l ();
IMPORT labs ();
IMPORT ld ();
IMPORT ldexp ();
IMPORT ldiv ();
IMPORT ldiv_r ();
IMPORT ledClose ();
IMPORT ledControl ();
IMPORT int ledId;
IMPORT ledOpen ();
IMPORT ledRead ();
IMPORT lexActions ();
IMPORT int lexClass;
IMPORT int lexNclasses;
IMPORT int lexStateTable;
IMPORT listen ();
IMPORT lkAddr ();
IMPORT lkup ();
IMPORT ll ();
IMPORT loadAoutInit ();
IMPORT loadModule ();
IMPORT loadModuleAt ();
IMPORT loadModuleAtSym ();
IMPORT loadModuleGet ();
IMPORT int loadRoutine;
IMPORT loadSegmentsAllocate ();
IMPORT loanBuild ();
IMPORT loattach ();
IMPORT int localToGlobalOffset;
IMPORT int localeFiles;
IMPORT localeconv ();
IMPORT localtime ();
IMPORT localtime_r ();
IMPORT log ();
IMPORT log10 ();
IMPORT log10f ();
IMPORT log2 ();
IMPORT log2f ();
IMPORT logFdAdd ();
IMPORT logFdDelete ();
IMPORT int logFdFromRlogin;
IMPORT logFdSet ();
IMPORT logInit ();
IMPORT logMsg ();
IMPORT logShow ();
IMPORT logTask ();
IMPORT int logTaskId;
IMPORT int logTaskOptions;
IMPORT int logTaskPriority;
IMPORT int logTaskStackSize;
IMPORT log__L ();
IMPORT logb ();
IMPORT logf ();
IMPORT logout ();
IMPORT int loif;
IMPORT longjmp ();
IMPORT looutput ();
IMPORT ls ();
IMPORT lsOld ();
IMPORT lseek ();
IMPORT lstAdd ();
IMPORT lstConcat ();
IMPORT lstCount ();
IMPORT lstDelete ();
IMPORT lstExtract ();
IMPORT lstFind ();
IMPORT lstFirst ();
IMPORT lstFree ();
IMPORT lstGet ();
IMPORT lstInit ();
IMPORT lstInsert ();
IMPORT lstLast ();
IMPORT lstNStep ();
IMPORT lstNext ();
IMPORT lstNth ();
IMPORT lstPrevious ();
IMPORT m ();
IMPORT mRegs ();
IMPORT m_adj ();
IMPORT m_cat ();
IMPORT m_clalloc ();
IMPORT m_copy ();
IMPORT m_expand ();
IMPORT m_free ();
IMPORT m_freem ();
IMPORT m_get ();
IMPORT m_getclr ();
IMPORT m_more ();
IMPORT m_pullup ();
IMPORT int m_want;
IMPORT malloc ();
IMPORT int mathAcosFunc;
IMPORT int mathAcosfFunc;
IMPORT int mathAsinFunc;
IMPORT int mathAsinfFunc;
IMPORT int mathAtan2Func;
IMPORT int mathAtan2fFunc;
IMPORT int mathAtanFunc;
IMPORT int mathAtanfFunc;
IMPORT int mathCbrtFunc;
IMPORT int mathCbrtfFunc;
IMPORT int mathCeilFunc;
IMPORT int mathCeilfFunc;
IMPORT int mathCosFunc;
IMPORT int mathCosfFunc;
IMPORT int mathCoshFunc;
IMPORT int mathCoshfFunc;
IMPORT mathErrNoInit ();
IMPORT int mathExpFunc;
IMPORT int mathExpfFunc;
IMPORT int mathFabsFunc;
IMPORT int mathFabsfFunc;
IMPORT int mathFiles;
IMPORT int mathFloorFunc;
IMPORT int mathFloorfFunc;
IMPORT int mathFmodFunc;
IMPORT int mathFmodfFunc;
IMPORT mathHardAcos ();
IMPORT mathHardAsin ();
IMPORT mathHardAtan ();
IMPORT mathHardAtan2 ();
IMPORT mathHardCeil ();
IMPORT mathHardCos ();
IMPORT mathHardCosh ();
IMPORT mathHardExp ();
IMPORT mathHardFabs ();
IMPORT mathHardFloor ();
IMPORT mathHardFmod ();
IMPORT mathHardInfinity ();
IMPORT mathHardInit ();
IMPORT mathHardIrint ();
IMPORT mathHardIround ();
IMPORT mathHardLog ();
IMPORT mathHardLog10 ();
IMPORT mathHardLog2 ();
IMPORT mathHardPow ();
IMPORT mathHardRound ();
IMPORT mathHardSin ();
IMPORT mathHardSincos ();
IMPORT mathHardSinh ();
IMPORT mathHardSqrt ();
IMPORT mathHardTan ();
IMPORT mathHardTanh ();
IMPORT mathHardTrunc ();
IMPORT int mathHypotFunc;
IMPORT int mathHypotfFunc;
IMPORT int mathInfinityFunc;
IMPORT int mathInfinityfFunc;
IMPORT int mathIrintFunc;
IMPORT int mathIrintfFunc;
IMPORT int mathIroundFunc;
IMPORT int mathIroundfFunc;
IMPORT int mathLog10Func;
IMPORT int mathLog10fFunc;
IMPORT int mathLog2Func;
IMPORT int mathLog2fFunc;
IMPORT int mathLogFunc;
IMPORT int mathLogfFunc;
IMPORT int mathPowFunc;
IMPORT int mathPowfFunc;
IMPORT int mathRoundFunc;
IMPORT int mathRoundfFunc;
IMPORT int mathSinFunc;
IMPORT int mathSincosFunc;
IMPORT int mathSincosfFunc;
IMPORT int mathSinfFunc;
IMPORT int mathSinhFunc;
IMPORT int mathSinhfFunc;
IMPORT int mathSqrtFunc;
IMPORT int mathSqrtfFunc;
IMPORT int mathTanFunc;
IMPORT int mathTanfFunc;
IMPORT int mathTanhFunc;
IMPORT int mathTanhfFunc;
IMPORT int mathTruncFunc;
IMPORT int mathTruncfFunc;
IMPORT int maxDrivers;
IMPORT int maxFiles;
IMPORT mbinit ();
IMPORT mblen ();
IMPORT int mbstat;
IMPORT mbstowcs ();
IMPORT mbtowc ();
IMPORT int mbufConfig;
IMPORT int mbufSem;
IMPORT mbufShow ();
IMPORT int mclfree;
IMPORT int mclrefcnt;
IMPORT memAddToPool ();
IMPORT int memDefaultAlignment;
IMPORT memFindMax ();
IMPORT memInit ();
IMPORT memLibInit ();
IMPORT memOptionsSet ();
IMPORT memPartAddToPool ();
IMPORT memPartAlignedAlloc ();
IMPORT memPartAlloc ();
IMPORT int memPartAllocErrorRtn;
IMPORT int memPartBlockErrorRtn;
IMPORT memPartBlockIsValid ();
IMPORT int memPartClassId;
IMPORT memPartCreate ();
IMPORT memPartFindMax ();
IMPORT memPartFree ();
IMPORT memPartInfoGet ();
IMPORT memPartInit ();
IMPORT memPartLibInit ();
IMPORT int memPartOptionsDefault;
IMPORT memPartOptionsSet ();
IMPORT memPartRealloc ();
IMPORT int memPartSemInitRtn;
IMPORT memPartShow ();
IMPORT memShow ();
IMPORT memShowInit ();
IMPORT int memSysPartId;
IMPORT memalign ();
IMPORT memchr ();
IMPORT memcmp ();
IMPORT memcpy ();
IMPORT memmove ();
IMPORT memset ();
IMPORT int mfree;
IMPORT mkdir ();
IMPORT mktime ();
IMPORT mmu40LibInit ();
IMPORT int mmuLibFuncs;
IMPORT int mmuNumPagesInFreeList;
IMPORT int mmuPageBlockSize;
IMPORT int mmuPageSize;
IMPORT int mmuPageSource;
IMPORT int mmuPhysAddrShift;
IMPORT int mmuStateTransArray;
IMPORT int mmuStateTransArraySize;
IMPORT modf ();
IMPORT moduleCheck ();
IMPORT int moduleClassId;
IMPORT moduleCreate ();
IMPORT moduleCreateHookAdd ();
IMPORT moduleCreateHookDelete ();
IMPORT moduleDelete ();
IMPORT moduleEach ();
IMPORT moduleFindByGroup ();
IMPORT moduleFindByName ();
IMPORT moduleFindByNameAndPath ();
IMPORT moduleFlagsGet ();
IMPORT moduleIdFigure ();
IMPORT moduleIdListGet ();
IMPORT moduleInfoGet ();
IMPORT moduleInit ();
IMPORT moduleLibInit ();
IMPORT moduleNameGet ();
IMPORT moduleSegAdd ();
IMPORT moduleSegEach ();
IMPORT moduleSegFirst ();
IMPORT moduleSegGet ();
IMPORT moduleSegNext ();
IMPORT moduleShow ();
IMPORT moduleTerminate ();
IMPORT int msgQClassId;
IMPORT msgQCreate ();
IMPORT msgQDelete ();
IMPORT msgQInfoGet ();
IMPORT msgQInit ();
IMPORT msgQLibInit ();
IMPORT msgQNumMsgs ();
IMPORT msgQReceive ();
IMPORT msgQSend ();
IMPORT msgQShow ();
IMPORT msgQShowInit ();
IMPORT int msgQSmInfoGetRtn;
IMPORT int msgQSmNumMsgsRtn;
IMPORT int msgQSmReceiveRtn;
IMPORT int msgQSmSendRtn;
IMPORT int msgQSmShowRtn;
IMPORT msgQTerminate ();
IMPORT int mutexOptionsFtpdLib;
IMPORT int mutexOptionsHostLib;
IMPORT int mutexOptionsIosLib;
IMPORT int mutexOptionsLogLib;
IMPORT int mutexOptionsMemLib;
IMPORT int mutexOptionsNetDrv;
IMPORT int mutexOptionsSelectLib;
IMPORT int mutexOptionsSymLib;
IMPORT int mutexOptionsTyLib;
IMPORT int mutexOptionsUnixLib;
IMPORT int mutexOptionsVmBaseLib;
IMPORT int namelessPrefix;
IMPORT netDevCreate ();
IMPORT netDrv ();
IMPORT netErrnoSet ();
IMPORT netHelp ();
IMPORT netJobAdd ();
IMPORT netLibInit ();
IMPORT netLsByName ();
IMPORT int netLsStr;
IMPORT netShowInit ();
IMPORT netTask ();
IMPORT int netTaskId;
IMPORT int netTaskOptions;
IMPORT int netTaskPriority;
IMPORT int netTaskSemId;
IMPORT int netTaskStackSize;
IMPORT netTypeAdd ();
IMPORT netTypeDelete ();
IMPORT netTypeInit ();
IMPORT null_hash ();
IMPORT null_init ();
IMPORT null_netmatch ();
IMPORT objAlloc ();
IMPORT objAllocExtra ();
IMPORT objCoreInit ();
IMPORT objCoreTerminate ();
IMPORT objFree ();
IMPORT objShow ();
IMPORT open ();
IMPORT opendir ();
IMPORT int pEvtDblBuffers;
IMPORT int pFppTaskIdPrevious;
IMPORT int pJobPool;
IMPORT int pRootMemStart;
IMPORT int pScrPad;
IMPORT int pScrPadIndex;
IMPORT int pTaskLastFpTcb;
IMPORT panic ();
IMPORT int panicSuspend;
IMPORT pathBuild ();
IMPORT pathCat ();
IMPORT pathCondense ();
IMPORT pathLastName ();
IMPORT pathLastNamePtr ();
IMPORT pathParse ();
IMPORT pathSplit ();
IMPORT pause ();
IMPORT pc ();
IMPORT period ();
IMPORT periodRun ();
IMPORT perror ();
IMPORT pfctlinput ();
IMPORT pffasttimo ();
IMPORT pffindproto ();
IMPORT pffindtype ();
IMPORT pfslowtimo ();
IMPORT pipeDevCreate ();
IMPORT pipeDrv ();
IMPORT int pipeMsgQOptions;
IMPORT pow ();
IMPORT pow_p ();
IMPORT powf ();
IMPORT int ppGlobalEnviron;
IMPORT printErr ();
IMPORT printErrno ();
IMPORT printExc ();
IMPORT printLogo ();
IMPORT printf ();
IMPORT int proxyArpHook;
IMPORT int proxyBroadcastHook;
IMPORT ptyDevCreate ();
IMPORT ptyDrv ();
IMPORT putc ();
IMPORT putchar ();
IMPORT putenv ();
IMPORT puts ();
IMPORT putw ();
IMPORT pwd ();
IMPORT qAdvance ();
IMPORT qCalibrate ();
IMPORT qCreate ();
IMPORT qDelete ();
IMPORT qEach ();
IMPORT int qFifoClassId;
IMPORT qFifoCreate ();
IMPORT qFifoDelete ();
IMPORT qFifoEach ();
IMPORT qFifoGet ();
IMPORT qFifoInfo ();
IMPORT qFifoInit ();
IMPORT qFifoPut ();
IMPORT qFifoRemove ();
IMPORT qFirst ();
IMPORT qGet ();
IMPORT qGetExpired ();
IMPORT qInfo ();
IMPORT qInit ();
IMPORT int qJobClassId;
IMPORT qJobCreate ();
IMPORT qJobDelete ();
IMPORT qJobEach ();
IMPORT qJobGet ();
IMPORT qJobInfo ();
IMPORT qJobInit ();
IMPORT qJobPut ();
IMPORT qJobTerminate ();
IMPORT qKey ();
IMPORT int qPriBMapClassId;
IMPORT qPriBMapCreate ();
IMPORT qPriBMapDelete ();
IMPORT qPriBMapEach ();
IMPORT qPriBMapGet ();
IMPORT qPriBMapInfo ();
IMPORT qPriBMapInit ();
IMPORT qPriBMapKey ();
IMPORT qPriBMapListCreate ();
IMPORT qPriBMapListDelete ();
IMPORT qPriBMapPut ();
IMPORT qPriBMapRemove ();
IMPORT qPriBMapResort ();
IMPORT qPriListAdvance ();
IMPORT qPriListCalibrate ();
IMPORT int qPriListClassId;
IMPORT qPriListCreate ();
IMPORT qPriListDelete ();
IMPORT qPriListEach ();
IMPORT int qPriListFromTailClassId;
IMPORT qPriListGet ();
IMPORT qPriListGetExpired ();
IMPORT qPriListInfo ();
IMPORT qPriListInit ();
IMPORT qPriListKey ();
IMPORT qPriListPut ();
IMPORT qPriListPutFromTail ();
IMPORT qPriListRemove ();
IMPORT qPriListResort ();
IMPORT qPriListTerminate ();
IMPORT qPut ();
IMPORT qRemove ();
IMPORT qResort ();
IMPORT qTerminate ();
IMPORT qsort ();
IMPORT raise ();
IMPORT rand ();
IMPORT raw_attach ();
IMPORT raw_bind ();
IMPORT raw_connaddr ();
IMPORT raw_ctlinput ();
IMPORT raw_detach ();
IMPORT raw_disconnect ();
IMPORT raw_init ();
IMPORT raw_input ();
IMPORT int raw_recvspace;
IMPORT int raw_sendspace;
IMPORT raw_usrreq ();
IMPORT int rawcb;
IMPORT rawintr ();
IMPORT int rawintrq;
IMPORT rcmd ();
IMPORT read ();
IMPORT readdir ();
IMPORT readv ();
IMPORT int readyQBMap;
IMPORT int readyQHead;
IMPORT realloc ();
IMPORT reboot ();
IMPORT rebootHookAdd ();
IMPORT recv ();
IMPORT recvfrom ();
IMPORT recvmsg ();
IMPORT int redirInFd;
IMPORT int redirOutFd;
IMPORT reld ();
IMPORT remCurIdGet ();
IMPORT remCurIdSet ();
IMPORT int remLastResvPort;
IMPORT remove ();
IMPORT rename ();
IMPORT repeat ();
IMPORT repeatRun ();
IMPORT reschedule ();
IMPORT int restartTaskName;
IMPORT int restartTaskOptions;
IMPORT int restartTaskPriority;
IMPORT int restartTaskStackSize;
IMPORT rewind ();
IMPORT rewinddir ();
IMPORT rindex ();
IMPORT rip_ctloutput ();
IMPORT rip_input ();
IMPORT rip_output ();
IMPORT int ripdst;
IMPORT int ripproto;
IMPORT int ripsrc;
IMPORT rlogChildTask ();
IMPORT int rlogChildTaskId;
IMPORT rlogInTask ();
IMPORT int rlogInTaskId;
IMPORT rlogInit ();
IMPORT rlogOutTask ();
IMPORT int rlogOutTaskId;
IMPORT int rlogShellName;
IMPORT int rlogTaskOptions;
IMPORT int rlogTaskPriority;
IMPORT int rlogTaskStackSize;
IMPORT int rlogTermType;
IMPORT rlogin ();
IMPORT rlogind ();
IMPORT int rlogindId;
IMPORT int rlogindSocket;
IMPORT rm ();
IMPORT rmdir ();
IMPORT rngBufGet ();
IMPORT rngBufPut ();
IMPORT rngCreate ();
IMPORT rngDelete ();
IMPORT rngFlush ();
IMPORT rngFreeBytes ();
IMPORT rngIsEmpty ();
IMPORT rngIsFull ();
IMPORT rngMoveAhead ();
IMPORT rngNBytes ();
IMPORT rngPutAhead ();
IMPORT int rootMemNBytes;
IMPORT int rootTaskId;
IMPORT round ();
IMPORT int roundRobinOn;
IMPORT int roundRobinSlice;
IMPORT roundf ();
IMPORT routeAdd ();
IMPORT routeCmd ();
IMPORT routeDelete ();
IMPORT routeEntryFill ();
IMPORT routeNetAdd ();
IMPORT routeShow ();
IMPORT routestatShow ();
IMPORT rresvport ();
IMPORT rtalloc ();
IMPORT rtfree ();
IMPORT int rthashsize;
IMPORT int rthost;
IMPORT rtinit ();
IMPORT rtioctl ();
IMPORT int rtmodified;
IMPORT int rtnet;
IMPORT rtredirect ();
IMPORT rtrequest ();
IMPORT int rtstat;
IMPORT int rttrash;
IMPORT s ();
IMPORT save_rte ();
IMPORT sbappend ();
IMPORT sbappendaddr ();
IMPORT sbappendrecord ();
IMPORT sbappendrights ();
IMPORT sbcompress ();
IMPORT sbdrop ();
IMPORT sbdroprecord ();
IMPORT sbflush ();
IMPORT sbrelease ();
IMPORT sbreserve ();
IMPORT sbseldequeue ();
IMPORT sbselqueue ();
IMPORT sbwait ();
IMPORT sbwakeup ();
IMPORT scalb ();
IMPORT scanCharSet ();
IMPORT scanField ();
IMPORT scanf ();
IMPORT schednetisr ();
IMPORT selNodeAdd ();
IMPORT selNodeDelete ();
IMPORT selWakeup ();
IMPORT selWakeupAll ();
IMPORT selWakeupListInit ();
IMPORT selWakeupListLen ();
IMPORT selWakeupType ();
IMPORT select ();
IMPORT selectInit ();
IMPORT semBCoreInit ();
IMPORT semBCreate ();
IMPORT semBGive ();
IMPORT semBGiveDefer ();
IMPORT semBInit ();
IMPORT semBLibInit ();
IMPORT semBTake ();
IMPORT semCCoreInit ();
IMPORT semCCreate ();
IMPORT semCGive ();
IMPORT semCGiveDefer ();
IMPORT semCInit ();
IMPORT semCLibInit ();
IMPORT semCTake ();
IMPORT int semClass;
IMPORT int semClassId;
IMPORT semClear ();
IMPORT semDelete ();
IMPORT semDestroy ();
IMPORT semFlush ();
IMPORT semFlushDefer ();
IMPORT int semFlushDeferTbl;
IMPORT int semFlushTbl;
IMPORT semGive ();
IMPORT semGiveDefer ();
IMPORT int semGiveDeferTbl;
IMPORT int semGiveTbl;
IMPORT semInfo ();
IMPORT semIntRestrict ();
IMPORT semInvalid ();
IMPORT semLibInit ();
IMPORT semMCoreInit ();
IMPORT semMCreate ();
IMPORT semMGive ();
IMPORT semMGiveForce ();
IMPORT semMGiveKern ();
IMPORT int semMGiveKernWork;
IMPORT semMInit ();
IMPORT semMLibInit ();
IMPORT semMPendQPut ();
IMPORT semMTake ();
IMPORT semOTake ();
IMPORT semQFlush ();
IMPORT semQFlushDefer ();
IMPORT semQGet ();
IMPORT semQInit ();
IMPORT semQPut ();
IMPORT semShow ();
IMPORT semShowInit ();
IMPORT int semSmInfoRtn;
IMPORT int semSmShowRtn;
IMPORT semTake ();
IMPORT int semTakeTbl;
IMPORT semTerminate ();
IMPORT send ();
IMPORT sendmsg ();
IMPORT sendto ();
IMPORT set_if_addr ();
IMPORT setbuf ();
IMPORT setbuffer ();
IMPORT sethostname ();
IMPORT setjmp ();
IMPORT setlinebuf ();
IMPORT setlocale ();
IMPORT setsockopt ();
IMPORT setvbuf ();
IMPORT shell ();
IMPORT int shellHistSize;
IMPORT shellHistory ();
IMPORT shellInit ();
IMPORT shellLock ();
IMPORT shellLoginInstall ();
IMPORT shellLogout ();
IMPORT shellLogoutInstall ();
IMPORT shellOrigStdSet ();
IMPORT shellPromptSet ();
IMPORT shellRestart ();
IMPORT shellScriptAbort ();
IMPORT int shellTaskId;
IMPORT int shellTaskName;
IMPORT int shellTaskOptions;
IMPORT int shellTaskPriority;
IMPORT int shellTaskStackSize;
IMPORT show ();
IMPORT shutdown ();
IMPORT int sigEvtRtn;
IMPORT sigInit ();
IMPORT sigPendDestroy ();
IMPORT sigPendInit ();
IMPORT sigPendKill ();
IMPORT sigaction ();
IMPORT sigaddset ();
IMPORT sigblock ();
IMPORT sigdelset ();
IMPORT sigemptyset ();
IMPORT sigfillset ();
IMPORT sigismember ();
IMPORT signal ();
IMPORT sigpending ();
IMPORT sigprocmask ();
IMPORT sigqueue ();
IMPORT sigqueueInit ();
IMPORT sigreturn ();
IMPORT sigsetjmp ();
IMPORT sigsetmask ();
IMPORT sigsuspend ();
IMPORT sigtimedwait ();
IMPORT sigvec ();
IMPORT sigwaitinfo ();
IMPORT sin ();
IMPORT sincos ();
IMPORT sincosf ();
IMPORT sinf ();
IMPORT sinh ();
IMPORT sinhf ();
IMPORT sllCount ();
IMPORT SL_LIST *sllCreate ();
IMPORT sllDelete ();
IMPORT SL_NODE *sllEach ();
IMPORT SL_NODE *sllGet ();
IMPORT sllInit ();
IMPORT SL_NODE *sllPrevious ();
IMPORT void sllPutAtHead ();
IMPORT void sllPutAtTail ();
IMPORT void sllRemove ();
IMPORT sllTerminate ();
IMPORT int smAliveTimeout;
IMPORT smAttach ();
IMPORT smCpuInfoGet ();
IMPORT int smCurMaxTries;
IMPORT smDetach ();
IMPORT smIfAttach ();
IMPORT smIfInput ();
IMPORT smIfLoanReturn ();
IMPORT int smIfVerbose;
IMPORT smInfoGet ();
IMPORT smInit ();
IMPORT smIsAlive ();
IMPORT smLockGive ();
IMPORT smLockTake ();
IMPORT int smMemPartAddToPoolRtn;
IMPORT int smMemPartAllocRtn;
IMPORT int smMemPartFindMaxRtn;
IMPORT int smMemPartFreeRtn;
IMPORT int smMemPartOptionsSetRtn;
IMPORT int smMemPartReallocRtn;
IMPORT int smMemPartShowRtn;
IMPORT smNetAttach ();
IMPORT smNetInetGet ();
IMPORT smNetInit ();
IMPORT int smNetLoanNum;
IMPORT int smNetMaxBytesDefault;
IMPORT smNetShow ();
IMPORT smNetShowInit ();
IMPORT int smNetVerbose;
IMPORT int smObjPoolMinusOne;
IMPORT int smObjTaskDeleteFailRtn;
IMPORT int smObjTcbFreeFailRtn;
IMPORT int smObjTcbFreeRtn;
IMPORT smPktAttach ();
IMPORT smPktBeat ();
IMPORT smPktCpuInfoGet ();
IMPORT smPktDetach ();
IMPORT smPktFreeGet ();
IMPORT smPktFreePut ();
IMPORT smPktInfoGet ();
IMPORT smPktInit ();
IMPORT int smPktMaxBytesDefault;
IMPORT int smPktMaxCpusDefault;
IMPORT int smPktMaxInputDefault;
IMPORT int smPktMemSizeDefault;
IMPORT smPktRecv ();
IMPORT smPktSend ();
IMPORT smPktSetup ();
IMPORT int smPktTasTries;
IMPORT smSetup ();
IMPORT smUtilDelay ();
IMPORT smUtilIntConnect ();
IMPORT smUtilIntGen ();
IMPORT smUtilMemProbe ();
IMPORT int smUtilNetRoutine;
IMPORT int smUtilObjRoutine;
IMPORT int smUtilPollTaskOptions;
IMPORT int smUtilPollTaskPriority;
IMPORT int smUtilPollTaskStackSize;
IMPORT smUtilProcNumGet ();
IMPORT smUtilRateGet ();
IMPORT smUtilSoftTas ();
IMPORT smUtilTas ();
IMPORT int smUtilTasChecks;
IMPORT smUtilTasClear ();
IMPORT int smUtilTasClearRtn;
IMPORT int smUtilVerbose;
IMPORT int sm_softc;
IMPORT so ();
IMPORT soabort ();
IMPORT soaccept ();
IMPORT sobind ();
IMPORT socantrcvmore ();
IMPORT socantsendmore ();
IMPORT sockInit ();
IMPORT socket ();
IMPORT soclose ();
IMPORT soconnect ();
IMPORT soconnect2 ();
IMPORT socreate ();
IMPORT sodisconnect ();
IMPORT sofree ();
IMPORT sogetopt ();
IMPORT soisconnected ();
IMPORT soisconnecting ();
IMPORT soisdisconnected ();
IMPORT soisdisconnecting ();
IMPORT solisten ();
IMPORT sonewconn ();
IMPORT soo_ioctl ();
IMPORT soo_select ();
IMPORT soo_unselect ();
IMPORT soqinsque ();
IMPORT soqremque ();
IMPORT soreceive ();
IMPORT soreserve ();
IMPORT sorflush ();
IMPORT sosend ();
IMPORT sosetopt ();
IMPORT soshutdown ();
IMPORT sowakeup ();
IMPORT int sowakeupHook;
IMPORT sp ();
IMPORT int spTaskOptions;
IMPORT int spTaskPriority;
IMPORT int spTaskStackSize;
IMPORT int splSemId;
IMPORT splSemInit ();
IMPORT int splTid;
IMPORT splimp ();
IMPORT splnet ();
IMPORT splx ();
IMPORT sprintf ();
IMPORT spy ();
IMPORT spyClkStart ();
IMPORT spyClkStop ();
IMPORT spyHelp ();
IMPORT spyReport ();
IMPORT spyStop ();
IMPORT spyTask ();
IMPORT int spyTaskId;
IMPORT int spyTaskOptions;
IMPORT int spyTaskPriority;
IMPORT int spyTaskStackSize;
IMPORT sqrt ();
IMPORT sqrtf ();
IMPORT squeeze ();
IMPORT sr ();
IMPORT srand ();
IMPORT sscanf ();
IMPORT int standAloneSymTbl;
IMPORT stat ();
IMPORT int statSymTbl;
IMPORT int statTbl;
IMPORT int statTblSize;
IMPORT statfs ();
IMPORT int stdioFiles;
IMPORT stdioFp ();
IMPORT stdioFpCreate ();
IMPORT stdioFpDestroy ();
IMPORT stdioInit ();
IMPORT stdioShow ();
IMPORT stdioShowInit ();
IMPORT int stdlibFiles;
IMPORT strcat ();
IMPORT strchr ();
IMPORT strcmp ();
IMPORT strcoll ();
IMPORT strcpy ();
IMPORT strcspn ();
IMPORT strerror ();
IMPORT strerror_r ();
IMPORT strftime ();
IMPORT int stringFiles;
IMPORT strlen ();
IMPORT strncat ();
IMPORT strncmp ();
IMPORT strncpy ();
IMPORT strpbrk ();
IMPORT strrchr ();
IMPORT strspn ();
IMPORT strstr ();
IMPORT strtod ();
IMPORT strtok ();
IMPORT strtok_r ();
IMPORT strtol ();
IMPORT strtoul ();
IMPORT strxfrm ();
IMPORT int subnetsarelocal;
IMPORT substrcmp ();
IMPORT swab ();
IMPORT symAdd ();
IMPORT symAlloc ();
IMPORT symEach ();
IMPORT symFindByCName ();
IMPORT symFindByName ();
IMPORT symFindByNameAndType ();
IMPORT symFindByValue ();
IMPORT symFindByValueAndType ();
IMPORT symFindSymbol ();
IMPORT symFree ();
IMPORT int symGroupDefault;
IMPORT symInit ();
IMPORT symLibInit ();
IMPORT int symLkupPgSz;
IMPORT symName ();
IMPORT symRemove ();
IMPORT symShow ();
IMPORT symShowInit ();
IMPORT symTblAdd ();
IMPORT int symTblClassId;
IMPORT symTblCreate ();
IMPORT symTblDelete ();
IMPORT symTblDestroy ();
IMPORT symTblInit ();
IMPORT symTblRemove ();
IMPORT symTblTerminate ();
IMPORT sys596ChanAtn ();
IMPORT sys596Init ();
IMPORT sys596IntAck ();
IMPORT sys596IntDisable ();
IMPORT sys596IntEnable ();
IMPORT sys596Port ();
IMPORT int sysAdaEnable;
IMPORT sysAuxClkConnect ();
IMPORT sysAuxClkDisable ();
IMPORT sysAuxClkEnable ();
IMPORT sysAuxClkRateGet ();
IMPORT sysAuxClkRateSet ();
IMPORT int sysBootFile;
IMPORT int sysBootHost;
IMPORT int sysBootLine;
IMPORT int sysBootParams;
IMPORT sysBspRev ();
IMPORT int sysBus;
IMPORT sysBusIntAck ();
IMPORT sysBusIntGen ();
IMPORT sysBusTas ();
IMPORT sysBusToLocalAdrs ();
IMPORT sysClkConnect ();
IMPORT sysClkDisable ();
IMPORT sysClkEnable ();
IMPORT sysClkRateGet ();
IMPORT sysClkRateSet ();
IMPORT int sysCplusEnable;
IMPORT int sysCpu;
IMPORT sysEnetAddrGet ();
IMPORT int sysExcMsg;
IMPORT int sysFlags;
IMPORT sysHwInit ();
IMPORT sysHwInit2 ();
IMPORT sysInit ();
IMPORT sysIntDisable ();
IMPORT sysIntEnable ();
IMPORT sysLocalToBusAdrs ();
IMPORT sysMailboxConnect ();
IMPORT sysMailboxEnable ();
IMPORT sysMemProbe ();
IMPORT sysMemTop ();
IMPORT sysModel ();
IMPORT sysNvRamGet ();
IMPORT sysNvRamSet ();
IMPORT int sysPhysMemDesc;
IMPORT int sysPhysMemDescNumEnt;
IMPORT sysProcNumGet ();
IMPORT sysProcNumSet ();
IMPORT int sysStartType;
IMPORT int sysSymTbl;
IMPORT sysToMonitor ();
IMPORT system ();
IMPORT tan ();
IMPORT tanf ();
IMPORT tanh ();
IMPORT tanhf ();
IMPORT taskActivate ();
IMPORT taskArgsGet ();
IMPORT taskArgsSet ();
IMPORT int taskBpHook;
IMPORT taskBpHookSet ();
IMPORT int taskClassId;
IMPORT taskCreat ();
IMPORT taskCreateHookAdd ();
IMPORT taskCreateHookDelete ();
IMPORT taskCreateHookShow ();
IMPORT int taskCreateTable;
IMPORT taskDelay ();
IMPORT taskDelete ();
IMPORT taskDeleteForce ();
IMPORT taskDeleteHookAdd ();
IMPORT taskDeleteHookDelete ();
IMPORT taskDeleteHookShow ();
IMPORT int taskDeleteTable;
IMPORT taskDestroy ();
IMPORT taskHookInit ();
IMPORT taskHookShowInit ();
IMPORT int taskIdCurrent;
IMPORT taskIdDefault ();
IMPORT taskIdFigure ();
IMPORT taskIdListGet ();
IMPORT taskIdListSort ();
IMPORT taskIdSelf ();
IMPORT taskIdVerify ();
IMPORT taskInfoGet ();
IMPORT taskInit ();
IMPORT taskIsReady ();
IMPORT taskIsSuspended ();
IMPORT taskLibInit ();
IMPORT taskLock ();
IMPORT taskName ();
IMPORT taskNameToId ();
IMPORT taskOptionsGet ();
IMPORT taskOptionsSet ();
IMPORT taskOptionsString ();
IMPORT int taskPriRangeCheck;
IMPORT taskPriorityGet ();
IMPORT taskPrioritySet ();
IMPORT int taskRegName;
IMPORT int taskRegsFmt;
IMPORT taskRegsGet ();
IMPORT taskRegsInit ();
IMPORT taskRegsSet ();
IMPORT taskRegsShow ();
IMPORT taskRestart ();
IMPORT taskResume ();
IMPORT taskRtnValueSet ();
IMPORT taskSRSet ();
IMPORT taskSafe ();
IMPORT taskShow ();
IMPORT taskShowInit ();
IMPORT taskSpawn ();
IMPORT taskStackAllot ();
IMPORT taskStatusString ();
IMPORT taskSuspend ();
IMPORT taskSwapHookAdd ();
IMPORT taskSwapHookAttach ();
IMPORT taskSwapHookDelete ();
IMPORT taskSwapHookDetach ();
IMPORT taskSwapHookShow ();
IMPORT int taskSwapReference;
IMPORT int taskSwapTable;
IMPORT taskSwitchHookAdd ();
IMPORT taskSwitchHookDelete ();
IMPORT taskSwitchHookShow ();
IMPORT int taskSwitchTable;
IMPORT taskTcb ();
IMPORT taskTerminate ();
IMPORT taskUndelay ();
IMPORT taskUnlock ();
IMPORT taskUnsafe ();
IMPORT taskVarAdd ();
IMPORT taskVarDelete ();
IMPORT taskVarGet ();
IMPORT taskVarInfo ();
IMPORT taskVarInit ();
IMPORT taskVarSet ();
IMPORT tcpDebugShow ();
IMPORT int tcpOutRsts;
IMPORT int tcpPatch;
IMPORT int tcpReportRtn;
IMPORT int tcpTraceRtn;
IMPORT int tcp_alpha;
IMPORT tcp_attach ();
IMPORT int tcp_backoff;
IMPORT int tcp_beta;
IMPORT tcp_canceltimers ();
IMPORT tcp_close ();
IMPORT tcp_ctlinput ();
IMPORT tcp_ctloutput ();
IMPORT tcp_disconnect ();
IMPORT tcp_dooptions ();
IMPORT tcp_drain ();
IMPORT tcp_drop ();
IMPORT tcp_fasttimo ();
IMPORT tcp_init ();
IMPORT int tcp_initopt;
IMPORT tcp_input ();
IMPORT int tcp_iss;
IMPORT int tcp_keepcnt;
IMPORT int tcp_keepidle;
IMPORT int tcp_keepinit;
IMPORT int tcp_keepintvl;
IMPORT int tcp_maxidle;
IMPORT tcp_mss ();
IMPORT tcp_newtcpcb ();
IMPORT tcp_notify ();
IMPORT int tcp_outflags;
IMPORT tcp_output ();
IMPORT tcp_pulloutofband ();
IMPORT tcp_quench ();
IMPORT tcp_reass ();
IMPORT int tcp_recvspace;
IMPORT tcp_respond ();
IMPORT int tcp_saveti;
IMPORT int tcp_sendspace;
IMPORT tcp_setpersist ();
IMPORT tcp_slowtimo ();
IMPORT tcp_template ();
IMPORT tcp_timers ();
IMPORT int tcp_ttl;
IMPORT tcp_usrclosed ();
IMPORT tcp_usrreq ();
IMPORT int tcpcb;
IMPORT int tcpcksum;
IMPORT int tcpprintfs;
IMPORT int tcprexmtthresh;
IMPORT int tcpstat;
IMPORT tcpstatShow ();
IMPORT int tcpstates;
IMPORT td ();
IMPORT telnetInTask ();
IMPORT int telnetInTaskId;
IMPORT telnetInit ();
IMPORT telnetOutTask ();
IMPORT int telnetOutTaskId;
IMPORT int telnetTaskOptions;
IMPORT int telnetTaskPriority;
IMPORT int telnetTaskStackSize;
IMPORT telnetd ();
IMPORT int telnetdId;
IMPORT int telnetdSocket;
IMPORT tftpCopy ();
IMPORT tftpErrorCreate ();
IMPORT tftpGet ();
IMPORT tftpInfoShow ();
IMPORT tftpInit ();
IMPORT tftpModeSet ();
IMPORT tftpPeerSet ();
IMPORT tftpPut ();
IMPORT tftpQuit ();
IMPORT int tftpReXmit;
IMPORT tftpSend ();
IMPORT tftpTask ();
IMPORT int tftpTaskOptions;
IMPORT int tftpTaskPriority;
IMPORT int tftpTaskStackSize;
IMPORT int tftpTimeout;
IMPORT int tftpTrace;
IMPORT int tftpVerbose;
IMPORT tftpXfer ();
IMPORT ti ();
IMPORT tickAnnounce ();
IMPORT tickGet ();
IMPORT int tickQHead;
IMPORT tickSet ();
IMPORT time ();
IMPORT int timeFiles;
IMPORT timex ();
IMPORT timexClear ();
IMPORT timexFunc ();
IMPORT timexHelp ();
IMPORT timexInit ();
IMPORT timexN ();
IMPORT timexPost ();
IMPORT timexPre ();
IMPORT timexShow ();
IMPORT tmpfile ();
IMPORT tmpnam ();
IMPORT tolower ();
IMPORT toupper ();
IMPORT tr ();
IMPORT int trcDefaultArgs;
IMPORT trcStack ();
IMPORT trunc ();
IMPORT truncf ();
IMPORT ts ();
IMPORT tt ();
IMPORT tyAbortFuncSet ();
IMPORT tyAbortSet ();
IMPORT int tyBackspaceChar;
IMPORT tyBackspaceSet ();
IMPORT tyCoDevCreate ();
IMPORT tyCoDrv ();
IMPORT int tyCoDv;
IMPORT tyCoInt ();
IMPORT tyCoIntEx ();
IMPORT tyCoIntRd ();
IMPORT tyCoIntWr ();
IMPORT int tyDeleteLineChar;
IMPORT tyDeleteLineSet ();
IMPORT tyDevInit ();
IMPORT tyEOFSet ();
IMPORT int tyEofChar;
IMPORT tyIRd ();
IMPORT tyITx ();
IMPORT tyIoctl ();
IMPORT tyMonitorTrapSet ();
IMPORT tyRead ();
IMPORT tyWrite ();
IMPORT int udb;
IMPORT int udpNoPorts;
IMPORT udp_ctlinput ();
IMPORT int udp_in;
IMPORT udp_init ();
IMPORT udp_input ();
IMPORT udp_notify ();
IMPORT udp_output ();
IMPORT int udp_recvspace;
IMPORT int udp_sendspace;
IMPORT int udp_ttl;
IMPORT udp_usrreq ();
IMPORT int udpcksum;
IMPORT int udpstat;
IMPORT udpstatShow ();
IMPORT uiomove ();
IMPORT ungetc ();
IMPORT unld ();
IMPORT unldByGroup ();
IMPORT unldByModuleId ();
IMPORT unldByNameAndPath ();
IMPORT unldTextSegmentCheck ();
IMPORT unlink ();
IMPORT int useloopback;
IMPORT usrBootLineCrack ();
IMPORT usrBootLineInit ();
IMPORT usrBpInit ();
IMPORT usrClock ();
IMPORT int usrExtraModules;
IMPORT usrInit ();
IMPORT usrKernelInit ();
IMPORT usrMmuInit ();
IMPORT usrNetIfAttach ();
IMPORT usrNetIfConfig ();
IMPORT usrNetInit ();
IMPORT usrRoot ();
IMPORT usrSlipInit ();
IMPORT usrStartupScript ();
IMPORT uswab ();
IMPORT utime ();
IMPORT valloc ();
IMPORT version ();
IMPORT vfdprintf ();
IMPORT vfprintf ();
IMPORT vmBaseGlobalMapInit ();
IMPORT vmBaseLibInit ();
IMPORT vmBasePageSizeGet ();
IMPORT vmBaseStateSet ();
IMPORT int vmContextClassId;
IMPORT int vmLibInfo;
IMPORT vprintf ();
IMPORT vsprintf ();
IMPORT int vxAbsTicks;
IMPORT int vxIntStackBase;
IMPORT int vxIntStackEnd;
IMPORT vxMemProbe ();
IMPORT vxMemProbeSup ();
IMPORT vxMemProbeTrap ();
IMPORT vxTas ();
IMPORT vxTaskEntry ();
IMPORT int vxTicks;
IMPORT int vxWorksVersion;
IMPORT int vxWorksVersionStr;
IMPORT int vxmIfOps;
IMPORT wakeup ();
IMPORT wcstombs ();
IMPORT wctomb ();
IMPORT wdCancel ();
IMPORT int wdClassId;
IMPORT wdCreate ();
IMPORT wdDelete ();
IMPORT wdDestroy ();
IMPORT wdInit ();
IMPORT wdLibInit ();
IMPORT wdShow ();
IMPORT wdShowInit ();
IMPORT wdStart ();
IMPORT wdTerminate ();
IMPORT wdTick ();
IMPORT whoami ();
IMPORT int wildcard;
IMPORT windDelay ();
IMPORT windDelete ();
IMPORT windExit ();
IMPORT windIntStackSet ();
IMPORT windPendQFlush ();
IMPORT windPendQGet ();
IMPORT windPendQPut ();
IMPORT windPendQRemove ();
IMPORT windPendQTerminate ();
IMPORT windPriNormalSet ();
IMPORT windPrioritySet ();
IMPORT windReadyQPut ();
IMPORT windReadyQRemove ();
IMPORT windResume ();
IMPORT windSemDelete ();
IMPORT windSpawn ();
IMPORT windSuspend ();
IMPORT windTickAnnounce ();
IMPORT windUndelay ();
IMPORT windWdCancel ();
IMPORT windWdStart ();
IMPORT workQAdd0 ();
IMPORT workQAdd1 ();
IMPORT workQAdd2 ();
IMPORT workQDoWork ();
IMPORT workQInit ();
IMPORT int workQIsEmpty;
IMPORT workQPanic ();
IMPORT int workQReadIx;
IMPORT int workQWriteIx;
IMPORT write ();
IMPORT writev ();
IMPORT int wvInstIsOn;
IMPORT int wvObjIsEnabled;
IMPORT int yyact;
IMPORT int yychar;
IMPORT int yychk;
IMPORT int yydebug;
IMPORT int yydef;
IMPORT int yyerrflag;
IMPORT int yyexca;
IMPORT int yylval;
IMPORT int yynerrs;
IMPORT int yypact;
IMPORT yyparse ();
IMPORT int yypgo;
IMPORT int yyr1;
IMPORT int yyr2;
IMPORT yystart ();
IMPORT int yyval;
IMPORT int zeroin_addr;

SYMBOL standTbl [2398] =
    {
    {{NULL},"__Randseed", (char*) &_Randseed, 0, N_EXT | N_DATA},
    {{NULL},"___assert", (char*) __assert, 0, N_EXT | N_TEXT},
    {{NULL},"___clocale", (char*) &__clocale, 0, N_EXT | N_DATA},
    {{NULL},"___costate", (char*) &__costate, 0, N_EXT | N_DATA},
    {{NULL},"___ctype", (char*) &__ctype, 0, N_EXT | N_DATA},
    {{NULL},"___daysSinceEpoch", (char*) __daysSinceEpoch, 0, N_EXT | N_TEXT},
    {{NULL},"___errno", (char*) __errno, 0, N_EXT | N_TEXT},
    {{NULL},"___exp10", (char*) __exp10, 0, N_EXT | N_TEXT},
    {{NULL},"___fixunsdfsi", (char*) __fixunsdfsi, 0, N_EXT | N_TEXT},
    {{NULL},"___fixunssfsi", (char*) __fixunssfsi, 0, N_EXT | N_TEXT},
    {{NULL},"___getDstInfo", (char*) __getDstInfo, 0, N_EXT | N_TEXT},
    {{NULL},"___getTime", (char*) __getTime, 0, N_EXT | N_TEXT},
    {{NULL},"___getZoneInfo", (char*) __getZoneInfo, 0, N_EXT | N_TEXT},
    {{NULL},"___julday", (char*) __julday, 0, N_EXT | N_TEXT},
    {{NULL},"___locale", (char*) &__locale, 0, N_EXT | N_DATA},
    {{NULL},"___loctime", (char*) &__loctime, 0, N_EXT | N_DATA},
    {{NULL},"___sclose", (char*) __sclose, 0, N_EXT | N_TEXT},
    {{NULL},"___sflags", (char*) __sflags, 0, N_EXT | N_TEXT},
    {{NULL},"___sflush", (char*) __sflush, 0, N_EXT | N_TEXT},
    {{NULL},"___sfvwrite", (char*) __sfvwrite, 0, N_EXT | N_TEXT},
    {{NULL},"___smakebuf", (char*) __smakebuf, 0, N_EXT | N_TEXT},
    {{NULL},"___sread", (char*) __sread, 0, N_EXT | N_TEXT},
    {{NULL},"___srefill", (char*) __srefill, 0, N_EXT | N_TEXT},
    {{NULL},"___srget", (char*) __srget, 0, N_EXT | N_TEXT},
    {{NULL},"___sseek", (char*) __sseek, 0, N_EXT | N_TEXT},
    {{NULL},"___stderr", (char*) __stderr, 0, N_EXT | N_TEXT},
    {{NULL},"___stdin", (char*) __stdin, 0, N_EXT | N_TEXT},
    {{NULL},"___stdout", (char*) __stdout, 0, N_EXT | N_TEXT},
    {{NULL},"___strxfrm", (char*) __strxfrm, 0, N_EXT | N_TEXT},
    {{NULL},"___swbuf", (char*) __swbuf, 0, N_EXT | N_TEXT},
    {{NULL},"___swrite", (char*) __swrite, 0, N_EXT | N_TEXT},
    {{NULL},"___swsetup", (char*) __swsetup, 0, N_EXT | N_TEXT},
    {{NULL},"__archHelp_msg", (char*) &_archHelp_msg, 0, N_EXT | N_DATA},
    {{NULL},"__clockRealtime", (char*) &_clockRealtime, 0, N_EXT | N_BSS},
    {{NULL},"__dbgAdrsChkRtn", (char*) &_dbgAdrsChkRtn, 0, N_EXT | N_DATA},
    {{NULL},"__dbgArchInit", (char*) _dbgArchInit, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgDsmInstRtn", (char*) &_dbgDsmInstRtn, 0, N_EXT | N_DATA},
    {{NULL},"__dbgFuncCallCheck", (char*) _dbgFuncCallCheck, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgInfoPCGet", (char*) _dbgInfoPCGet, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgInstPtrAlign", (char*) _dbgInstPtrAlign, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgInstSizeGet", (char*) _dbgInstSizeGet, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgIntrInfoRestore", (char*) _dbgIntrInfoRestore, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgIntrInfoSave", (char*) _dbgIntrInfoSave, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgRegsAdjust", (char*) _dbgRegsAdjust, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgRetAdrsGet", (char*) _dbgRetAdrsGet, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgSStepClear", (char*) _dbgSStepClear, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgSStepSet", (char*) _dbgSStepSet, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgTaskBPModeClear", (char*) _dbgTaskBPModeClear, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgTaskBPModeSet", (char*) _dbgTaskBPModeSet, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgTaskPCGet", (char*) _dbgTaskPCGet, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgTaskPCSet", (char*) _dbgTaskPCSet, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgTaskSStepSet", (char*) _dbgTaskSStepSet, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgTraceDisable", (char*) _dbgTraceDisable, 0, N_EXT | N_TEXT},
    {{NULL},"__dbgVecInit", (char*) _dbgVecInit, 0, N_EXT | N_TEXT},
    {{NULL},"__func_bdall", (char*) &_func_bdall, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtBufferCheck", (char*) &_func_evtBufferCheck, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogM0", (char*) &_func_evtLogM0, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogM1", (char*) &_func_evtLogM1, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogM2", (char*) &_func_evtLogM2, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogM3", (char*) &_func_evtLogM3, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogO", (char*) &_func_evtLogO, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogOIntLock", (char*) &_func_evtLogOIntLock, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogPoint", (char*) &_func_evtLogPoint, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogString", (char*) &_func_evtLogString, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogT0", (char*) &_func_evtLogT0, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogT1", (char*) &_func_evtLogT1, 0, N_EXT | N_BSS},
    {{NULL},"__func_evtLogTSched", (char*) &_func_evtLogTSched, 0, N_EXT | N_BSS},
    {{NULL},"__func_excBaseHook", (char*) &_func_excBaseHook, 0, N_EXT | N_BSS},
    {{NULL},"__func_excInfoShow", (char*) &_func_excInfoShow, 0, N_EXT | N_BSS},
    {{NULL},"__func_excIntHook", (char*) &_func_excIntHook, 0, N_EXT | N_BSS},
    {{NULL},"__func_excJobAdd", (char*) &_func_excJobAdd, 0, N_EXT | N_BSS},
    {{NULL},"__func_excPanicHook", (char*) &_func_excPanicHook, 0, N_EXT | N_BSS},
    {{NULL},"__func_fclose", (char*) &_func_fclose, 0, N_EXT | N_BSS},
    {{NULL},"__func_fppTaskRegsShow", (char*) &_func_fppTaskRegsShow, 0, N_EXT | N_BSS},
    {{NULL},"__func_ftpLs", (char*) &_func_ftpLs, 0, N_EXT | N_BSS},
    {{NULL},"__func_logMsg", (char*) &_func_logMsg, 0, N_EXT | N_BSS},
    {{NULL},"__func_memalign", (char*) &_func_memalign, 0, N_EXT | N_BSS},
    {{NULL},"__func_netLsByName", (char*) &_func_netLsByName, 0, N_EXT | N_BSS},
    {{NULL},"__func_remCurIdGet", (char*) &_func_remCurIdGet, 0, N_EXT | N_BSS},
    {{NULL},"__func_remCurIdSet", (char*) &_func_remCurIdSet, 0, N_EXT | N_BSS},
    {{NULL},"__func_scrPadToBuffer", (char*) &_func_scrPadToBuffer, 0, N_EXT | N_BSS},
    {{NULL},"__func_selTyAdd", (char*) &_func_selTyAdd, 0, N_EXT | N_BSS},
    {{NULL},"__func_selTyDelete", (char*) &_func_selTyDelete, 0, N_EXT | N_BSS},
    {{NULL},"__func_selWakeupAll", (char*) &_func_selWakeupAll, 0, N_EXT | N_BSS},
    {{NULL},"__func_selWakeupListInit", (char*) &_func_selWakeupListInit, 0, N_EXT | N_BSS},
    {{NULL},"__func_sigExcKill", (char*) &_func_sigExcKill, 0, N_EXT | N_BSS},
    {{NULL},"__func_sigTimeoutRecalc", (char*) &_func_sigTimeoutRecalc, 0, N_EXT | N_BSS},
    {{NULL},"__func_sigprocmask", (char*) &_func_sigprocmask, 0, N_EXT | N_BSS},
    {{NULL},"__func_smObjObjShow", (char*) &_func_smObjObjShow, 0, N_EXT | N_BSS},
    {{NULL},"__func_symFindByValueAndType", (char*) &_func_symFindByValueAndType, 0, N_EXT | N_BSS},
    {{NULL},"__func_taskRegsShowRtn", (char*) &_func_taskRegsShowRtn, 0, N_EXT | N_BSS},
    {{NULL},"__func_tmrConnect", (char*) &_func_tmrConnect, 0, N_EXT | N_BSS},
    {{NULL},"__func_tmrDisable", (char*) &_func_tmrDisable, 0, N_EXT | N_BSS},
    {{NULL},"__func_tmrEnable", (char*) &_func_tmrEnable, 0, N_EXT | N_BSS},
    {{NULL},"__func_tmrFreq", (char*) &_func_tmrFreq, 0, N_EXT | N_BSS},
    {{NULL},"__func_tmrPeriod", (char*) &_func_tmrPeriod, 0, N_EXT | N_BSS},
    {{NULL},"__func_tmrStamp", (char*) &_func_tmrStamp, 0, N_EXT | N_BSS},
    {{NULL},"__func_tmrStampLock", (char*) &_func_tmrStampLock, 0, N_EXT | N_BSS},
    {{NULL},"__func_valloc", (char*) &_func_valloc, 0, N_EXT | N_BSS},
    {{NULL},"__insque", (char*) _insque, 0, N_EXT | N_TEXT},
    {{NULL},"__l_PITBL", (char*) &_l_PITBL, 0, N_EXT | N_DATA},
    {{NULL},"__l_denorm", (char*) _l_denorm, 0, N_EXT | N_TEXT},
    {{NULL},"__l_dnrm_lp", (char*) _l_dnrm_lp, 0, N_EXT | N_TEXT},
    {{NULL},"__l_dst_nan", (char*) _l_dst_nan, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fabsd", (char*) _l_fabsd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fabss", (char*) _l_fabss, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fabsx", (char*) _l_fabsx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_facosd", (char*) _l_facosd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_facoss", (char*) _l_facoss, 0, N_EXT | N_TEXT},
    {{NULL},"__l_facosx", (char*) _l_facosx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_faddd", (char*) _l_faddd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fadds", (char*) _l_fadds, 0, N_EXT | N_TEXT},
    {{NULL},"__l_faddx", (char*) _l_faddx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fasind", (char*) _l_fasind, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fasins", (char*) _l_fasins, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fasinx", (char*) _l_fasinx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fatand", (char*) _l_fatand, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fatanhd", (char*) _l_fatanhd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fatanhs", (char*) _l_fatanhs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fatanhx", (char*) _l_fatanhx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fatans", (char*) _l_fatans, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fatanx", (char*) _l_fatanx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fcosd", (char*) _l_fcosd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fcoshd", (char*) _l_fcoshd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fcoshs", (char*) _l_fcoshs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fcoshx", (char*) _l_fcoshx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fcoss", (char*) _l_fcoss, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fcosx", (char*) _l_fcosx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fdivd", (char*) _l_fdivd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fdivs", (char*) _l_fdivs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fdivx", (char*) _l_fdivx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fetoxd", (char*) _l_fetoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fetoxm1d", (char*) _l_fetoxm1d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fetoxm1s", (char*) _l_fetoxm1s, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fetoxm1x", (char*) _l_fetoxm1x, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fetoxs", (char*) _l_fetoxs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fetoxx", (char*) _l_fetoxx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fgetexpd", (char*) _l_fgetexpd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fgetexps", (char*) _l_fgetexps, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fgetexpx", (char*) _l_fgetexpx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fgetmand", (char*) _l_fgetmand, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fgetmans", (char*) _l_fgetmans, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fgetmanx", (char*) _l_fgetmanx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fintd", (char*) _l_fintd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fintrzd", (char*) _l_fintrzd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fintrzs", (char*) _l_fintrzs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fintrzx", (char*) _l_fintrzx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fints", (char*) _l_fints, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fintx", (char*) _l_fintx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flog10d", (char*) _l_flog10d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flog10s", (char*) _l_flog10s, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flog10x", (char*) _l_flog10x, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flog2d", (char*) _l_flog2d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flog2s", (char*) _l_flog2s, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flog2x", (char*) _l_flog2x, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flognd", (char*) _l_flognd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flognp1d", (char*) _l_flognp1d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flognp1s", (char*) _l_flognp1s, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flognp1x", (char*) _l_flognp1x, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flogns", (char*) _l_flogns, 0, N_EXT | N_TEXT},
    {{NULL},"__l_flognx", (char*) _l_flognx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fmodd", (char*) _l_fmodd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fmods", (char*) _l_fmods, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fmodx", (char*) _l_fmodx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fmuld", (char*) _l_fmuld, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fmuls", (char*) _l_fmuls, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fmulx", (char*) _l_fmulx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fnegd", (char*) _l_fnegd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fnegs", (char*) _l_fnegs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fnegx", (char*) _l_fnegx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fremd", (char*) _l_fremd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_frems", (char*) _l_frems, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fremx", (char*) _l_fremx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fscaled", (char*) _l_fscaled, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fscales", (char*) _l_fscales, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fscalex", (char*) _l_fscalex, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsind", (char*) _l_fsind, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsinhd", (char*) _l_fsinhd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsinhs", (char*) _l_fsinhs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsinhx", (char*) _l_fsinhx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsins", (char*) _l_fsins, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsinx", (char*) _l_fsinx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsqrtd", (char*) _l_fsqrtd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsqrts", (char*) _l_fsqrts, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsqrtx", (char*) _l_fsqrtx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsubd", (char*) _l_fsubd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsubs", (char*) _l_fsubs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_fsubx", (char*) _l_fsubx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftand", (char*) _l_ftand, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftanhd", (char*) _l_ftanhd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftanhs", (char*) _l_ftanhs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftanhx", (char*) _l_ftanhx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftans", (char*) _l_ftans, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftanx", (char*) _l_ftanx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftentoxd", (char*) _l_ftentoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftentoxs", (char*) _l_ftentoxs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftentoxx", (char*) _l_ftentoxx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftwotoxd", (char*) _l_ftwotoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftwotoxs", (char*) _l_ftwotoxs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ftwotoxx", (char*) _l_ftwotoxx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_l_sint", (char*) _l_l_sint, 0, N_EXT | N_TEXT},
    {{NULL},"__l_l_sintd", (char*) _l_l_sintd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_l_sintrz", (char*) _l_l_sintrz, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ld_minf", (char*) _l_ld_minf, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ld_mone", (char*) _l_ld_mone, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ld_mpi2", (char*) _l_ld_mpi2, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ld_mzero", (char*) _l_ld_mzero, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ld_pinf", (char*) _l_ld_pinf, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ld_pone", (char*) _l_ld_pone, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ld_ppi2", (char*) _l_ld_ppi2, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ld_pzero", (char*) _l_ld_pzero, 0, N_EXT | N_TEXT},
    {{NULL},"__l_mon_nan", (char*) _l_mon_nan, 0, N_EXT | N_TEXT},
    {{NULL},"__l_nrm_set", (char*) _l_nrm_set, 0, N_EXT | N_TEXT},
    {{NULL},"__l_nrm_zero", (char*) _l_nrm_zero, 0, N_EXT | N_TEXT},
    {{NULL},"__l_pmod", (char*) _l_pmod, 0, N_EXT | N_TEXT},
    {{NULL},"__l_prem", (char*) _l_prem, 0, N_EXT | N_TEXT},
    {{NULL},"__l_pscale", (char*) _l_pscale, 0, N_EXT | N_TEXT},
    {{NULL},"__l_round", (char*) _l_round, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sabs", (char*) _l_sabs, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sacos", (char*) _l_sacos, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sacosd", (char*) _l_sacosd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sadd", (char*) _l_sadd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sasin", (char*) _l_sasin, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sasind", (char*) _l_sasind, 0, N_EXT | N_TEXT},
    {{NULL},"__l_satan", (char*) _l_satan, 0, N_EXT | N_TEXT},
    {{NULL},"__l_satand", (char*) _l_satand, 0, N_EXT | N_TEXT},
    {{NULL},"__l_satanh", (char*) _l_satanh, 0, N_EXT | N_TEXT},
    {{NULL},"__l_satanhd", (char*) _l_satanhd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_scos", (char*) _l_scos, 0, N_EXT | N_TEXT},
    {{NULL},"__l_scosd", (char*) _l_scosd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_scosh", (char*) _l_scosh, 0, N_EXT | N_TEXT},
    {{NULL},"__l_scoshd", (char*) _l_scoshd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sdiv", (char*) _l_sdiv, 0, N_EXT | N_TEXT},
    {{NULL},"__l_setox", (char*) _l_setox, 0, N_EXT | N_TEXT},
    {{NULL},"__l_setoxd", (char*) _l_setoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_setoxm1", (char*) _l_setoxm1, 0, N_EXT | N_TEXT},
    {{NULL},"__l_setoxm1d", (char*) _l_setoxm1d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_setoxm1i", (char*) _l_setoxm1i, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sgetexp", (char*) _l_sgetexp, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sgetexpd", (char*) _l_sgetexpd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sgetman", (char*) _l_sgetman, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sgetmand", (char*) _l_sgetmand, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sinf", (char*) _l_sinf, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sint", (char*) _l_sint, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sintd", (char*) _l_sintd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sintdo", (char*) _l_sintdo, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sintrz", (char*) _l_sintrz, 0, N_EXT | N_TEXT},
    {{NULL},"__l_slog10", (char*) _l_slog10, 0, N_EXT | N_TEXT},
    {{NULL},"__l_slog10d", (char*) _l_slog10d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_slog2", (char*) _l_slog2, 0, N_EXT | N_TEXT},
    {{NULL},"__l_slog2d", (char*) _l_slog2d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_slogn", (char*) _l_slogn, 0, N_EXT | N_TEXT},
    {{NULL},"__l_slognd", (char*) _l_slognd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_slognp1", (char*) _l_slognp1, 0, N_EXT | N_TEXT},
    {{NULL},"__l_slognp1d", (char*) _l_slognp1d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_smod", (char*) _l_smod, 0, N_EXT | N_TEXT},
    {{NULL},"__l_smul", (char*) _l_smul, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sneg", (char*) _l_sneg, 0, N_EXT | N_TEXT},
    {{NULL},"__l_snzrinx", (char*) _l_snzrinx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sone", (char*) _l_sone, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sopr_inf", (char*) _l_sopr_inf, 0, N_EXT | N_TEXT},
    {{NULL},"__l_spi_2", (char*) _l_spi_2, 0, N_EXT | N_TEXT},
    {{NULL},"__l_src_nan", (char*) _l_src_nan, 0, N_EXT | N_TEXT},
    {{NULL},"__l_srem", (char*) _l_srem, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sscale", (char*) _l_sscale, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssin", (char*) _l_ssin, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssincos", (char*) _l_ssincos, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssincosd", (char*) _l_ssincosd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssincosi", (char*) _l_ssincosi, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssincosnan", (char*) _l_ssincosnan, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssincosz", (char*) _l_ssincosz, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssind", (char*) _l_ssind, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssinh", (char*) _l_ssinh, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssinhd", (char*) _l_ssinhd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sslog10", (char*) _l_sslog10, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sslog10d", (char*) _l_sslog10d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sslog2", (char*) _l_sslog2, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sslog2d", (char*) _l_sslog2d, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sslogn", (char*) _l_sslogn, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sslognd", (char*) _l_sslognd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sslognp1", (char*) _l_sslognp1, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssqrt", (char*) _l_ssqrt, 0, N_EXT | N_TEXT},
    {{NULL},"__l_ssub", (char*) _l_ssub, 0, N_EXT | N_TEXT},
    {{NULL},"__l_stan", (char*) _l_stan, 0, N_EXT | N_TEXT},
    {{NULL},"__l_stand", (char*) _l_stand, 0, N_EXT | N_TEXT},
    {{NULL},"__l_stanh", (char*) _l_stanh, 0, N_EXT | N_TEXT},
    {{NULL},"__l_stanhd", (char*) _l_stanhd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_stentox", (char*) _l_stentox, 0, N_EXT | N_TEXT},
    {{NULL},"__l_stentoxd", (char*) _l_stentoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_sto_cos", (char*) _l_sto_cos, 0, N_EXT | N_TEXT},
    {{NULL},"__l_stwotox", (char*) _l_stwotox, 0, N_EXT | N_TEXT},
    {{NULL},"__l_stwotoxd", (char*) _l_stwotoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__l_szero", (char*) _l_szero, 0, N_EXT | N_TEXT},
    {{NULL},"__l_szr_inf", (char*) _l_szr_inf, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_avoid_unsupp", (char*) _l_t_avoid_unsupp, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_dz", (char*) _l_t_dz, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_dz2", (char*) _l_t_dz2, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_extdnrm", (char*) _l_t_extdnrm, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_frcinx", (char*) _l_t_frcinx, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_inx2", (char*) _l_t_inx2, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_operr", (char*) _l_t_operr, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_ovfl", (char*) _l_t_ovfl, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_ovfl2", (char*) _l_t_ovfl2, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_resdnrm", (char*) _l_t_resdnrm, 0, N_EXT | N_TEXT},
    {{NULL},"__l_t_unfl", (char*) _l_t_unfl, 0, N_EXT | N_TEXT},
    {{NULL},"__l_tag", (char*) _l_tag, 0, N_EXT | N_TEXT},
    {{NULL},"__pSigQueueFreeHead", (char*) &_pSigQueueFreeHead, 0, N_EXT | N_BSS},
    {{NULL},"__procNumWasSet", (char*) &_procNumWasSet, 0, N_EXT | N_DATA},
    {{NULL},"__remque", (char*) _remque, 0, N_EXT | N_TEXT},
    {{NULL},"__setjmpSetup", (char*) _setjmpSetup, 0, N_EXT | N_TEXT},
    {{NULL},"__sigCtxLoad", (char*) _sigCtxLoad, 0, N_EXT | N_TEXT},
    {{NULL},"__sigCtxRtnValSet", (char*) _sigCtxRtnValSet, 0, N_EXT | N_TEXT},
    {{NULL},"__sigCtxSave", (char*) _sigCtxSave, 0, N_EXT | N_TEXT},
    {{NULL},"__sigCtxSetup", (char*) _sigCtxSetup, 0, N_EXT | N_TEXT},
    {{NULL},"__sigCtxStackEnd", (char*) _sigCtxStackEnd, 0, N_EXT | N_TEXT},
    {{NULL},"__sigfaulttable", (char*) &_sigfaulttable, 0, N_EXT | N_DATA},
    {{NULL},"__x_BIGRN", (char*) &_x_BIGRN, 0, N_EXT | N_DATA},
    {{NULL},"__x_BIGRP", (char*) &_x_BIGRP, 0, N_EXT | N_DATA},
    {{NULL},"__x_BIGRZRM", (char*) &_x_BIGRZRM, 0, N_EXT | N_DATA},
    {{NULL},"__x_PIRN", (char*) &_x_PIRN, 0, N_EXT | N_DATA},
    {{NULL},"__x_PIRP", (char*) &_x_PIRP, 0, N_EXT | N_DATA},
    {{NULL},"__x_PIRZRM", (char*) &_x_PIRZRM, 0, N_EXT | N_DATA},
    {{NULL},"__x_PITBL", (char*) &_x_PITBL, 0, N_EXT | N_DATA},
    {{NULL},"__x_PTENRM", (char*) &_x_PTENRM, 0, N_EXT | N_DATA},
    {{NULL},"__x_PTENRN", (char*) &_x_PTENRN, 0, N_EXT | N_DATA},
    {{NULL},"__x_PTENRP", (char*) &_x_PTENRP, 0, N_EXT | N_DATA},
    {{NULL},"__x_SMALRN", (char*) &_x_SMALRN, 0, N_EXT | N_DATA},
    {{NULL},"__x_SMALRP", (char*) &_x_SMALRP, 0, N_EXT | N_DATA},
    {{NULL},"__x_SMALRZRM", (char*) &_x_SMALRZRM, 0, N_EXT | N_DATA},
    {{NULL},"__x_ap_st_n", (char*) _x_ap_st_n, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ap_st_z", (char*) _x_ap_st_z, 0, N_EXT | N_TEXT},
    {{NULL},"__x_b1238_fix", (char*) _x_b1238_fix, 0, N_EXT | N_TEXT},
    {{NULL},"__x_bindec", (char*) _x_bindec, 0, N_EXT | N_TEXT},
    {{NULL},"__x_binstr", (char*) _x_binstr, 0, N_EXT | N_TEXT},
    {{NULL},"__x_calc_e", (char*) _x_calc_e, 0, N_EXT | N_TEXT},
    {{NULL},"__x_calc_m", (char*) _x_calc_m, 0, N_EXT | N_TEXT},
    {{NULL},"__x_decbin", (char*) _x_decbin, 0, N_EXT | N_TEXT},
    {{NULL},"__x_denorm", (char*) _x_denorm, 0, N_EXT | N_TEXT},
    {{NULL},"__x_dest_dbl", (char*) _x_dest_dbl, 0, N_EXT | N_TEXT},
    {{NULL},"__x_dest_ext", (char*) _x_dest_ext, 0, N_EXT | N_TEXT},
    {{NULL},"__x_dest_sgl", (char*) _x_dest_sgl, 0, N_EXT | N_TEXT},
    {{NULL},"__x_dnrm_lp", (char*) _x_dnrm_lp, 0, N_EXT | N_TEXT},
    {{NULL},"__x_do_func", (char*) _x_do_func, 0, N_EXT | N_TEXT},
    {{NULL},"__x_dst_nan", (char*) _x_dst_nan, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_bsun", (char*) _x_fpsp_bsun, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_done", (char*) _x_fpsp_done, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_dz", (char*) _x_fpsp_dz, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_fline", (char*) _x_fpsp_fline, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_fmt_error", (char*) _x_fpsp_fmt_error, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_ill_inst", (char*) _x_fpsp_ill_inst, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_inex", (char*) _x_fpsp_inex, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_operr", (char*) _x_fpsp_operr, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_ovfl", (char*) _x_fpsp_ovfl, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_snan", (char*) _x_fpsp_snan, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_unfl", (char*) _x_fpsp_unfl, 0, N_EXT | N_TEXT},
    {{NULL},"__x_fpsp_unsupp", (char*) _x_fpsp_unsupp, 0, N_EXT | N_TEXT},
    {{NULL},"__x_g_dfmtou", (char*) _x_g_dfmtou, 0, N_EXT | N_TEXT},
    {{NULL},"__x_g_opcls", (char*) _x_g_opcls, 0, N_EXT | N_TEXT},
    {{NULL},"__x_g_rndpr", (char*) _x_g_rndpr, 0, N_EXT | N_TEXT},
    {{NULL},"__x_gen_except", (char*) _x_gen_except, 0, N_EXT | N_TEXT},
    {{NULL},"__x_get_fline", (char*) _x_get_fline, 0, N_EXT | N_TEXT},
    {{NULL},"__x_get_op", (char*) _x_get_op, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ld_minf", (char*) _x_ld_minf, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ld_mone", (char*) _x_ld_mone, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ld_mpi2", (char*) _x_ld_mpi2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ld_mzero", (char*) _x_ld_mzero, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ld_pinf", (char*) _x_ld_pinf, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ld_pone", (char*) _x_ld_pone, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ld_ppi2", (char*) _x_ld_ppi2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ld_pzero", (char*) _x_ld_pzero, 0, N_EXT | N_TEXT},
    {{NULL},"__x_mem_read", (char*) _x_mem_read, 0, N_EXT | N_TEXT},
    {{NULL},"__x_mem_write", (char*) _x_mem_write, 0, N_EXT | N_TEXT},
    {{NULL},"__x_norm", (char*) _x_norm, 0, N_EXT | N_TEXT},
    {{NULL},"__x_nrm_set", (char*) _x_nrm_set, 0, N_EXT | N_TEXT},
    {{NULL},"__x_nrm_zero", (char*) _x_nrm_zero, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ovf_r_k", (char*) _x_ovf_r_k, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ovf_r_x2", (char*) _x_ovf_r_x2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ovf_r_x3", (char*) _x_ovf_r_x3, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ovf_res", (char*) _x_ovf_res, 0, N_EXT | N_TEXT},
    {{NULL},"__x_p_move", (char*) _x_p_move, 0, N_EXT | N_TEXT},
    {{NULL},"__x_pmod", (char*) _x_pmod, 0, N_EXT | N_TEXT},
    {{NULL},"__x_prem", (char*) _x_prem, 0, N_EXT | N_TEXT},
    {{NULL},"__x_pscale", (char*) _x_pscale, 0, N_EXT | N_TEXT},
    {{NULL},"__x_pwrten", (char*) _x_pwrten, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_bsun", (char*) _x_real_bsun, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_dz", (char*) _x_real_dz, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_inex", (char*) _x_real_inex, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_operr", (char*) _x_real_operr, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_ovfl", (char*) _x_real_ovfl, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_snan", (char*) _x_real_snan, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_trace", (char*) _x_real_trace, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_unfl", (char*) _x_real_unfl, 0, N_EXT | N_TEXT},
    {{NULL},"__x_real_unsupp", (char*) _x_real_unsupp, 0, N_EXT | N_TEXT},
    {{NULL},"__x_reg_dest", (char*) _x_reg_dest, 0, N_EXT | N_TEXT},
    {{NULL},"__x_res_func", (char*) _x_res_func, 0, N_EXT | N_TEXT},
    {{NULL},"__x_round", (char*) _x_round, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sacos", (char*) _x_sacos, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sacosd", (char*) _x_sacosd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sasin", (char*) _x_sasin, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sasind", (char*) _x_sasind, 0, N_EXT | N_TEXT},
    {{NULL},"__x_satan", (char*) _x_satan, 0, N_EXT | N_TEXT},
    {{NULL},"__x_satand", (char*) _x_satand, 0, N_EXT | N_TEXT},
    {{NULL},"__x_satanh", (char*) _x_satanh, 0, N_EXT | N_TEXT},
    {{NULL},"__x_satanhd", (char*) _x_satanhd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sc_mul", (char*) _x_sc_mul, 0, N_EXT | N_TEXT},
    {{NULL},"__x_scos", (char*) _x_scos, 0, N_EXT | N_TEXT},
    {{NULL},"__x_scosd", (char*) _x_scosd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_scosh", (char*) _x_scosh, 0, N_EXT | N_TEXT},
    {{NULL},"__x_scoshd", (char*) _x_scoshd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_serror", (char*) _x_serror, 0, N_EXT | N_TEXT},
    {{NULL},"__x_setox", (char*) _x_setox, 0, N_EXT | N_TEXT},
    {{NULL},"__x_setoxd", (char*) _x_setoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_setoxm1", (char*) _x_setoxm1, 0, N_EXT | N_TEXT},
    {{NULL},"__x_setoxm1d", (char*) _x_setoxm1d, 0, N_EXT | N_TEXT},
    {{NULL},"__x_setoxm1i", (char*) _x_setoxm1i, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sgetexp", (char*) _x_sgetexp, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sgetexpd", (char*) _x_sgetexpd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sgetman", (char*) _x_sgetman, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sgetmand", (char*) _x_sgetmand, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sinf", (char*) _x_sinf, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sint", (char*) _x_sint, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sintd", (char*) _x_sintd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sintdo", (char*) _x_sintdo, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sintrz", (char*) _x_sintrz, 0, N_EXT | N_TEXT},
    {{NULL},"__x_slog10", (char*) _x_slog10, 0, N_EXT | N_TEXT},
    {{NULL},"__x_slog10d", (char*) _x_slog10d, 0, N_EXT | N_TEXT},
    {{NULL},"__x_slog2", (char*) _x_slog2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_slog2d", (char*) _x_slog2d, 0, N_EXT | N_TEXT},
    {{NULL},"__x_slogn", (char*) _x_slogn, 0, N_EXT | N_TEXT},
    {{NULL},"__x_slognd", (char*) _x_slognd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_slognp1", (char*) _x_slognp1, 0, N_EXT | N_TEXT},
    {{NULL},"__x_slognp1d", (char*) _x_slognp1d, 0, N_EXT | N_TEXT},
    {{NULL},"__x_smod", (char*) _x_smod, 0, N_EXT | N_TEXT},
    {{NULL},"__x_smovcr", (char*) _x_smovcr, 0, N_EXT | N_TEXT},
    {{NULL},"__x_snzrinx", (char*) _x_snzrinx, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sone", (char*) _x_sone, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sopr_inf", (char*) _x_sopr_inf, 0, N_EXT | N_TEXT},
    {{NULL},"__x_spi_2", (char*) _x_spi_2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_src_nan", (char*) _x_src_nan, 0, N_EXT | N_TEXT},
    {{NULL},"__x_srem", (char*) _x_srem, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sscale", (char*) _x_sscale, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssin", (char*) _x_ssin, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssincos", (char*) _x_ssincos, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssincosd", (char*) _x_ssincosd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssincosi", (char*) _x_ssincosi, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssincosnan", (char*) _x_ssincosnan, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssincosz", (char*) _x_ssincosz, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssind", (char*) _x_ssind, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssinh", (char*) _x_ssinh, 0, N_EXT | N_TEXT},
    {{NULL},"__x_ssinhd", (char*) _x_ssinhd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sslog10", (char*) _x_sslog10, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sslog10d", (char*) _x_sslog10d, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sslog2", (char*) _x_sslog2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sslog2d", (char*) _x_sslog2d, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sslogn", (char*) _x_sslogn, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sslognd", (char*) _x_sslognd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sslognp1", (char*) _x_sslognp1, 0, N_EXT | N_TEXT},
    {{NULL},"__x_stan", (char*) _x_stan, 0, N_EXT | N_TEXT},
    {{NULL},"__x_stand", (char*) _x_stand, 0, N_EXT | N_TEXT},
    {{NULL},"__x_stanh", (char*) _x_stanh, 0, N_EXT | N_TEXT},
    {{NULL},"__x_stanhd", (char*) _x_stanhd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_stentox", (char*) _x_stentox, 0, N_EXT | N_TEXT},
    {{NULL},"__x_stentoxd", (char*) _x_stentoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sto_cos", (char*) _x_sto_cos, 0, N_EXT | N_TEXT},
    {{NULL},"__x_sto_res", (char*) _x_sto_res, 0, N_EXT | N_TEXT},
    {{NULL},"__x_store", (char*) _x_store, 0, N_EXT | N_TEXT},
    {{NULL},"__x_stwotox", (char*) _x_stwotox, 0, N_EXT | N_TEXT},
    {{NULL},"__x_stwotoxd", (char*) _x_stwotoxd, 0, N_EXT | N_TEXT},
    {{NULL},"__x_szero", (char*) _x_szero, 0, N_EXT | N_TEXT},
    {{NULL},"__x_szr_inf", (char*) _x_szr_inf, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_avoid_unsupp", (char*) _x_t_avoid_unsupp, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_dz", (char*) _x_t_dz, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_dz2", (char*) _x_t_dz2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_extdnrm", (char*) _x_t_extdnrm, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_frcinx", (char*) _x_t_frcinx, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_inx2", (char*) _x_t_inx2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_operr", (char*) _x_t_operr, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_ovfl", (char*) _x_t_ovfl, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_ovfl2", (char*) _x_t_ovfl2, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_resdnrm", (char*) _x_t_resdnrm, 0, N_EXT | N_TEXT},
    {{NULL},"__x_t_unfl", (char*) _x_t_unfl, 0, N_EXT | N_TEXT},
    {{NULL},"__x_tblpre", (char*) &_x_tblpre, 0, N_EXT | N_DATA},
    {{NULL},"__x_unf_sub", (char*) _x_unf_sub, 0, N_EXT | N_TEXT},
    {{NULL},"__x_uni_getop", (char*) _x_uni_getop, 0, N_EXT | N_TEXT},
    {{NULL},"__x_uns_getop", (char*) _x_uns_getop, 0, N_EXT | N_TEXT},
    {{NULL},"_a0", (char*) a0, 0, N_EXT | N_TEXT},
    {{NULL},"_a1", (char*) a1, 0, N_EXT | N_TEXT},
    {{NULL},"_a2", (char*) a2, 0, N_EXT | N_TEXT},
    {{NULL},"_a3", (char*) a3, 0, N_EXT | N_TEXT},
    {{NULL},"_a4", (char*) a4, 0, N_EXT | N_TEXT},
    {{NULL},"_a5", (char*) a5, 0, N_EXT | N_TEXT},
    {{NULL},"_a6", (char*) a6, 0, N_EXT | N_TEXT},
    {{NULL},"_a7", (char*) a7, 0, N_EXT | N_TEXT},
    {{NULL},"_abort", (char*) abort, 0, N_EXT | N_TEXT},
    {{NULL},"_abs", (char*) abs, 0, N_EXT | N_TEXT},
    {{NULL},"_accept", (char*) accept, 0, N_EXT | N_TEXT},
    {{NULL},"_acos", (char*) acos, 0, N_EXT | N_TEXT},
    {{NULL},"_acosf", (char*) acosf, 0, N_EXT | N_TEXT},
    {{NULL},"_activeQHead", (char*) &activeQHead, 0, N_EXT | N_BSS},
    {{NULL},"_addSegNames", (char*) addSegNames, 0, N_EXT | N_TEXT},
    {{NULL},"_afswitch", (char*) &afswitch, 0, N_EXT | N_DATA},
    {{NULL},"_arpAdd", (char*) arpAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_arpCmd", (char*) arpCmd, 0, N_EXT | N_TEXT},
    {{NULL},"_arpDelete", (char*) arpDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_arpFlush", (char*) arpFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_arpShow", (char*) arpShow, 0, N_EXT | N_TEXT},
    {{NULL},"_arpinput", (char*) arpinput, 0, N_EXT | N_TEXT},
    {{NULL},"_arpioctl", (char*) arpioctl, 0, N_EXT | N_TEXT},
    {{NULL},"_arpresolve", (char*) arpresolve, 0, N_EXT | N_TEXT},
    {{NULL},"_arptab", (char*) &arptab, 0, N_EXT | N_BSS},
    {{NULL},"_arptabShow", (char*) arptabShow, 0, N_EXT | N_TEXT},
    {{NULL},"_arptab_size", (char*) &arptab_size, 0, N_EXT | N_DATA},
    {{NULL},"_arptfree", (char*) arptfree, 0, N_EXT | N_TEXT},
    {{NULL},"_arptimer", (char*) arptimer, 0, N_EXT | N_TEXT},
    {{NULL},"_arptnew", (char*) arptnew, 0, N_EXT | N_TEXT},
    {{NULL},"_arpwhohas", (char*) arpwhohas, 0, N_EXT | N_TEXT},
    {{NULL},"_asctime", (char*) asctime, 0, N_EXT | N_TEXT},
    {{NULL},"_asctime_r", (char*) asctime_r, 0, N_EXT | N_TEXT},
    {{NULL},"_asin", (char*) asin, 0, N_EXT | N_TEXT},
    {{NULL},"_asinf", (char*) asinf, 0, N_EXT | N_TEXT},
    {{NULL},"_assertFiles", (char*) &assertFiles, 0, N_EXT | N_DATA},
    {{NULL},"_atan", (char*) atan, 0, N_EXT | N_TEXT},
    {{NULL},"_atan2", (char*) atan2, 0, N_EXT | N_TEXT},
    {{NULL},"_atan2f", (char*) atan2f, 0, N_EXT | N_TEXT},
    {{NULL},"_atanf", (char*) atanf, 0, N_EXT | N_TEXT},
    {{NULL},"_atexit", (char*) atexit, 0, N_EXT | N_TEXT},
    {{NULL},"_atof", (char*) atof, 0, N_EXT | N_TEXT},
    {{NULL},"_atoi", (char*) atoi, 0, N_EXT | N_TEXT},
    {{NULL},"_atol", (char*) atol, 0, N_EXT | N_TEXT},
    {{NULL},"_b", (char*) b, 0, N_EXT | N_TEXT},
    {{NULL},"_bcmp", (char*) bcmp, 0, N_EXT | N_TEXT},
    {{NULL},"_bcopy", (char*) bcopy, 0, N_EXT | N_TEXT},
    {{NULL},"_bcopyBytes", (char*) bcopyBytes, 0, N_EXT | N_TEXT},
    {{NULL},"_bcopyLongs", (char*) bcopyLongs, 0, N_EXT | N_TEXT},
    {{NULL},"_bcopyWords", (char*) bcopyWords, 0, N_EXT | N_TEXT},
    {{NULL},"_bcopy_to_mbufs", (char*) bcopy_to_mbufs, 0, N_EXT | N_TEXT},
    {{NULL},"_bd", (char*) bd, 0, N_EXT | N_TEXT},
    {{NULL},"_bdTask", (char*) bdTask, 0, N_EXT | N_TEXT},
    {{NULL},"_bdall", (char*) bdall, 0, N_EXT | N_TEXT},
    {{NULL},"_bfill", (char*) bfill, 0, N_EXT | N_TEXT},
    {{NULL},"_bfillBytes", (char*) bfillBytes, 0, N_EXT | N_TEXT},
    {{NULL},"_bind", (char*) bind, 0, N_EXT | N_TEXT},
    {{NULL},"_bindresvport", (char*) bindresvport, 0, N_EXT | N_TEXT},
    {{NULL},"_binvert", (char*) binvert, 0, N_EXT | N_TEXT},
    {{NULL},"_bootBpAnchorExtract", (char*) bootBpAnchorExtract, 0, N_EXT | N_TEXT},
    {{NULL},"_bootChange", (char*) bootChange, 0, N_EXT | N_TEXT},
    {{NULL},"_bootNetmaskExtract", (char*) bootNetmaskExtract, 0, N_EXT | N_TEXT},
    {{NULL},"_bootParamsErrorPrint", (char*) bootParamsErrorPrint, 0, N_EXT | N_TEXT},
    {{NULL},"_bootParamsPrompt", (char*) bootParamsPrompt, 0, N_EXT | N_TEXT},
    {{NULL},"_bootParamsShow", (char*) bootParamsShow, 0, N_EXT | N_TEXT},
    {{NULL},"_bootScanNum", (char*) bootScanNum, 0, N_EXT | N_TEXT},
    {{NULL},"_bootStringToStruct", (char*) bootStringToStruct, 0, N_EXT | N_TEXT},
    {{NULL},"_bootStructToString", (char*) bootStructToString, 0, N_EXT | N_TEXT},
    {{NULL},"_bsearch", (char*) bsearch, 0, N_EXT | N_TEXT},
    {{NULL},"_bspVersion", (char*) bspVersion, 0, N_EXT | N_TEXT},
    {{NULL},"_bswap", (char*) bswap, 0, N_EXT | N_TEXT},
    {{NULL},"_bufferAddress", (char*) &bufferAddress, 0, N_EXT | N_BSS},
    {{NULL},"_build_cluster", (char*) build_cluster, 0, N_EXT | N_TEXT},
    {{NULL},"_bzero", (char*) bzero, 0, N_EXT | N_TEXT},
    {{NULL},"_c", (char*) c, 0, N_EXT | N_TEXT},
    {{NULL},"_cache040DataDisable", (char*) cache040DataDisable, 0, N_EXT | N_TEXT},
    {{NULL},"_cache040WriteBufferFlush", (char*) cache040WriteBufferFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchClear", (char*) cacheArchClear, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchClearEntry", (char*) cacheArchClearEntry, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchDisable", (char*) cacheArchDisable, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchDmaFree", (char*) cacheArchDmaFree, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchDmaMalloc", (char*) cacheArchDmaMalloc, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchEnable", (char*) cacheArchEnable, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchInvalidate", (char*) cacheArchInvalidate, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchLibInit", (char*) cacheArchLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchLock", (char*) cacheArchLock, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchTextUpdate", (char*) cacheArchTextUpdate, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheArchUnlock", (char*) cacheArchUnlock, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheCACRGet", (char*) cacheCACRGet, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheCACRSet", (char*) cacheCACRSet, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheCINV", (char*) cacheCINV, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheCPUSH", (char*) cacheCPUSH, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheClear", (char*) cacheClear, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDTTR0Get", (char*) cacheDTTR0Get, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDTTR0ModeSet", (char*) cacheDTTR0ModeSet, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDTTR0Set", (char*) cacheDTTR0Set, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDataEnabled", (char*) &cacheDataEnabled, 0, N_EXT | N_DATA},
    {{NULL},"_cacheDataMode", (char*) &cacheDataMode, 0, N_EXT | N_DATA},
    {{NULL},"_cacheDisable", (char*) cacheDisable, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDmaFree", (char*) cacheDmaFree, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDmaFreeRtn", (char*) &cacheDmaFreeRtn, 0, N_EXT | N_DATA},
    {{NULL},"_cacheDmaFuncs", (char*) &cacheDmaFuncs, 0, N_EXT | N_DATA},
    {{NULL},"_cacheDmaMalloc", (char*) cacheDmaMalloc, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDmaMallocRtn", (char*) &cacheDmaMallocRtn, 0, N_EXT | N_DATA},
    {{NULL},"_cacheDrvFlush", (char*) cacheDrvFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDrvInvalidate", (char*) cacheDrvInvalidate, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDrvPhysToVirt", (char*) cacheDrvPhysToVirt, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheDrvVirtToPhys", (char*) cacheDrvVirtToPhys, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheEnable", (char*) cacheEnable, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheFlush", (char*) cacheFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheFuncsSet", (char*) cacheFuncsSet, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheInvalidate", (char*) cacheInvalidate, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheLib", (char*) &cacheLib, 0, N_EXT | N_DATA},
    {{NULL},"_cacheLibInit", (char*) cacheLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheLock", (char*) cacheLock, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheMmuAvailable", (char*) &cacheMmuAvailable, 0, N_EXT | N_DATA},
    {{NULL},"_cacheNullFuncs", (char*) &cacheNullFuncs, 0, N_EXT | N_DATA},
    {{NULL},"_cachePipeFlush", (char*) cachePipeFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheTextUpdate", (char*) cacheTextUpdate, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheUnlock", (char*) cacheUnlock, 0, N_EXT | N_TEXT},
    {{NULL},"_cacheUserFuncs", (char*) &cacheUserFuncs, 0, N_EXT | N_DATA},
    {{NULL},"_calloc", (char*) calloc, 0, N_EXT | N_TEXT},
    {{NULL},"_cbrt", (char*) cbrt, 0, N_EXT | N_TEXT},
    {{NULL},"_cbrtf", (char*) cbrtf, 0, N_EXT | N_TEXT},
    {{NULL},"_cd", (char*) cd, 0, N_EXT | N_TEXT},
    {{NULL},"_ceil", (char*) ceil, 0, N_EXT | N_TEXT},
    {{NULL},"_ceilf", (char*) ceilf, 0, N_EXT | N_TEXT},
    {{NULL},"_cfree", (char*) cfree, 0, N_EXT | N_TEXT},
    {{NULL},"_changeReg", (char*) changeReg, 0, N_EXT | N_TEXT},
    {{NULL},"_chdir", (char*) chdir, 0, N_EXT | N_TEXT},
    {{NULL},"_checkInetAddrField", (char*) checkInetAddrField, 0, N_EXT | N_TEXT},
    {{NULL},"_checkStack", (char*) checkStack, 0, N_EXT | N_TEXT},
    {{NULL},"_check_trailer", (char*) check_trailer, 0, N_EXT | N_TEXT},
    {{NULL},"_checksum", (char*) checksum, 0, N_EXT | N_TEXT},
    {{NULL},"_cksum", (char*) cksum, 0, N_EXT | N_TEXT},
    {{NULL},"_classClassId", (char*) &classClassId, 0, N_EXT | N_DATA},
    {{NULL},"_classCreate", (char*) classCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_classDestroy", (char*) classDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_classHelpConnect", (char*) classHelpConnect, 0, N_EXT | N_TEXT},
    {{NULL},"_classInit", (char*) classInit, 0, N_EXT | N_TEXT},
    {{NULL},"_classLibInit", (char*) classLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_classMemPartIdSet", (char*) classMemPartIdSet, 0, N_EXT | N_TEXT},
    {{NULL},"_classShow", (char*) classShow, 0, N_EXT | N_TEXT},
    {{NULL},"_classShowConnect", (char*) classShowConnect, 0, N_EXT | N_TEXT},
    {{NULL},"_classShowInit", (char*) classShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_clearerr", (char*) clearerr, 0, N_EXT | N_TEXT},
    {{NULL},"_clock", (char*) clock, 0, N_EXT | N_TEXT},
    {{NULL},"_clockLibInit", (char*) clockLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_clock_getres", (char*) clock_getres, 0, N_EXT | N_TEXT},
    {{NULL},"_clock_gettime", (char*) clock_gettime, 0, N_EXT | N_TEXT},
    {{NULL},"_clock_setres", (char*) clock_setres, 0, N_EXT | N_TEXT},
    {{NULL},"_clock_settime", (char*) clock_settime, 0, N_EXT | N_TEXT},
    {{NULL},"_clock_show", (char*) clock_show, 0, N_EXT | N_TEXT},
    {{NULL},"_close", (char*) close, 0, N_EXT | N_TEXT},
    {{NULL},"_closedir", (char*) closedir, 0, N_EXT | N_TEXT},
    {{NULL},"_clusterConfig", (char*) &clusterConfig, 0, N_EXT | N_DATA},
    {{NULL},"_connect", (char*) connect, 0, N_EXT | N_TEXT},
    {{NULL},"_connectWithTimeout", (char*) connectWithTimeout, 0, N_EXT | N_TEXT},
    {{NULL},"_consoleFd", (char*) &consoleFd, 0, N_EXT | N_BSS},
    {{NULL},"_consoleName", (char*) &consoleName, 0, N_EXT | N_BSS},
    {{NULL},"_copy", (char*) copy, 0, N_EXT | N_TEXT},
    {{NULL},"_copyStreams", (char*) copyStreams, 0, N_EXT | N_TEXT},
    {{NULL},"_copyright_wind_river", (char*) &copyright_wind_river, 0, N_EXT | N_DATA},
    {{NULL},"_copysign", (char*) copysign, 0, N_EXT | N_TEXT},
    {{NULL},"_cos", (char*) cos, 0, N_EXT | N_TEXT},
    {{NULL},"_cosf", (char*) cosf, 0, N_EXT | N_TEXT},
    {{NULL},"_cosh", (char*) cosh, 0, N_EXT | N_TEXT},
    {{NULL},"_coshf", (char*) coshf, 0, N_EXT | N_TEXT},
    {{NULL},"_cplusDemangle", (char*) cplusDemangle, 0, N_EXT | N_TEXT},
    {{NULL},"_cplusLibInit", (char*) cplusLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_cplusLoadFixup", (char*) cplusLoadFixup, 0, N_EXT | N_TEXT},
    {{NULL},"_cplusMatchMangled", (char*) cplusMatchMangled, 0, N_EXT | N_TEXT},
    {{NULL},"_cplusUnloadFixup", (char*) cplusUnloadFixup, 0, N_EXT | N_TEXT},
    {{NULL},"_creat", (char*) creat, 0, N_EXT | N_TEXT},
    {{NULL},"_creationDate", (char*) &creationDate, 0, N_EXT | N_DATA},
    {{NULL},"_creationDateStr", (char*) &creationDateStr, 0, N_EXT | N_DATA},
    {{NULL},"_cret", (char*) cret, 0, N_EXT | N_TEXT},
    {{NULL},"_ctime", (char*) ctime, 0, N_EXT | N_TEXT},
    {{NULL},"_ctime_r", (char*) ctime_r, 0, N_EXT | N_TEXT},
    {{NULL},"_ctypeFiles", (char*) &ctypeFiles, 0, N_EXT | N_DATA},
    {{NULL},"_currentContext", (char*) &currentContext, 0, N_EXT | N_DATA},
    {{NULL},"_d", (char*) d, 0, N_EXT | N_TEXT},
    {{NULL},"_d0", (char*) d0, 0, N_EXT | N_TEXT},
    {{NULL},"_d1", (char*) d1, 0, N_EXT | N_TEXT},
    {{NULL},"_d2", (char*) d2, 0, N_EXT | N_TEXT},
    {{NULL},"_d3", (char*) d3, 0, N_EXT | N_TEXT},
    {{NULL},"_d4", (char*) d4, 0, N_EXT | N_TEXT},
    {{NULL},"_d5", (char*) d5, 0, N_EXT | N_TEXT},
    {{NULL},"_d6", (char*) d6, 0, N_EXT | N_TEXT},
    {{NULL},"_d7", (char*) d7, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBpStub", (char*) dbgBpStub, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBreakNotifyInstall", (char*) dbgBreakNotifyInstall, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBreakpoint", (char*) dbgBreakpoint, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBrkAdd", (char*) dbgBrkAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBrkDelete", (char*) dbgBrkDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBrkExists", (char*) dbgBrkExists, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBrkGet", (char*) dbgBrkGet, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBrkIgnoreDefault", (char*) dbgBrkIgnoreDefault, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgBrkIgnoreRtn", (char*) &dbgBrkIgnoreRtn, 0, N_EXT | N_DATA},
    {{NULL},"_dbgHelp", (char*) dbgHelp, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgInit", (char*) dbgInit, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgLockUnbreakable", (char*) &dbgLockUnbreakable, 0, N_EXT | N_DATA},
    {{NULL},"_dbgPrintCall", (char*) dbgPrintCall, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgSafeUnbreakable", (char*) &dbgSafeUnbreakable, 0, N_EXT | N_DATA},
    {{NULL},"_dbgStepQuiet", (char*) dbgStepQuiet, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgTrace", (char*) dbgTrace, 0, N_EXT | N_TEXT},
    {{NULL},"_dbgTraceStub", (char*) dbgTraceStub, 0, N_EXT | N_TEXT},
    {{NULL},"_devs", (char*) devs, 0, N_EXT | N_TEXT},
    {{NULL},"_difftime", (char*) difftime, 0, N_EXT | N_TEXT},
    {{NULL},"_diskFormat", (char*) diskFormat, 0, N_EXT | N_TEXT},
    {{NULL},"_diskInit", (char*) diskInit, 0, N_EXT | N_TEXT},
    {{NULL},"_div", (char*) div, 0, N_EXT | N_TEXT},
    {{NULL},"_div_r", (char*) div_r, 0, N_EXT | N_TEXT},
    {{NULL},"_dllAdd", (char*) dllAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_dllCount", (char*) dllCount, 0, N_EXT | N_TEXT},
    {{NULL},"_dllCreate", (char*) dllCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_dllDelete", (char*) dllDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_dllEach", (char*) dllEach, 0, N_EXT | N_TEXT},
    {{NULL},"_dllGet", (char*) dllGet, 0, N_EXT | N_TEXT},
    {{NULL},"_dllInit", (char*) dllInit, 0, N_EXT | N_TEXT},
    {{NULL},"_dllInsert", (char*) dllInsert, 0, N_EXT | N_TEXT},
    {{NULL},"_dllRemove", (char*) dllRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_dllTerminate", (char*) dllTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_do_protocol", (char*) do_protocol, 0, N_EXT | N_TEXT},
    {{NULL},"_do_protocol_with_type", (char*) do_protocol_with_type, 0, N_EXT | N_TEXT},
    {{NULL},"_domaininit", (char*) domaininit, 0, N_EXT | N_TEXT},
    {{NULL},"_domains", (char*) &domains, 0, N_EXT | N_BSS},
    {{NULL},"_drem", (char*) drem, 0, N_EXT | N_TEXT},
    {{NULL},"_drvTable", (char*) &drvTable, 0, N_EXT | N_BSS},
    {{NULL},"_dsmData", (char*) dsmData, 0, N_EXT | N_TEXT},
    {{NULL},"_dsmInst", (char*) dsmInst, 0, N_EXT | N_TEXT},
    {{NULL},"_dsmNbytes", (char*) dsmNbytes, 0, N_EXT | N_TEXT},
    {{NULL},"_eiEnetAddr", (char*) &eiEnetAddr, 0, N_EXT | N_DATA},
    {{NULL},"_eiattach", (char*) eiattach, 0, N_EXT | N_TEXT},
    {{NULL},"_enpNewROM", (char*) &enpNewROM, 0, N_EXT | N_BSS},
    {{NULL},"_enpShow", (char*) enpShow, 0, N_EXT | N_TEXT},
    {{NULL},"_enpattach", (char*) enpattach, 0, N_EXT | N_TEXT},
    {{NULL},"_envLibInit", (char*) envLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_envPrivateCreate", (char*) envPrivateCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_envPrivateDestroy", (char*) envPrivateDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_envShow", (char*) envShow, 0, N_EXT | N_TEXT},
    {{NULL},"_errno", (char*) &errno, 0, N_EXT | N_BSS},
    {{NULL},"_errnoGet", (char*) errnoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_errnoOfTaskGet", (char*) errnoOfTaskGet, 0, N_EXT | N_TEXT},
    {{NULL},"_errnoOfTaskSet", (char*) errnoOfTaskSet, 0, N_EXT | N_TEXT},
    {{NULL},"_errnoSet", (char*) errnoSet, 0, N_EXT | N_TEXT},
    {{NULL},"_etherAddrResolve", (char*) etherAddrResolve, 0, N_EXT | N_TEXT},
    {{NULL},"_etherInputHookAdd", (char*) etherInputHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_etherInputHookDelete", (char*) etherInputHookDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_etherInputHookRtn", (char*) &etherInputHookRtn, 0, N_EXT | N_DATA},
    {{NULL},"_etherOutput", (char*) etherOutput, 0, N_EXT | N_TEXT},
    {{NULL},"_etherOutputHookAdd", (char*) etherOutputHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_etherOutputHookDelete", (char*) etherOutputHookDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_etherOutputHookRtn", (char*) &etherOutputHookRtn, 0, N_EXT | N_DATA},
    {{NULL},"_ether_attach", (char*) ether_attach, 0, N_EXT | N_TEXT},
    {{NULL},"_ether_output", (char*) ether_output, 0, N_EXT | N_TEXT},
    {{NULL},"_ether_sprintf", (char*) ether_sprintf, 0, N_EXT | N_TEXT},
    {{NULL},"_etherbroadcastaddr", (char*) &etherbroadcastaddr, 0, N_EXT | N_DATA},
    {{NULL},"_evtBufIsEmpty", (char*) &evtBufIsEmpty, 0, N_EXT | N_BSS},
    {{NULL},"_evtBufOverflow", (char*) &evtBufOverflow, 0, N_EXT | N_BSS},
    {{NULL},"_evtBufPostMortem", (char*) &evtBufPostMortem, 0, N_EXT | N_BSS},
    {{NULL},"_evtBufSem", (char*) &evtBufSem, 0, N_EXT | N_BSS},
    {{NULL},"_evtLogOIsOn", (char*) &evtLogOIsOn, 0, N_EXT | N_BSS},
    {{NULL},"_evtLogTIsOn", (char*) &evtLogTIsOn, 0, N_EXT | N_BSS},
    {{NULL},"_exattach", (char*) exattach, 0, N_EXT | N_TEXT},
    {{NULL},"_excExcHandle", (char*) excExcHandle, 0, N_EXT | N_TEXT},
    {{NULL},"_excExcepHook", (char*) &excExcepHook, 0, N_EXT | N_BSS},
    {{NULL},"_excHookAdd", (char*) excHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_excInit", (char*) excInit, 0, N_EXT | N_TEXT},
    {{NULL},"_excIntHandle", (char*) excIntHandle, 0, N_EXT | N_TEXT},
    {{NULL},"_excIntStub", (char*) excIntStub, 0, N_EXT | N_TEXT},
    {{NULL},"_excJobAdd", (char*) excJobAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_excMsgQId", (char*) &excMsgQId, 0, N_EXT | N_BSS},
    {{NULL},"_excShowInit", (char*) excShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_excStub", (char*) excStub, 0, N_EXT | N_TEXT},
    {{NULL},"_excTask", (char*) excTask, 0, N_EXT | N_TEXT},
    {{NULL},"_excTaskId", (char*) &excTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_excTaskOptions", (char*) &excTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_excTaskPriority", (char*) &excTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_excTaskStackSize", (char*) &excTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_excVecInit", (char*) excVecInit, 0, N_EXT | N_TEXT},
    {{NULL},"_execute", (char*) execute, 0, N_EXT | N_TEXT},
    {{NULL},"_exit", (char*) exit, 0, N_EXT | N_TEXT},
    {{NULL},"_exp", (char*) exp, 0, N_EXT | N_TEXT},
    {{NULL},"_exp__E", (char*) exp__E, 0, N_EXT | N_TEXT},
    {{NULL},"_expf", (char*) expf, 0, N_EXT | N_TEXT},
    {{NULL},"_fabs", (char*) fabs, 0, N_EXT | N_TEXT},
    {{NULL},"_fabsf", (char*) fabsf, 0, N_EXT | N_TEXT},
    {{NULL},"_fclose", (char*) fclose, 0, N_EXT | N_TEXT},
    {{NULL},"_fdTable", (char*) &fdTable, 0, N_EXT | N_BSS},
    {{NULL},"_fdopen", (char*) fdopen, 0, N_EXT | N_TEXT},
    {{NULL},"_fdprintf", (char*) fdprintf, 0, N_EXT | N_TEXT},
    {{NULL},"_feof", (char*) feof, 0, N_EXT | N_TEXT},
    {{NULL},"_ferror", (char*) ferror, 0, N_EXT | N_TEXT},
    {{NULL},"_fflush", (char*) fflush, 0, N_EXT | N_TEXT},
    {{NULL},"_ffsMsb", (char*) ffsMsb, 0, N_EXT | N_TEXT},
    {{NULL},"_fgetc", (char*) fgetc, 0, N_EXT | N_TEXT},
    {{NULL},"_fgetpos", (char*) fgetpos, 0, N_EXT | N_TEXT},
    {{NULL},"_fgets", (char*) fgets, 0, N_EXT | N_TEXT},
    {{NULL},"_fieldSzIncludeSign", (char*) &fieldSzIncludeSign, 0, N_EXT | N_DATA},
    {{NULL},"_fileno", (char*) fileno, 0, N_EXT | N_TEXT},
    {{NULL},"_finite", (char*) finite, 0, N_EXT | N_TEXT},
    {{NULL},"_fioFltInstall", (char*) fioFltInstall, 0, N_EXT | N_TEXT},
    {{NULL},"_fioFormatV", (char*) fioFormatV, 0, N_EXT | N_TEXT},
    {{NULL},"_fioRdString", (char*) fioRdString, 0, N_EXT | N_TEXT},
    {{NULL},"_fioRead", (char*) fioRead, 0, N_EXT | N_TEXT},
    {{NULL},"_fioScanV", (char*) fioScanV, 0, N_EXT | N_TEXT},
    {{NULL},"_floatInit", (char*) floatInit, 0, N_EXT | N_TEXT},
    {{NULL},"_floor", (char*) floor, 0, N_EXT | N_TEXT},
    {{NULL},"_floorf", (char*) floorf, 0, N_EXT | N_TEXT},
    {{NULL},"_fmod", (char*) fmod, 0, N_EXT | N_TEXT},
    {{NULL},"_fmodf", (char*) fmodf, 0, N_EXT | N_TEXT},
    {{NULL},"_fopen", (char*) fopen, 0, N_EXT | N_TEXT},
    {{NULL},"_fpClassId", (char*) &fpClassId, 0, N_EXT | N_DATA},
    {{NULL},"_fpCtlRegName", (char*) &fpCtlRegName, 0, N_EXT | N_DATA},
    {{NULL},"_fpRegName", (char*) &fpRegName, 0, N_EXT | N_DATA},
    {{NULL},"_fpTypeGet", (char*) fpTypeGet, 0, N_EXT | N_TEXT},
    {{NULL},"_fppArchInit", (char*) fppArchInit, 0, N_EXT | N_TEXT},
    {{NULL},"_fppArchTaskCreateInit", (char*) fppArchTaskCreateInit, 0, N_EXT | N_TEXT},
    {{NULL},"_fppCreateHookRtn", (char*) &fppCreateHookRtn, 0, N_EXT | N_BSS},
    {{NULL},"_fppDisplayHookRtn", (char*) &fppDisplayHookRtn, 0, N_EXT | N_BSS},
    {{NULL},"_fppDtoDx", (char*) fppDtoDx, 0, N_EXT | N_TEXT},
    {{NULL},"_fppDxtoD", (char*) fppDxtoD, 0, N_EXT | N_TEXT},
    {{NULL},"_fppInit", (char*) fppInit, 0, N_EXT | N_TEXT},
    {{NULL},"_fppProbe", (char*) fppProbe, 0, N_EXT | N_TEXT},
    {{NULL},"_fppProbeSup", (char*) fppProbeSup, 0, N_EXT | N_TEXT},
    {{NULL},"_fppProbeTrap", (char*) fppProbeTrap, 0, N_EXT | N_TEXT},
    {{NULL},"_fppReset", (char*) fppReset, 0, N_EXT | N_TEXT},
    {{NULL},"_fppRestore", (char*) fppRestore, 0, N_EXT | N_TEXT},
    {{NULL},"_fppSave", (char*) fppSave, 0, N_EXT | N_TEXT},
    {{NULL},"_fppShowInit", (char*) fppShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_fppTaskRegsCFmt", (char*) &fppTaskRegsCFmt, 0, N_EXT | N_DATA},
    {{NULL},"_fppTaskRegsDFmt", (char*) &fppTaskRegsDFmt, 0, N_EXT | N_DATA},
    {{NULL},"_fppTaskRegsGet", (char*) fppTaskRegsGet, 0, N_EXT | N_TEXT},
    {{NULL},"_fppTaskRegsSet", (char*) fppTaskRegsSet, 0, N_EXT | N_TEXT},
    {{NULL},"_fppTaskRegsShow", (char*) fppTaskRegsShow, 0, N_EXT | N_TEXT},
    {{NULL},"_fprintf", (char*) fprintf, 0, N_EXT | N_TEXT},
    {{NULL},"_fputc", (char*) fputc, 0, N_EXT | N_TEXT},
    {{NULL},"_fputs", (char*) fputs, 0, N_EXT | N_TEXT},
    {{NULL},"_fread", (char*) fread, 0, N_EXT | N_TEXT},
    {{NULL},"_free", (char*) free, 0, N_EXT | N_TEXT},
    {{NULL},"_freopen", (char*) freopen, 0, N_EXT | N_TEXT},
    {{NULL},"_frexp", (char*) frexp, 0, N_EXT | N_TEXT},
    {{NULL},"_fscanf", (char*) fscanf, 0, N_EXT | N_TEXT},
    {{NULL},"_fseek", (char*) fseek, 0, N_EXT | N_TEXT},
    {{NULL},"_fsetpos", (char*) fsetpos, 0, N_EXT | N_TEXT},
    {{NULL},"_fstat", (char*) fstat, 0, N_EXT | N_TEXT},
    {{NULL},"_fstatfs", (char*) fstatfs, 0, N_EXT | N_TEXT},
    {{NULL},"_ftell", (char*) ftell, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpCommand", (char*) ftpCommand, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpDataConnGet", (char*) ftpDataConnGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpDataConnInit", (char*) ftpDataConnInit, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpDebug", (char*) &ftpDebug, 0, N_EXT | N_DATA},
    {{NULL},"_ftpErrorSuppress", (char*) &ftpErrorSuppress, 0, N_EXT | N_DATA},
    {{NULL},"_ftpHookup", (char*) ftpHookup, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpLogin", (char*) ftpLogin, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpLs", (char*) ftpLs, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpReplyGet", (char*) ftpReplyGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpVerbose", (char*) &ftpVerbose, 0, N_EXT | N_DATA},
    {{NULL},"_ftpXfer", (char*) ftpXfer, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpdDebug", (char*) &ftpdDebug, 0, N_EXT | N_DATA},
    {{NULL},"_ftpdDelete", (char*) ftpdDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpdInit", (char*) ftpdInit, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpdTask", (char*) ftpdTask, 0, N_EXT | N_TEXT},
    {{NULL},"_ftpdTaskPriority", (char*) &ftpdTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_ftpdWorkTaskOptions", (char*) &ftpdWorkTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_ftpdWorkTaskPriority", (char*) &ftpdWorkTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_ftpdWorkTaskStackSize", (char*) &ftpdWorkTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_fwrite", (char*) fwrite, 0, N_EXT | N_TEXT},
    {{NULL},"_gccUss040Init", (char*) gccUss040Init, 0, N_EXT | N_TEXT},
    {{NULL},"_getc", (char*) getc, 0, N_EXT | N_TEXT},
    {{NULL},"_getchar", (char*) getchar, 0, N_EXT | N_TEXT},
    {{NULL},"_getcwd", (char*) getcwd, 0, N_EXT | N_TEXT},
    {{NULL},"_getenv", (char*) getenv, 0, N_EXT | N_TEXT},
    {{NULL},"_gethostname", (char*) gethostname, 0, N_EXT | N_TEXT},
    {{NULL},"_getpeername", (char*) getpeername, 0, N_EXT | N_TEXT},
    {{NULL},"_gets", (char*) gets, 0, N_EXT | N_TEXT},
    {{NULL},"_getsockname", (char*) getsockname, 0, N_EXT | N_TEXT},
    {{NULL},"_getsockopt", (char*) getsockopt, 0, N_EXT | N_TEXT},
    {{NULL},"_getw", (char*) getw, 0, N_EXT | N_TEXT},
    {{NULL},"_getwd", (char*) getwd, 0, N_EXT | N_TEXT},
    {{NULL},"_gmtime", (char*) gmtime, 0, N_EXT | N_TEXT},
    {{NULL},"_gmtime_r", (char*) gmtime_r, 0, N_EXT | N_TEXT},
    {{NULL},"_h", (char*) h, 0, N_EXT | N_TEXT},
    {{NULL},"_hashClassId", (char*) &hashClassId, 0, N_EXT | N_DATA},
    {{NULL},"_hashFuncIterScale", (char*) hashFuncIterScale, 0, N_EXT | N_TEXT},
    {{NULL},"_hashFuncModulo", (char*) hashFuncModulo, 0, N_EXT | N_TEXT},
    {{NULL},"_hashFuncMultiply", (char*) hashFuncMultiply, 0, N_EXT | N_TEXT},
    {{NULL},"_hashKeyCmp", (char*) hashKeyCmp, 0, N_EXT | N_TEXT},
    {{NULL},"_hashKeyStrCmp", (char*) hashKeyStrCmp, 0, N_EXT | N_TEXT},
    {{NULL},"_hashLibInit", (char*) hashLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblCreate", (char*) hashTblCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblDelete", (char*) hashTblDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblDestroy", (char*) hashTblDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblEach", (char*) hashTblEach, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblFind", (char*) hashTblFind, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblInit", (char*) hashTblInit, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblPut", (char*) hashTblPut, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblRemove", (char*) hashTblRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_hashTblTerminate", (char*) hashTblTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_help", (char*) help, 0, N_EXT | N_TEXT},
    {{NULL},"_hostAdd", (char*) hostAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_hostDelete", (char*) hostDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_hostGetByAddr", (char*) hostGetByAddr, 0, N_EXT | N_TEXT},
    {{NULL},"_hostGetByName", (char*) hostGetByName, 0, N_EXT | N_TEXT},
    {{NULL},"_hostList", (char*) &hostList, 0, N_EXT | N_BSS},
    {{NULL},"_hostListSem", (char*) &hostListSem, 0, N_EXT | N_BSS},
    {{NULL},"_hostShow", (char*) hostShow, 0, N_EXT | N_TEXT},
    {{NULL},"_hostTblInit", (char*) hostTblInit, 0, N_EXT | N_TEXT},
    {{NULL},"_hypot", (char*) hypot, 0, N_EXT | N_TEXT},
    {{NULL},"_hypotf", (char*) hypotf, 0, N_EXT | N_TEXT},
    {{NULL},"_i", (char*) i, 0, N_EXT | N_TEXT},
    {{NULL},"_iam", (char*) iam, 0, N_EXT | N_TEXT},
    {{NULL},"_icmp_error", (char*) icmp_error, 0, N_EXT | N_TEXT},
    {{NULL},"_icmp_input", (char*) icmp_input, 0, N_EXT | N_TEXT},
    {{NULL},"_icmp_reflect", (char*) icmp_reflect, 0, N_EXT | N_TEXT},
    {{NULL},"_icmp_send", (char*) icmp_send, 0, N_EXT | N_TEXT},
    {{NULL},"_icmpstat", (char*) &icmpstat, 0, N_EXT | N_BSS},
    {{NULL},"_icmpstatShow", (char*) icmpstatShow, 0, N_EXT | N_TEXT},
    {{NULL},"_ifAddrGet", (char*) ifAddrGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifAddrSet", (char*) ifAddrSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifAttachChange", (char*) &ifAttachChange, 0, N_EXT | N_BSS},
    {{NULL},"_ifBroadcastGet", (char*) ifBroadcastGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifBroadcastSet", (char*) ifBroadcastSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifDstAddrGet", (char*) ifDstAddrGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifDstAddrSet", (char*) ifDstAddrSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifFlagChange", (char*) ifFlagChange, 0, N_EXT | N_TEXT},
    {{NULL},"_ifFlagGet", (char*) ifFlagGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifFlagSet", (char*) ifFlagSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifMaskGet", (char*) ifMaskGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifMaskSet", (char*) ifMaskSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifMetricGet", (char*) ifMetricGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifMetricSet", (char*) ifMetricSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifRouteDelete", (char*) ifRouteDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_ifShow", (char*) ifShow, 0, N_EXT | N_TEXT},
    {{NULL},"_if_attach", (char*) if_attach, 0, N_EXT | N_TEXT},
    {{NULL},"_if_dettach", (char*) if_dettach, 0, N_EXT | N_TEXT},
    {{NULL},"_if_down", (char*) if_down, 0, N_EXT | N_TEXT},
    {{NULL},"_if_qflush", (char*) if_qflush, 0, N_EXT | N_TEXT},
    {{NULL},"_if_slowtimo", (char*) if_slowtimo, 0, N_EXT | N_TEXT},
    {{NULL},"_ifa_ifwithaddr", (char*) ifa_ifwithaddr, 0, N_EXT | N_TEXT},
    {{NULL},"_ifa_ifwithdstaddr", (char*) ifa_ifwithdstaddr, 0, N_EXT | N_TEXT},
    {{NULL},"_ifa_ifwithnet", (char*) ifa_ifwithnet, 0, N_EXT | N_TEXT},
    {{NULL},"_ifconf", (char*) ifconf, 0, N_EXT | N_TEXT},
    {{NULL},"_ifinit", (char*) ifinit, 0, N_EXT | N_TEXT},
    {{NULL},"_ifioctl", (char*) ifioctl, 0, N_EXT | N_TEXT},
    {{NULL},"_ifnet", (char*) &ifnet, 0, N_EXT | N_BSS},
    {{NULL},"_ifptoia", (char*) ifptoia, 0, N_EXT | N_TEXT},
    {{NULL},"_ifqmaxlen", (char*) &ifqmaxlen, 0, N_EXT | N_DATA},
    {{NULL},"_ifreset", (char*) ifreset, 0, N_EXT | N_TEXT},
    {{NULL},"_ifunit", (char*) ifunit, 0, N_EXT | N_TEXT},
    {{NULL},"_in_arpinput", (char*) in_arpinput, 0, N_EXT | N_TEXT},
    {{NULL},"_in_broadcast", (char*) in_broadcast, 0, N_EXT | N_TEXT},
    {{NULL},"_in_canforward", (char*) in_canforward, 0, N_EXT | N_TEXT},
    {{NULL},"_in_cksum", (char*) in_cksum, 0, N_EXT | N_TEXT},
    {{NULL},"_in_control", (char*) in_control, 0, N_EXT | N_TEXT},
    {{NULL},"_in_iaonnetof", (char*) in_iaonnetof, 0, N_EXT | N_TEXT},
    {{NULL},"_in_ifaddr", (char*) &in_ifaddr, 0, N_EXT | N_BSS},
    {{NULL},"_in_ifaddr_remove", (char*) in_ifaddr_remove, 0, N_EXT | N_TEXT},
    {{NULL},"_in_ifinit", (char*) in_ifinit, 0, N_EXT | N_TEXT},
    {{NULL},"_in_interfaces", (char*) &in_interfaces, 0, N_EXT | N_BSS},
    {{NULL},"_in_lnaof", (char*) in_lnaof, 0, N_EXT | N_TEXT},
    {{NULL},"_in_localaddr", (char*) in_localaddr, 0, N_EXT | N_TEXT},
    {{NULL},"_in_losing", (char*) in_losing, 0, N_EXT | N_TEXT},
    {{NULL},"_in_makeaddr_b", (char*) in_makeaddr_b, 0, N_EXT | N_TEXT},
    {{NULL},"_in_netof", (char*) in_netof, 0, N_EXT | N_TEXT},
    {{NULL},"_in_pcballoc", (char*) in_pcballoc, 0, N_EXT | N_TEXT},
    {{NULL},"_in_pcbbind", (char*) in_pcbbind, 0, N_EXT | N_TEXT},
    {{NULL},"_in_pcbconnect", (char*) in_pcbconnect, 0, N_EXT | N_TEXT},
    {{NULL},"_in_pcbdetach", (char*) in_pcbdetach, 0, N_EXT | N_TEXT},
    {{NULL},"_in_pcbdisconnect", (char*) in_pcbdisconnect, 0, N_EXT | N_TEXT},
    {{NULL},"_in_pcblookup", (char*) in_pcblookup, 0, N_EXT | N_TEXT},
    {{NULL},"_in_pcbnotify", (char*) in_pcbnotify, 0, N_EXT | N_TEXT},
    {{NULL},"_in_rtchange", (char*) in_rtchange, 0, N_EXT | N_TEXT},
    {{NULL},"_in_setpeeraddr", (char*) in_setpeeraddr, 0, N_EXT | N_TEXT},
    {{NULL},"_in_setsockaddr", (char*) in_setsockaddr, 0, N_EXT | N_TEXT},
    {{NULL},"_index", (char*) index, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_addr", (char*) inet_addr, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_hash", (char*) inet_hash, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_lnaof", (char*) inet_lnaof, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_makeaddr", (char*) inet_makeaddr, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_makeaddr_b", (char*) inet_makeaddr_b, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_netmatch", (char*) inet_netmatch, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_netof", (char*) inet_netof, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_netof_string", (char*) inet_netof_string, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_network", (char*) inet_network, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_ntoa", (char*) inet_ntoa, 0, N_EXT | N_TEXT},
    {{NULL},"_inet_ntoa_b", (char*) inet_ntoa_b, 0, N_EXT | N_TEXT},
    {{NULL},"_inetctlerrmap", (char*) &inetctlerrmap, 0, N_EXT | N_DATA},
    {{NULL},"_inetdomain", (char*) &inetdomain, 0, N_EXT | N_DATA},
    {{NULL},"_inetstatShow", (char*) inetstatShow, 0, N_EXT | N_TEXT},
    {{NULL},"_inetsw", (char*) &inetsw, 0, N_EXT | N_DATA},
    {{NULL},"_infinity", (char*) infinity, 0, N_EXT | N_TEXT},
    {{NULL},"_infinityf", (char*) infinityf, 0, N_EXT | N_TEXT},
    {{NULL},"_intCnt", (char*) &intCnt, 0, N_EXT | N_DATA},
    {{NULL},"_intConnect", (char*) intConnect, 0, N_EXT | N_TEXT},
    {{NULL},"_intContext", (char*) intContext, 0, N_EXT | N_TEXT},
    {{NULL},"_intCount", (char*) intCount, 0, N_EXT | N_TEXT},
    {{NULL},"_intEnt", (char*) intEnt, 0, N_EXT | N_TEXT},
    {{NULL},"_intExit", (char*) intExit, 0, N_EXT | N_TEXT},
    {{NULL},"_intHandlerCreate", (char*) intHandlerCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_intLevelSet", (char*) intLevelSet, 0, N_EXT | N_TEXT},
    {{NULL},"_intLock", (char*) intLock, 0, N_EXT | N_TEXT},
    {{NULL},"_intLockIntSR", (char*) &intLockIntSR, 0, N_EXT | N_DATA},
    {{NULL},"_intLockLevelGet", (char*) intLockLevelGet, 0, N_EXT | N_TEXT},
    {{NULL},"_intLockLevelSet", (char*) intLockLevelSet, 0, N_EXT | N_TEXT},
    {{NULL},"_intLockMask", (char*) &intLockMask, 0, N_EXT | N_DATA},
    {{NULL},"_intLockTaskSR", (char*) &intLockTaskSR, 0, N_EXT | N_DATA},
    {{NULL},"_intRestrict", (char*) intRestrict, 0, N_EXT | N_TEXT},
    {{NULL},"_intUnlock", (char*) intUnlock, 0, N_EXT | N_TEXT},
    {{NULL},"_intVBRSet", (char*) intVBRSet, 0, N_EXT | N_TEXT},
    {{NULL},"_intVecBaseGet", (char*) intVecBaseGet, 0, N_EXT | N_TEXT},
    {{NULL},"_intVecBaseSet", (char*) intVecBaseSet, 0, N_EXT | N_TEXT},
    {{NULL},"_intVecGet", (char*) intVecGet, 0, N_EXT | N_TEXT},
    {{NULL},"_intVecSet", (char*) intVecSet, 0, N_EXT | N_TEXT},
    {{NULL},"_intVecTableWriteProtect", (char*) intVecTableWriteProtect, 0, N_EXT | N_TEXT},
    {{NULL},"_ioDefDevGet", (char*) ioDefDevGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioDefDirGet", (char*) ioDefDirGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioDefPath", (char*) &ioDefPath, 0, N_EXT | N_BSS},
    {{NULL},"_ioDefPathCat", (char*) ioDefPathCat, 0, N_EXT | N_TEXT},
    {{NULL},"_ioDefPathGet", (char*) ioDefPathGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioDefPathSet", (char*) ioDefPathSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioFullFileNameGet", (char*) ioFullFileNameGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioGlobalStdGet", (char*) ioGlobalStdGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioGlobalStdSet", (char*) ioGlobalStdSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioMaxLinkLevels", (char*) &ioMaxLinkLevels, 0, N_EXT | N_DATA},
    {{NULL},"_ioTaskStdGet", (char*) ioTaskStdGet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioTaskStdSet", (char*) ioTaskStdSet, 0, N_EXT | N_TEXT},
    {{NULL},"_ioctl", (char*) ioctl, 0, N_EXT | N_TEXT},
    {{NULL},"_iosClose", (char*) iosClose, 0, N_EXT | N_TEXT},
    {{NULL},"_iosCreate", (char*) iosCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDelete", (char*) iosDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDevAdd", (char*) iosDevAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDevDelete", (char*) iosDevDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDevFind", (char*) iosDevFind, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDevShow", (char*) iosDevShow, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDrvInstall", (char*) iosDrvInstall, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDrvRemove", (char*) iosDrvRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDrvShow", (char*) iosDrvShow, 0, N_EXT | N_TEXT},
    {{NULL},"_iosDvList", (char*) &iosDvList, 0, N_EXT | N_BSS},
    {{NULL},"_iosFdDevFind", (char*) iosFdDevFind, 0, N_EXT | N_TEXT},
    {{NULL},"_iosFdFree", (char*) iosFdFree, 0, N_EXT | N_TEXT},
    {{NULL},"_iosFdFreeHookRtn", (char*) &iosFdFreeHookRtn, 0, N_EXT | N_DATA},
    {{NULL},"_iosFdNew", (char*) iosFdNew, 0, N_EXT | N_TEXT},
    {{NULL},"_iosFdNewHookRtn", (char*) &iosFdNewHookRtn, 0, N_EXT | N_DATA},
    {{NULL},"_iosFdSet", (char*) iosFdSet, 0, N_EXT | N_TEXT},
    {{NULL},"_iosFdShow", (char*) iosFdShow, 0, N_EXT | N_TEXT},
    {{NULL},"_iosFdValue", (char*) iosFdValue, 0, N_EXT | N_TEXT},
    {{NULL},"_iosInit", (char*) iosInit, 0, N_EXT | N_TEXT},
    {{NULL},"_iosIoctl", (char*) iosIoctl, 0, N_EXT | N_TEXT},
    {{NULL},"_iosLibInitialized", (char*) &iosLibInitialized, 0, N_EXT | N_DATA},
    {{NULL},"_iosNextDevGet", (char*) iosNextDevGet, 0, N_EXT | N_TEXT},
    {{NULL},"_iosOpen", (char*) iosOpen, 0, N_EXT | N_TEXT},
    {{NULL},"_iosRead", (char*) iosRead, 0, N_EXT | N_TEXT},
    {{NULL},"_iosShowInit", (char*) iosShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_iosWrite", (char*) iosWrite, 0, N_EXT | N_TEXT},
    {{NULL},"_ipFragCreates", (char*) &ipFragCreates, 0, N_EXT | N_BSS},
    {{NULL},"_ipFragFails", (char*) &ipFragFails, 0, N_EXT | N_BSS},
    {{NULL},"_ipFragOKs", (char*) &ipFragOKs, 0, N_EXT | N_BSS},
    {{NULL},"_ipInDelivers", (char*) &ipInDelivers, 0, N_EXT | N_BSS},
    {{NULL},"_ipInUnknownProtos", (char*) &ipInUnknownProtos, 0, N_EXT | N_BSS},
    {{NULL},"_ipOutDatagrams", (char*) &ipOutDatagrams, 0, N_EXT | N_BSS},
    {{NULL},"_ipOutDiscards", (char*) &ipOutDiscards, 0, N_EXT | N_BSS},
    {{NULL},"_ipOutNoRoutes", (char*) &ipOutNoRoutes, 0, N_EXT | N_BSS},
    {{NULL},"_ipReasmOKs", (char*) &ipReasmOKs, 0, N_EXT | N_BSS},
    {{NULL},"_ipReasmReqds", (char*) &ipReasmReqds, 0, N_EXT | N_BSS},
    {{NULL},"_ipTimeToLive", (char*) &ipTimeToLive, 0, N_EXT | N_DATA},
    {{NULL},"_ip_ctloutput", (char*) ip_ctloutput, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_deq", (char*) ip_deq, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_dooptions", (char*) ip_dooptions, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_drain", (char*) ip_drain, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_enq", (char*) ip_enq, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_forward", (char*) ip_forward, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_freef", (char*) ip_freef, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_id", (char*) &ip_id, 0, N_EXT | N_BSS},
    {{NULL},"_ip_init", (char*) ip_init, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_insertoptions", (char*) ip_insertoptions, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_nhops", (char*) &ip_nhops, 0, N_EXT | N_DATA},
    {{NULL},"_ip_optcopy", (char*) ip_optcopy, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_output", (char*) ip_output, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_pcbopts", (char*) ip_pcbopts, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_protox", (char*) &ip_protox, 0, N_EXT | N_BSS},
    {{NULL},"_ip_reass", (char*) ip_reass, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_rtaddr", (char*) ip_rtaddr, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_slowtimo", (char*) ip_slowtimo, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_srcroute", (char*) ip_srcroute, 0, N_EXT | N_TEXT},
    {{NULL},"_ip_stripoptions", (char*) ip_stripoptions, 0, N_EXT | N_TEXT},
    {{NULL},"_ipaddr", (char*) &ipaddr, 0, N_EXT | N_DATA},
    {{NULL},"_ipcksum", (char*) &ipcksum, 0, N_EXT | N_DATA},
    {{NULL},"_ipforward_rt", (char*) &ipforward_rt, 0, N_EXT | N_BSS},
    {{NULL},"_ipforwarding", (char*) &ipforwarding, 0, N_EXT | N_DATA},
    {{NULL},"_ipintr", (char*) ipintr, 0, N_EXT | N_TEXT},
    {{NULL},"_ipintrq", (char*) &ipintrq, 0, N_EXT | N_BSS},
    {{NULL},"_ipprintfs", (char*) &ipprintfs, 0, N_EXT | N_DATA},
    {{NULL},"_ipqmaxlen", (char*) &ipqmaxlen, 0, N_EXT | N_DATA},
    {{NULL},"_ipsendredirects", (char*) &ipsendredirects, 0, N_EXT | N_DATA},
    {{NULL},"_ipstat", (char*) &ipstat, 0, N_EXT | N_BSS},
    {{NULL},"_ipstatShow", (char*) ipstatShow, 0, N_EXT | N_TEXT},
    {{NULL},"_iptime", (char*) iptime, 0, N_EXT | N_TEXT},
    {{NULL},"_irint", (char*) irint, 0, N_EXT | N_TEXT},
    {{NULL},"_irintf", (char*) irintf, 0, N_EXT | N_TEXT},
    {{NULL},"_iround", (char*) iround, 0, N_EXT | N_TEXT},
    {{NULL},"_iroundf", (char*) iroundf, 0, N_EXT | N_TEXT},
    {{NULL},"_isalnum", (char*) isalnum, 0, N_EXT | N_TEXT},
    {{NULL},"_isalpha", (char*) isalpha, 0, N_EXT | N_TEXT},
    {{NULL},"_isatty", (char*) isatty, 0, N_EXT | N_TEXT},
    {{NULL},"_iscntrl", (char*) iscntrl, 0, N_EXT | N_TEXT},
    {{NULL},"_isdigit", (char*) isdigit, 0, N_EXT | N_TEXT},
    {{NULL},"_isgraph", (char*) isgraph, 0, N_EXT | N_TEXT},
    {{NULL},"_islower", (char*) islower, 0, N_EXT | N_TEXT},
    {{NULL},"_isprint", (char*) isprint, 0, N_EXT | N_TEXT},
    {{NULL},"_ispunct", (char*) ispunct, 0, N_EXT | N_TEXT},
    {{NULL},"_isspace", (char*) isspace, 0, N_EXT | N_TEXT},
    {{NULL},"_isupper", (char*) isupper, 0, N_EXT | N_TEXT},
    {{NULL},"_isxdigit", (char*) isxdigit, 0, N_EXT | N_TEXT},
    {{NULL},"_kernelInit", (char*) kernelInit, 0, N_EXT | N_TEXT},
    {{NULL},"_kernelIsIdle", (char*) &kernelIsIdle, 0, N_EXT | N_BSS},
    {{NULL},"_kernelState", (char*) &kernelState, 0, N_EXT | N_BSS},
    {{NULL},"_kernelTimeSlice", (char*) kernelTimeSlice, 0, N_EXT | N_TEXT},
    {{NULL},"_kernelVersion", (char*) kernelVersion, 0, N_EXT | N_TEXT},
    {{NULL},"_kill", (char*) kill, 0, N_EXT | N_TEXT},
    {{NULL},"_ksleep", (char*) ksleep, 0, N_EXT | N_TEXT},
    {{NULL},"_l", (char*) l, 0, N_EXT | N_TEXT},
    {{NULL},"_labs", (char*) labs, 0, N_EXT | N_TEXT},
    {{NULL},"_ld", (char*) ld, 0, N_EXT | N_TEXT},
    {{NULL},"_ldexp", (char*) ldexp, 0, N_EXT | N_TEXT},
    {{NULL},"_ldiv", (char*) ldiv, 0, N_EXT | N_TEXT},
    {{NULL},"_ldiv_r", (char*) ldiv_r, 0, N_EXT | N_TEXT},
    {{NULL},"_ledClose", (char*) ledClose, 0, N_EXT | N_TEXT},
    {{NULL},"_ledControl", (char*) ledControl, 0, N_EXT | N_TEXT},
    {{NULL},"_ledId", (char*) &ledId, 0, N_EXT | N_BSS},
    {{NULL},"_ledOpen", (char*) ledOpen, 0, N_EXT | N_TEXT},
    {{NULL},"_ledRead", (char*) ledRead, 0, N_EXT | N_TEXT},
    {{NULL},"_lexActions", (char*) lexActions, 0, N_EXT | N_TEXT},
    {{NULL},"_lexClass", (char*) &lexClass, 0, N_EXT | N_DATA},
    {{NULL},"_lexNclasses", (char*) &lexNclasses, 0, N_EXT | N_DATA},
    {{NULL},"_lexStateTable", (char*) &lexStateTable, 0, N_EXT | N_DATA},
    {{NULL},"_listen", (char*) listen, 0, N_EXT | N_TEXT},
    {{NULL},"_lkAddr", (char*) lkAddr, 0, N_EXT | N_TEXT},
    {{NULL},"_lkup", (char*) lkup, 0, N_EXT | N_TEXT},
    {{NULL},"_ll", (char*) ll, 0, N_EXT | N_TEXT},
    {{NULL},"_loadAoutInit", (char*) loadAoutInit, 0, N_EXT | N_TEXT},
    {{NULL},"_loadModule", (char*) loadModule, 0, N_EXT | N_TEXT},
    {{NULL},"_loadModuleAt", (char*) loadModuleAt, 0, N_EXT | N_TEXT},
    {{NULL},"_loadModuleAtSym", (char*) loadModuleAtSym, 0, N_EXT | N_TEXT},
    {{NULL},"_loadModuleGet", (char*) loadModuleGet, 0, N_EXT | N_TEXT},
    {{NULL},"_loadRoutine", (char*) &loadRoutine, 0, N_EXT | N_DATA},
    {{NULL},"_loadSegmentsAllocate", (char*) loadSegmentsAllocate, 0, N_EXT | N_TEXT},
    {{NULL},"_loanBuild", (char*) loanBuild, 0, N_EXT | N_TEXT},
    {{NULL},"_loattach", (char*) loattach, 0, N_EXT | N_TEXT},
    {{NULL},"_localToGlobalOffset", (char*) &localToGlobalOffset, 0, N_EXT | N_BSS},
    {{NULL},"_localeFiles", (char*) &localeFiles, 0, N_EXT | N_DATA},
    {{NULL},"_localeconv", (char*) localeconv, 0, N_EXT | N_TEXT},
    {{NULL},"_localtime", (char*) localtime, 0, N_EXT | N_TEXT},
    {{NULL},"_localtime_r", (char*) localtime_r, 0, N_EXT | N_TEXT},
    {{NULL},"_log", (char*) log, 0, N_EXT | N_TEXT},
    {{NULL},"_log10", (char*) log10, 0, N_EXT | N_TEXT},
    {{NULL},"_log10f", (char*) log10f, 0, N_EXT | N_TEXT},
    {{NULL},"_log2", (char*) log2, 0, N_EXT | N_TEXT},
    {{NULL},"_log2f", (char*) log2f, 0, N_EXT | N_TEXT},
    {{NULL},"_logFdAdd", (char*) logFdAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_logFdDelete", (char*) logFdDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_logFdFromRlogin", (char*) &logFdFromRlogin, 0, N_EXT | N_DATA},
    {{NULL},"_logFdSet", (char*) logFdSet, 0, N_EXT | N_TEXT},
    {{NULL},"_logInit", (char*) logInit, 0, N_EXT | N_TEXT},
    {{NULL},"_logMsg", (char*) logMsg, 0, N_EXT | N_TEXT},
    {{NULL},"_logShow", (char*) logShow, 0, N_EXT | N_TEXT},
    {{NULL},"_logTask", (char*) logTask, 0, N_EXT | N_TEXT},
    {{NULL},"_logTaskId", (char*) &logTaskId, 0, N_EXT | N_DATA},
    {{NULL},"_logTaskOptions", (char*) &logTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_logTaskPriority", (char*) &logTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_logTaskStackSize", (char*) &logTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_log__L", (char*) log__L, 0, N_EXT | N_TEXT},
    {{NULL},"_logb", (char*) logb, 0, N_EXT | N_TEXT},
    {{NULL},"_logf", (char*) logf, 0, N_EXT | N_TEXT},
    {{NULL},"_logout", (char*) logout, 0, N_EXT | N_TEXT},
    {{NULL},"_loif", (char*) &loif, 0, N_EXT | N_BSS},
    {{NULL},"_longjmp", (char*) longjmp, 0, N_EXT | N_TEXT},
    {{NULL},"_looutput", (char*) looutput, 0, N_EXT | N_TEXT},
    {{NULL},"_ls", (char*) ls, 0, N_EXT | N_TEXT},
    {{NULL},"_lsOld", (char*) lsOld, 0, N_EXT | N_TEXT},
    {{NULL},"_lseek", (char*) lseek, 0, N_EXT | N_TEXT},
    {{NULL},"_lstAdd", (char*) lstAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_lstConcat", (char*) lstConcat, 0, N_EXT | N_TEXT},
    {{NULL},"_lstCount", (char*) lstCount, 0, N_EXT | N_TEXT},
    {{NULL},"_lstDelete", (char*) lstDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_lstExtract", (char*) lstExtract, 0, N_EXT | N_TEXT},
    {{NULL},"_lstFind", (char*) lstFind, 0, N_EXT | N_TEXT},
    {{NULL},"_lstFirst", (char*) lstFirst, 0, N_EXT | N_TEXT},
    {{NULL},"_lstFree", (char*) lstFree, 0, N_EXT | N_TEXT},
    {{NULL},"_lstGet", (char*) lstGet, 0, N_EXT | N_TEXT},
    {{NULL},"_lstInit", (char*) lstInit, 0, N_EXT | N_TEXT},
    {{NULL},"_lstInsert", (char*) lstInsert, 0, N_EXT | N_TEXT},
    {{NULL},"_lstLast", (char*) lstLast, 0, N_EXT | N_TEXT},
    {{NULL},"_lstNStep", (char*) lstNStep, 0, N_EXT | N_TEXT},
    {{NULL},"_lstNext", (char*) lstNext, 0, N_EXT | N_TEXT},
    {{NULL},"_lstNth", (char*) lstNth, 0, N_EXT | N_TEXT},
    {{NULL},"_lstPrevious", (char*) lstPrevious, 0, N_EXT | N_TEXT},
    {{NULL},"_m", (char*) m, 0, N_EXT | N_TEXT},
    {{NULL},"_mRegs", (char*) mRegs, 0, N_EXT | N_TEXT},
    {{NULL},"_m_adj", (char*) m_adj, 0, N_EXT | N_TEXT},
    {{NULL},"_m_cat", (char*) m_cat, 0, N_EXT | N_TEXT},
    {{NULL},"_m_clalloc", (char*) m_clalloc, 0, N_EXT | N_TEXT},
    {{NULL},"_m_copy", (char*) m_copy, 0, N_EXT | N_TEXT},
    {{NULL},"_m_expand", (char*) m_expand, 0, N_EXT | N_TEXT},
    {{NULL},"_m_free", (char*) m_free, 0, N_EXT | N_TEXT},
    {{NULL},"_m_freem", (char*) m_freem, 0, N_EXT | N_TEXT},
    {{NULL},"_m_get", (char*) m_get, 0, N_EXT | N_TEXT},
    {{NULL},"_m_getclr", (char*) m_getclr, 0, N_EXT | N_TEXT},
    {{NULL},"_m_more", (char*) m_more, 0, N_EXT | N_TEXT},
    {{NULL},"_m_pullup", (char*) m_pullup, 0, N_EXT | N_TEXT},
    {{NULL},"_m_want", (char*) &m_want, 0, N_EXT | N_BSS},
    {{NULL},"_malloc", (char*) malloc, 0, N_EXT | N_TEXT},
    {{NULL},"_mathAcosFunc", (char*) &mathAcosFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathAcosfFunc", (char*) &mathAcosfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathAsinFunc", (char*) &mathAsinFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathAsinfFunc", (char*) &mathAsinfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathAtan2Func", (char*) &mathAtan2Func, 0, N_EXT | N_DATA},
    {{NULL},"_mathAtan2fFunc", (char*) &mathAtan2fFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathAtanFunc", (char*) &mathAtanFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathAtanfFunc", (char*) &mathAtanfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathCbrtFunc", (char*) &mathCbrtFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathCbrtfFunc", (char*) &mathCbrtfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathCeilFunc", (char*) &mathCeilFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathCeilfFunc", (char*) &mathCeilfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathCosFunc", (char*) &mathCosFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathCosfFunc", (char*) &mathCosfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathCoshFunc", (char*) &mathCoshFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathCoshfFunc", (char*) &mathCoshfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathErrNoInit", (char*) mathErrNoInit, 0, N_EXT | N_TEXT},
    {{NULL},"_mathExpFunc", (char*) &mathExpFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathExpfFunc", (char*) &mathExpfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathFabsFunc", (char*) &mathFabsFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathFabsfFunc", (char*) &mathFabsfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathFiles", (char*) &mathFiles, 0, N_EXT | N_DATA},
    {{NULL},"_mathFloorFunc", (char*) &mathFloorFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathFloorfFunc", (char*) &mathFloorfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathFmodFunc", (char*) &mathFmodFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathFmodfFunc", (char*) &mathFmodfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathHardAcos", (char*) mathHardAcos, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardAsin", (char*) mathHardAsin, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardAtan", (char*) mathHardAtan, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardAtan2", (char*) mathHardAtan2, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardCeil", (char*) mathHardCeil, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardCos", (char*) mathHardCos, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardCosh", (char*) mathHardCosh, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardExp", (char*) mathHardExp, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardFabs", (char*) mathHardFabs, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardFloor", (char*) mathHardFloor, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardFmod", (char*) mathHardFmod, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardInfinity", (char*) mathHardInfinity, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardInit", (char*) mathHardInit, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardIrint", (char*) mathHardIrint, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardIround", (char*) mathHardIround, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardLog", (char*) mathHardLog, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardLog10", (char*) mathHardLog10, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardLog2", (char*) mathHardLog2, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardPow", (char*) mathHardPow, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardRound", (char*) mathHardRound, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardSin", (char*) mathHardSin, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardSincos", (char*) mathHardSincos, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardSinh", (char*) mathHardSinh, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardSqrt", (char*) mathHardSqrt, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardTan", (char*) mathHardTan, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardTanh", (char*) mathHardTanh, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHardTrunc", (char*) mathHardTrunc, 0, N_EXT | N_TEXT},
    {{NULL},"_mathHypotFunc", (char*) &mathHypotFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathHypotfFunc", (char*) &mathHypotfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathInfinityFunc", (char*) &mathInfinityFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathInfinityfFunc", (char*) &mathInfinityfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathIrintFunc", (char*) &mathIrintFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathIrintfFunc", (char*) &mathIrintfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathIroundFunc", (char*) &mathIroundFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathIroundfFunc", (char*) &mathIroundfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathLog10Func", (char*) &mathLog10Func, 0, N_EXT | N_DATA},
    {{NULL},"_mathLog10fFunc", (char*) &mathLog10fFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathLog2Func", (char*) &mathLog2Func, 0, N_EXT | N_DATA},
    {{NULL},"_mathLog2fFunc", (char*) &mathLog2fFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathLogFunc", (char*) &mathLogFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathLogfFunc", (char*) &mathLogfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathPowFunc", (char*) &mathPowFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathPowfFunc", (char*) &mathPowfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathRoundFunc", (char*) &mathRoundFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathRoundfFunc", (char*) &mathRoundfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathSinFunc", (char*) &mathSinFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathSincosFunc", (char*) &mathSincosFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathSincosfFunc", (char*) &mathSincosfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathSinfFunc", (char*) &mathSinfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathSinhFunc", (char*) &mathSinhFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathSinhfFunc", (char*) &mathSinhfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathSqrtFunc", (char*) &mathSqrtFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathSqrtfFunc", (char*) &mathSqrtfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathTanFunc", (char*) &mathTanFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathTanfFunc", (char*) &mathTanfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathTanhFunc", (char*) &mathTanhFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathTanhfFunc", (char*) &mathTanhfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathTruncFunc", (char*) &mathTruncFunc, 0, N_EXT | N_DATA},
    {{NULL},"_mathTruncfFunc", (char*) &mathTruncfFunc, 0, N_EXT | N_DATA},
    {{NULL},"_maxDrivers", (char*) &maxDrivers, 0, N_EXT | N_BSS},
    {{NULL},"_maxFiles", (char*) &maxFiles, 0, N_EXT | N_BSS},
    {{NULL},"_mbinit", (char*) mbinit, 0, N_EXT | N_TEXT},
    {{NULL},"_mblen", (char*) mblen, 0, N_EXT | N_TEXT},
    {{NULL},"_mbstat", (char*) &mbstat, 0, N_EXT | N_BSS},
    {{NULL},"_mbstowcs", (char*) mbstowcs, 0, N_EXT | N_TEXT},
    {{NULL},"_mbtowc", (char*) mbtowc, 0, N_EXT | N_TEXT},
    {{NULL},"_mbufConfig", (char*) &mbufConfig, 0, N_EXT | N_DATA},
    {{NULL},"_mbufSem", (char*) &mbufSem, 0, N_EXT | N_DATA},
    {{NULL},"_mbufShow", (char*) mbufShow, 0, N_EXT | N_TEXT},
    {{NULL},"_mclfree", (char*) &mclfree, 0, N_EXT | N_BSS},
    {{NULL},"_mclrefcnt", (char*) &mclrefcnt, 0, N_EXT | N_BSS},
    {{NULL},"_memAddToPool", (char*) memAddToPool, 0, N_EXT | N_TEXT},
    {{NULL},"_memDefaultAlignment", (char*) &memDefaultAlignment, 0, N_EXT | N_DATA},
    {{NULL},"_memFindMax", (char*) memFindMax, 0, N_EXT | N_TEXT},
    {{NULL},"_memInit", (char*) memInit, 0, N_EXT | N_TEXT},
    {{NULL},"_memLibInit", (char*) memLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_memOptionsSet", (char*) memOptionsSet, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartAddToPool", (char*) memPartAddToPool, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartAlignedAlloc", (char*) memPartAlignedAlloc, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartAlloc", (char*) memPartAlloc, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartAllocErrorRtn", (char*) &memPartAllocErrorRtn, 0, N_EXT | N_DATA},
    {{NULL},"_memPartBlockErrorRtn", (char*) &memPartBlockErrorRtn, 0, N_EXT | N_DATA},
    {{NULL},"_memPartBlockIsValid", (char*) memPartBlockIsValid, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartClassId", (char*) &memPartClassId, 0, N_EXT | N_DATA},
    {{NULL},"_memPartCreate", (char*) memPartCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartFindMax", (char*) memPartFindMax, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartFree", (char*) memPartFree, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartInfoGet", (char*) memPartInfoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartInit", (char*) memPartInit, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartLibInit", (char*) memPartLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartOptionsDefault", (char*) &memPartOptionsDefault, 0, N_EXT | N_DATA},
    {{NULL},"_memPartOptionsSet", (char*) memPartOptionsSet, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartRealloc", (char*) memPartRealloc, 0, N_EXT | N_TEXT},
    {{NULL},"_memPartSemInitRtn", (char*) &memPartSemInitRtn, 0, N_EXT | N_DATA},
    {{NULL},"_memPartShow", (char*) memPartShow, 0, N_EXT | N_TEXT},
    {{NULL},"_memShow", (char*) memShow, 0, N_EXT | N_TEXT},
    {{NULL},"_memShowInit", (char*) memShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_memSysPartId", (char*) &memSysPartId, 0, N_EXT | N_DATA},
    {{NULL},"_memalign", (char*) memalign, 0, N_EXT | N_TEXT},
    {{NULL},"_memchr", (char*) memchr, 0, N_EXT | N_TEXT},
    {{NULL},"_memcmp", (char*) memcmp, 0, N_EXT | N_TEXT},
    {{NULL},"_memcpy", (char*) memcpy, 0, N_EXT | N_TEXT},
    {{NULL},"_memmove", (char*) memmove, 0, N_EXT | N_TEXT},
    {{NULL},"_memset", (char*) memset, 0, N_EXT | N_TEXT},
    {{NULL},"_mfree", (char*) &mfree, 0, N_EXT | N_BSS},
    {{NULL},"_mkdir", (char*) mkdir, 0, N_EXT | N_TEXT},
    {{NULL},"_mktime", (char*) mktime, 0, N_EXT | N_TEXT},
    {{NULL},"_mmu40LibInit", (char*) mmu40LibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_mmuLibFuncs", (char*) &mmuLibFuncs, 0, N_EXT | N_BSS},
    {{NULL},"_mmuNumPagesInFreeList", (char*) &mmuNumPagesInFreeList, 0, N_EXT | N_DATA},
    {{NULL},"_mmuPageBlockSize", (char*) &mmuPageBlockSize, 0, N_EXT | N_BSS},
    {{NULL},"_mmuPageSize", (char*) &mmuPageSize, 0, N_EXT | N_BSS},
    {{NULL},"_mmuPageSource", (char*) &mmuPageSource, 0, N_EXT | N_DATA},
    {{NULL},"_mmuPhysAddrShift", (char*) &mmuPhysAddrShift, 0, N_EXT | N_DATA},
    {{NULL},"_mmuStateTransArray", (char*) &mmuStateTransArray, 0, N_EXT | N_BSS},
    {{NULL},"_mmuStateTransArraySize", (char*) &mmuStateTransArraySize, 0, N_EXT | N_BSS},
    {{NULL},"_modf", (char*) modf, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleCheck", (char*) moduleCheck, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleClassId", (char*) &moduleClassId, 0, N_EXT | N_DATA},
    {{NULL},"_moduleCreate", (char*) moduleCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleCreateHookAdd", (char*) moduleCreateHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleCreateHookDelete", (char*) moduleCreateHookDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleDelete", (char*) moduleDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleEach", (char*) moduleEach, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleFindByGroup", (char*) moduleFindByGroup, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleFindByName", (char*) moduleFindByName, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleFindByNameAndPath", (char*) moduleFindByNameAndPath, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleFlagsGet", (char*) moduleFlagsGet, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleIdFigure", (char*) moduleIdFigure, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleIdListGet", (char*) moduleIdListGet, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleInfoGet", (char*) moduleInfoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleInit", (char*) moduleInit, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleLibInit", (char*) moduleLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleNameGet", (char*) moduleNameGet, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleSegAdd", (char*) moduleSegAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleSegEach", (char*) moduleSegEach, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleSegFirst", (char*) moduleSegFirst, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleSegGet", (char*) moduleSegGet, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleSegNext", (char*) moduleSegNext, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleShow", (char*) moduleShow, 0, N_EXT | N_TEXT},
    {{NULL},"_moduleTerminate", (char*) moduleTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQClassId", (char*) &msgQClassId, 0, N_EXT | N_DATA},
    {{NULL},"_msgQCreate", (char*) msgQCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQDelete", (char*) msgQDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQInfoGet", (char*) msgQInfoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQInit", (char*) msgQInit, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQLibInit", (char*) msgQLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQNumMsgs", (char*) msgQNumMsgs, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQReceive", (char*) msgQReceive, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQSend", (char*) msgQSend, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQShow", (char*) msgQShow, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQShowInit", (char*) msgQShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_msgQSmInfoGetRtn", (char*) &msgQSmInfoGetRtn, 0, N_EXT | N_BSS},
    {{NULL},"_msgQSmNumMsgsRtn", (char*) &msgQSmNumMsgsRtn, 0, N_EXT | N_BSS},
    {{NULL},"_msgQSmReceiveRtn", (char*) &msgQSmReceiveRtn, 0, N_EXT | N_BSS},
    {{NULL},"_msgQSmSendRtn", (char*) &msgQSmSendRtn, 0, N_EXT | N_BSS},
    {{NULL},"_msgQSmShowRtn", (char*) &msgQSmShowRtn, 0, N_EXT | N_BSS},
    {{NULL},"_msgQTerminate", (char*) msgQTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_mutexOptionsFtpdLib", (char*) &mutexOptionsFtpdLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsHostLib", (char*) &mutexOptionsHostLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsIosLib", (char*) &mutexOptionsIosLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsLogLib", (char*) &mutexOptionsLogLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsMemLib", (char*) &mutexOptionsMemLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsNetDrv", (char*) &mutexOptionsNetDrv, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsSelectLib", (char*) &mutexOptionsSelectLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsSymLib", (char*) &mutexOptionsSymLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsTyLib", (char*) &mutexOptionsTyLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsUnixLib", (char*) &mutexOptionsUnixLib, 0, N_EXT | N_DATA},
    {{NULL},"_mutexOptionsVmBaseLib", (char*) &mutexOptionsVmBaseLib, 0, N_EXT | N_DATA},
    {{NULL},"_namelessPrefix", (char*) &namelessPrefix, 0, N_EXT | N_DATA},
    {{NULL},"_netDevCreate", (char*) netDevCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_netDrv", (char*) netDrv, 0, N_EXT | N_TEXT},
    {{NULL},"_netErrnoSet", (char*) netErrnoSet, 0, N_EXT | N_TEXT},
    {{NULL},"_netHelp", (char*) netHelp, 0, N_EXT | N_TEXT},
    {{NULL},"_netJobAdd", (char*) netJobAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_netLibInit", (char*) netLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_netLsByName", (char*) netLsByName, 0, N_EXT | N_TEXT},
    {{NULL},"_netLsStr", (char*) &netLsStr, 0, N_EXT | N_DATA},
    {{NULL},"_netShowInit", (char*) netShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_netTask", (char*) netTask, 0, N_EXT | N_TEXT},
    {{NULL},"_netTaskId", (char*) &netTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_netTaskOptions", (char*) &netTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_netTaskPriority", (char*) &netTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_netTaskSemId", (char*) &netTaskSemId, 0, N_EXT | N_DATA},
    {{NULL},"_netTaskStackSize", (char*) &netTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_netTypeAdd", (char*) netTypeAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_netTypeDelete", (char*) netTypeDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_netTypeInit", (char*) netTypeInit, 0, N_EXT | N_TEXT},
    {{NULL},"_null_hash", (char*) null_hash, 0, N_EXT | N_TEXT},
    {{NULL},"_null_init", (char*) null_init, 0, N_EXT | N_TEXT},
    {{NULL},"_null_netmatch", (char*) null_netmatch, 0, N_EXT | N_TEXT},
    {{NULL},"_objAlloc", (char*) objAlloc, 0, N_EXT | N_TEXT},
    {{NULL},"_objAllocExtra", (char*) objAllocExtra, 0, N_EXT | N_TEXT},
    {{NULL},"_objCoreInit", (char*) objCoreInit, 0, N_EXT | N_TEXT},
    {{NULL},"_objCoreTerminate", (char*) objCoreTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_objFree", (char*) objFree, 0, N_EXT | N_TEXT},
    {{NULL},"_objShow", (char*) objShow, 0, N_EXT | N_TEXT},
    {{NULL},"_open", (char*) open, 0, N_EXT | N_TEXT},
    {{NULL},"_opendir", (char*) opendir, 0, N_EXT | N_TEXT},
    {{NULL},"_pEvtDblBuffers", (char*) &pEvtDblBuffers, 0, N_EXT | N_BSS},
    {{NULL},"_pFppTaskIdPrevious", (char*) &pFppTaskIdPrevious, 0, N_EXT | N_BSS},
    {{NULL},"_pJobPool", (char*) &pJobPool, 0, N_EXT | N_BSS},
    {{NULL},"_pRootMemStart", (char*) &pRootMemStart, 0, N_EXT | N_BSS},
    {{NULL},"_pScrPad", (char*) &pScrPad, 0, N_EXT | N_BSS},
    {{NULL},"_pScrPadIndex", (char*) &pScrPadIndex, 0, N_EXT | N_BSS},
    {{NULL},"_pTaskLastFpTcb", (char*) &pTaskLastFpTcb, 0, N_EXT | N_DATA},
    {{NULL},"_panic", (char*) panic, 0, N_EXT | N_TEXT},
    {{NULL},"_panicSuspend", (char*) &panicSuspend, 0, N_EXT | N_BSS},
    {{NULL},"_pathBuild", (char*) pathBuild, 0, N_EXT | N_TEXT},
    {{NULL},"_pathCat", (char*) pathCat, 0, N_EXT | N_TEXT},
    {{NULL},"_pathCondense", (char*) pathCondense, 0, N_EXT | N_TEXT},
    {{NULL},"_pathLastName", (char*) pathLastName, 0, N_EXT | N_TEXT},
    {{NULL},"_pathLastNamePtr", (char*) pathLastNamePtr, 0, N_EXT | N_TEXT},
    {{NULL},"_pathParse", (char*) pathParse, 0, N_EXT | N_TEXT},
    {{NULL},"_pathSplit", (char*) pathSplit, 0, N_EXT | N_TEXT},
    {{NULL},"_pause", (char*) pause, 0, N_EXT | N_TEXT},
    {{NULL},"_pc", (char*) pc, 0, N_EXT | N_TEXT},
    {{NULL},"_period", (char*) period, 0, N_EXT | N_TEXT},
    {{NULL},"_periodRun", (char*) periodRun, 0, N_EXT | N_TEXT},
    {{NULL},"_perror", (char*) perror, 0, N_EXT | N_TEXT},
    {{NULL},"_pfctlinput", (char*) pfctlinput, 0, N_EXT | N_TEXT},
    {{NULL},"_pffasttimo", (char*) pffasttimo, 0, N_EXT | N_TEXT},
    {{NULL},"_pffindproto", (char*) pffindproto, 0, N_EXT | N_TEXT},
    {{NULL},"_pffindtype", (char*) pffindtype, 0, N_EXT | N_TEXT},
    {{NULL},"_pfslowtimo", (char*) pfslowtimo, 0, N_EXT | N_TEXT},
    {{NULL},"_pipeDevCreate", (char*) pipeDevCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_pipeDrv", (char*) pipeDrv, 0, N_EXT | N_TEXT},
    {{NULL},"_pipeMsgQOptions", (char*) &pipeMsgQOptions, 0, N_EXT | N_DATA},
    {{NULL},"_pow", (char*) pow, 0, N_EXT | N_TEXT},
    {{NULL},"_pow_p", (char*) pow_p, 0, N_EXT | N_TEXT},
    {{NULL},"_powf", (char*) powf, 0, N_EXT | N_TEXT},
    {{NULL},"_ppGlobalEnviron", (char*) &ppGlobalEnviron, 0, N_EXT | N_BSS},
    {{NULL},"_printErr", (char*) printErr, 0, N_EXT | N_TEXT},
    {{NULL},"_printErrno", (char*) printErrno, 0, N_EXT | N_TEXT},
    {{NULL},"_printExc", (char*) printExc, 0, N_EXT | N_TEXT},
    {{NULL},"_printLogo", (char*) printLogo, 0, N_EXT | N_TEXT},
    {{NULL},"_printf", (char*) printf, 0, N_EXT | N_TEXT},
    {{NULL},"_proxyArpHook", (char*) &proxyArpHook, 0, N_EXT | N_DATA},
    {{NULL},"_proxyBroadcastHook", (char*) &proxyBroadcastHook, 0, N_EXT | N_DATA},
    {{NULL},"_ptyDevCreate", (char*) ptyDevCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_ptyDrv", (char*) ptyDrv, 0, N_EXT | N_TEXT},
    {{NULL},"_putc", (char*) putc, 0, N_EXT | N_TEXT},
    {{NULL},"_putchar", (char*) putchar, 0, N_EXT | N_TEXT},
    {{NULL},"_putenv", (char*) putenv, 0, N_EXT | N_TEXT},
    {{NULL},"_puts", (char*) puts, 0, N_EXT | N_TEXT},
    {{NULL},"_putw", (char*) putw, 0, N_EXT | N_TEXT},
    {{NULL},"_pwd", (char*) pwd, 0, N_EXT | N_TEXT},
    {{NULL},"_qAdvance", (char*) qAdvance, 0, N_EXT | N_TEXT},
    {{NULL},"_qCalibrate", (char*) qCalibrate, 0, N_EXT | N_TEXT},
    {{NULL},"_qCreate", (char*) qCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_qDelete", (char*) qDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_qEach", (char*) qEach, 0, N_EXT | N_TEXT},
    {{NULL},"_qFifoClassId", (char*) &qFifoClassId, 0, N_EXT | N_DATA},
    {{NULL},"_qFifoCreate", (char*) qFifoCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_qFifoDelete", (char*) qFifoDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_qFifoEach", (char*) qFifoEach, 0, N_EXT | N_TEXT},
    {{NULL},"_qFifoGet", (char*) qFifoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_qFifoInfo", (char*) qFifoInfo, 0, N_EXT | N_TEXT},
    {{NULL},"_qFifoInit", (char*) qFifoInit, 0, N_EXT | N_TEXT},
    {{NULL},"_qFifoPut", (char*) qFifoPut, 0, N_EXT | N_TEXT},
    {{NULL},"_qFifoRemove", (char*) qFifoRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_qFirst", (char*) qFirst, 0, N_EXT | N_TEXT},
    {{NULL},"_qGet", (char*) qGet, 0, N_EXT | N_TEXT},
    {{NULL},"_qGetExpired", (char*) qGetExpired, 0, N_EXT | N_TEXT},
    {{NULL},"_qInfo", (char*) qInfo, 0, N_EXT | N_TEXT},
    {{NULL},"_qInit", (char*) qInit, 0, N_EXT | N_TEXT},
    {{NULL},"_qJobClassId", (char*) &qJobClassId, 0, N_EXT | N_DATA},
    {{NULL},"_qJobCreate", (char*) qJobCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_qJobDelete", (char*) qJobDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_qJobEach", (char*) qJobEach, 0, N_EXT | N_TEXT},
    {{NULL},"_qJobGet", (char*) qJobGet, 0, N_EXT | N_TEXT},
    {{NULL},"_qJobInfo", (char*) qJobInfo, 0, N_EXT | N_TEXT},
    {{NULL},"_qJobInit", (char*) qJobInit, 0, N_EXT | N_TEXT},
    {{NULL},"_qJobPut", (char*) qJobPut, 0, N_EXT | N_TEXT},
    {{NULL},"_qJobTerminate", (char*) qJobTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_qKey", (char*) qKey, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapClassId", (char*) &qPriBMapClassId, 0, N_EXT | N_DATA},
    {{NULL},"_qPriBMapCreate", (char*) qPriBMapCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapDelete", (char*) qPriBMapDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapEach", (char*) qPriBMapEach, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapGet", (char*) qPriBMapGet, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapInfo", (char*) qPriBMapInfo, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapInit", (char*) qPriBMapInit, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapKey", (char*) qPriBMapKey, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapListCreate", (char*) qPriBMapListCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapListDelete", (char*) qPriBMapListDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapPut", (char*) qPriBMapPut, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapRemove", (char*) qPriBMapRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriBMapResort", (char*) qPriBMapResort, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListAdvance", (char*) qPriListAdvance, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListCalibrate", (char*) qPriListCalibrate, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListClassId", (char*) &qPriListClassId, 0, N_EXT | N_DATA},
    {{NULL},"_qPriListCreate", (char*) qPriListCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListDelete", (char*) qPriListDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListEach", (char*) qPriListEach, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListFromTailClassId", (char*) &qPriListFromTailClassId, 0, N_EXT | N_DATA},
    {{NULL},"_qPriListGet", (char*) qPriListGet, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListGetExpired", (char*) qPriListGetExpired, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListInfo", (char*) qPriListInfo, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListInit", (char*) qPriListInit, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListKey", (char*) qPriListKey, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListPut", (char*) qPriListPut, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListPutFromTail", (char*) qPriListPutFromTail, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListRemove", (char*) qPriListRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListResort", (char*) qPriListResort, 0, N_EXT | N_TEXT},
    {{NULL},"_qPriListTerminate", (char*) qPriListTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_qPut", (char*) qPut, 0, N_EXT | N_TEXT},
    {{NULL},"_qRemove", (char*) qRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_qResort", (char*) qResort, 0, N_EXT | N_TEXT},
    {{NULL},"_qTerminate", (char*) qTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_qsort", (char*) qsort, 0, N_EXT | N_TEXT},
    {{NULL},"_raise", (char*) raise, 0, N_EXT | N_TEXT},
    {{NULL},"_rand", (char*) rand, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_attach", (char*) raw_attach, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_bind", (char*) raw_bind, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_connaddr", (char*) raw_connaddr, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_ctlinput", (char*) raw_ctlinput, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_detach", (char*) raw_detach, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_disconnect", (char*) raw_disconnect, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_init", (char*) raw_init, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_input", (char*) raw_input, 0, N_EXT | N_TEXT},
    {{NULL},"_raw_recvspace", (char*) &raw_recvspace, 0, N_EXT | N_DATA},
    {{NULL},"_raw_sendspace", (char*) &raw_sendspace, 0, N_EXT | N_DATA},
    {{NULL},"_raw_usrreq", (char*) raw_usrreq, 0, N_EXT | N_TEXT},
    {{NULL},"_rawcb", (char*) &rawcb, 0, N_EXT | N_BSS},
    {{NULL},"_rawintr", (char*) rawintr, 0, N_EXT | N_TEXT},
    {{NULL},"_rawintrq", (char*) &rawintrq, 0, N_EXT | N_BSS},
    {{NULL},"_rcmd", (char*) rcmd, 0, N_EXT | N_TEXT},
    {{NULL},"_read", (char*) read, 0, N_EXT | N_TEXT},
    {{NULL},"_readdir", (char*) readdir, 0, N_EXT | N_TEXT},
    {{NULL},"_readv", (char*) readv, 0, N_EXT | N_TEXT},
    {{NULL},"_readyQBMap", (char*) &readyQBMap, 0, N_EXT | N_BSS},
    {{NULL},"_readyQHead", (char*) &readyQHead, 0, N_EXT | N_BSS},
    {{NULL},"_realloc", (char*) realloc, 0, N_EXT | N_TEXT},
    {{NULL},"_reboot", (char*) reboot, 0, N_EXT | N_TEXT},
    {{NULL},"_rebootHookAdd", (char*) rebootHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_recv", (char*) recv, 0, N_EXT | N_TEXT},
    {{NULL},"_recvfrom", (char*) recvfrom, 0, N_EXT | N_TEXT},
    {{NULL},"_recvmsg", (char*) recvmsg, 0, N_EXT | N_TEXT},
    {{NULL},"_redirInFd", (char*) &redirInFd, 0, N_EXT | N_BSS},
    {{NULL},"_redirOutFd", (char*) &redirOutFd, 0, N_EXT | N_BSS},
    {{NULL},"_reld", (char*) reld, 0, N_EXT | N_TEXT},
    {{NULL},"_remCurIdGet", (char*) remCurIdGet, 0, N_EXT | N_TEXT},
    {{NULL},"_remCurIdSet", (char*) remCurIdSet, 0, N_EXT | N_TEXT},
    {{NULL},"_remLastResvPort", (char*) &remLastResvPort, 0, N_EXT | N_DATA},
    {{NULL},"_remove", (char*) remove, 0, N_EXT | N_TEXT},
    {{NULL},"_rename", (char*) rename, 0, N_EXT | N_TEXT},
    {{NULL},"_repeat", (char*) repeat, 0, N_EXT | N_TEXT},
    {{NULL},"_repeatRun", (char*) repeatRun, 0, N_EXT | N_TEXT},
    {{NULL},"_reschedule", (char*) reschedule, 0, N_EXT | N_TEXT},
    {{NULL},"_restartTaskName", (char*) &restartTaskName, 0, N_EXT | N_DATA},
    {{NULL},"_restartTaskOptions", (char*) &restartTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_restartTaskPriority", (char*) &restartTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_restartTaskStackSize", (char*) &restartTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_rewind", (char*) rewind, 0, N_EXT | N_TEXT},
    {{NULL},"_rewinddir", (char*) rewinddir, 0, N_EXT | N_TEXT},
    {{NULL},"_rindex", (char*) rindex, 0, N_EXT | N_TEXT},
    {{NULL},"_rip_ctloutput", (char*) rip_ctloutput, 0, N_EXT | N_TEXT},
    {{NULL},"_rip_input", (char*) rip_input, 0, N_EXT | N_TEXT},
    {{NULL},"_rip_output", (char*) rip_output, 0, N_EXT | N_TEXT},
    {{NULL},"_ripdst", (char*) &ripdst, 0, N_EXT | N_DATA},
    {{NULL},"_ripproto", (char*) &ripproto, 0, N_EXT | N_DATA},
    {{NULL},"_ripsrc", (char*) &ripsrc, 0, N_EXT | N_DATA},
    {{NULL},"_rlogChildTask", (char*) rlogChildTask, 0, N_EXT | N_TEXT},
    {{NULL},"_rlogChildTaskId", (char*) &rlogChildTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_rlogInTask", (char*) rlogInTask, 0, N_EXT | N_TEXT},
    {{NULL},"_rlogInTaskId", (char*) &rlogInTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_rlogInit", (char*) rlogInit, 0, N_EXT | N_TEXT},
    {{NULL},"_rlogOutTask", (char*) rlogOutTask, 0, N_EXT | N_TEXT},
    {{NULL},"_rlogOutTaskId", (char*) &rlogOutTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_rlogShellName", (char*) &rlogShellName, 0, N_EXT | N_DATA},
    {{NULL},"_rlogTaskOptions", (char*) &rlogTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_rlogTaskPriority", (char*) &rlogTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_rlogTaskStackSize", (char*) &rlogTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_rlogTermType", (char*) &rlogTermType, 0, N_EXT | N_DATA},
    {{NULL},"_rlogin", (char*) rlogin, 0, N_EXT | N_TEXT},
    {{NULL},"_rlogind", (char*) rlogind, 0, N_EXT | N_TEXT},
    {{NULL},"_rlogindId", (char*) &rlogindId, 0, N_EXT | N_BSS},
    {{NULL},"_rlogindSocket", (char*) &rlogindSocket, 0, N_EXT | N_BSS},
    {{NULL},"_rm", (char*) rm, 0, N_EXT | N_TEXT},
    {{NULL},"_rmdir", (char*) rmdir, 0, N_EXT | N_TEXT},
    {{NULL},"_rngBufGet", (char*) rngBufGet, 0, N_EXT | N_TEXT},
    {{NULL},"_rngBufPut", (char*) rngBufPut, 0, N_EXT | N_TEXT},
    {{NULL},"_rngCreate", (char*) rngCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_rngDelete", (char*) rngDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_rngFlush", (char*) rngFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_rngFreeBytes", (char*) rngFreeBytes, 0, N_EXT | N_TEXT},
    {{NULL},"_rngIsEmpty", (char*) rngIsEmpty, 0, N_EXT | N_TEXT},
    {{NULL},"_rngIsFull", (char*) rngIsFull, 0, N_EXT | N_TEXT},
    {{NULL},"_rngMoveAhead", (char*) rngMoveAhead, 0, N_EXT | N_TEXT},
    {{NULL},"_rngNBytes", (char*) rngNBytes, 0, N_EXT | N_TEXT},
    {{NULL},"_rngPutAhead", (char*) rngPutAhead, 0, N_EXT | N_TEXT},
    {{NULL},"_rootMemNBytes", (char*) &rootMemNBytes, 0, N_EXT | N_BSS},
    {{NULL},"_rootTaskId", (char*) &rootTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_round", (char*) round, 0, N_EXT | N_TEXT},
    {{NULL},"_roundRobinOn", (char*) &roundRobinOn, 0, N_EXT | N_BSS},
    {{NULL},"_roundRobinSlice", (char*) &roundRobinSlice, 0, N_EXT | N_BSS},
    {{NULL},"_roundf", (char*) roundf, 0, N_EXT | N_TEXT},
    {{NULL},"_routeAdd", (char*) routeAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_routeCmd", (char*) routeCmd, 0, N_EXT | N_TEXT},
    {{NULL},"_routeDelete", (char*) routeDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_routeEntryFill", (char*) routeEntryFill, 0, N_EXT | N_TEXT},
    {{NULL},"_routeNetAdd", (char*) routeNetAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_routeShow", (char*) routeShow, 0, N_EXT | N_TEXT},
    {{NULL},"_routestatShow", (char*) routestatShow, 0, N_EXT | N_TEXT},
    {{NULL},"_rresvport", (char*) rresvport, 0, N_EXT | N_TEXT},
    {{NULL},"_rtalloc", (char*) rtalloc, 0, N_EXT | N_TEXT},
    {{NULL},"_rtfree", (char*) rtfree, 0, N_EXT | N_TEXT},
    {{NULL},"_rthashsize", (char*) &rthashsize, 0, N_EXT | N_DATA},
    {{NULL},"_rthost", (char*) &rthost, 0, N_EXT | N_BSS},
    {{NULL},"_rtinit", (char*) rtinit, 0, N_EXT | N_TEXT},
    {{NULL},"_rtioctl", (char*) rtioctl, 0, N_EXT | N_TEXT},
    {{NULL},"_rtmodified", (char*) &rtmodified, 0, N_EXT | N_DATA},
    {{NULL},"_rtnet", (char*) &rtnet, 0, N_EXT | N_BSS},
    {{NULL},"_rtredirect", (char*) rtredirect, 0, N_EXT | N_TEXT},
    {{NULL},"_rtrequest", (char*) rtrequest, 0, N_EXT | N_TEXT},
    {{NULL},"_rtstat", (char*) &rtstat, 0, N_EXT | N_BSS},
    {{NULL},"_rttrash", (char*) &rttrash, 0, N_EXT | N_BSS},
    {{NULL},"_s", (char*) s, 0, N_EXT | N_TEXT},
    {{NULL},"_save_rte", (char*) save_rte, 0, N_EXT | N_TEXT},
    {{NULL},"_sbappend", (char*) sbappend, 0, N_EXT | N_TEXT},
    {{NULL},"_sbappendaddr", (char*) sbappendaddr, 0, N_EXT | N_TEXT},
    {{NULL},"_sbappendrecord", (char*) sbappendrecord, 0, N_EXT | N_TEXT},
    {{NULL},"_sbappendrights", (char*) sbappendrights, 0, N_EXT | N_TEXT},
    {{NULL},"_sbcompress", (char*) sbcompress, 0, N_EXT | N_TEXT},
    {{NULL},"_sbdrop", (char*) sbdrop, 0, N_EXT | N_TEXT},
    {{NULL},"_sbdroprecord", (char*) sbdroprecord, 0, N_EXT | N_TEXT},
    {{NULL},"_sbflush", (char*) sbflush, 0, N_EXT | N_TEXT},
    {{NULL},"_sbrelease", (char*) sbrelease, 0, N_EXT | N_TEXT},
    {{NULL},"_sbreserve", (char*) sbreserve, 0, N_EXT | N_TEXT},
    {{NULL},"_sbseldequeue", (char*) sbseldequeue, 0, N_EXT | N_TEXT},
    {{NULL},"_sbselqueue", (char*) sbselqueue, 0, N_EXT | N_TEXT},
    {{NULL},"_sbwait", (char*) sbwait, 0, N_EXT | N_TEXT},
    {{NULL},"_sbwakeup", (char*) sbwakeup, 0, N_EXT | N_TEXT},
    {{NULL},"_scalb", (char*) scalb, 0, N_EXT | N_TEXT},
    {{NULL},"_scanCharSet", (char*) scanCharSet, 0, N_EXT | N_TEXT},
    {{NULL},"_scanField", (char*) scanField, 0, N_EXT | N_TEXT},
    {{NULL},"_scanf", (char*) scanf, 0, N_EXT | N_TEXT},
    {{NULL},"_schednetisr", (char*) schednetisr, 0, N_EXT | N_TEXT},
    {{NULL},"_selNodeAdd", (char*) selNodeAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_selNodeDelete", (char*) selNodeDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_selWakeup", (char*) selWakeup, 0, N_EXT | N_TEXT},
    {{NULL},"_selWakeupAll", (char*) selWakeupAll, 0, N_EXT | N_TEXT},
    {{NULL},"_selWakeupListInit", (char*) selWakeupListInit, 0, N_EXT | N_TEXT},
    {{NULL},"_selWakeupListLen", (char*) selWakeupListLen, 0, N_EXT | N_TEXT},
    {{NULL},"_selWakeupType", (char*) selWakeupType, 0, N_EXT | N_TEXT},
    {{NULL},"_select", (char*) select, 0, N_EXT | N_TEXT},
    {{NULL},"_selectInit", (char*) selectInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semBCoreInit", (char*) semBCoreInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semBCreate", (char*) semBCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_semBGive", (char*) semBGive, 0, N_EXT | N_TEXT},
    {{NULL},"_semBGiveDefer", (char*) semBGiveDefer, 0, N_EXT | N_TEXT},
    {{NULL},"_semBInit", (char*) semBInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semBLibInit", (char*) semBLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semBTake", (char*) semBTake, 0, N_EXT | N_TEXT},
    {{NULL},"_semCCoreInit", (char*) semCCoreInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semCCreate", (char*) semCCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_semCGive", (char*) semCGive, 0, N_EXT | N_TEXT},
    {{NULL},"_semCGiveDefer", (char*) semCGiveDefer, 0, N_EXT | N_TEXT},
    {{NULL},"_semCInit", (char*) semCInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semCLibInit", (char*) semCLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semCTake", (char*) semCTake, 0, N_EXT | N_TEXT},
    {{NULL},"_semClass", (char*) &semClass, 0, N_EXT | N_BSS},
    {{NULL},"_semClassId", (char*) &semClassId, 0, N_EXT | N_DATA},
    {{NULL},"_semClear", (char*) semClear, 0, N_EXT | N_TEXT},
    {{NULL},"_semDelete", (char*) semDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_semDestroy", (char*) semDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_semFlush", (char*) semFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_semFlushDefer", (char*) semFlushDefer, 0, N_EXT | N_TEXT},
    {{NULL},"_semFlushDeferTbl", (char*) &semFlushDeferTbl, 0, N_EXT | N_DATA},
    {{NULL},"_semFlushTbl", (char*) &semFlushTbl, 0, N_EXT | N_DATA},
    {{NULL},"_semGive", (char*) semGive, 0, N_EXT | N_TEXT},
    {{NULL},"_semGiveDefer", (char*) semGiveDefer, 0, N_EXT | N_TEXT},
    {{NULL},"_semGiveDeferTbl", (char*) &semGiveDeferTbl, 0, N_EXT | N_DATA},
    {{NULL},"_semGiveTbl", (char*) &semGiveTbl, 0, N_EXT | N_DATA},
    {{NULL},"_semInfo", (char*) semInfo, 0, N_EXT | N_TEXT},
    {{NULL},"_semIntRestrict", (char*) semIntRestrict, 0, N_EXT | N_TEXT},
    {{NULL},"_semInvalid", (char*) semInvalid, 0, N_EXT | N_TEXT},
    {{NULL},"_semLibInit", (char*) semLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semMCoreInit", (char*) semMCoreInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semMCreate", (char*) semMCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_semMGive", (char*) semMGive, 0, N_EXT | N_TEXT},
    {{NULL},"_semMGiveForce", (char*) semMGiveForce, 0, N_EXT | N_TEXT},
    {{NULL},"_semMGiveKern", (char*) semMGiveKern, 0, N_EXT | N_TEXT},
    {{NULL},"_semMGiveKernWork", (char*) &semMGiveKernWork, 0, N_EXT | N_BSS},
    {{NULL},"_semMInit", (char*) semMInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semMLibInit", (char*) semMLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semMPendQPut", (char*) semMPendQPut, 0, N_EXT | N_TEXT},
    {{NULL},"_semMTake", (char*) semMTake, 0, N_EXT | N_TEXT},
    {{NULL},"_semOTake", (char*) semOTake, 0, N_EXT | N_TEXT},
    {{NULL},"_semQFlush", (char*) semQFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_semQFlushDefer", (char*) semQFlushDefer, 0, N_EXT | N_TEXT},
    {{NULL},"_semQGet", (char*) semQGet, 0, N_EXT | N_TEXT},
    {{NULL},"_semQInit", (char*) semQInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semQPut", (char*) semQPut, 0, N_EXT | N_TEXT},
    {{NULL},"_semShow", (char*) semShow, 0, N_EXT | N_TEXT},
    {{NULL},"_semShowInit", (char*) semShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_semSmInfoRtn", (char*) &semSmInfoRtn, 0, N_EXT | N_BSS},
    {{NULL},"_semSmShowRtn", (char*) &semSmShowRtn, 0, N_EXT | N_BSS},
    {{NULL},"_semTake", (char*) semTake, 0, N_EXT | N_TEXT},
    {{NULL},"_semTakeTbl", (char*) &semTakeTbl, 0, N_EXT | N_DATA},
    {{NULL},"_semTerminate", (char*) semTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_send", (char*) send, 0, N_EXT | N_TEXT},
    {{NULL},"_sendmsg", (char*) sendmsg, 0, N_EXT | N_TEXT},
    {{NULL},"_sendto", (char*) sendto, 0, N_EXT | N_TEXT},
    {{NULL},"_set_if_addr", (char*) set_if_addr, 0, N_EXT | N_TEXT},
    {{NULL},"_setbuf", (char*) setbuf, 0, N_EXT | N_TEXT},
    {{NULL},"_setbuffer", (char*) setbuffer, 0, N_EXT | N_TEXT},
    {{NULL},"_sethostname", (char*) sethostname, 0, N_EXT | N_TEXT},
    {{NULL},"_setjmp", (char*) setjmp, 0, N_EXT | N_TEXT},
    {{NULL},"_setlinebuf", (char*) setlinebuf, 0, N_EXT | N_TEXT},
    {{NULL},"_setlocale", (char*) setlocale, 0, N_EXT | N_TEXT},
    {{NULL},"_setsockopt", (char*) setsockopt, 0, N_EXT | N_TEXT},
    {{NULL},"_setvbuf", (char*) setvbuf, 0, N_EXT | N_TEXT},
    {{NULL},"_shell", (char*) shell, 0, N_EXT | N_TEXT},
    {{NULL},"_shellHistSize", (char*) &shellHistSize, 0, N_EXT | N_DATA},
    {{NULL},"_shellHistory", (char*) shellHistory, 0, N_EXT | N_TEXT},
    {{NULL},"_shellInit", (char*) shellInit, 0, N_EXT | N_TEXT},
    {{NULL},"_shellLock", (char*) shellLock, 0, N_EXT | N_TEXT},
    {{NULL},"_shellLoginInstall", (char*) shellLoginInstall, 0, N_EXT | N_TEXT},
    {{NULL},"_shellLogout", (char*) shellLogout, 0, N_EXT | N_TEXT},
    {{NULL},"_shellLogoutInstall", (char*) shellLogoutInstall, 0, N_EXT | N_TEXT},
    {{NULL},"_shellOrigStdSet", (char*) shellOrigStdSet, 0, N_EXT | N_TEXT},
    {{NULL},"_shellPromptSet", (char*) shellPromptSet, 0, N_EXT | N_TEXT},
    {{NULL},"_shellRestart", (char*) shellRestart, 0, N_EXT | N_TEXT},
    {{NULL},"_shellScriptAbort", (char*) shellScriptAbort, 0, N_EXT | N_TEXT},
    {{NULL},"_shellTaskId", (char*) &shellTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_shellTaskName", (char*) &shellTaskName, 0, N_EXT | N_DATA},
    {{NULL},"_shellTaskOptions", (char*) &shellTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_shellTaskPriority", (char*) &shellTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_shellTaskStackSize", (char*) &shellTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_show", (char*) show, 0, N_EXT | N_TEXT},
    {{NULL},"_shutdown", (char*) shutdown, 0, N_EXT | N_TEXT},
    {{NULL},"_sigEvtRtn", (char*) &sigEvtRtn, 0, N_EXT | N_BSS},
    {{NULL},"_sigInit", (char*) sigInit, 0, N_EXT | N_TEXT},
    {{NULL},"_sigPendDestroy", (char*) sigPendDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_sigPendInit", (char*) sigPendInit, 0, N_EXT | N_TEXT},
    {{NULL},"_sigPendKill", (char*) sigPendKill, 0, N_EXT | N_TEXT},
    {{NULL},"_sigaction", (char*) sigaction, 0, N_EXT | N_TEXT},
    {{NULL},"_sigaddset", (char*) sigaddset, 0, N_EXT | N_TEXT},
    {{NULL},"_sigblock", (char*) sigblock, 0, N_EXT | N_TEXT},
    {{NULL},"_sigdelset", (char*) sigdelset, 0, N_EXT | N_TEXT},
    {{NULL},"_sigemptyset", (char*) sigemptyset, 0, N_EXT | N_TEXT},
    {{NULL},"_sigfillset", (char*) sigfillset, 0, N_EXT | N_TEXT},
    {{NULL},"_sigismember", (char*) sigismember, 0, N_EXT | N_TEXT},
    {{NULL},"_signal", (char*) signal, 0, N_EXT | N_TEXT},
    {{NULL},"_sigpending", (char*) sigpending, 0, N_EXT | N_TEXT},
    {{NULL},"_sigprocmask", (char*) sigprocmask, 0, N_EXT | N_TEXT},
    {{NULL},"_sigqueue", (char*) sigqueue, 0, N_EXT | N_TEXT},
    {{NULL},"_sigqueueInit", (char*) sigqueueInit, 0, N_EXT | N_TEXT},
    {{NULL},"_sigreturn", (char*) sigreturn, 0, N_EXT | N_TEXT},
    {{NULL},"_sigsetjmp", (char*) sigsetjmp, 0, N_EXT | N_TEXT},
    {{NULL},"_sigsetmask", (char*) sigsetmask, 0, N_EXT | N_TEXT},
    {{NULL},"_sigsuspend", (char*) sigsuspend, 0, N_EXT | N_TEXT},
    {{NULL},"_sigtimedwait", (char*) sigtimedwait, 0, N_EXT | N_TEXT},
    {{NULL},"_sigvec", (char*) sigvec, 0, N_EXT | N_TEXT},
    {{NULL},"_sigwaitinfo", (char*) sigwaitinfo, 0, N_EXT | N_TEXT},
    {{NULL},"_sin", (char*) sin, 0, N_EXT | N_TEXT},
    {{NULL},"_sincos", (char*) sincos, 0, N_EXT | N_TEXT},
    {{NULL},"_sincosf", (char*) sincosf, 0, N_EXT | N_TEXT},
    {{NULL},"_sinf", (char*) sinf, 0, N_EXT | N_TEXT},
    {{NULL},"_sinh", (char*) sinh, 0, N_EXT | N_TEXT},
    {{NULL},"_sinhf", (char*) sinhf, 0, N_EXT | N_TEXT},
    {{NULL},"_sllCount", (char*) sllCount, 0, N_EXT | N_TEXT},
    {{NULL},"_sllCreate", (char*) sllCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_sllDelete", (char*) sllDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_sllEach", (char*) sllEach, 0, N_EXT | N_TEXT},
    {{NULL},"_sllGet", (char*) sllGet, 0, N_EXT | N_TEXT},
    {{NULL},"_sllInit", (char*) sllInit, 0, N_EXT | N_TEXT},
    {{NULL},"_sllPrevious", (char*) sllPrevious, 0, N_EXT | N_TEXT},
    {{NULL},"_sllPutAtHead", (char*) sllPutAtHead, 0, N_EXT | N_TEXT},
    {{NULL},"_sllPutAtTail", (char*) sllPutAtTail, 0, N_EXT | N_TEXT},
    {{NULL},"_sllRemove", (char*) sllRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_sllTerminate", (char*) sllTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_smAliveTimeout", (char*) &smAliveTimeout, 0, N_EXT | N_DATA},
    {{NULL},"_smAttach", (char*) smAttach, 0, N_EXT | N_TEXT},
    {{NULL},"_smCpuInfoGet", (char*) smCpuInfoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_smCurMaxTries", (char*) &smCurMaxTries, 0, N_EXT | N_DATA},
    {{NULL},"_smDetach", (char*) smDetach, 0, N_EXT | N_TEXT},
    {{NULL},"_smIfAttach", (char*) smIfAttach, 0, N_EXT | N_TEXT},
    {{NULL},"_smIfInput", (char*) smIfInput, 0, N_EXT | N_TEXT},
    {{NULL},"_smIfLoanReturn", (char*) smIfLoanReturn, 0, N_EXT | N_TEXT},
    {{NULL},"_smIfVerbose", (char*) &smIfVerbose, 0, N_EXT | N_DATA},
    {{NULL},"_smInfoGet", (char*) smInfoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_smInit", (char*) smInit, 0, N_EXT | N_TEXT},
    {{NULL},"_smIsAlive", (char*) smIsAlive, 0, N_EXT | N_TEXT},
    {{NULL},"_smLockGive", (char*) smLockGive, 0, N_EXT | N_TEXT},
    {{NULL},"_smLockTake", (char*) smLockTake, 0, N_EXT | N_TEXT},
    {{NULL},"_smMemPartAddToPoolRtn", (char*) &smMemPartAddToPoolRtn, 0, N_EXT | N_DATA},
    {{NULL},"_smMemPartAllocRtn", (char*) &smMemPartAllocRtn, 0, N_EXT | N_DATA},
    {{NULL},"_smMemPartFindMaxRtn", (char*) &smMemPartFindMaxRtn, 0, N_EXT | N_BSS},
    {{NULL},"_smMemPartFreeRtn", (char*) &smMemPartFreeRtn, 0, N_EXT | N_DATA},
    {{NULL},"_smMemPartOptionsSetRtn", (char*) &smMemPartOptionsSetRtn, 0, N_EXT | N_BSS},
    {{NULL},"_smMemPartReallocRtn", (char*) &smMemPartReallocRtn, 0, N_EXT | N_BSS},
    {{NULL},"_smMemPartShowRtn", (char*) &smMemPartShowRtn, 0, N_EXT | N_BSS},
    {{NULL},"_smNetAttach", (char*) smNetAttach, 0, N_EXT | N_TEXT},
    {{NULL},"_smNetInetGet", (char*) smNetInetGet, 0, N_EXT | N_TEXT},
    {{NULL},"_smNetInit", (char*) smNetInit, 0, N_EXT | N_TEXT},
    {{NULL},"_smNetLoanNum", (char*) &smNetLoanNum, 0, N_EXT | N_DATA},
    {{NULL},"_smNetMaxBytesDefault", (char*) &smNetMaxBytesDefault, 0, N_EXT | N_DATA},
    {{NULL},"_smNetShow", (char*) smNetShow, 0, N_EXT | N_TEXT},
    {{NULL},"_smNetShowInit", (char*) smNetShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_smNetVerbose", (char*) &smNetVerbose, 0, N_EXT | N_DATA},
    {{NULL},"_smObjPoolMinusOne", (char*) &smObjPoolMinusOne, 0, N_EXT | N_BSS},
    {{NULL},"_smObjTaskDeleteFailRtn", (char*) &smObjTaskDeleteFailRtn, 0, N_EXT | N_BSS},
    {{NULL},"_smObjTcbFreeFailRtn", (char*) &smObjTcbFreeFailRtn, 0, N_EXT | N_BSS},
    {{NULL},"_smObjTcbFreeRtn", (char*) &smObjTcbFreeRtn, 0, N_EXT | N_BSS},
    {{NULL},"_smPktAttach", (char*) smPktAttach, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktBeat", (char*) smPktBeat, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktCpuInfoGet", (char*) smPktCpuInfoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktDetach", (char*) smPktDetach, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktFreeGet", (char*) smPktFreeGet, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktFreePut", (char*) smPktFreePut, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktInfoGet", (char*) smPktInfoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktInit", (char*) smPktInit, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktMaxBytesDefault", (char*) &smPktMaxBytesDefault, 0, N_EXT | N_DATA},
    {{NULL},"_smPktMaxCpusDefault", (char*) &smPktMaxCpusDefault, 0, N_EXT | N_DATA},
    {{NULL},"_smPktMaxInputDefault", (char*) &smPktMaxInputDefault, 0, N_EXT | N_DATA},
    {{NULL},"_smPktMemSizeDefault", (char*) &smPktMemSizeDefault, 0, N_EXT | N_DATA},
    {{NULL},"_smPktRecv", (char*) smPktRecv, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktSend", (char*) smPktSend, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktSetup", (char*) smPktSetup, 0, N_EXT | N_TEXT},
    {{NULL},"_smPktTasTries", (char*) &smPktTasTries, 0, N_EXT | N_DATA},
    {{NULL},"_smSetup", (char*) smSetup, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilDelay", (char*) smUtilDelay, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilIntConnect", (char*) smUtilIntConnect, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilIntGen", (char*) smUtilIntGen, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilMemProbe", (char*) smUtilMemProbe, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilNetRoutine", (char*) &smUtilNetRoutine, 0, N_EXT | N_DATA},
    {{NULL},"_smUtilObjRoutine", (char*) &smUtilObjRoutine, 0, N_EXT | N_DATA},
    {{NULL},"_smUtilPollTaskOptions", (char*) &smUtilPollTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_smUtilPollTaskPriority", (char*) &smUtilPollTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_smUtilPollTaskStackSize", (char*) &smUtilPollTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_smUtilProcNumGet", (char*) smUtilProcNumGet, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilRateGet", (char*) smUtilRateGet, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilSoftTas", (char*) smUtilSoftTas, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilTas", (char*) smUtilTas, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilTasChecks", (char*) &smUtilTasChecks, 0, N_EXT | N_DATA},
    {{NULL},"_smUtilTasClear", (char*) smUtilTasClear, 0, N_EXT | N_TEXT},
    {{NULL},"_smUtilTasClearRtn", (char*) &smUtilTasClearRtn, 0, N_EXT | N_DATA},
    {{NULL},"_smUtilVerbose", (char*) &smUtilVerbose, 0, N_EXT | N_DATA},
    {{NULL},"_sm_softc", (char*) &sm_softc, 0, N_EXT | N_DATA},
    {{NULL},"_so", (char*) so, 0, N_EXT | N_TEXT},
    {{NULL},"_soabort", (char*) soabort, 0, N_EXT | N_TEXT},
    {{NULL},"_soaccept", (char*) soaccept, 0, N_EXT | N_TEXT},
    {{NULL},"_sobind", (char*) sobind, 0, N_EXT | N_TEXT},
    {{NULL},"_socantrcvmore", (char*) socantrcvmore, 0, N_EXT | N_TEXT},
    {{NULL},"_socantsendmore", (char*) socantsendmore, 0, N_EXT | N_TEXT},
    {{NULL},"_sockInit", (char*) sockInit, 0, N_EXT | N_TEXT},
    {{NULL},"_socket", (char*) socket, 0, N_EXT | N_TEXT},
    {{NULL},"_soclose", (char*) soclose, 0, N_EXT | N_TEXT},
    {{NULL},"_soconnect", (char*) soconnect, 0, N_EXT | N_TEXT},
    {{NULL},"_soconnect2", (char*) soconnect2, 0, N_EXT | N_TEXT},
    {{NULL},"_socreate", (char*) socreate, 0, N_EXT | N_TEXT},
    {{NULL},"_sodisconnect", (char*) sodisconnect, 0, N_EXT | N_TEXT},
    {{NULL},"_sofree", (char*) sofree, 0, N_EXT | N_TEXT},
    {{NULL},"_sogetopt", (char*) sogetopt, 0, N_EXT | N_TEXT},
    {{NULL},"_soisconnected", (char*) soisconnected, 0, N_EXT | N_TEXT},
    {{NULL},"_soisconnecting", (char*) soisconnecting, 0, N_EXT | N_TEXT},
    {{NULL},"_soisdisconnected", (char*) soisdisconnected, 0, N_EXT | N_TEXT},
    {{NULL},"_soisdisconnecting", (char*) soisdisconnecting, 0, N_EXT | N_TEXT},
    {{NULL},"_solisten", (char*) solisten, 0, N_EXT | N_TEXT},
    {{NULL},"_sonewconn", (char*) sonewconn, 0, N_EXT | N_TEXT},
    {{NULL},"_soo_ioctl", (char*) soo_ioctl, 0, N_EXT | N_TEXT},
    {{NULL},"_soo_select", (char*) soo_select, 0, N_EXT | N_TEXT},
    {{NULL},"_soo_unselect", (char*) soo_unselect, 0, N_EXT | N_TEXT},
    {{NULL},"_soqinsque", (char*) soqinsque, 0, N_EXT | N_TEXT},
    {{NULL},"_soqremque", (char*) soqremque, 0, N_EXT | N_TEXT},
    {{NULL},"_soreceive", (char*) soreceive, 0, N_EXT | N_TEXT},
    {{NULL},"_soreserve", (char*) soreserve, 0, N_EXT | N_TEXT},
    {{NULL},"_sorflush", (char*) sorflush, 0, N_EXT | N_TEXT},
    {{NULL},"_sosend", (char*) sosend, 0, N_EXT | N_TEXT},
    {{NULL},"_sosetopt", (char*) sosetopt, 0, N_EXT | N_TEXT},
    {{NULL},"_soshutdown", (char*) soshutdown, 0, N_EXT | N_TEXT},
    {{NULL},"_sowakeup", (char*) sowakeup, 0, N_EXT | N_TEXT},
    {{NULL},"_sowakeupHook", (char*) &sowakeupHook, 0, N_EXT | N_DATA},
    {{NULL},"_sp", (char*) sp, 0, N_EXT | N_TEXT},
    {{NULL},"_spTaskOptions", (char*) &spTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_spTaskPriority", (char*) &spTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_spTaskStackSize", (char*) &spTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_splSemId", (char*) &splSemId, 0, N_EXT | N_DATA},
    {{NULL},"_splSemInit", (char*) splSemInit, 0, N_EXT | N_TEXT},
    {{NULL},"_splTid", (char*) &splTid, 0, N_EXT | N_BSS},
    {{NULL},"_splimp", (char*) splimp, 0, N_EXT | N_TEXT},
    {{NULL},"_splnet", (char*) splnet, 0, N_EXT | N_TEXT},
    {{NULL},"_splx", (char*) splx, 0, N_EXT | N_TEXT},
    {{NULL},"_sprintf", (char*) sprintf, 0, N_EXT | N_TEXT},
    {{NULL},"_spy", (char*) spy, 0, N_EXT | N_TEXT},
    {{NULL},"_spyClkStart", (char*) spyClkStart, 0, N_EXT | N_TEXT},
    {{NULL},"_spyClkStop", (char*) spyClkStop, 0, N_EXT | N_TEXT},
    {{NULL},"_spyHelp", (char*) spyHelp, 0, N_EXT | N_TEXT},
    {{NULL},"_spyReport", (char*) spyReport, 0, N_EXT | N_TEXT},
    {{NULL},"_spyStop", (char*) spyStop, 0, N_EXT | N_TEXT},
    {{NULL},"_spyTask", (char*) spyTask, 0, N_EXT | N_TEXT},
    {{NULL},"_spyTaskId", (char*) &spyTaskId, 0, N_EXT | N_DATA},
    {{NULL},"_spyTaskOptions", (char*) &spyTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_spyTaskPriority", (char*) &spyTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_spyTaskStackSize", (char*) &spyTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_sqrt", (char*) sqrt, 0, N_EXT | N_TEXT},
    {{NULL},"_sqrtf", (char*) sqrtf, 0, N_EXT | N_TEXT},
    {{NULL},"_squeeze", (char*) squeeze, 0, N_EXT | N_TEXT},
    {{NULL},"_sr", (char*) sr, 0, N_EXT | N_TEXT},
    {{NULL},"_srand", (char*) srand, 0, N_EXT | N_TEXT},
    {{NULL},"_sscanf", (char*) sscanf, 0, N_EXT | N_TEXT},
    {{NULL},"_standAloneSymTbl", (char*) &standAloneSymTbl, 0, N_EXT | N_BSS},
    {{NULL},"_stat", (char*) stat, 0, N_EXT | N_TEXT},
    {{NULL},"_statSymTbl", (char*) &statSymTbl, 0, N_EXT | N_BSS},
    {{NULL},"_statTbl", (char*) &statTbl, 0, N_EXT | N_DATA},
    {{NULL},"_statTblSize", (char*) &statTblSize, 0, N_EXT | N_DATA},
    {{NULL},"_statfs", (char*) statfs, 0, N_EXT | N_TEXT},
    {{NULL},"_stdioFiles", (char*) &stdioFiles, 0, N_EXT | N_DATA},
    {{NULL},"_stdioFp", (char*) stdioFp, 0, N_EXT | N_TEXT},
    {{NULL},"_stdioFpCreate", (char*) stdioFpCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_stdioFpDestroy", (char*) stdioFpDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_stdioInit", (char*) stdioInit, 0, N_EXT | N_TEXT},
    {{NULL},"_stdioShow", (char*) stdioShow, 0, N_EXT | N_TEXT},
    {{NULL},"_stdioShowInit", (char*) stdioShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_stdlibFiles", (char*) &stdlibFiles, 0, N_EXT | N_DATA},
    {{NULL},"_strcat", (char*) strcat, 0, N_EXT | N_TEXT},
    {{NULL},"_strchr", (char*) strchr, 0, N_EXT | N_TEXT},
    {{NULL},"_strcmp", (char*) strcmp, 0, N_EXT | N_TEXT},
    {{NULL},"_strcoll", (char*) strcoll, 0, N_EXT | N_TEXT},
    {{NULL},"_strcpy", (char*) strcpy, 0, N_EXT | N_TEXT},
    {{NULL},"_strcspn", (char*) strcspn, 0, N_EXT | N_TEXT},
    {{NULL},"_strerror", (char*) strerror, 0, N_EXT | N_TEXT},
    {{NULL},"_strerror_r", (char*) strerror_r, 0, N_EXT | N_TEXT},
    {{NULL},"_strftime", (char*) strftime, 0, N_EXT | N_TEXT},
    {{NULL},"_stringFiles", (char*) &stringFiles, 0, N_EXT | N_DATA},
    {{NULL},"_strlen", (char*) strlen, 0, N_EXT | N_TEXT},
    {{NULL},"_strncat", (char*) strncat, 0, N_EXT | N_TEXT},
    {{NULL},"_strncmp", (char*) strncmp, 0, N_EXT | N_TEXT},
    {{NULL},"_strncpy", (char*) strncpy, 0, N_EXT | N_TEXT},
    {{NULL},"_strpbrk", (char*) strpbrk, 0, N_EXT | N_TEXT},
    {{NULL},"_strrchr", (char*) strrchr, 0, N_EXT | N_TEXT},
    {{NULL},"_strspn", (char*) strspn, 0, N_EXT | N_TEXT},
    {{NULL},"_strstr", (char*) strstr, 0, N_EXT | N_TEXT},
    {{NULL},"_strtod", (char*) strtod, 0, N_EXT | N_TEXT},
    {{NULL},"_strtok", (char*) strtok, 0, N_EXT | N_TEXT},
    {{NULL},"_strtok_r", (char*) strtok_r, 0, N_EXT | N_TEXT},
    {{NULL},"_strtol", (char*) strtol, 0, N_EXT | N_TEXT},
    {{NULL},"_strtoul", (char*) strtoul, 0, N_EXT | N_TEXT},
    {{NULL},"_strxfrm", (char*) strxfrm, 0, N_EXT | N_TEXT},
    {{NULL},"_subnetsarelocal", (char*) &subnetsarelocal, 0, N_EXT | N_DATA},
    {{NULL},"_substrcmp", (char*) substrcmp, 0, N_EXT | N_TEXT},
    {{NULL},"_swab", (char*) swab, 0, N_EXT | N_TEXT},
    {{NULL},"_symAdd", (char*) symAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_symAlloc", (char*) symAlloc, 0, N_EXT | N_TEXT},
    {{NULL},"_symEach", (char*) symEach, 0, N_EXT | N_TEXT},
    {{NULL},"_symFindByCName", (char*) symFindByCName, 0, N_EXT | N_TEXT},
    {{NULL},"_symFindByName", (char*) symFindByName, 0, N_EXT | N_TEXT},
    {{NULL},"_symFindByNameAndType", (char*) symFindByNameAndType, 0, N_EXT | N_TEXT},
    {{NULL},"_symFindByValue", (char*) symFindByValue, 0, N_EXT | N_TEXT},
    {{NULL},"_symFindByValueAndType", (char*) symFindByValueAndType, 0, N_EXT | N_TEXT},
    {{NULL},"_symFindSymbol", (char*) symFindSymbol, 0, N_EXT | N_TEXT},
    {{NULL},"_symFree", (char*) symFree, 0, N_EXT | N_TEXT},
    {{NULL},"_symGroupDefault", (char*) &symGroupDefault, 0, N_EXT | N_DATA},
    {{NULL},"_symInit", (char*) symInit, 0, N_EXT | N_TEXT},
    {{NULL},"_symLibInit", (char*) symLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_symLkupPgSz", (char*) &symLkupPgSz, 0, N_EXT | N_DATA},
    {{NULL},"_symName", (char*) symName, 0, N_EXT | N_TEXT},
    {{NULL},"_symRemove", (char*) symRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_symShow", (char*) symShow, 0, N_EXT | N_TEXT},
    {{NULL},"_symShowInit", (char*) symShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_symTblAdd", (char*) symTblAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_symTblClassId", (char*) &symTblClassId, 0, N_EXT | N_DATA},
    {{NULL},"_symTblCreate", (char*) symTblCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_symTblDelete", (char*) symTblDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_symTblDestroy", (char*) symTblDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_symTblInit", (char*) symTblInit, 0, N_EXT | N_TEXT},
    {{NULL},"_symTblRemove", (char*) symTblRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_symTblTerminate", (char*) symTblTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_sys596ChanAtn", (char*) sys596ChanAtn, 0, N_EXT | N_TEXT},
    {{NULL},"_sys596Init", (char*) sys596Init, 0, N_EXT | N_TEXT},
    {{NULL},"_sys596IntAck", (char*) sys596IntAck, 0, N_EXT | N_TEXT},
    {{NULL},"_sys596IntDisable", (char*) sys596IntDisable, 0, N_EXT | N_TEXT},
    {{NULL},"_sys596IntEnable", (char*) sys596IntEnable, 0, N_EXT | N_TEXT},
    {{NULL},"_sys596Port", (char*) sys596Port, 0, N_EXT | N_TEXT},
    {{NULL},"_sysAdaEnable", (char*) &sysAdaEnable, 0, N_EXT | N_DATA},
    {{NULL},"_sysAuxClkConnect", (char*) sysAuxClkConnect, 0, N_EXT | N_TEXT},
    {{NULL},"_sysAuxClkDisable", (char*) sysAuxClkDisable, 0, N_EXT | N_TEXT},
    {{NULL},"_sysAuxClkEnable", (char*) sysAuxClkEnable, 0, N_EXT | N_TEXT},
    {{NULL},"_sysAuxClkRateGet", (char*) sysAuxClkRateGet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysAuxClkRateSet", (char*) sysAuxClkRateSet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysBootFile", (char*) &sysBootFile, 0, N_EXT | N_BSS},
    {{NULL},"_sysBootHost", (char*) &sysBootHost, 0, N_EXT | N_BSS},
    {{NULL},"_sysBootLine", (char*) &sysBootLine, 0, N_EXT | N_DATA},
    {{NULL},"_sysBootParams", (char*) &sysBootParams, 0, N_EXT | N_BSS},
    {{NULL},"_sysBspRev", (char*) sysBspRev, 0, N_EXT | N_TEXT},
    {{NULL},"_sysBus", (char*) &sysBus, 0, N_EXT | N_DATA},
    {{NULL},"_sysBusIntAck", (char*) sysBusIntAck, 0, N_EXT | N_TEXT},
    {{NULL},"_sysBusIntGen", (char*) sysBusIntGen, 0, N_EXT | N_TEXT},
    {{NULL},"_sysBusTas", (char*) sysBusTas, 0, N_EXT | N_TEXT},
    {{NULL},"_sysBusToLocalAdrs", (char*) sysBusToLocalAdrs, 0, N_EXT | N_TEXT},
    {{NULL},"_sysClkConnect", (char*) sysClkConnect, 0, N_EXT | N_TEXT},
    {{NULL},"_sysClkDisable", (char*) sysClkDisable, 0, N_EXT | N_TEXT},
    {{NULL},"_sysClkEnable", (char*) sysClkEnable, 0, N_EXT | N_TEXT},
    {{NULL},"_sysClkRateGet", (char*) sysClkRateGet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysClkRateSet", (char*) sysClkRateSet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysCplusEnable", (char*) &sysCplusEnable, 0, N_EXT | N_DATA},
    {{NULL},"_sysCpu", (char*) &sysCpu, 0, N_EXT | N_DATA},
    {{NULL},"_sysEnetAddrGet", (char*) sysEnetAddrGet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysExcMsg", (char*) &sysExcMsg, 0, N_EXT | N_DATA},
    {{NULL},"_sysFlags", (char*) &sysFlags, 0, N_EXT | N_BSS},
    {{NULL},"_sysHwInit", (char*) sysHwInit, 0, N_EXT | N_TEXT},
    {{NULL},"_sysHwInit2", (char*) sysHwInit2, 0, N_EXT | N_TEXT},
    {{NULL},"_sysInit", (char*) sysInit, 0, N_EXT | N_TEXT},
    {{NULL},"_sysIntDisable", (char*) sysIntDisable, 0, N_EXT | N_TEXT},
    {{NULL},"_sysIntEnable", (char*) sysIntEnable, 0, N_EXT | N_TEXT},
    {{NULL},"_sysLocalToBusAdrs", (char*) sysLocalToBusAdrs, 0, N_EXT | N_TEXT},
    {{NULL},"_sysMailboxConnect", (char*) sysMailboxConnect, 0, N_EXT | N_TEXT},
    {{NULL},"_sysMailboxEnable", (char*) sysMailboxEnable, 0, N_EXT | N_TEXT},
    {{NULL},"_sysMemProbe", (char*) sysMemProbe, 0, N_EXT | N_TEXT},
    {{NULL},"_sysMemTop", (char*) sysMemTop, 0, N_EXT | N_TEXT},
    {{NULL},"_sysModel", (char*) sysModel, 0, N_EXT | N_TEXT},
    {{NULL},"_sysNvRamGet", (char*) sysNvRamGet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysNvRamSet", (char*) sysNvRamSet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysPhysMemDesc", (char*) &sysPhysMemDesc, 0, N_EXT | N_DATA},
    {{NULL},"_sysPhysMemDescNumEnt", (char*) &sysPhysMemDescNumEnt, 0, N_EXT | N_DATA},
    {{NULL},"_sysProcNumGet", (char*) sysProcNumGet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysProcNumSet", (char*) sysProcNumSet, 0, N_EXT | N_TEXT},
    {{NULL},"_sysStartType", (char*) &sysStartType, 0, N_EXT | N_BSS},
    {{NULL},"_sysSymTbl", (char*) &sysSymTbl, 0, N_EXT | N_BSS},
    {{NULL},"_sysToMonitor", (char*) sysToMonitor, 0, N_EXT | N_TEXT},
    {{NULL},"_system", (char*) system, 0, N_EXT | N_TEXT},
    {{NULL},"_tan", (char*) tan, 0, N_EXT | N_TEXT},
    {{NULL},"_tanf", (char*) tanf, 0, N_EXT | N_TEXT},
    {{NULL},"_tanh", (char*) tanh, 0, N_EXT | N_TEXT},
    {{NULL},"_tanhf", (char*) tanhf, 0, N_EXT | N_TEXT},
    {{NULL},"_taskActivate", (char*) taskActivate, 0, N_EXT | N_TEXT},
    {{NULL},"_taskArgsGet", (char*) taskArgsGet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskArgsSet", (char*) taskArgsSet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskBpHook", (char*) &taskBpHook, 0, N_EXT | N_BSS},
    {{NULL},"_taskBpHookSet", (char*) taskBpHookSet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskClassId", (char*) &taskClassId, 0, N_EXT | N_DATA},
    {{NULL},"_taskCreat", (char*) taskCreat, 0, N_EXT | N_TEXT},
    {{NULL},"_taskCreateHookAdd", (char*) taskCreateHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_taskCreateHookDelete", (char*) taskCreateHookDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_taskCreateHookShow", (char*) taskCreateHookShow, 0, N_EXT | N_TEXT},
    {{NULL},"_taskCreateTable", (char*) &taskCreateTable, 0, N_EXT | N_BSS},
    {{NULL},"_taskDelay", (char*) taskDelay, 0, N_EXT | N_TEXT},
    {{NULL},"_taskDelete", (char*) taskDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_taskDeleteForce", (char*) taskDeleteForce, 0, N_EXT | N_TEXT},
    {{NULL},"_taskDeleteHookAdd", (char*) taskDeleteHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_taskDeleteHookDelete", (char*) taskDeleteHookDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_taskDeleteHookShow", (char*) taskDeleteHookShow, 0, N_EXT | N_TEXT},
    {{NULL},"_taskDeleteTable", (char*) &taskDeleteTable, 0, N_EXT | N_BSS},
    {{NULL},"_taskDestroy", (char*) taskDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_taskHookInit", (char*) taskHookInit, 0, N_EXT | N_TEXT},
    {{NULL},"_taskHookShowInit", (char*) taskHookShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_taskIdCurrent", (char*) &taskIdCurrent, 0, N_EXT | N_BSS},
    {{NULL},"_taskIdDefault", (char*) taskIdDefault, 0, N_EXT | N_TEXT},
    {{NULL},"_taskIdFigure", (char*) taskIdFigure, 0, N_EXT | N_TEXT},
    {{NULL},"_taskIdListGet", (char*) taskIdListGet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskIdListSort", (char*) taskIdListSort, 0, N_EXT | N_TEXT},
    {{NULL},"_taskIdSelf", (char*) taskIdSelf, 0, N_EXT | N_TEXT},
    {{NULL},"_taskIdVerify", (char*) taskIdVerify, 0, N_EXT | N_TEXT},
    {{NULL},"_taskInfoGet", (char*) taskInfoGet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskInit", (char*) taskInit, 0, N_EXT | N_TEXT},
    {{NULL},"_taskIsReady", (char*) taskIsReady, 0, N_EXT | N_TEXT},
    {{NULL},"_taskIsSuspended", (char*) taskIsSuspended, 0, N_EXT | N_TEXT},
    {{NULL},"_taskLibInit", (char*) taskLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_taskLock", (char*) taskLock, 0, N_EXT | N_TEXT},
    {{NULL},"_taskName", (char*) taskName, 0, N_EXT | N_TEXT},
    {{NULL},"_taskNameToId", (char*) taskNameToId, 0, N_EXT | N_TEXT},
    {{NULL},"_taskOptionsGet", (char*) taskOptionsGet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskOptionsSet", (char*) taskOptionsSet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskOptionsString", (char*) taskOptionsString, 0, N_EXT | N_TEXT},
    {{NULL},"_taskPriRangeCheck", (char*) &taskPriRangeCheck, 0, N_EXT | N_DATA},
    {{NULL},"_taskPriorityGet", (char*) taskPriorityGet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskPrioritySet", (char*) taskPrioritySet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskRegName", (char*) &taskRegName, 0, N_EXT | N_DATA},
    {{NULL},"_taskRegsFmt", (char*) &taskRegsFmt, 0, N_EXT | N_DATA},
    {{NULL},"_taskRegsGet", (char*) taskRegsGet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskRegsInit", (char*) taskRegsInit, 0, N_EXT | N_TEXT},
    {{NULL},"_taskRegsSet", (char*) taskRegsSet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskRegsShow", (char*) taskRegsShow, 0, N_EXT | N_TEXT},
    {{NULL},"_taskRestart", (char*) taskRestart, 0, N_EXT | N_TEXT},
    {{NULL},"_taskResume", (char*) taskResume, 0, N_EXT | N_TEXT},
    {{NULL},"_taskRtnValueSet", (char*) taskRtnValueSet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSRSet", (char*) taskSRSet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSafe", (char*) taskSafe, 0, N_EXT | N_TEXT},
    {{NULL},"_taskShow", (char*) taskShow, 0, N_EXT | N_TEXT},
    {{NULL},"_taskShowInit", (char*) taskShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSpawn", (char*) taskSpawn, 0, N_EXT | N_TEXT},
    {{NULL},"_taskStackAllot", (char*) taskStackAllot, 0, N_EXT | N_TEXT},
    {{NULL},"_taskStatusString", (char*) taskStatusString, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSuspend", (char*) taskSuspend, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwapHookAdd", (char*) taskSwapHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwapHookAttach", (char*) taskSwapHookAttach, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwapHookDelete", (char*) taskSwapHookDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwapHookDetach", (char*) taskSwapHookDetach, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwapHookShow", (char*) taskSwapHookShow, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwapReference", (char*) &taskSwapReference, 0, N_EXT | N_BSS},
    {{NULL},"_taskSwapTable", (char*) &taskSwapTable, 0, N_EXT | N_BSS},
    {{NULL},"_taskSwitchHookAdd", (char*) taskSwitchHookAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwitchHookDelete", (char*) taskSwitchHookDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwitchHookShow", (char*) taskSwitchHookShow, 0, N_EXT | N_TEXT},
    {{NULL},"_taskSwitchTable", (char*) &taskSwitchTable, 0, N_EXT | N_BSS},
    {{NULL},"_taskTcb", (char*) taskTcb, 0, N_EXT | N_TEXT},
    {{NULL},"_taskTerminate", (char*) taskTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_taskUndelay", (char*) taskUndelay, 0, N_EXT | N_TEXT},
    {{NULL},"_taskUnlock", (char*) taskUnlock, 0, N_EXT | N_TEXT},
    {{NULL},"_taskUnsafe", (char*) taskUnsafe, 0, N_EXT | N_TEXT},
    {{NULL},"_taskVarAdd", (char*) taskVarAdd, 0, N_EXT | N_TEXT},
    {{NULL},"_taskVarDelete", (char*) taskVarDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_taskVarGet", (char*) taskVarGet, 0, N_EXT | N_TEXT},
    {{NULL},"_taskVarInfo", (char*) taskVarInfo, 0, N_EXT | N_TEXT},
    {{NULL},"_taskVarInit", (char*) taskVarInit, 0, N_EXT | N_TEXT},
    {{NULL},"_taskVarSet", (char*) taskVarSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tcpDebugShow", (char*) tcpDebugShow, 0, N_EXT | N_TEXT},
    {{NULL},"_tcpOutRsts", (char*) &tcpOutRsts, 0, N_EXT | N_BSS},
    {{NULL},"_tcpPatch", (char*) &tcpPatch, 0, N_EXT | N_DATA},
    {{NULL},"_tcpReportRtn", (char*) &tcpReportRtn, 0, N_EXT | N_DATA},
    {{NULL},"_tcpTraceRtn", (char*) &tcpTraceRtn, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_alpha", (char*) &tcp_alpha, 0, N_EXT | N_BSS},
    {{NULL},"_tcp_attach", (char*) tcp_attach, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_backoff", (char*) &tcp_backoff, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_beta", (char*) &tcp_beta, 0, N_EXT | N_BSS},
    {{NULL},"_tcp_canceltimers", (char*) tcp_canceltimers, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_close", (char*) tcp_close, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_ctlinput", (char*) tcp_ctlinput, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_ctloutput", (char*) tcp_ctloutput, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_disconnect", (char*) tcp_disconnect, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_dooptions", (char*) tcp_dooptions, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_drain", (char*) tcp_drain, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_drop", (char*) tcp_drop, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_fasttimo", (char*) tcp_fasttimo, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_init", (char*) tcp_init, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_initopt", (char*) &tcp_initopt, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_input", (char*) tcp_input, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_iss", (char*) &tcp_iss, 0, N_EXT | N_BSS},
    {{NULL},"_tcp_keepcnt", (char*) &tcp_keepcnt, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_keepidle", (char*) &tcp_keepidle, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_keepinit", (char*) &tcp_keepinit, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_keepintvl", (char*) &tcp_keepintvl, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_maxidle", (char*) &tcp_maxidle, 0, N_EXT | N_BSS},
    {{NULL},"_tcp_mss", (char*) tcp_mss, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_newtcpcb", (char*) tcp_newtcpcb, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_notify", (char*) tcp_notify, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_outflags", (char*) &tcp_outflags, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_output", (char*) tcp_output, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_pulloutofband", (char*) tcp_pulloutofband, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_quench", (char*) tcp_quench, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_reass", (char*) tcp_reass, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_recvspace", (char*) &tcp_recvspace, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_respond", (char*) tcp_respond, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_saveti", (char*) &tcp_saveti, 0, N_EXT | N_BSS},
    {{NULL},"_tcp_sendspace", (char*) &tcp_sendspace, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_setpersist", (char*) tcp_setpersist, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_slowtimo", (char*) tcp_slowtimo, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_template", (char*) tcp_template, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_timers", (char*) tcp_timers, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_ttl", (char*) &tcp_ttl, 0, N_EXT | N_DATA},
    {{NULL},"_tcp_usrclosed", (char*) tcp_usrclosed, 0, N_EXT | N_TEXT},
    {{NULL},"_tcp_usrreq", (char*) tcp_usrreq, 0, N_EXT | N_TEXT},
    {{NULL},"_tcpcb", (char*) &tcpcb, 0, N_EXT | N_BSS},
    {{NULL},"_tcpcksum", (char*) &tcpcksum, 0, N_EXT | N_DATA},
    {{NULL},"_tcpprintfs", (char*) &tcpprintfs, 0, N_EXT | N_DATA},
    {{NULL},"_tcprexmtthresh", (char*) &tcprexmtthresh, 0, N_EXT | N_DATA},
    {{NULL},"_tcpstat", (char*) &tcpstat, 0, N_EXT | N_BSS},
    {{NULL},"_tcpstatShow", (char*) tcpstatShow, 0, N_EXT | N_TEXT},
    {{NULL},"_tcpstates", (char*) &tcpstates, 0, N_EXT | N_DATA},
    {{NULL},"_td", (char*) td, 0, N_EXT | N_TEXT},
    {{NULL},"_telnetInTask", (char*) telnetInTask, 0, N_EXT | N_TEXT},
    {{NULL},"_telnetInTaskId", (char*) &telnetInTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_telnetInit", (char*) telnetInit, 0, N_EXT | N_TEXT},
    {{NULL},"_telnetOutTask", (char*) telnetOutTask, 0, N_EXT | N_TEXT},
    {{NULL},"_telnetOutTaskId", (char*) &telnetOutTaskId, 0, N_EXT | N_BSS},
    {{NULL},"_telnetTaskOptions", (char*) &telnetTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_telnetTaskPriority", (char*) &telnetTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_telnetTaskStackSize", (char*) &telnetTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_telnetd", (char*) telnetd, 0, N_EXT | N_TEXT},
    {{NULL},"_telnetdId", (char*) &telnetdId, 0, N_EXT | N_BSS},
    {{NULL},"_telnetdSocket", (char*) &telnetdSocket, 0, N_EXT | N_BSS},
    {{NULL},"_tftpCopy", (char*) tftpCopy, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpErrorCreate", (char*) tftpErrorCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpGet", (char*) tftpGet, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpInfoShow", (char*) tftpInfoShow, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpInit", (char*) tftpInit, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpModeSet", (char*) tftpModeSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpPeerSet", (char*) tftpPeerSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpPut", (char*) tftpPut, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpQuit", (char*) tftpQuit, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpReXmit", (char*) &tftpReXmit, 0, N_EXT | N_DATA},
    {{NULL},"_tftpSend", (char*) tftpSend, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpTask", (char*) tftpTask, 0, N_EXT | N_TEXT},
    {{NULL},"_tftpTaskOptions", (char*) &tftpTaskOptions, 0, N_EXT | N_DATA},
    {{NULL},"_tftpTaskPriority", (char*) &tftpTaskPriority, 0, N_EXT | N_DATA},
    {{NULL},"_tftpTaskStackSize", (char*) &tftpTaskStackSize, 0, N_EXT | N_DATA},
    {{NULL},"_tftpTimeout", (char*) &tftpTimeout, 0, N_EXT | N_DATA},
    {{NULL},"_tftpTrace", (char*) &tftpTrace, 0, N_EXT | N_DATA},
    {{NULL},"_tftpVerbose", (char*) &tftpVerbose, 0, N_EXT | N_DATA},
    {{NULL},"_tftpXfer", (char*) tftpXfer, 0, N_EXT | N_TEXT},
    {{NULL},"_ti", (char*) ti, 0, N_EXT | N_TEXT},
    {{NULL},"_tickAnnounce", (char*) tickAnnounce, 0, N_EXT | N_TEXT},
    {{NULL},"_tickGet", (char*) tickGet, 0, N_EXT | N_TEXT},
    {{NULL},"_tickQHead", (char*) &tickQHead, 0, N_EXT | N_BSS},
    {{NULL},"_tickSet", (char*) tickSet, 0, N_EXT | N_TEXT},
    {{NULL},"_time", (char*) time, 0, N_EXT | N_TEXT},
    {{NULL},"_timeFiles", (char*) &timeFiles, 0, N_EXT | N_DATA},
    {{NULL},"_timex", (char*) timex, 0, N_EXT | N_TEXT},
    {{NULL},"_timexClear", (char*) timexClear, 0, N_EXT | N_TEXT},
    {{NULL},"_timexFunc", (char*) timexFunc, 0, N_EXT | N_TEXT},
    {{NULL},"_timexHelp", (char*) timexHelp, 0, N_EXT | N_TEXT},
    {{NULL},"_timexInit", (char*) timexInit, 0, N_EXT | N_TEXT},
    {{NULL},"_timexN", (char*) timexN, 0, N_EXT | N_TEXT},
    {{NULL},"_timexPost", (char*) timexPost, 0, N_EXT | N_TEXT},
    {{NULL},"_timexPre", (char*) timexPre, 0, N_EXT | N_TEXT},
    {{NULL},"_timexShow", (char*) timexShow, 0, N_EXT | N_TEXT},
    {{NULL},"_tmpfile", (char*) tmpfile, 0, N_EXT | N_TEXT},
    {{NULL},"_tmpnam", (char*) tmpnam, 0, N_EXT | N_TEXT},
    {{NULL},"_tolower", (char*) tolower, 0, N_EXT | N_TEXT},
    {{NULL},"_toupper", (char*) toupper, 0, N_EXT | N_TEXT},
    {{NULL},"_tr", (char*) tr, 0, N_EXT | N_TEXT},
    {{NULL},"_trcDefaultArgs", (char*) &trcDefaultArgs, 0, N_EXT | N_DATA},
    {{NULL},"_trcStack", (char*) trcStack, 0, N_EXT | N_TEXT},
    {{NULL},"_trunc", (char*) trunc, 0, N_EXT | N_TEXT},
    {{NULL},"_truncf", (char*) truncf, 0, N_EXT | N_TEXT},
    {{NULL},"_ts", (char*) ts, 0, N_EXT | N_TEXT},
    {{NULL},"_tt", (char*) tt, 0, N_EXT | N_TEXT},
    {{NULL},"_tyAbortFuncSet", (char*) tyAbortFuncSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tyAbortSet", (char*) tyAbortSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tyBackspaceChar", (char*) &tyBackspaceChar, 0, N_EXT | N_DATA},
    {{NULL},"_tyBackspaceSet", (char*) tyBackspaceSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tyCoDevCreate", (char*) tyCoDevCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_tyCoDrv", (char*) tyCoDrv, 0, N_EXT | N_TEXT},
    {{NULL},"_tyCoDv", (char*) &tyCoDv, 0, N_EXT | N_BSS},
    {{NULL},"_tyCoInt", (char*) tyCoInt, 0, N_EXT | N_TEXT},
    {{NULL},"_tyCoIntEx", (char*) tyCoIntEx, 0, N_EXT | N_TEXT},
    {{NULL},"_tyCoIntRd", (char*) tyCoIntRd, 0, N_EXT | N_TEXT},
    {{NULL},"_tyCoIntWr", (char*) tyCoIntWr, 0, N_EXT | N_TEXT},
    {{NULL},"_tyDeleteLineChar", (char*) &tyDeleteLineChar, 0, N_EXT | N_DATA},
    {{NULL},"_tyDeleteLineSet", (char*) tyDeleteLineSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tyDevInit", (char*) tyDevInit, 0, N_EXT | N_TEXT},
    {{NULL},"_tyEOFSet", (char*) tyEOFSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tyEofChar", (char*) &tyEofChar, 0, N_EXT | N_DATA},
    {{NULL},"_tyIRd", (char*) tyIRd, 0, N_EXT | N_TEXT},
    {{NULL},"_tyITx", (char*) tyITx, 0, N_EXT | N_TEXT},
    {{NULL},"_tyIoctl", (char*) tyIoctl, 0, N_EXT | N_TEXT},
    {{NULL},"_tyMonitorTrapSet", (char*) tyMonitorTrapSet, 0, N_EXT | N_TEXT},
    {{NULL},"_tyRead", (char*) tyRead, 0, N_EXT | N_TEXT},
    {{NULL},"_tyWrite", (char*) tyWrite, 0, N_EXT | N_TEXT},
    {{NULL},"_udb", (char*) &udb, 0, N_EXT | N_BSS},
    {{NULL},"_udpNoPorts", (char*) &udpNoPorts, 0, N_EXT | N_BSS},
    {{NULL},"_udp_ctlinput", (char*) udp_ctlinput, 0, N_EXT | N_TEXT},
    {{NULL},"_udp_in", (char*) &udp_in, 0, N_EXT | N_DATA},
    {{NULL},"_udp_init", (char*) udp_init, 0, N_EXT | N_TEXT},
    {{NULL},"_udp_input", (char*) udp_input, 0, N_EXT | N_TEXT},
    {{NULL},"_udp_notify", (char*) udp_notify, 0, N_EXT | N_TEXT},
    {{NULL},"_udp_output", (char*) udp_output, 0, N_EXT | N_TEXT},
    {{NULL},"_udp_recvspace", (char*) &udp_recvspace, 0, N_EXT | N_DATA},
    {{NULL},"_udp_sendspace", (char*) &udp_sendspace, 0, N_EXT | N_DATA},
    {{NULL},"_udp_ttl", (char*) &udp_ttl, 0, N_EXT | N_DATA},
    {{NULL},"_udp_usrreq", (char*) udp_usrreq, 0, N_EXT | N_TEXT},
    {{NULL},"_udpcksum", (char*) &udpcksum, 0, N_EXT | N_DATA},
    {{NULL},"_udpstat", (char*) &udpstat, 0, N_EXT | N_BSS},
    {{NULL},"_udpstatShow", (char*) udpstatShow, 0, N_EXT | N_TEXT},
    {{NULL},"_uiomove", (char*) uiomove, 0, N_EXT | N_TEXT},
    {{NULL},"_ungetc", (char*) ungetc, 0, N_EXT | N_TEXT},
    {{NULL},"_unld", (char*) unld, 0, N_EXT | N_TEXT},
    {{NULL},"_unldByGroup", (char*) unldByGroup, 0, N_EXT | N_TEXT},
    {{NULL},"_unldByModuleId", (char*) unldByModuleId, 0, N_EXT | N_TEXT},
    {{NULL},"_unldByNameAndPath", (char*) unldByNameAndPath, 0, N_EXT | N_TEXT},
    {{NULL},"_unldTextSegmentCheck", (char*) unldTextSegmentCheck, 0, N_EXT | N_TEXT},
    {{NULL},"_unlink", (char*) unlink, 0, N_EXT | N_TEXT},
    {{NULL},"_useloopback", (char*) &useloopback, 0, N_EXT | N_DATA},
    {{NULL},"_usrBootLineCrack", (char*) usrBootLineCrack, 0, N_EXT | N_TEXT},
    {{NULL},"_usrBootLineInit", (char*) usrBootLineInit, 0, N_EXT | N_TEXT},
    {{NULL},"_usrBpInit", (char*) usrBpInit, 0, N_EXT | N_TEXT},
    {{NULL},"_usrClock", (char*) usrClock, 0, N_EXT | N_TEXT},
    {{NULL},"_usrExtraModules", (char*) &usrExtraModules, 0, N_EXT | N_DATA},
    {{NULL},"_usrInit", (char*) usrInit, 0, N_EXT | N_TEXT},
    {{NULL},"_usrKernelInit", (char*) usrKernelInit, 0, N_EXT | N_TEXT},
    {{NULL},"_usrMmuInit", (char*) usrMmuInit, 0, N_EXT | N_TEXT},
    {{NULL},"_usrNetIfAttach", (char*) usrNetIfAttach, 0, N_EXT | N_TEXT},
    {{NULL},"_usrNetIfConfig", (char*) usrNetIfConfig, 0, N_EXT | N_TEXT},
    {{NULL},"_usrNetInit", (char*) usrNetInit, 0, N_EXT | N_TEXT},
    {{NULL},"_usrRoot", (char*) usrRoot, 0, N_EXT | N_TEXT},
    {{NULL},"_usrSlipInit", (char*) usrSlipInit, 0, N_EXT | N_TEXT},
    {{NULL},"_usrStartupScript", (char*) usrStartupScript, 0, N_EXT | N_TEXT},
    {{NULL},"_uswab", (char*) uswab, 0, N_EXT | N_TEXT},
    {{NULL},"_utime", (char*) utime, 0, N_EXT | N_TEXT},
    {{NULL},"_valloc", (char*) valloc, 0, N_EXT | N_TEXT},
    {{NULL},"_version", (char*) version, 0, N_EXT | N_TEXT},
    {{NULL},"_vfdprintf", (char*) vfdprintf, 0, N_EXT | N_TEXT},
    {{NULL},"_vfprintf", (char*) vfprintf, 0, N_EXT | N_TEXT},
    {{NULL},"_vmBaseGlobalMapInit", (char*) vmBaseGlobalMapInit, 0, N_EXT | N_TEXT},
    {{NULL},"_vmBaseLibInit", (char*) vmBaseLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_vmBasePageSizeGet", (char*) vmBasePageSizeGet, 0, N_EXT | N_TEXT},
    {{NULL},"_vmBaseStateSet", (char*) vmBaseStateSet, 0, N_EXT | N_TEXT},
    {{NULL},"_vmContextClassId", (char*) &vmContextClassId, 0, N_EXT | N_DATA},
    {{NULL},"_vmLibInfo", (char*) &vmLibInfo, 0, N_EXT | N_DATA},
    {{NULL},"_vprintf", (char*) vprintf, 0, N_EXT | N_TEXT},
    {{NULL},"_vsprintf", (char*) vsprintf, 0, N_EXT | N_TEXT},
    {{NULL},"_vxAbsTicks", (char*) &vxAbsTicks, 0, N_EXT | N_BSS},
    {{NULL},"_vxIntStackBase", (char*) &vxIntStackBase, 0, N_EXT | N_BSS},
    {{NULL},"_vxIntStackEnd", (char*) &vxIntStackEnd, 0, N_EXT | N_BSS},
    {{NULL},"_vxMemProbe", (char*) vxMemProbe, 0, N_EXT | N_TEXT},
    {{NULL},"_vxMemProbeSup", (char*) vxMemProbeSup, 0, N_EXT | N_TEXT},
    {{NULL},"_vxMemProbeTrap", (char*) vxMemProbeTrap, 0, N_EXT | N_TEXT},
    {{NULL},"_vxTas", (char*) vxTas, 0, N_EXT | N_TEXT},
    {{NULL},"_vxTaskEntry", (char*) vxTaskEntry, 0, N_EXT | N_TEXT},
    {{NULL},"_vxTicks", (char*) &vxTicks, 0, N_EXT | N_BSS},
    {{NULL},"_vxWorksVersion", (char*) &vxWorksVersion, 0, N_EXT | N_DATA},
    {{NULL},"_vxWorksVersionStr", (char*) &vxWorksVersionStr, 0, N_EXT | N_DATA},
    {{NULL},"_vxmIfOps", (char*) &vxmIfOps, 0, N_EXT | N_BSS},
    {{NULL},"_wakeup", (char*) wakeup, 0, N_EXT | N_TEXT},
    {{NULL},"_wcstombs", (char*) wcstombs, 0, N_EXT | N_TEXT},
    {{NULL},"_wctomb", (char*) wctomb, 0, N_EXT | N_TEXT},
    {{NULL},"_wdCancel", (char*) wdCancel, 0, N_EXT | N_TEXT},
    {{NULL},"_wdClassId", (char*) &wdClassId, 0, N_EXT | N_DATA},
    {{NULL},"_wdCreate", (char*) wdCreate, 0, N_EXT | N_TEXT},
    {{NULL},"_wdDelete", (char*) wdDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_wdDestroy", (char*) wdDestroy, 0, N_EXT | N_TEXT},
    {{NULL},"_wdInit", (char*) wdInit, 0, N_EXT | N_TEXT},
    {{NULL},"_wdLibInit", (char*) wdLibInit, 0, N_EXT | N_TEXT},
    {{NULL},"_wdShow", (char*) wdShow, 0, N_EXT | N_TEXT},
    {{NULL},"_wdShowInit", (char*) wdShowInit, 0, N_EXT | N_TEXT},
    {{NULL},"_wdStart", (char*) wdStart, 0, N_EXT | N_TEXT},
    {{NULL},"_wdTerminate", (char*) wdTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_wdTick", (char*) wdTick, 0, N_EXT | N_TEXT},
    {{NULL},"_whoami", (char*) whoami, 0, N_EXT | N_TEXT},
    {{NULL},"_wildcard", (char*) &wildcard, 0, N_EXT | N_BSS},
    {{NULL},"_windDelay", (char*) windDelay, 0, N_EXT | N_TEXT},
    {{NULL},"_windDelete", (char*) windDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_windExit", (char*) windExit, 0, N_EXT | N_TEXT},
    {{NULL},"_windIntStackSet", (char*) windIntStackSet, 0, N_EXT | N_TEXT},
    {{NULL},"_windPendQFlush", (char*) windPendQFlush, 0, N_EXT | N_TEXT},
    {{NULL},"_windPendQGet", (char*) windPendQGet, 0, N_EXT | N_TEXT},
    {{NULL},"_windPendQPut", (char*) windPendQPut, 0, N_EXT | N_TEXT},
    {{NULL},"_windPendQRemove", (char*) windPendQRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_windPendQTerminate", (char*) windPendQTerminate, 0, N_EXT | N_TEXT},
    {{NULL},"_windPriNormalSet", (char*) windPriNormalSet, 0, N_EXT | N_TEXT},
    {{NULL},"_windPrioritySet", (char*) windPrioritySet, 0, N_EXT | N_TEXT},
    {{NULL},"_windReadyQPut", (char*) windReadyQPut, 0, N_EXT | N_TEXT},
    {{NULL},"_windReadyQRemove", (char*) windReadyQRemove, 0, N_EXT | N_TEXT},
    {{NULL},"_windResume", (char*) windResume, 0, N_EXT | N_TEXT},
    {{NULL},"_windSemDelete", (char*) windSemDelete, 0, N_EXT | N_TEXT},
    {{NULL},"_windSpawn", (char*) windSpawn, 0, N_EXT | N_TEXT},
    {{NULL},"_windSuspend", (char*) windSuspend, 0, N_EXT | N_TEXT},
    {{NULL},"_windTickAnnounce", (char*) windTickAnnounce, 0, N_EXT | N_TEXT},
    {{NULL},"_windUndelay", (char*) windUndelay, 0, N_EXT | N_TEXT},
    {{NULL},"_windWdCancel", (char*) windWdCancel, 0, N_EXT | N_TEXT},
    {{NULL},"_windWdStart", (char*) windWdStart, 0, N_EXT | N_TEXT},
    {{NULL},"_workQAdd0", (char*) workQAdd0, 0, N_EXT | N_TEXT},
    {{NULL},"_workQAdd1", (char*) workQAdd1, 0, N_EXT | N_TEXT},
    {{NULL},"_workQAdd2", (char*) workQAdd2, 0, N_EXT | N_TEXT},
    {{NULL},"_workQDoWork", (char*) workQDoWork, 0, N_EXT | N_TEXT},
    {{NULL},"_workQInit", (char*) workQInit, 0, N_EXT | N_TEXT},
    {{NULL},"_workQIsEmpty", (char*) &workQIsEmpty, 0, N_EXT | N_BSS},
    {{NULL},"_workQPanic", (char*) workQPanic, 0, N_EXT | N_TEXT},
    {{NULL},"_workQReadIx", (char*) &workQReadIx, 0, N_EXT | N_BSS},
    {{NULL},"_workQWriteIx", (char*) &workQWriteIx, 0, N_EXT | N_BSS},
    {{NULL},"_write", (char*) write, 0, N_EXT | N_TEXT},
    {{NULL},"_writev", (char*) writev, 0, N_EXT | N_TEXT},
    {{NULL},"_wvInstIsOn", (char*) &wvInstIsOn, 0, N_EXT | N_BSS},
    {{NULL},"_wvObjIsEnabled", (char*) &wvObjIsEnabled, 0, N_EXT | N_BSS},
    {{NULL},"_yyact", (char*) &yyact, 0, N_EXT | N_DATA},
    {{NULL},"_yychar", (char*) &yychar, 0, N_EXT | N_BSS},
    {{NULL},"_yychk", (char*) &yychk, 0, N_EXT | N_DATA},
    {{NULL},"_yydebug", (char*) &yydebug, 0, N_EXT | N_BSS},
    {{NULL},"_yydef", (char*) &yydef, 0, N_EXT | N_DATA},
    {{NULL},"_yyerrflag", (char*) &yyerrflag, 0, N_EXT | N_BSS},
    {{NULL},"_yyexca", (char*) &yyexca, 0, N_EXT | N_DATA},
    {{NULL},"_yylval", (char*) &yylval, 0, N_EXT | N_BSS},
    {{NULL},"_yynerrs", (char*) &yynerrs, 0, N_EXT | N_BSS},
    {{NULL},"_yypact", (char*) &yypact, 0, N_EXT | N_DATA},
    {{NULL},"_yyparse", (char*) yyparse, 0, N_EXT | N_TEXT},
    {{NULL},"_yypgo", (char*) &yypgo, 0, N_EXT | N_DATA},
    {{NULL},"_yyr1", (char*) &yyr1, 0, N_EXT | N_DATA},
    {{NULL},"_yyr2", (char*) &yyr2, 0, N_EXT | N_DATA},
    {{NULL},"_yystart", (char*) yystart, 0, N_EXT | N_TEXT},
    {{NULL},"_yyval", (char*) &yyval, 0, N_EXT | N_BSS},
    {{NULL},"_zeroin_addr", (char*) &zeroin_addr, 0, N_EXT | N_BSS},
    };

ULONG standTblSize = 2398;

