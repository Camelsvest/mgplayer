#include "_demo_imdct.h"

#include "mp_core.h"
#include "mp_msg.h"

int g_imdcthandle;
aac_dec_ctx_s g_decoder;

extern int vendor_imdct_type;

void _demo_init_imdct(void)
{
	g_imdcthandle = -1;
	memset( (unsigned char *)&g_decoder, 0, sizeof(g_decoder) );

	if ( vendor_imdct_type > 0 )
	{
		g_imdcthandle = open( "/dev/aac", O_RDWR );
		if ( g_imdcthandle < 0 )
			mp_msg( MSGT_CPLAYER, MSGL_INFO, "### warning: cannot open device for hardware imdct %d\n", g_imdcthandle );
		else
			mp_msg( MSGT_CPLAYER, MSGL_INFO, "### open device for hardware imdct success %d\n", g_imdcthandle );
	}
}

void _demo_uninit_imdct(void)
{
	if ( g_imdcthandle >= 0 )
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### close hardware imdct device ###\n" );
		close( g_imdcthandle );
		g_imdcthandle = -1 ;
	}
}
