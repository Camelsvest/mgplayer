#include <sys/mman.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "vdedef.h"
#include "DrvVDE.h"
#include "VpuApi.h"

#include "mp_core.h"
#include "mp_msg.h"

#include "../libavcodec/avcodec.h"

S_VDE g_vde;
S_VDE2 g_vde2;
#if 0
static void CheckVersion(void)
{
	
	RetCode ret = RETCODE_SUCCESS;
		
///	char productstr[18]={0};
	char versionstr[18]={0};
	UINT32 versioninfo;///,size;
	UINT16 pn;
	UINT16 version;
	UINT8  ipprjnum;
///	vde_workbuf_t	workbuf;
	
	ret = VPU_GetVersionInfo( &versioninfo );	
	
	if( ret != RETCODE_SUCCESS )
	{
	    mp_msg( MSGT_CPLAYER, MSGL_INFO, "vde GetVersion failed !\n");
	    exit_player(EXIT_NONE);
	}         	
  
	//printf("versioninfo = 0x%x\n", versioninfo); 
	
	pn = (Uint16)(versioninfo>>16);
	version = (Uint16)versioninfo;
	ipprjnum = (Uint8)(pn);
	

	if (pn != PRJ_BODA_DX_5B)
	{	
	    mp_msg( MSGT_CPLAYER, MSGL_INFO, "Video Codec ID is wrong\n");
	    exit_player(EXIT_NONE);
	}

			
	sprintf( versionstr, "%04d.%04d.%08d", (version>>(12))&0x0f, (version>>(8))&0x0f, (version)&0xff );
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "vde Firmware Version => %s\n\n", versionstr );	

}

static void VideoDecodeMap(void)
{
	UINT32 volatile vworkbuf,tmpsize;
	
	
	// Get Video Decoder IP Register size
	if((ioctl(g_vde.vde_fd, VDE_GET_IP_REG_SIZE, &g_vde.IPsize)) < 0)		
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Get IP Reg size fail\n");
		goto _demo_error_;
	}	
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "vde IP Reg size = 0x%x\n",g_vde.IPsize);
	
	
	g_vde.pIPReg = mmap(NULL, g_vde.IPsize, PROT_READ|PROT_WRITE, MAP_SHARED, g_vde.vde_fd, 0);
	g_vde.BIT_BASE = (UINT32)g_vde.pIPReg;
	if(g_vde.pIPReg == MAP_FAILED)
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Map vde IP Reg Failed!\n");
		goto _demo_error_;
	}	
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Mapped vde IP Reg Virtual addr = 0x%x\n",(unsigned int)g_vde.pIPReg);	
	
	// Get Bitstream Buffer Information	
	if((ioctl(g_vde.vde_fd, VDE_GET_BITSTREAM_INFO, &g_vde.bitstreambuf)) < 0)		
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Get vde bitstream info fail\n");
		goto _demo_error_;
	}	
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Get Bistream Buffer Physical addr = 0x%x, size = 0x%x,\n",g_vde.bitstreambuf.addr,g_vde.bitstreambuf.size);
	
	g_vde.pVBitStartBuffer = mmap(NULL, g_vde.bitstreambuf.size, PROT_READ|PROT_WRITE, MAP_SHARED, g_vde.vde_fd, 0x1000);
	g_vde.pVBitStreamBuffer = g_vde.pVBitStartBuffer;
	if(g_vde.pVBitStreamBuffer == MAP_FAILED)
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Map vde Bitstream Buffer Failed!\n");
		goto _demo_error_;
	}	
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Mapped Bitstream Buffer Virtual addr = 0x%x\n",(unsigned int)g_vde.pVBitStreamBuffer);
	
	// Get Working Buffer Information
	if((ioctl(g_vde.vde_fd, VDE_GET_WORKBUF_INFO, &g_vde.workbuf)) < 0)	
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Get vde WORKBUF info fail\n");
		goto _demo_error_;
	}	
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Get vde physical WorkBuf addr= 0x%x size = 0x%x\n",g_vde.workbuf.addr, g_vde.workbuf.size);
	
	g_vde.pVworkbuf = mmap(NULL, (g_vde.workbuf.size + 4095)/4096*4096, PROT_READ|PROT_WRITE, MAP_SHARED, g_vde.vde_fd, 0x1000 + (g_vde.bitstreambuf.size + 4095)/4096*4096);
	vworkbuf = (UINT32)g_vde.pVworkbuf;
	if((void *)vworkbuf == MAP_FAILED)
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Map vpe Work Buffer Failed!\n");
		goto _demo_error_;
	}
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Mapped vde virtual WorkBuf addr= 0x%x\n",vworkbuf);			
	
	// Get Frame Buffer Information
	if((ioctl(g_vde.vde_fd, VDE_GET_FRAME_BUF_INFO, &g_vde.framebuf)) < 0)		
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Get Frame Buffer info fail\n");
		goto _demo_error_;
	}	
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Frame Buffer physical addr= 0x%x size = 0x%x\n",g_vde.framebuf.addr, g_vde.framebuf.size);
	g_vde.pFrameBuf = (unsigned char *)g_vde.framebuf.addr;
	
	tmpsize = 0x1000 + (g_vde.bitstreambuf.size + 4095)/4096 * 4096  + (g_vde.workbuf.size + 4095)/4096*4096;
	g_vde.pVirtualFrameBuf = mmap(NULL, (g_vde.framebuf.size+4095)/4096*4096, PROT_READ|PROT_WRITE, MAP_SHARED, g_vde.vde_fd, tmpsize);		
	if(g_vde.pVirtualFrameBuf == MAP_FAILED)
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Frame Buffer Map Failed!\n");
		goto _demo_error_;
	}	
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Mapped Frame Buffer Virtual addr = 0x%x\n",(unsigned int)g_vde.pVirtualFrameBuf);	
	
	// Get Slice Buffer Information	
	if((ioctl(g_vde.vde_fd, VDE_GET_SLICE_BUF_INFO, &g_vde.slicebuf)) < 0)		
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Get Slice Buffer info fail\n");
		goto _demo_error_;
	}	
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Slice Buffer physical addr= 0x%x size = 0x%x\n",g_vde.slicebuf.addr, g_vde.slicebuf.size);	
	
	// Init vde and download vde firmware code
	VPU_Init(g_vde.workbuf.addr, vworkbuf);
	
	// Get vde firmware version
	CheckVersion();
	
	return;

