/* _demo_osd.h
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

#ifndef __DEMO_OSD_H__
#define __DEMO_OSD_H__

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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <linux/videodev.h>

#include "mp_core.h"
#include "mp_msg.h"

#define IOCTL_FB_LOCK			_IOW('v', 64, unsigned int)
#define IOCTL_FB_UNLOCK			_IOW('v', 65, unsigned int)
#define IOCTL_FB_LOCK_RESET		_IOW('v', 66, unsigned int)
#define IOCTL_WAIT_VSYNC		_IOW('v', 67, unsigned int)

enum e_vendor_state
{
	VENDOR_STATE_UNKOWN,
	VENDOR_STATE_PLAYING,
	VENDOR_STATE_PAUSING,
	VENDOR_STATE_IDLING
};

typedef struct
{
	int vendor_osd_level;
	int vendor_osd_shmid;
	int vendor_osd_shmsize;
	int vendor_osd_count;
	int vendor_pindex;
	int vendor_state;
	int vendor_shm;
	int shmid;
	char * shm_start;
	uint8_t *center;
	uint8_t *center2;
	int fb_line_len;
	int					vpost_fd;
	unsigned char		*frame_buffer;
	unsigned char		*frame_buffer2;
	int					fb_page;
	int					fb_yres;
	unsigned short		lcm_bytes_per_pixel;
} S_OSDDEMO;

void _demo_init_osd(void);

void _demo_init_osd2(void);

void _demo_uninit_osd2(void);

void _demo_draw_osd(void);

void _demo_draw_osd2(void);

void _demo_set_mplayer_status(enum exit_reason how);

int _demo_check_mplayer_status(void);

void _demo_set_sync_time(float pos);

void _demo_set_vendor_state(enum e_vendor_state vendor_state);

void _demo_set_time_length(float time_length);

#endif//__DEMO_OSD_H__
