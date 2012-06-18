/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON twi sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : wboot_twi.h
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
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

