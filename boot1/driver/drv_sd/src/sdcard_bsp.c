/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, Allwinner Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : init_sdcard.c
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
#include  "sdcard_bsp.h"

extern  CARD_INFO_T             card_info[1];
extern  __register_grout_t     *sd_reg;
extern  smc_idma_des 		   *pdes;


static __s32 _update_clock_register(__u32 cmd, __register_grout_t  *sd_reg)
{
	__s32 timeout = 0xffff;

	sd_reg->sd_reg_cmd = cmd;
	while(timeout--)
	{
		if(!(((__u32)1 << 31) & (sd_reg->sd_reg_cmd)))
		{
		    return 0;
		}

		if((1<< 12) & (sd_reg->sd_reg_rintsts))
		{
			sd_reg->sd_reg_rintsts = 1<<12;
			return -1;
		}
	}

	if(timeout < 0)
		return -1;

	return 0;
}

__s32 SDC_Init(void)
{
	__u32 cfg, i;
	__s32 time;

	/*reset SDC/FIFO/DMA*/
	time = 0xffff;
	cfg = 0x7;
	sd_reg->sd_reg_ctrl = cfg;
	while(cfg & 0x7)
	{
	    time --;
	    if(time < 0)
	    {
	        return -1;
	    }
	    cfg = sd_reg->sd_reg_ctrl;
	}
	for(i=0;i<50;i++);
	//clear all int status
	cfg = sd_reg->sd_reg_rintsts;
	cfg |= 0xFFFF;
	sd_reg->sd_reg_rintsts = cfg;
	//上升沿读数据
    cfg = (1 << 9);
	sd_reg->sd_reg_ctrl = cfg;
	cfg = 0x70008;
	sd_reg->sd_reg_fifoth = cfg;

	return 0;
}

__s32 SDC_Exit(__s32 gpio_no)
{
    return 0;
}

/*******************************************************************************
*								SDC_ConfigCardHost
*
* Description : configure card register with a typical driver stage setting

* Arguments	  : *card_info	-- point to the structure include card block size、timing、card type and so on。

*				Card_no     -- card number, 0 or 1

* Returns	  :	0 = success,-1 = fail.

* Notes		  : none
********************************************************************************/
__s32 SDC_ConfigCardHost(SDC_CARD_CONFIG_INFO_T *card_cfg_info)
{
	__u32 cfg, reg_val;

	/*set timeout*/
	cfg = (((card_cfg_info->data_timeout)&0xffffff) << 8 |(card_cfg_info->response_timeout)&0xff);
	sd_reg->sd_reg_tmout = cfg;

	/*set blk size = 0x200 */
	cfg = (card_cfg_info->block_size) & 0xffff;
	sd_reg->sd_reg_blksiz = cfg;

	/*set bus_width */
	sd_reg->sd_reg_buswidth = card_cfg_info->bus_width>>2;
	/*disable card clock*/
    sd_reg->sd_reg_clkctrl &= ~(1 << 16);
	cfg = (1<<21)|(1<<13)|(__u32)1<<31;
	if(_update_clock_register(cfg, sd_reg))
		return -1;

	/*change clock divide*/
	reg_val = sd_reg->sd_reg_clkctrl;
	reg_val &= ~0xff;
	reg_val |= card_cfg_info->clock_div;
	sd_reg->sd_reg_clkctrl = reg_val;
	cfg = (1<<21)|(1<<13)|(__u32)1<<31;
	if(_update_clock_register(cfg, sd_reg))
		return -1;

	/*enable clock*/
	sd_reg->sd_reg_clkctrl |= (((card_cfg_info->power_mode & 0x01 ) << 17) | (1 << 16));
	cfg = (1<<21)|(1<<13)|(__u32)1<<31;
	if(_update_clock_register(cfg, sd_reg))
	{
		return -1;
    }

	return 0;
}

