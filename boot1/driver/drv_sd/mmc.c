/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: MMC driver for General mmc operations
 * Author: Aaron <leafy.myeh@allwinnertech.com>
 * Date: 2012-2-3 14:18:18
 */
#include "mmc_def.h"
#include "mmc.h"


/* Set block count limit because of 16 bit register limit on some hardware*/
#ifndef CONFIG_SYS_MMC_MAX_BLK_COUNT
#define CONFIG_SYS_MMC_MAX_BLK_COUNT 65535
#endif

static struct mmc* mmc_devices[MAX_MMC_NUM];

static void forloop(unsigned n)
{
	while(n--);
}

static void udelay(unsigned us)
{
	while (us--)
		forloop(100);
}

int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	return mmc->send_cmd(mmc, cmd, data);
}

int mmc_send_status(struct mmc *mmc, int timeout)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = MMC_CMD_SEND_STATUS;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	do {
		err = mmc_send_cmd(mmc, &cmd, NULL);
		if (err) {
			mmcinfo("Send status failed\n");
			return err;
		}
		else if (cmd.response[0] & MMC_STATUS_RDY_FOR_DATA)
			break;

		udelay(10000);

		if (cmd.response[0] & MMC_STATUS_MASK) {
			mmcinfo("Status Error: 0x%08X\n", cmd.response[0]);
			return COMM_ERR;
		}
	} while (--timeout);

	if (timeout <= 0) {
		mmcinfo("Timeout waiting card ready\n");
		return TIMEOUT;
	}

	return 0;
}

int mmc_set_blocklen(struct mmc *mmc, int len)
{
	struct mmc_cmd cmd;

	cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = len;
	cmd.flags = 0;

	return mmc_send_cmd(mmc, &cmd, NULL);
}

struct mmc *find_mmc_device(int dev_num)
{
	if (mmc_devices[dev_num] != NULL)
		return mmc_devices[dev_num];

	mmcinfo("MMC Device %d not found\n", dev_num);

	return NULL;
}

static unsigned long mmc_erase_t(struct mmc *mmc, unsigned long start, unsigned blkcnt)
{
	struct mmc_cmd cmd;
	unsigned long end;
	int err, start_cmd, end_cmd;

	if (mmc->high_capacity)
		end = start + blkcnt - 1;
	else {
		end = (start + blkcnt - 1) * mmc->write_bl_len;
		start *= mmc->write_bl_len;
	}

	if (IS_SD(mmc)) {
		start_cmd = SD_CMD_ERASE_WR_BLK_START;
		end_cmd = SD_CMD_ERASE_WR_BLK_END;
	} else {
		start_cmd = MMC_CMD_ERASE_GROUP_START;
		end_cmd = MMC_CMD_ERASE_GROUP_END;
	}

	cmd.cmdidx = start_cmd;
	cmd.cmdarg = start;
	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = end_cmd;
	cmd.cmdarg = end;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = MMC_CMD_ERASE;
	cmd.cmdarg = SECURE_ERASE;
	cmd.resp_type = MMC_RSP_R1b;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	return 0;

err_out:
	mmcdbg("mmc erase failed\n");
	return err;
}

int
mmc_berase(int dev_num, unsigned long start, unsigned blkcnt)
{
	int err = 0;
	struct mmc *mmc = find_mmc_device(dev_num);
//	unsigned blk = 0, blk_r = 0;
	void* src = (void*)0x41000000;

	if (!mmc) {
		mmcinfo("Can not find mmc dev\n");
		return -1;
	}

	memset(src, 0, 512*blkcnt);
	mmcinfo("erase blk %d ~ %d\n", start, start + blkcnt - 1);
	err = mmc_bwrite(dev_num, start, blkcnt, src);
	if(!err) {
		mmcinfo("erase failed\n");
	}

	return err;
	/*
	if ((start % mmc->erase_grp_size) || (blkcnt % mmc->erase_grp_size))
		mmcdbg("\n\nCaution! Your devices Erase group is 0x%x\n"
			"The erase range would be change to 0x%x~0x%x\n\n",
		       mmc->erase_grp_size, start & ~(mmc->erase_grp_size - 1),
		       ((start + blkcnt + mmc->erase_grp_size)
		       & ~(mmc->erase_grp_size - 1)) - 1);

	while (blk < blkcnt) {
		blk_r = ((blkcnt - blk) > mmc->erase_grp_size) ?
			mmc->erase_grp_size : (blkcnt - blk);
		err = mmc_erase_t(mmc, start + blk, blk_r);
		if (err)
			break;

		blk += blk_r;
	}
	return blk;
	*/
}

