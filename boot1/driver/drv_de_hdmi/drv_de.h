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
#ifndef  _DRV_DE_H_
#define  _DRV_DE_H_

#include "egon2.h"
#include "de_bsp/bsp_display.h"

#define sys_get_value(n)    (*((volatile __u8 *)(n)))          /* byte input */
#define sys_put_value(n,c)  (*((volatile __u8 *)(n))  = (c))   /* byte output */
#define sys_get_hvalue(n)   (*((volatile __u16 *)(n)))         /* half word input */
#define sys_put_hvalue(n,c) (*((volatile __u16 *)(n)) = (c))   /* half word output */
#define sys_get_wvalue(n)   (*((volatile __u32 *)(n)))          /* word input */
#define sys_put_wvalue(n,c) (*((volatile __u32 *)(n))  = (c))   /* word output */
#define sys_set_bit(n,c)    (*((volatile __u8 *)(n)) |= (c))   /* byte bit set */
#define sys_clr_bit(n,c)    (*((volatile __u8 *)(n)) &=~(c))   /* byte bit clear */
#define sys_set_hbit(n,c)   (*((volatile __u16 *)(n))|= (c))   /* half word bit set */
#define sys_clr_hbit(n,c)   (*((volatile __u16 *)(n))&=~(c))   /* half word bit clear */
#define sys_set_wbit(n,c)   (*((volatile __u32 *)(n))|= (c))    /* word bit set */
#define sys_cmp_wvalue(n,c) (c == (*((volatile __u32 *) (n))))
#define sys_clr_wbit(n,c)   (*((volatile __u32 *)(n))&=~(c))

extern   __s32 DRV_DE_INIT(void);
extern   __s32 DRV_DE_EXIT(void);
extern   __u32 DRV_DE_OPEN(__u32 mid, void *p_arg);
extern   __s32 DRV_DE_CLOSE(__u32 hd);
extern   __u32 DRV_DE_READ(void *pdata, __u32 size, __u32 n, __u32 hd);
extern   __u32 DRV_DE_WRITE(const void *pdata, __u32 size, __u32 n, __u32 hd);
extern   __s32 DRV_DE_IOCTRL(__u32 hd, __u32 cmd, __s32 aux, void *pbuffer);
extern   __s32 DRV_DE_Standby(__u32 cmd, void *pArg);

#endif  /* _DRV_FM_H_ */