_demo_error_:
	close(g_vde.vde_fd);
	exit_player(EXIT_NONE);
}

void _demo_init_vde(void)
{
	char vde_device[] = "/dev/vde";
	int i32Counter = 0;

	memset( (unsigned char *)&g_vde, 0, sizeof(g_vde) );

_RETRY_OPEN:

	g_vde.vde_fd = open(vde_device, O_RDWR);		
	if (g_vde.vde_fd < 0)
	{
		mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s error %d, re-try open ###\n", vde_device, i32Counter );
		if ( i32Counter++ >= 100 )
		{
			mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s error, exit MPlayer !!! ###\n", vde_device );
			exit_player(EXIT_NONE);
		}

		usleep( 500000 );
		goto _RETRY_OPEN;

///		exit_player(EXIT_NONE);
///		return;
	}	
	else
		mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s successfully ###\n", vde_device );

	VideoDecodeMap();
}

void _demo_uninit_vde(void)
{
	if ( g_vde.pIPReg )
	{
		munmap( g_vde.pIPReg, g_vde.IPsize );
		g_vde.pIPReg = NULL;
	}

	if ( g_vde.pVBitStartBuffer )
	{
		munmap( g_vde.pVBitStartBuffer, g_vde.bitstreambuf.size );
		g_vde.pVBitStartBuffer = NULL;
	}

	if ( g_vde.pVworkbuf )
	{
		munmap( g_vde.pVworkbuf, ((g_vde.workbuf.size + 4095)/4096*4096) );
		g_vde.pVworkbuf = NULL;
	}

	if ( g_vde.pVirtualFrameBuf )
	{
		munmap( g_vde.pVirtualFrameBuf, ((g_vde.framebuf.size+4095)/4096*4096) );
		g_vde.pVirtualFrameBuf = NULL;
	}

	if ( g_vde.vde_fd > 0 )
	{
		close( g_vde.vde_fd );
		g_vde.vde_fd = -1;
	}
}

