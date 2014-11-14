/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: MMC driver for winner's mmc controller
 * Author: Aaron <leafy.myeh@allwinnertech.com>
 * Date: 2012-2-3 14:18:18
 */
#include "string.h"
#include "mmc_def.h"
#include "mmc.h"

/* winner's mmc controller definition */
struct sunxi_mmc {
	u32 gctrl;         /* (0x00) SMC Global Control Register */
	u32 clkcr;         /* (0x04) SMC Clock Control Register */
	u32 timeout;       /* (0x08) SMC Time Out Register */
	u32 width;         /* (0x0C) SMC Bus Width Register */
	u32 blksz;         /* (0x10) SMC Block Size Register */
	u32 bytecnt;       /* (0x14) SMC Byte Count Register */
	u32 cmd;           /* (0x18) SMC Command Register */
	u32 arg;           /* (0x1C) SMC Argument Register */
	u32 resp0;         /* (0x20) SMC Response Register 0 */
	u32 resp1;         /* (0x24) SMC Response Register 1 */
	u32 resp2;         /* (0x28) SMC Response Register 2 */
	u32 resp3;         /* (0x2C) SMC Response Register 3 */
	u32 imask;         /* (0x30) SMC Interrupt Mask Register */
	u32 mint;          /* (0x34) SMC Masked Interrupt Status Register */
	u32 rint;          /* (0x38) SMC Raw Interrupt Status Register */
	u32 status;        /* (0x3C) SMC Status Register */
	u32 ftrglevel;     /* (0x40) SMC FIFO Threshold Watermark Register */
	u32 funcsel;       /* (0x44) SMC Function Select Register */
	u32 cbcr;          /* (0x48) SMC CIU Byte Count Register */
	u32 bbcr;          /* (0x4C) SMC BIU Byte Count Register */
	u32 dbgc;          /* (0x50) SMC Debug Enable Register */
	u32 res0[9];       /* (0x54~0x74) */
	u32 hwrst;         /* (0x78) SMC eMMC Hardware Reset Register */
	u32 res1;          /* (0x7c) */
	u32 dmac;          /* (0x80) SMC IDMAC Control Register */
	u32 dlba;          /* (0x84) SMC IDMAC Descriptor List Base Address Register */
	u32 idst;          /* (0x88) SMC IDMAC Status Register */
	u32 idie;          /* (0x8C) SMC IDMAC Interrupt Enable Register */
	u32 chda;          /* (0x90) */
	u32 cbda;          /* (0x94) */
	u32 res2[26];      /* (0x98~0xff) */
	u32 thldc;		 /* (0x100) Card Threshold Control Register */
	u32 res3[2];		 /* (0x104~0x10b) */
	u32 dsbd;		 /* (0x10c) eMMC4.5 DDR Start Bit Detection Control */
	u32 res4[60];		/* (0x110~0x1ff) */
	u32 fifo;          /* (0x200) SMC FIFO Access Address */
};

#ifdef MMC_DEBUG
static void dumphex32(char* name, char* base, int len)
{
	__u32 i;

	mmcmsg("dump %s registers:", name);
	for (i=0; i<len; i+=4)
	{
		if (!(i&0xf))
			mmcmsg("\n%p : ", base + i);
		mmcmsg("%x ", readl(base + i));
	}
	mmcmsg("\n");
}

