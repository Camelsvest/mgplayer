#if 0
//------------------------------------------------------------------------------
// File: VpuApi.h
//
//------------------------------------------------------------------------------
#ifndef VPUAPI_H_INCLUDED
#define VPUAPI_H_INCLUDED
//------------------------------------------------------------------------------
// system dependent definition
//------------------------------------------------------------------------------

#define vdePortW32(addr, val)    (*((volatile UINT32 *)(addr))=val)
#define vdePortR32(addr)         (*((volatile UINT32 *)(addr)))
  
#define VpuWriteReg( ADDR, DATA )	vdePortW32(ADDR,DATA) 
#define VpuReadReg( ADDR )		vdePortR32(ADDR)	   
#define VpuWriteMem( ADDR, DATA )	vdePortW32(ADDR,DATA) 
#define VpuReadMem( ADDR )		vdePortR32(ADDR)		


#define CODE_BUF_SIZE (32 * 1024)
#define WORK_BUF_SIZE	(396 * 1024)	// V2.6.1
#define PARA_BUF2_SIZE	(0)
#define PARA_BUF_SIZE	(1 * 1024)



#define IMAGE_ENDIAN			0		// 0 (64 bit little endian), 1 (64 bit big endian), 2 (32 bit swaped little endian), 3 (32 bit swaped big endian)
#define STREAM_ENDIAN			0       // 0 (64 bit little endian), 1 (64 bit big endian), 2 (32 bit swaped little endian), 3 (32 bit swaped big endian)
#define	CBCR_INTERLEAVE			0		// 0 (chroma seperate mode), 1 (chroma interleave mode)

///typedef unsigned int    UINT;
///typedef unsigned char   BYTE;
#define DEFAULT_SEARCHRAM_ADDR  0x280000
#define STREAM_FULL_EMPTY_CHECK_DISABLE 0


#ifdef __cplusplus
extern "C" {
#endif

	int		VPU_IsBusy(void);
	//RetCode VPU_Init(PhysicalAddress workBuf);
	RetCode VPU_Init(PhysicalAddress workBuf, Uint32 virtualworkbuf);
	RetCode VPU_GetVersionInfo( Uint32 *versionInfo );

	// function for decode
	RetCode VPU_DecOpen(DecHandle *, DecOpenParam *);
	RetCode VPU_DecClose(DecHandle);
	RetCode VPU_DecSetEscSeqInit( 
		DecHandle handle, 
		int escape );
	RetCode VPU_DecGetInitialInfo(
		DecHandle handle,
		DecInitialInfo * info);
//#ifndef SLICE_BUF
#if 1
	RetCode VPU_DecRegisterFrameBuffer(
		DecHandle handle,
		FrameBuffer * bufArray,
		int num,
		int stride,
		DecBufInfo *pBufInfo);
#else
	RetCode VPU_DecRegisterFrameBuffer(
		DecHandle handle,
		FrameBuffer * bufArray,
		int num,
		int stride);
#endif
	RetCode VPU_DecGetBitstreamBuffer(
		DecHandle handle,
		PhysicalAddress * prdPrt,
		PhysicalAddress * pwrPtr,
		Uint32 * size );
	RetCode VPU_DecUpdateBitstreamBuffer(
		DecHandle handle,
		Uint32 size);
	RetCode VPU_SleepWake(
		DecHandle handle, 
		int iSleepWake);

	RetCode VPU_DecStartOneFrame( 
		DecHandle handle, 
		DecParam *param ); 
	RetCode VPU_DecGetOutputInfo(
		DecHandle handle,
		DecOutputInfo * info);
	RetCode VPU_DecBitBufferFlush(
		DecHandle handle);
	RetCode VPU_DecClrDispFlag(
		DecHandle handle, int index);
	RetCode VPU_DecGiveCommand(
		DecHandle handle,
		CodecCommand cmd,
		void * parameter);	
	
	
#ifdef __cplusplus
}
#endif

#endif
#else
#endif
