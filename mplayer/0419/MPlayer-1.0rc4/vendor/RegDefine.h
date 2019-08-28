//------------------------------------------------------------------------------
// File: RegDefine.h
//
//------------------------------------------------------------------------------

#ifndef REGDEFINE_H_INCLUDED
#define REGDEFINE_H_INCLUDED

#include "DrvVDE.h"
//------------------------------------------------------------------------------
// REGISTER BASE
//------------------------------------------------------------------------------
#if 0
#define BIT_BASE                0xB1007000
#define VDO_BA					BIT_BASE
#else
///extern  unsigned int BIT_BASE;                
extern S_VDE g_vde;
#endif

//------------------------------------------------------------------------------
// HARDWARE REGISTER
//------------------------------------------------------------------------------
#define BIT_CODE_RUN                (g_vde.BIT_BASE + 0x000)
#define BIT_CODE_DOWN               (g_vde.BIT_BASE + 0x004)
#define BIT_INT_REQ                 (g_vde.BIT_BASE + 0x008)
#define BIT_INT_CLEAR               (g_vde.BIT_BASE + 0x00C)
#define BIT_CODE_RESET				(g_vde.BIT_BASE + 0x014)							
#define BIT_CUR_PC                  (g_vde.BIT_BASE + 0x018)

//------------------------------------------------------------------------------
// GLOBAL REGISTER
//------------------------------------------------------------------------------
#define BIT_CODE_BUF_ADDR           (g_vde.BIT_BASE + 0x100)
#define BIT_WORK_BUF_ADDR           (g_vde.BIT_BASE + 0x104)
#define BIT_PARA_BUF_ADDR           (g_vde.BIT_BASE + 0x108)
#define BIT_BIT_STREAM_CTRL         (g_vde.BIT_BASE + 0x10C)
#define BIT_FRAME_MEM_CTRL          (g_vde.BIT_BASE + 0x110)
#define CMD_DEC_DISPLAY_REORDER     (g_vde.BIT_BASE + 0x114)
#define	BIT_BIT_STREAM_PARAM		(g_vde.BIT_BASE + 0x114)

#define BIT_RD_PTR_0                (g_vde.BIT_BASE + 0x120)
#define BIT_WR_PTR_0                (g_vde.BIT_BASE + 0x124)
#define BIT_RD_PTR_1                (g_vde.BIT_BASE + 0x128)
#define BIT_WR_PTR_1                (g_vde.BIT_BASE + 0x12C)
#define BIT_RD_PTR_2                (g_vde.BIT_BASE + 0x130)
#define BIT_WR_PTR_2                (g_vde.BIT_BASE + 0x134)
#define BIT_RD_PTR_3                (g_vde.BIT_BASE + 0x138)
#define BIT_WR_PTR_3                (g_vde.BIT_BASE + 0x13C)
#define BIT_SEARCH_RAM_BASE_ADDR	(g_vde.BIT_BASE + 0x140)
#define BIT_SEARCH_RAM_SIZE			(g_vde.BIT_BASE + 0x144)
#define	BIT_FRM_DIS_FLG_0			(g_vde.BIT_BASE + 0x150)
#define	BIT_FRM_DIS_FLG_1			(g_vde.BIT_BASE + 0x154)
#define	BIT_FRM_DIS_FLG_2			(g_vde.BIT_BASE + 0x158)
#define	BIT_FRM_DIS_FLG_3			(g_vde.BIT_BASE + 0x15C)

#define BIT_BUSY_FLAG               (g_vde.BIT_BASE + 0x160)
#define BIT_RUN_COMMAND             (g_vde.BIT_BASE + 0x164)
#define BIT_RUN_INDEX               (g_vde.BIT_BASE + 0x168)
#define BIT_RUN_COD_STD             (g_vde.BIT_BASE + 0x16C)
#define BIT_INT_ENABLE              (g_vde.BIT_BASE + 0x170)
#define BIT_INT_REASON              (g_vde.BIT_BASE + 0x174)


#define BIT_CMD_0                   (g_vde.BIT_BASE + 0x1E0)
#define BIT_CMD_1                   (g_vde.BIT_BASE + 0x1E4)

#define BIT_MSG_0                   (g_vde.BIT_BASE + 0x1F0)
#define BIT_MSG_1                   (g_vde.BIT_BASE + 0x1F4)
#define BIT_MSG_2                   (g_vde.BIT_BASE + 0x1F8)
#define BIT_MSG_3                   (g_vde.BIT_BASE + 0x1FC)


//------------------------------------------------------------------------------
// [DEC SEQ INIT] COMMAND
//------------------------------------------------------------------------------
#define CMD_DEC_SEQ_BB_START        (g_vde.BIT_BASE + 0x180)
#define CMD_DEC_SEQ_BB_SIZE         (g_vde.BIT_BASE + 0x184)
#define CMD_DEC_SEQ_OPTION          (g_vde.BIT_BASE + 0x188)
#define CMD_DEC_SEQ_SRC_SIZE        (g_vde.BIT_BASE + 0x18C)
#define CMD_DEC_SEQ_START_BYTE		(g_vde.BIT_BASE + 0x190)
#define CMD_DEC_SEQ_INIT_ESCAPE		(g_vde.BIT_BASE + 0x114)
#define CMD_DEC_SEQ_MP4_CLASS       (g_vde.BIT_BASE + 0x19C)

