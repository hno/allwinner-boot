/*
 * (C) Copyright 2012
 *     wangflord@allwinnertech.com
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 */
#ifndef  __boot0_i_h
#define  __boot0_i_h

#define  UBOOT_BASE  0x4a000000
#define  FPGA_PLATFORM   1
//#define  CONFIG_SUN6I_FPGA
#undef   CONFIG_SUN6I_FPGA

#include "types.h"
#include "storage_media_cfg.h"
#include "boot0_v2.h"
#include "boot1_v2.h"
#include "boot0_misc.h"
#include "arch.h"
#include "drv/boot0_drv.h"
#include "sys_op/sys_op.h"
#include "lib/lib.h"
#include "nand_for_boot.h"
#include "load_Boot1_from_nand/load_Boot1_from_nand.h"
#include "load_boot1_from_sdmmc/load_boot1_from_sdmmc.h"
#include "dram_for_debug.h"
#include "mctl_hal.h"


#endif     //  ifndef __boot0_i_h

/* end of boot0_i.h */
