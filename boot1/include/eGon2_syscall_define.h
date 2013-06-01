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
#ifndef __SYSCALL_DEFINE_H_
#define __SYSCALL_DEFINE_H_	1

#define  EGON2_SWI_BASE                     0

#define  EGON2_SWI_FOR_TEST     			0x01
#define  EGON2_SWI_GET_PARA                 0x02    //参数传递
#define  EGON2_SWI_JUMP_TO                  0x03    //跳转
#define  EGON2_SWI_JUMP_FEL                 0x04    //跳转到Fel
#define  EGON2_SWI_STANDBY					0x05    //进入standby模式
#define  EGON2_SWI_JUMP_TO_LINUX			0x06	//进入linux模式

//用户注册/卸载中断的reg/unreg
//起始数据编号 0x20
#define  EGON2_SWI_IRQ_BASE                  0x20
#define  EGON2_SWI_IRQ_REG		            (EGON2_SWI_BASE + EGON2_SWI_IRQ_BASE + 0x00)
#define  EGON2_SWI_IRQ_UNREG		        (EGON2_SWI_BASE + EGON2_SWI_IRQ_BASE + 0x01)

#define  EGON2_SWI_ENABLE_IRQ		        (EGON2_SWI_BASE + EGON2_SWI_IRQ_BASE + 0x02)
#define  EGON2_SWI_DISABLE_IRQ	            (EGON2_SWI_BASE + EGON2_SWI_IRQ_BASE + 0x03)

//内置TWI硬件操作
#define  EGON2_SWI_TWI_BASE                  0x2A
#define  EGON2_SWI_TWI_READ                 (EGON2_SWI_BASE + EGON2_SWI_TWI_BASE + 0x00)
#define  EGON2_SWI_TWI_WRITE                (EGON2_SWI_BASE + EGON2_SWI_TWI_BASE + 0x01)
//#define  EGON2_SWI_TWI_INIT                 (EGON2_SWI_BASE + EGON2_SWI_TWI_BASE + 0x02)
//#define  EGON2_SWI_TWI_EXIT                 (EGON2_SWI_BASE + EGON2_SWI_TWI_BASE + 0x03)
//#define  EGON2_SWI_TWI_SETFREQ              (EGON2_SWI_BASE + EGON2_SWI_TWI_BASE + 0x04)
//内存分配
//起始数据编号 0x40
#define  EGON2_SWI_MEM_BASE                  0x30
#define  EGON2_SWI_MALLOC                   (EGON2_SWI_BASE + EGON2_SWI_MEM_BASE + 0x00)
#define  EGON2_SWI_CALLOC                   (EGON2_SWI_BASE + EGON2_SWI_MEM_BASE + 0x01)
#define  EGON2_SWI_REALLOC                  (EGON2_SWI_BASE + EGON2_SWI_MEM_BASE + 0x02)
#define  EGON2_SWI_FREE                     (EGON2_SWI_BASE + EGON2_SWI_MEM_BASE + 0x03)

//内置POWER操作
//起始数据编号 0x38
#define  EGON2_SWI_POWER_BASE                0x38
#define  EGON2_SWI_POWER_GET_SOURCE         (EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x00)
#define  EGON2_SWI_POWER_GET_BAT_VOL        (EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x01)
#define  EGON2_SWI_POWER_GET_KEY            (EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x02)
#define  EGON2_SWI_POWER_CHECK_STARTUP      (EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x03)
#define  EGON2_SWI_POWER_SET_SW1            (EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x04)
#define  EGON2_SWI_POWER_SET_OFF            (EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x05)
#define  EGON2_SWI_POWER_GET_LEVEL_STATUS   (EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x06)
#define  EGON2_SWI_POWER_SET_OFF_VOL	    (EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x07)
#define  EGON2_SWI_POWER_BATTERY_CAL		(EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x08)
#define  EGON2_SWI_POWER_CUR_LIMIT			(EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x09)
#define  EGON2_SWI_POWER_VOL_LIMIT			(EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x0A)
#define  EGON2_SWI_POWER_TYPE				(EGON2_SWI_BASE + EGON2_SWI_POWER_BASE + 0x0B)

