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
#include "sw_iic.h"

#define   i2c_ctl    twi_ports[twi_index]


static sw_iic_t twi_ports[3] = {
	(sw_iic_t)CFG_SW_IIC_CTL0,
	(sw_iic_t)CFG_SW_IIC_CTL1,
	(sw_iic_t)CFG_SW_IIC_CTL2
};

static   __u32  twi_index = 0;

static void _for_loop(unsigned int loop)
{
	unsigned int i;

	for( i = 0; i < loop; i++ );
}
__s32  eGon2_twi_setfreq(__u32 system_clock, __u32 twi_run_clock);
/*
**********************************************************************************************************************
*                                               sw_iic_exit
*
* Description:  通过IIC控制器读取IIC设备一个字节，暂时只支持标准的设备
*
* Arguments  :
*
* Returns    :   读取成功返回0，否则返回 -1
*
* Notes      :    none
*
**********************************************************************************************************************
*/
static __s32 _iic_SendStart(void)
{
    __s32  time = 0xfffff;
    __u32  tmp_val;

    i2c_ctl->srst = 1;

	i2c_ctl->ctl |= 0x20;

    while((time--)&&(!(i2c_ctl->ctl & 0x08)));
	if(time <= 0)
	{
		return SW_IIC_OPERATION_START_NO_INT;
	}

	tmp_val = i2c_ctl->status;
    if(tmp_val != 0x08)
    {
		return -1;
    }

    return SW_IIC_OPERATION_NO_ERR;
}
/*
**********************************************************************************************************************
*                                               TWIC_SendReStart
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
__s32 _iic_SendReStart(void)
{
    __s32  time = 0xffff;
    __u32  tmp_val;

	tmp_val = i2c_ctl->ctl & 0xC0;
	tmp_val |= 0x20;
	i2c_ctl->ctl = tmp_val;

    while( (time--) && (!(i2c_ctl->ctl & 0x08)) );
	if(time <= 0)
	{
		return -1;
	}

	tmp_val = i2c_ctl->status;
    if(tmp_val != 0x10)
    {
		return -1;
    }

    return 0;
}
/*
**********************************************************************************************************************
*                                               TWIC_SendSlaveAddr
*
* Description:
*
* Arguments  :
*
* Returns    :    EPDK_OK = successed;   EPDK_FAIL = failed
*
* Notes      :     none
*
**********************************************************************************************************************
*/
static __s32 _iic_SendSlaveAddr(__u32 saddr,  __u32 rw)
{
    __s32  time = 0xffff;
    __u32  tmp_val;

	if(( rw != SW_IIC_READ ) && ( rw != SW_IIC_WRITE ))
	{
		return -1;
	}

	i2c_ctl->data = ((saddr & 0xff) << 1) | rw;
	i2c_ctl->ctl &=  0xF7;

	while(( time-- ) && (!( i2c_ctl->ctl & 0x08 )));
	if(time <= 0)
	{
		return -1;
	}

	tmp_val = i2c_ctl->status;
	if(rw == SW_IIC_WRITE)//+write
	{
		if(tmp_val != 0x18)
		{
			return -1;
		}
	}
	else//+read
	{
		if(tmp_val != 0x40)
		{
			return -1;
		}
	}

	return 0;
}
/*
**********************************************************************************************************************
*                                               _iic_SendByteAddr
*
* Description:
*
* Arguments  :
*
* Returns    :    EPDK_OK = successed;   EPDK_FAIL = failed
*
* Notes      :     none
*
**********************************************************************************************************************
*/
static __s32 _iic_SendByteAddr(__u32  byteaddr)
{
    __s32  time = 0xffff;
    __u32  tmp_val;

	i2c_ctl->data = byteaddr & 0xff;
	i2c_ctl->ctl &= 0xF7;

	while( (time--) && (!(i2c_ctl->ctl & 0x08)) );
	if(time <= 0)
	{
		return -1;
	}

	tmp_val = i2c_ctl->status;
	if(tmp_val != 0x28)
	{
		return -1;
	}

	return 0;
}
/*
**********************************************************************************************************************
*                                               TWIC_GetData
*
* Description:
*
* Arguments  :
*
* Returns    :    EPDK_OK = successed;   EPDK_FAIL = failed
*
* Notes      :     none
*
**********************************************************************************************************************
*/
static __s32 _iic_GetData(__u8 *data_addr, __u32 data_count)
{
    __s32  time = 0xffff;
    __u32  tmp_val, i;

	if(data_count == 1)
	{
		i2c_ctl->ctl &= 0xF7;

		while( (time--) && (!(i2c_ctl->ctl & 0x08)) );
		if(time <= 0)
		{
			return -1;
		}
		for(time=0;time<100;time++);
		*data_addr = i2c_ctl->data;

		tmp_val = i2c_ctl->status;
		if(tmp_val != 0x58)
		{
			return -1;
		}
	}
	else
	{
		for(i=0; i< data_count - 1; i++)
		{
			time = 0xffff;
			tmp_val = i2c_ctl->ctl & 0xF7;
			tmp_val |= 0x04;
		    i2c_ctl->ctl = tmp_val;

			while( (time--) && (!(i2c_ctl->ctl & 0x08)) );
			if(time <= 0)
			{
				return -1;
			}
			for(time=0;time<100;time++);
			time = 0xffff;
			data_addr[i] = i2c_ctl->data;
		    while( (time--) && (i2c_ctl->status != 0x50) );
			if(time <= 0)
			{
				return -1;
			}
		}

        time = 0xffff;
		i2c_ctl->ctl &= 0xF3;
		while( (time--) && (!(i2c_ctl->ctl & 0x08)) );
		if(time <= 0)
		{
			return -1;
		}
		for(time=0;time<100;time++);
		data_addr[data_count - 1] = i2c_ctl->data;
	    while( (time--) && (i2c_ctl->status != 0x58) );
		if(time <= 0)
		{
			return -1;
		}
	}

	return 0;
}
/*
**********************************************************************************************************************
*                                               _iic_SendData
*
* Description:
*
* Arguments  :
*
* Returns    :    EPDK_OK = successed;   EPDK_FAIL = failed
*
* Notes      :     none
*
**********************************************************************************************************************
*/
static __s32 _iic_SendData(__u8  *data_addr, __u32 data_count)
{
    __s32  time = 0xffff;
    __u32  i;

	for(i=0; i< data_count; i++)
	{
		time = 0xffff;
		i2c_ctl->data = data_addr[i];
		i2c_ctl->ctl &= 0xF7;

		while( (time--) && (!(i2c_ctl->ctl & 0x08)) );
		if(time <= 0)
		{
			return -1;
		}
		time = 0xffff;
		while( (time--) && (i2c_ctl->status != 0x28) );
		if(time <= 0)
		{
			return -1;
		}
	}

	return 0;
}
/*
**********************************************************************************************************************
*                                               _iic_SendSlaveAddr10
*
* Description:
*
* Arguments  :
*
* Returns    :    EPDK_OK = successed;   EPDK_FAIL = failed
*
* Notes      :     none
*
**********************************************************************************************************************
*/
__s32 _iic_SendSlaveAddr10(__u32  slave_addr,  __u32  rw)
{
    if(_iic_SendSlaveAddr(0x78 | ((slave_addr >> 8) & 0x03), rw) != 0)
    {
		return -1;
	}
	if(_iic_SendData((__u8 *)&slave_addr, SW_IIC_READ) != 0)
	{
		return -1;
	}

	return 0;
}
/*
**********************************************************************************************************************
*                                               _iic_Stop
*
* Description:
*
* Arguments  :
*
* Returns    :    EPDK_OK = successed;   EPDK_FAIL = failed
*
* Notes      :     none
*
**********************************************************************************************************************
*/
__s32 _iic_Stop(void)
{
    __s32  time = 0xffff;
    __u32  tmp_val;

	tmp_val = (i2c_ctl->ctl & 0xC0) | 0x10;
	i2c_ctl->ctl = tmp_val;
	while( (time--) && (i2c_ctl->ctl & 0x10) );
	if(time <= 0)
	{
		return -1;
	}
	time = 0xffff;
	while( (time--) && (i2c_ctl->status != 0xf8) );
	tmp_val = i2c_ctl->status;
	if(tmp_val != 0xf8)
	{
		return -1;
	}

	return 0;
}
/*
**********************************************************************************************************************
*                                               sw_iic_init
*
* Description:    Init I2C controller
*
* Arguments  :    sw_iic_t i2c_ctl
*
* Returns    :    0 = successed;   -1 = failed
*
* Notes      :    none
*
**********************************************************************************************************************
*/
__s32 eGon2_twi_init(__s32 twi_port, void *twi_ctrl, __u32 system_clock, __u32 twi_run_clock)
{
    __s32   reset_delay;

    CCMU_REG_APB_MOD1 &= ~(1 << (0 + twi_port));
    _for_loop(100);
    CCMU_REG_APB_MOD1 |=  (1 << (0 + twi_port));

    eGon2_GPIO_Set_cfg((normal_gpio_cfg *)twi_ctrl, 2, 1);

    twi_index = twi_port;

    reset_delay = 0xffff;
    i2c_ctl->srst = 1;
    while((i2c_ctl->srst) && (reset_delay--));
    if(reset_delay <= 0)
    {
        return -1;
    }

    return eGon2_twi_setfreq(system_clock, twi_run_clock);
}
/*
**********************************************************************************************************************
*                                               sw_iic_setfreq
*
* Description:    SET I2C controller Frequency
*
* Arguments  :    sw_iic_t i2c_ctl
*
* Returns    :    0 = successed;   -1 = failed
*
* Notes      :    none
*
**********************************************************************************************************************
*/
__s32  eGon2_twi_setfreq(__u32 system_clock, __u32 twi_run_clock)
{
    __u32 clk_m        = 0;
    __u32 clk_n        = 0;
    __u32 src_clk      = system_clock / 10;
    __u32 _2_pow_clk_n = 1;
    __u32 sclk_real    = 0;
    __u32 divider      = src_clk / twi_run_clock;

	if (system_clock <= 32000)
	{
		clk_m = 1;
        clk_n = 0;
	}
    else if (system_clock <= 24000000)        //不到24M的情况
    {
        clk_m = 2;
        clk_n = 1;
    }
    else
    {
        while (clk_n < 8)
        {
            clk_m = (divider/_2_pow_clk_n) - 1;
            while (clk_m < 16)
            {
                sclk_real = src_clk/(clk_m+1)/_2_pow_clk_n;
                if (sclk_real <= twi_run_clock)
                    goto set_clk;
                else
                    clk_m++;
            }

            clk_n++;
            _2_pow_clk_n <<= 1;
        }
    }

set_clk:
    i2c_ctl->clk = (clk_m<<3) | clk_n;
    i2c_ctl->ctl = 0x40;
    i2c_ctl->eft = 0;

    return 0;
}