static unsigned long
mmc_write_blocks(struct mmc *mmc, unsigned long start, unsigned blkcnt, const void*src)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int timeout = 1000;

	if ((start + blkcnt) > mmc->lba) {
		mmcinfo("MMC: block number 0x%lx exceeds max(0x%lx)\n",
			start + blkcnt, mmc->lba);
		return 0;
	}

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_WRITE_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->write_bl_len;

	if(cmd.cmdidx == MMC_CMD_WRITE_SINGLE_BLOCK) {
		cmd.resp_type = MMC_RSP_R1b;
	} else {
		cmd.resp_type = MMC_RSP_R1;
	}
	cmd.flags = 0;

	data.b.src = src;
	data.blocks = blkcnt;
	data.blocksize = mmc->write_bl_len;
	data.flags = MMC_DATA_WRITE;

	if (mmc_send_cmd(mmc, &cmd, &data)) {
		mmcinfo("mmc write failed\n");
		return 0;
	}

	/* SPI multiblock writes terminate using a special
	 * token, not a STOP_TRANSMISSION request.
	 */
	if (!mmc_host_is_spi(mmc) && blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			mmcinfo("mmc fail to send stop cmd\n");
			return 0;
		}
	}

	/* Waiting for the ready status */
	mmc_send_status(mmc, timeout);
	return blkcnt;
}

unsigned long
mmc_bwrite(int dev_num, unsigned long start, unsigned blkcnt, const void*src)
{
	unsigned cur, blocks_todo = blkcnt;

	struct mmc *mmc = find_mmc_device(dev_num);

	if (blkcnt == 0) {
		mmcinfo("blkcnt should not be 0\n");
		return 0;
	}
	if (!mmc) {
		mmcinfo("Can not found device\n");
		return 0;
	}

	if (mmc_set_blocklen(mmc, mmc->write_bl_len)) {
		mmcinfo("set block len failed\n");
		return 0;
	}

	do {
		cur = (blocks_todo > mmc->b_max) ?  mmc->b_max : blocks_todo;
		if(mmc_write_blocks(mmc, start, cur, src) != cur) {
			mmcinfo("write block failed\n");
			return 0;
		}
		blocks_todo -= cur;
		start += cur;
//		src += cur * mmc->write_bl_len;
		src = (char*)src + cur * mmc->write_bl_len;
	} while (blocks_todo > 0);

	return blkcnt;
}

int mmc_read_blocks(struct mmc *mmc, void *dst, unsigned long start, unsigned blkcnt)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int timeout = 1000;

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->read_bl_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.b.dest = dst;
	data.blocks = blkcnt;
	data.blocksize = mmc->read_bl_len;
	data.flags = MMC_DATA_READ;

	if (mmc_send_cmd(mmc, &cmd, &data)) {
		mmcinfo(" read blcok failed\n");
		return 0;
	}

	if (blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			mmcinfo("mmc fail to send stop cmd\n");
			return 0;
		}

		/* Waiting for the ready status */
		mmc_send_status(mmc, timeout);
	}

	return blkcnt;
}

unsigned long
mmc_bread(int dev_num, unsigned long start, unsigned blkcnt, void *dst)
{
	unsigned cur, blocks_todo = blkcnt;
	struct mmc *mmc = find_mmc_device(dev_num);

	if (blkcnt == 0) {
		mmcinfo("blkcnt should not be 0\n");
		return 0;
	}
	if (!mmc) {
		mmcinfo("Can not find mmc dev\n");
		return 0;
	}

	if ((start + blkcnt) > mmc->lba) {
		mmcinfo("MMC: block number 0x%x exceeds max(0x%x)\n",
			start + blkcnt, mmc->lba);
		return 0;
	}

	if (mmc_set_blocklen(mmc, mmc->read_bl_len)) {
		mmcinfo("Set block len failed\n");
		return 0;
	}

	do {
		cur = (blocks_todo > mmc->b_max) ?  mmc->b_max : blocks_todo;
		if(mmc_read_blocks(mmc, dst, start, cur) != cur) {
			mmcinfo("block read failed\n");
			return 0;
		}
		blocks_todo -= cur;
		start += cur;
//		dst += cur * mmc->read_bl_len;
		dst = (char*)dst + cur * mmc->read_bl_len;
	} while (blocks_todo > 0);

	return blkcnt;
}

