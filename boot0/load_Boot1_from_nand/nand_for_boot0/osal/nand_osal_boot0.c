/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : nand_osal_boot0.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include "egon2.h"


#define PIOC_REGS_BASE 		0x01c20800
__hdle   nand_hDMA;
struct lib_dma
{
    volatile unsigned int config;           /* DMA配置参数              */
    volatile unsigned int src_addr;         /* DMA传输源地址            */
    volatile unsigned int dst_addr;         /* DMA传输目的地址          */
    volatile unsigned int bytes;            /* DMA传输字节数            */
    volatile unsigned int page_size;        /* DMA传输PAGE SIZE         */
    volatile unsigned int page_step;        /* DMA传输PAGE STEP         */
    volatile unsigned int comity_counter;   /* DMA传输comity counter    */
};

typedef volatile struct lib_dma *lib_dma_t;


lib_dma_t  DMAChannel_D = (lib_dma_t)( 0x01c02000 + 0x300);

/*
****************************************************************************************************
*
*             OSAL_DmaRequest
*
*  Description:
*       request dma
*
*  Parameters:
*		type	0: normal timer
*				1: special timer
*  Return value:
*		dma handler
*		if 0, fail
****************************************************************************************************
*/
int NAND_RequestDMA(void)
{
    return (__hdle)DMAChannel_D;
}
/*
****************************************************************************************************
*
*             OSAL_DmaRelease
*
*  Description:
*       release dma
*
*  Parameters:
*       hDma	dma handler
*
*  Return value:
*		EPDK_OK/FAIL
****************************************************************************************************
*/
__s32 NAND_ReleaseDMA(void)
{
    /* stop dma                 */
    DMAChannel_D->config = 0;   /* stop dma                                           */

    return 0;
}
/*
****************************************************************************************************
*
*             OSAL_DmaConfig
*
*  Description:
*       start interrupt
*
*  Parameters:
*       hTmr	timer handler
*		pArg    *(pArg + 0)         ctrl
*               *(pArg + 1)         page size
*               *(pArg + 2)         page step
*               *(pArg + 3)         comity & block count
*
*  Return value:
*		EPDK_OK/FAIL
*
**********************************************************************************************************************
*/
/* DMA 基础配置  */
#define  CSP_DMAC_DMATYPE_NORMAL         			0
#define  CSP_DMAC_DMATYPE_DEDICATED      			1

#define CSP_DMAC_CFG_CONTINUOUS_ENABLE              (0x01)	//(0x01<<29)
#define CSP_DMAC_CFG_CONTINUOUS_DISABLE             (0x00)	//(0x01<<29)

//* DMA 等待时钟 */
#define	CSP_DMAC_CFG_WAIT_1_DMA_CLOCK				(0x00)	//(0x00<<26)
#define	CSP_DMAC_CFG_WAIT_2_DMA_CLOCK				(0x01)	//(0x01<<26)
#define	CSP_DMAC_CFG_WAIT_3_DMA_CLOCK				(0x02)	//(0x02<<26)
#define	CSP_DMAC_CFG_WAIT_4_DMA_CLOCK				(0x03)	//(0x03<<26)
#define	CSP_DMAC_CFG_WAIT_5_DMA_CLOCK				(0x04)	//(0x04<<26)
#define	CSP_DMAC_CFG_WAIT_6_DMA_CLOCK				(0x05)	//(0x05<<26)
#define	CSP_DMAC_CFG_WAIT_7_DMA_CLOCK				(0x06)	//(0x06<<26)
#define	CSP_DMAC_CFG_WAIT_8_DMA_CLOCK				(0x07)	//(0x07<<26)

/* DMA 传输源端 配置 */
/* DMA 目的端 传输宽度 */
#define	CSP_DMAC_CFG_DEST_DATA_WIDTH_8BIT			(0x00)	//(0x00<<24)
#define	CSP_DMAC_CFG_DEST_DATA_WIDTH_16BIT			(0x01)	//(0x01<<24)
#define	CSP_DMAC_CFG_DEST_DATA_WIDTH_32BIT			(0x02)	//(0x02<<24)

/* DMA 目的端 突发传输模式 */
#define	CSP_DMAC_CFG_DEST_1_BURST       			(0x00)	//(0x00<<23)
#define	CSP_DMAC_CFG_DEST_4_BURST		    		(0x01)	//(0x01<<23)

