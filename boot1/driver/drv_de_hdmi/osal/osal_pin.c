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
#include "egon2.h"
#include "types.h"

__hdle OSAL_GPIO_Request(user_gpio_set_t *gpio_list, __u32 group_count_max)
{
    //__inf("OSAL_GPIO_Request, port:%d, port_num:%d, data:%d\n", gpio_list->port, gpio_list->port_num, gpio_list->data);

    return wBoot_GPIO_Request(gpio_list, group_count_max);
}

__hdle OSAL_GPIO_Request_Ex(char *main_name, const char *sub_name)
{
	user_gpio_set_t     *gpio_init;
	__u32			    gpio_count;
	__u32               gpio_hd;

	gpio_count = wBoot_script_parser_mainkey_get_gpio_count(main_name);
	if(gpio_count)
	{
		gpio_init = wBoot_malloc(gpio_count * sizeof(user_gpio_set_t));
		if(gpio_init)
		{
			wBoot_script_parser_mainkey_get_gpio_cfg(main_name, (void *)gpio_init, gpio_count);
			gpio_hd = wBoot_GPIO_Request(gpio_init, gpio_count);

			wBoot_free(gpio_init);

			return gpio_hd;
		}
	}
	
    return 0;
}

//if_release_to_default_status:
    //如果是0或者1，表示释放后的GPIO处于输入状态，输入状状态不会导致外部电平的错误。
    //如果是2，表示释放后的GPIO状态不变，即释放的时候不管理当前GPIO的硬件寄存器。
__s32 OSAL_GPIO_Release(__hdle p_handler, __s32 if_release_to_default_status)
{
    //__inf("OSAL_GPIO_Release\n");
    wBoot_GPIO_Release(p_handler, if_release_to_default_status);

    return 0;
}

__s32 OSAL_GPIO_DevGetAllPins_Status(unsigned p_handler, user_gpio_set_t *gpio_status, unsigned gpio_count_max, unsigned if_get_from_hardware)
{
    return wBoot_GPIO_Get_All_PIN_Status(p_handler, gpio_status, gpio_count_max, if_get_from_hardware);
}

__s32 OSAL_GPIO_DevGetONEPins_Status(unsigned p_handler, user_gpio_set_t *gpio_status,const char *gpio_name,unsigned if_get_from_hardware)
{
    return wBoot_GPIO_Get_One_PIN_Status(p_handler, gpio_status,gpio_name,if_get_from_hardware);
}

__s32 OSAL_GPIO_DevSetONEPin_Status(u32 p_handler, user_gpio_set_t *gpio_status, const char *gpio_name, __u32 if_set_to_current_input_status)
{
    return wBoot_GPIO_Set_One_PIN_Status(p_handler, gpio_status, gpio_name, if_set_to_current_input_status);
}

__s32 OSAL_GPIO_DevSetONEPIN_IO_STATUS(u32 p_handler, __u32 if_set_to_output_status, const char *gpio_name)
{
    return wBoot_GPIO_Set_One_PIN_IO_Status(p_handler, if_set_to_output_status, gpio_name);
}

__s32 OSAL_GPIO_DevSetONEPIN_PULL_STATUS(u32 p_handler, __u32 set_pull_status, const char *gpio_name)
{
    return wBoot_GPIO_Set_One_PIN_Pull(p_handler, set_pull_status, gpio_name);
}

__s32 OSAL_GPIO_DevREAD_ONEPIN_DATA(u32 p_handler, const char *gpio_name)
{
    return wBoot_GPIO_Read_One_PIN_Value(p_handler, gpio_name);
}

__s32 OSAL_GPIO_DevWRITE_ONEPIN_DATA(u32 p_handler, __u32 value_to_gpio, const char *gpio_name)
{
    return wBoot_GPIO_Write_One_PIN_Value(p_handler, value_to_gpio, gpio_name);
}

int OSAL_Script_FetchParser_Data(char *main_name, char *sub_name, int value[], int count)
{
	return wBoot_script_parser_fetch(main_name, sub_name, value, count);
}