static void dumpmmcreg(struct sunxi_mmc *reg)
{
	mmcmsg("gctrl     0x%x\n", reg->gctrl     );
	mmcmsg("clkcr     0x%x\n", reg->clkcr     );
	mmcmsg("timeout   0x%x\n", reg->timeout   );
	mmcmsg("width     0x%x\n", reg->width     );
	mmcmsg("blksz     0x%x\n", reg->blksz     );
	mmcmsg("bytecnt   0x%x\n", reg->bytecnt   );
	mmcmsg("cmd       0x%x\n", reg->cmd       );
	mmcmsg("arg       0x%x\n", reg->arg       );
	mmcmsg("resp0     0x%x\n", reg->resp0     );
	mmcmsg("resp1     0x%x\n", reg->resp1     );
	mmcmsg("resp2     0x%x\n", reg->resp2     );
	mmcmsg("resp3     0x%x\n", reg->resp3     );
	mmcmsg("imask     0x%x\n", reg->imask     );
	mmcmsg("mint      0x%x\n", reg->mint      );
	mmcmsg("rint      0x%x\n", reg->rint      );
	mmcmsg("status    0x%x\n", reg->status    );
	mmcmsg("ftrglevel 0x%x\n", reg->ftrglevel );
	mmcmsg("funcsel   0x%x\n", reg->funcsel   );
	mmcmsg("dmac      0x%x\n", reg->dmac      );
	mmcmsg("dlba      0x%x\n", reg->dlba      );
	mmcmsg("idst      0x%x\n", reg->idst      );
	mmcmsg("idie      0x%x\n", reg->idie      );
}
#else
#define dumphex32(fmt...)
#define dumpmmcreg(fmt...)
#endif /* SUNXI_mmcdbg */

struct sunxi_mmc_des {
	u32             :1,
		dic         :1, /* disable interrupt on completion */
		last_des    :1, /* 1-this data buffer is the last buffer */
		first_des   :1, /* 1-data buffer is the first buffer,
						   0-data buffer contained in the next descriptor is 1st buffer */
		des_chain   :1, /* 1-the 2nd address in the descriptor is the next descriptor address */
		end_of_ring :1, /* 1-last descriptor flag when using dual data buffer in descriptor */
					:24,
		card_err_sum:1, /* transfer error flag */
		own			:1; /* des owner:1-idma owns it, 0-host owns it */
#if defined MMC_SUN4I
#define SDXC_DES_NUM_SHIFT 12
#define SDXC_DES_BUFFER_MAX_LEN	(1 << SDXC_DES_NUM_SHIFT)
	u32	data_buf1_sz:13,
	    data_buf2_sz:13,
    				:6;
#else
#define SDXC_DES_NUM_SHIFT 15
#define SDXC_DES_BUFFER_MAX_LEN	(1 << SDXC_DES_NUM_SHIFT)
	u32 data_buf1_sz:16,
	    data_buf2_sz:16;
#endif
	u32	buf_addr_ptr1;
	u32	buf_addr_ptr2;
};

struct sunxi_mmc_host {
	struct sunxi_mmc *reg;
	u32  mmc_no;
	u32  mclk;
	u32  hclkrst;
	u32  hclkbase;
	u32  mclkbase;
	u32  database;
	u32  fatal_err;
	struct sunxi_mmc_des *pdes;
};

/* support 4 mmc hosts */
struct mmc mmc_dev[MAX_MMC_NUM];
struct sunxi_mmc_host mmc_host[MAX_MMC_NUM];
s32 smc_set_card_clk(u32 smc_no, u32 cclk,u32 bus_width,u32 ddr);

static int mmc_resource_init(int sdc_no)
{
	struct sunxi_mmc_host* mmchost = &mmc_host[sdc_no];

	mmcdbg("init mmc %d resource\n", sdc_no);
	mmchost->reg = (struct sunxi_mmc *)(MMC_REG_BASE + sdc_no * 0x1000);
	mmchost->database = (u32)mmchost->reg + MMC_REG_FIFO_OS;
	mmchost->hclkbase = CCMU_HCLKGATE0_BASE;
	mmchost->hclkrst = CCMU_HCLKRST0_BASE;
	if (sdc_no == 0)
		mmchost->mclkbase = CCMU_MMC0_CLK_BASE;
	else if (sdc_no == 2)
		mmchost->mclkbase = CCMU_MMC2_CLK_BASE;
	else {
		mmcinfo("Wrong mmc NO.: %d\n", sdc_no);
		return -1;
	}
	mmchost->mmc_no = sdc_no;

	return 0;
}

