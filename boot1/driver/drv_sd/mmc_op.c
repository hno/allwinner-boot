/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * Description: MMC driver for general mmc operations
 * Author: Aaron <leafy.myeh@allwinnertech.com>
 * Date: 2012-2-3 14:18:18
 */
#include "mmc_def.h"
#include "mmc.h"

#define EMMC_BOOT_START_BLOCK (0)
#define SD_BOOT_START_BLOCK (16)

//#define NO_USE_BOOT0

static unsigned bootcard_offset;

extern int sunxi_mmc_init(int sdc_no, unsigned bus_width);
extern int sunxi_mmc_exit(int sdc_no);

__s32 SDMMC_PhyInit(__u32 card_no, __u32 bus_width)
{
	int ret = 0;
	struct mmc *mmc = NULL;

	mmcdbg("card%d SDMMC_PhyInit start\n",card_no);

	ret = sunxi_mmc_init(card_no, bus_width);
	if ( ret <= 0) {
		mmcinfo("SDMMC_PhyInit:Init SD/MMC card failed !\n");
		return -1;
	}
	mmcdbg("SDMMC_PhyInit:Phy Init SD/MMC card succeed!\n");

#ifndef NO_USE_BOOT0
	mmc = find_mmc_device(card_no);
	if (!(mmc->part_support & PART_SUPPORT)) {
		mmcinfo("Card doesn't support part_switch\n");
		mmcdbg("card%d SDMMC_PhyInit end\n",card_no);
		return 0;
	}
#else
	return 0;
#endif

	ret = mmc_switch_boot_bus_cond(card_no, MMC_SWITCH_BOOT_SDR_NORMAL, MMC_SWITCH_BOOT_RST_BUS_COND, MMC_SWITCH_BOOT_BUS_SDRx4_DDRx4);
	if (ret) {
		mmcinfo("SDMMC_PhyInit:mmc switch boot cond failed\n");
		ret = -1;
		goto out;
	}
	mmcdbg("SDMMC_PhyInit:mmc switch boot cond succeed\n");

	ret = mmc_switch_boot_part(card_no, MMC_SWITCH_PART_BOOT_ACK_ENB, MMC_SWITCH_PART_BOOT_PART_1);	//正常模式的配置
	if (ret) {
		mmcinfo("SDMMC_PhyInit:mmc switch boot part failed\n");
		ret = -1;
		goto out;
	}
	mmcdbg("SDMMC_PhyInit:mmc switch boot part succeed\n");

	ret = mmc_switch_part(card_no, MMC_SWITCH_PART_BOOT_PART_1);
	if (ret) {
		mmcinfo("SDMMC_PhyInit:mmc switch access boot0 failed\n");
		ret = -1;
		goto out;
	}

	mmcdbg("SDMMC_PhyInit:mmc switch access boot0 succeed\n");

	mmc->part_num = BOOT0_PART;//boot0
	mmc->lba = mmc->boot0_lba;
	mmcdbg("SDMMC_PhyInit:part num:%d\n",mmc->part_num);
	mmcdbg("card%d SDMMC_PhyInit end\n",card_no);
out:	return ret;
}

__s32 SDMMC_PhyExit(__u32 card_no)
{
	mmcdbg("SDMMC_PhyExit start\n");
	sunxi_mmc_exit(card_no);
	mmcdbg("SDMMC_PhyExit end\n");
    return 0;
}

__s32 SDMMC_PhyRead(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	int ret = 0;
	mmcdbg("SDMMC_PhyRead start\n");

#ifndef NO_USE_BOOT0
	if (!(mmc->part_support & PART_SUPPORT))
#endif
	{
		mmcdbg("Card doesn't support part_switch\n");
		ret = mmc_bread(card_no, start_sector, nsector, buf);
		if(!ret)
			ret = -1;
		if(ret == -1)
			mmcinfo("1.SDMMC_PhyRead failed\n");
		mmcdbg("SDMMC_PhyRead end\n");
		return ret;
	}

	if(mmc->part_num!=BOOT0_PART){
		mmcinfo("SDMMC_PhyRead part_num%d should be %d\n",mmc->part_num,BOOT0_PART);
		return -1;//
	}

	//eMMC boot from boot0 0 sector
	if(start_sector<(EMMC_BOOT_START_BLOCK+SD_BOOT_START_BLOCK)){
		mmcinfo("SDMMC_PhyRead wrong start_sector%d\n",start_sector);
		return -1;//
	}

	ret = mmc_bread(card_no, start_sector-SD_BOOT_START_BLOCK, nsector, buf);
	if(!ret)
		ret = -1;
	if(ret == -1)
		mmcinfo("2.SDMMC_PhyRead failed\n");
	mmcdbg("SDMMC_PhyRead end\n");
	return ret;
}

