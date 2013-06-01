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
#include "string.h"

typedef struct
{
	int mul_sel;
	int pull;
	int drv_level;
	int data;
}
gpio_status_set_t;

typedef struct
{
    char    gpio_name[32];
    int port;
	int port_num;
    gpio_status_set_t user_gpio_status;
    gpio_status_set_t hardware_gpio_status;
}
system_gpio_set_t;
/*
****************************************************************************************************
*
*             CSP_PIN_init
*
*  Description:
*       init
*
*  Parameters:
*		p_sys_srv	:	本csp依赖的系统服务
*		p_cfg		:	pin的配置
*  Return value:
*		EBSP_TRUE/EBSP_FALSE
****************************************************************************************************
*/
__s32 eGon2_GPIO_init(void)          //GPIO初始化函数接口
{
	return 0;
}
/*
****************************************************************************************************
*
*             CSP_PIN_exit
*
*  Description:
*       exit
*
*  Parameters:
*
*  Return value:
*		EBSP_TRUE/EBSP_FALSE
****************************************************************************************************
*/
__s32 eGon2_GPIO_exit(void)		    //GPIO推出函数接口
{
	return 0;
}
/*
************************************************************************************************************
*
*                                             CSP_PIN_DEV_req
*
*    函数名称：
*
*    参数列表：gpio_list      存放所有用到的GPIO数据的数组，GPIO将直接使用这个数组
*
*			   group_count_max  数组的成员个数，GPIO设定的时候，将操作的GPIO最大不超过这个值
*
*    返回值  ：
*
*    说明    ：暂时没有做冲突检查
*
*
************************************************************************************************************
*/
//用户传进的数据必须保存
//保存的方法，可以是有用户申请的时候，去malloc，存在的问题是可能 malloc动作会比较多，可能出现申请很小数据的情况
//可以在初始化的时候把所有GPIO全部申请好,大小的依据是IC中所有可以分配的GPIO的个数，但是比较难以控制。还存在保存对应GPIO对应数据的问题
//
//决定：使用malloc的方法，毕竟更加灵活。
__hdle eGon2_GPIO_Request(user_gpio_set_t *gpio_list, __u32 group_count_max)  //设备申请GPIO函数接口
{
    char               *user_gpio_buf;                                        //按照char类型申请
	system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;					  //user_gpio_set将是申请内存的句柄
	user_gpio_set_t  *tmp_user_gpio_data;
	__u32				real_gpio_count = 0;                                  //保存真正有效的GPIO的个数
	__u32               tmp_group_func_data;
	__u32               tmp_group_pull_data;
	__u32               tmp_group_dlevel_data;
	__u32               tmp_group_data_data;
	__u32               pull_change = 0;
	__u32               dlevel_change = 0, data_change = 0;
	volatile __u32  *tmp_group_func_addr = NULL, *tmp_group_pull_addr = NULL;
	volatile __u32  *tmp_group_dlevel_addr = NULL, *tmp_group_data_addr = NULL;
	__u32  port, port_num, port_num_func, port_num_pull;
	__u32  pre_port = 0x7fffffff, pre_port_num_func = 0x7fffffff;
	__u32  pre_port_num_pull = 0x7fffffff;
	__s32               i, tmp_val;

	//检查用户传进的数据的有效性
	if((!gpio_list) || (!group_count_max))
	{
		return (__hdle)0;
	}
	//查看用户数据真正有效的个数
	for(i = 0; i < group_count_max; i++)
	{
		tmp_user_gpio_data = gpio_list + i;                 //gpio_set依次指向每个GPIO数组成员
	    if(!tmp_user_gpio_data->port)
	    {
	    	continue;
	    }
	    real_gpio_count ++;
	}
    //根据有效的GPIO个数申请空间
	user_gpio_buf = (char *)eGon2_malloc(16 + sizeof(system_gpio_set_t) * real_gpio_count);   //申请内存，多申请16个字节，用于存放GPIO个数等信息
	if(!user_gpio_buf)
	{
		return (__hdle)0;
	}
	memset(user_gpio_buf, 0, 16 + sizeof(system_gpio_set_t) * real_gpio_count);         //首先全部清零
	*(int *)user_gpio_buf = real_gpio_count;                                           //保存有效的GPIO个数
	user_gpio_set = (system_gpio_set_t *)(user_gpio_buf + 16);                         //指向第一个结构体
	//保存用户数据
	for(i = 0; i < group_count_max; i++)
	{
		tmp_sys_gpio_data  = user_gpio_set + i;             //tmp_sys_gpio_data指向申请的GPIO空间
		tmp_user_gpio_data = gpio_list + i;                 //gpio_set依次指向用户的每个GPIO数组成员
	    port     = tmp_user_gpio_data->port;                //读出端口数值
	    port_num = tmp_user_gpio_data->port_num;            //读出端口中的某一个GPIO
	    if(!port)
	    {
	    	continue;
	    }
	    //开始保存用户数据
	    strcpy(tmp_sys_gpio_data->gpio_name, tmp_user_gpio_data->gpio_name);
	    tmp_sys_gpio_data->port                       = port;
	    tmp_sys_gpio_data->port_num                   = port_num;
	    tmp_sys_gpio_data->user_gpio_status.mul_sel   = tmp_user_gpio_data->mul_sel;
	    tmp_sys_gpio_data->user_gpio_status.pull      = tmp_user_gpio_data->pull;
	    tmp_sys_gpio_data->user_gpio_status.drv_level = tmp_user_gpio_data->drv_level;
	    tmp_sys_gpio_data->user_gpio_status.data      = tmp_user_gpio_data->data;


        port_num_func = (port_num >> 3);
        port_num_pull = (port_num >> 4);

        if(tmp_group_func_addr)                             //必须检查寄存器变量是否被赋值
        {
            if((port_num_pull != pre_port_num_pull) || (port != pre_port))    //如果发现当前引脚的端口不一致，或者所在的pull寄存器不一致
            {
                *tmp_group_func_addr   = tmp_group_func_data;    //回写功能寄存器
                if(pull_change)
                {
                    pull_change = 0;
                    *tmp_group_pull_addr   = tmp_group_pull_data;    //回写pull寄存器
                }
                if(dlevel_change)
                {
                    dlevel_change = 0;
                    *tmp_group_dlevel_addr = tmp_group_dlevel_data;  //回写driver level寄存器
                }
                if(data_change)
                {
                    data_change = 0;
                    *tmp_group_data_addr   = tmp_group_data_data;    //回写
                }

                tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //更新功能寄存器地址
                tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);   //更新pull寄存器
                tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull); //更新level寄存器
                tmp_group_data_addr    = PIO_REG_DATA(port);                  //更新data寄存器

                tmp_group_func_data    = *tmp_group_func_addr;
                tmp_group_pull_data    = *tmp_group_pull_addr;
                tmp_group_dlevel_data  = *tmp_group_dlevel_addr;
                tmp_group_data_data    = *tmp_group_data_addr;
            }
            else if(pre_port_num_func != port_num_func)                       //如果发现当前引脚的功能寄存器不一致
            {
                *tmp_group_func_addr   = tmp_group_func_data;    //则只回写功能寄存器
                tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //更新功能寄存器地址

                tmp_group_func_data    = *tmp_group_func_addr;
            }
        }
        else
        {
            tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //更新功能寄存器地址
            tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);   //更新pull寄存器
            tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull); //更新level寄存器
            tmp_group_data_addr    = PIO_REG_DATA(port);                  //更新data寄存器

            tmp_group_func_data    = *tmp_group_func_addr;
            tmp_group_pull_data    = *tmp_group_pull_addr;
            tmp_group_dlevel_data  = *tmp_group_dlevel_addr;
            tmp_group_data_data    = *tmp_group_data_addr;
        }
		//保存当前硬件寄存器数据
        pre_port_num_pull = port_num_pull;                      //设置当前GPIO成为前一个GPIO
        pre_port_num_func = port_num_func;
        pre_port          = port;

        //更新功能寄存器
        tmp_val = (port_num - (port_num_func<<3)) << 2;
        tmp_sys_gpio_data->hardware_gpio_status.mul_sel = (tmp_group_func_data >> tmp_val) & 0x07;
        tmp_group_func_data &= ~(                              0x07  << tmp_val);
        tmp_group_func_data |=  (tmp_user_gpio_data->mul_sel & 0x07) << tmp_val;
        //根据pull的值决定是否更新pull寄存器
        tmp_val              =  (port_num - (port_num_pull<<4)) << 1;
        tmp_sys_gpio_data->hardware_gpio_status.pull = (tmp_group_pull_data >> tmp_val) & 0x03;
        if(tmp_user_gpio_data->pull >= 0)
        {
            tmp_group_pull_data &= ~(                           0x03  << tmp_val);
            tmp_group_pull_data |=  (tmp_user_gpio_data->pull & 0x03) << tmp_val;
            pull_change = 1;
        }
        //根据driver level的值决定是否更新driver level寄存器
        tmp_sys_gpio_data->hardware_gpio_status.drv_level = (tmp_group_dlevel_data >> tmp_val) & 0x03;
        if(tmp_user_gpio_data->drv_level >= 0)
        {
            tmp_group_dlevel_data &= ~(                                0x03  << tmp_val);
            tmp_group_dlevel_data |=  (tmp_user_gpio_data->drv_level & 0x03) << tmp_val;
            dlevel_change = 1;
        }
        //根据用户输入，以及功能分配决定是否更新data寄存器
        if(tmp_user_gpio_data->mul_sel == 1)
        {
            if(tmp_val >= 0)
            {
            	tmp_val = tmp_user_gpio_data->data;
            	tmp_val &= 1;
                tmp_group_data_data &= ~(1 << port_num);
                tmp_group_data_data |= tmp_val << port_num;
                data_change = 1;
            }
        }
    }
    //for循环结束，如果存在还没有回写的寄存器，这里写回到硬件当中
    if(tmp_group_func_addr)                         //只要更新过寄存器地址，就可以对硬件赋值
    {                                               //那么把所有的值全部回写到硬件寄存器
        *tmp_group_func_addr   = tmp_group_func_data;       //回写功能寄存器
        if(pull_change)
        {
            *tmp_group_pull_addr   = tmp_group_pull_data;    //回写pull寄存器
        }
        if(dlevel_change)
        {
            *tmp_group_dlevel_addr = tmp_group_dlevel_data;  //回写driver level寄存器
        }
        if(data_change)
        {
            *tmp_group_data_addr   = tmp_group_data_data;    //回写data寄存器
        }
    }

    return (__hdle)user_gpio_buf;
}

