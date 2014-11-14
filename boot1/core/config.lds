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
		*(EXCLUDE_FILE(standby/*.o arm_board/arm_start.o).text)
	}

	.arm_start ALIGN(4):
	{
		_arm_start = .;
		arm_board/arm_start.o (.text)
		_arm_end = .;
	}

	.data ALIGN(4):
	{
		*(.data)
		*(EXCLUDE_FILE(standby/*.o).data)
	}
	.rodata ALIGN(4):
	{
		*(.rodata)
		*(EXCLUDE_FILE(standby/*.o).rodata)
	}

	.bss ALIGN(4):
	{
		_bss_start = .;
		*(EXCLUDE_FILE(standby/*.o).bss)
		_bss_end = .;
	}

	_standby_start_lma = .;
	.boot_standby_text 0x400 :
	AT ( ADDR (.bss) + SIZEOF (.bss) )
	{
		_standby_start = .;
		standby/*.o(.text .data .rodata .bss)
		_standby_end = .;
	}
	.notes ALIGN(4):
	{
		*(.note.*)
	}
}