__s32 SDMMC_PhyWrite(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	int ret = 0;
	mmcdbg("SDMMC_PhyWrite start\n");

#ifndef NO_USE_BOOT0
	if (!(mmc->part_support & PART_SUPPORT))
#endif
	{
		mmcdbg("Card doesn't support part_switch\n");
		ret = mmc_bwrite(card_no, start_sector, nsector, buf);
		if(!ret)
			ret = -1;
		if(ret==-1)
			mmcinfo("1.SDMMC_PhyWrite failed\n");
		mmcdbg("SDMMC_PhyWrite end\n");
		return ret;
	}


	if(mmc->part_num!=BOOT0_PART){
		mmcinfo("SDMMC_PhyWrite part_num%d should be %d\n",mmc->part_num,BOOT0_PART);
		return -1;//
	}

	//eMMC boot from boot0 0 sector
	if(start_sector<(EMMC_BOOT_START_BLOCK+SD_BOOT_START_BLOCK)){
		mmcinfo("SDMMC_PhyWrite wrong start_sector%d\n",start_sector);
		return -1;//
	}

	ret = mmc_bwrite(card_no, start_sector-SD_BOOT_START_BLOCK, nsector, buf);
	if(!ret)
		ret = -1;
	if(ret==-1)
		mmcinfo("2.SDMMC_PhyWrite failed\n");
	mmcdbg("SDMMC_PhyWrite end\n");
	return ret;
}

__s32 SDMMC_PhyDiskSize(__u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	mmcdbg("SDMMC_PhyDiskSize start\n");

#ifndef NO_USE_BOOT0
	if (!(mmc->part_support & PART_SUPPORT))
#endif
	{
		mmcdbg("Card doesn't support part_switch\n");
		mmc->lba = mmc->user_lba;
		mmcdbg("SDMMC_PhyDiskSize end\n");
		return mmc->lba;;
	}


	if(mmc->part_num!=BOOT0_PART){
		mmcinfo("SDMMC_PhyDiskSize part_num%d should be %d\n",mmc->part_num,BOOT0_PART);
		return -1;//
	}

	mmc->lba = mmc->boot0_lba;
	mmcdbg("SDMMC_PhyDiskSize end\n");
	return mmc->lba;
}

__s32 SDMMC_PhyErase(__u32 block, __u32 nblock, __u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	int ret = 0;
	mmcdbg("card%d SDMMC_PhyErase start\n",card_no);


	if(mmc==NULL){
		mmcinfo("SDMMC_PhyErase mmc should not be NULL\n");
		return -1;
	}

#ifndef NO_USE_BOOT0
	if (!(mmc->part_support & PART_SUPPORT))
#endif
	{
		mmcdbg("Card doesn't support part_switch\n");
		ret = mmc_berase(card_no, block, nblock);
		if(!ret)
			ret = -1;
		if(ret == -1)
			mmcinfo("1.SDMMC_PhyErase failed\n");
		mmcdbg("SDMMC_PhyErase end\n");
		return ret;
	}


	if(mmc->part_num!=BOOT0_PART){
		mmcinfo("SDMMC_PhyErase part_num%d should be %d",mmc->part_num,BOOT0_PART);
		return -1;//
	}

	//eMMC boot from boot0 0 sector
	if(block<(EMMC_BOOT_START_BLOCK+SD_BOOT_START_BLOCK)){
		mmcinfo("SDMMC_PhyErase wrong start_sector%d\n",block);
		return -1;//
	}

	ret = mmc_berase(card_no, block-SD_BOOT_START_BLOCK, nblock);
	if(!ret)
		ret = -1;
	if(ret == -1)
		mmcinfo("2.SDMMC_PhyErase failed\n");
	mmcdbg("card%d SDMMC_PhyErase end\n",card_no);
	return ret;
}

