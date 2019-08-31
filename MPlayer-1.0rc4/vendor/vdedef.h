//------------------------------------------------------------------------------
// File: vdodef.h
//
// Copyright (c) Nuvoton Electronics Corp.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef VDODEF_H_INCLUDED
#define VDODEF_H_INCLUDED
#if 0
typedef unsigned char	Uint8;
typedef unsigned int	Uint32;
typedef unsigned short	Uint16;
typedef Uint32 PhysicalAddress;

typedef unsigned char	UINT8;
typedef unsigned int	UINT32;
typedef unsigned short	UINT16;

#define     PRJ_BODA_DX_5B	0xE606
#define 	MAX_FRAME				(36)     // AVC REF 16, REC 1 , FULL DUP 4	
#define NUM_FRAME_BUF			MAX_FRAME
#define TRUE 1
#define FALSE 0
#define STREAM_END_SIZE			( 0 )

#define STREAM_FILL_SIZE		( 512 * 4 )  //  4 * 1024 | 512 | 512+256( wrap around test )

#define     Console_Printf  printf
//typedef Uint32 PhysicalAddress;

//------------------------------------------------------------------------------
// common struct and definition
//------------------------------------------------------------------------------
//Interrupt Status bit number 
typedef enum {
     INT_INIT_COMPLETE_NUM=0,   //0
     INT_SEQ_INIT_NUM,          //1
     INT_SEQ_END_NUM,           //2
     INT_PIC_RUN_NUM,           //3
     INT_SET_FRAME_BUF_NUM,     //4
     INT_ENC_HEADER_NUM,        //5
     INT_ENC_PARA_SET_NUM,      //6
     INT_DEC_PARA_SET_NUM,      //7
     INT_DEC_BUF_FLUSH_NUM,     //8
     INT_BUF_EMPTY_NUM=14,      //14
     INT_BUF_FUL_NUM            //15
} EmuInterruptEnableBit;

typedef enum {
	STD_MPEG4 = 0 ,
	STD_H263,
	STD_AVC
} CodStd;

typedef enum {
	RETCODE_SUCCESS,
	RETCODE_FAILURE,
	RETCODE_INVALID_HANDLE,
	RETCODE_INVALID_PARAM,
	RETCODE_INVALID_COMMAND,
	RETCODE_ROTATOR_OUTPUT_NOT_SET,
	RETCODE_ROTATOR_STRIDE_NOT_SET,
	RETCODE_FRAME_NOT_COMPLETE,
	RETCODE_INVALID_FRAME_BUFFER,
	RETCODE_INSUFFICIENT_FRAME_BUFFERS,
	RETCODE_INVALID_STRIDE,
	RETCODE_WRONG_CALL_SEQUENCE,
	RETCODE_CALLED_BEFORE,
	RETCODE_NOT_INITIALIZED,
	RETCODE_DEBLOCKING_OUTPUT_NOT_SET,
	RETCODE_NOTSUPPORTED
} RetCode;

typedef enum {
	ENABLE_ROTATION,
	DISABLE_ROTATION,
	ENABLE_MIRRORING,
	DISABLE_MIRRORING,
	SET_MIRROR_DIRECTION,
	SET_ROTATION_ANGLE,
	SET_ROTATOR_OUTPUT,
	SET_ROTATOR_STRIDE,
	DEC_SET_SPS_RBSP,
	DEC_SET_PPS_RBSP,
	DEC_SET_DEBLOCK_OUTPUT
} CodecCommand;

typedef struct {
	PhysicalAddress bufY;
	PhysicalAddress bufCb;
	PhysicalAddress bufCr;
} FrameBuffer;

typedef struct
{
    Uint32    left;
    Uint32    top;
    Uint32    right;
    Uint32    bottom;
} Rect;


typedef enum {
	MIRDIR_NONE,
	MIRDIR_VER,
	MIRDIR_HOR,
	MIRDIR_HOR_VER
} MirrorDirection;
		
struct CodecInst;

//------------------------------------------------------------------------------
// decode struct and definition
//------------------------------------------------------------------------------

typedef struct CodecInst DecInst;
typedef DecInst * DecHandle;

typedef struct {
	CodStd bitstreamFormat;
    int  sorensonEnable;
	PhysicalAddress bitstreamBuffer;
	int  bitstreamBufferSize;
	int  mp4DeblkEnable;
	int  reorderEnable;			
	int  filePlayEnable;
	int  picWidth;
	int  picHeight;
	int	 dynamicAllocEnable;
	int	streamStartByteOffset;
} DecOpenParam;

