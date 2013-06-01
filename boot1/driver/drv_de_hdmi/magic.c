/*
* (C) Copyright 2007-2013
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Martin zheng <zhengjiewen@allwinnertech.com>
*
* See file CREDITS for list of people who contributed to this
* project.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
*/
#include "drv_de.h"


const eGon2_mod_t modinfo =
{
    {'e','G','o','n','2','d','r','v'},		//.magic
    0x01000000,                				//.version
    EGON2_EMOD_TYPE_DRV_DISP,                //.mod id
    0,                                      //.入口地址，驱动(模块)应该填空
	{                                       //.mif
	    &DRV_DE_INIT,
	    &DRV_DE_EXIT,
	    &DRV_DE_OPEN,
	    &DRV_DE_CLOSE,
	    &DRV_DE_READ,
    	&DRV_DE_WRITE,
	    &DRV_DE_IOCTRL,
	    &DRV_DE_Standby
	}
};