/*******************************************************************************
*								SDC_SendCmd
*
* Description : host(SDC) send cmd to card

* Arguments	  : *cmd	   -- point to the structure of card cmd info with index、type
*							  mode、data transfer direction and so on

*				*response  -- point to the response value if cmd needs

*				card_no	   -- card number, 0 or 1

* Returns	  :	0 = success,-1 = fail.

* Notes		  : none
********************************************************************************/
__s32 SDC_SendCmd(SDC_CMD_T *cmd, void*response)
{
	__u32 cfg;
	__s32 timeout;

	/*set command argument*/
	sd_reg->sd_reg_cmdarg = cmd->arg;

	/*set cmd*/
	cfg = (cmd->index & 0x3f);
	cfg |= cmd->send_initialization <<15;
	cfg |= cmd->auto_stop << 12;
	/*response expect*/
	if (cmd->type != BC)
	{
		cfg |= (0x1 << 6);
		cfg |= (cmd->response_len << 7);
		cfg |= (cmd->check_response_crc << 8);
	}
	/*transfer data expect*/
	if(cmd->type == ADTC)
	{
		cfg |= ((1 << 9)|(1<<13));
		cfg |= (cmd->transfer_dir << 10);
		cfg |= (cmd->transfer_mode << 11);
	}
	/*other set*/
	cfg |= ((__u32)1 << 31);

	sd_reg->sd_reg_cmd = cfg;

	timeout = 0xffff;
	while((timeout--)&&(!(sd_reg->sd_reg_rintsts & 0x04)));
	sd_reg->sd_reg_rintsts = 0x01 << 2;
	if(timeout < 0)
	{
		return -1;
    }

    cfg = sd_reg->sd_reg_rintsts;

	/*need response*/
	if(cmd->type != BC)
	{
		if (cfg & 0x102)
		{
		    /*response timeout or response error*/
			sd_reg->sd_reg_rintsts = 0x102;
			return -1;
		}

		if(cmd->check_response_crc)
		{
			if(cfg & (0x1 << 6))
			{
			    /*check crc err*/
			    sd_reg->sd_reg_rintsts = 0x01 << 6;
				return -1;
			}
		}

		/*get 128 bytes response value*/
		*((__u32 *)response + 0) = sd_reg->sd_reg_resp0;
		if (cmd->response_len == 1)
		{
			*((__u32 *)response + 1) = sd_reg->sd_reg_resp1;
			*((__u32 *)response + 2) = sd_reg->sd_reg_resp2;
			*((__u32 *)response + 3) = sd_reg->sd_reg_resp3;
		}
	}

	return 0;
}

/*******************************************************************************
*								SDC_ReadData
*
* Description : host(SDC) send read cmd(single or multiple) to card and get data

* Arguments	  : *cmd		-- point to the structure of card cmd info with index、type
*							  mode、data transfer direction and so on

*				*buf  		-- data buffer

*				byte_cnt 	-- data byte count

*				access_mode -- transfer data by DMA(1) or AHB(0)

*				card_no		-- card number

* Returns	  :	0 = success,-1 = fail.

* Notes		  : none
********************************************************************************/
#define SDXC_DES_NUM_SHIFT            (13)  //8192 == 1<<13;
#define SDXC_DES_BUFFER_MAX_LEN       (1 << SDXC_DES_NUM_SHIFT)

//static __u32 printk = 0;
//#define my_printk(...)			{if (printk) (wlibc_uprintf(__VA_ARGS__));}

#define my_printk(...)

