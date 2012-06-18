/*
************************************************************************************************************************
*                                                         eGON
*                                         the Embedded GO-ON Bootloader System
*
*                             Copyright(C), 2006-2008, Allwinner Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : sdcard_bsp.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2007.12.13
*
* Description : This file provides some basic functions to operating Nand Flash Controller rather
*             than Nand Flash chips.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang       2007.12.13      1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  _SDCARD_BSP_H_
#define  _SDCARD_BSP_H_

#include  "string.h"

#define   SD_CARD_NO                       (2)

#define   CARD_OK                          0
#define   CARD_FAIL                        (-1)

#define   HIGH_SPEED			           0x10
#define   HIGH_CAP			               0x20

#define  SDC_READ_REG(reg)                 (reg)
#define  SDC_WRITE_REG(reg, val)           ((reg) = (val))

#define  SDC_OFFSET                        0x1000
/*************SD/MMC controller define******************/
/*offset*/
#define SD_REG_o_CTRL		0x00
#define SD_REG_o_CLKCTRL	0x04
#define SD_REG_o_TMOUT		0x08
#define SD_REG_o_BUSWIDTH	0x0c
#define SD_REG_o_BLKSIZ		0x10
#define SD_REG_o_BYTCNT		0x14
#define SD_REG_o_CMD		0x18
#define SD_REG_o_CMDARG		0x1c
#define SD_REG_o_RESP0		0x20
#define SD_REG_o_RESP1		0x24
#define SD_REG_o_RESP2		0x28
#define SD_REG_o_RESP3		0x2c
#define SD_REG_o_INTMASK	0x30
#define SD_REG_o_MINTTSTS	0x34
#define SD_REG_o_RINTSTS	0x38
#define SD_REG_o_STATUS		0x3c
#define SD_REG_o_FIFOTH		0x40
#define SD_REG_o_FUNS       0x44
#define SD_REG_o_TCBCNT		0x48
#define SD_REG_o_TBBCNT		0x4c
#define SD_REG_o_DBGC       0x50
#define SD_REG_o_FIFOC		0x64
#define SD_REG_o_FIFO		0x100

/* register group*/
typedef struct __REGISTER_GROUP
{
    volatile __u32      sd_reg_ctrl;			//0x00
    volatile __u32      sd_reg_clkctrl;         //0x04
    volatile __u32      sd_reg_tmout;           //0x08
    volatile __u32      sd_reg_buswidth;        //0x0C
    volatile __u32      sd_reg_blksiz;          //0x10
    volatile __u32      sd_reg_bytcnt;          //0x14
    volatile __u32      sd_reg_cmd;             //0x18
    volatile __u32      sd_reg_cmdarg;          //0x1C
    volatile __u32      sd_reg_resp0;           //0x20
    volatile __u32      sd_reg_resp1;           //0x24
    volatile __u32      sd_reg_resp2;           //0x28
    volatile __u32      sd_reg_resp3;           //0x2C
    volatile __u32      sd_reg_intmask;         //0x30
    volatile __u32      sd_reg_minttsts;        //0x34
    volatile __u32      sd_reg_rintsts;         //0x38
    volatile __u32      sd_reg_status;          //0x3C
    volatile __u32      sd_reg_fifoth;          //0x40
    volatile __u32      sd_reg_funs;            //0x44
    volatile __u32      sd_reg_tcbcnt;          //0x48
    volatile __u32      sd_reg_tbbcnt;          //0x4C
    volatile __u32      sd_reg_dbgc;            //0x50
    volatile __u32      sd_reg_reserved[11];    //0x54~0x7c
    volatile __u32      sd_reg_idmac;           //0x80
    volatile __u32      sd_reg_desba;           //0x84
    volatile __u32      sd_reg_idmast;          //0x88
    volatile __u32      sd_reg_idmaie;          //0x8C
    volatile __u32      sd_reg_curdes;          //0x90
    volatile __u32      sd_reg_curbuf;          //0x94
}__register_grout_t;

