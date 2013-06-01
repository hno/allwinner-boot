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
#include  "include.h"

/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 eGon2_key_init(void)
{
	__u32  reg_val;

	/* control register */
	reg_val  = KEYC_REG_CTRL;
	reg_val &= ~((7<<1) | (0xffU << 24));
	reg_val |=  (0x01 << 6);
	reg_val |=  (0x01 << 0);
	KEYC_REG_CTRL = reg_val;
	//clear pending
	KEYC_REG_INT     = 0;
	KEYC_REG_INT_STATUS |= 0x1f1f;

    return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 eGon2_key_exit(void)
{
    KEYC_REG_CTRL = 0;
	KEYC_REG_INT_STATUS |= 0x1f1f;

    return 0;
}