int mmc_go_idle(struct mmc* mmc)
{
	struct mmc_cmd cmd;
	int err;

	udelay(1000);

	cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_NONE;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err) {
		mmcinfo("go idle failed\n");
		return err;
	}

	udelay(2000);

	return 0;
}

int
sd_send_op_cond(struct mmc *mmc)
{
	int timeout = 1000;
	int err;
	struct mmc_cmd cmd;

	do {
		cmd.cmdidx = MMC_CMD_APP_CMD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);
		if (err) {
				mmcinfo("send app cmd failed\n");
				return err;
		}

		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;

		/*
		 * Most cards do not answer if some reserved bits
		 * in the ocr are set. However, Some controller
		 * can set bit 7 (reserved for low voltages), but
		 * how to manage low voltages SD card is not yet
		 * specified.
		 */
		cmd.cmdarg = mmc_host_is_spi(mmc) ? 0 :
			(mmc->voltages & 0xff8000);

		if (mmc->version == SD_VERSION_2)
			cmd.cmdarg |= OCR_HCS;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err) {
			mmcinfo("send cmd41 failed\n");
			return err;
		}

		udelay(1000);
	} while ((!(cmd.response[0] & OCR_BUSY)) && timeout--);

	if (timeout <= 0){
		mmcinfo("wait card init failed\n");
		return UNUSABLE_ERR;
	}

	if (mmc->version != SD_VERSION_2)
		mmc->version = SD_VERSION_1_0;

	if (mmc_host_is_spi(mmc)) { /* read OCR for spi */
		cmd.cmdidx = MMC_CMD_SPI_READ_OCR;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);
		if (err) {
				mmcinfo("spi read ocr failed\n");
				return err;
		}
	}

	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 0;

	return 0;
}

int mmc_send_op_cond(struct mmc *mmc)
{
	int timeout = 10000;
	struct mmc_cmd cmd;
	int err;

	/* Some cards seem to need this */
	mmc_go_idle(mmc);

 	/* Asking to the card its capabilities */
 	cmd.cmdidx = MMC_CMD_SEND_OP_COND;
 	cmd.resp_type = MMC_RSP_R3;
 	cmd.cmdarg = 0;
 	cmd.flags = 0;

 	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err) {
		mmcinfo("mmc send op cond failed\n");
		return err;
	}

 	udelay(1000);

	do {
		cmd.cmdidx = MMC_CMD_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = (mmc_host_is_spi(mmc) ? 0 :
				(mmc->voltages &
				(cmd.response[0] & OCR_VOLTAGE_MASK)) |
				(cmd.response[0] & OCR_ACCESS_MODE));

		if (mmc->host_caps & MMC_MODE_HC)
			cmd.cmdarg |= OCR_HCS;

		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);
		if (err) {
				mmcinfo("mmc send op cond failed\n");
				return err;
		}

		udelay(1000);
	} while (!(cmd.response[0] & OCR_BUSY) && timeout--);

	if (timeout <= 0) {
		mmcinfo("wait for mmc init failed\n");
		return UNUSABLE_ERR;
	}

	if (mmc_host_is_spi(mmc)) { /* read OCR for spi */
		cmd.cmdidx = MMC_CMD_SPI_READ_OCR;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);
		if (err)
			return err;
	}

	mmc->version = MMC_VERSION_UNKNOWN;
	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 1;

	return 0;
}


int mmc_send_ext_csd(struct mmc *mmc, char *ext_csd)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int err;

	/* Get the Card Status Register */
	cmd.cmdidx = MMC_CMD_SEND_EXT_CSD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	data.b.dest = ext_csd;
	data.blocks = 1;
	data.blocksize = 512;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);
	if(err) {
		mmcinfo("mmc send ext csd failed\n");
	}

	return err;
}