typedef struct {
    __u32                   :1,
            dic             :1,     //disable interrupt on completion
            last_des        :1,     //1-this data buffer is the last buffer
            first_des       :1,     //1-data buffer is the first buffer, 0-data buffer contained in the next descriptor is the first data buffer
            des_chain       :1,     //1-the 2nd address in the descriptor is the next descriptor address
            end_of_ring     :1,     //1-last descriptor flag when using dual data buffer in descriptor
                            :24,
            card_err_sum    :1,     //transfer error flag
            own             :1;     //des owner:1-idma owns it, 0-host owns it

    __u32   data_buf1_sz    :13,
            data_buf2_sz    :13,
                            :6;
    __u32   buf_addr_ptr1;
    __u32   buf_addr_ptr2;

} smc_idma_des;

/*command mode*/
enum{
	SD_PROPRIETARY_CMD, 			// sd mem standard command
	SD_SPECIFIC_CMD,				// sd mem private command
	MMC_SPECIFIC_CMD,				// mmc private command
	SDIO_SPECIFIC_CMD,				// sdio private command
	CEATA_SPECIFIC_CMD				// ceata private command
};

/*command type*/
enum{
	BC,								// broadcast command without response
	BCR,							// broadcast command with response
	AC,								// addressed comamnd without data transfer
	ADTC							// addressed command with data transfer
};

/*command index*/
/*basic commands*/
#define GO_IDLE_STATE   		0  	// bc,rest all cards to idle state
#define ALL_SEND_CID	    	2  	// bcr,asks any card to send CID numbers,R2
#define SEND_RELATIVE_ADDR  	3  	// bcr,ask the card to publish a new relative address,R6
#define SET_DSR					4	// bc,program the DSR of all cards,for sd 2.0
#define SWITCH_FUNC	        	6  	// atdc,checks swithcable function(mode 0),And swtich card function(mode 1),R1,for sd 1.x
#define SELECT_OR_DESECLT_CARD 	7  	// ac,select or deselect one card,R1
#define SEND_IF_COND			8	// bcr,sends Sd memory card interface condition,R7,for sd 2.0
#define MMC_SEND_EXT_CSD		8	// adtc,the card sends it's ex_csd as a block of data,R1
#define SEND_CSD	        	9  	// ac,addressed card send CSD,R2
#define SEND_CID  				10  // ac, addressed card send CID,R2
#define READ_DATA_UNTIL_STOP  	11  // atdc,read data stream from catd until a STOP_TRANSMITISSION follows,R1
#define STOP_TRANSMISSION		12 	// ac£¬termilate a multiple block read/write operation,R1b
#define SEND_STATUS				13 	// ac£¬addressed card send its status regesiter,R1
#define GO_INACTIVE_STATE		15 	// ac£¬sets the card to inactive state

/*block read commands*/
#define SET_BLOCKLEN			16 	// ac£¬select a block length for all read/write cmds,R1
#define READ_SINGLE_BLOCK		17 	// adtc£¬reads a block of the size seclected by SET_BLOCKLEN,R1
#define READ_MULTIPLE_BLOCK		18 	// adtc,continuously send blocks of data until interrupted by a stop transmission commmad,R1

/*block write commands*/
#define WRITE_SINGLE_BLOCK		24 	// adtc£¬writes a block of the size seclected by SET_BLOCKLEN,R1
#define WRITE_MULTIPLE_BLOCK	25 	// adtc,continuously writes blocks of data until interrupted by a stop transmission commmad,R1
#define PROGRAM_CSD  			27 	// adtc,program the programmable bits of CSD,R1

/* write protection commands*/
#define SET_WRITE_PROT			28 	// ac,sets the write protect bit of the addressed group ,R1b
#define CLR_WRITE_PROT			29 	// ac, clears the write protect bit of the addressed group ,R1b
#define SEND_WRITE_PROT 		30 	// adtc,ask the card to send status of the write protection bits,R1

/* erase commands*/
#define ERASE_WR_BLK_START		32 	// ac,sets the address of the first write bloock to be erased,R1
#define ERASE_WR_BLK_END		33 	// ac,sets the address of the last write bloock to be erased,R1
#define ERASE 					38  // ac,erase all selected write blocks,R1

