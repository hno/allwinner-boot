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
#ifndef	_WBOOT_TWI_H_
#define	_WBOOT_TWI_H_

typedef enum  wboot_twi_cmd_set
{
    WBOOT_TWI_SETFREQUENCY,                                  //设置TWI运行频率
    WBOOT_TWI_
}
wboot_twi_cmd_set_t;

//eGon2内置了twi驱动，这是驱动和外界传递数据使用的数据结构
typedef struct  _twi_init_arg
{
	__u32   twi_control_index;      //TWI控制器编号，0对应控制器0,1对应控制器1，依次类推
	__u32   twi_run_clock;          //TWI运行的时钟频率，以 Hz 为单位
}
_twi_init_arg_t;

typedef struct  _twi_arg
{
    __u32    slave_addr;             //从设备地址
    __u32    slave_addr_flag;        //从设置地址宽度  0：7bit； 1：10bit
	__u8    *byte_addr;              //需要读写的数据在从设备中的地址
	__u32    byte_addr_width;        //从设置地址宽度  0或1：8 bit; 其它：数字代表字节数
	__u32    byte_count;             //一次读写要完成的字节数
	__u32    if_restart;             //是否需要发送restart命令
	__u8    *data;                   //数据的地址
}
_twi_arg_t;




#endif	//_WBOOT_TWI_H_

/* end of _WBOOT_TWI_H_ */

