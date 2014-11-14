/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON arm boot sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :
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
#ifndef  __OSAL_DE_H__
#define  __OSAL_DE_H__

#include "egon2.h"

#ifdef __FPGA_DEBUG__
#define AW_IRQ_DEFE0        59
#define AW_IRQ_LCD0         57
#define AW_IRQ_MIPIDSI      35
#define AW_IRQ_LCD1         100
#define AW_IRQ_DEFE1        100
#else
#define AW_IRQ_DEFE0        125
#define AW_IRQ_LCD0         118
#define AW_IRQ_MIPIDSI      121
#define AW_IRQ_LCD1         119
#define AW_IRQ_DEFE1        126
#endif

typedef enum
{
    SYS_CLK_PLL3,
    SYS_CLK_PLL7,
    SYS_CLK_PLL8,
    SYS_CLK_PLL9,
    SYS_CLK_PLL10,
    SYS_CLK_PLL3X2,
    SYS_CLK_PLL6,
    SYS_CLK_PLL6x2,
    SYS_CLK_PLL7X2,
    SYS_CLK_MIPIPLL,

    MOD_CLK_DEBE0,
    MOD_CLK_DEBE1,
    MOD_CLK_DEFE0,
    MOD_CLK_DEFE1,
    MOD_CLK_DEMIX,
    MOD_CLK_LCD0CH0,
    MOD_CLK_LCD0CH1,
    MOD_CLK_LCD1CH0,
    MOD_CLK_LCD1CH1,
    MOD_CLK_HDMI,
    MOD_CLK_HDMI_DDC,
    MOD_CLK_MIPIDSIS,
    MOD_CLK_MIPIDSIP,
    MOD_CLK_IEPDRC0,
    MOD_CLK_IEPDRC1,
    MOD_CLK_IEPDEU0,
    MOD_CLK_IEPDEU1,
    MOD_CLK_LVDS,
    
    AHB_CLK_MIPIDSI,
    AHB_CLK_LCD0,
    AHB_CLK_LCD1,
    AHB_CLK_CSI0,
    AHB_CLK_CSI1,
    AHB_CLK_HDMI,
    AHB_CLK_DEBE0,
    AHB_CLK_DEBE1,
    AHB_CLK_DEFE0,
    AHB_CLK_DEFE1,
    AHB_CLK_DEU0,
    AHB_CLK_DEU1,
    AHB_CLK_DRC0,
    AHB_CLK_DRC1,
    AHB_CLK_TVE0, //not exist in a31
    AHB_CLK_TVE1, //not exist in a31
    
    DRAM_CLK_DRC0,
    DRAM_CLK_DRC1,
    DRAM_CLK_DEU0,
    DRAM_CLK_DEU1,
    DRAM_CLK_DEFE0,
    DRAM_CLK_DEFE1,
    DRAM_CLK_DEBE0,
    DRAM_CLK_DEBE1,
}__disp_clk_id_t;




typedef __s32 (*ISRCallback)( void *pArg);
/* 刷新标记位 */
#define  CACHE_FLUSH_I_CACHE_REGION				0  /* 清除I-cache中代表主存中一块区域的cache行 			*/
#define  CACHE_FLUSH_D_CACHE_REGION				1  /* 清除D-cache中代表主存中一块区域的cache行 			*/
#define  CACHE_FLUSH_CACHE_REGION				2  /* 清除D-cache和I-cache中代表主存中一块区域的cache行 */
#define  CACHE_CLEAN_D_CACHE_REGION				3  /* 清理D-cache中代表主存中一块区域的cache行 			*/
#define  CACHE_CLEAN_FLUSH_D_CACHE_REGION	 	4  /* 清理并清除D-cache中代表主存中一块区域的cache行 	*/
#define  CACHE_CLEAN_FLUSH_CACHE_REGION			5  /* 清理并清除D-cache，接下来解除I-cache 				*/

__hdle OSAL_GPIO_Request(user_gpio_set_t *gpio_list, __u32 group_count_max);
__hdle OSAL_GPIO_Request_Ex(char *main_name, const char *sub_name);
__s32 OSAL_GPIO_Release(__hdle p_handler, __s32 if_release_to_default_status);
__s32 OSAL_GPIO_DevGetAllPins_Status(unsigned p_handler, user_gpio_set_t *gpio_status, unsigned gpio_count_max, unsigned if_get_from_hardware);
__s32 OSAL_GPIO_DevGetONEPins_Status(unsigned p_handler, user_gpio_set_t *gpio_status,const char *gpio_name,unsigned if_get_from_hardware);
__s32 OSAL_GPIO_DevSetONEPin_Status(u32 p_handler, user_gpio_set_t *gpio_status, const char *gpio_name, __u32 if_set_to_current_input_status);
__s32 OSAL_GPIO_DevSetONEPIN_IO_STATUS(u32 p_handler, __u32 if_set_to_output_status, const char *gpio_name);
__s32 OSAL_GPIO_DevSetONEPIN_PULL_STATUS(u32 p_handler, __u32 set_pull_status, const char *gpio_name);
__s32 OSAL_GPIO_DevREAD_ONEPIN_DATA(u32 p_handler, const char *gpio_name);
__s32 OSAL_GPIO_DevWRITE_ONEPIN_DATA(u32 p_handler, __u32 value_to_gpio, const char *gpio_name);

int OSAL_Script_FetchParser_Data(char *main_name, char *sub_name, int value[], int count);

__s32 OSAL_CCMU_SetSrcFreq(__u32 nSclkNo, __u32 nFreq);
__u32 OSAL_CCMU_GetSrcFreq(__u32 nSclkNo);
__hdle OSAL_CCMU_OpenMclk(__s32 nMclkNo);
__s32  OSAL_CCMU_CloseMclk(__hdle hMclk);
__s32 OSAL_CCMU_SetMclkSrc(__hdle hMclk, __u32 nSclkNo);
__s32 OSAL_CCMU_GetMclkSrc(__hdle hMclk);
__s32 OSAL_CCMU_SetMclkDiv(__hdle hMclk, __s32 nDiv);
__u32 OSAL_CCMU_GetMclkDiv(__hdle hMclk);
__s32 OSAL_CCMU_MclkOnOff(__hdle hMclk, __s32 bOnOff);
__s32 OSAL_CCMU_MclkReset(__hdle hMclk, __s32 bReset);

void OSAL_InterruptDisable(__u32 IrqNo);
void OSAL_InterruptEnable(__u32 IrqNo);
int OSAL_RegISR(__u32 IrqNo,
				__u32 Flags,
				ISRCallback Handler,
				void *pArg,
				__u32 DataSize,
				__u32 Prio);
void OSAL_UnRegISR(__u32 IrqNo, ISRCallback Handler, void *pArg);

void *OSAL_PAtoVA(void * pa);
void *OSAL_VAtoPA(void * pa);

void * OSAL_malloc(__u32 Size);
void OSAL_free(void * pAddr);
void * OSAL_PhyAlloc(__u32 Size);
void OSAL_PhyFree(void *pAddr, __u32 Size);

void OSAL_IrqLock(__u32 *cpu_sr);
void OSAL_IrqUnLock(__u32 cpu_sr);

void OSAL_CacheRangeFlush(void *Address, __u32 Length, __u32 Flags);

int OSAL_sw_get_ic_ver(void);

#define sys_get_wvalue(n)   (*((volatile __u32 *)(n)))          /* word input */
#define sys_put_wvalue(n,c) (*((volatile __u32 *)(n))  = (c))   /* word output */


#endif  //__OSAL_DE_H__