/*MMC private command*/
#define MMC_SEND_OF_COND		1   // bcr,Activates the card's initialization process,R3
#define MMC_SWITCH				6	// ac,switch the mode of operation of the selected card or modifies tge EXT_CSD,R1b
#define MMC_BUSTEST_R			14	// adtc,a host reads the reversed testing data pattern from a card,R1
#define MMC_BUSTEST_W			19	// adtc,a host sends the bus teset data pattern to a card,R1
#define MMC_FAST_IO				39  // ac,used to read or write 8 bit registers,R4
#define MMC_GO_IRQ_STATE		40	// bcr,sets the system info interrupt mode,R5

/* lock card commands*/
#define LOCK_OR_UNLOCK			42 	// lock or unlock sd card

/*sd io command*/
#define IO_SEND_OP_COND			5   // for SD IO.similar to ACMD41 for sd mem
#define IO_RW_DIRECT			52
#define IO_RW_EXTENDED			53
#define SD_IO_CMD54				54

/* application specfic commands*/
#define APP_CMD					55 	// ac,indicates the next cmd is an specific cmd,R1
#define GEN_CMD					56 	// adtc,send or get a block of data,R1

/*ce-ata command*/
#define CEATA_RW_MULTIPLE_REGISTER	60
#define CEATA_RW_MULTIPLE_BLOCK	61

/*application specific commands used by sd mem*/
#define SET_BUS_WIDTH			6  	// ac,define the bus width(00 = 1bit, 10 = 4bit),R1
#define SD_STATUS				13 	// adtc,send the sd card status,R1
#define SEND_NUM_WR_BLOCKS		22 	// adtc,send the number of written write blocks,R1
#define SET_WR_BLK_ERASE_CNT	23 	// ac,set the number of write blocks to be pre-erased before writing,R1
#define SD_APP_OP_COND			41 	// bcr,asks the accessed card to send its OCR(operating conditon register) content,R3
#define SET_CLR_CARD_DETECT		42 	// ac,connect or disconnect the pull up resistor of the card for card detect,R1
#define SEND_SCR				51 	// adtc,reads the SCR(sd configure register),R1

typedef struct{
	__u8 	clock_div;				// clock divide data (o - 255)
	__u8    power_mode;				// power mode, low power or others
	__u32 	bus_width;				// bus mode 1bit/4bit/8bit
	__u32 	block_size;				// the byte cnt of one block
	__u32 	data_timeout;			// read data timeout
	__u32 	response_timeout;		// response timeout

}SDC_CARD_CONFIG_INFO_T;


typedef struct{
	__u8 	mode; 					// normal sd cmd or application specific cmd
	__u8 	type; 					// bc,bcr,ac,adtc
	__u32 	arg; 					// comamd argument

	__u32 	index         		: 6;// cmd index
	__u32 	response_len  		: 1;// response data length. 0 - short, 1 - long
	__u32   auto_stop           : 1;
	__u32 	check_response_crc  : 1;// determine if check response crc, 0 - not 1 - yes
	__u32 	transfer_dir  		: 1;// data trans direction, 0 - read, 1 - write
	__u32 	transfer_mode 		: 1;// data trans mode , 0 - block trans , 1 - stream trans
	__u32   send_initialization : 1;// need 74 clocks cycle before send current cmd
	__u32 	reserved      		: 20;
}SDC_CMD_T;

extern   __s32 SDC_Init     (void                          );
extern   __s32 SDC_Exit     (__s32 gpio_no                 );
extern   __u32 SDC_Config   (__s32 gpio_no                 );
extern   __s32 SDC_SendCmd  (SDC_CMD_T *cmd, void*response );
extern   __s32 SDC_ReadData (SDC_CMD_T *cmd, void *buf, __u32 byte_cnt, __u32 card_no);
extern   __s32 SDC_ReadDataExt(SDC_CMD_T *cmd, void *buf, __u32 byte_cnt, __u32 card_no);
extern   __s32 SDC_WriteData(SDC_CMD_T *cmd, void *buf, __u32 byte_cnt, __u32 card_no);
extern   __s32 SDC_ConfigCardHost(SDC_CARD_CONFIG_INFO_T *card_cfg_info              );


#endif  /*_SDCARD_BSP_H_*/
