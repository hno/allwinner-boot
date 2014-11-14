/*
*********************************************************************************************************
*                                                AR100 SYSTEM
*                                     AR100 Software System Develop Kits
*                                              	  p2wi module
*
*                                    (c) Copyright 2012-2016, Sunny China
*                                             All Rights Reserved
*
* File    : p2wi.c
* By      : Sunny
* Version : v1.0
* Date    : 2012-5-22
* Descript: p2wi module.
* Update  : date                auther      ver     notes
*           2012-5-22 9:49:38	Sunny       1.0     Create this file.
*********************************************************************************************************
*/
#include "include.h"
#include "p2wi.h"

//p2wi can be config used or not

__s32 p2wi_init(void)
{
	__u32 reg_val;
#ifdef	CONFIG_SUN6I_FPGA
	//PG[24][25] used as p2wi SCK and SDA under FPGA platform.
	egon2_writel(0x33, 0x01c20800 + 0xE4);
#endif
	reg_val = egon2_readl(0x1f01400 + 0x28);
	reg_val |= (1 << 0);
	egon2_writel(reg_val, 0x1f01400 + 0x28);

	reg_val = egon2_readl(0x1f02C00 + 0x00);
	reg_val &= ~((0x07 << 0) | (0x07 << 4));
	reg_val |=  ((0x03 << 0) | (0x03 << 4));
	egon2_writel(reg_val, 0x1f02C00 + 0x00);

	//enbale twi clock, set reset as de-assert state.
	reg_val = egon2_readl(0x1f01400 + 0x28);
	reg_val |= (1 << 3);
	egon2_writel(reg_val, 0x1f01400 + 0x28);

	reg_val = egon2_readl(0x1f01400 + 0xb0);
	reg_val |= (1 << 3);
	egon2_writel(reg_val, 0x1f01400 + 0xb0);

	//reset p2wi controller
	egon2_writel(P2WI_SOFT_RST, P2WI_REG_CTRL);

	//set p2wi clock
	p2wi_set_clk(P2WI_SCK);

	//pmu initialize as p2wi mode
	p2wi_set_pmu_mode(0x68, 0x3e, 0x3e);

	return 0;
}

__s32 p2wi_exit(void)
{
	__u32 reg_val;

	reg_val = egon2_readl(0x1f01400 + 0xb0);
	reg_val &= ~(1 << 3);
	egon2_writel(reg_val, 0x1f01400 + 0xb0);

	reg_val = egon2_readl(0x1f01400 + 0x28);
	reg_val &= ~(1 << 3);
	egon2_writel(reg_val, 0x1f01400 + 0x28);

#ifdef	CONFIG_SUN6I_FPGA
	//PG[24][25] used as p2wi SCK and SDA under FPGA platform.
	egon2_writel(0x0, 0x01c20800 + 0xE4);
#endif
	return 0;
}

__s32 p2wi_read(u8 *addr, u8 *data, u32 len)
{
	__u32 i;
	__u32 addr0 = 0, addr1 = 0;
	__u32 data0 = 0, data1 = 0;

	if((len >= PMU_TRANS_BYTE_MAX) || (len <= 0))
	{
		eGon2_printf("p2wi read error\n");

		return -1;
	}

	for (i = 0; i < len; i++)
	{
		if (i < 4)
		{
			//pack 8bit addr0~addr3 into 32bit addr0
			addr0 |= addr[i] << (i * 8);
		}
		else
		{
			//pack 8bit addr4~addr7 into 32bit addr1
			addr1 |= addr[i] << ((i - 4) * 8);
		}
	}

	//write address to register
	egon2_writel(addr0, P2WI_REG_DADDR0);
	egon2_writel(addr1, P2WI_REG_DADDR1);
	egon2_writel((len - 1) | (1 << 4), P2WI_REG_DLEN);

	//start transmit
	egon2_writel(egon2_readl(P2WI_REG_CTRL) | P2WI_START_TRANS, P2WI_REG_CTRL);
	//wait transfer complete
	if (p2wi_wait_state() != 0)
	{
		eGon2_printf("p2wi read failed\n");
		return -1;
	}

	//read out data
	data0 = egon2_readl(P2WI_REG_DATA0);
	data1 = egon2_readl(P2WI_REG_DATA1);
	for (i = 0; i < len; i++)
	{
		if (i < 4)
		{
			data[i] = (data0 >> (i * 8)) & 0xff;
		}
		else
		{
			data[i] = (data1 >> ((i - 4) * 8)) & 0xff;
		}
	}

	return 0;
}

