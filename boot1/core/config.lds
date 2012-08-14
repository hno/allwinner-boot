OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SECTIONS
{
	. = 0x42400000;
	.boot_head ALIGN(4):
	{
		parameters/Boot1_head.o(.rodata)
	}

	.text ALIGN(4):
	{
		start/asm_start.o (.text)
		*(EXCLUDE_FILE(standby/*.o arm_board/arm_start.o drivers/iic/sw_iic.o).text)
	}

	.arm_start ALIGN(4):
	{
		_arm_start = .;
		arm_board/arm_start.o (.text)
		_arm_end = .;
	}

	.data ALIGN(4):
	{
		*(EXCLUDE_FILE(standby/*.o drivers/iic/sw_iic.o).data)
	}
	.rodata ALIGN(4):
	{
		*(EXCLUDE_FILE(standby/*.o drivers/iic/sw_iic.o).rodata)
	}

	.bss ALIGN(4):
	{
		_bss_start = .;
		*(EXCLUDE_FILE(standby/*.o drivers/iic/sw_iic.o).bss)
	}
	_bss_end = ( ADDR (.bss) + SIZEOF (.bss) );

	.notes ALIGN(4):
	{
		*(.note.*)
	}

	_standby_start_lma = ( ADDR (.notes) + SIZEOF (.notes) );
	.boot_standby_text 0x400 :
	AT ( ADDR (.notes) + SIZEOF (.notes) )
	{
		_standby_start = .;
		standby/*.o(.text .data .rodata .bss)
		drivers/iic/sw_iic.o(.text .data .rodata .bss)
		_standby_end = .;
	}
}

