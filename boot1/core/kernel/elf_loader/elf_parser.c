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
#include "elf_parser.h"
#include "elf_private.h"
#include "mod_private.h"
#include "include.h"
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
static __s32 _is_spec_section(void *elf_img, struct __ELF32SHEAD *elf32shead, __u8 *section_name_section_base, eGon2_mod_t **priv_save)
//确定是否为特殊的段，并确定其子类型，是一个驱动或者是应用
{
	eGon2_mod_t	*tmp_header = NULL;
	__u8 * section_name = NULL;
	*priv_save = NULL;

    //name成员指定了这个section的名字。它的值是section报头字符表section的索引
	section_name = (__u32)elf_img + section_name_section_base + elf32shead->name;

	//如果找到了EGON2_MAGIC段
	if(!strcmp((void *)section_name, eGon2_HEADER_COMM_MAGIC))
	{
        //offset保存了这个段在文件中的偏移
        //因此，tmp_header指向了该段的首地址
		tmp_header = (eGon2_mod_t *) ((__u32)elf_img + (__u32)(elf32shead->offset));
		//很显然，priv指向的是该段(自定义段的具体内容)，和ELF文件已经没有太大关系
        *priv_save = tmp_header;
        if(tmp_header->magic[7] == 'p')
        {
            return eGON2_SUB_TYPE_APP;
        }
        else
        {
            return eGON2_SUB_TYPE_DRV;
        }
    }

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
static __s32 _load_section(__u32 elf_type, void *elf_img, struct __ELF32SHEAD *elf32shead)
{

	__u32 copyclear = 0;

	//解析段，判断段的类型
	//SHT_PROGBITS: 该section保存被程序定义了的一些信息，它的格式和意义取决于程序本身
	//SHT_NOBITS  : 该类型的section在文件中不占空间，但是类似SHT_PROGBITS。尽管该section不包含字节，
	//              sh_offset成员包含了概念上的文件偏移量
	switch (elf32shead->type)
	{
		//程序段，包括RO,RW段
		case EELF_SHT_PROGBITS:
		{
		    //SHF_ALLOC : 该section在进程执行过程中占据着内存
			if(elf32shead->flags & EELF_SHF_ALLOC)
			{
			    //SHF_EXECINSTR : 该section包含了可执行的机器指令
			    //SHF_WRITE     : 该section包含了在进程执行过程中可被写的数据
            	if((elf32shead->flags & EELF_SHF_EXECINSTR) || (elf32shead->flags & EELF_SHF_WRITE))
        		{
            		copyclear = 1;
        		}
			}
			break;
		}
		//NOBITS段
        case EELF_SHT_NOBITS:
		{
		    //SHF_ALLOC : 该section在进程执行过程中占据着内存
        	if(elf32shead->flags & EELF_SHF_ALLOC)
			{
            	copyclear = 2;
        	}
			break;
		}
        default:
           return 0;
	}

	if(copyclear == 1)
	{
	    //把整个数据区拷贝到对应地址
   		memcpy((void *)(elf32shead->addr),(void *)((__u32)elf_img + (__u32)(elf32shead->offset)), elf32shead->size);

		return 1;
	}
	else if(copyclear == 2)
	{
	    //把内存区数据清空
		memset((void *)(elf32shead->addr), 0, elf32shead->size);

		return 1;
    }

	return 0;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
__s32 elf_loader(void * elf_img, __u32 *entry)
{
	__u32   i = 0;
	__u8	sepc_section_index = 0;	//特殊段的index
	struct  __ELF32HEAD 	_elf32head;
	struct  __ELF32SHEAD 	_elf32shead;
	struct  __ELF32HEAD     *elf32head  = &_elf32head;
	struct  __ELF32SHEAD    *elf32shead = &_elf32shead;
	         eGon2_mod_t     *priv = NULL;
	__u32	section_name_section_offset = 0;	//保存sction名的section 在文件中的位置
	__u32                    elf_type;

	elf32head = (struct __ELF32HEAD  *)elf_img;
	elf_type = 0;

    //check it, 判断是否为有效的elf文件
    //elf文件的头部是4个固定字节，分别是0x7f, 'E', 'L', 'F',如果不满足这个条件，则可以认为不是elf格式的文件
	if(	(elf32head->ident[EI_MAG0] != ELFMAG0 ) ||
		(elf32head->ident[EI_MAG1] != ELFMAG1 ) ||
		(elf32head->ident[EI_MAG2] != ELFMAG2 ) ||
		(elf32head->ident[EI_MAG3] != ELFMAG3 ) )
	{
		*entry = 0;
        eGon2_printf("elf file header magic not match\n");

		return -1;
	}

	//设置入口地址
	//entry是elf提供的第一个传输控制的虚拟地址，在那启动进程。假如文件没有如何关联的入口点，该成员就保持为0
	if((__u32)(elf32head->entry) < 0x40000000)
	{
	    eGon2_printf("elf file entry address not correct\n");

	    return -1;
	}

	//计算section_name_section_offset
	//shoff保存着section头表（section header table）在elf文件中的偏移量(以字节计数)
	//shstrndx保存着跟section名字字符表相关入口的section头表(section header table)索引。
	//elf的所有的section表都存放在一起，每个表都是按照固定的数据结构
	//因此，elf32shead指向了section名字字符表相关入口的section头表的地址
	elf32shead = ( struct __ELF32SHEAD *)((__u32)elf_img + elf32head->shoff + (sizeof(struct __ELF32SHEAD))*(elf32head->shstrndx));
	//offset成员给出了该section的字节偏移量(从文件开始计数)
	//section_name_section_offset保存了section名字字符表在文件中的偏移
	section_name_section_offset = elf32shead->offset;

	//shnum成员保存着在程序头表中入口的个数
	//这个for循环遍历了所有的程序头表
	for(i = 0; i < elf32head->shnum; i++)
	{
		__s32 temp = 0;

        //这里elf32shead依次指向每一个section表的地址
		elf32shead = ( struct __ELF32SHEAD *)((__u32)elf_img + elf32head->shoff + (sizeof(struct __ELF32SHEAD)) * i);
		temp = _is_spec_section(elf_img, elf32shead, (__u8 *)section_name_section_offset, &priv);

		if(temp == eGON2_SUB_TYPE_APP || temp == eGON2_SUB_TYPE_DRV)
		{
			elf_type = temp;
			sepc_section_index = i;

			break;
		}
	}
	if(elf_type == 0)
	{
	    eGon2_printf("elf file is not a app or driver\n");

		return -1;
	}

	//是fex支持的elf文件
	for(i = 0; i < elf32head->shnum; i++)
	{
	    //这里elf32shead依次指向每一个section表的地址
		elf32shead = ( struct __ELF32SHEAD *)((__u32)elf_img + elf32head->shoff + (sizeof(struct __ELF32SHEAD)) * i);

		//只要不是前面找到的特定段
		if(sepc_section_index != i)
		{
			//普通段落的处理
			if(_load_section(elf_type, elf_img, elf32shead) < 0)
			{
				return -1;
			}
		}
	}

	//处理特殊段
	if(priv != NULL)
	{
		if(elf_type == eGON2_SUB_TYPE_DRV)
		{
			return eGon2_driver_reg(priv);
		}
		else if(elf_type == eGON2_SUB_TYPE_APP)
		{
			*entry = (__u32)priv->main;

			return 0;
		}
	}

    eGon2_printf("can't find elf file entry\n");

	return -1;
}

