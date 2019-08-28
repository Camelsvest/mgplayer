/* _demo_imdct.h
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

#ifndef __DEMO_IMDCT_H__
#define __DEMO_IMDCT_H__

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "vendor/w55fa92_aac.h"

#ifdef __cplusplus
extern "C" {
#endif

void _demo_init_imdct(void);

void _demo_uninit_imdct(void);

#ifdef __cplusplus
}
#endif

#endif//__DEMO_IMDCT_H__
