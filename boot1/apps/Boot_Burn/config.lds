OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SECTIONS
{
	. = 0x42800000;
	 EGON2_MAGIC : { magic.o(.rodata) }
	.text : { *(.text) *(.rodata)}
	.data   : { *(.data)   }
	.bss    : { *(.bss)    }

}
