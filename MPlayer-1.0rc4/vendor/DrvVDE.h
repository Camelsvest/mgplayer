#if 0
/* DrvVDE.h
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

#ifndef __ASM_ARM_W55FA95_DRVVDE_H
#define __ASM_ARM_W55FA95_DRVVDE_H

#include <unistd.h>

#include <linux/config.h>
///#include <linux/videodev.h>

#include <asm/io.h>

#include <sys/ioctl.h>

typedef struct vde_workbuf{
	unsigned int/*__u32*/	addr;
	unsigned int/*__u32*/	size;
} vde_workbuf_t;

typedef struct vde_frame_info{
	unsigned int/*__u32*/	width;
	unsigned int/*__u32*/	height;
} vde_frame_info_t;



#define VDE_INIT			_IO ('v', 130)
#define VDE_GET_FW_INFO			_IOR('v', 131, unsigned int)
#define VDE_GET_WORKBUF_INFO		_IOR('v', 132, struct vde_workbuf)
#define VDE_GET_BITSTREAM_INFO		_IOR('v', 133, struct vde_workbuf)
#define VDE_GET_FRAME_BUF_INFO		_IOR('v', 134, struct vde_workbuf)
#define VDE_GET_IP_REG_SIZE		_IOR('v', 135, unsigned int)
#define VDE_GET_SLICE_BUF_INFO		_IOR('v', 136, struct vde_workbuf)
#define VDE_WAIT_INTERRUPT		_IOR('v', 137, unsigned int)
#define VDE_GET_RESERVED_FRAME_INFO	_IOR('v', 138, struct vde_frame_info)
#define VDE_GET_MIN_FRAME_NUM		_IOR('v', 139, unsigned int)

typedef struct
{
	int					vde_fd;
	unsigned int		IPsize;
	unsigned char		*pIPReg;
	unsigned int		BIT_BASE;
	vde_workbuf_t		bitstreambuf;
	unsigned char		*pVBitStartBuffer;
	unsigned char		*pVBitStreamBuffer;
	vde_workbuf_t		workbuf;
	unsigned char		*pVworkbuf;
	vde_workbuf_t		framebuf;
	unsigned char		*pFrameBuf;
	unsigned char		*pVirtualFrameBuf;
	vde_workbuf_t		slicebuf;
	vde_frame_info_t	alloc_frame;
} S_VDE;

#endif//__ASM_ARM_W55FA95_DRVVDE_H
#else
#ifndef __DEMO_VDE_H__
#define __DEMO_VDE_H__

#include <linux/types.h>

// ioctl flag, should be consistent with driver definition
#define FAVC_IOCTL_DECODE_GET_BSSIZE		0x4186
#define FAVC_IOCTL_DECODE_GET_OUTPUT_INFO	0x4188
#define FAVC_IOCTL_DECODE_INIT				0x4170
#define FAVC_IOCTL_DECODE_FRAME				0x4172

/*
Version 1.0: Draft release
*/
 
#define H264VER          0x00010000
#define H264VER_MAJOR    (H264VER>>16)
#define H264VER_MINOR    (H264VER&0xffff)

#define FAVC_DECODER_DEV  "/dev/w55fa92_264dec"

typedef struct avc_workbuf
{
	__u32 addr;
	__u32 size;
	__u32 offset;
} avc_workbuf_t;

typedef struct
{
	__u32 bEndOfDec;				// Used by driver
	__u32 u32Width;					// Decoded bitstream width
	__u32 u32Height;				// Decoded bitstream height
	__u32 u32UsedBytes;				// Reported used bitstream byte in buffer. It is not accurate. The inaccuracy is less than 256 bytes
	__u32 u32FrameNum;				// Decoded slice number
//	Uint8 * pu8FrameBaseAddr_phy;	// output frame buffer
//	Uint8 * pu8FrameBaseAddr_U_phy;	// frame buffer (U) if output format is yuv420
//	Uint8 * pu8FrameBaseAddr_V_phy;	// frame buffer (V) if output format is yuv420
	__u32 isDisplayOut;				// 0 -> Buffer in reorder buffer, 1 -> available buffer, -1 -> last flush frame
	__u32 isISlice;					// 1-> I Slice, 0 -> P slice
	__u32 Reserved0;
} FAVC_DEC_RESULT;

typedef struct
{
	__u32 u32API_version;
	__u32 u32MaxWidth;				// Not used now
	__u32 u32MaxHeight;				// Not used now
	__u32 u32FrameBufferWidth;		// LCM buffer width (in pixel)
	__u32 u32FrameBufferHeight;		// LCM buffer height (in pixel)
//	__u32 u32CacheAlign;			// Not used now
	__u32 u32Pkt_size;				// Current decoding bitstream length
	__u8 *pu8Pkt_buf;				// Current decoding bitstream buffer address
	__u32 pu8Display_addr[3];		// Buffer address for decoded data
	__u32 got_picture;				// 0 -> Decoding has someting error. 1 -> decoding is OK in current bitstream
//	__u32 u32BS_buf_sz;				// Reserved bitstream buffer size for decoding (Updated by driver in initialization)
	__u8 * pu8BitStream_phy;		// physical address. buffer for bitstream
//	__u8 * pu8BitStream;			// virtual address.
//	__u8 * mb_info_phy;				// physical address for MB info (Updated by driver in initialization)
//	__u8 * intra_pred_phy;			// Physical address for intra predication (Updated by driver in initialization)
	__u32 u32OutputFmt;				// Decoded output format, 0-> Planar YUV420 format, 1-> Packet YUV422 foramt
	__u32 crop_x;					// pixel unit: crop x start point at decoded-frame (not supported now)
	__u32 crop_y;					// pixel unit: crop y start point at decoded-frame (not supported now)
	FAVC_DEC_RESULT tResult;		// Return decoding result by driver
} FAVC_DEC_PARAM;

typedef struct
{
	int					vde_fd;
	int					bitstreamsize;
	avc_workbuf_t		outputbuf;
	unsigned char		*pVBitStartBuffer;
	unsigned char		*pVBitStreamBuffer;
	FAVC_DEC_PARAM		tDecParam;
} S_VDE;

#endif//__DEMO_VDE_H__
#endif