/* DMA 目的端 地址变化模式 */
#define	CSP_DMAC_CFG_DEST_ADDR_TYPE_LINEAR_MODE		(0x00)	//(0x00<<21)
#define	CSP_DMAC_CFG_DEST_ADDR_TYPE_IO_MODE 		(0x01)	//(0x01<<21)
#define	CSP_DMAC_CFG_DEST_ADDR_TYPE_HPAGE_MODE 		(0x02)	//(0x02<<21)
#define	CSP_DMAC_CFG_DEST_ADDR_TYPE_VPAGE_MODE 		(0x03)	//(0x03<<21)


/* DMA 传输源端 配置 */
/* DMA 源端 传输宽度 */
#define	CSP_DMAC_CFG_SRC_DATA_WIDTH_8BIT			(0x00)	//(0x00<<8)
#define	CSP_DMAC_CFG_SRC_DATA_WIDTH_16BIT			(0x01)	//(0x01<<8)
#define	CSP_DMAC_CFG_SRC_DATA_WIDTH_32BIT			(0x02)	//(0x02<<8)

/* DMA 源端 突发传输模式 */
#define	CSP_DMAC_CFG_SRC_1_BURST       				(0x00)	//(0x00<<7)
#define	CSP_DMAC_CFG_SRC_4_BURST		    		(0x01)	//(0x01<<7)

/* DMA 源端 地址变化模式 */
#define	CSP_DMAC_CFG_SRC_ADDR_TYPE_LINEAR_MODE		(0x00)	//(0x00<<5)
#define	CSP_DMAC_CFG_SRC_ADDR_TYPE_IO_MODE 			(0x01)	//(0x01<<5)
#define	CSP_DMAC_CFG_SRC_ADDR_TYPE_HPAGE_MODE 		(0x02)	//(0x02<<5)
#define	CSP_DMAC_CFG_SRC_ADDR_TYPE_VPAGE_MODE 		(0x03)	//(0x03<<5)

/* DMA 传输目的端 D型DMA 目的选择 */
#define	CSP_DMAC_CFG_DEST_TYPE_D_SRAM 				(0x00)	//(0x00<<16)
#define	CSP_DMAC_CFG_DEST_TYPE_D_SDRAM				(0x01)	//(0x01<<16)
#define	CSP_DMAC_CFG_DEST_TYPE_TCON0				(0x02)	//(0x02<<16)
#define	CSP_DMAC_CFG_DEST_TYPE_NFC  		    	(0x03)	//(0x03<<16)

/* DMA 传输源端 D型DMA 目的选择 */
#define	CSP_DMAC_CFG_SRC_TYPE_D_SRAM 				(0x00)	//(0x00<<0)
#define	CSP_DMAC_CFG_SRC_TYPE_D_SDRAM				(0x01)	//(0x01<<0)
#define	CSP_DMAC_CFG_SRC_TYPE_TCON0				    (0x02)	//(0x02<<0)
#define	CSP_DMAC_CFG_SRC_TYPE_NFC  		    	   	(0x03)	//(0x03<<0)