static int mmc_clk_io_onoff(int sdc_no, int onoff)
{
	struct sunxi_mmc_host* mmchost = &mmc_host[sdc_no];
	u32 rval;
	u32 gpioc_base = 0x01c20800 + 0x48;
	u32 gpiof_base = 0x01c20800 + 0xb4;

	mmcdbg("init mmc %d clock and io\n", sdc_no);
	/* config gpio */
	switch (sdc_no) {
        case 0:
            /* D1-PF0, D0-PF1, CLK-PF2, CMD-PF3, D3-PF4, D4-PF5 */
            if (onoff) {
	            writel(0x222222, gpiof_base + 0x0);
	            writel(0x555, gpiof_base + 0x1c);
	            //writel(0xaaa, gpiof_base + 0x14);
	            writel(0xfff, gpiof_base + 0x14);
	        } else {
	            writel(0x404044, gpiof_base + 0x0);
	            writel(0, gpiof_base + 0x1c);
	            writel(0x555, gpiof_base + 0x14);
	        }
            break;

        case 2:
            /* CMD-PC6, CLK-PC7, D0-PC8, D1-PC9, D2-PC10, D3-PC11 */
            if (onoff) {
	            writel(0x33000000, gpioc_base + 0x0);
	            writel(0x3333, gpioc_base + 0x4);
	            writel(0x555 << 12, gpioc_base + 0x1c);
	            writel(0xaaa << 12, gpioc_base + 0x14);
            } else {
	            writel(0, gpioc_base + 0x0);
	            writel(0, gpioc_base + 0x4);
	            writel(0, gpioc_base + 0x1c);
	            writel(0x55555555, gpioc_base + 0x14);
            }
            break;

        default:
        	mmcinfo("Wrong mmc NO.: %d\n", sdc_no);
            return -1;
	}

	/* config ahb clock */
	rval = readl(mmchost->hclkbase);
	if (onoff)
		rval |= (1 << (8 + sdc_no));
	else
		rval &= ~(1 << (8 + sdc_no));
	writel(rval, mmchost->hclkbase);
	/* release reset */
	rval = readl(mmchost->hclkrst);
	if (onoff)
		rval |= (1 << (8 + sdc_no));
	else
		rval &= ~(1 << (8 + sdc_no));
	writel(rval, mmchost->hclkrst);

	/* config mod clock */
	if (onoff) {
		smc_set_card_clk(sdc_no,400000,1,0);
		mmcdbg("init mmc mclk %d\n", mmchost->mclk);
	} else {
		writel(0, mmchost->mclkbase);
	}
	mmcdbg("init mmc %d clock and io over\n", sdc_no);
	dumphex32("ccmu", (char*)0x01c20000, 0x100);
	dumphex32("gpio", (char*)0x01c20800, 0x100);
	dumphex32("mmc", (char*)mmchost->reg, 0x100);
    return 0;
}


static int mmc_update_clk(struct mmc *mmc)
{
	struct sunxi_mmc_host* mmchost = (struct sunxi_mmc_host *)mmc->priv;
	unsigned int cmd;
	unsigned timeout = 0xfffff;

	cmd = (1U << 31) | (1 << 21) | (1 << 13);
	writel(cmd, &mmchost->reg->cmd);

	while((readl(&mmchost->reg->cmd)&0x80000000) && --timeout);
	if (!timeout) {
		mmcinfo("update clk failed\n");
		dumphex32("mmc", (char*)mmchost->reg, 0x100);
		return -1;
	}

	writel(readl(&mmchost->reg->rint), &mmchost->reg->rint);
	return 0;
}


u32 ccm_get_pll6_dev_clk(void)
{
	u32 rval = 0;
	u32 n, k;
	rval = readl(CCMU_PLL6_CLK_BASE);
	n = (0x1f & (rval >> 8)) + 1;
	k = (0x3 & (rval >> 4)) + 1;
	return (24000000 * n * k)>>1;
}

/* 400K, SCLK-24M, CCLK-400K
 * 25M, SCLK-PLL6(200M/180M), CCLK-25M(22.5M)
 * 50M, SCLK-PLL6(200M/180M), CCLK-50M(45M)
 * 100M, SCLK-PLL6(200M/180M), CCLK-50M(90M)
 * 200M, SCLK-PLL6(200M/180M), CCLK-200M(180M)
 */