//把普通IO的申请和DEVICE的IO申请做到一起，减少外部接口
/*
****************************************************************************************************
*
*             CSP_PIN_DEV_release
*
*  Description:
*       释放某逻辑设备的pin
*
*  Parameters:
* 		p_handler	:	handler
*
*  Return value:
*		EBSP_TRUE/EBSP_FALSE
****************************************************************************************************
*/
__s32 eGon2_GPIO_Release(__hdle p_handler, __s32 if_release_to_default_status)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
    __u32               tmp_group_func_data;
    volatile __u32  *tmp_group_func_addr = NULL;
    __u32               port, port_num, port_num_func;
    __u32               pre_port = 0x7fffffff, pre_port_num_func = 0x7fffffff;
    __u32               i;

    //检查传进的句柄的有效性
    if(!p_handler)
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    if(!group_count_max)
    {
        return EGON2_FAIL;
    }
    if(if_release_to_default_status == 2)
    {
        eGon2_free((char *)p_handler);

        return EGON2_OK;
    }
    user_gpio_set = (system_gpio_set_t *)(tmp_buf + 16);
	//读取用户数据
	for(i = 0; i < group_count_max; i++)
	{
		tmp_sys_gpio_data  = user_gpio_set + i;             //tmp_sys_gpio_data指向申请的GPIO空间
	    port     = tmp_sys_gpio_data->port;                 //读出端口数值
	    port_num = tmp_sys_gpio_data->port_num;             //读出端口中的某一个GPIO

        port_num_func = (port_num >> 3);

        if(tmp_group_func_addr)                             //必须检查寄存器变量是否被赋值
        {
            if((pre_port_num_func != port_num_func) || (port != pre_port)) //如果发现当前引脚的端口不一致，或者所在的pull寄存器不一致
            {
                *tmp_group_func_addr = tmp_group_func_data;                //回写功能寄存器
                tmp_group_func_addr  = PIO_REG_CFG(port, port_num_func);   //更新功能寄存器地址

                tmp_group_func_data = *tmp_group_func_addr;
            }
        }
        else
        {
            tmp_group_func_addr = PIO_REG_CFG(port, port_num_func);       //更新功能寄存器地址

            tmp_group_func_data = *tmp_group_func_addr;
        }

        pre_port_num_func = port_num_func;
        pre_port          = port;
        //更新功能寄存器
        tmp_group_func_data &= ~(0x07 << ((port_num - (port_num_func<<3)) << 2));
        //根据pull的值决定是否更新pull寄存器
    }
    if(tmp_group_func_addr)                              //只要更新过寄存器地址，就可以对硬件赋值
    {                                                    //那么把所有的值全部回写到硬件寄存器
        *tmp_group_func_addr   = tmp_group_func_data;    //回写功能寄存器
    }

    //free(p_handler);                                //释放内存
	eGon2_free((char *)p_handler);

    return EGON2_OK;
}
/*
**********************************************************************************************************************
*                                               CSP_PIN_Get_All_Gpio_Status
*
* Description:
*				获取用户申请过的所有GPIO的状态
* Arguments  :
*		p_handler	:	handler
*		gpio_status	:	保存用户数据的数组
*		gpio_count_max	:	数组最大个数，避免数组越界
*       if_get_user_set_flag   :   读取标志，表示读取用户设定数据或者是实际数据
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  eGon2_GPIO_Get_All_PIN_Status(__hdle p_handler, user_gpio_set_t *gpio_status, __u32 gpio_count_max, __u32 if_get_from_hardware)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
    user_gpio_set_t  *script_gpio;
    __u32               port_num_func, port_num_pull;
    volatile __u32     *tmp_group_func_addr = NULL, *tmp_group_pull_addr;
    volatile __u32     *tmp_group_data_addr, *tmp_group_dlevel_addr;
    __u32               port, port_num;
    __u32               pre_port = 0x7fffffff, pre_port_num_func = 0x7fffffff, pre_port_num_pull = 0x7fffffff;
    __u32               i;

    //检查传进的句柄的有效性
    if((!p_handler) || (!gpio_status))
    {
        return EGON2_FAIL;
    }
    if(gpio_count_max <= 0)
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    if(group_count_max <= 0)
    {
        return EGON2_FAIL;
    }
    user_gpio_set = (system_gpio_set_t *)(tmp_buf + 16);
    if(group_count_max > gpio_count_max)
    {
        group_count_max = gpio_count_max;
    }
	//读取用户数据
	//表示读取用户给定的数据
	if(!if_get_from_hardware)
	{
	    for(i = 0; i < group_count_max; i++)
	    {
    	    tmp_sys_gpio_data = user_gpio_set + i;             //tmp_sys_gpio_data指向申请的GPIO空间
    	    script_gpio       = gpio_status + i;               //script_gpio指向用户传进的空间

    	    script_gpio->port      = tmp_sys_gpio_data->port;                       //读出port数据
    	    script_gpio->port_num  = tmp_sys_gpio_data->port_num;                   //读出port_num数据
    	    script_gpio->pull      = tmp_sys_gpio_data->user_gpio_status.pull;      //读出pull数据
    	    script_gpio->mul_sel   = tmp_sys_gpio_data->user_gpio_status.mul_sel;   //读出功能数据
    	    script_gpio->drv_level = tmp_sys_gpio_data->user_gpio_status.drv_level; //读出驱动能力数据
    	    script_gpio->data      = tmp_sys_gpio_data->user_gpio_status.data;      //读出data数据
    	    strcpy(script_gpio->gpio_name, tmp_sys_gpio_data->gpio_name);
	    }
	}
	else
	{
		for(i = 0; i < group_count_max; i++)
    	{
    		tmp_sys_gpio_data = user_gpio_set + i;             //tmp_sys_gpio_data指向申请的GPIO空间
    		script_gpio       = gpio_status + i;               //script_gpio指向用户传进的空间

    	    port     = tmp_sys_gpio_data->port;                //读出端口数值
    	    port_num = tmp_sys_gpio_data->port_num;            //读出端口中的某一个GPIO

            script_gpio->port = port;                          //读出port数据
            script_gpio->port_num  = port_num;                 //读出port_num数据
            strcpy(script_gpio->gpio_name, tmp_sys_gpio_data->gpio_name);

            port_num_func = (port_num >> 3);
            port_num_pull = (port_num >> 4);

            if(tmp_group_func_addr)                             //必须检查寄存器变量是否被赋值
            {
                if((port_num_pull != pre_port_num_pull) || (port != pre_port))    //如果发现当前引脚的端口不一致，或者所在的pull寄存器不一致
                {
                    tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //更新功能寄存器地址
                    tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);   //更新pull寄存器
                    tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull); //更新level寄存器
                    tmp_group_data_addr    = PIO_REG_DATA(port);                  //更新data寄存器
                }
                else if(pre_port_num_func != port_num_func)                       //如果发现当前引脚的功能寄存器不一致
                {
                    tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //更新功能寄存器地址
                }
            }
            else
            {
                tmp_group_func_addr    = PIO_REG_CFG(port, port_num_func);   //更新功能寄存器地址
                tmp_group_pull_addr    = PIO_REG_PULL(port, port_num_pull);   //更新pull寄存器
                tmp_group_dlevel_addr  = PIO_REG_DLEVEL(port, port_num_pull); //更新level寄存器
                tmp_group_data_addr    = PIO_REG_DATA(port);                  //更新data寄存器
            }

            pre_port_num_pull = port_num_pull;
            pre_port_num_func = port_num_func;
            pre_port          = port;
            //给用户控件赋值
            script_gpio->pull      = (*tmp_group_pull_addr   >> ((port_num - (port_num_pull<<4))<<1)) & 0x03;    //读出pull数据
    	    script_gpio->drv_level = (*tmp_group_dlevel_addr >> ((port_num - (port_num_pull<<4))<<1)) & 0x03;    //读出功能数据
    	    script_gpio->mul_sel   = (*tmp_group_func_addr   >> ((port_num - (port_num_pull<<3))<<2)) & 0x07;    //读出驱动能力数据
    	    if(script_gpio->mul_sel <= 1)
    	    {
    	    	script_gpio->data  = (*tmp_group_data_addr   >>   port_num) & 0x01;                              //读出data数据
        	}
        	else
        	{
        		script_gpio->data = -1;
        	}
        }
	}

	return EGON2_OK;
}
/*
**********************************************************************************************************************
*                                               CSP_PIN_Get_All_Gpio_Status
*
* Description:
*				获取用户申请过的所有GPIO的状态
* Arguments  :
*		p_handler	:	handler
*		gpio_status	:	保存用户数据的数组
*		gpio_name	:	要操作的GPIO的名称
*       if_get_user_set_flag   :   读取标志，表示读取用户设定数据或者是实际数据
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  eGon2_GPIO_Get_One_PIN_Status(__hdle p_handler, user_gpio_set_t *gpio_status, const char *gpio_name, __u32 if_get_from_hardware)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
    __u32               port_num_func, port_num_pull;
    __u32               port, port_num;
    __u32               i, tmp_val1, tmp_val2;

    //检查传进的句柄的有效性
    if((!p_handler) || (!gpio_status) || (!gpio_name))
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    if(group_count_max <= 0)
    {
        return EGON2_FAIL;
    }
    user_gpio_set = (system_gpio_set_t *)(tmp_buf + 16);
	//读取用户数据
	//表示读取用户给定的数据

    for(i = 0; i < group_count_max; i++)
    {
        tmp_sys_gpio_data = user_gpio_set + i;             //tmp_sys_gpio_data指向申请的GPIO空间
        if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
        {
            continue;
        }
        strcpy(gpio_status->gpio_name, tmp_sys_gpio_data->gpio_name);
        port                   = tmp_sys_gpio_data->port;
        port_num               = tmp_sys_gpio_data->port_num;
        gpio_status->port      = port;                                              //读出port数据
	    gpio_status->port_num  = port_num;                                          //读出port_num数据

        if(!if_get_from_hardware)                                                    //当前要求读出用户设计的数据
        {
        	gpio_status->mul_sel   = tmp_sys_gpio_data->user_gpio_status.mul_sel;   //从用户传进数据中读出功能数据
            gpio_status->pull      = tmp_sys_gpio_data->user_gpio_status.pull;      //从用户传进数据中读出pull数据
            gpio_status->drv_level = tmp_sys_gpio_data->user_gpio_status.drv_level; //从用户传进数据中读出驱动能力数据
	        gpio_status->data      = tmp_sys_gpio_data->user_gpio_status.data;      //从用户传进数据中读出data数据
        }
        else                                                                        //当前读出寄存器实际的参数
        {
            port_num_func = (port_num >> 3);
            port_num_pull = (port_num >> 4);

            tmp_val1 = (port_num - (port_num_func<<3)<<2);
            tmp_val2 = (port_num - (port_num_pull<<4)<<1);
            gpio_status->mul_sel   = (PIO_REG_CFG_VALUE(port, port_num_func)>>tmp_val1) & 0x07;       //从硬件中读出功能寄存器
            gpio_status->pull      = (PIO_REG_PULL_VALUE(port, port_num_pull)>>tmp_val2) & 0x03;      //从硬件中读出pull寄存器
            gpio_status->drv_level = (PIO_REG_DLEVEL_VALUE(port, port_num_pull)>>tmp_val2) & 0x03;    //从硬件中读出level寄存器
            if(gpio_status->mul_sel <= 1)
            {
                gpio_status->data = (PIO_REG_DATA_VALUE(port) >> port_num) & 0x01;                     //从硬件中读出data寄存器
            }
            else
            {
                gpio_status->data = -1;
            }
        }

        break;
    }

    return EGON2_OK;
}
/*
**********************************************************************************************************************
*                                               CSP_PIN_Set_One_Gpio_Status
*
* Description:
*				获取用户申请过的GPIO的某一个的状态
* Arguments  :
*		p_handler	:	handler
*		gpio_status	:	保存用户数据的数组
*		gpio_name	:	要操作的GPIO的名称
*       if_get_user_set_flag   :   读取标志，表示读取用户设定数据或者是实际数据
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32  eGon2_GPIO_Set_One_PIN_Status(__hdle p_handler, user_gpio_set_t *gpio_status, const char *gpio_name, __u32 if_set_to_current_input_status)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
    user_gpio_set_t     script_gpio;
    volatile __u32     *tmp_addr;
    __u32               port_num_func, port_num_pull;
    __u32               port, port_num;
    __u32               i, reg_val, tmp_val;

    //检查传进的句柄的有效性
    if((!p_handler) || (!gpio_name))
    {
        return EGON2_FAIL;
    }
    if((if_set_to_current_input_status) && (!gpio_status))
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    if(group_count_max <= 0)
    {
        return EGON2_FAIL;
    }
    user_gpio_set = (system_gpio_set_t *)(tmp_buf + 16);
	//读取用户数据
	//表示读取用户给定的数据
    for(i = 0; i < group_count_max; i++)
    {
        tmp_sys_gpio_data = user_gpio_set + i;             //tmp_sys_gpio_data指向申请的GPIO空间
        if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
        {
            continue;
        }

        port          = tmp_sys_gpio_data->port;                           //读出port数据
        port_num      = tmp_sys_gpio_data->port_num;                       //读出port_num数据
        port_num_func = (port_num >> 3);
        port_num_pull = (port_num >> 4);

        if(if_set_to_current_input_status)                                 //根据当前用户设定修正
        {
            //修改FUCN寄存器
            script_gpio.mul_sel   = gpio_status->mul_sel;
            script_gpio.pull      = gpio_status->pull;
            script_gpio.drv_level = gpio_status->drv_level;
            script_gpio.data      = gpio_status->data;
        }
        else
        {
            script_gpio.mul_sel   = tmp_sys_gpio_data->user_gpio_status.mul_sel;
            script_gpio.pull      = tmp_sys_gpio_data->user_gpio_status.pull;
            script_gpio.drv_level = tmp_sys_gpio_data->user_gpio_status.drv_level;
            script_gpio.data      = tmp_sys_gpio_data->user_gpio_status.data;
        }

        if(script_gpio.mul_sel >= 0)
        {
        	tmp_addr = PIO_REG_CFG(port, port_num_func);
            reg_val = *tmp_addr;                      								 //修改FUNC寄存器
            tmp_val = (port_num - (port_num_func<<3))<<2;
            reg_val &= ~(0x07 << tmp_val);
            reg_val |=  (script_gpio.mul_sel) << tmp_val;
            *tmp_addr = reg_val;
        }
        //修改PULL寄存器
        if(script_gpio.pull >= 0)
        {
        	tmp_addr = PIO_REG_PULL(port, port_num_pull);
            reg_val = *tmp_addr;								                     //修改FUNC寄存器
            tmp_val = (port_num - (port_num_pull<<4))<<1;
            reg_val &= ~(0x07 << tmp_val);
            reg_val |=  (script_gpio.pull) << tmp_val;
            *tmp_addr = reg_val;
        }
        //修改DLEVEL寄存器
        if(script_gpio.drv_level >= 0)
        {
        	tmp_addr = PIO_REG_DLEVEL(port, port_num_pull);
            reg_val = *tmp_addr;                       								  //修改FUNC寄存器
            tmp_val = (port_num - (port_num_pull<<4))<<1;
            reg_val &= ~(0x07 << tmp_val);
            reg_val |=  (script_gpio.drv_level) << tmp_val;
            *tmp_addr = reg_val;
        }
        //修改data寄存器
        if(script_gpio.mul_sel == 1)
        {
            if(script_gpio.data >= 0)
            {
            	tmp_addr = PIO_REG_DATA(port);
                reg_val = *tmp_addr;                                   				   //修改DATA寄存器
                reg_val &= ~(0x01 << port_num);
                reg_val |=  (script_gpio.data & 0x01) << port_num;
                *tmp_addr = reg_val;
            }
        }

        break;
    }

    return EGON2_OK;
}

__s32  eGon2_GPIO_Set_One_PIN_IO_Status(__hdle p_handler, __u32 if_set_to_output_status, const char *gpio_name)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set = NULL, *tmp_sys_gpio_data;
    volatile __u32      *tmp_group_func_addr = NULL;
    __u32               port, port_num, port_num_func;
    __u32				i, reg_val;

    //检查传进的句柄的有效性
    if(!p_handler)
    {
        return EGON2_FAIL;
    }
    if(if_set_to_output_status > 1)
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);
    if(group_count_max == 0)
    {
        return EGON2_FAIL;
    }
    else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
			    tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGON2_FAIL;
	}

    port     = user_gpio_set->port;
    port_num = user_gpio_set->port_num;
    port_num_func = port_num >> 3;

    tmp_group_func_addr = PIO_REG_CFG(port, port_num_func);
    reg_val = *tmp_group_func_addr;
    reg_val &= ~(0x07 << ((port_num - (port_num_func<<3)<<2)));
    reg_val |=  (if_set_to_output_status << ((port_num - (port_num_func<<3)<<2)));
    *tmp_group_func_addr = reg_val;

    return EGON2_OK;
}
__s32  eGon2_GPIO_Set_One_PIN_Pull(__hdle p_handler, __u32 set_pull_status, const char *gpio_name)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
    volatile __u32      *tmp_group_func_addr = NULL;
    __u32               port, port_num, port_num_pull;
	__u32				i, reg_val;
    //检查传进的句柄的有效性
    if(!p_handler)
    {
        return EGON2_FAIL;
    }
    if(set_pull_status >= 4)
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);
    if(group_count_max == 0)
    {
        return EGON2_FAIL;
    }
    else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
			    tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGON2_FAIL;
	}

    port     = user_gpio_set->port;
    port_num = user_gpio_set->port_num;
    port_num_pull = port_num >> 4;

    tmp_group_func_addr = PIO_REG_PULL(port, port_num_pull);
    reg_val = *tmp_group_func_addr;
    reg_val &= ~(0x03 << ((port_num - (port_num_pull<<4)<<1)));
    reg_val |=  (set_pull_status << ((port_num - (port_num_pull<<4)<<1)));
    *tmp_group_func_addr = reg_val;

    return EGON2_OK;
}
__s32  eGon2_GPIO_Set_One_PIN_driver_level(__hdle p_handler, __u32 set_driver_level, const char *gpio_name)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set, *tmp_sys_gpio_data;
    volatile __u32      *tmp_group_func_addr = NULL;
    __u32               port, port_num, port_num_dlevel;
	__u32				i, reg_val;
    //检查传进的句柄的有效性
    if(!p_handler)
    {
        return EGON2_FAIL;
    }
    if(set_driver_level >= 4)
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);

    if(group_count_max == 0)
    {
        return EGON2_FAIL;
    }
    else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
			    tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGON2_FAIL;
	}

    port     = user_gpio_set->port;
    port_num = user_gpio_set->port_num;
    port_num_dlevel = port_num >> 4;

    tmp_group_func_addr = PIO_REG_DLEVEL(port, port_num_dlevel);
    reg_val = *tmp_group_func_addr;
    reg_val &= ~(0x03 << ((port_num - (port_num_dlevel<<4)<<1)));
    reg_val |=  (set_driver_level << ((port_num - (port_num_dlevel<<4)<<1)));
    *tmp_group_func_addr = reg_val;

    return EGON2_OK;
}
__s32  eGon2_GPIO_Read_One_PIN_Value(__hdle p_handler, const char *gpio_name)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set = NULL, *tmp_sys_gpio_data;
    __u32               port, port_num, port_num_func, func_val;
	__u32				i, reg_val;
    //检查传进的句柄的有效性
    if(!p_handler)
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);

    if(group_count_max == 0)
    {
        return EGON2_FAIL;
    }
    else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
			    tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGON2_FAIL;
	}

    port     = user_gpio_set->port;
    port_num = user_gpio_set->port_num;
    port_num_func = port_num >> 3;

    reg_val  = PIO_REG_CFG_VALUE(port, port_num_func);
    func_val = (reg_val >> (port_num - (port_num_func<<3)<<2)) & 0x07;
    if(func_val == 0)
    {
        reg_val = (PIO_REG_DATA_VALUE(port) >> port_num) & 0x01;

        return reg_val;
    }

    return EGON2_FAIL;
}
__s32  eGon2_GPIO_Write_One_PIN_Value(__hdle p_handler, __u32 value_to_gpio, const char *gpio_name)
{
    char               *tmp_buf;                                        //转换成char类型
    __u32               group_count_max;                                //最大GPIO个数
    system_gpio_set_t  *user_gpio_set = NULL, *tmp_sys_gpio_data;
    volatile __u32     *tmp_group_data_addr = NULL;
    __u32               port, port_num, port_num_func, func_val;
	__u32				i, reg_val;
    //检查传进的句柄的有效性
    if(!p_handler)
    {
        return EGON2_FAIL;
    }
    if(value_to_gpio >= 2)
    {
        return EGON2_FAIL;
    }
    tmp_buf = (char *)p_handler;
    group_count_max = *(int *)tmp_buf;
    tmp_sys_gpio_data = (system_gpio_set_t *)(tmp_buf + 16);

    if(group_count_max == 0)
    {
        return EGON2_FAIL;
    }
    else if(group_count_max == 1)
	{
		user_gpio_set = tmp_sys_gpio_data;
	}
	else if(gpio_name)
	{
		for(i=0; i<group_count_max; i++)
		{
			if(strcmp(gpio_name, tmp_sys_gpio_data->gpio_name))
			{
                tmp_sys_gpio_data ++;
				continue;
			}
			user_gpio_set = tmp_sys_gpio_data;
			break;
		}
	}
	if(!user_gpio_set)
	{
		return EGON2_FAIL;
	}

    port     = user_gpio_set->port;
    port_num = user_gpio_set->port_num;
    port_num_func = port_num >> 3;

    reg_val  = PIO_REG_CFG_VALUE(port, port_num_func);
    func_val = (reg_val >> (port_num - (port_num_func<<3)<<2)) & 0x07;
    if(func_val == 1)
    {
        tmp_group_data_addr = PIO_REG_DATA(port);
        reg_val = *tmp_group_data_addr;
        reg_val &= ~(1 << port_num);
        reg_val |=  (value_to_gpio << port_num);
        *tmp_group_data_addr = reg_val;

        return EGON2_OK;
    }

    return EGON2_FAIL;
}


