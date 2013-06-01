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

typedef struct swi_regs
{
    u32 reg[13];     	// R0  ~  R12
    u32 cpsr;      	// spsr on entry to SWI //
    u32 pc;        	// lr on entry to SWI //
}swi_regs_t;
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
void eGon2_swi_handler_entry(__u32 swi_number, struct swi_regs * swi_reg)
{
	switch(swi_number)
	{
/* SWI 测试 */
		case EGON2_SWI_FOR_TEST:
		{
			eGon2_printf("swi_handler_entry()........swi just for fun \n");
		}
		break;

/* standby mode */
		case EGON2_SWI_STANDBY:
		{
			swi_reg->reg[0] = eGon2_standby_mode();
		}
		break;

/* 参数传递 */
        case EGON2_SWI_GET_PARA:
        {
            swi_reg->reg[0] = eGon2_dispatch_parameters(swi_reg->reg[0], (void *)swi_reg->reg[1]);
        }
        break;
/* 跳转服务 */
        case EGON2_SWI_JUMP_TO:
        {
            eGon2_jump_to(swi_reg->reg[0]);
        }
        break;
		case EGON2_SWI_JUMP_FEL:
		{
			eGon2_jump_Fel( );
		}
		break;
		case EGON2_SWI_JUMP_TO_LINUX:
		{
			eGon2_jump_to_android_linux(swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[2], swi_reg->reg[3]);
		}

 /* 中断服务 */
		case EGON2_SWI_IRQ_REG:
		{
			swi_reg->reg[0] = eGon2_InsINT_Func(swi_reg->reg[0], (int *)swi_reg->reg[1], (void *)swi_reg->reg[2]);
		}
		break;


		case EGON2_SWI_IRQ_UNREG:
		{
			swi_reg->reg[0] = eGon2_UnsInt_Func(swi_reg->reg[0]);
		}
		break;

		case EGON2_SWI_ENABLE_IRQ:
		{
		    swi_reg->reg[0] = eGon2_EnableInt(swi_reg->reg[0]);
		}
		break;

		case EGON2_SWI_DISABLE_IRQ:
        {
            swi_reg->reg[0] = eGon2_DisableInt(swi_reg->reg[0]);
        }
        break;
/* timer回调函数申请 */
        case EGON2_SWI_TIMER_REQ:
        {
            swi_reg->reg[0] = eGon2_timer_request((void *)swi_reg->reg[0], (void *)swi_reg->reg[1]);
        }
        break;

        case EGON2_SWI_TIMER_REL:
        {
            swi_reg->reg[0] = eGon2_timer_release(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_TIMER_START:
        {
            swi_reg->reg[0] = eGon2_timer_start(swi_reg->reg[0], swi_reg->reg[1],swi_reg->reg[2]);
        }
        break;

        case EGON2_SWI_TIMER_STOP:
        {
            swi_reg->reg[0] = eGon2_timer_stop(swi_reg->reg[0]);
        }
        break;

		case EGON2_SWI_SYSTEM_RESET:
		{
			eGon2_watchdog_enable();
		}
		break;

		case EGON2_SWI_TIMER_DELAY:
		{
			eGon2_timer_delay(swi_reg->reg[0]);
		}
		break;

/* 内存管理 */
        case EGON2_SWI_MALLOC:
        {
            swi_reg->reg[0] = (__u32 )eGon2_malloc(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_CALLOC:
        {
            swi_reg->reg[0] = (__u32 )eGon2_calloc(swi_reg->reg[0], swi_reg->reg[1]);
        }
        break;

        case EGON2_SWI_REALLOC:
        {
            swi_reg->reg[0] = (__u32 )eGon2_realloc((void *)swi_reg->reg[0], swi_reg->reg[1]);
        }
        break;

        case EGON2_SWI_FREE:
        {
             eGon2_free((void *)swi_reg->reg[0]);
        }
        break;

/* 内置TWI操作 */
        case EGON2_SWI_TWI_READ:
        {
            swi_reg->reg[0] = eGon2_twi_read((void *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_TWI_WRITE:
        {
            swi_reg->reg[0] = eGon2_twi_write((void *)swi_reg->reg[0]);
        }
        break;

//        case EGON2_SWI_TWI_INIT:
//        {
//            swi_reg->reg[0] = eGon2_twi_init(swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[1]);
//        }
//        break;
//
//        case EGON2_SWI_TWI_EXIT:
//        {
//            swi_reg->reg[0] = eGon2_twi_exit();
//        }
//        break;

//        case EGON2_SWI_TWI_SETFREQ:
//        {
//            swi_reg->reg[0] = eGon2_twi_setfreq(swi_reg->reg[0]);
//        }
//        break;
/* 内置POWER操作 */
        case EGON2_SWI_POWER_GET_SOURCE:
        {
            swi_reg->reg[0] = eGon2_power_get_dcin_battery_exist((__u32 *)swi_reg->reg[0], (__u32 *)swi_reg->reg[1]);
        }
        break;

        case EGON2_SWI_POWER_GET_BAT_VOL:
        {
            swi_reg->reg[0] = eGon2_power_get_battery_vol((__u32 *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_POWER_SET_OFF:
        {
            eGon2_power_off();
        }
        break;

        case EGON2_SWI_POWER_GET_KEY:
        {
            swi_reg->reg[0] = eGon2_power_get_key();
        }
        break;

        case EGON2_SWI_POWER_CHECK_STARTUP:
        {
            swi_reg->reg[0] = eGon2_power_check_startup();
        }
        break;

        case EGON2_SWI_POWER_SET_SW1:
        {
            swi_reg->reg[0] = eGon2_power_set_sw1(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_POWER_GET_LEVEL_STATUS:
        {
        	swi_reg->reg[0] = eGon2_get_power_vol_level();
        }
        break;

        case EGON2_SWI_POWER_SET_OFF_VOL:
        {
        	swi_reg->reg[0] = eGon2_set_power_off_vol();
        }
        break;

        case EGON2_SWI_POWER_BATTERY_CAL:
        {
        	swi_reg->reg[0] = eGon2_power_axp_rest_cal();
        }
        break;

		case EGON2_SWI_POWER_CUR_LIMIT:
		{
			swi_reg->reg[0] = eGon2_power_vbus_cur_limit(swi_reg->reg[0]);
		}
		break;

		case EGON2_SWI_POWER_VOL_LIMIT:
		{
			swi_reg->reg[0] = eGon2_power_vbus_vol_limit(swi_reg->reg[0]);
		}
		break;

		case EGON2_SWI_POWER_TYPE:
		{
			swi_reg->reg[0] = eGon2_power_type();
		}
		break;
		/* cache 操作 */
//		case SWI_NR_CACHE_FLUSH_DCACHE:
//		{
//			eLIBs_FlushDCacheRegion((void *)(swi_reg->reg[0]),swi_reg->reg[1]);
//		}
//		break;
//
//
//		case SWI_NR_CACHE_CLEAN_DCACHE:
//		{
//			eLIBs_CleanDCacheRegion((void *)(swi_reg->reg[0]),swi_reg->reg[1]);
//		}
//		break;

/* 内置块设备操作 */
		case EGON2_SWI_BLOCK_SIZE:
		{
			swi_reg->reg[0] = eGon2_block_size();
		}
		break;

        case EGON2_SWI_BLOCK_DEV_INIT:
        {
            swi_reg->reg[0] = eGon2_block_device_init();
        }
        break;

        case EGON2_SWI_BLOCK_DEV_READ:
        {
            swi_reg->reg[0] = eGon2_block_device_read(swi_reg->reg[0], swi_reg->reg[1], (void *)swi_reg->reg[2]);
        }
        break;

        case EGON2_SWI_BLOCK_DEV_WRITE:
        {
            swi_reg->reg[0] = eGon2_block_device_write(swi_reg->reg[0], swi_reg->reg[1], (void *)swi_reg->reg[2]);
        }
        break;

        case EGON2_SWI_BLOCK_DEV_EXIT:
        {
            swi_reg->reg[0] = eGon2_block_device_exit();
        }
        break;

        case EGON2_SWI_BLOCK_DDEV_READ:
        {
			swi_reg->reg[0] = eGon2_block_phyread(swi_reg->reg[0], swi_reg->reg[1], (void *)swi_reg->reg[2]);
        }
        break;

        case EGON2_SWI_BLOCK_DDEV_WRITE:
        {
			swi_reg->reg[0] = eGon2_block_phywrite(swi_reg->reg[0], swi_reg->reg[1], (void *)swi_reg->reg[2]);
        }
        break;
/* 应用程序运行 */
        case EGON2_SWI_RUN_APP :
        {
            swi_reg->reg[0] = eGon2_run_app(swi_reg->reg[0], (char **)swi_reg->reg[1]);
        }
        break;
/* 驱动管理操作 */
		case EGON2_SWI_DRV_REG :
		{
			swi_reg->reg[0] = eGon2_driver_install((void *)swi_reg->reg[0]);
		}
		break;

		case EGON2_SWI_DRV_UNREG :
		{
			swi_reg->reg[0] = eGon2_driver_unreg(swi_reg->reg[0]);
		}
		break;

		case EGON2_SWI_DRV_OPEN :
		{
			swi_reg->reg[0] = eGon2_driver_open(swi_reg->reg[0], (void *)swi_reg->reg[1]);
		}
		break;

		case EGON2_SWI_DRV_CLOSE :
		{
			swi_reg->reg[0] = eGon2_driver_close(swi_reg->reg[0]);
		}
		break;

		case EGON2_SWI_DRV_READ :
		{
			swi_reg->reg[0] = eGon2_driver_read((void *)swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[2], swi_reg->reg[3]);
		}
		break;

		case EGON2_SWI_DRV_WRITE :
		{
			(swi_reg->reg[0]) = eGon2_driver_write((void *)swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[2], swi_reg->reg[3]);
		}
		break;

		case EGON2_SWI_DRV_IOCTL :
		{
			(swi_reg->reg[0])  = eGon2_driver_ioctl(swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[2], (void *)swi_reg->reg[3]);
		}
		break;
/* 文件操作 */
        case EGON2_SWI_FS_OPEN:
        {
            (swi_reg->reg[0]) = (__u32)FS_fopen((const char *)swi_reg->reg[0], (char *)swi_reg->reg[1]);
        }
        break;

        case EGON2_SWI_FS_READ:
        {
            (swi_reg->reg[0]) = FS_fread((void *)swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[2], (void *)swi_reg->reg[3]);
        }
        break;

        case EGON2_SWI_FS_WRITE:
        {
            (swi_reg->reg[0]) = FS_fwrite((void *)swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[2], (void *)swi_reg->reg[3]);
        }
        break;

        case EGON2_SWI_FS_TELL:
        {
            (swi_reg->reg[0]) = FS_ftell((void *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_FS_LEN:
        {
            (swi_reg->reg[0]) = FS_filelen((void *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_FS_SEEK:
        {
            (swi_reg->reg[0]) = FS_fseek((void *)swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[2]);
        }
        break;

        case EGON2_SWI_FS_EOF:
        {
            (swi_reg->reg[0]) = FS_feof((void *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_FS_CLOSE:
        {
            (swi_reg->reg[0]) = FS_fclose((void *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_FS_RM:
        {
            (swi_reg->reg[0]) = FS_rmfile((const char *)swi_reg->reg[0]);
        }
        break;
/* 目录 操作 */
        case EGON2_SWI_DIR_OPEN:
        {
            (swi_reg->reg[0]) = (__u32)FS_opendir((const char *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DIR_CLOSE:
        {
            (swi_reg->reg[0]) = FS_closedir((void *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DIR_MAKE:
        {
            (swi_reg->reg[0]) = FS_mkdir((char *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DIR_ERASE:
        {
            (swi_reg->reg[0]) = FS_rmdir((char *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DIR_READ:
        {
            (swi_reg->reg[0]) = FS_readdir((void *)swi_reg->reg[0], (ENTRY *)swi_reg->reg[1]);
        }
        break;
/* 分区mount操作 */
        case EGON2_SWI_PART_MOUNT:
        {
            (swi_reg->reg[0]) = FSMount((char )swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_PART_UNMOUNT:
        {
            (swi_reg->reg[0]) = FSUnmount((char )swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_PART_INFO:
        {
            (swi_reg->reg[0]) = FS_getpart_info((const char *)swi_reg->reg[0], (const char *)swi_reg->reg[1], (part_info_t *)swi_reg->reg[2]);
        }
        break;

        case EGON2_SWI_PART_COUNT:
        {
        	(swi_reg->reg[0]) = FS_getpart_count(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_PART_START:
        {
        	(swi_reg->reg[0]) = FS_getpart_start(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_PART_CAPACITY:
        {
        	(swi_reg->reg[0]) = FS_getpart_capacity(swi_reg->reg[0]);
        }
        break;
/* DMA 操作*/
        case EGON2_SWI_DMA_REQ:
        {
            (swi_reg->reg[0]) = eGon2_RequestDMA(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DMA_REL:
        {
            (swi_reg->reg[0]) = eGon2_ReleaseDMA(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DMA_START:
        {
            (swi_reg->reg[0]) = eGon2_StartDMA(swi_reg->reg[0], swi_reg->reg[1], swi_reg->reg[2], swi_reg->reg[3]);
        }
        break;

        case EGON2_SWI_DMA_STOP:
        {
            (swi_reg->reg[0]) = eGon2_StopDMA(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DMA_SETTING:
        {
            (swi_reg->reg[0]) = eGon2_SettingDMA(swi_reg->reg[0], (void *)swi_reg->reg[1]);
        }
        break;

        case EGON2_SWI_DMA_RESTART:
        {
            (swi_reg->reg[0]) = eGon2_RestartDMA(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DMA_QUERYSTATE:
        {
            (swi_reg->reg[0]) = eGon2_QueryDMAStatus(swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_DMA_QUERYCHAN:
        {
            (swi_reg->reg[0]) = eGon2_QueryDMAChannalNo(swi_reg->reg[0]);
        }
        break;

//		case SWI_NR_KERNEL_ENABLE_IRQ :
//		{
//			u32 temp;
//			__asm{
//				MRS		temp , CPSR
//				BIC		temp , temp , #0x80
//				MSR		CPSR_c , temp
//			};
//		}
//		break;
//
//		case SWI_NR_KERNEL_DISABLE_IRQ :
//		{
//			u32 temp;
//			__asm{
//				MRS		temp , CPSR
//				ORR		temp , temp , #0x80
//				MSR		CPSR_c , temp
//			};
//		}
//		break;

		case EGON2_SWI_UART_DEBUG:
		//case EGON2_SWI_DISPLAY_DEBUG:
			eGon2_printf((void *)(swi_reg->reg[0]));
		break;

		case EGON2_SWI_UART_NTDEBUG:
			eFG_printf((void *)(swi_reg->reg[0]));
		break;

		case EGON2_SWI_UART_GETC:
		{
		    swi_reg->reg[0] = eGon2_getc(0);
		}
		break;

		case EGON2_SWI_UART_GETC_DELAY:
		{
			swi_reg->reg[0] = eGon2_getc(swi_reg->reg[0]);
		}
		break;

		case EGON2_SWI_UART_GETS:
		{
            swi_reg->reg[0] = eGon2_getall((char *)(swi_reg->reg[0]));
		}
        break;

        case EGON2_SWI_UART_PUTC:
        {
            eGon2_putc(swi_reg->reg[0]);
        }
        break;
// 时钟操作
        case EGON2_SWI_CLOCK:
        {
        	swi_reg->reg[0] = eGon2_clock_get(swi_reg->reg[0]);
        }
        break;
//key操作
        case EGON2_SWI_KEY_GET_STATUS:
        {
            swi_reg->reg[0] = eGon2_key_get_status();
        }
        break;

        case EGON2_SWI_KEY_GET_VALUE:
        {
        	swi_reg->reg[0] = eGon2_key_get_value();
        }
        break;
/* 脚本 操作*/
        case EGON2_SWI_SCRIPT_FETCH:
        {
        	swi_reg->reg[0] = eGon2_script_parser_fetch((char *)swi_reg->reg[0], (char *)swi_reg->reg[1], (int *)swi_reg->reg[2], swi_reg->reg[3]);
        }
        break;

        case EGON2_SWI_SCRIPT_PATCH:
        {
        	swi_reg->reg[0] = eGon2_script_parser_patch((char *)swi_reg->reg[0], (char *)swi_reg->reg[1], swi_reg->reg[2]);
        }
        break;

        case EGON2_SWI_SCRIPT_SUBKEY_COUNT:
        {
        	swi_reg->reg[0] = eGon2_script_parser_subkey_count((char *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_SCRIPT_MAINKEY_COUNT:
        {
        	swi_reg->reg[0] = eGon2_script_parser_mainkey_count();
        }
        break;

        case EGON2_SWI_SCRIPT_MAINKEY_GPIO_COUNT:
        {
        	swi_reg->reg[0] = eGon2_script_parser_mainkey_get_gpio_count((char *)swi_reg->reg[0]);
        }
        break;

        case EGON2_SWI_SCRIPT_MAINKEY_GPIO_DATA:
		{
			swi_reg->reg[0] = eGon2_script_parser_mainkey_get_gpio_cfg((char *)swi_reg->reg[0], (void *)swi_reg->reg[1], swi_reg->reg[2]);
		}
		break;
/* GPIO 操作 */
        case EGON2_SWI_GPIO_REQUEST:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Request((user_gpio_set_t *)swi_reg->reg[0], swi_reg->reg[1]);
        }
        break;
        case EGON2_SWI_GPIO_RELEASE:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Release((__hdle)swi_reg->reg[0], swi_reg->reg[1]);
        }
        break;
        case EGON2_SWI_GPIO_GET_ALL_PIN_STATUS:
    	{
        	swi_reg->reg[0] = eGon2_GPIO_Get_All_PIN_Status((__hdle)swi_reg->reg[0], (user_gpio_set_t *)swi_reg->reg[1], swi_reg->reg[2], swi_reg->reg[3]);
        }
        break;
        case EGON2_SWI_GPIO_GET_ONE_PIN_STATUS:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Get_One_PIN_Status((__hdle)swi_reg->reg[0], (user_gpio_set_t *)swi_reg->reg[1], (const char *)swi_reg->reg[2], swi_reg->reg[3]);
        }
        break;
        case EGON2_SWI_GPIO_SET_ONE_PIN_STATUS:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Set_One_PIN_Status((__hdle)swi_reg->reg[0], (user_gpio_set_t *)swi_reg->reg[1], (const char *)swi_reg->reg[2], swi_reg->reg[3]);
        }
        break;
        case EGON2_SWI_GPIO_SET_ONE_PIN_IO:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Set_One_PIN_IO_Status((__hdle)swi_reg->reg[0], swi_reg->reg[1], (const char *)swi_reg->reg[2]);
        }
        break;
        case EGON2_SWI_GPIO_SET_ONE_PIN_PULL:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Set_One_PIN_Pull((__hdle)swi_reg->reg[0], swi_reg->reg[1], (const char *)swi_reg->reg[2]);
        }
        break;
        case EGON2_SWI_GPIO_SET_ONE_PIN_DLEVEL:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Set_One_PIN_driver_level((__hdle)swi_reg->reg[0], swi_reg->reg[1], (const char *)swi_reg->reg[2]);
        }
        break;
        case EGON2_SWI_GPIO_SET_ONE_PIN_DATA:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Write_One_PIN_Value((__hdle)swi_reg->reg[0], swi_reg->reg[1], (const char *)swi_reg->reg[2]);
        }
        break;
        case EGON2_SWI_GPIO_GET_ONE_PIN_DATA:
        {
        	swi_reg->reg[0] = eGon2_GPIO_Read_One_PIN_Value((__hdle)swi_reg->reg[0], (const char *)swi_reg->reg[2]);
        }
        break;
		default:
		{
			swi_reg->reg[0] = (__u32)(-1);
			eGon2_printf("ERROR : swi entry mismatch");
		    eGon2_printf("The input swi number is %d, %x\n", swi_number, swi_number);
		}
		break;

	}

	return;
}

