#ifndef   _MCTL_HAL_H
#define   _MCTL_HAL_H

typedef struct __DRAM_PARA
{
	//normal configuration
	//unsigned int         dram_baseaddr;	//dram_baseaddr		0x40000000
	unsigned int           dram_clk;		//dram_clk			DDR2: 24		LPDDR2: 120
	unsigned int           dram_type;		//dram_type			DDR2: 2				DDR3: 3				LPDDR2: 6
    //unsigned int			dram_cas;
    unsigned int			dram_zq;
    unsigned int			dram_odt_en;

	//control configuration
	unsigned int			dram_para1;
					/******************************************************************************************
    				 * dram_para1		bit x definition
    				 *					bit 31:28 bank_size:	0: 4 banks		1: 8 banks
    				 *					bit 27:20 row_num:		n: n rows
    				 *					bit 19:16 page_size:    2: 2KByte	4: 4KByte		8: 8KByte
    				 *					bit 15:0  dram_size:	n: n MByte
    				 ******************************************************************************************/
    unsigned int			dram_para2;
					/******************************************************************************************
    				 * dram_para2		bit x definition
    				 * 					bit 15:12 rank_num:		1: 1 rank		2: 2 ranks
    				 *					bit 11:8  ch_num:		1: 1 channel	2: 2 channel
    				 *					bit 7:4	  access mode:	0: sequence		1: interleave
    				 *					bit 3:0   bus_width:	0: 16-bit		1: 32-bit
    				 ******************************************************************************************/

	//timing configuration
	unsigned int			dram_mr0;
    unsigned int			dram_mr1;
    unsigned int			dram_mr2;
    unsigned int			dram_mr3;
    unsigned int			dram_tpr0;	//[31:0]tzqcsi
    unsigned int			dram_tpr1;	//[31:22]texsr		[21:12]tdpd		[11:2]tzqcl		[1:0]tprea
    unsigned int			dram_tpr2;	//[31:23]trfc		[22:15]trefi	[14:7]tmrr		[6:0]tzqcs
    unsigned int			dram_tpr3;	//[31:25]trstl		[24:19]tras		[18:13]trc		[12:7]txpdll
    							//[6:3]trp			[2:0]tmrd
    unsigned int			dram_tpr4;	//[31:27]tcksre		[26:22]tcksrx	[21:17]tcke		[16:12]tmod
    							//[11:8]trtw		[7:4]tal		[3:0]tcl
    unsigned int			dram_tpr5;	//[31:28]tcwl		[27:24]trcd		[23:20]trrd		[19:16]trtp
    							//[15:12]twr		[11:8]twtr		[7:4]tckesr		[3:1]txp
   	unsigned int			dram_tpr6;	//[31:29]tdqs

    //reserved for future use
    unsigned int			dram_tpr7;
    unsigned int			dram_tpr8;
    unsigned int			dram_tpr9;
    unsigned int			dram_tpr10;
    unsigned int			dram_tpr11;
    unsigned int			dram_tpr12;
	//debug only
    unsigned int			dram_tpr13;
    				/******************************************************************************************
    				 * dram_dbg			bit x definition [default value]
    				 * 					bit 6 DRAM base address [0]:		0: 0x40000000		1: user definition
    				 * 					bit 5 2T mode enable [0]:			0: 2T mode disable	1: 2T mode enable
    				 * 					bit 4:3 bonding ID [00]:			00: A31				01: A31S
    				 * 														10:	A3x PHONE		11: reserved
    				 * 					bit 2 bus width lock [1]:			0: auto detect		1: user lock
    				 * 					bit 1 channel lock [1]:				0: auto detect		1: user lock
    				 *					bit 0 timing parameters setup [1]:  0: auto detect		1: user definition
    				 ******************************************************************************************/

}__dram_para_t;

extern unsigned int DRAMC_init(__dram_para_t *para);
extern unsigned int DRAMC_init_auto(__dram_para_t *para);
//extern unsigned int mctl_parameter(__dram_para_t *para);
extern unsigned int mctl_sys_init(void);
extern unsigned int mctl_reset_release(void);
extern unsigned int mctl_dll_init(unsigned int ch_index, __dram_para_t *para);
extern unsigned int mctl_channel_init(unsigned int ch_index, __dram_para_t *para);
extern unsigned int mctl_com_init(__dram_para_t *para);
extern unsigned int mctl_port_cfg(void);
//extern unsigned int DRAMC_init_EX(__dram_para_t *para);

extern unsigned int ss_bonding_id(void);
extern void paraconfig(unsigned int *para, unsigned int mask, unsigned int value);

#endif  //_MCTL_HAL_H










