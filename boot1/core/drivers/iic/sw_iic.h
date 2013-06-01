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
#ifndef _SW_IIC_H_
#define _SW_IIC_H_

#define CFG_SW_IIC_MAX      4
#define CFG_SW_IIC_CTL0	   TWIC0_REGS_BASE	      /* iic0 */
#define CFG_SW_IIC_CTL1	   TWIC1_REGS_BASE	      /* iic1 */
#define CFG_SW_IIC_CTL2	   TWIC2_REGS_BASE	      /* iic2 */

#define SW_IIC_READ        (1)
#define SW_IIC_WRITE       (0)

struct sw_iic
{
    volatile unsigned int addr;        /* slave address     */
    volatile unsigned int xaddr;       /* extend address    */
    volatile unsigned int data;        /* data              */
    volatile unsigned int ctl;         /* control           */
    volatile unsigned int status;      /* status            */
    volatile unsigned int clk;         /* clock             */
    volatile unsigned int srst;        /* soft reset        */
    volatile unsigned int eft;         /* enhanced future   */
};

typedef volatile struct sw_iic *sw_iic_t;

enum  _sw_iic_operation_status
{
    SW_IIC_OPERATION_NO_ERR         = 0,
    SW_IIC_OPERATION_START_NO_INT   = -1

};

#endif /* _SW_IIC_H_ */

/*end of sw_iic.h*/
