# Hey Emacs, this is a -*- makefile -*-
#
# WinARM template makefile
# by Martin Thomas, Kaiserslautern, Germany
# <eversmith@heizung-thomas.de>
#
# based on the WinAVR makefile written by Eric B. Weddington, J鰎g Wunsch, et al.
# Released to the Public Domain
# Please read the make user manual!
#
#
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make program = Download the hex file to the device, using lpc21isp
#
# (TODO: make filename.s = Just compile filename.c into the assembler code only)
#
# To rebuild project do "make clean" then "make all".
#
# Changelog:
# - 17. Feb. 2005  - added thumb-interwork support (mth)
# - 28. Apr. 2005  - added C++ support (mth)
# - 29. Arp. 2005  - changed handling for lst-Filename (mth)
#

#FLASH_TOOL = ULINK

OEM ?= 0

ifeq ($(OEM), 0)
OBJNAME   = ZXVI
endif

ifeq ($(OEM), 2)
OBJNAME   = ZXVI-TANDA
endif

OBJNAME   ?= ZXVI-O$(OEM)

VERSION    = 4
PATCHLEVEL = 2
MAXPOINTCNT ?= 8

MAKE          = make
CROSS_COMPILE = arm-none-eabi-

GUI_EN   = 0
OS_EN    = 1
MYLIB_EN = 1
FATFS_EN = 0

DEBUG_EN    ?= 1
NDEBUG_EN   ?= 0
PASSWORD_EN ?= 0
EN_CRP      ?= 0

# MCU name and submodel
MCU      = cortex-m0
CPU      = NUC131SC2AE
SUBMDL   = ./bsp/NUC131SC2AE
THUMB    = -mthumb
F_CPU    = 12
# Optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

VERBOSE =  

ifeq ($(MCU),cortex-m3)
LIBEXT = _m3
endif

ifeq ($(MCU),cortex-m0)
LIBEXT = _m0
endif


export MCU CPU THUMB
export CROSS_COMPILE ALL_FLAGS
export VERBOSE OPT


MYLIBS   = -los$(LIBEXT) -losc$(LIBEXT) -lHgui$(LIBEXT)

LDDIRS   = ../mylib_t

## Create ROM-Image (final)
RUN_MODE=ROM_RUN
## Create RAM-Image (debugging)
#RUN_MODE=RAM_RUN


# Output format. (can be srec, ihex, binary)
FORMAT = ihex


# Target file name (without extension).
ifeq ($(DEBUG_EN), 1)
OUTNAME = $(OBJNAME)_dbg
else
OUTNAME = $(OBJNAME)_$(VERSION)_$(PATCHLEVEL)_$(MAXPOINTCNT)

endif

TARGET = $(HEXDIR)/$(OUTNAME)

TOPDIR = $(shell pwd)
MYDIR  = default

LDDIRS += myLib
# Object files directory
OBJDIR = obj

# HEX files directory
HEXDIR = testhex


# List C source files here. (C dependencies are automatically generated.)
# use file-extension c for "c-only"-files
SRC  = main.c shell.c debug.c

SRC += OS_lib.c
SRC +=$(patsubst data/%.c,%.c,$(wildcard data/*.c))
SRC +=$(patsubst drive/%.c,%.c,$(wildcard drive/*.c))
SRC +=$(patsubst bsp/%.c,%.c,$(wildcard bsp/*.c))
ASRC = $(patsubst bsp/%.s,%.s,$(wildcard bsp/*.s))


# List C++ source files here.
# use file-extension cpp for C++-files
CPPSRC =


# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = 

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#DEBUG = stabs
DEBUG = dwarf-2

EXTRAINCDIRS =  ./app ./drive ./data ./bsp bsp/ARM_inc ./dataGUI \
				../mylib_t ../mylib_t/guiH

VPATH = $(EXTRAINCDIRS)
IPATH = $(EXTRAINCDIRS)


COMPILER  ?= $(OBJDIR)
ifneq ($(strip $(COMPILER)),)
COMPILER  ?= gcc
endif

COMPILER  ?= $(OBJDIR)
ALL_FLAGS = $(THUMB) -mcpu=$(MCU) 

# Place -D or -U options for C here
CDEFS =  $(RUN_MODE)
CDEFS += CPU=$(CPU)
CDEFS += $(CPU)
CDEFS += HNAME=$(OBJNAME)
CDEFS += VERSION=$(VERSION)
CDEFS += PATCHLEVEL=$(PATCHLEVEL)
CDEFS += MAXPOINT=$(MAXPOINTCNT)
CDEFS += DEBUG=$(DEBUG_EN)
CDEFS += EN_CRP=$(EN_CRP)
CDEFS += F_CPU=$(F_CPU)000000UL
CDEFS += OEM=$(OEM)
ifeq ($(NDEBUG_EN), 1)
CDEFS += NDEBUG
endif

ifeq ($(PASSWORD_EN), 1)
CDEFS += PASSWORD_HAIPPO
endif

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN       = -------- begin -------- 
MSG_END         = --------  end  --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER  = Size after:
MSG_FLASH       = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE     = Creating Symbol Table:
MSG_LINKING          = Linking:
MSG_COMPILING        = Compiling C:
MSG_COMPILING_ARM    = "Compiling C (ARM-only):"
MSG_COMPILINGCPP     = Compiling C++:
MSG_COMPILINGCPP_ARM = "Compiling C++ (ARM-only):"
MSG_ASSEMBLING       = Assembling:
MSG_ASSEMBLING_ARM   = "Assembling (ARM-only):"
MSG_CLEANING         = Cleaning project:
MSG_LPC21_RESETREMINDER = You may have to bring the target in bootloader-mode now.


# Default target.
all: begin gccversion build copy sizeafter finished end

buildall: updataCFG hex copy sizeafter 


#build: elf hex lss sym
build: elf hex bin lss sym

# Target: clean project.
clean: begin clean_list finished end


clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(OBJDIR)
	$(REMOVE) $(HEXDIR)

clean_file:
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(OBJDIR)/*
	$(REMOVE) $(HEXDIR)/*


elf: $(TARGET).elf
hex: $(TARGET).hex
bin: $(TARGET).bin
lss: $(TARGET).lss
sym: $(TARGET).sym


#  查询函数库是否存在，如果不存在编译
FLIBOSC = ../mylib/libosc.a 
FINEOSC = $(wildcard $(FLIBOSC))

lib: 
	@echo build lib =======================================
	$(MAKE) -C ../mylib_t

libbuild:
ifeq ($(FINEOSC), )
	$(MAKE) -C ../mylib_t 
	$(RM) $(TARGET).elf
endif

libclean:
	$(MAKE) -C ../mylib_t    clean

copy:
	$(COPY) $(TARGET).hex D:/$(TARGET).hex
	@echo

updataCFG:
	@echo updata MAX_POINT_COUNT
	@echo " " >> ./app/app_cfg.h

# Eye candy.
begin:
	@echo
	@echo $(MSG_BEGIN) 
	@echo DEFINDS: $(CDEFS)
	@echo ALL FLAGS: $(ALL_FLAGS)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo


# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A -x $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi


# Display compiler version information.
gccversion :
	@$(CC) --version


$(shell mkdir $(HEXDIR)   2>/dev/null  )
$(shell mkdir D:\$(HEXDIR)   2>/dev/null  )


-include  libmake.mk

# Listing of phony targets.
.PHONY : all buildall begin finish end sizebefore sizeafter gccversion \
	build elf hex lss sym clean clean_list clean_file program isp copy