s32 smc_set_card_clk(u32 smc_no, u32 cclk,u32 bus_width,u32 ddr)
{
	struct sunxi_mmc_host* mmchost = &mmc_host[smc_no];
	u32 sclk = 24000000;
	u32 div;
	u32 rval;
	u32 src = 0;
	u32 mclk_base =mmchost->mclkbase;
	u32 m, n;
	u32 outclk_pha = 0;
	u32 samclk_pha = 0;
	u32 ddr8_clk;

	if (cclk > 400000) {
		src = 1;
		sclk = ccm_get_pll6_dev_clk();
		outclk_pha = 3;
		samclk_pha = 5;
		/* if in ddr and width8 mode, module clock must be twice of card clock */
		if (bus_width == 8 && ddr) {
			ddr8_clk = cclk << 1;
			div = (2 * sclk + ddr8_clk) / (2 * ddr8_clk);
		} else {
			div = (2 * sclk + cclk) / (2 * cclk);
		}
		div = div==0 ? 1 : div;
		if (div > 128) {
			m = 1;
			n = 0;
			mmcinfo("Source clock is too high\n");
		} else if (div > 64) {
			n = 3;
			m = div >> 3;
		} else if (div > 32) {
			n = 2;
			m = div >> 2;
		} else if (div > 16) {
			n = 1;
			m = div >> 1;
		} else {
			n = 0;
			m = div;
		}

		rval = (1U << 31) | (src << 24) | (samclk_pha << 20)
	    		| (n << 16) | (outclk_pha << 8) | (m - 1);
		writel(rval, mclk_base);

		/* clear internal divider */
		rval = readl(&mmchost->reg->clkcr) & (~0xff);
		if (bus_width == 8 && ddr)
			rval |= 0x1;
		writel(rval, &mmchost->reg->clkcr);
	} else {
		src = 0;
		sclk = 24000000;
		outclk_pha = 0;
		samclk_pha = 0;

		div = (2 * sclk + cclk) / (2 * cclk);
		div = div==0 ? 1 : div;
		if (div > 128) {
			m = 1;
			n = 0;
			mmcinfo("Source clock is too high\n");
		} else if (div > 64) {
			n = 3;
			m = div >> 3;
		} else if (div > 32) {
			n = 2;
			m = div >> 2;
		} else if (div > 16) {
			n = 1;
			m = div >> 1;
		} else {
			n = 0;
			m = div;
		}

		rval = (1U << 31) | (src << 24) | (samclk_pha << 20)
	    		| (n << 16) | (outclk_pha << 8) | (m - 1);
		writel(rval, mclk_base);

		/* clear internal divider */
		rval = readl(&mmchost->reg->clkcr) & (~0xff);
		writel(rval,& mmchost->reg->clkcr);
	}

	mmchost->mclk = sclk/(1<<n)/m;
	mmcdbg("Card clock=%d\n",mmchost->mclk);
	//dumphex32("ccmu", (char*)0x01c20000, 0x100);

	return cclk;
}


static int mmc_config_clock(struct mmc *mmc)
{
	struct sunxi_mmc_host* mmchost = (struct sunxi_mmc_host *)mmc->priv;
	unsigned rval = readl(&mmchost->reg->clkcr);

	/*
	 * CLKCREG[7:0]: divider
	 * CLKCREG[16]:  on/off
	 * CLKCREG[17]:  power save
	 */
	/* Disable Clock */
	rval &= ~(1 << 16);
	writel(rval, &mmchost->reg->clkcr);
	if(mmc_update_clk(mmc)) {
		mmcinfo("disable clock failed\n");
		return -1;
	}
	/* Change Divider Factor */
	smc_set_card_clk(mmchost->mmc_no,mmc->clock,mmc->bus_width,0);
	/* Re-enable Clock */
	rval |= (1 << 16);
	writel(rval, &mmchost->reg->clkcr);
	if(mmc_update_clk(mmc)) {
		mmcinfo("re-enable clock failed\n");
		return -1;
	}
	return 0;
}