static FRAME_BUF *GetFrameBuffer(int index)
{
    return &g_vde2.FrameBuf[index];
}

static void FrameBufferInit(int picX, int picY,int StartAddr)
{
    int  Ysize;
    int  addrY;
    int  i;
    int  Cbsize;

//printf("X= 0x%x, Y = 0x%x, Addr = 0x%x\n",picX, picY, StartAddr);

    Ysize = ((picX+15)& ~15) * ((picY+15) & ~15);
    Cbsize = Ysize/4;       // YUV420
    
    addrY = StartAddr;    
	
    for (i=0; i<MAX_FRAME; i++) {
        g_vde2.FrameBuf[i].Index  = i;

        g_vde2.FrameBuf[i].AddrY = addrY;
        g_vde2.FrameBuf[i].AddrCb = g_vde2.FrameBuf[i].AddrY  + Ysize;
        g_vde2.FrameBuf[i].AddrCr = g_vde2.FrameBuf[i].AddrCb + Cbsize;
        addrY = g_vde2.FrameBuf[i].AddrCr + Cbsize;
        
        g_vde2.FrameBuf[i].StrideY = picX;
        g_vde2.FrameBuf[i].StrideC = picX/2;

    }
    
}

int _demo_decoder_seqinit( AVCodecContext *avctx )
{
#if 0
	PhysicalAddress paWrPtr,paRdPtr;
	int size = 0;
#endif

	g_vde2.ret = VPU_DecOpen(&g_vde2.handle, &g_vde2.decOP);
	if( g_vde2.ret != RETCODE_SUCCESS )
	{
		av_log( avctx, AV_LOG_INFO, "VPU_DecOpen failed Error code is 0x%x \n", g_vde2.ret );
		goto ERR_DEC_INIT;
	}

#if 0
	g_vde2.ret = VPU_DecGetBitstreamBuffer(g_vde2.handle, &paRdPtr, &paWrPtr, (Uint32 *)&size);
	if( g_vde2.ret != RETCODE_SUCCESS )
	{
		av_log( avctx, AV_LOG_INFO, "VPU_DecGetBitstreamBuffer failed Error code is 0x%x \n", g_vde2.ret );
		goto ERR_DEC_OPEN;				
	}
#endif

	g_vde2.ret = VPU_DecUpdateBitstreamBuffer(g_vde2.handle, g_vde2.decParam.chunkSize);
	if( g_vde2.ret != RETCODE_SUCCESS )
	{
		av_log( avctx, AV_LOG_INFO, "VPU_DecGetBitstreamBuffer failed Error code is 0x%x \n", g_vde2.ret );
		goto ERR_DEC_OPEN;				
	}

	VPU_DecSetEscSeqInit( g_vde2.handle, 1 );
	g_vde2.ret = VPU_DecGetInitialInfo(g_vde2.handle, &g_vde2.initialInfo);
//printf("VPU_DecGetInitialInfo  g_vde2.ret = 0x%x\n",g_vde2.ret);		
	if( g_vde2.ret != RETCODE_SUCCESS )
	{
		//Console_Printf("VPU_DecGetInitialInfo failed Error code is 0x%x \n", g_vde2.ret );
		if(g_vde2.ret == RETCODE_NOTSUPPORTED)
		{
			av_log( avctx, AV_LOG_INFO, "VPU_DecGetInitialInfo failed Error code is 0x%x (Not supported)\n", g_vde2.ret );
			av_log( avctx, AV_LOG_INFO, "VPU SIZE X = %d Y = %d\n", g_vde2.initialInfo.picWidth,g_vde2.initialInfo.picHeight );
		}
		else
			av_log( avctx, AV_LOG_INFO, "VPU_DecGetInitialInfo failed Error code is 0x%x \n", g_vde2.ret );
	
		goto ERR_DEC_OPEN;
	}
	VPU_DecSetEscSeqInit( g_vde2.handle, 0 );
	
	
	g_vde2.fRateInfo = g_vde2.initialInfo.frameRateInfo;

	g_vde2.framebufWidth = ( ( g_vde2.initialInfo.picWidth + 15 ) & ~15 );
	g_vde2.framebufHeight = ( ( g_vde2.initialInfo.picHeight + 15 ) & ~15 );
	
	
#if 1	
{
int framerate;
framerate = (g_vde2.fRateInfo & 0xffff)%(((g_vde2.fRateInfo >> 16) + 1)&0xffff);
	av_log( avctx, AV_LOG_INFO, "Width: %u picHeight: %u frameRate: %d.%2d",
		g_vde2.initialInfo.picWidth, g_vde2.initialInfo.picHeight, (int)(g_vde2.fRateInfo & 0xffff)/(((g_vde2.fRateInfo >> 16) + 1)&0xffff),framerate);
}		
					
#else		
	Console_Printf("Dec InitialInfo =>\n minframeBuffercount: %u\n picWidth: %u\n picHeight: %u\n frameRate: %.2f\n frRes: %u\n frDiv: %u\n nextDecodedNum: %u\n",
		g_vde2.initialInfo.minFrameBufferCount, g_vde2.initialInfo.picWidth, g_vde2.initialInfo.picHeight, (double)(g_vde2.fRateInfo & 0xffff)/
		(((g_vde2.fRateInfo >> 16) + 1)&0xffff), g_vde2.fRateInfo & 0xffff, g_vde2.fRateInfo >> 16, g_vde2.initialInfo.nextDecodedIdxNum);		
#endif	

	if( g_vde2.decConfig.useRot )
		g_vde2.needFrameBufCount = g_vde2.initialInfo.minFrameBufferCount * 2;
	else
		g_vde2.needFrameBufCount = g_vde2.initialInfo.minFrameBufferCount  + ( ( g_vde2.decOP.mp4DeblkEnable == 1 )? 2 : 0 );        
		
av_log( avctx, AV_LOG_INFO, ", FrameBufCnt=%d\n",g_vde2.needFrameBufCount);	

	if((ioctl(g_vde.vde_fd, VDE_GET_RESERVED_FRAME_INFO, &g_vde.alloc_frame)) < 0)		
	{
		close(g_vde.vde_fd);
		av_log( avctx, AV_LOG_INFO,"Get Reserved frame info fail\n");
		goto ERR_DEC_OPEN;
	}	
	av_log( avctx, AV_LOG_INFO,"fa95 vde allocate max frame size = %d x %d\n",g_vde.alloc_frame.width,g_vde.alloc_frame.height);	
	
	if((ioctl(g_vde.vde_fd, VDE_GET_MIN_FRAME_NUM, &g_vde2.minAllocateBuf)) < 0)		
	{
		close(g_vde.vde_fd);
		av_log( avctx, AV_LOG_INFO,"Get min Allocate buffer number fail\n");
		goto ERR_DEC_OPEN;
	}
	av_log( avctx, AV_LOG_INFO,"Get min Allocate buffer number = %d\n",g_vde2.minAllocateBuf);		
	
	av_log( avctx, AV_LOG_INFO,"Required Buffer size = %d, Driver allocate %d\n",g_vde2.framebufWidth * g_vde2.framebufHeight * g_vde2.needFrameBufCount*3/2, 
			(g_vde.alloc_frame.width * g_vde.alloc_frame.height * g_vde2.minAllocateBuf*3/2+4095) / 4096 * 4096);	
	if (g_vde2.framebufWidth * g_vde2.framebufHeight * g_vde2.needFrameBufCount > g_vde.alloc_frame.width * g_vde.alloc_frame.height * g_vde2.minAllocateBuf)
	{
		av_log( avctx, AV_LOG_INFO,"Required Buffer is larger than driver allocated\n");
		goto ERR_DEC_OPEN;
	}		

    // Buffer allocation is dependent on application.    
	av_log( avctx, AV_LOG_INFO,"### g_vde2.initialInfo.picWidth = %d, g_vde2.initialInfo.picHeight = %d ###\n", g_vde2.initialInfo.picWidth, g_vde2.initialInfo.picHeight);
	FrameBufferInit( g_vde2.initialInfo.picWidth, g_vde2.initialInfo.picHeight,(int)g_vde.pFrameBuf );
	
	for( g_vde2.i = 0; g_vde2.i < g_vde2.needFrameBufCount; ++g_vde2.i ) 
	{
		g_vde2.pFrame[g_vde2.i] = GetFrameBuffer(g_vde2.i);
		g_vde2.frameBuf[g_vde2.i].bufY = g_vde2.pFrame[g_vde2.i]->AddrY;
		g_vde2.frameBuf[g_vde2.i].bufCb = g_vde2.pFrame[g_vde2.i]->AddrCb;
		g_vde2.frameBuf[g_vde2.i].bufCr = g_vde2.pFrame[g_vde2.i]->AddrCr;
		
		//printf("g_vde2.i=%x,Y=0x%x, Cb=0x%x, Cr=0x%x\n",g_vde2.i,g_vde2.frameBuf[g_vde2.i].bufY , g_vde2.frameBuf[g_vde2.i].bufCb,g_vde2.frameBuf[g_vde2.i].bufCr);
	}	
	
	g_vde2.stride = g_vde2.framebufWidth ;
	
	// Register frame buffers requested by the decoder.
	//printf("slicebuf.addr = 0x%x, slicebuf.size = 0x%x\n",slicebuf.addr, slicebuf.size);
	g_vde2.decSliceBufInfo.avcSliceBufInfo.sliceSaveBuffer = g_vde.slicebuf.addr;
	g_vde2.decSliceBufInfo.avcSliceBufInfo.sliceSaveBufferSize = g_vde.slicebuf.size;
	
	// Register frame buffers requested by the decoder.

	g_vde2.ret = VPU_DecRegisterFrameBuffer( g_vde2.handle, g_vde2.frameBuf, g_vde2.initialInfo.minFrameBufferCount, g_vde2.stride, &g_vde2.decSliceBufInfo );
	
	if( g_vde2.ret != RETCODE_SUCCESS )
	{
		av_log( avctx, AV_LOG_INFO,"VPU_DecRegisterFrameBuffer failed Error code is 0x%x \n", g_vde2.ret );
		goto ERR_DEC_OPEN;
	}
	//printf("VPU_DecRegisterFrameBuffer\n");
	
	g_vde2.decParam.prescanEnable = g_vde2.decConfig.prescanEnable;
	
	if( g_vde2.decConfig.useRot )
	{
		g_vde2.rotIdx = g_vde2.initialInfo.minFrameBufferCount;
		if( g_vde2.decOP.mp4DeblkEnable == 1 )
			g_vde2.deblkIdx = g_vde2.rotIdx + 2;			
			
		g_vde2.dispIdx = g_vde2.rotIdx;			
	}
	else
	{
		if( g_vde2.decOP.mp4DeblkEnable == 1 )
		{
			g_vde2.deblkIdx = g_vde2.initialInfo.minFrameBufferCount;
			g_vde2.dispIdx = g_vde2.deblkIdx;				
    	}
    	else
			g_vde2.dispIdx = 0;    	
	}
	
		
	if (g_vde2.decConfig.useRot)
	{
		VPU_DecGiveCommand(g_vde2.handle, SET_ROTATION_ANGLE, &g_vde2.decConfig.rotAngle);
		g_vde2.mirrorDirection = g_vde2.decConfig.mirDir;
		VPU_DecGiveCommand(g_vde2.handle, SET_MIRROR_DIRECTION, &g_vde2.mirrorDirection);

		g_vde2.rotStride = ( g_vde2.decConfig.rotAngle == 90 || g_vde2.decConfig.rotAngle == 270 ) ? g_vde2.framebufHeight : g_vde2.framebufWidth;
			
		VPU_DecGiveCommand(g_vde2.handle, SET_ROTATOR_STRIDE, &g_vde2.rotStride);	    
    }

///	g_vde2.decParam.chunkSize = 0;
///	g_vde.pVBitStreamBuffer = g_vde.pVBitStartBuffer;

	return 0;

ERR_DEC_OPEN:
    av_log( avctx, AV_LOG_INFO, "### ERR_DEC_OPEN ###\n" );

	// Now that we are done with decoding, close the open instance.
	g_vde2.ret = VPU_DecClose(g_vde2.handle);
	if( g_vde2.ret == RETCODE_FRAME_NOT_COMPLETE )
	{
		VPU_DecGetOutputInfo(g_vde2.handle, &g_vde2.outputInfo);
		VPU_DecClose(g_vde2.handle);
	}

ERR_DEC_INIT:
	av_log( avctx, AV_LOG_INFO, "### ERR_DEC_INIT ###\n" );

	return -1;
}
#else
void _demo_init_vde(void)
{
	int i32Counter = 0;

	memset( (unsigned char *)&g_vde, 0, sizeof(g_vde) );

_RETRY_OPEN_:

	g_vde.vde_fd = open( FAVC_DECODER_DEV, O_RDWR );
	if ( g_vde.vde_fd <= 0 )
	{
		mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s error %d, re-try open ###\n", FAVC_DECODER_DEV, i32Counter );
		if ( i32Counter++ >= 100 )
		{
			mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s error, exit MPlayer !!! ###\n", FAVC_DECODER_DEV );
			exit_player( EXIT_NONE );
		}

		usleep( 500000 );
		goto _RETRY_OPEN_;
	}
	else
		mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s successfully ###\n", FAVC_DECODER_DEV );

	if ( (ioctl( g_vde.vde_fd, FAVC_IOCTL_DECODE_GET_BSSIZE, &g_vde.bitstreamsize )) < 0 )
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### Get vde bitstream size fail ###\n" );
		goto _DEMO_ERROR_;
	}
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "### g_vde.bitstreamsize = %d ###\n", g_vde.bitstreamsize );

	if ( (ioctl( g_vde.vde_fd, FAVC_IOCTL_DECODE_GET_OUTPUT_INFO, &g_vde.outputbuf )) < 0 )
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### Get output buffer size fail ###\n" );
		goto _DEMO_ERROR_;
	}
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "### g_vde.outputbuf.addr = 0x%x, g_vde.outputbuf.size = %d, g_vde.outputbuf.offset = %d ###\n", g_vde.outputbuf.addr, g_vde.outputbuf.size, g_vde.outputbuf.offset );

	g_vde.pVBitStartBuffer = mmap( NULL, g_vde.bitstreamsize, PROT_READ|PROT_WRITE, MAP_SHARED, g_vde.vde_fd, 4096 );
	g_vde.pVBitStreamBuffer = g_vde.pVBitStartBuffer;
	if( g_vde.pVBitStreamBuffer == MAP_FAILED )
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "Map vde Bitstream Buffer Failed!\n");
		goto _DEMO_ERROR_;
	}
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "Mapped Bitstream Buffer Virtual addr = 0x%x\n",(unsigned int)g_vde.pVBitStreamBuffer);

	return;