/*
**********************************************************************************************************************
*                                               sw_iic_exit
*
* Description:  exit I2C controller
*
* Arguments  :   sw_iic_t i2c_ctl
*
* Returns    :   0 = successed;   -1 = failed
*
* Notes      :    none
*
**********************************************************************************************************************
*/
__s32 eGon2_twi_exit(void)
{
    CCMU_REG_APB_MOD1 &= ~(1 << (0 + twi_index));

    return 0;
}
/*
****************************************************************************************************
*
*                                       sw_iic_read
*
*  Description:
*
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 eGon2_twi_read(void  *twi_arg)
{
    __u32 tmp, rw_flag;
    __s32 i;
    __s32 ret = -1;
    _twi_arg_t  *arg = (_twi_arg_t *)twi_arg;

    if(arg->if_restart)
    {
        tmp = 0;
        rw_flag  = SW_IIC_WRITE;
    }
    else if(arg->byte_addr_width <= 3)
    {
        tmp = arg->byte_addr_width;
        rw_flag  = SW_IIC_READ;
    }
    else
    {
        return -1;
    }

    i2c_ctl->eft = tmp;
    if(_iic_SendStart() != 0)
	{
		ret = -1;
		goto _iic_read_stop;
	}
	if(arg->slave_addr_flag)//10 bit saddr
	{
    	if(_iic_SendSlaveAddr10(arg->slave_addr, rw_flag) != 0)
    	{
    		ret = -1;
		    goto _iic_read_stop;
    	}
	}
	else
	{
	    if(_iic_SendSlaveAddr(arg->slave_addr, rw_flag) != 0)
	    {
    		ret = -1;
		    goto _iic_read_stop;
    	}
    }
    //send byte address
    for (i = arg->byte_addr_width-1; i>=0; i--)
    {
    	if(_iic_SendByteAddr(arg->byte_addr[i] & 0xff) != 0)
    	{
    		ret = -1;
		    goto _iic_read_stop;
    	}
    }
    //restart
    if(arg->if_restart)
    {
        if(_iic_SendReStart() != 0)
    	{
    		ret = -1;
		    goto _iic_read_stop;
    	}
        //send slave address + read flag
    	if(arg->slave_addr_flag)//10bit saddr
    	{
        	if(_iic_SendSlaveAddr10(arg->slave_addr, SW_IIC_READ) == -1)
        	{
        		ret = -1;
		        goto _iic_read_stop;
        	}
    	}
    	else
    	{
    	    if(_iic_SendSlaveAddr(arg->slave_addr, SW_IIC_READ) == -1)
    	    {
        		ret = -1;
		        goto _iic_read_stop;
        	}
        }
    }

    //get data
	if(_iic_GetData(arg->data, arg->byte_count) != 0)
	{
		ret = -1;
		goto _iic_read_stop;
	}
    ret = 0;

_iic_read_stop:
	if(_iic_Stop() != 0)
	{
		ret = -1;
	}

	//restore the byte address counter to default
	return ret;
}

/*
****************************************************************************************************
*
*             TWIC_Write
*
*  Description:
*       DRV_MOpen
*
*  Parameters:
*
*  Return value:
*       EPDK_OK
*       EPDK_FAIL
****************************************************************************************************
*/
__s32 eGon2_twi_write(void  *twi_arg)
{
    __s16 i;
    __s32 ret = -1;
    _twi_arg_t  *arg = (_twi_arg_t *)twi_arg;

    i2c_ctl->eft = 0;
	if(_iic_SendStart() != 0)
	{
		ret = -1;
		goto _iic_write_stop;
	}
	if(arg->slave_addr_flag)
	{
    	if(_iic_SendSlaveAddr10(arg->slave_addr, SW_IIC_WRITE) != 0)
    	{
    		ret = -1;
		    goto _iic_write_stop;
    	}
	}
	else
	{
	    if(_iic_SendSlaveAddr(arg->slave_addr, SW_IIC_WRITE) != 0)
	    {
    		ret = -1;
		    goto _iic_write_stop;
    	}
    }

    for (i = arg->byte_addr_width-1; i >= 0; i--)
    {
    	if(_iic_SendByteAddr(arg->byte_addr[i] & 0xff) != 0)
    	{
    		ret = -1;
		    goto _iic_write_stop;
    	}
    }

	if(_iic_SendData(arg->data, arg->byte_count) != 0)
	{
		ret = -1;
		goto _iic_write_stop;
	}
    ret = 0;

_iic_write_stop:
	if(_iic_Stop() != 0)
	{
		ret = -1;
	}

	return ret;
}


