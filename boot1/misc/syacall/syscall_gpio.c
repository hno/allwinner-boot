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
#include "syscall_i.h"

//__swi(EGON2_SWI_GPIO_REQUEST)        unsigned int wBoot_GPIO_Request				 (user_gpio_set_t *gpio_list, 	    unsigned int group_count_max			 );
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
unsigned int wBoot_GPIO_Request(user_gpio_set_t *gpio_list, 	    unsigned int group_count_max)
{
	unsigned int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_REQUEST));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_RELEASE)                 int wBoot_GPIO_Release				 (unsigned int p_handler, 	            		 int if_release_to_default_status);
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
int wBoot_GPIO_Release(unsigned int p_handler, int if_release_to_default_status)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_RELEASE));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_GET_ALL_PIN_STATUS)      int wBoot_GPIO_Get_All_PIN_Status	     (unsigned int p_handler, user_gpio_set_t *gpio_status, unsigned int gpio_count_max,  unsigned int if_get_from_hardware);
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
int wBoot_GPIO_Get_All_PIN_Status(unsigned int p_handler, user_gpio_set_t *gpio_status, unsigned int gpio_count_max,  unsigned int if_get_from_hardware)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_GET_ALL_PIN_STATUS));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_GET_ONE_PIN_STATUS)      int wBoot_GPIO_Get_One_PIN_Status	     (unsigned int p_handler, user_gpio_set_t *gpio_status, 		 const char *gpio_name, unsigned int if_get_from_hardware);
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
int wBoot_GPIO_Get_One_PIN_Status(unsigned int p_handler, user_gpio_set_t *gpio_status, const char *gpio_name, unsigned int if_get_from_hardware)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_GET_ONE_PIN_STATUS));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_STATUS)      int wBoot_GPIO_Set_One_PIN_Status	     (unsigned int p_handler, user_gpio_set_t *gpio_status, 	     const char *gpio_name, unsigned int if_set_to_current_input_status);
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
int wBoot_GPIO_Set_One_PIN_Status(unsigned int p_handler, user_gpio_set_t *gpio_status, const char *gpio_name, unsigned int if_set_to_current_input_status)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_SET_ONE_PIN_STATUS));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_IO)          int wBoot_GPIO_Set_One_PIN_IO_Status   (unsigned int p_handler, unsigned int if_set_to_output_status, const char *gpio_name);
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
int wBoot_GPIO_Set_One_PIN_IO_Status(unsigned int p_handler, unsigned int if_set_to_output_status, const char *gpio_name)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_SET_ONE_PIN_IO));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_PULL)        int wBoot_GPIO_Set_One_PIN_Pull	     (unsigned int p_handler, unsigned int set_pull_status,   	     const char *gpio_name);
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
int wBoot_GPIO_Set_One_PIN_Pull(unsigned int p_handler, unsigned int set_pull_status, const char *gpio_name)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_SET_ONE_PIN_PULL));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_DLEVEL)      int wBoot_GPIO_Set_One_PIN_driver_level(unsigned int p_handler, unsigned int set_driver_level,  		 const char *gpio_name);
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
int wBoot_GPIO_Set_One_PIN_driver_level(unsigned int p_handler, unsigned int set_driver_level, const char *gpio_name)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_SET_ONE_PIN_DLEVEL));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_SET_ONE_PIN_DATA)        int wBoot_GPIO_Write_One_PIN_Value	 (unsigned int p_handler, unsigned int value_to_gpio,     		 const char *gpio_name);
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
int wBoot_GPIO_Write_One_PIN_Value(unsigned int p_handler, unsigned int value_to_gpio, const char *gpio_name)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_SET_ONE_PIN_DATA));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}
//__swi(EGON2_SWI_GPIO_GET_ONE_PIN_DATA)        int wBoot_GPIO_Read_One_PIN_Value	     (unsigned int p_handler, 								   		 const char *gpio_name);
///*
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
int wBoot_GPIO_Read_One_PIN_Value(unsigned int p_handler, const char *gpio_name)
{
	int ret;

	asm("stmfd sp!, {lr}");
	asm("swi %0"::"Ir"(EGON2_SWI_GPIO_GET_ONE_PIN_DATA));
	asm("mov %0, r0":"=r"(ret));
	asm("ldmfd sp!, {lr}");

	return ret;
}


