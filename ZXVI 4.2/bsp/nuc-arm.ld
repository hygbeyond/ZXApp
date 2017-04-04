/******************************************************************************
 *
 * standalone.ld - Linker script for applications using startup.c and
 *                 DriverLib.
 *
 * Copyright (c) 2005-2007 Luminary Micro, Inc.  All rights reserved.
 *
 * Software License Agreement
 *
 * Luminary Micro, Inc. (LMI) is supplying this software for use solely and
 * exclusively on LMI's microcontroller products.
 *
 * The software is owned by LMI and/or its suppliers, and is protected under
 * applicable copyright laws.  All rights are reserved.  Any use in violation
 * of the foregoing restrictions may subject the user to criminal sanctions
 * under applicable laws, as well as to civil liability for the breach of the
 * terms and conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * This is part of revision 1049 of the Stellaris Driver Library.
 *
 *****************************************************************************/


__ram_size__       = LENGTH(SRAM);
__ram_top_stack__  = ORIGIN(SRAM) + __ram_size__ ;
__stack_end__      = __ram_top_stack__ ;

__flash_32k        = ORIGIN(FLASH) + 32K;
__flash_60k        = ORIGIN(FLASH) + 60K;
__flash_64k        = ORIGIN(FLASH) + 64K;
__flash_128k       = ORIGIN(FLASH) + 128K;
__flash_256k       = ORIGIN(FLASH) + 256K;
__flash_end        = ORIGIN(FLASH) + LENGTH(FLASH);


__DataFlash        = __flash_end;
__DataFlash_end    = __DataFlash + 4K;

/* Section Definitions */
SECTIONS
{
   .text : ALIGN(4)
  {
    FILL( 0xFF)
	KEEP(*(.isr_vector .isr_vector.*))
    *(.after_vectors .after_vectors.*)

	*(.text .text.*)
	*(.gnu.linkonce.t.*)
	*(.glue_7)
	*(.glue_7t)
	*(.gcc_except_table)
    . = ALIGN(4);
	*(.gnu.linkonce.r.*)
  } > FLASH

 
  .rodata : ALIGN(4)
  {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  } > FLASH

  .shell : ALIGN(4)
  {
      KEEP( *(.shell .shell.*));
      PROVIDE (shell_end = .);
  } > FLASH

  PROVIDE (shell_start = LOADADDR(.shell) );
  PROVIDE (shell_size = SIZEOF(.shell) );

  .drive : ALIGN(4)
  {
      KEEP( *(.drive .drive.*));
      PROVIDE (drive_end = .);
  } > FLASH
  PROVIDE (drive_start = LOADADDR(.drive) );
  PROVIDE (drive_size = SIZEOF(.drive)); 

  /*
   * for exception handling/unwind - some Newlib functions (in common
   * with C++ and STDC++) use this.
   */
	.ARM.extab : ALIGN(4)
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH
	__exidx_start = .;
	
	.ARM.exidx : ALIGN(4)
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH


	/* .ctors .dtors are used for c++ constructors/destructors */
	/* added by Martin Thomas 4/2005 based on Anglia Design example */
	.ctors :
	{
		PROVIDE(__ctors_start__ = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		. = ALIGN(4);
		PROVIDE(__ctors_end__ = .);
		. = ALIGN(4);
	} >FLASH
	
	.dtors :
	{
		PROVIDE(__dtors_start__ = .); 
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		. = ALIGN(4);
		PROVIDE(__dtors_end__ = .);
		. = ALIGN(4);
	} >FLASH
  
    
	__exidx_end = .;
	
	_etext = .;
		

  .data : AT (_etext)  ALIGN(4)
  {
	_data = .;
	*(vtable vtable.*)
	*(.data .data.*)
	*(.gnu.linkonce.d*)
	. = ALIGN(4);
	_edata = . ;
  } > SRAM
 
  /* .bss section which is used for uninitialized data */
  .bss (NOLOAD) : ALIGN(4)
  {
    _bss = . ;
    *(.bss .bss.*)
    *(.gnu.linkonce.b*)
    *(COMMON)
	_ebss = . ;
  } > SRAM

  .udata (NOLOAD) : ALIGN(4)
  {
      *(.udata);
  } > USRAM

  __udata_end = .;
  PROVIDE(_udata_end = .);

  .ndata (NOLOAD) : ALIGN(4)
  {
      *(.ndata);
  } > NSRAM
  __ndata_end = .;
  PROVIDE(_ndata_end = .);
  
  
  .noinit (NOLOAD): ALIGN(4)
  {
      PROVIDE (__noinit_start = .);
      *(.noinit);
  } > SRAM

  .stack (NOLOAD) :  ALIGN(32)
  {
      PROVIDE (stack_start = .);
      *(.stack*)
      PROVIDE (_stack = .);
  } > SRAM
 
  . = ALIGN(4);
  _end = . ;
  PROVIDE (__end = .);
  PROVIDE (_stack_end = __stack_end__ );
  PROVIDE (_ram_top = __ram_top_stack__ );

}