static void mmc_set_ios(struct mmc *mmc)
{
	struct sunxi_mmc_host* mmchost = (struct sunxi_mmc_host *)mmc->priv;

	mmcdbg("ios: bus: %d, clock: %d\n", mmc->bus_width, mmc->clock);

	/* Change clock first */
	if (mmc->clock)
		if (mmc_config_clock(mmc)) {
			mmchost->fatal_err = 1;
			mmcinfo("Config clock failed\n");
			return;
		}

	/* Change bus width */
	if (mmc->bus_width == 8)
		writel(2, &mmchost->reg->width);
	else if (mmc->bus_width == 4)
		writel(1, &mmchost->reg->width);
	else
		writel(0, &mmchost->reg->width);
}

static int mmc_core_init(struct mmc *mmc)
{
	struct sunxi_mmc_host* mmchost = (struct sunxi_mmc_host *)mmc->priv;
	/* Reset controller */
	writel(0x7, &mmchost->reg->gctrl);
	return 0;
}
static int mmc_trans_data_by_cpu(struct mmc *mmc, struct mmc_data *data)
{
	struct sunxi_mmc_host* mmchost = (struct sunxi_mmc_host *)mmc->priv;
	unsigned i;
	unsigned byte_cnt = data->blocksize * data->blocks;
	unsigned *buff;
	unsigned timeout = 0xffffff;

	if (data->flags & MMC_DATA_READ) {
		buff = (unsigned int *)data->b.dest;
		for (i=0; i<(byte_cnt>>2); i++) {
			while(--timeout && (readl(&mmchost->reg->status)&(1 << 2)));
			if (timeout <= 0)
				goto out;
			buff[i] = readl(mmchost->database);
			timeout = 0xffffff;
		}
	} else {
		buff = (unsigned int *)data->b.src;
		for (i=0; i<(byte_cnt>>2); i++) {
			while(--timeout && (readl(&mmchost->reg->status)&(1 << 3)));
			if (timeout <= 0)
				goto out;
			writel(buff[i], mmchost->database);
			timeout = 0xffffff;
		}
	}

out:
	if (timeout <= 0) {
		mmcinfo("transfer by cpu failed\n");
		return -1;
	}

	return 0;
}