__s32 p2wi_write(u8 *addr, u8 *data, u32 len)
{
	__u32 i;
	__u32 addr0 = 0, addr1 = 0;
	__u32 data0 = 0, data1 = 0;

	if((len >= PMU_TRANS_BYTE_MAX) || (len <= 0))
	{
		eGon2_printf("p2wi write error\n");

		return -1;
	}

	for (i = 0; i < len; i++)
	{
		if (i < 4)
		{
			//pack 8bit data0~data3 into 32bit data0
			//pack 8bit addr0~addr3 into 32bit addr0
			addr0 |= addr[i] << (i * 8);
			data0 |= data[i] << (i * 8);
		}
		else
		{
			//pack 8bit data4~data7 into 32bit data1
			//pack 8bit addr4~addr7 into 32bit addr4
			addr1 |= addr[i] << ((i - 4) * 8);
			data1 |= data[i] << ((i - 4) * 8);
		}
	}

	//write register
	egon2_writel(addr0, P2WI_REG_DADDR0);
	egon2_writel(addr1, P2WI_REG_DADDR1);
	egon2_writel(data0, P2WI_REG_DATA0);
	egon2_writel(data1, P2WI_REG_DATA1);
	egon2_writel(len - 1, P2WI_REG_DLEN);

	//start transfer
	egon2_writel(egon2_readl(P2WI_REG_CTRL) | P2WI_START_TRANS, P2WI_REG_CTRL);
	//wait transfer complete
	if (p2wi_wait_state() != 0)
	{
		eGon2_printf("p2wi write failed\n");
		return -1;
	}

	return 0;
}

void p2wi_set_pmu_mode(u32 slave_addr, u32 reg, u32 data)
{
	//set pmu work mode
	egon2_writel(P2WI_PMU_INIT | (slave_addr) | (reg << 8) | (data << 16), P2WI_REG_PMCR);

	//wait pmu mode set complete
	while(egon2_readl(P2WI_REG_PMCR) & P2WI_PMU_INIT)
	{
		;
	}
}

void p2wi_set_clk(u32 sck)
{
#ifndef	CONFIG_SUN6I_FPGA
	u32 src_clk = 12000000;
	u32 div, sda_odly, rval;
#ifdef DEBUG
	eGon2_printf("p2wi apb1 clock = %d\n", src_clk);
#endif
	div = src_clk / sck / 2 - 1;
#ifdef DEBUG
	eGon2_printf("div = %d\n", div);
#endif
	sda_odly = div >> 1;
#ifdef DEBUG
	eGon2_printf("sda_odly = %d\n", sda_odly);
#endif
	rval = div | (sda_odly << 8);
#ifdef DEBUG
	eGon2_printf("rval = %x\n", rval);
#endif
	egon2_writel(rval, P2WI_REG_CCR);
#ifdef DEBUG
	eGon2_printf("P2WI_REG_CCR = %x\n", egon2_readl(P2WI_REG_CCR));
#endif
#else
	//fpga platform: apb clock = 24M, p2wi clock = 3M.
	egon2_writel(0x103, P2WI_REG_CCR);
#endif
}

__s32 p2wi_wait_state(void)
{
	__s32  ret = -1;
	__u32  stat;

	while (1)
	{
		stat = egon2_readl(P2WI_REG_STAT);
		if (stat & P2WI_TERR_INT)
		{
			//transfer error
			eGon2_printf("p2wi transfer error [%x]\n", ((stat >> 8) & 0xff));
			ret = -1;
			break;
		}
		if (stat & P2WI_TOVER_INT)
		{
			//transfer complete
			ret = 0;
			break;
		}
	}
	//clear state flag
	egon2_writel(stat, P2WI_REG_STAT);

	return ret;
}