typedef struct {
	int picWidth;			// {(PicX+15)/16} * 16
	int picHeight;			// {(PicY+15)/16} * 16
	Uint32 frameRateInfo;
	Rect picCropRect;
	int mp4_dataPartitionEnable;
	int mp4_reversibleVlcEnable;
	int mp4_shortVideoHeader;
	int h263_annexJEnable;
	int minFrameBufferCount;
	int frameBufDelay;		
	int	nextDecodedIdxNum;
} DecInitialInfo;

typedef struct{
	PhysicalAddress sliceSaveBuffer;
	int sliceSaveBufferSize;
} DecAvcSliceBufInfo;

typedef	struct{
	DecAvcSliceBufInfo avcSliceBufInfo;
} DecBufInfo;

typedef struct {
	int prescanEnable;
	int prescanMode;	
	int dispReorderBuf;
	int iframeSearchEnable;
	int skipframeMode;
	int skipframeNum;
	int chunkSize;
	int	picStartByteOffset;
	PhysicalAddress picStreamBufferAddr;
} DecParam;

typedef struct {
	int indexFrameDisplay;
	int indexFrameDecoded;
	int picType;
	int numOfErrMBs;
	int prescanresult;
	int	indexFrameNextDecoded[3];
	int decodingSuccess;
	int	useDirectModeForVc1;		// for VC1 RTL bug (to detect error currupted frame)
} DecOutputInfo;

typedef struct {
	Uint32 * paraSet;
	int size;
} DecParamSet;

typedef struct {
	int  Index;
	int  AddrY;
	int  AddrCb;
	int  AddrCr;
	int  StrideY;
	int  StrideC;
	
	int  DispY;
	int  DispCb;
	int  DispCr;

	int  MvColBuf;
} FRAME_BUF;


typedef struct {
	int  bitFormat;
	int sorenson;
	int mode264;
	int rotAngle;
	int mirDir;
	int useRot;
	int outNum;
	int prescanEnable;
	int checkeos;
	int mp4DeblkEnable;
	int iframeSearchEnable;
	int chromaInterleave;
} DecConfigParam;

typedef struct
{
	RetCode				ret;
	DecHandle			handle;
	DecOpenParam		decOP;
	DecParam			decParam;
	DecInitialInfo		initialInfo;
	Uint32				fRateInfo;
	Uint32				framebufWidth;
	Uint32				framebufHeight;
	DecConfigParam		decConfig;
	int					needFrameBufCount;
	int					minAllocateBuf;
	int					i;
	FRAME_BUF *			pFrame[NUM_FRAME_BUF];
	FrameBuffer			frameBuf[NUM_FRAME_BUF];
	int					stride;
	DecBufInfo			decSliceBufInfo;
	int					rotIdx;
	int					deblkIdx;
	int					dispIdx;
	MirrorDirection 	mirrorDirection;
	int					rotStride;
	DecOutputInfo		outputInfo;
	int					needSeqInit;
	int					waitingEn;
	int					frameIdx;
	int					BITPicRunOK;
	int					BITBufferEmptyOK;
	Uint32				Intflag;
	int					decodefinish;
	int					saveIdx;
	int					count;
	unsigned int		virtualY;
	int					prevDispIdx;
	int					totalNumofErrMbs;
	FRAME_BUF			FrameBuf[MAX_FRAME];
} S_VDE2;

void _demo_init_vde(void);

void _demo_uninit_vde(void);

#include "../libavcodec/avcodec.h"

int _demo_decoder_seqinit( AVCodecContext *avctx );

#else
typedef enum {
	RETCODE_SUCCESS
} RetCode;

typedef struct {
	int chunkSize;
} DecParam;

typedef struct
{
	int					needSeqInit;
	RetCode				ret;
	DecParam			decParam;
	int					framebufWidth;
	int					framebufHeight;
	unsigned int		virtualY;
	int					prevDispIdx;
} S_VDE2;

#ifdef __cplusplus
extern "C" {
#endif

void _demo_init_vde(void);

void _demo_uninit_vde(void);

#include "../libavcodec/avcodec.h"

int _demo_decoder_seqinit( AVCodecContext *avctx );

#ifdef __cplusplus
}
#endif

#endif
#endif	//VDODEF_H_INCLUDED