__s32 BOOT_TWI_Read(__u32 arg1, __u8 *arg2, __u8 *arg3)
{
    _twi_arg_t  twi_data;

    twi_data.slave_addr = arg1;
    twi_data.slave_addr_flag = 0;
    twi_data.byte_addr  = arg2;
    twi_data.byte_addr_width = 1;
    twi_data.byte_count = 1;
    twi_data.data = arg3;
    twi_data.if_restart = 1;

    return eGon2_twi_read((void *)&twi_data);
}

__s32  BOOT_TWI_Write(__u32 arg1, __u8 *arg2, __u8 *arg3)
{
    _twi_arg_t  twi_data;

    twi_data.slave_addr = arg1;
    twi_data.slave_addr_flag = 0;
    twi_data.byte_addr  = arg2;
    twi_data.byte_addr_width = 1;
    twi_data.byte_count = 1;
    twi_data.data = arg3;

    return eGon2_twi_write((void *)&twi_data);
}

__s32 BOOT_TWI_ReadEx(__u32 arg1, __u8 *arg2, __u8 *arg3, __u8 count)
{
    _twi_arg_t  twi_data;

    twi_data.slave_addr = arg1;
    twi_data.slave_addr_flag = 0;
    twi_data.byte_addr  = arg2;
    twi_data.byte_addr_width = 1;
    twi_data.byte_count = count;
    twi_data.data = arg3;
    twi_data.if_restart = 1;

    return eGon2_twi_read((void *)&twi_data);
}

__s32  BOOT_TWI_WriteEx(__u32 arg1, __u8 *arg2, __u8 *arg3, __u8 count)
{
    _twi_arg_t  twi_data;

    twi_data.slave_addr = arg1;
    twi_data.slave_addr_flag = 0;
    twi_data.byte_addr  = arg2;
    twi_data.byte_addr_width = 1;
    twi_data.byte_count = count;
    twi_data.data = arg3;

    return eGon2_twi_write((void *)&twi_data);
}





