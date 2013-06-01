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

#include  "include.h"
#include  "boot_img.h"
#include  "BootOS_i.h"
#include  "common.h"
#include  "setup.h"



static struct tag *params;

static void setup_start_tag (char *para_addr);
//static void setup_memory_tags (void);
static void setup_commandline_tag (char *commandline, __u32 cmdline_length);
//static void setup_initrd_tag (__u32 initrd_start, __u32 initrd_end);
//void setup_serial_tag (struct tag **tmp);
//void setup_revision_tag(struct tag **in_params);
static void setup_end_tag (void);


/* Boot android style linux kernel and ramdisk */
int do_boot_linux (char *para_addr, char *cmdline, __u32 cmdline_length)
{

	setup_start_tag (para_addr);
//	setup_serial_tag (&params);
//	setup_revision_tag (&params);
#if  0
	/*----------------------------------------------------------------------
	 * We don't need to set memory tags, since kernel will squash_mem_tags()
	 * in arch/arm/kernel/setup.c. If we set memory tags, kernel will print
	 * "Ignoring unrecognised tag 0x00000000" at boot time.
	 *----------------------------------------------------------------------*/
	/* setup_memory_tags (bd); */
#endif
	setup_commandline_tag (cmdline, cmdline_length);
//#ifdef CONFIG_INITRD_TAG
//	if (hdr->ramdisk_size)
//		setup_initrd_tag (bd, initrd_start, initrd_end);
//#endif
	setup_end_tag ();

	return 0;
}


static void setup_start_tag (char *paddr)
{
	params = (struct tag *)paddr;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}


//static void setup_memory_tags (void)
//{
//	int i;
//
//	for (i = 0; i < 1; i++)
//	{
//		params->hdr.tag = ATAG_MEM;
//		params->hdr.size = tag_size (tag_mem32);
//
//		params->u.mem.start = EGON2_DRAM_BASE;
//		params->u.mem.size = 512<<20;
//
//		params = tag_next (params);
//	}
//}



static void setup_commandline_tag (char *commandline, __u32 cmdline_length)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	commandline[cmdline_length]='\0';
	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}


//static void setup_initrd_tag (__u32 initrd_start, __u32 initrd_end)
//{
//	/* an ATAG_INITRD node tells the kernel where the compressed
//	 * ramdisk can be found. ATAG_RDIMG is a better name, actually.
//	 */
//	params->hdr.tag = ATAG_INITRD2;
//	params->hdr.size = tag_size (tag_initrd);
//
//	params->u.initrd.start = initrd_start;
//	params->u.initrd.size = initrd_end - initrd_start;
//
//	params = tag_next (params);
//}



//void setup_serial_tag (struct tag **tmp)
//{
//	struct tag *params = *tmp;
//	struct tag_serialnr serialnr;
//	void get_board_serial(struct tag_serialnr *serialnr);
//
//	get_board_serial(&serialnr);
//	params->hdr.tag = ATAG_SERIAL;
//	params->hdr.size = tag_size (tag_serialnr);
//	params->u.serialnr.low = serialnr.low;
//	params->u.serialnr.high= serialnr.high;
//	params = tag_next (params);
//	*tmp = params;
//}



static void setup_end_tag (void)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