//调试接口，起始编号
#define  EGON2_SWI_DEBUG_BASE               0x4A
#define  EGON2_SWI_UART_NTDEBUG				(EGON2_SWI_BASE + EGON2_SWI_DEBUG_BASE - 0x01)
#define  EGON2_SWI_UART_DEBUG			    (EGON2_SWI_BASE + EGON2_SWI_DEBUG_BASE + 0x00)
#define  EGON2_SWI_DISPLAY_DEBUG            (EGON2_SWI_BASE + EGON2_SWI_DEBUG_BASE + 0x01)
#define  EGON2_SWI_UART_GETC                (EGON2_SWI_BASE + EGON2_SWI_DEBUG_BASE + 0x02)
#define  EGON2_SWI_UART_GETS                (EGON2_SWI_BASE + EGON2_SWI_DEBUG_BASE + 0x03)
#define  EGON2_SWI_UART_PUTC                (EGON2_SWI_BASE + EGON2_SWI_DEBUG_BASE + 0x04)
#define  EGON2_SWI_UART_GETC_DELAY			(EGON2_SWI_BASE + EGON2_SWI_DEBUG_BASE + 0x05)
//--------------------------------------------------------------------
//  drv 操作部份   200~299
//--------------------------------------------------------------------
//起始数据编号 0x50
#define  EGON2_SWI_DRV_BASE                 0x50
#define  EGON2_SWI_DRV_REG			        (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x00)
#define  EGON2_SWI_DRV_UNREG			    (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x01)
#define  EGON2_SWI_DRV_OPEN			        (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x02)
#define  EGON2_SWI_DRV_CLOSE			    (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x03)
#define  EGON2_SWI_DRV_READ			        (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x04)
#define  EGON2_SWI_DRV_WRITE			    (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x05)
#define  EGON2_SWI_DRV_IOCTL			    (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x06)

#define  EGON2_SWI_RUN_APP                  (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x07)
//处于方便使用的情况，为用户提供的便捷通道，同时保持高速度
#define  EGON2_SWI_BLOCK_SIZE				(EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x08)

#define  EGON2_SWI_BLOCK_DEV_INIT           (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x09)
#define  EGON2_SWI_BLOCK_DEV_READ           (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x0a)
#define  EGON2_SWI_BLOCK_DEV_WRITE          (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x0b)
#define  EGON2_SWI_BLOCK_DEV_EXIT           (EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x0c)

#define  EGON2_SWI_BLOCK_DDEV_READ			(EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x0d)
#define  EGON2_SWI_BLOCK_DDEV_WRITE			(EGON2_SWI_BASE + EGON2_SWI_DRV_BASE + 0x0e)
//文件操作 起始数据编号 0x60
#define  EGON2_SWI_FS_BASE                  0x60
#define  EGON2_SWI_FS_OPEN			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x00)
#define  EGON2_SWI_FS_READ			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x01)
#define  EGON2_SWI_FS_WRITE			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x02)
#define  EGON2_SWI_FS_TELL			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x03)
#define  EGON2_SWI_FS_LEN			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x04)
#define  EGON2_SWI_FS_SEEK			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x05)
#define  EGON2_SWI_FS_EOF			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x07)
#define  EGON2_SWI_FS_CLOSE			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x08)
#define  EGON2_SWI_FS_RM			        (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x09)

#define  EGON2_SWI_PART_MOUNT               (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x0a)
#define  EGON2_SWI_PART_UNMOUNT             (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x0b)
#define  EGON2_SWI_PART_INFO                (EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x0c)
#define  EGON2_SWI_PART_COUNT				(EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x0d)
#define  EGON2_SWI_PART_START				(EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x0e)
#define  EGON2_SWI_PART_CAPACITY			(EGON2_SWI_BASE + EGON2_SWI_FS_BASE + 0x0f)