int mmc_switch(struct mmc *mmc, u8 set, u8 index, u8 value)
{
	struct mmc_cmd cmd;
	int timeout = 1000;
	int ret;

	cmd.cmdidx = MMC_CMD_SWITCH;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
				 (index << 16) |
				 (value << 8);
	cmd.flags = 0;

	ret = mmc_send_cmd(mmc, &cmd, NULL);
	if(ret) {
		mmcinfo("mmc switch failed\n");
	}


	/* Waiting for the ready status */
	mmc_send_status(mmc, timeout);

	return ret;

}

int mmc_change_freq(struct mmc *mmc)
{
	char ext_csd[512];
	char cardtype;
	int err;
	int retry = 10;

	mmc->card_caps = 0;

	if (mmc_host_is_spi(mmc))
		return 0;

	/* Only version 4 supports high-speed */
	if (mmc->version < MMC_VERSION_4)
		return 0;

	mmc->card_caps |= MMC_MODE_4BIT;

	err = mmc_send_ext_csd(mmc, ext_csd);
	if (err) {
		mmcinfo("mmc get ext csd failed\n");
		return err;
	}

	cardtype = ext_csd[196] & 0xf;

		//retry for Toshiba emmc。for the first time Toshiba emmc change to HS，
	//it will return response crc err，so retry。
	do{
		err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);
		if(!err) {
			break;
		}
		mmcinfo("retry mmc switch(cmd6)\n");
	}while(retry--);

	if (err) {
		mmcinfo("mmc change to hs failed\n");
		return err;
	}

	/* Now check to see that it worked */
	err = mmc_send_ext_csd(mmc, ext_csd);

	if (err) {
		mmcinfo("send ext csd faild\n");
		return err;
	}

	/* No high-speed support */
	if (!ext_csd[185])
		return 0;

	/* High Speed is set, there are two types: 52MHz and 26MHz */
	if (cardtype & MMC_HS_52MHZ)
		mmc->card_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
	else
		mmc->card_caps |= MMC_MODE_HS;

	return 0;
}


int mmc_switch_boot_bus_cond(int dev_num, u32 boot_mode, u32 rst_bus_cond, u32 bus_width)
{
	char ext_csd[512]={0};
	unsigned char boot_bus_cond = 0;
	int ret = 0;
	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc) {
		mmcinfo("can not find mmc device\n");
		return -1;
	}


	boot_bus_cond = (mmc->boot_bus_cond &
			  	(~MMC_SWITCH_BOOT_MODE_MASK) & (~MMC_SWITCH_BOOT_RST_BUS_COND_MASK) & (~MMC_SWITCH_BOOT_BUS_WIDTH_MASK))
			  | ((boot_mode << 3) & MMC_SWITCH_BOOT_MODE_MASK)
			  | ((rst_bus_cond << 2) & MMC_SWITCH_BOOT_RST_BUS_COND_MASK)
			  | ((bus_width) & MMC_SWITCH_BOOT_BUS_WIDTH_MASK);

	ret = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BOOT_BUS_COND,boot_bus_cond);
	if(ret){
		mmcinfo("switch bus cond failed\n");
		return -1;
	}
	ret = mmc_send_ext_csd(mmc, ext_csd);
	if(ret){
		mmcinfo("send ext csd failed\n");
		return -1;
	}
	mmcdbg("boot bus cond:0x%x\n",ext_csd[EXT_CSD_BOOT_BUS_COND]);
	if(boot_bus_cond!=ext_csd[EXT_CSD_BOOT_BUS_COND])	{
		mmcinfo("Set boot bus cond failed,now bus con is 0x%x\n",ext_csd[EXT_CSD_BOOT_BUS_COND]);
		return -1;
	}
	mmc->boot_bus_cond = boot_bus_cond;
	return ret;
}