#define RET_DEC_SEQ_SUCCESS         (g_vde.BIT_BASE + 0x1C0)
#define RET_DEC_SEQ_SRC_FMT         (g_vde.BIT_BASE + 0x1C4)
#define RET_DEC_SEQ_SRC_SIZE        (g_vde.BIT_BASE + 0x1C4)
#define RET_DEC_SEQ_SRC_F_RATE      (g_vde.BIT_BASE + 0x1C8)
#define RET_DEC_SEQ_FRAME_NEED      (g_vde.BIT_BASE + 0x1CC)
#define RET_DEC_SEQ_FRAME_DELAY     (g_vde.BIT_BASE + 0x1D0)
#define RET_DEC_SEQ_INFO            (g_vde.BIT_BASE + 0x1D4)
#define RET_DEC_SEQ_CROP_LEFT_RIGHT (g_vde.BIT_BASE + 0x1D8)
#define RET_DEC_SEQ_CROP_TOP_BOTTOM (g_vde.BIT_BASE + 0x1DC)
#define	RET_DEC_SEQ_NEXT_FRAME_NUM	(g_vde.BIT_BASE + 0x1E0)

//------------------------------------------------------------------------------
// [DEC PIC RUN] COMMAND
//------------------------------------------------------------------------------
#define CMD_DEC_PIC_ROT_MODE        (g_vde.BIT_BASE + 0x180)
#define CMD_DEC_PIC_ROT_ADDR_Y      (g_vde.BIT_BASE + 0x184)
#define CMD_DEC_PIC_ROT_ADDR_CB     (g_vde.BIT_BASE + 0x188)
#define CMD_DEC_PIC_ROT_ADDR_CR     (g_vde.BIT_BASE + 0x18C)

#define CMD_DEC_PIC_DBK_ADDR_Y		(g_vde.BIT_BASE + 0x190)
#define CMD_DEC_PIC_DBK_ADDR_CB		(g_vde.BIT_BASE + 0x194)
#define CMD_DEC_PIC_DBK_ADDR_CR		(g_vde.BIT_BASE + 0x198)
#define CMD_DEC_PIC_ROT_STRIDE      (g_vde.BIT_BASE + 0x19C)
#define CMD_DEC_PIC_OPTION			(g_vde.BIT_BASE + 0x1A0)
#define	CMD_DEC_PIC_SKIP_NUM		(g_vde.BIT_BASE + 0x1A4)
#define	CMD_DEC_PIC_CHUNK_SIZE		(g_vde.BIT_BASE + 0x1A8)
#define	CMD_DEC_PIC_BB_START		(g_vde.BIT_BASE + 0x1AC)
#define CMD_DEC_PIC_START_BYTE		(g_vde.BIT_BASE + 0x1B0)

#define RET_DEC_PIC_FRAME_NUM       (g_vde.BIT_BASE + 0x1C0)
#define RET_DEC_PIC_FRAME_IDX       (g_vde.BIT_BASE + 0x1C4)
#define RET_DEC_PIC_ERR_MB          (g_vde.BIT_BASE + 0x1C8)
#define RET_DEC_PIC_TYPE            (g_vde.BIT_BASE + 0x1CC)
#define RET_DEC_PIC_OPTION			(g_vde.BIT_BASE + 0x1D4)
#define RET_DEC_PIC_SUCCESS			(g_vde.BIT_BASE + 0x1D8)
#define RET_DEC_PIC_CUR_IDX			(g_vde.BIT_BASE + 0x1DC)
#define	RET_DEC_PIC_NEXT_IDX		(g_vde.BIT_BASE + 0x1E0)

//------------------------------------------------------------------------------
// [SET FRAME BUF] COMMAND
//------------------------------------------------------------------------------
#define CMD_SET_FRAME_BUF_NUM       	(g_vde.BIT_BASE + 0x180)
#define CMD_SET_FRAME_BUF_STRIDE    	(g_vde.BIT_BASE + 0x184)
#define CMD_SET_FRAME_SLICE_BB_START	(g_vde.BIT_BASE + 0x188)
#define CMD_SET_FRAME_SLICE_BB_SIZE		(g_vde.BIT_BASE + 0x18C)

//------------------------------------------------------------------------------
// [DEC_PARA_SET] COMMAND
//------------------------------------------------------------------------------
#define CMD_DEC_PARA_SET_TYPE       (g_vde.BIT_BASE + 0x180)
#define CMD_DEC_PARA_SET_SIZE       (g_vde.BIT_BASE + 0x184)

//------------------------------------------------------------------------------
// [FIRMWARE VERSION] COMMAND  
// [32:16] project number => 
// [16:0]  version => xxxx.xxxx.xxxxxxxx 
//------------------------------------------------------------------------------
#define RET_VER_NUM					(g_vde.BIT_BASE + 0x1c0)


#endif
