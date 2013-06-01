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
#ifndef  __OSAL_DE_H__
#define  __OSAL_DE_H__

#include "egon2.h"

/* define system clock id       */
typedef enum __AW_CCU_SYS_CLK
{
    AW_SYS_CLK_NONE,    /* invalid clock id                     */

    AW_SYS_CLK_LOSC,    /* "losc"       ,LOSC, 32768 hz clock   */
    AW_SYS_CLK_HOSC,    /* "hosc"       ,HOSC, 24Mhz clock      */

    AW_SYS_CLK_PLL1,    /* "core_pll"   ,PLL1 clock             */
    AW_SYS_CLK_PLL2,    /* "audio_pll"  ,PLL2 clock             */
    AW_SYS_CLK_PLL2X8,  /* "audio_pllx8"  ,PLL2 8x clock        */
    AW_SYS_CLK_PLL3,    /* "video_pll0" ,PLL3 clock             */
    AW_SYS_CLK_PLL3X2,  /* "video_pll0x2" ,PLL3 2x clock        */
    AW_SYS_CLK_PLL4,    /* "ve_pll"     ,PLL4 clock             */
    AW_SYS_CLK_PLL5,    /* "sdram_pll"  ,PLL5 clock             */
    AW_SYS_CLK_PLL5M,   /* "sdram_pll_m",PLL5 M clock           */
    AW_SYS_CLK_PLL5P,   /* "sdram_pll_p",PLL5 P clock           */
    AW_SYS_CLK_PLL6,    /* "sata_pll"   ,PLL6 clock, just used
                           as source of sata_pll_m and sata_pll_2,
                           users should not use this clock dirctly
                        */
    AW_SYS_CLK_PLL7,    /* "video_pll1" ,PLL7 clock             */
    AW_SYS_CLK_PLL7X2,  /* "video_pll1x2" ,PLL7 2x clock        */
    AW_SYS_CLK_200M,    /* "200m_pll"   ,200Mhz clock           */

    AW_SYS_CLK_CPU,     /* "cpu"        ,CPU clock              */
    AW_SYS_CLK_AXI,     /* "axi"        ,AXI clock              */
    AW_SYS_CLK_AHB,     /* "ahb"        ,AHB clock              */
    AW_SYS_CLK_APB0,    /* "apb"        ,APB0 clock             */
    AW_SYS_CLK_APB1,    /* "apb1"       ,APB1 clock             */

    /* add by kevin, 2011-7-21 19:01 */
    AW_SYS_CLK_PLL6M,   /* "sata_pll_m" ,PLL6 M clock, just for SATA    */
    AW_SYS_CLK_PLL62,   /* "sata_pll_2" ,PLL6 2 clock, for module       */

    AW_SYS_CLK_CNT      /* invalid id, for calc count           */

} __aw_ccu_sys_clk_e;