static int mmc_trans_data_by_dma(struct mmc *mmc, struct mmc_data *data)
{
	struct sunxi_mmc_host* mmchost = (struct sunxi_mmc_host *)mmc->priv;
	struct sunxi_mmc_des *pdes = mmchost->pdes;
	unsigned byte_cnt = data->blocksize * data->blocks;
	unsigned char *buff;
	unsigned des_idx = 0;
	unsigned buff_frag_num = 0;
	unsigned remain;
	unsigned i, rval;

	buff = data->flags & MMC_DATA_READ ?
			(unsigned char *)data->b.dest : (unsigned char *)data->b.src;
	buff_frag_num = byte_cnt >> SDXC_DES_NUM_SHIFT;
	remain = byte_cnt & (SDXC_DES_BUFFER_MAX_LEN-1);
	if (remain)
		buff_frag_num ++;
	else
		remain = SDXC_DES_BUFFER_MAX_LEN;

	wlibc_CleanFlushDCacheRegion(buff, (unsigned long)byte_cnt);
//OSAL_CacheRangeFlush(buff, (unsigned long)byte_cnt, CACHE_CLEAN_FLUSH_D_CACHE_REGION);
	for (i=0; i < buff_frag_num; i++, des_idx++) {
		memset((void*)&pdes[des_idx], 0, sizeof(struct sunxi_mmc_des));
		pdes[des_idx].des_chain = 1;
		pdes[des_idx].own = 1;
		pdes[des_idx].dic = 1;
		if (buff_frag_num > 1 && i != buff_frag_num-1) {
			pdes[des_idx].data_buf1_sz = SDXC_DES_BUFFER_MAX_LEN;
		} else
			pdes[des_idx].data_buf1_sz = remain;

		pdes[des_idx].buf_addr_ptr1 = (u32)buff + i * SDXC_DES_BUFFER_MAX_LEN;
		if (i==0)
			pdes[des_idx].first_des = 1;

		if (i == buff_frag_num-1) {
			pdes[des_idx].dic = 0;
			pdes[des_idx].last_des = 1;
			pdes[des_idx].end_of_ring = 1;
			pdes[des_idx].buf_addr_ptr2 = 0;
		} else {
			pdes[des_idx].buf_addr_ptr2 = (u32)&pdes[des_idx+1];
		}
		mmcdbg("frag %d, remain %d, des[%d](%x): "
			"[0] = %x, [1] = %x, [2] = %x, [3] = %x\n",
			i, remain, des_idx, (u32)&pdes[des_idx],
			(u32)((u32*)&pdes[des_idx])[0], (u32)((u32*)&pdes[des_idx])[1],
			(u32)((u32*)&pdes[des_idx])[2], (u32)((u32*)&pdes[des_idx])[3]);
	}
	wlibc_CleanFlushDCacheRegion(pdes, sizeof(struct sunxi_mmc_des) * (des_idx+1));
//	OSAL_CacheRangeFlush(pdes, sizeof(struct sunxi_mmc_des) * (des_idx+1), CACHE_CLEAN_FLUSH_D_CACHE_REGION);

	/*
	 * GCTRLREG
	 * GCTRL[2]	: DMA reset
	 * GCTRL[5]	: DMA enable
	 *
	 * IDMACREG
	 * IDMAC[0]	: IDMA soft reset
	 * IDMAC[1]	: IDMA fix burst flag
	 * IDMAC[7]	: IDMA on
	 *
	 * IDIECREG
	 * IDIE[0]	: IDMA transmit interrupt flag
	 * IDIE[1]	: IDMA receive interrupt flag
	 */
	rval = readl(&mmchost->reg->gctrl);
	writel(rval|(1 << 5)|(1 << 2), &mmchost->reg->gctrl);	/* dma enable */
	writel((1 << 0), &mmchost->reg->dmac); /* idma reset */
	writel((1 << 1) | (1 << 7), &mmchost->reg->dmac); /* idma on */
	rval = readl(&mmchost->reg->idie) & (~3);
	if (data->flags & MMC_DATA_WRITE)
		rval |= (1 << 0);
	else
		rval |= (1 << 1);
	writel(rval, &mmchost->reg->idie);
	writel((u32)pdes, &mmchost->reg->dlba);
	writel((2U<<28)|(7<<16)|8, &mmchost->reg->ftrglevel);

	return 0;
}

static int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
			struct mmc_data *data)
{
	struct sunxi_mmc_host* mmchost = (struct sunxi_mmc_host *)mmc->priv;
	unsigned int cmdval = 0x80000000;
	signed int timeout = 0;
	int error = 0;
	unsigned int status = 0;
	unsigned int usedma = 0;
	unsigned int bytecnt = 0;