__s32 NAND_DMAConfigStart(int rw, unsigned int buff_addr, int len)
{
    CSP_dma_config_t dma_param;
	CSP_dma_config_t    * pDma = &dma_param;
    __u32  dma_cfg;

	if(rw)  //write
	{
		if(buff_addr & 0xC0000000) //DRAM
			dma_param.src_drq_type = CSP_DMAC_CFG_SRC_TYPE_D_SDRAM;
		else  //SRAM
			dma_param.src_drq_type = CSP_DMAC_CFG_SRC_TYPE_D_SRAM;
		dma_param.src_addr_type = CSP_DMAC_CFG_SRC_ADDR_TYPE_LINEAR_MODE;  //linemode
		dma_param.src_burst_length = CSP_DMAC_CFG_SRC_4_BURST;  //burst mode
		dma_param.src_data_width = CSP_DMAC_CFG_SRC_DATA_WIDTH_32BIT;  //32bit

		dma_param.dst_drq_type = CSP_DMAC_CFG_DEST_TYPE_NFC;
		dma_param.dst_addr_type = CSP_DMAC_CFG_DEST_ADDR_TYPE_IO_MODE; //IO mode
		dma_param.dst_burst_length = CSP_DMAC_CFG_DEST_4_BURST; // burst mode
		dma_param.dst_data_width = CSP_DMAC_CFG_DEST_DATA_WIDTH_32BIT; //32 bit

		dma_param.wait_state = CSP_DMAC_CFG_WAIT_1_DMA_CLOCK; // invalid value
		dma_param.continuous_mode = CSP_DMAC_CFG_CONTINUOUS_DISABLE; //no continous

		dma_param.cmt_blk_cnt =  0x7f077f07; //commit register

	}
	else //read
	{
		dma_param.src_drq_type = CSP_DMAC_CFG_SRC_TYPE_NFC;
		dma_param.src_addr_type = CSP_DMAC_CFG_SRC_ADDR_TYPE_IO_MODE;  //IO mode
		dma_param.src_burst_length = CSP_DMAC_CFG_SRC_4_BURST;  //burst mode
		dma_param.src_data_width = CSP_DMAC_CFG_SRC_DATA_WIDTH_32BIT;  //32bit

		if(buff_addr & 0xC0000000) //DRAM
			dma_param.dst_drq_type = CSP_DMAC_CFG_DEST_TYPE_D_SDRAM;
		else  //SRAM
			dma_param.dst_drq_type = CSP_DMAC_CFG_DEST_TYPE_D_SRAM;
		dma_param.dst_addr_type = CSP_DMAC_CFG_DEST_ADDR_TYPE_LINEAR_MODE; //line mode
		dma_param.dst_burst_length = CSP_DMAC_CFG_DEST_4_BURST; // burst mode
		dma_param.dst_data_width = CSP_DMAC_CFG_DEST_DATA_WIDTH_32BIT; //32 bit

		dma_param.wait_state = CSP_DMAC_CFG_WAIT_1_DMA_CLOCK; // invalid value
		dma_param.continuous_mode = CSP_DMAC_CFG_CONTINUOUS_DISABLE; //no continous

		dma_param.cmt_blk_cnt =  0x7f077f07; //commit register
	}


    dma_cfg = (pDma->src_drq_type     << 0)     |     \
              (pDma->src_addr_type    << 5)     |     \
              (pDma->src_burst_length << 7)     |     \
              (pDma->src_data_width   << 9)     |     \
              (pDma->dst_drq_type     << 16)    |     \
              (pDma->dst_addr_type    << 21)    |     \
              (pDma->dst_burst_length << 23)    |     \
              (pDma->dst_data_width   << 25)    |     \
              (pDma->continuous_mode  << 29);

    DMAChannel_D->config         = dma_cfg;
    DMAChannel_D->comity_counter = pDma->cmt_blk_cnt;

    if(!rw)
    {
        //这是读操作，读的时候刷新目的地址，原则就是刷新DRAM(SRAM)
    	//wlibc_CleanFlushDCacheRegion((void *)buff_addr, len);
        DMAChannel_D->src_addr = 0x01c03000;
        DMAChannel_D->dst_addr = buff_addr;

    }
    else
    {
        //这是写操作，写的时候刷新源地址，原则就是刷新DRAM(SRAM)
    	//wlibc_CleanFlushDCacheRegion((void *)buff_addr, len);
        DMAChannel_D->src_addr = buff_addr;
        DMAChannel_D->dst_addr = 0x01c03000;

    }

    DMAChannel_D->bytes    = len;
    DMAChannel_D->config   = DMAChannel_D->config | 0x80000000;   /* start dma                                               */

    return 0;
}

/*
**********************************************************************************************************************
*                                       OSAL_DmaQueryStatus
*
* Description: This function is used to query interrupt pending and clear pending bits if some pending is be set
*
* Arguments  : hDma         dma handle
*
* Returns    : bit31~24     main interrupt no
*              bit23~16     sub interrupt no
*              bit1         dma end interrupt flag
*              bit0         dma half end interrupt flag
**********************************************************************************************************************
*/
__u32  	NAND_QueryDmaStat(void)
{
	return ((DMAChannel_D->config) & (0x1 << 30));
}


int NAND_WaitDmaFinish(void)
{
    return 0;
}
void NAND_AHBEnable(void)
{
    *(volatile __u32 *)(CCMU_REGS_BASE + 0x60) |= 1 << 13;
}

void NAND_AHBDisable(void)
{
    *(volatile __u32 *)(CCMU_REGS_BASE + 0x60) &= ~(1 << 13);
}

void NAND_ClkRequest(void)
{
    return ;
}

void NAND_ClkRelease(void)
{
    return ;
}

void NAND_ClkEnable(void)
{
	*(volatile __u32 *)(CCMU_REGS_BASE + 0x80) |= 1U << 31;

    return ;
}

void NAND_ClkDisable(void)
{
	*(volatile __u32 *)(CCMU_REGS_BASE + 0x80) &= ~(1U << 31);

    return ;
}


