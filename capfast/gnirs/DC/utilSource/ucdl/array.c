static struct {
        void *v;
        char *c;
} rcsid = {
        &rcsid,
        "$Id: array.c,v 1.2 2009/05/27 19:33:35 fkraemer Exp $"
};
/******************************************************************************
 * Program:	control
 * File:	array.c
 * Purpose:	declare the arrays to gives the variable number (etc) index.
 * Author:	David Koski
 * Copyright:   Aura Inc.  All rights reserved.
 * History:
 *		20Jul92	created						dak
 *              22-May-1995 Changed name fo commands to ircommand name 
 *              change needed use tcl-tk and tcl-dp commands is used 
 *              globaly by tkWindows   SLP 
 *
 *****************************************************************************/

#include "protocol.h"
#include "util.h"

p_arr ircommands[] = {
    { "image_done", IMAGE_DONE},
    { "begin_xmit", BEGIN_XMIT},
    { "set_var", SET_VAR},
    { "read_var", READ_VAR},
    { "debug_msg", DEBUG_MSG},
    { "abort_msg", ABORT_MSG},
    { "stop_msg", STOP_MSG},
    { "start_msg", START_MSG},
    { "pause", PAUSE},
    { "resume", RESUME},
    { "var_read", VAR_READ},
    { "read_hk", READ_HK},
    { "set_var_ak", SET_VAR_AK},
    { "akk", AKK},
    { "akk_fail", AKK_FAIL},
    { "kill_proc", KILL_PROC},
    { "execute_proc", EXECUTE_PROC},
    { "check_hk", CHECK_HK},
    { "error", MSG_ERROR},
    { "", -1}
};

p_arr variables[] = {
    { "trace_flag", 0},
    { "frames", 1},			/* dspw */
    { "received", 2},
    { "prog", 1},
    { "image_size", 4},
    { "lnr", 5},
    { "coadds", 6},
    { "mode", 7},
    { "progress", 8},
    { "npix", 9},
    { "fdata", 11},
    { "delay", 12},
    { "p_filename", P_FILENAME},	/* b011 */
    { "p_path", P_PATH},
    { "p_num", P_NUM},
    { "p_pixel_dir", P_PIXEL_DIR}, 
    { "h_name", 1},
    { "h_form", 2},
    { "h_data", 3},
    { "h_comm", 4},
    { "hk_data", 10},
    { "hk_screen", 11},
    { "p_mode", 12},
    { "p_hk", 13},
    { "hk_save", 14},
    { "cur_disp", 15},
    { "im_list", 16},
    { "p_pos", 17},
    { "hk_data2", 18},
    { "dspw_form", 19},
    { "cur_pic", 20},
    { "numarrays", 1},			/* inst */
    { "echo_me", 2},
    { "status_report", 3},
    { "deactivate", 4},
    { "hkbuf", 5},
    { "protection", 6},
    { "scbreg_val", 7},
    { "camera_power", 8},
    { "arrayd2a", 9},
    { "servo", 10},
    { "filters", 11},
    { "a2d_freeze", 12},
    { "spad_mode", 13},
    { "wheel_pos", 14},
    { "cntrl_reg", 4},			/* seq */
    { "int_time", 7},
    { "fint_time", 8},
    { "spad_filter", 9},
    { "ucode_gvars", 10},
    { "", },
    { "", },
    { "", },
    { "", -1},
};

p_arr nodes[] = {
    { "b011", 1},
    { "b016", 1},
    { "seq", 10},
    { "seql0", 10},
    { "seql1", 11},
    { "seqr0", 50},
    { "seqr1", 51},
    { "inst", 2},
    { "dspl0", 100},
    { "dspr0", 200},
    { "dspl1", 101},
    { "dspr1", 201},
    { "dspl2", 102},
    { "dspr2", 202},
    { "dspl3", 103},
    { "dspr3", 203},
    { "", -1},
};

p_arr spad_modes[] = {
    { "efilt", 1},
    { "gain", 2},
    { "mref", 3},
    { "lights", 4},
    { "dbias", 5},
#if defined(SQIID) || defined(IRIM) || defined(CRSP) || defined(IRS) || defined(CIRIM)
    { "high", 0},
    { "low", 1},
    { "hi", 0},
    { "lo", 1},
#elif defined(ALADDIN) || defined(ALAD1024)
    { "on", 1},
    { "off", 0},
    { "f1", 0},
    { "f20", 1},
    { "k1", 1},
    { "k2", 2},
#else
    { "on", 1},
    { "off", 0},
    { "f1", 0},
    { "f7", 1},
    { "f15", 3},
    { "k1", 2},
    { "k2", 1},
#endif
    { "", },
    { "", -1},
};

p_arr efilt_modes[] = {
#if defined(SQIID) || defined(IRIM) || defined(CRSP) || defined(IRS) || defined(CIRIM)
    { "high", 0 },
    { "low", 1 },
#elif defined(ALADDIN) || defined(ALAD1024)
    { "f1", 0},
    { "f20", 1},
#else
    { "f1", 0},
    { "f7", 1},
    { "error (2)", 2},
    { "f15", 3},
#endif
    { "", -1},
};

p_arr gain_modes[] = {
#if defined(SQIID) || defined(IRIM) || defined(CRSP) || defined(IRS) || defined(CIRIM)
    { "high", 0 },
    { "low", 1 },
#elif defined(ALADDIN) || defined(ALAD1024)
    { "error (0)", 0},
    { "k1", 1},
    { "k2", 2},
    { "error (3)", 3},
#else
    { "error (0)", 0},
    { "k2", 1},
    { "k1", 2},
    { "error (3)", 3},
#endif
    { "", -1},
};

