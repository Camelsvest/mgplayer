#include "jpegcodec.h"

#include "mp_core.h"
#include "mp_msg.h"

#include <dirent.h>

S_JPEG g_jpeg;
jpeg_param_t g_jpeg_dparam;

#if 0
void _demo_init_hwjpeg(void)
{
	int i;
	char device[] = "/dev/video0";

#define RETRY_NUM 4
	memset( (unsigned char *)&g_jpeg, 0, sizeof(g_jpeg) );
	for ( i = 0; i < RETRY_NUM; ++i )
	{
		g_jpeg.jpeg_fd = open( device, O_RDWR );		//maybe /dev/video1, video2, ...
		if ( g_jpeg.jpeg_fd < 0 )
		{
			mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Error: failed to open %s!\n", device );
			exit_player(EXIT_NONE);
		}

		if ( (ioctl( g_jpeg.jpeg_fd, JPEG_GET_JPEG_BUFFER, &g_jpeg.total_jpeg_buffer_size )) < 0 )
		{
			mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Try to open %s.\n", device );
			close( g_jpeg.jpeg_fd );
			mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Close %s.\n", device );
			device[10]++;
		}
		else
		{
			mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - %s opened. fd = %d.\n", device, g_jpeg.jpeg_fd );
			break;
		}
	}

	if ( (ioctl( g_jpeg.jpeg_fd, JPEG_GET_JPEG_BUFFER, &g_jpeg.total_jpeg_buffer_size)) < 0 )
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Error: ioctl JPEG_GET_JPEG_BUFFER!\n" );
		exit_player(EXIT_NONE);
	}
	else
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Total JPEG buffer size = %d.\n", g_jpeg.total_jpeg_buffer_size );

	g_jpeg.jpeg_buff = mmap(	NULL,
								g_jpeg.total_jpeg_buffer_size,
								(PROT_READ|PROT_WRITE),
								MAP_SHARED,
								g_jpeg.jpeg_fd,
								0
								);

	if ( g_jpeg.jpeg_buff == MAP_FAILED )
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Error: JPEG buffer mmap failed!\n" );
		exit_player(EXIT_NONE);
	}
}
#else
void _demo_init_hwjpeg(void)
{
	DIR *dir;
	unsigned int u32Counter = 0;
	unsigned int u32Flag = 1;
	char device[] = "/dev/video0";

	memset( (unsigned char *)&g_jpeg, 0, sizeof(g_jpeg) );
	/* Check device for jpegcodec "/dev/video0" or "/dev/video1" */
	/* Try to open folder "/sys/class/video4linux/video1/",
		if the folder exists, jpegcodec is "/dev/video1", otherwises jpegcodec is "/dev/video0" */
	dir = opendir("/sys/class/video4linux/video1/");
	if(dir)
	{
		closedir(dir);
		device[10]++;
	}
///	printf("### HW JPEG Decode - Info: JPEGCodec is %s.\n",device);

	for ( ;; )
	{
		g_jpeg.jpeg_fd = open( device, O_RDWR );
		if ( g_jpeg.jpeg_fd < 0 )
		{
			usleep( 10000 );
			if ( u32Flag )
			{
				u32Flag = 0;
				mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Info: JPEG Engine is busy!\n" );
			}
			if ( u32Counter++ > 100 )
			{
				u32Counter = 0;
				mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Info: JPEG Engine is busy!\n" );
			}
			continue;
		}
		else
		{
			break;
		}
	}
///	g_jpeg.is_init = GetTimer();
///	if ( g_jpeg.jpeg_fd < 0 )
	{
///		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Info: JPEG Engine is busy!\n" );
///		exit_player(EXIT_NONE);
	}

	if ( (ioctl( g_jpeg.jpeg_fd, JPEG_GET_JPEG_BUFFER, &g_jpeg.total_jpeg_buffer_size)) < 0 )
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Error: ioctl JPEG_GET_JPEG_BUFFER!\n" );
		exit_player(EXIT_NONE);
	}
///	else
///		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Total JPEG buffer size = %d.\n", g_jpeg.total_jpeg_buffer_size );

	g_jpeg.jpeg_buff = mmap(	NULL,
								g_jpeg.total_jpeg_buffer_size,
								(PROT_READ|PROT_WRITE),
								MAP_SHARED,
								g_jpeg.jpeg_fd,
								0
								);

	if ( g_jpeg.jpeg_buff == MAP_FAILED )
	{
		mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Error: JPEG buffer mmap failed!\n" );
		exit_player(EXIT_NONE);
	}
}

void _demo_uninit_hwjpeg(void)
{
	if ( g_jpeg.jpeg_buff )
	{
		munmap( g_jpeg.jpeg_buff, g_jpeg.total_jpeg_buffer_size );
		g_jpeg.jpeg_buff = NULL;
	}

	if ( g_jpeg.jpeg_fd > 0 )
	{
		close( g_jpeg.jpeg_fd );
		g_jpeg.jpeg_fd = -1;
	}
///	mp_msg( MSGT_CPLAYER, MSGL_INFO, "### HW JPEG - Info: _demo_uninit_hwjpeg\n" );
///	g_jpeg.is_init = GetTimer() - g_jpeg.is_init;
///	mp_msg( MSGT_CPLAYER, MSGL_INFO, "### close - open = %d microsecond ###\n", g_jpeg.is_init);
///	usleep( 10000 );
}
#endif