__s32 SDMMC_LogicalInit(__u32 card_no, __u32 card_offset, __u32 bus_width)
{
	__s32 ret = 0;
	//struct mmc *mmc = find_mmc_device(card_no);
	mmcdbg("SDMMC_LogicalInit start\n");

	bootcard_offset = card_offset;


	ret = sunxi_mmc_init(card_no, bus_width);
	if ( ret <= 0) {
		mmcinfo("SDMMC_PhyInit:Init SD/MMC card failed !\n");
		return -1;
	}
	mmcdbg("SDMMC_LogicalInit:mmc switch access part user succeed\n");
	mmcdbg("SDMMC_LogicalInit end\n");
	return ret;
}

__s32 SDMMC_LogicalExit(__u32 card_no)
{
	int ret = 0;
	bootcard_offset = 0;
	mmcdbg("SDMMC_LogicalExit start\n");
	ret = SDMMC_PhyExit(card_no);
	if(ret)
		mmcinfo("LogicalExit failed\n");
  mmcdbg("SDMMC_LogicalExit end\n");
  return ret;
}

__s32 SDMMC_LogicalRead(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	int ret = 0;
	mmcdbg("SDMMC_LogicalRead start\n");
	if(mmc->part_num!=USER_PART){
		mmcinfo("SDMMC_LogicalRead part_num%d should be %d",mmc->part_num,USER_PART);
		return -1;//
	}
	ret = mmc_bread(card_no, start_sector + bootcard_offset, nsector, buf);
	if(!ret)
		ret = -1;
	 if(ret == -1)
		mmcinfo("LogicalRead failed\n");
	mmcdbg("SDMMC_LogicalRead end\n");
	return ret;
}

__s32 SDMMC_LogicalRead1(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	int ret = 0;
	mmcdbg("SDMMC_LogicalRead start\n");
	if(mmc->part_num!=USER_PART){
		mmcinfo("SDMMC_LogicalRead part_num%d should be %d",mmc->part_num,USER_PART);
		return -1;//
	}
	ret = mmc_bread(card_no, start_sector, nsector, buf);
	if(!ret)
		ret = -1;
	 if(ret == -1)
		mmcinfo("LogicalRead failed\n");
	mmcdbg("SDMMC_LogicalRead end\n");
	return ret;
}

__s32 SDMMC_LogicalWrite(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	int ret = 0;
	mmcdbg("SDMMC_LogicalWrite start\n");
	if(mmc->part_num!=USER_PART){
		mmcinfo("SDMMC_LogicalWrite part_num%d should be %d",mmc->part_num,USER_PART);
		return -1;//
	}
	ret = mmc_bwrite(card_no, start_sector + bootcard_offset, nsector, buf);
	mmcdbg("SDMMC_LogicalWrite end\n");
	return ret;
}

__s32 SDMMC_LogicalWrite1(__u32 start_sector, __u32 nsector, void *buf, __u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	int ret = 0;
	mmcdbg("SDMMC_LogicalWrite start\n");
	if(mmc->part_num!=USER_PART){
		mmcinfo("SDMMC_LogicalWrite part_num%d should be %d",mmc->part_num,USER_PART);
		return -1;//
	}
	ret = mmc_bwrite(card_no, start_sector, nsector, buf);
	mmcdbg("SDMMC_LogicalWrite end\n");
	return ret;
}

__s32 SDMMC_LogicalDiskSize(__u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	mmcdbg("SDMMC_LogicalDiskSize start\n");
	if(mmc->part_num!=USER_PART){
		mmcinfo("SDMMC_LogicalDiskSize part_num%d should be %d",mmc->part_num,USER_PART);
		return -1;
	}

	mmc->lba = mmc->user_lba;
	mmcdbg("SDMMC_LogicalDiskSize end\n");
	return mmc->lba-bootcard_offset;
}

__s32 SDMMC_LogicaErase(__u32 block, __u32 nblock, __u32 card_no)
{
	struct mmc *mmc = find_mmc_device(card_no);
	int ret = 0;
	mmcdbg("card%d SDMMC_LogicaErase start\n",card_no);
	if(mmc->part_num!=USER_PART){
		mmcinfo("SDMMC_LogicaErase part_num%d should be %d",mmc->part_num,USER_PART);
		return -1;
	}
	ret = mmc_berase(card_no, block + bootcard_offset, nblock);
	if(!ret)
		ret = -1;
	 if(ret == -1)
		mmcinfo("LogicaErase failed\n");
	mmcdbg("card%d SDMMC_LogicaErase end\n",card_no);
	return ret;
}