int mmc_switch_boot_part(int dev_num, u32 boot_ack, u32 boot_part)
{
	char ext_csd[512]={0};
	unsigned char part_config = 0;
	int ret = 0;
	struct mmc *mmc = find_mmc_device(dev_num);
	if (!mmc)
		return -1;

	part_config = (mmc->part_config & (~MMC_SWITCH_PART_BOOT_PART_MASK) & (~MMC_SWITCH_PART_BOOT_ACK_MASK))
			  				| ((boot_part << 3) & MMC_SWITCH_PART_BOOT_PART_MASK) | (boot_ack << 6);
	ret = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONF,part_config);

	if(ret){
		mmcinfo("switch boot partd failed\n");
		return -1;
	}
	ret = mmc_send_ext_csd(mmc, ext_csd);
	if(ret){
		mmcinfo("send ext csd failed\n");
		return -1;
	}

	mmcdbg("part conf:0x%x\n",ext_csd[EXT_CSD_PART_CONF]);
	if(part_config!=ext_csd[EXT_CSD_PART_CONF])	{
		mmcinfo("switch boot part failed,now part conf is 0x%x\n",ext_csd[EXT_CSD_PART_CONF]);
		return -1;
	}
	mmc->part_config = part_config;
	return ret;
}



int mmc_switch_part(int dev_num, unsigned int part_num)
{
	char ext_csd[512]={0};
	unsigned char part_config = 0;
	int ret = 0;
	struct mmc *mmc = find_mmc_device(dev_num);

	if (!mmc)
		return -1;

	part_config = (mmc->part_config & ~PART_ACCESS_MASK)
			  					| (part_num & PART_ACCESS_MASK);
	ret = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONF,part_config);
	if(ret){
		mmcinfo("mmc switch part failed\n");
		return -1;
	}
	ret = mmc_send_ext_csd(mmc, ext_csd);
	if(ret){
		mmcinfo("send ext csd failed\n");
		return -1;
	}
	mmcdbg("part conf:0x%x\n",ext_csd[EXT_CSD_PART_CONF]);
	if(part_config!=ext_csd[EXT_CSD_PART_CONF])	{
		mmcinfo("switch boot part failed,now bus con is 0x%x\n",ext_csd[EXT_CSD_PART_CONF]);
		return -1;
	}
	mmc->part_config = part_config;
	return ret;

}


int sd_switch(struct mmc *mmc, int mode, int group, u8 value, u8 *resp)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	/* Switch the frequency */
	cmd.cmdidx = SD_CMD_SWITCH_FUNC;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = (mode << 31) | 0xffffff;
	cmd.cmdarg &= ~(0xf << (group * 4));
	cmd.cmdarg |= value << (group * 4);
	cmd.flags = 0;

	data.b.dest = (char *)resp;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	return mmc_send_cmd(mmc, &cmd, &data);
}


int sd_change_freq(struct mmc *mmc)
{
	int err;
	struct mmc_cmd cmd;
	u32 scr[2];
	u32 switch_status[16];
	struct mmc_data data;
	int timeout;

	mmc->card_caps = 0;

	if (mmc_host_is_spi(mmc))
		return 0;

	/* Read the SCR to find out if this card supports higher speeds */
	cmd.cmdidx = MMC_CMD_APP_CMD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err) {
		mmcinfo("Send app cmd failed\n");
		return err;
	}

	cmd.cmdidx = SD_CMD_APP_SEND_SCR;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	timeout = 3;

retry_scr:
	data.b.dest = (char *)&scr;
	data.blocksize = 8;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	if (err) {
		if (timeout--)
			goto retry_scr;

		mmcinfo("Send scr failed\n");
		return err;
	}

	mmc->scr[0] = __be32_to_cpu(scr[0]);
	mmc->scr[1] = __be32_to_cpu(scr[1]);

	switch ((mmc->scr[0] >> 24) & 0xf) {
		case 0:
			mmc->version = SD_VERSION_1_0;
			break;
		case 1:
			mmc->version = SD_VERSION_1_10;
			break;
		case 2:
			mmc->version = SD_VERSION_2;
			break;
		default:
			mmc->version = SD_VERSION_1_0;
			break;
	}

	if (mmc->scr[0] & SD_DATA_4BIT)
		mmc->card_caps |= MMC_MODE_4BIT;

	/* Version 1.0 doesn't support switching */
	if (mmc->version == SD_VERSION_1_0)
		return 0;

	timeout = 4;
	while (timeout--) {
		err = sd_switch(mmc, SD_SWITCH_CHECK, 0, 1,
				(u8 *)&switch_status);

		if (err){
			mmcinfo("Check high speed status faild\n");
			return err;
		}

		/* The high-speed function is busy.  Try again */
		if (!(__be32_to_cpu(switch_status[7]) & SD_HIGHSPEED_BUSY))
			break;
	}

	/* If high-speed isn't supported, we return */
	if (!(__be32_to_cpu(switch_status[3]) & SD_HIGHSPEED_SUPPORTED))
		return 0;

	err = sd_switch(mmc, SD_SWITCH_SWITCH, 0, 1, (u8 *)&switch_status);

	if (err){
		mmcinfo("switch to high speed failed\n");
		return err;
	}

	if ((__be32_to_cpu(switch_status[4]) & 0x0f000000) == 0x01000000)
		mmc->card_caps |= MMC_MODE_HS;

	return 0;
}