__s32 SDC_ReadData(SDC_CMD_T *cmd, void *buf, __u32 byte_cnt, __u32 card_no)
{
	__u32 cfg;
	__s32 timeout, ret = -1;
	__u8  response[128];
	__u32 des_idx = 0;
    __u32 buff_frag_num = 0;
    __u32 remain;

    card_no &= 0x03;
    buff_frag_num = byte_cnt >> SDXC_DES_NUM_SHIFT;
    remain = byte_cnt & ((1 << SDXC_DES_NUM_SHIFT) - 1);
    if (remain)
    {
        buff_frag_num++;
    }
    else
    {
        remain = SDXC_DES_BUFFER_MAX_LEN;
    }

	my_printk("rd, byte_cnt %d, frag num %d, remain %d\n", byte_cnt, buff_frag_num, remain);
	memset((void*)pdes, 0, sizeof(smc_idma_des) * buff_frag_num);

	/*set byte count*/
    sd_reg->sd_reg_bytcnt	= byte_cnt;
    sd_reg->sd_reg_blksiz   = 512;

	//设置dma
	sd_reg->sd_reg_ctrl     &= ~(1U << 31);             // switch dma transfer mode
    sd_reg->sd_reg_ctrl     |= (1U << 5);               // enable mda
    sd_reg->sd_reg_idmac    |= 1;                       // reset idma
    sd_reg->sd_reg_idmac    |= (1U << 1) | (1U << 7);   // idma on
    sd_reg->sd_reg_idmaie   |= (1U << 1);               // enable idma over interrupt flag
    //config idma descriptor
    for (des_idx=0; des_idx<buff_frag_num; des_idx++)
    {
        pdes[des_idx].des_chain = 1;
        pdes[des_idx].own = 1;
        pdes[des_idx].dic = 1;
        if (buff_frag_num > 1 && des_idx != buff_frag_num-1)
        {
            pdes[des_idx].data_buf1_sz = SDXC_DES_BUFFER_MAX_LEN;
        }
        else
        {
            pdes[des_idx].data_buf1_sz = remain;
        }

        pdes[des_idx].buf_addr_ptr1 = (__u32)buf + des_idx * SDXC_DES_BUFFER_MAX_LEN;
        if (des_idx == 0)
        {
            pdes[des_idx].first_des = 1;
        }

        if (des_idx == buff_frag_num-1)
        {
            pdes[des_idx].dic = 0;
            pdes[des_idx].last_des = 1;
            pdes[des_idx].end_of_ring = 1;
            pdes[des_idx].buf_addr_ptr2 = 0;
        }
        else
        {
            pdes[des_idx].buf_addr_ptr2 = (__u32)(&pdes[des_idx+1]);
        }
        my_printk("pdes[%d](%x), [0] = %x, [1] = %x, [2] = %x, [3] = %x\n",
        			des_idx, (__u32)&pdes[des_idx],
        			((__u32*)&pdes[des_idx])[0],((__u32*)&pdes[des_idx])[1],
        			((__u32*)&pdes[des_idx])[2],((__u32*)&pdes[des_idx])[3]);
    }

    //flush cache for idma descriptor and data buffer
    wlibc_CleanFlushDCacheRegion(pdes, sizeof(smc_idma_des) * buff_frag_num);
    wlibc_CleanFlushDCacheRegion(buf,  byte_cnt);

    sd_reg->sd_reg_desba = (__u32)pdes;

	/*send read cmd*/
	if (SDC_SendCmd(cmd, response))
	{
		//disable card
		sd_reg->sd_reg_bytcnt = 512;

		goto _read_err_;
	}

	/*wait data transfer over*/
	timeout = 0x600*byte_cnt;
	while((timeout--)&&(!(sd_reg->sd_reg_rintsts & (0x1<<3))));
	sd_reg->sd_reg_rintsts = 0x1<<3;
	if(timeout < 0)
	{
		//如果数据传输超时 退出
		my_printk(" wait data transfer over timeout\n");
		goto _read_err_;
    }

    //wait idma over
    timeout = 0x7ffff;
    while((timeout--)&&(!(sd_reg->sd_reg_idmast & ((0x1<<1) | (0x1<<8)))));
    sd_reg->sd_reg_idmast = (0x1<<1) | (0x1<<8);
    if(timeout < 0)
	{
		//如果数据传输超时 退出
		goto _read_err_;
    }

	/*check data transfer error*/
	cfg = sd_reg->sd_reg_rintsts;
	if (cfg & ((1 << 7)| (1 << 9)))
	{
		sd_reg->sd_reg_rintsts = (1<<7)|(1<<9);

		goto _read_err_;
	}
	ret = 0;

    sd_reg->sd_reg_idmast = 0x337;  //clear idma flags
    sd_reg->sd_reg_idmaie = 0;
    sd_reg->sd_reg_ctrl   &= ~(1U << 5);
    sd_reg->sd_reg_idmac  = 0;

_read_err_:

	sd_reg->sd_reg_ctrl |=  (1 << 2) | (1 << 1);              //reset dma and fifo
    sd_reg->sd_reg_rintsts |= 0x1ffff;

	return ret;
}
/*******************************************************************************
*								SDC_ReadDataExt
*
* Description : host(SDC) send read cmd(single or multiple) to card and get data

* Arguments	  : *cmd		-- point to the structure of card cmd info with index、type
*							  mode、data transfer direction and so on

*				*buf  		-- data buffer

*				byte_cnt 	-- data byte count

*				access_mode -- transfer data by DMA(1) or AHB(0)

*				card_no		-- card number

* Returns	  :	0 = success,-1 = fail.

* Notes		  : none
********************************************************************************/

