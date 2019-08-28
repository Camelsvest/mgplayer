#include "config.h"

#include "_demo_osd.h"

S_OSDDEMO g_osddemo;

typedef struct
{
	int i32Status;
	int i32Offset;
	int i32Size;
	int i32TargetX;
	int i32TargetY;
	int i32TargetOffset;
	int i32Width;
	int i32Height;
} S_VENDOR_OSD;

typedef struct
{
	int i32ColorKeyStatus;
	int i32ColorKeyValue;
} S_VENDOR_COLORKEY;

typedef struct
{
	int i32MplayerStatus;
	int i32MplayerCheck1;
	int i32MplayerCheck2;
	volatile int i32MplayerLockStatus;
	float pos;
	float time_length;
} S_VENDOR_SYNC;

static S_VENDOR_OSD * s_psVendorOsd;

static S_VENDOR_COLORKEY * s_psVendorColorkey;

static S_VENDOR_SYNC * s_psVendorSync;

void _demo_init_osd(void)
{
	if ( (g_osddemo.shmid = shmget( (key_t)g_osddemo.vendor_osd_shmid, g_osddemo.vendor_osd_shmsize, 0 )) == -1 )
	{
		g_osddemo.vendor_shm = -1;
		mp_msg(MSGT_CPLAYER, MSGL_INFO, "### Skipped g_osddemo.shmid, g_osddemo.vendor_pindex = %d. ###\n", g_osddemo.vendor_pindex);
	}
	else
	{
		mp_msg(MSGT_CPLAYER, MSGL_INFO, "### get g_osddemo.shmid success = %d, g_osddemo.vendor_pindex = %d. ###\n", g_osddemo.vendor_osd_shmid, g_osddemo.vendor_pindex);

		if ( (g_osddemo.shm_start = (char *)shmat( g_osddemo.shmid, NULL, 0 )) == NULL )
			mp_msg(MSGT_CPLAYER, MSGL_INFO, "### get g_osddemo.shm_start failed, g_osddemo.vendor_pindex = %d. ###\n", g_osddemo.vendor_pindex);
		else
		{
			g_osddemo.vendor_shm = 1;
			s_psVendorOsd = (S_VENDOR_OSD *)g_osddemo.shm_start;
			s_psVendorColorkey = (S_VENDOR_COLORKEY *)((unsigned int)g_osddemo.shm_start + sizeof(S_VENDOR_OSD) * g_osddemo.vendor_osd_count);
			s_psVendorSync = (S_VENDOR_SYNC *)((unsigned int)s_psVendorColorkey + sizeof(S_VENDOR_COLORKEY));
			mp_msg(MSGT_CPLAYER, MSGL_INFO, "### get g_osddemo.shm_start success, size = %d, g_osddemo.vendor_pindex = %d. ###\n", g_osddemo.vendor_osd_shmsize, g_osddemo.vendor_pindex);
			mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> s_psVendorSync->i32MplayerStatus = %d. ###\n", g_osddemo.vendor_pindex, s_psVendorSync->i32MplayerStatus);
			mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> s_psVendorSync->i32MplayerCheck1 = %d. ###\n", g_osddemo.vendor_pindex, s_psVendorSync->i32MplayerCheck1);
			mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> s_psVendorSync->i32MplayerCheck2 = %d. ###\n", g_osddemo.vendor_pindex, s_psVendorSync->i32MplayerCheck2);
			mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> s_psVendorSync->i32MplayerLockStatus = %d. ###\n", g_osddemo.vendor_pindex, s_psVendorSync->i32MplayerLockStatus);
			mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> s_psVendorSync->pos = %.1f.\n", g_osddemo.vendor_pindex, s_psVendorSync->pos );
			mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> s_psVendorSync->time_length = %.2lf.\n", g_osddemo.vendor_pindex, s_psVendorSync->time_length );
		}
	}
}

void _demo_init_osd2(void)
{
	g_osddemo.center2 = g_osddemo.frame_buffer;
#if 0
	mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> g_osddemo.center = 0x%x.\n", g_osddemo.vendor_pindex, (unsigned int)g_osddemo.center );
	mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> g_osddemo.center2 = 0x%x.\n", g_osddemo.vendor_pindex, (unsigned int)g_osddemo.center2 );
	mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> g_osddemo.frame_buffer = 0x%x.\n", g_osddemo.vendor_pindex, (unsigned int)g_osddemo.frame_buffer );
	mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> g_osddemo.frame_buffer2 = 0x%x.\n", g_osddemo.vendor_pindex, (unsigned int)g_osddemo.frame_buffer2 );
	mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> g_osddemo.fb_page = %d.\n", g_osddemo.vendor_pindex, g_osddemo.fb_page );
	mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> g_osddemo.fb_line_len = %d.\n", g_osddemo.vendor_pindex, g_osddemo.fb_line_len );
	mp_msg(MSGT_CPLAYER, MSGL_INFO, "### %d --> g_osddemo.fb_yres = %d.\n", g_osddemo.vendor_pindex, g_osddemo.fb_yres );
#endif
	if ( g_osddemo.fb_page == 0 )
	{
		memcpy( g_osddemo.frame_buffer2, g_osddemo.frame_buffer, g_osddemo.fb_yres * g_osddemo.fb_line_len );
	}
}