/* define module clock id       */
typedef enum __AW_CCU_MOD_CLK
{
    AW_MOD_CLK_NONE,/* invalid clock id             */

    AW_MOD_CLK_NFC,         /* "nfc"            */
    AW_MOD_CLK_MSC,         /* "msc"            */
    AW_MOD_CLK_SDC0,        /* "sdc0"           */
    AW_MOD_CLK_SDC1,        /* "sdc1"           */
    AW_MOD_CLK_SDC2,        /* "sdc2"           */
    AW_MOD_CLK_SDC3,        /* "sdc3"           */
    AW_MOD_CLK_TS,          /* "ts"             */
    AW_MOD_CLK_SS,          /* "ss"             */
    AW_MOD_CLK_SPI0,        /* "spi0"           */
    AW_MOD_CLK_SPI1,        /* "spi1"           */
    AW_MOD_CLK_SPI2,        /* "spi2"           */
    AW_MOD_CLK_PATA,        /* "pata"           */
    AW_MOD_CLK_IR0,         /* "ir0"            */
    AW_MOD_CLK_IR1,         /* "ir1"            */
    AW_MOD_CLK_I2S,         /* "i2s"            */
    AW_MOD_CLK_AC97,        /* "ac97"           */
    AW_MOD_CLK_SPDIF,       /* "spdif"          */
    AW_MOD_CLK_KEYPAD,      /* "key_pad"        */
    AW_MOD_CLK_SATA,        /* "sata"           */
    AW_MOD_CLK_USBPHY,      /* "usb_phy"        */
    AW_MOD_CLK_USBPHY0,     /* "usb_phy0"       */
    AW_MOD_CLK_USBPHY1,     /* "usb_phy1"       */
    AW_MOD_CLK_USBPHY2,     /* "usb_phy2"       */
    AW_MOD_CLK_USBOHCI0,    /* "usb_ohci0"      */
    AW_MOD_CLK_USBOHCI1,    /* "usb_ohci1"      */
    AW_MOD_CLK_GPS,         /* "com"            */
    AW_MOD_CLK_SPI3,        /* "spi3"           */
    AW_MOD_CLK_DEBE0,       /* "de_image0"      */
    AW_MOD_CLK_DEBE1,       /* "de_image1"      */
    AW_MOD_CLK_DEFE0,       /* "de_scale0"      */
    AW_MOD_CLK_DEFE1,       /* "de_scale1"      */
    AW_MOD_CLK_DEMIX,       /* "de_mix"         */
    AW_MOD_CLK_LCD0CH0,     /* "lcd0_ch0"       */
    AW_MOD_CLK_LCD1CH0,     /* "lcd1_ch0"       */
    AW_MOD_CLK_CSIISP,      /* "csi_isp"        */
    AW_MOD_CLK_TVD,         /* "tvd"            */
    AW_MOD_CLK_LCD0CH1_S1,  /* "lcd0_ch1_s1"    */
    AW_MOD_CLK_LCD0CH1_S2,  /* "lcd0_ch1_s2"    */
    AW_MOD_CLK_LCD1CH1_S1,  /* "lcd1_ch1_s1"    */
    AW_MOD_CLK_LCD1CH1_S2,  /* "lcd1_ch1_s2"    */
    AW_MOD_CLK_CSI0,        /* "csi0"           */
    AW_MOD_CLK_CSI1,        /* "csi1"           */
    AW_MOD_CLK_VE,          /* "ve"             */
    AW_MOD_CLK_ADDA,        /* "audio_codec"    */
    AW_MOD_CLK_AVS,         /* "avs"            */
    AW_MOD_CLK_ACE,         /* "ace"            */
    AW_MOD_CLK_LVDS,        /* "lvds"           */
    AW_MOD_CLK_HDMI,        /* "hdmi"           */
    AW_MOD_CLK_MALI,        /* "mali"           */
    AW_MOD_CLK_TWI0,        /* "twi0"           */
    AW_MOD_CLK_TWI1,        /* "twi1"           */
    AW_MOD_CLK_TWI2,        /* "twi2"           */
    AW_MOD_CLK_CAN,         /* "can"            */
    AW_MOD_CLK_SCR,         /* "scr"            */
    AW_MOD_CLK_PS20,        /* "ps0"            */
    AW_MOD_CLK_PS21,        /* "ps1"            */
    AW_MOD_CLK_UART0,       /* "uart0"          */
    AW_MOD_CLK_UART1,       /* "uart1"          */
    AW_MOD_CLK_UART2,       /* "uart2"          */
    AW_MOD_CLK_UART3,       /* "uart3"          */
    AW_MOD_CLK_UART4,       /* "uart4"          */
    AW_MOD_CLK_UART5,       /* "uart5"          */
    AW_MOD_CLK_UART6,       /* "uart6"          */
    AW_MOD_CLK_UART7,       /* "uart7"          */

    /* clock gating for hang to AXI bus */
    AW_MOD_CLK_AXI_DRAM,    /* "axi_dram"       */

    /* clock gating for hang to AHB bus */
    AW_MOD_CLK_AHB_USB0,    /* "ahb_usb0"       */
    AW_MOD_CLK_AHB_EHCI0,   /* "ahb_ehci0"      */
    AW_MOD_CLK_AHB_OHCI0,   /* "ahb_ohci0"      */
    AW_MOD_CLK_AHB_SS,      /* "ahb_ss"         */
    AW_MOD_CLK_AHB_DMA,     /* "ahb_dma"        */
    AW_MOD_CLK_AHB_BIST,    /* "ahb_bist"       */
    AW_MOD_CLK_AHB_SDMMC0,  /* "ahb_sdc0"       */
    AW_MOD_CLK_AHB_SDMMC1,  /* "ahb_sdc1"       */
    AW_MOD_CLK_AHB_SDMMC2,  /* "ahb_sdc2"       */
    AW_MOD_CLK_AHB_SDMMC3,  /* "ahb_sdc3"       */
    AW_MOD_CLK_AHB_MS,      /* "ahb_msc"        */
    AW_MOD_CLK_AHB_NAND,    /* "ahb_nfc"        */
    AW_MOD_CLK_AHB_SDRAM,   /* "ahb_sdramc"     */
    AW_MOD_CLK_AHB_ACE,     /* "ahb_ace"        */
    AW_MOD_CLK_AHB_EMAC,    /* "ahb_emac"       */
    AW_MOD_CLK_AHB_TS,      /* "ahb_ts"         */
    AW_MOD_CLK_AHB_SPI0,    /* "ahb_spi0"       */
    AW_MOD_CLK_AHB_SPI1,    /* "ahb_spi1"       */
    AW_MOD_CLK_AHB_SPI2,    /* "ahb_spi2"       */
    AW_MOD_CLK_AHB_SPI3,    /* "ahb_spi3"       */
    AW_MOD_CLK_AHB_PATA,    /* "ahb_pata"       */
    AW_MOD_CLK_AHB_SATA,    /* "ahb_sata"       */
    AW_MOD_CLK_AHB_GPS,     /* "ahb_com"        */
    AW_MOD_CLK_AHB_VE,      /* "ahb_ve"         */
    AW_MOD_CLK_AHB_TVD,     /* "ahb_tvd"        */
    AW_MOD_CLK_AHB_TVE0,    /* "ahb_tve0"       */
    AW_MOD_CLK_AHB_TVE1,    /* "ahb_tve1"       */
    AW_MOD_CLK_AHB_LCD0,    /* "ahb_lcd0"       */
    AW_MOD_CLK_AHB_LCD1,    /* "ahb_lcd1"       */
    AW_MOD_CLK_AHB_CSI0,    /* "ahb_csi0"       */
    AW_MOD_CLK_AHB_CSI1,    /* "ahb_csi1"       */
    AW_MOD_CLK_AHB_HDMI,    /* "ahb_hdmi"       */
    AW_MOD_CLK_AHB_DEBE0,   /* "ahb_de_image0"  */
    AW_MOD_CLK_AHB_DEBE1,   /* "ahb_de_image1"  */
    AW_MOD_CLK_AHB_DEFE0,   /* "ahb_de_scale0"  */
    AW_MOD_CLK_AHB_DEFE1,   /* "ahb_de_scale1"  */
    AW_MOD_CLK_AHB_MP,      /* "ahb_de_mix"     */
    AW_MOD_CLK_AHB_MALI,    /* "ahb_mali"       */

    /* clock gating for hang APB bus */
    AW_MOD_CLK_APB_ADDA,    /* "apb_audio_codec"    */
    AW_MOD_CLK_APB_SPDIF,   /* "apb_spdif"          */
    AW_MOD_CLK_APB_AC97,    /* "apb_ac97"           */
    AW_MOD_CLK_APB_I2S,     /* "apb_i2s"            */
    AW_MOD_CLK_APB_PIO,     /* "apb_pio"            */
    AW_MOD_CLK_APB_IR0,     /* "apb_ir0"            */
    AW_MOD_CLK_APB_IR1,     /* "apb_ir1"            */
    AW_MOD_CLK_APB_KEYPAD,  /* "apb_key_pad"        */
    AW_MOD_CLK_APB_TWI0,    /* "apb_twi0"           */
    AW_MOD_CLK_APB_TWI1,    /* "apb_twi1"           */
    AW_MOD_CLK_APB_TWI2,    /* "apb_twi2"           */
    AW_MOD_CLK_APB_CAN,     /* "apb_can"            */
    AW_MOD_CLK_APB_SCR,     /* "apb_scr"            */
    AW_MOD_CLK_APB_PS20,    /* "apb_ps0"            */
    AW_MOD_CLK_APB_PS21,    /* "apb_ps1"            */
    AW_MOD_CLK_APB_UART0,   /* "apb_uart0"          */
    AW_MOD_CLK_APB_UART1,   /* "apb_uart1"          */
    AW_MOD_CLK_APB_UART2,   /* "apb_uart2"          */
    AW_MOD_CLK_APB_UART3,   /* "apb_uart3"          */
    AW_MOD_CLK_APB_UART4,   /* "apb_uart4"          */
    AW_MOD_CLK_APB_UART5,   /* "apb_uart5"          */
    AW_MOD_CLK_APB_UART6,   /* "apb_uart6"          */
    AW_MOD_CLK_APB_UART7,   /* "apb_uart7"          */

    /* clock gating for access dram */
    AW_MOD_CLK_SDRAM_VE,    /* "sdram_ve"           */
    AW_MOD_CLK_SDRAM_CSI0,  /* "sdram_csi0"         */
    AW_MOD_CLK_SDRAM_CSI1,  /* "sdram_csi1"         */
    AW_MOD_CLK_SDRAM_TS,    /* "sdram_ts"           */
    AW_MOD_CLK_SDRAM_TVD,   /* "sdram_tvd"          */
    AW_MOD_CLK_SDRAM_TVE0,  /* "sdram_tve0"         */
    AW_MOD_CLK_SDRAM_TVE1,  /* "sdram_tve1"         */
    AW_MOD_CLK_SDRAM_DEFE0, /* "sdram_de_scale0"    */
    AW_MOD_CLK_SDRAM_DEFE1, /* "sdram_de_scale1"    */
    AW_MOD_CLK_SDRAM_DEBE0, /* "sdram_de_image0"    */
    AW_MOD_CLK_SDRAM_DEBE1, /* "sdram_de_image1"    */
    AW_MOD_CLK_SDRAM_DEMP,  /* "sdram_de_mix"       */
    AW_MOD_CLK_SDRAM_ACE,   /* "sdram_ace"          */

    AW_MOD_CLK_AHB_EHCI1,   /* "ahb_ehci1"          */
    AW_MOD_CLK_AHB_OHCI1,   /* "ahb_ohci1"          */

    AW_MOD_CLK_CNT

} __aw_ccu_mod_clk_e;



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

void OSAL_IrqLock(__u32 *cpu_sr);
void OSAL_IrqUnLock(__u32 cpu_sr);

void OSAL_CacheRangeFlush(void *Address, __u32 Length, __u32 Flags);

int OSAL_sw_get_ic_ver(void);

#define sys_get_wvalue(n)   (*((volatile __u32 *)(n)))          /* word input */
#define sys_put_wvalue(n,c) (*((volatile __u32 *)(n))  = (c))   /* word output */


#endif  //__OSAL_DE_H__

