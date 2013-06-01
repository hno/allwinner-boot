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
#include "include.h"

extern void jump_to( __u32 addr );
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
void eGon2_jump_to(__u32 addr)
{
	eGon2_power_set_data_buffer(0x0e);
	eGon2_timer_exit();			//关闭timer
    eGon2_key_exit();           //关闭按键
    eGon2_twi_exit();           //关闭TWI设备
    eGon2_block_device_exit(); //关闭用到的存储设备
    close_sys_int( );      // close system interrupt
	eGon2_Int_Exit( );     // 关闭所有中断
	disable_icache();      // 关闭icache
	mmu_disable();		   // 关闭mmu
    flush_dcache();		   // 关闭dcache
    disable_dcache();
    jump_to( addr );

    return;
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
void eGon2_jump_to_android_linux(__s32 zero, __s32 mod_id, __u32 paddr, __u32 kernal_addr)
{
	void (*kernel_entry)(int zero, int arch, uint params);
	eGon2_power_set_data_buffer(0x0e);
	eGon2_timer_exit();			//关闭timer
    eGon2_key_exit();           //关闭按键
    eGon2_twi_exit();           //关闭TWI设备
    eGon2_block_device_exit(); //关闭用到的存储设备
    close_sys_int( );      // close system interrupt
	eGon2_Int_Exit( );     // 关闭所有中断
	disable_icache();      // 关闭icache
	mmu_disable();		   // 关闭mmu
    flush_dcache();		   // 关闭dcache
    disable_dcache();

	kernel_entry = (void (*)(int, int, __u32))(kernal_addr);
	kernel_entry(zero, mod_id, paddr);

    return;
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
void eGon2_jump_Fel( void )
{
	eGon2_power_set_data_buffer(0);
	eGon2_timer_exit();			//关闭timer
    eGon2_key_exit();           //关闭按键
    eGon2_twi_exit();           //关闭TWI设备
	eGon2_block_device_exit(); //关闭用到的存储设备
	close_sys_int( );                   // close system interrupt
	set_vect_high_addr( );              // set interrupt vector low address
	eGon2_Int_Exit( );     // 关闭所有中断
	disable_icache();      // 关闭icache
	mmu_disable();		   // 关闭mmu
    flush_dcache();		   // 关闭dcache
    disable_dcache();
    {
    	__u32 timedly;

    	for(timedly = 0; timedly < 5000; timedly ++);
    }
	jump_to( FEL_BASE );

	return ;
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
void eGon2_simple_jump_Fel( void )
{
	eGon2_timer_exit();	   //关闭timer
    eGon2_key_exit();      //关闭按键
    eGon2_twi_exit();      //关闭TWI设备
	close_sys_int( );      // close system interrupt
	set_vect_high_addr( ); // set interrupt vector low address
	eGon2_Int_Exit( );     // 关闭所有中断
	disable_icache();      // 关闭icache
	mmu_disable();		   // 关闭mmu
    flush_dcache();		   // 关闭dcache
    disable_dcache();
    {
    	__u32 timedly;

    	for(timedly = 0; timedly < 5000; timedly ++);
    }
	jump_to( FEL_BASE );

	return ;
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
void eGon2_power_off(void)
{
	eGon2_power_set_data_buffer(0);
	eGon2_block_device_exit();
	eGon2_set_power_off_vol();

	for(;;);
}