void _demo_uninit_osd2(void)
{
	ioctl( g_osddemo.vpost_fd, IOCTL_FB_LOCK_RESET );
///	ioctl( g_osddemo.vpost_fd, IOCTL_FB_UNLOCK );
}

void _demo_draw_osd(void)
{
	uint8_t *pu8FB;
	uint8_t *pu8OsdFB;
	uint16_t *pu16FB;
	uint16_t *pu16OsdFB;
	uint32_t *pu32FB;
	uint32_t *pu32OsdFB;
	int i, j, height, width;
	unsigned int u32ColorKeyValue, osd_line_len, tempvalue;

///	mp_msg(MSGT_VO, MSGL_INFO, "### g_osddemo.vendor_osd_level = %d ###\n", g_osddemo.vendor_osd_level);

	if ( (g_osddemo.vendor_osd_level == 1) && (g_osddemo.vendor_shm == 1) )
	{
		if ( s_psVendorColorkey->i32ColorKeyStatus == 0 )
		{
			for ( i = 0; i < g_osddemo.vendor_osd_count; i++ )
			{
				switch ( s_psVendorOsd[i].i32Status )
				{
				case 0:
					break;
				case 1:
					pu8FB = (unsigned char *)((unsigned int)g_osddemo.center + s_psVendorOsd[i].i32TargetOffset);
					pu8OsdFB = (unsigned char *)((unsigned int)g_osddemo.shm_start + s_psVendorOsd[i].i32Offset);
					osd_line_len = s_psVendorOsd[i].i32Width * g_osddemo.lcm_bytes_per_pixel;
					for ( height = 0; height < s_psVendorOsd[i].i32Height; height++ )
					{
						memcpy( pu8FB, pu8OsdFB, osd_line_len );
						pu8FB = (unsigned char *)((unsigned int)pu8FB + g_osddemo.fb_line_len);
						pu8OsdFB = (unsigned char *)((unsigned int)pu8OsdFB + osd_line_len);
					}
					break;
				default:
					break;
				}
			}
		}
		else
		{
			u32ColorKeyValue = s_psVendorColorkey->i32ColorKeyValue;
			for ( i = 0; i < g_osddemo.vendor_osd_count; i++ )
			{
				switch ( s_psVendorOsd[i].i32Status )
				{
				case 0:
					break;
				case 1:
					j = 0;
					if ( g_osddemo.lcm_bytes_per_pixel == 2 )
					{
						pu16FB = (unsigned short *)((unsigned int)g_osddemo.center + s_psVendorOsd[i].i32TargetOffset);
						pu16OsdFB = (unsigned short *)((unsigned int)g_osddemo.shm_start + s_psVendorOsd[i].i32Offset);
						for ( height = 0; height < s_psVendorOsd[i].i32Height; height++ )
						{
							for ( width = 0; width < s_psVendorOsd[i].i32Width; width++ )
							{
								tempvalue = pu16OsdFB[j++];
								if ( tempvalue != u32ColorKeyValue )
									pu16FB[width] = tempvalue;
							}
							pu16FB = (unsigned short *)((unsigned int)pu16FB + g_osddemo.fb_line_len);
						}
					}
					else/* if ( g_osddemo.lcm_bytes_per_pixel == 4 )*/
					{
						pu32FB = (unsigned int *)((unsigned int)g_osddemo.center + s_psVendorOsd[i].i32TargetOffset);
						pu32OsdFB = (unsigned int *)((unsigned int)g_osddemo.shm_start + s_psVendorOsd[i].i32Offset);
						for ( height = 0; height < s_psVendorOsd[i].i32Height; height++ )
						{
							for ( width = 0; width < s_psVendorOsd[i].i32Width; width++ )
							{
								tempvalue = pu32OsdFB[j++];
								if ( tempvalue != u32ColorKeyValue )
									pu32FB[width] = tempvalue;
							}
							pu32FB = (unsigned int *)((unsigned int)pu32FB + g_osddemo.fb_line_len);
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void _demo_draw_osd2(void)
{
	uint8_t *pu8FB;
	uint8_t *pu8OsdFB;
	uint16_t *pu16FB;
	uint16_t *pu16OsdFB;
	uint32_t *pu32FB;
	uint32_t *pu32OsdFB;
	int i, j, height, width;
	unsigned int u32ColorKeyValue, osd_line_len, tempvalue;

	memcpy( g_osddemo.frame_buffer, g_osddemo.frame_buffer2, g_osddemo.fb_yres * g_osddemo.fb_line_len );

///	mp_msg(MSGT_VO, MSGL_INFO, "### g_osddemo.vendor_osd_level = %d ###\n", g_osddemo.vendor_osd_level);
	if ( (g_osddemo.vendor_osd_level == 1) && (g_osddemo.vendor_shm == 1) )
	{
		if ( s_psVendorColorkey->i32ColorKeyStatus == 0 )
		{
			for ( i = 0; i < g_osddemo.vendor_osd_count; i++ )
			{
				switch ( s_psVendorOsd[i].i32Status )
				{
				case 0:
					break;
				case 1:
					pu8FB = (unsigned char *)((unsigned int)g_osddemo.center2 + s_psVendorOsd[i].i32TargetOffset);
					pu8OsdFB = (unsigned char *)((unsigned int)g_osddemo.shm_start + s_psVendorOsd[i].i32Offset);
					osd_line_len = s_psVendorOsd[i].i32Width * g_osddemo.lcm_bytes_per_pixel;
					for ( height = 0; height < s_psVendorOsd[i].i32Height; height++ )
					{
						memcpy( pu8FB, pu8OsdFB, osd_line_len );
						pu8FB = (unsigned char *)((unsigned int)pu8FB + g_osddemo.fb_line_len);
						pu8OsdFB = (unsigned char *)((unsigned int)pu8OsdFB + osd_line_len);
					}
					break;
				default:
					break;
				}
			}
		}
		else
		{
			u32ColorKeyValue = s_psVendorColorkey->i32ColorKeyValue;
			for ( i = 0; i < g_osddemo.vendor_osd_count; i++ )
			{
				switch ( s_psVendorOsd[i].i32Status )
				{
				case 0:
					break;
				case 1:
					j = 0;
					if ( g_osddemo.lcm_bytes_per_pixel == 2 )
					{
						pu16FB = (unsigned short *)((unsigned int)g_osddemo.center2 + s_psVendorOsd[i].i32TargetOffset);
						pu16OsdFB = (unsigned short *)((unsigned int)g_osddemo.shm_start + s_psVendorOsd[i].i32Offset);
						for ( height = 0; height < s_psVendorOsd[i].i32Height; height++ )
						{
							for ( width = 0; width < s_psVendorOsd[i].i32Width; width++ )
							{
								tempvalue = pu16OsdFB[j++];
								if ( tempvalue != u32ColorKeyValue )
									pu16FB[width] = tempvalue;
							}
							pu16FB = (unsigned short *)((unsigned int)pu16FB + g_osddemo.fb_line_len);
						}
					}
					else/* if ( g_osddemo.lcm_bytes_per_pixel == 4 )*/
					{
						pu32FB = (unsigned int *)((unsigned int)g_osddemo.center2 + s_psVendorOsd[i].i32TargetOffset);
						pu32OsdFB = (unsigned int *)((unsigned int)g_osddemo.shm_start + s_psVendorOsd[i].i32Offset);
						for ( height = 0; height < s_psVendorOsd[i].i32Height; height++ )
						{
							for ( width = 0; width < s_psVendorOsd[i].i32Width; width++ )
							{
								tempvalue = pu32OsdFB[j++];
								if ( tempvalue != u32ColorKeyValue )
									pu32FB[width] = tempvalue;
							}
							pu32FB = (unsigned int *)((unsigned int)pu32FB + g_osddemo.fb_line_len);
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}

///	ioctl( g_osddemo.vpost_fd, IOCTL_FB_LOCK_RESET );
	ioctl( g_osddemo.vpost_fd, IOCTL_FB_UNLOCK );
	ioctl( g_osddemo.vpost_fd, IOCTL_WAIT_VSYNC );
	ioctl( g_osddemo.vpost_fd, IOCTL_FB_LOCK );
}

void _demo_set_mplayer_status(enum exit_reason how)
{
	if ( s_psVendorSync == NULL )
	{
		return;
	}
	s_psVendorSync->i32MplayerStatus = how;
}

int _demo_check_mplayer_status(void)
{
	if ( s_psVendorSync == NULL )
	{
		return 0;
	}

	if ( g_osddemo.vendor_pindex == 0 )
	{
		s_psVendorSync->i32MplayerCheck1 = 1;
		if ( s_psVendorSync->i32MplayerCheck2 == 1 )
			return 0;
		return 1;
	}
	else if ( g_osddemo.vendor_pindex == 1 )
	{
		s_psVendorSync->i32MplayerCheck2 = 1;
		if ( s_psVendorSync->i32MplayerCheck1 == 1 )
			return 0;
		return 1;
	}

	return 1;
}

void _demo_set_sync_time(float pos)
{
	if ( s_psVendorSync == NULL )
	{
		return;
	}

	s_psVendorSync->i32MplayerLockStatus = 1;
	s_psVendorSync->pos = pos;
	s_psVendorSync->i32MplayerLockStatus = 0;
}

void _demo_set_vendor_state(enum e_vendor_state vendor_state)
{
	if ( s_psVendorSync == NULL )
	{
		return;
	}

	s_psVendorSync->i32MplayerLockStatus = 1;
	s_psVendorSync->i32MplayerStatus = vendor_state;
	s_psVendorSync->i32MplayerLockStatus = 0;
}

void _demo_set_time_length(float time_length)
{
	if ( s_psVendorSync == NULL )
	{
		return;
	}

	s_psVendorSync->i32MplayerLockStatus = 1;
	s_psVendorSync->time_length = time_length;
	s_psVendorSync->i32MplayerLockStatus = 0;
}