	if (mmchost->fatal_err) {
		mmcinfo("Found fatal err,so no send cmd\n");
		return -1;
	}
	if (cmd->resp_type & MMC_RSP_BUSY)
		mmcdbg("mmc cmd %d check rsp busy\n", cmd->cmdidx);
	if (cmd->cmdidx == 12) {
		timeout = 0x4ffffff;
		//mmcinfo("c12b");
		do {
			status = readl(&mmchost->reg->status);
			if (!timeout--) {
				error = -1;
				mmcinfo("cmd12 check busy timeout\n");
				goto out;
			}
			//mmcinfo("check busy\n");
		} while (status & (1 << 9));
		return 0;
	}
	/*
	 * CMDREG
	 * CMD[5:0]	: Command index
	 * CMD[6]	: Has response
	 * CMD[7]	: Long response
	 * CMD[8]	: Check response CRC
	 * CMD[9]	: Has data
	 * CMD[10]	: Write
	 * CMD[11]	: Steam mode
	 * CMD[12]	: Auto stop
	 * CMD[13]	: Wait previous over
	 * CMD[14]	: About cmd
	 * CMD[15]	: Send initialization
	 * CMD[21]	: Update clock
	 * CMD[31]	: Load cmd
	 */
	if (!cmd->cmdidx)
		cmdval |= (1 << 15);
	if (cmd->resp_type & MMC_RSP_PRESENT)
		cmdval |= (1 << 6);
	if (cmd->resp_type & MMC_RSP_136)
		cmdval |= (1 << 7);
	if (cmd->resp_type & MMC_RSP_CRC)
		cmdval |= (1 << 8);
	if (data) {
		if ((u32)data->b.dest & 0x3) {
			mmcinfo("dest is not 4 byte align\n");
			error = -1;
			goto out;
		}

		cmdval |= (1 << 9) | (1 << 13);
		if (data->flags & MMC_DATA_WRITE)
			cmdval |= (1 << 10);
		if (data->blocks > 1)
			cmdval |= (1 << 12);
		writel(data->blocksize, &mmchost->reg->blksz);
		writel(data->blocks * data->blocksize, &mmchost->reg->bytecnt);
	}

	mmcdbg("mmc %d, cmd %d(0x%x), arg 0x%x\n", mmchost->mmc_no, cmd->cmdidx, cmdval|cmd->cmdidx, cmd->cmdarg);
	writel(cmd->cmdarg, &mmchost->reg->arg);
	if (!data)
		writel(cmdval|cmd->cmdidx, &mmchost->reg->cmd);

