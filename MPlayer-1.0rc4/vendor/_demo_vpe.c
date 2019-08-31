#include "w55fa95_vpe.h"

#include "mp_core.h"
#include "mp_msg.h"

S_VPE g_vpe;

extern void _demo_flip_page(void);

void _demo_init_vpe(void)
{
	char vpe_device[] = "/dev/vpe";
	int i32Counter = 0;

	memset( (unsigned char *)&g_vpe, 0, sizeof(g_vpe) );

_RETRY_OPEN:

	g_vpe.vpe_fd = open(vpe_device, O_RDWR);		
	if (g_vpe.vpe_fd < 0)
	{
		mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s error %d, re-try open ###\n", vpe_device, i32Counter );
		if ( i32Counter++ >= 100 )
		{
			mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s error, exit MPlayer !!! ###\n", vpe_device );
			exit_player(EXIT_NONE);
		}

		usleep( 500000 );
		goto _RETRY_OPEN;
///		exit_player(EXIT_NONE);
///		return;
	}	
	else
		mp_msg( MSGT_CPLAYER, MSGL_FATAL, "### open %s successfully ###\n", vpe_device );

	if( ioctl(g_vpe.vpe_fd, VPE_INIT, NULL)  < 0)
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### VPE_INIT fail ###\n" );
		goto _demo_error_;
	}

	g_vpe.value = 0x5a;
	if((ioctl(g_vpe.vpe_fd, VPE_IO_SET, &g_vpe.value)) < 0)
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### VPE_IO_SET fail ###\n" );
		goto _demo_error_;
	}

	g_vpe.demo_mmu_mode = 0;	// 1 mmu mode; 0 non-mmu mode

	if((ioctl(g_vpe.vpe_fd, VPE_SET_MMU_MODE, &g_vpe.demo_mmu_mode)) < 0)
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### VPE_SET_MMU_MODE fail ###\n" );
		goto _demo_error_;
	}

	return;

_demo_error_:
	close(g_vpe.vpe_fd);
	exit_player(EXIT_NONE);
}

void _demo_uninit_vpe(void)
{
	if ( g_vpe.vpe_fd > 0 )
	{
		close( g_vpe.vpe_fd );
		g_vpe.vpe_fd = -1;
	}
}

void _demo_flip_page2(void)
{
	g_vpe.wait_complete = 0;
	_demo_flip_page();
}
