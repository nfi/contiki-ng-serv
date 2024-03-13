OUTPUT_ARCH( "riscv" )
ENTRY(_start)

SECTIONS
{
  START_ADDRESS = 0x00000000;
  STACK_SIZE = 0x1000;
  
  . = START_ADDRESS;
  .text : { *(.text) }

  /* data */
  .gnu_build_id : { *(.note.gnu.build-id) }
  .data : { *(.data) }
  .data2 : { *(.data2) }

  .rodata : { *(.rodata) }
  .sdata : { *(.sdata) }
  .debug : { *(.debug) }

  .bss : { *(.bss) }

  /* stack */
  . = ALIGN(4);
  stack_bottom = .;
  . += STACK_SIZE;
  . = ALIGN(4);
  stack_top = .;
  _end = .;
}
