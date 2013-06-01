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
#ifndef	_WBOOT_DMA_H_
#define	_WBOOT_DMA_H_

typedef enum
{
    NDMA_IR                      = 0,
    NDMA_SPDIF                      ,
    NDMA_IIS                        ,
    NDMA_AC97                       ,
    NDMA_SPI0                       ,
    NDMA_SPI1                       ,
    NDMA_UART0                   = 8,
    NDMA_UART1                      ,
    NDMA_UART2                      ,
    NDMA_UART3                      ,
    NDMA_AUDIO                      ,
    NDMA_TP                         ,
    NDMA_SRAM                   = 16,
    NDMA_SDRAM                      ,
    NDMA_
}
__ndma_drq_type_t;

typedef enum
{
    DDMA_SRC_SRAM                   = 0,
    DDMA_SRC_SDRAM                     ,
    DDMA_SRC_LCD                       ,
    DDMA_SRC_NAND                      ,
    DDMA_SRC_USB0                      ,
    DDMA_SRC_USB1                      ,
    DDMA_SRC_SD0                       ,
    DDMA_SRC_SD1                       ,
    DDMA_SRC_SD2                       ,
    DDMA_SRC_SD3                       ,
    DDMA_SRC_MS                        ,
    DDMA_SRC_EMAC                   = 0xc,
    DDMA_SRC_SS                     = 0xe,
    DDMA_SRC_
}
__ddma_src_type_t;

typedef enum
{
    DDMA_DST_SRAM                   = 0,
    DDMA_DST_SDRAM                     ,
    DDMA_DST_LCD                       ,
    DDMA_DST_NAND                      ,
    DDMA_DST_USB0                      ,
    DDMA_DST_USB1                      ,
    DDMA_DST_SD0                       ,
    DDMA_DST_SD1                       ,
    DDMA_DST_SD2                       ,
    DDMA_DST_SD3                       ,
    DDMA_DST_MS                        ,
    DDMA_DST_EMAC                   = 0xb,
    DDMA_DST_SS                     = 0xd,
    DDMA_DST_
}
__ddma_dst_type_t;

typedef struct  __dma_config_set
{
    unsigned int      src_drq_type     ; //源地址存储类型，如DRAM, SPI,NAND等，根据选择NDMA或者DDMA, 选择 __ndma_drq_type_t或者 __ddma_src_type_t
    unsigned int      src_addr_type    ; //原地址类型 NDMA下 0:递增模式  1:保持不变  DDMA下 0:递增模式  1:保持不变  2:H模式  3:V模式
    unsigned int      src_burst_length ; //发起一次burst宽度 填0对应于1，填1对应于4,
    unsigned int      src_data_width   ; //数据传输宽度，0:一次传输8bit，1:一次传输16bit，2:一次传输32bit，3:保留
    unsigned int      dst_drq_type     ; //源地址存储类型，如DRAM, SPI,NAND等，根据选择NDMA或者DDMA, 选择 __ndma_drq_type_t或者 __ddma_dst_type_t
    unsigned int      dst_addr_type    ; //原地址类型 NDMA下 0:递增模式  1:保持不变  DDMA下 0:递增模式  1:保持不变  2:H模式  3:V模式
    unsigned int      dst_burst_length ; //发起一次burst宽度 填0对应于1，填1对应于4,
    unsigned int      dst_data_width   ; //数据传输宽度，0:一次传输8bit，1:一次传输16bit，2:一次传输32bit，3:保留
    unsigned int      wait_state       ; //等待时钟个数 选择范围从0-7，只对NDMA有效
    unsigned int      continuous_mode  ; //选择连续工作模式 0:传输一次即结束 1:反复传输，当一次DMA传输结束后，重新开始传输
}
__dma_config_t;

typedef struct 	__dma_setting_set
{
    __dma_config_t         cfg;	    	    //DMA配置参数
    unsigned int           pgsz;            //DE使用参数，块拷贝使用
    unsigned int           pgstp;           //DE使用参数，块拷贝使用
    unsigned int           cmt_blk_cnt;     //DE使用参数，块拷贝使用
}__dma_setting_t;

typedef struct  CSP_dma_config
{
    unsigned int      src_drq_type     ; //源地址存储类型，如DRAM, SPI,NAND等，根据选择NDMA或者DDMA, 选择 __ndma_drq_type_t或者 __ddma_src_type_t
    unsigned int      src_addr_type    ; //原地址类型 NDMA下 0:递增模式  1:保持不变  DDMA下 0:递增模式  1:保持不变  2:H模式  3:V模式
    unsigned int      src_burst_length ; //发起一次burst宽度 填0对应于1，填1对应于4,
    unsigned int      src_data_width   ; //数据传输宽度，0:一次传输8bit，1:一次传输16bit，2:一次传输32bit，3:保留
    unsigned int      dst_drq_type     ; //源地址存储类型，如DRAM, SPI,NAND等，根据选择NDMA或者DDMA, 选择 __ndma_drq_type_t或者 __ddma_dst_type_t
    unsigned int      dst_addr_type    ; //原地址类型 NDMA下 0:递增模式  1:保持不变  DDMA下 0:递增模式  1:保持不变  2:H模式  3:V模式
    unsigned int      dst_burst_length ; //发起一次burst宽度 填0对应于1，填1对应于4,
    unsigned int      dst_data_width   ; //数据传输宽度，0:一次传输8bit，1:一次传输16bit，2:一次传输32bit，3:保留
    unsigned int      wait_state       ; //等待时钟个数 选择范围从0-7，只对NDMA有效
    unsigned int      continuous_mode  ; //选择连续工作模式 0:传输一次即结束 1:反复传输，当一次DMA传输结束后，重新开始传输

    unsigned int      cmt_blk_cnt	   ; //DMA传输comity counter
}CSP_dma_config_t;

#endif	//_WBOOT_DMA_H_

/* end of _WBOOT_DMA_H_ */

