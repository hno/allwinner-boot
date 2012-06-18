/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, Allwinner Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : sdcard_op.c
*
* Author : Jerry
*
* Version : 1.1.0
*
* Date : 2009.5
*
* Description :
*
* Others : None
*
*
* History :
*
*
************************************************************************************************************************
*/
#include  "sdcard_op.h"

CARD_INFO_T             card_info[1];
__register_grout_t     *sd_reg = (__register_grout_t *)0x01c10f00;

/*tran speed rate unit by 100kbit/s*/
__u32 transpeed_rate_unit[8] = {1,10,100,1000,0,0,0,0};
/*tran speed time value unit by 0.1*/
__u32 transpeed_time_value[16] = {0,10,12,13,15,20,25,30,35,40,45,50,55,60,70,80};
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
__s32 _get_card_status(__u8 *response)
{
    SDC_CMD_T cmd = {0};

	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = AC;
	cmd.arg = card_info->rca << 16;
	cmd.index = SEND_STATUS;
	cmd.response_len = 0;
	cmd.check_response_crc = 1;

	if (CARD_OK != SDC_SendCmd(&cmd, response))
		return -1;

	return 0;
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
__s32 _to_idle_state(void)
{
	SDC_CMD_T cmd = {0};

	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = BC;
	cmd.arg = 0;
	cmd.index = GO_IDLE_STATE;
	cmd.send_initialization = 1;

	if(0 != SDC_SendCmd(&cmd, NULL))
		return -1;

	return 0;
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
/*addressed card goes into transfer state after selected*/
__s32 _select_card(void)
{
	SDC_CMD_T cmd = {0};
	__u8 response[128];

	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = AC;
	cmd.arg = card_info->rca << 16;
	cmd.index = SELECT_OR_DESECLT_CARD;
	cmd.response_len = 0;
	cmd.check_response_crc = 1;

	if (0 != SDC_SendCmd(&cmd, response))
		return -1;

	return 0;
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
__s32 _get_card_type(void)
{
	SDC_CMD_T  cmd = {0};
	__u8       response[128];

	/*make cards to idle state*/
	if(0 != _to_idle_state())
		return -1;

	/*try to detect card*/
	/*is sd mem2.0 card?*/
	/*send CMD8 , VHS = 0001b, check pattern = 0xaa*/
	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = BCR;
	cmd.arg = 0x1aa;
	cmd.index = SEND_IF_COND;

	if (0 == SDC_SendCmd(&cmd, response))
	{
		/*invalid response?*/
		if ((((__u32 *)response)[0] & 0xfff) == 0x1aa)
		{
			//__inf("SD 2.0 card found\n");
			card_info->type = SDMEM;

			return 0;
		}
	}
	/*is sd mem 1.x?*/
	/*send ACMD41*/
	cmd.mode = SD_SPECIFIC_CMD;
	cmd.type = AC;
	cmd.arg = 0;
	cmd.index = APP_CMD;
	if (0 == SDC_SendCmd(&cmd,response))
	{
		cmd.type = BCR;
		cmd.arg = 0x40000000|0xFF0000;
		cmd.index = SD_APP_OP_COND;

		if(0 == SDC_SendCmd(&cmd, response))
		{
			//__inf("SD 1.x card found\n");
			card_info->type = SDMEM;

			return 0;
		}
	}

	/*is MMC card*/
	card_info->type = MMC;

	return 0;
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
__s32 _detail_sdmem_type(void)
{
	__u32 time;
	SDC_CMD_T cmd = {0};
    __u8   is_sd20 = 0;
    __u8   response[128];
    __s32  try_time = 6000;

	/*make cards to idle state*/
	if(0 != _to_idle_state())
		return -1;

	/*send CMD8 , VHS = 0001b, check pattern = 0xaa*/
	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = BCR;
	cmd.arg = 0x1aa;
	cmd.index = SEND_IF_COND;

	if (0 == SDC_SendCmd(&cmd, response))
	{
		/*invalid response?*/
		if ((((__u32 *)response)[0] & 0xfff) == 0x1aa)
			is_sd20 = 1;
	}

	/*send ACMD41,HCS = 0 or 1b,Vdd Vol = 3.0 - 3.6 v*/
	while(1)
	{
	    if(try_time <= 0)
	    {
	        return -1;
	    }
	    try_time --;
		memset(&cmd,0x0,sizeof(SDC_CMD_T));
		cmd.mode = SD_SPECIFIC_CMD;
		cmd.type = AC;
		cmd.arg = 0;
		cmd.index = APP_CMD;
		if (0 != SDC_SendCmd(&cmd,response))
			return -1;
		cmd.type = BCR;

		/*HSC = 1 if support CMD8*/
		if (is_sd20)
			cmd.arg |= (__u32)1 << 30;

		cmd.arg |= 0xFC0000;
		cmd.index = SD_APP_OP_COND;

		/*response timeout or crc err?*/
		if (0 != SDC_SendCmd(&cmd, response))
			return -1;

		/*vol is compatible?*/
		if ((*((__u32 *)response) & 0xFC0000) != 0xFC0000)
			return -1;
		time = 0;
		while(time++ < 1250);
		/* is ready?*/
		if ( *((__u32 *)response) & ((__u32)1 << 31) )
		{
			break;
	    }
	};

	/*now ,the card is in ready state*/
	if (is_sd20)
	{
		if ( *((__u32 *)response) & ((__u32)1 << 30) )
			card_info->type |= HIGH_CAP;
	}

	return 0;
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
__s32 _detail_mmc_type(void)
{
	SDC_CMD_T cmd = {0};
	__u32 time;
	__u8 response[128];
	__s32  try_time = 200;

	/*make cards to idle state*/
	if(0 != _to_idle_state())
		return -1;

	/*send CMD1, to ready state*/
	cmd.mode = MMC_SPECIFIC_CMD;
	cmd.type = BCR;
	cmd.arg = (__u32)1 << 30 | 0xFF8000;
	cmd.index = MMC_SEND_OF_COND;

	while(1)
	{
	    if(try_time <= 0)
	    {
	        return -1;
	    }
	    try_time --;
		if (0 != SDC_SendCmd(&cmd, response))
			return -1;

		/*vol is compatible?*/
		if ( (*((__u32 *)response) & 0xFF8000) != 0xFF8000)
			return -1;

		/* is ready?*/
		if ( *((__u32 *)response) & ((__u32)1 << 31) )
			break;

		time = 0;
		while(time++ < 1250);
	}
	/*response sector mode 10b or byte mode 00b,indicate the capacity*/
	if ( *((__u32 *)response) & ((__u32)1 << 30) )
		card_info->type |= HIGH_CAP;

	return 0;
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
__s32 _to_identification_state(void)
{
	SDC_CMD_T cmd = {0};
	__u8 response[128];

	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = BCR;
	cmd.arg = 0;
	cmd.index = ALL_SEND_CID;
	cmd.response_len = 1;

	if ( 0 != SDC_SendCmd(&cmd, response))
		return -1;

	return 0;
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
__s32 _publish_rca_to_standby_state(void)
{
	SDC_CMD_T cmd = {0};
	__u8   response[128];

	/*send CMD3 , the card publishes a new RCA , then go into stand by state*/
	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = BCR;
	cmd.arg = 0;
	cmd.index = SEND_RELATIVE_ADDR;
	cmd.response_len = 0;
	cmd.check_response_crc = 1;
	if ( 0 != SDC_SendCmd(&cmd, response))
		return -1;

	card_info->rca = (*((__u32 *)response) >> 16) & 0xffff;

	return 0;
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
__s32 _switch_sdmem_bus(__u32 tmp_bus_width)
{
	__u32     temp;
	SDC_CMD_T cmd = {0};
	__u8 response[128];

	/*set bus_width*/
	cmd.mode = SD_SPECIFIC_CMD;
	cmd.type = AC;
	cmd.arg = card_info->rca << 16;
	cmd.index = APP_CMD;
	if (0 != SDC_SendCmd(&cmd, response))
	{
		return -1;
	}
	cmd.mode = SD_SPECIFIC_CMD;
	cmd.type = AC;
	cmd.arg = 0;
	temp = (tmp_bus_width) >> 1;
	while(temp)
	{
		temp >>= 1;
		cmd.arg++;
	}
	cmd.index = SET_BUS_WIDTH;
	if (0 != SDC_SendCmd(&cmd, response))
	{
		return -1;
	}

	return 0;
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
__s32 _switch_mmc_bus(__u32 tmp_bus_width)
{
	SDC_CMD_T cmd = {0};
    __u8 response[128];

	/*switch bus width,send CMD6(MMC_SWITCH)*/
	cmd.mode = MMC_SPECIFIC_CMD;
	cmd.type = AC;
	cmd.arg = ((tmp_bus_width >> 2) << 8) | (0xb7 << 16) | (0x3)<<24;
	cmd.index = MMC_SWITCH;
	cmd.check_response_crc = 1;

	if (0 != SDC_SendCmd(&cmd, response))
	{
		return -1;
	}

	return 0;
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
__s32 _get_src(__u32 card_no, __u32 *bus_width)
{
    SDC_CMD_T cmd = {0};
    __u8  response[128];

	if ((card_info->type & 0xf) == SDMEM)
    {
        /* get scr*/
        cmd.mode = SD_SPECIFIC_CMD;
	    cmd.type = AC;
	    cmd.arg = card_info->rca << 16;
	    cmd.index = APP_CMD;

	    if (CARD_OK == SDC_SendCmd(&cmd, response))
	    {
	        memset(&cmd, 0x0, sizeof(SDC_CMD_T));
        	cmd.mode = SD_SPECIFIC_CMD;
        	cmd.type = ADTC;
        	cmd.arg = 0;
        	cmd.index = SEND_SCR;

        	if (0 != SDC_ReadDataExt(&cmd, card_info->scr, 64/8, card_no))
        	{
        		return -1;
        	}
            *bus_width           = (card_info->scr[0]>>8) & 0x0f;
    	    card_info->spec_ver  = (card_info->scr[0]>>0) & 0x03;
    	    if(*bus_width == 5)
    	    {
    	        *bus_width = 4;
    	    }
    	    else
    	    {
    	        *bus_width = 1;
    	    }
        }
    }

    return 0;
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
__s32 _switch_sdmem_speed_mode(__u32 card_no)
{
	__u8 switch_status[64];
	SDC_CMD_T cmd = {0};

    if(card_info->spec_ver == 0)
    {
        return 0;
    }
	/*send cmd6,mode = 1,fuction group 1, function 1 set 0;*/
	cmd.mode = SD_SPECIFIC_CMD;
	cmd.type = ADTC;
	cmd.arg = ((__u32)0 << 31) | (0xfffff1);
	cmd.index = SWITCH_FUNC;

	if (CARD_OK != SDC_ReadDataExt(&cmd, switch_status, 512/8, card_no))
	{
		return -1;
	}
	/*check status*/
	if((switch_status[12] != 0x80) || (switch_status[13] != 0x03))
	{
	    return -1;
	}
	if ((switch_status[16] & 0xf) != 0x01)
	{
		return -1;
	}
    //switch
	memset(&cmd, 0x0, sizeof(SDC_CMD_T));

	/*send cmd6,mode = 1,fuction group 1, function 1 set 0;*/
	cmd.mode = SD_SPECIFIC_CMD;
	cmd.type = ADTC;
	cmd.arg = ((__u32)1 << 31) | (0xfffff1);
	cmd.index = SWITCH_FUNC;

	if (CARD_OK != SDC_ReadDataExt(&cmd, switch_status, 512/8, card_no))
	{
		return -1;
	}

	/*check status*/
	if ((switch_status[16] & 0xf) != 0x01)
	{
		return -1;
	}
	card_info->type |= HIGH_SPEED;

	return 0;

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
__s32 _switch_mmc_speed_mode(__u32 card_no)
{
	SDC_CMD_T cmd = {0};
	__u8   response[128];

	/*send cmd6 with value = 0x1,index = 0xb9(byte 185),access = 0x3*/
	cmd.mode = MMC_SPECIFIC_CMD;
	cmd.type = AC;
	cmd.arg = (1 << 8) | (0xb9 << 16) | (0x3 << 24);
	cmd.index = MMC_SWITCH;

	if (0 != SDC_SendCmd(&cmd, response))
	{
		return -1;
	}

	return 0;

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
__s32 _set_speed_mode(__u32 card_no)
{
	switch(card_info->type & 0xf)
	{
		case SDMEM:
			return (_switch_sdmem_speed_mode(card_no));
		case MMC:
		//case CEATA:
		    if (!(card_info->type & HIGH_SPEED))
		    {
		        return 0;
		    }
		    else
		    {
			    return (_switch_mmc_speed_mode(card_no));
			}

		default:
			return -1;
	}
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
__s32 _get_cid_csd(void)
{
	SDC_CMD_T cmd = {0};
	__u8 TRAN_SPEED,READ_BL_LEN,C_SIZE_MULTI;
	__u32 C_SIZE;

	/*get cid*/
	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = AC;
	cmd.arg = card_info->rca << 16;
	cmd.index = SEND_CID;
	cmd.response_len = 1;
	cmd.check_response_crc = 1;
	if (CARD_OK != SDC_SendCmd(&cmd, card_info->cid))
		return CARD_FAIL;

	/*get csd*/
	memset(&cmd,0x0,sizeof(SDC_CMD_T));
	cmd.mode = SD_PROPRIETARY_CMD;
	cmd.type = AC;
	cmd.arg = card_info->rca << 16;
	cmd.index = SEND_CSD;
	cmd.response_len = 1;
	cmd.check_response_crc = 1;
	if (CARD_OK != SDC_SendCmd(&cmd, card_info->csd))
		return CARD_FAIL;

	/*compute max bus_width*/
	if ((card_info->type & 0xf) == MMC)
	{
		/*mmc 4.0 or later version support sd 1/4/8 bit bus,8 bit can not support*/
		if ((card_info->csd[3] & 0x3c000000) == 0x10000000)
			card_info->bus_width = 4;
		else
			card_info->bus_width = 1;

    	/*compute max tran speed,bit[103:96]*/
    	TRAN_SPEED = (card_info->csd[3] & 0xff);
    	card_info->tran_speed = transpeed_rate_unit[TRAN_SPEED & 0x7]
    									*transpeed_time_value[(TRAN_SPEED & 0x78) >> 3] / 100;
		if (card_info->tran_speed > 20)
			card_info->type |= HIGH_SPEED;
    }

	/*compute user capacity*/
	if ( ((card_info->type) & (~HIGH_SPEED)) == (SDMEM | HIGH_CAP))
	{
		/*high capacity SD 2.0 card ,READ_BL_LEN - bit[83:80],C_SIZE-bit[69:48]*/
		READ_BL_LEN = (card_info->csd[2] >> 16 ) & 0xf;
		C_SIZE = ((card_info->csd[1] >> 16)&0xffff) | ((card_info->csd[2] & 0x3f) << 16);
		card_info->volume = (1 << READ_BL_LEN) * (C_SIZE+1) * 2;
	}
	else
	{
		/*READ_BL_LEN - bit[83:80],C_SIZE-bit[73:62],C_SIZE_MULTI-bit[49:47]*/
		READ_BL_LEN = (card_info->csd[2] >> 16 ) & 0xf;
		C_SIZE = ((card_info->csd[1] >> 30)&0x3) | ((card_info->csd[2] & 0x3ff) << 2);
		C_SIZE_MULTI = (card_info->csd[1] >> 15) & 0x7;
		card_info->volume = (1 << READ_BL_LEN) * (C_SIZE + 1) * (1 << (2+C_SIZE_MULTI)) / 512;
	}

	return 0;
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
__s32 _identificate_card(__u32 system_clock)
{
	SDC_CARD_CONFIG_INFO_T default_card_info = {0,0,1,512,0xffffff,0xff};

	default_card_info.clock_div = system_clock * 1000/2/200;
	/*default config card*/
	if ( 0 != SDC_ConfigCardHost(&default_card_info))
	{
		return -1;
	}

	/*get card type*/
	if (0 != _get_card_type())
	{
		return -1;
	}
	/*verify card type,then card is in ready state*/
	if(card_info->type == SDMEM)
	{
	    if(-1 == _detail_sdmem_type())
	    {
	        return -1;
	    }
	}
	else
	{
	    if(-1 == _detail_mmc_type())
	    {
	        return -1;
	    }
	}

	/*send CMD2, send CID,then the card is in identification state*/
	if (0 != _to_identification_state())
		return -1;

	/*send CMD3, publish RCA, then the card is in stand by state*/
	if (0 != _publish_rca_to_standby_state())
		return -1;

	card_info->type &= ~HIGH_SPEED;
	card_info->bus_width = 1;

	switch(card_info->type & 0xf)
	{
		case SDMEM:
		case MMC:
//		case CEATA:
		/*get CID and CSD, set bus_width , speed mode and user capacity*/
			if(0 != _get_cid_csd())
				return -1;
			break;

		default:
			return -1;
	}

	return 0;
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
__s32 _set_bus_width(__u32 tmp_bus_width)
{
	/*default bus width is 1*/
	if (tmp_bus_width == 1)
		return 0;

	switch((card_info->type & 0xf))
	{
		case SDMEM:
			return(_switch_sdmem_bus(tmp_bus_width));
//		case CEATA:
		case MMC:
			return(_switch_mmc_bus(tmp_bus_width));
//		case SDIO:
//			return(_switch_sdio_bus());
		default:
			return -1;
	}

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
__s32 _config_card(__u32 sdcard_clock, __u32 speed_mode, __u32 select_line, __u32 card_no)
{
	SDC_CARD_CONFIG_INFO_T   typical_card_info;
	__u32  dst_clk, div_factor = 0;
    __u32  tmp_bus_width = 1;
	/*select card*/
	if (0 !=_select_card())
		return -1;

    _get_src(card_no, &tmp_bus_width);
    if(speed_mode)
    {
        _set_speed_mode(card_no);
    }
    /*set bus_width*/
    if(select_line != 1)
    {
	    if(!(_set_bus_width(tmp_bus_width)))
	    {
	        card_info->bus_width = tmp_bus_width;
	    }
	    else
	    {
	        card_info->bus_width = 1;
        }
	}
	/*typical config card*/
	/*set typical argument*/
	dst_clk = 25;
	if(card_info->type & HIGH_SPEED)     //如果是高速卡
    {
        dst_clk = 50;
    }
    if(sdcard_clock <= dst_clk)
    {
        div_factor = 0;
    }
    else
    {
        div_factor = sdcard_clock/(dst_clk<<1);
        if(sdcard_clock%dst_clk)
        {
            div_factor ++;
        }
    }

	typical_card_info.clock_div = div_factor;
	typical_card_info.bus_width = card_info->bus_width;
	typical_card_info.power_mode = 0;
	typical_card_info.block_size = 512;
	typical_card_info.data_timeout = 0xffffff;
	typical_card_info.response_timeout = 0xff;

	if( 0 != SDC_ConfigCardHost(&typical_card_info))
	{
		return -1;
	}

	return 0;
}
/*******************************************************************************
*								SD_Init
*
* Description : Initialize SD card controller，reset SDC，set dma，set irq .

* Arguments	  :

*

* Returns	  :	0 = success,-1 = fail.

* Notes		  : none
********************************************************************************/
__s32 SDMMC_Init(__s32 card_no, __u32 sysclk, __u32 speed_mode, __u32 line_mode)
{
    card_no &= 0x03;
    sd_reg = (__register_grout_t *)(0x01c0f000 + card_no * 0x1000);
	/*initial sd control machine*/
	if(SDC_Init())
	{
	    return -1;
	}

	/*card identification*/
	if(0 == _identificate_card(sysclk))
	{
		if(0 !=_config_card(sysclk, speed_mode, line_mode, card_no))
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	sd_reg->sd_reg_ctrl |= (1 << 6);

	return card_info->volume;
}
/*******************************************************************************
*								SD_Exit
*
* Description : 配置pin到输入状态，配置clock到关闭状态

* Arguments	  :

*

*

* Returns	  :	0

* Notes		  : none
********************************************************************************/
__s32 SDMMC_Exit(__s32 gpio_no)
{
	return SDC_Exit(gpio_no);
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
__s32 SDMMC_GetDiskSize(void)
{
	return card_info->volume;
}
/*******************************************************************************
*								SD_Read
*
* Description : read some blocks data from sd card.

* Arguments	  : block  -- the start block number

*			  	nblock -- blocks len

*				*buf   -- data buffer

* Returns	  :	0 = success,-1 = fail.

* Notes		  : none
********************************************************************************/
__s32 SDMMC_Read(__u32 block,__u32 nblock, void *buf, __u32 card_no)
{
	SDC_CMD_T cmd;

	memset(&cmd,0,sizeof(SDC_CMD_T));
	cmd.type = ADTC;
	cmd.check_response_crc = 1;
	cmd.response_len = 0;
	cmd.transfer_dir = 0;
	cmd.transfer_mode = 0;

	cmd.mode = SD_PROPRIETARY_CMD;
	if(nblock == 1)
	{
		cmd.index = READ_SINGLE_BLOCK;
	}
	else
	{
		cmd.index = READ_MULTIPLE_BLOCK;
		cmd.auto_stop = 1;
	}

	if ((card_info->type) & HIGH_CAP)
		cmd.arg = block;
	else
		cmd.arg = block * 512;

	if (0 != SDC_ReadData(&cmd, buf, nblock*512, card_no))
	//if (0 != SDC_ReadDataExt1(&cmd, buf, nblock*512, card_no))
	{
		return -1;
	}

	return 0;
}
/*******************************************************************************
*								SD_Read
*
* Description : read some blocks data from sd card.

* Arguments	  : block  -- the start block number

*			  	nblock -- blocks len

*				*buf   -- data buffer

* Returns	  :	0 = success,-1 = fail.

* Notes		  : none
********************************************************************************/
__s32 SDMMC_Write(__u32 block,__u32 nblock, void *buf, __u32 card_no)
{
	SDC_CMD_T cmd;
	__u8  response[128];

	memset(&cmd,0,sizeof(SDC_CMD_T));
	cmd.type = ADTC;
	cmd.check_response_crc = 1;
	cmd.response_len = 0;
	cmd.transfer_dir = 1;
	cmd.transfer_mode = 0;

	cmd.mode = SD_PROPRIETARY_CMD;
	if(nblock == 1)
	{
		cmd.index = WRITE_SINGLE_BLOCK;
	}
	else
	{
		cmd.index = WRITE_MULTIPLE_BLOCK;
		cmd.auto_stop = 1;
	}

	if ((card_info->type) & HIGH_CAP)
		cmd.arg = block;
	else
		cmd.arg = block * 512;

	if (0 != SDC_WriteData(&cmd, buf, nblock*512, card_no))
	{
		return -1;
	}

	while(1)
	{
		if (_get_card_status(response))
		{
			return -1;
		}
		if (*((__u32 *)response) & (0x100))
			break;
	}

	return 0;
}
/*******************************************************************************
*								SD_SetSampleEdge
*
* Description :

* Arguments	  :

*

*

* Returns	  :

* Notes		  : none
********************************************************************************/
__s32 SD_SetSampleEdge(__u32 sample_edge)
{
    sd_reg->sd_reg_ctrl &= ~(1 << 9);
    sd_reg->sd_reg_ctrl |=  (sample_edge << 9);

    return 0;
}