_DEMO_ERROR_:

	close( g_vde.vde_fd );
	exit_player( EXIT_NONE );
}

void _demo_uninit_vde(void)
{
	if ( g_vde.pVBitStartBuffer )
	{
		munmap( g_vde.pVBitStartBuffer, g_vde.bitstreamsize );
		g_vde.pVBitStartBuffer = NULL;
	}

	if ( g_vde.vde_fd > 0 )
	{
		close( g_vde.vde_fd );
		g_vde.vde_fd = -1;
	}
}

int _demo_decoder_seqinit( AVCodecContext *avctx )
{
	memset( &g_vde.tDecParam, 0, sizeof(FAVC_DEC_PARAM) );
	g_vde.tDecParam.u32API_version = H264VER;

	g_vde.tDecParam.u32OutputFmt = 0; // 1->Packet YUV422 format, 0-> planar YUV420 foramt

	g_vde.tDecParam.u32FrameBufferWidth = -1;
	g_vde.tDecParam.u32FrameBufferHeight = -1;

	// Output : Packet YUV422 or Planar YUV420
	if ( (ioctl( g_vde.vde_fd, FAVC_IOCTL_DECODE_INIT, &g_vde.tDecParam )) < 0 )
	{
		av_log( avctx, AV_LOG_INFO, "### FAVC_IOCTL_DECODE_INIT: memory allocation failed ###\n" );
		return -1;
	}

	g_vde.tDecParam.pu8Pkt_buf = g_vde.pVBitStartBuffer;
	g_vde2.prevDispIdx = 0;

	return RETCODE_SUCCESS;
}
#endif