__s32 SDC_ReadDataExt(SDC_CMD_T *cmd, void *buf, __u32 byte_cnt, __u32 card_no)
{
	__u32 cfg, i;
	__s32 timeout, ret = -1;
	__u8  response[128];
	__u32 temp;

    card_no &= 0x03;

	//设置dma方式
	sd_reg->sd_reg_ctrl     |= (1U << 31);             // switch ahb transfer mode

	/*set byte count*/
	sd_reg->sd_reg_bytcnt = byte_cnt;
	/*set byte count*/
	sd_reg->sd_reg_blksiz = byte_cnt;
	/*send read cmd*/
	if (SDC_SendCmd(cmd, response))
	{
		//disable card
		goto _readex_err_;
	}

	//配置dma的config参数
	for(i = 0; i < (byte_cnt >> 2); i++)
	{

		timeout = 0xffffffff;
		temp = sd_reg->sd_reg_status;
		while( temp & 0x04 )
		{
			if(0 == timeout--)
			{
				goto _readex_err_;
			}

			temp = sd_reg->sd_reg_status;
		}

 	 	((__u32 *)buf)[i] = *((volatile unsigned int *)(0x01c0f000 + card_no * 0x1000 + SD_REG_o_FIFO));
	}
	sd_reg->sd_reg_blksiz = 512;
	/*wait data transfer over*/
    timeout = 0x7ffff;
	while((timeout--)&&(!(sd_reg->sd_reg_rintsts & (0x1<<3))));
	sd_reg->sd_reg_rintsts = 0x1<<3;
	if(timeout < 0)
	{
		//如果数据传输超时 退出
		goto _readex_err_;
    }

	/*check data transfer error*/
	cfg = sd_reg->sd_reg_rintsts;
	if (cfg & ((1 << 7)| (1 << 9)))
	{
		sd_reg->sd_reg_rintsts = (1<<7)|(1<<9);

		goto _readex_err_;
	}
	ret = 0;

_readex_err_:

	sd_reg->sd_reg_ctrl     &= ~(1U << 31);
	sd_reg->sd_reg_blksiz = 512;
	sd_reg->sd_reg_ctrl |=  (1 << 2);

	return ret;
}
/*******************************************************************************
*								SDC_WriteData
*
* Description : host(SDC) send read cmd(single or multiple) to card and get data

* Arguments	  : *cmd		-- point to the structure of card cmd info with index、type
*							  mode、data transfer direction and so on

*				*buf  		-- data buffer

*				byte_cnt 	-- data byte count

*				access_mode -- transfer data by DMA(1) or AHB(0)

*				card_no		-- card number

* Returns	  :	0 = success,-1 = fail.

* Notes		  : none
********************************************************************************/