/* frequency bases */
/* divided by 10 to be nice to platforms without floating point */
static const int fbase[] = {
	10000,
	100000,
	1000000,
	10000000,
};

/* Multiplier values for TRAN_SPEED.  Multiplied by 10 to be nice
 * to platforms without floating point.
 */
static const int multipliers[] = {
	0,	/* reserved */
	10,
	12,
	13,
	15,
	20,
	25,
	30,
	35,
	40,
	45,
	50,
	55,
	60,
	70,
	80,
};

void mmc_set_ios(struct mmc *mmc)
{
	mmc->set_ios(mmc);
}

void mmc_set_clock(struct mmc *mmc, u32 clock)
{
	if (clock > mmc->f_max)
		clock = mmc->f_max;

	if (clock < mmc->f_min)
		clock = mmc->f_min;

	mmc->clock = clock;

	mmc_set_ios(mmc);
}

void mmc_set_bus_width(struct mmc *mmc, u32 width)
{
	mmc->bus_width = width;

	mmc_set_ios(mmc);
}

int mmc_startup(struct mmc *mmc)
{
	int err;
	u32 mult, freq;
	unsigned long long cmult, csize, capacity;
	struct mmc_cmd cmd;
	char ext_csd[512];
	int timeout = 1000;

	/* Put the Card in Identify Mode */
	cmd.cmdidx = mmc_host_is_spi(mmc) ? MMC_CMD_SEND_CID :
		MMC_CMD_ALL_SEND_CID; /* cmd not supported in spi */
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err){
		mmcinfo("Put the Card in Identify Mode failed\n");
		return err;
	}

	memcpy(mmc->cid, cmd.response, 16);

	/*
	 * For MMC cards, set the Relative Address.
	 * For SD cards, get the Relatvie Address.
	 * This also puts the cards into Standby State
	 */
	if (!mmc_host_is_spi(mmc)) { /* cmd not supported in spi */
		cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
		cmd.cmdarg = mmc->rca << 16;
		cmd.resp_type = MMC_RSP_R6;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err) {
			mmcinfo("send rca failed\n");
			return err;
		}

		if (IS_SD(mmc))
			mmc->rca = (cmd.response[0] >> 16) & 0xffff;
	}

	/* Get the Card-Specific Data */
	cmd.cmdidx = MMC_CMD_SEND_CSD;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	/* Waiting for the ready status */
	mmc_send_status(mmc, timeout);

	if (err) {
		mmcinfo("get csd failed\n");
		return err;
	}

	mmc->csd[0] = cmd.response[0];
	mmc->csd[1] = cmd.response[1];
	mmc->csd[2] = cmd.response[2];
	mmc->csd[3] = cmd.response[3];

	if (mmc->version == MMC_VERSION_UNKNOWN) {
		int version = (cmd.response[0] >> 26) & 0xf;

		switch (version) {
			case 0:
				mmc->version = MMC_VERSION_1_2;
				break;
			case 1:
				mmc->version = MMC_VERSION_1_4;
				break;
			case 2:
				mmc->version = MMC_VERSION_2_2;
				break;
			case 3:
				mmc->version = MMC_VERSION_3;
				break;
			case 4:
				mmc->version = MMC_VERSION_4;
				break;
			default:
				mmc->version = MMC_VERSION_1_2;
				break;
		}
	}

	/* divide frequency by 10, since the mults are 10x bigger */
	freq = fbase[(cmd.response[0] & 0x7)];
	mult = multipliers[((cmd.response[0] >> 3) & 0xf)];

	mmc->tran_speed = freq * mult;

	mmc->read_bl_len = 1 << ((cmd.response[1] >> 16) & 0xf);

	if (IS_SD(mmc))
		mmc->write_bl_len = mmc->read_bl_len;
	else
		mmc->write_bl_len = 1 << ((cmd.response[3] >> 22) & 0xf);

	if (mmc->high_capacity) {
		csize = (mmc->csd[1] & 0x3f) << 16
			| (mmc->csd[2] & 0xffff0000) >> 16;
		cmult = 8;
	} else {
		csize = (mmc->csd[1] & 0x3ff) << 2
			| (mmc->csd[2] & 0xc0000000) >> 30;
		cmult = (mmc->csd[2] & 0x00038000) >> 15;
	}

	mmc->capacity = (csize + 1) << (cmult + 2);
	mmc->capacity *= mmc->read_bl_len;

	if (mmc->read_bl_len > 512)
		mmc->read_bl_len = 512;

	if (mmc->write_bl_len > 512)
		mmc->write_bl_len = 512;

	/* Select the card, and put it into Transfer Mode */
	if (!mmc_host_is_spi(mmc)) { /* cmd not supported in spi */
		cmd.cmdidx = MMC_CMD_SELECT_CARD;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.cmdarg = mmc->rca << 16;
		cmd.flags = 0;
		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err) {
			mmcinfo("Select the card failed\n");
			return err;
		}
	}

	/*
	 * For SD, its erase group is always one sector
	 */
	mmc->erase_grp_size = 1;
	mmc->part_config = MMCPART_NOAVAILABLE;
	if (!IS_SD(mmc) && (mmc->version >= MMC_VERSION_4)) {
		/* check  ext_csd version and capacity */
		err = mmc_send_ext_csd(mmc, ext_csd);
		if (!err & (ext_csd[192] >= 2)) {
			/*
			 * According to the JEDEC Standard, the value of
			 * ext_csd's capacity is valid if the value is more
			 * than 2GB
			 */
			capacity = ext_csd[212] << 0 | ext_csd[213] << 8 |
				   ext_csd[214] << 16 | ext_csd[215] << 24;
			capacity *= 512;
			if ((capacity >> 20) > 2 * 1024)
				mmc->capacity = capacity;
		}

		/*
		 * Check whether GROUP_DEF is set, if yes, read out
		 * group size from ext_csd directly, or calculate
		 * the group size from the csd value.
		 */
		if (ext_csd[175])
			mmc->erase_grp_size = ext_csd[224] * 512 * 1024;
		else {
			int erase_gsz, erase_gmul;
			erase_gsz = (mmc->csd[2] & 0x00007c00) >> 10;
			erase_gmul = (mmc->csd[2] & 0x000003e0) >> 5;
			mmc->erase_grp_size = (erase_gsz + 1)
				* (erase_gmul + 1);
		}

		/* store the partition info of emmc */
		if (ext_csd[160] & PART_SUPPORT){
			mmc->part_support = ext_csd[160];
			mmc->part_config = ext_csd[179];
			mmc->boot0_lba	= 128000*ext_csd[226]/512;
			mmc->boot1_lba	= 128000*ext_csd[226]/512;
			mmc->boot_bus_cond = ext_csd[177];
		}
	}

	if (IS_SD(mmc))
		err = sd_change_freq(mmc);
	else
		err = mmc_change_freq(mmc);

	if (err) {
		mmcinfo("Change speed mode failed\n");
		return err;
	}

	/* Restrict card's capabilities by what the host can do */
	mmc->card_caps &= mmc->host_caps;

	if (IS_SD(mmc)) {
		if (mmc->card_caps & MMC_MODE_4BIT) {
			cmd.cmdidx = MMC_CMD_APP_CMD;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmdarg = mmc->rca << 16;
			cmd.flags = 0;

			err = mmc_send_cmd(mmc, &cmd, NULL);
			if (err) {
				mmcinfo("send app cmd failed\n");
				return err;
			}

			cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmdarg = 2;
			cmd.flags = 0;
			err = mmc_send_cmd(mmc, &cmd, NULL);
			if (err) {
				mmcinfo("sd set bus width failed\n");
				return err;
			}

			mmc_set_bus_width(mmc, 4);
		}

		if (mmc->card_caps & MMC_MODE_HS)
			mmc_set_clock(mmc, 50000000);
		else
			mmc_set_clock(mmc, 25000000);
	} else {
		if (mmc->card_caps & MMC_MODE_4BIT) {
			/* Set the card to use 4 bit*/
			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH,
					EXT_CSD_BUS_WIDTH_4);

			if (err) {
				mmcinfo("mmc switch bus width failed\n");
				return err;
			}

			mmc_set_bus_width(mmc, 4);
		} else if (mmc->card_caps & MMC_MODE_8BIT) {
			/* Set the card to use 8 bit*/
			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH,
					EXT_CSD_BUS_WIDTH_8);

			if (err) {
				mmcinfo("mmc switch bus width8 failed\n");
				return err;
			}

			mmc_set_bus_width(mmc, 8);
		}

		if (mmc->card_caps & MMC_MODE_HS) {
			if (mmc->card_caps & MMC_MODE_HS_52MHz)
				mmc_set_clock(mmc, 52000000);
			else
				mmc_set_clock(mmc, 26000000);
		} else
			mmc_set_clock(mmc, 20000000);
	}

	/* fill in device description */
	mmc->blksz = mmc->read_bl_len;
	//mmc->lba = mmc->capacity/mmc->read_bl_len;
	mmc->lba = mmc->capacity/512;		//read_bl_len已经被限制到512，所以这里直接除以512
	mmc->user_lba = mmc->lba;

	mmcinfo("SD/MMC Card: %dbit, capacity: %dMB\n",
					mmc->card_caps & MMC_MODE_4BIT ? 4 : 1, mmc->lba >> 11);
	mmcinfo("vendor: Man %x Snr %x\n", (mmc->cid[0] >> 8) & 0xffffff,
					(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
	mmcinfo("product: %c%c%c%c%c\n", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
	mmcinfo("revision: %d.%d\n", mmc->cid[2] >> 28, (mmc->cid[2] >> 24) & 0xf);
	mmcinfo("boot0 capacity: %dKB,boot1 capacity: %dKB\n"
						,mmc->boot0_lba*512/1024,mmc->boot1_lba*512/1024);
	return 0;
}

int mmc_send_if_cond(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = SD_CMD_SEND_IF_COND;
	/* We set the bit if the host supports voltages between 2.7 and 3.6 V */
	cmd.cmdarg = ((mmc->voltages & 0xff8000) != 0) << 8 | 0xaa;
	cmd.resp_type = MMC_RSP_R7;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err) {
		mmcinfo("mmc send if cond failed\n");
		return err;
	}

	if ((cmd.response[0] & 0xff) != 0xaa)
		return UNUSABLE_ERR;
	else
		mmc->version = SD_VERSION_2;

	return 0;
}

int mmc_init(struct mmc *mmc)
{
	int err;

	if (mmc->has_init){
		mmcinfo("Has init\n");
		return 0;
	}

	err = mmc->init(mmc);
	if (err) {
		mmcinfo("mmc host init failed\n");
		return err;
	}

	mmc_set_bus_width(mmc, 1);
	mmc_set_clock(mmc, 1);

	/* Reset the Card */
	err = mmc_go_idle(mmc);
	if (err) {
		mmcinfo("reset card failed\n");
		return err;
	}

	/* The internal partition reset to user partition(0) at every CMD0*/
	mmc->part_num = 0;

	/* Test for SD version 2 */
	err = mmc_send_if_cond(mmc);

	/* Now try to get the SD card's operating condition */
	err = sd_send_op_cond(mmc);
	__inf("the first time to get sd card's operation condition\n");
	/* If the command timed out, we check for an MMC card */
	//if (err == TIMEOUT) {
	if(err) {
		err = mmc_send_op_cond(mmc);
		if(err) {
			mmcinfo("Card did not respond to voltage select!\n");
			return UNUSABLE_ERR;
		}
	}

	err = mmc_startup(mmc);
	if(err) {
		mmcinfo("mmc start up failed\n");
		mmc->has_init = 0;
	}	else {
		mmc->has_init = 1;
	}

	return err;
}

int mmc_register(int dev_num, struct mmc *mmc)
{
	mmc_devices[dev_num] = mmc;

	if (!mmc->b_max)
		mmc->b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT;

	return mmc_init(mmc);
}

int mmc_unregister(int dev_num)
{
	mmc_devices[dev_num] = NULL;
	mmcdbg("mmc%d unregister\n", dev_num);
	return 0;
}