//目录操作 起始数据编号 0x70
#define  EGON2_SWI_DIR_BASE                  0x70
#define  EGON2_SWI_DIR_OPEN			        (EGON2_SWI_BASE + EGON2_SWI_DIR_BASE + 0x00)
#define  EGON2_SWI_DIR_CLOSE		        (EGON2_SWI_BASE + EGON2_SWI_DIR_BASE + 0x01)
#define  EGON2_SWI_DIR_MAKE			        (EGON2_SWI_BASE + EGON2_SWI_DIR_BASE + 0x02)
#define  EGON2_SWI_DIR_ERASE    	        (EGON2_SWI_BASE + EGON2_SWI_DIR_BASE + 0x03)
#define  EGON2_SWI_DIR_READ    	            (EGON2_SWI_BASE + EGON2_SWI_DIR_BASE + 0x04)
//--------------------------------------------------------------------
//  timer 操作部份
//--------------------------------------------------------------------
//起始数据编号 0x80
#define  EGON2_SWI_TIMER_BASE               0x80
#define  EGON2_SWI_TIMER_REQ		        (EGON2_SWI_BASE + EGON2_SWI_TIMER_BASE + 0x00)
#define  EGON2_SWI_TIMER_REL		        (EGON2_SWI_BASE + EGON2_SWI_TIMER_BASE + 0x01)
#define  EGON2_SWI_TIMER_START		        (EGON2_SWI_BASE + EGON2_SWI_TIMER_BASE + 0x02)
#define  EGON2_SWI_TIMER_STOP               (EGON2_SWI_BASE + EGON2_SWI_TIMER_BASE + 0x03)
#define  EGON2_SWI_SYSTEM_RESET				(EGON2_SWI_BASE + EGON2_SWI_TIMER_BASE + 0x04)
#define  EGON2_SWI_TIMER_DELAY				(EGON2_SWI_BASE + EGON2_SWI_TIMER_BASE + 0x05)
//--------------------------------------------------------------------
//  dma 操作部份
//--------------------------------------------------------------------
//起始数据编号 0x90
#define  EGON2_SWI_DMA_BASE                 0x90
#define  EGON2_SWI_DMA_REQ		            (EGON2_SWI_BASE + EGON2_SWI_DMA_BASE + 0x00)
#define  EGON2_SWI_DMA_REL		            (EGON2_SWI_BASE + EGON2_SWI_DMA_BASE + 0x01)
#define  EGON2_SWI_DMA_START		        (EGON2_SWI_BASE + EGON2_SWI_DMA_BASE + 0x02)
#define  EGON2_SWI_DMA_STOP 		        (EGON2_SWI_BASE + EGON2_SWI_DMA_BASE + 0x03)
#define  EGON2_SWI_DMA_SETTING 		        (EGON2_SWI_BASE + EGON2_SWI_DMA_BASE + 0x04)
#define  EGON2_SWI_DMA_RESTART 		        (EGON2_SWI_BASE + EGON2_SWI_DMA_BASE + 0x05)
#define  EGON2_SWI_DMA_QUERYSTATE 	        (EGON2_SWI_BASE + EGON2_SWI_DMA_BASE + 0x06)
#define  EGON2_SWI_DMA_QUERYCHAN            (EGON2_SWI_BASE + EGON2_SWI_DMA_BASE + 0x07)
//--------------------------------------------------------------------
//  clock 操作部份
//--------------------------------------------------------------------
//起始数据编号 0xA0
#define  EGON2_SWI_CLOCK_BASE				0xA0
#define  EGON2_SWI_CLOCK					(EGON2_SWI_BASE + EGON2_SWI_CLOCK_BASE + 0x00)
//--------------------------------------------------------------------
//  key 操作部份
//--------------------------------------------------------------------
//起始数据编号 0xA8
#define  EGON2_SWI_KEY_BASE				    0xA8
#define  EGON2_SWI_KEY_GET_STATUS    		(EGON2_SWI_BASE + EGON2_SWI_KEY_BASE + 0x00)
#define  EGON2_SWI_KEY_GET_VALUE    		(EGON2_SWI_BASE + EGON2_SWI_KEY_BASE + 0x01)
//--------------------------------------------------------------------
//  配置脚本 操作部份
//--------------------------------------------------------------------
//起始数据编号 0xB0
#define  EGON2_SWI_SCRIPT_BASE					0xB0
#define  EGON2_SWI_SCRIPT_FETCH					(EGON2_SWI_BASE + EGON2_SWI_SCRIPT_BASE + 0x00)
#define  EGON2_SWI_SCRIPT_SUBKEY_COUNT			(EGON2_SWI_BASE + EGON2_SWI_SCRIPT_BASE + 0x01)
#define  EGON2_SWI_SCRIPT_MAINKEY_COUNT			(EGON2_SWI_BASE + EGON2_SWI_SCRIPT_BASE + 0x02)
#define  EGON2_SWI_SCRIPT_MAINKEY_GPIO_COUNT	(EGON2_SWI_BASE + EGON2_SWI_SCRIPT_BASE + 0x03)
#define  EGON2_SWI_SCRIPT_MAINKEY_GPIO_DATA		(EGON2_SWI_BASE + EGON2_SWI_SCRIPT_BASE + 0x04)
#define  EGON2_SWI_SCRIPT_PATCH		            (EGON2_SWI_BASE + EGON2_SWI_SCRIPT_BASE + 0x05)
//--------------------------------------------------------------------
//  GPIO 操作部份
//--------------------------------------------------------------------
//起始数据编号 0xC0
#define  EGON2_SWI_GPIO_BASE					0xC0
#define  EGON2_SWI_GPIO_REQUEST					(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x00)
#define  EGON2_SWI_GPIO_RELEASE					(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x01)
#define  EGON2_SWI_GPIO_GET_ALL_PIN_STATUS		(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x02)
#define  EGON2_SWI_GPIO_GET_ONE_PIN_STATUS		(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x03)
#define  EGON2_SWI_GPIO_SET_ONE_PIN_STATUS		(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x04)
#define  EGON2_SWI_GPIO_SET_ONE_PIN_IO			(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x05)
#define  EGON2_SWI_GPIO_SET_ONE_PIN_PULL		(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x06)
#define  EGON2_SWI_GPIO_SET_ONE_PIN_DLEVEL		(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x07)
#define  EGON2_SWI_GPIO_SET_ONE_PIN_DATA		(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x08)
#define  EGON2_SWI_GPIO_GET_ONE_PIN_DATA		(EGON2_SWI_BASE + EGON2_SWI_GPIO_BASE + 0x09)


#endif  /*#ifndef __SYSCALL_DEFINE_H_*/


