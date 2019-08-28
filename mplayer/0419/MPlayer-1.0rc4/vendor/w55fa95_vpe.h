/* DrvVPE.h
 *
 * Copyright (c) 2009 Nuvoton technology corporation
 * All rights reserved.
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __ASM_ARM_W55FA95_DRVVPE_H
#define __ASM_ARM_W55FA95_DRVVPE_H

//#include <linux/config.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
//#include <asm/io.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev.h>

/* VPE source format */
typedef enum
{
	VPE_SRC_PLANAR_YONLY  =0,	 
	VPE_SRC_PLANAR_YUV420 =1,
	VPE_SRC_PLANAR_YUV411 =2,
	VPE_SRC_PLANAR_YUV422 =3,		
	VPE_SRC_PLANAR_YUV422T=5,	
	VPE_SRC_PLANAR_YUV444 = 9,
	
	VPE_SRC_PACKET_YUV422 = 12,
	VPE_SRC_PACKET_RGB555 = 13,
	VPE_SRC_PACKET_RGB565 = 14,
	VPE_SRC_PACKET_RGB888 = 15
}E_VPE_SRC_FMT;
/* VPE destination format */
typedef enum 
{
	VPE_DST_PACKET_YUV422=0,
	VPE_DST_PACKET_RGB555,
	VPE_DST_PACKET_RGB565,
	VPE_DST_PACKET_RGB888
}E_VPE_DST_FMT;

/* operation */
typedef enum 
{
	VPE_OP_NORMAL=0x0,
	VPE_OP_RIGHT=0x1,
	VPE_OP_LEFT,
	VPE_OP_UPSIDEDOWN,
	VPE_OP_FLIP,
	VPE_OP_FLOP,
	VPE_DDA_SCALE
}E_VPE_CMD_OP;

/* scale algorithm */
typedef enum
{
	VPE_SCALE_DDA = 0,				/* 3x3 and Bilinear are disabled */
	//VPE_SCALE_3X3 = 1,				/* Only enable 3x3, Not support now. It has to be approached by 2 steps*/
	VPE_SCALE_BILINEAR = 2,			/* Only enable Bilinear */
	//VPE_SCALE_3X3_BILINEAR = 3		/* Both downscale are enabled, Not support now */
}E_VPE_SCALE_MODE;

/* frame mode or on the fly mode */
typedef enum
{
	VPE_HOST_FRAME  =0,	 		// Block base (8x8 or 16x16)
	VPE_HOST_VDEC_LINE =1,		// Line base. for H264, H.263 annex-j. (4x4 block) 
	VPE_HOST_JPEG =2,			// 
	VPE_HOST_VDEC_SW =3		// Software, Block base 
}E_VPE_HOST;

typedef struct vpe_transform{
	__u32	src_addrPacY;
	__u32	src_addrU;
	__u32	src_addrV;
	__u32	src_format;
	__u32	src_width;
	__u32	src_height;	
	__u32	src_leftoffset;
	__u32	src_rightoffset;	

	__u32	dest_addrPac;
	__u32	dest_format;
	__u32	dest_width;
	__u32	dest_height;
	__u32	dest_leftoffset;
	__u32	dest_rightoffset;
	
	__u32	algorithm;
	E_VPE_CMD_OP	rotation;
} vpe_transform_t;

#define VPE_INIT						_IO ('k', 130)
#define VPE_IO_SET					_IOW('k', 131, unsigned int)
#define VPE_IO_GET					_IOR('k', 132, vpe_transform_t *)
//#define VPE_IO_GET					_IOR('k', 132, unsigned int)
#define VPE_GET_MMU_MODE			_IOR('k', 133, unsigned int)
#define VPE_SET_FORMAT_TRANSFORM	_IOW('k', 134, vpe_transform_t *)
#define VPE_WAIT_INTERRUPT			_IO ('k', 135)
#define VPE_TRIGGER					_IO ('k', 136)
#define VPE_STOP						_IO ('k', 137)
#define VPE_POLLING_INTERRUPT			_IOR('k', 138, unsigned int)
#define VPE_SET_MMU_MODE			_IOW('k', 139, unsigned int)

#define IOCTL_LCD_GET_DMA_BASE          _IOR('v', 32, unsigned int *)

#define VIDEO_FORMAT_CHANGE			_IOW('v', 50, unsigned int)	//frame buffer format change
#define DISPLAY_MODE_RGB555	0
#define DISPLAY_MODE_RGB565	1
#define DISPLAY_MODE_CBYCRY	4
#define DISPLAY_MODE_YCBYCR	5
#define DISPLAY_MODE_CRYCBY	6
#define DISPLAY_MODE_YCRYCB	7

typedef struct
{
	int					vpe_fd;
	int					value;
	unsigned int		vpostBuffer;
	unsigned char		*center;
	int					wait_complete;
	unsigned int		ptr_y;
	unsigned int		ptr_u;
	unsigned int		ptr_v;
	int					total_x;
	int					total_y;
	int					PAT_WIDTH;
	int					PAT_HEIGHT;
	unsigned short		lcm_width;
	unsigned short		lcm_height;
	unsigned short		lcm_bits_per_pixel;
	unsigned short		lcm_bytes_per_pixel;
	int					is_init;
	int					offset0;
	int					offset1;
	int					offset2;
	int					current_scale_type;
	int					current_rotate_type;
	vpe_transform_t		vpe_setting;
	unsigned char		demo_mmu_mode;
} S_VPE;

void _demo_init_vpe(void);

void _demo_uninit_vpe(void);

void _demo_flip_page2(void);

#endif//__ASM_ARM_W55FA95_DRVVPE_H