/*
**********************************************************************************************************************
*
*             NAND_GetCmuClk
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
__u32 NAND_GetCmuClk(void)
{
	__u32 reg_val;
	__u32 factor_n;
	__u32 factor_k;
	__u32 clock;

	reg_val  = *(volatile unsigned int *)(0x01c20000 + 0x28);//PLL6
	factor_n = (reg_val >> 8) & 0x1f;
	factor_k = ((reg_val >> 4) & 0x3) + 1;

	clock = 24 * factor_n * factor_k/2;

	return clock;
}
/*
**********************************************************************************************************************
*
*             NAND_GetCmuClk
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
void NAND_SetClk(__u32 nand_max_clock)
{
	__u32 edo_clk, cmu_clk;
	__u32 cfg;
	__u32 nand_clk_divid_ratio;

	/*set nand clock*/
	//edo_clk = (nand_max_clock > 20)?(nand_max_clock-10):nand_max_clock;
	edo_clk = nand_max_clock * 2;

    cmu_clk = NAND_GetCmuClk( );
	nand_clk_divid_ratio = cmu_clk / edo_clk;
	if (cmu_clk % edo_clk)
			nand_clk_divid_ratio++;
	if (nand_clk_divid_ratio){
		if (nand_clk_divid_ratio > 16)
			nand_clk_divid_ratio = 15;
		else
			nand_clk_divid_ratio--;
	}
	/*set nand clock gate on*/
	cfg = *(volatile __u32 *)(CCMU_REGS_BASE + 0x80);

	/*gate on nand clock*/
	cfg |= (1U << 31);
	/*take cmu pll6 as nand src block*/
	cfg &= ~(0x3 << 24);
	cfg |=  (0x1 << 24);
	//set divn = 0
	cfg &= ~(0x03 << 16);

	/*set ratio*/
	cfg &= ~(0x0f << 0);
	cfg |= (nand_clk_divid_ratio & 0xf) << 0;

	*(volatile __u32 *)(CCMU_REGS_BASE + 0x80) = cfg;
}

__s32 NAND_GetClk(void)
{
	__u32 cmu_clk;
	__u32 cfg;
	__u32 nand_max_clock, nand_clk_divid_ratio;

	/*set nand clock*/
    cmu_clk = NAND_GetCmuClk( );

    /*set nand clock gate on*/
	cfg = *(volatile __u32 *)(CCMU_REGS_BASE + 0x80);
    nand_clk_divid_ratio = ((cfg)&0xf) +1;
    nand_max_clock = cmu_clk/(2*nand_clk_divid_ratio);

    return nand_max_clock;


}

/*
**********************************************************************************************************************
*
*             NAND_GetCmuClk
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
extern  __s32 boot_set_gpio(void  *user_gpio_list, __u32 group_count_max, __s32 set_gpio);

void NAND_PIORequest(void)
{
//	boot_set_gpio((void *)BOOT_STORAGE_CODE1, 0, 1);
*(volatile uint *)(0x01c20800 + 0x48) = 0x22222222;
*(volatile uint *)(0x01c20800 + 0x4C) = 0x22222222;
*(volatile uint *)(0x01c20800 + 0x50) = 0x2222222;
*(volatile uint *)(0x01c20800 + 0x54) = 0x2;

}
/*
**********************************************************************************************************************
*
*             NAND_ReleasePin
*
*  Description:
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
void NAND_PIORelease(void)
{
	return ;
}
void NAND_EnRbInt(void)
{
	return ;
}


void NAND_ClearRbInt(void)
{
	return ;
}

int NAND_WaitRbReady(void)
{
	return 0;
}

void NAND_RbInterrupt(void)
{
	return ;
}
/*
************************************************************************************************************
*
*                                             OSAL_malloc
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ： 这是一个虚假的malloc函数，目的只是提供这样一个函数，避免编译不通过
*               本身不提供任何的函数功能
*
************************************************************************************************************
*/
void* NAND_Malloc(unsigned int Size)
{
	return (void *)0x40000000;
}
/*
************************************************************************************************************
*
*                                             OSAL_free
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ： 这是一个虚假的free函数，目的只是提供这样一个函数，避免编译不通过
*               本身不提供任何的函数功能
*
************************************************************************************************************
*/
void NAND_Free(void *pAddr, unsigned int Size)
{
    return ;
}

void *NAND_IORemap(unsigned int base_addr, unsigned int size)
{
    return (void *)base_addr;
}
/*
**********************************************************************************************************************
*
*             OSAL_printf
*
*  Description:  用户可以自行设定是否需要打印
*
*
*  Parameters:
*
*
*  Return value:
*
*
**********************************************************************************************************************
*/
__s32 NAND_Print(const char * str, ...)
{
	//UART_printf2(str);
    return 0;
}