	/*
	 * transfer data and check status
	 * STATREG[2] : FIFO empty
	 * STATREG[3] : FIFO full
	 */
	if (data) {
		int ret = 0;
		bytecnt = data->blocksize * data->blocks;
		mmcdbg("trans data %d bytes\n", bytecnt);
#ifdef MMC_TRANS_BY_DMA
		if (bytecnt > 64) {
#else
		if (0) {
#endif
			usedma = 1;
			writel(readl(&mmchost->reg->gctrl)&(~0x80000000), &mmchost->reg->gctrl);
			ret = mmc_trans_data_by_dma(mmc, data);
			writel(cmdval|cmd->cmdidx, &mmchost->reg->cmd);
		} else {
			writel(readl(&mmchost->reg->gctrl)|0x80000000, &mmchost->reg->gctrl);
			writel(cmdval|cmd->cmdidx, &mmchost->reg->cmd);
			ret = mmc_trans_data_by_cpu(mmc, data);
		}
		if (ret) {
			mmcinfo("Transfer failed\n");
			error = readl(&mmchost->reg->rint) & 0xbbc2;
			if(!error)
				error = 0xffffffff;
			goto out;
		}
	}

	timeout = 0xffffff;
	do {
		status = readl(&mmchost->reg->rint);
		if (!timeout-- || (status & 0xbbc2)) {
			error = status & 0xbbc2;
			if(!error)
				error = 0xffffffff;
			mmcinfo("cmd %d timeout, err %x\n", cmd->cmdidx, error);
			//dumphex32("mmc", (char*)mmchost->reg, 0x100);
			goto out;
		}
	} while (!(status&0x4));

	if (data) {
		unsigned done = 0;
		timeout = usedma ? 0xffff*bytecnt : 0xffff;
		if(timeout<=0)
			timeout = 0x7fffffff;
		do {
			status = readl(&mmchost->reg->rint);
			if (!timeout-- || (status & 0xbbc2)) {
				error = status & 0xbbc2;
				if(!error)
					error = 0xffffffff;//represet software timeout
				mmcinfo("data timeout, err %x\n", error);
				goto out;
			}
			if (data->blocks > 1)
				done = status & (1 << 14);
			else
				done = status & (1 << 3);
		} while (!done);
	}

	if (cmd->resp_type & MMC_RSP_BUSY) {
		timeout = 0x4ffffff;
		do {
			status = readl(&mmchost->reg->status);
			if (!timeout--) {
				error = -1;
				mmcinfo("busy timeout\n");
				goto out;
			}
		} while (status & (1 << 9));
	}
	if (cmd->resp_type & MMC_RSP_136) {
		cmd->response[0] = readl(&mmchost->reg->resp3);
		cmd->response[1] = readl(&mmchost->reg->resp2);
		cmd->response[2] = readl(&mmchost->reg->resp1);
		cmd->response[3] = readl(&mmchost->reg->resp0);
		mmcdbg("mmc resp 0x%x 0x%x 0x%x 0x%x\n",
			cmd->response[3], cmd->response[2],
			cmd->response[1], cmd->response[0]);
	} else {
		cmd->response[0] = readl(&mmchost->reg->resp0);
		mmcdbg("mmc resp 0x%x\n", cmd->response[0]);
	}
out:
	if (data && usedma) {
	/* IDMASTAREG
	 * IDST[0] : idma tx int
	 * IDST[1] : idma rx int
	 * IDST[2] : idma fatal bus error
	 * IDST[4] : idma descriptor invalid
	 * IDST[5] : idma error summary
	 * IDST[8] : idma normal interrupt sumary
	 * IDST[9] : idma abnormal interrupt sumary
	 */
		status = readl(&mmchost->reg->idst);


		writel(status, &mmchost->reg->idst);
        writel(0, &mmchost->reg->idie);
        writel(0, &mmchost->reg->dmac);
        writel(readl(&mmchost->reg->gctrl)&(~(1 << 5)), &mmchost->reg->gctrl);
	}
	if (error) {
		dumphex32("mmc", (char*)mmchost->reg, 0x100);
		writel(0x7, &mmchost->reg->gctrl);
		mmc_update_clk(mmc);
		mmcinfo("mmc cmd %d err %x\n", cmd->cmdidx, error);
	}
	writel(0xffffffff, &mmchost->reg->rint);

	if (error)
		return -1;
	else
		return 0;
}

int sunxi_mmc_init(int sdc_no, unsigned bus_width)
{
	struct mmc *mmc;
	int ret;

	memset(&mmc_dev[sdc_no], 0, sizeof(struct mmc));
	memset(&mmc_host[sdc_no], 0, sizeof(struct sunxi_mmc_host));
	mmc = &mmc_dev[sdc_no];

	strcpy(mmc->name, "SUNXI SD/MMC");
	mmc->priv = &mmc_host[sdc_no];
	mmc->send_cmd = mmc_send_cmd;
	mmc->set_ios = mmc_set_ios;
	mmc->init = mmc_core_init;

	mmc->voltages = MMC_VDD_29_30|MMC_VDD_30_31|MMC_VDD_31_32|MMC_VDD_32_33|
	                MMC_VDD_33_34|MMC_VDD_34_35|MMC_VDD_35_36;
	mmc->host_caps = MMC_MODE_HS_52MHz|MMC_MODE_HS|MMC_MODE_HC;
	if (bus_width==4)
		mmc->host_caps |= MMC_MODE_4BIT;

	mmc->f_min = 300000;
	mmc->f_max = 52000000;

    mmc_host[sdc_no].pdes = (struct sunxi_mmc_des*)0x42000000;
	if (mmc_resource_init(sdc_no)) {
		mmcinfo("resource init failed\n");
		return -1;
	}

	mmc_clk_io_onoff(sdc_no, 1);
	ret = mmc_register(sdc_no, mmc);
	if (ret < 0) {
		mmcinfo("mmc register failed\n");
		return -1;
	}

	return mmc->lba;
}

int sunxi_mmc_exit(int sdc_no)
{
	mmc_clk_io_onoff(sdc_no, 0);
	mmc_unregister(sdc_no);
	memset(&mmc_dev[sdc_no], 0, sizeof(struct mmc));
	memset(&mmc_host[sdc_no], 0, sizeof(struct sunxi_mmc_host));

	mmcdbg("sunxi mmc%d exit\n",sdc_no);
	return 0;
}