__s32 SDC_WriteData(SDC_CMD_T *cmd, void *buf, __u32 byte_cnt, __u32 card_no)
{
	__u32 cfg;
	__s32 timeout, ret = -1;
	__u8  response[128];
	__u32 des_idx = 0;
    __u32 buff_frag_num = 0;
    __u32 remain;

    card_no &= 0x03;
    buff_frag_num = byte_cnt >> SDXC_DES_NUM_SHIFT;
    remain = byte_cnt & ((1 << SDXC_DES_NUM_SHIFT) - 1);
    if (remain)
    {
        buff_frag_num++;
    }
    else
    {
        remain = SDXC_DES_BUFFER_MAX_LEN;
    }

	my_printk("wr, byte_cnt %d, frag num %d, remain %d\n", byte_cnt, buff_frag_num, remain);
	memset((void*)pdes, 0, sizeof(smc_idma_des) * buff_frag_num);

	/*set byte count*/
    sd_reg->sd_reg_bytcnt	= byte_cnt;
    sd_reg->sd_reg_blksiz   = 512;

	//设置dma
	sd_reg->sd_reg_ctrl     &= ~(1U << 31);             // switch dma transfer mode
    sd_reg->sd_reg_ctrl     |= (1U << 5);               // enable mda
    sd_reg->sd_reg_idmac    |= 1;                       // reset idma
    sd_reg->sd_reg_idmac    |= (1U << 1) | (1U << 7);   // idma on
    sd_reg->sd_reg_idmaie   |= (1U << 0);               // enable idma over interrupt flag
    //config idma descriptor
    for (des_idx=0; des_idx<buff_frag_num; des_idx++)
    {
        pdes[des_idx].des_chain = 1;
        pdes[des_idx].own = 1;
        pdes[des_idx].dic = 1;
        if (buff_frag_num > 1 && des_idx != buff_frag_num-1)
        {
            pdes[des_idx].data_buf1_sz = SDXC_DES_BUFFER_MAX_LEN;
        }
        else
        {
            pdes[des_idx].data_buf1_sz = remain;
        }

        pdes[des_idx].buf_addr_ptr1 = (__u32)buf + des_idx * SDXC_DES_BUFFER_MAX_LEN;
        if (des_idx == 0)
        {
            pdes[des_idx].first_des = 1;
        }

        if (des_idx == buff_frag_num-1)
        {
            pdes[des_idx].dic = 0;
            pdes[des_idx].last_des = 1;
            pdes[des_idx].end_of_ring = 1;
            pdes[des_idx].buf_addr_ptr2 = 0;
        }
        else
        {
            pdes[des_idx].buf_addr_ptr2 = (__u32)(&pdes[des_idx+1]);
        }

        my_printk("pdes[%d](%x), [0] = %x, [1] = %x, [2] = %x, [3] = %x\n",
        			des_idx, (__u32)&pdes[des_idx],
        			((__u32*)&pdes[des_idx])[0],((__u32*)&pdes[des_idx])[1],
        			((__u32*)&pdes[des_idx])[2],((__u32*)&pdes[des_idx])[3]);
    }

    //flush cache for idma descriptor and data buffer
    wlibc_CleanFlushDCacheRegion(pdes, sizeof(smc_idma_des) * buff_frag_num);
    wlibc_CleanFlushDCacheRegion(buf,  byte_cnt);

    sd_reg->sd_reg_desba = (__u32)pdes;

	/*send read cmd*/
	if (SDC_SendCmd(cmd, response))
	{
		//disable card
		sd_reg->sd_reg_bytcnt = 512;

		goto _read_err_;
	}

	/*wait data transfer over*/
	timeout = 0x7ffff;
	while((timeout--)&&(!(sd_reg->sd_reg_rintsts & (0x1<<3))));
	sd_reg->sd_reg_rintsts = 0x1<<3;
	if(timeout < 0)
	{
		//如果数据传输超时 退出
		goto _read_err_;
    }

    //wait idma over
    timeout = 0x7ffff;
    while((timeout--)&&(!(sd_reg->sd_reg_idmast & ((0x1<<0) | (0x1<<8)))));
    sd_reg->sd_reg_idmast = (0x1<<1) | (0x1<<8);
    if(timeout < 0)
	{
		//如果数据传输超时 退出
		goto _read_err_;
    }

	/*check data transfer error*/
	cfg = sd_reg->sd_reg_rintsts;
	if (cfg & ((1 << 7)| (1 << 9)))
	{
		sd_reg->sd_reg_rintsts = (1<<7)|(1<<9);

		goto _read_err_;
	}
	ret = 0;

    sd_reg->sd_reg_idmast = 0x337;  //clear idma flags
    sd_reg->sd_reg_idmaie = 0;
    sd_reg->sd_reg_ctrl   &= ~(1U << 5);
    sd_reg->sd_reg_idmac  = 0;

_read_err_:

	sd_reg->sd_reg_ctrl |=  (1 << 2) | (1 << 1);              //reset dma and fifo
    sd_reg->sd_reg_rintsts |= 0x1ffff;

	return ret;
}

