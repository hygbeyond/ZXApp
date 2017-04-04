




CROSS_COMPILE ?= arm-none-eabi-

# MCU name and submodel
MCU      ?= cortex-m3
THUMB    ?= -mthumb
OPT      ?= s

VERBOSE ?=  


COMPILER  ?= $(OBJDIR)
ALL_FLAGS ?= $(THUMB) -mcpu=$(MCU) 

COBJ      = $(SRC:%.c=$(OBJDIR)/%.o)
AOBJ      = $(ASRC:%.s=$(OBJDIR)/%.o)

SHELL = sh
CC      = $(CROSS_COMPILE)gcc
CPP     = $(CROSS_COMPILE)g++
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE    = $(CROSS_COMPILE)size
NM      = $(CROSS_COMPILE)nm
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
REMOVE  = rm -rf
COPY    = cp
ELFDWT  = elfdwt.exe



#
# The flags passed to the assembler.
#
AFLAGS = -MD  ${patsubst %,-D%,${subst :, ,${CDEFS}}}

CFLAGS = ${patsubst %,-D%,${subst :, ,${CDEFS}}}
CFLAGS += -gdwarf-2  -O${OPT} -std=gnu99 
CFLAGS += -Wall 
CFLAGS += -Wcast-align -Wcast-qual -Wimplicit
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused

CFLAGS += -ffunction-sections
CFLAGS += -Wa,-adhlns=$(@:%.o=%.lst)  -MD -MP -MF  ${@:.o=.d}



#Support for newlibc-lpc (file: libnewlibc-lpc.a)
NEWLIBLPC = 
MATH_LIB = -lm 

#CPLUSPLUS_LIB = -lstdc++

# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -nostartfiles -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(patsubst %,-L%,$(LDDIRS))
LDFLAGS += $(MYLIBS) $(NEWLIBLPC) $(MATH_LIB)
LDFLAGS += -lc -lgcc
LDFLAGS += $(CPLUSPLUS_LIB)
LDFLAGS += -Wl,--gc-sections

ifneq ($(strip $(SUBMDL)),)
# Set Linker-Script Depending On Selected Memory
ifeq ($(RUN_MODE),RAM_RUN)
LDFLAGS +=-T$(SUBMDL)-RAM.ld
else
LDFLAGS +=-T$(SUBMDL)-ROM.ld
endif
else
LDFLAGS +=-T./bsp/LPC17xx-ROM.ld
endif



#lib: ${COMPILER}/$(TARGET).a 


elf: $(TARGET).elf
hex: $(TARGET).hex
bin: $(TARGET).bin
lss: $(TARGET).lss
sym: $(TARGET).sym


#
# Add the include file paths to AFLAGS and CFLAGS.
#
IPATH += .
AFLAGS+=${patsubst %,-I%,${subst :, ,${IPATH}}}
CFLAGS+=${patsubst %,-I%,${subst :, ,${IPATH}}}


#
# The rule for building the object file from each C source file.
#
${COMPILER}/%.o: %.c
	@if [ 'x${VERBOSE}' = x ];                            \
	 then                                                 \
	     echo "  CC    ${<}";                             \
	 else                                                 \
	     echo ${CC} -c ${ALL_FLAGS} ${CFLAGS}  -o ${@} ${<}; \
	 fi
	@${CC} -c ${ALL_FLAGS} ${CFLAGS} -o ${@} ${<}  ${GENDEPFLAGS}


#
# The rule for building the object file from each assembly source file.
#
${COMPILER}/%.o: %.s
	@if [ 'x${VERBOSE}' = x ];                               \
	 then                                                    \
	     echo "  AS    ${<}";                                \
	 else                                                    \
	     echo ${CC} -c ${ALL_FLAGS} ${AFLAGS} -o ${@} -c ${<}; \
	 fi
	@${CC} -c ${ALL_FLAGS}  ${AFLAGS} -o ${@} -c ${<} ${GENDEPFLAGS} 


#
# The rule for creating an object library.
#
.PRECIOUS :  $(AOBJ) $(COBJ)
.SECONDARY : $(TARGET).a 
%.a:   $(AOBJ) $(COBJ)
	@if [ 'x${VERBOSE}' = x ];     \
	 then                          \
	     echo "  AR    ${@}";      \
	 else                          \
	     echo ${AR} -cr ${@} ${^}; \
	 fi
	@${AR} -cr ${@} ${^}

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS :  $(AOBJ) $(COBJ)
%.elf:  $(AOBJ) $(COBJ)   
	@if [ 'x${VERBOSE}' = x ];                            \
	 then                                                 \
	     echo "  LD    ${@}";                             \
	 else                                                 \
	     echo $(CC) $(THUMB) $(ALL_CFLAGS) --output $@  ${^}  $(LDFLAGS); \
	 fi
	@$(CC)  ${ALL_FLAGS}  $(ALL_CFLAGS) --output $@  ${^} $(LDFLAGS)


# Create final output files (.hex, .eep) from ELF output file.
# TODO: handling the .eeprom-section should be redundant
%.hex: %.elf
	@if [ 'x${VERBOSE}' = x ];                            \
	 then                                                 \
	     echo "  HEX    ${@}";                             \
	 else                                                 \
	     echo $(OBJCOPY) -O ihex $< $@; \
	 fi
	@$(OBJCOPY) -O ihex $< $@  -R .udata  -R .ndata

%.bin: %.elf
	@if [ 'x${VERBOSE}' = x ];                            \
	 then                                                 \
	     echo "  BIN    ${@}";                             \
	 else                                                 \
	     echo $(OBJCOPY) -O binary $< $@; \
	 fi
	@$(OBJCOPY) -O binary $< $@  -R .udata  -R .ndata


# Create extended listing file from ELF output file.
# testing: option -C
%.lss: %.elf
	@if [ 'x${VERBOSE}' = x ];                            \
	 then                                                 \
	     echo "  LSS    ${@}";                             \
	 else                                                 \
	     echo $(OBJDUMP) -h -S -C $< > $@; \
	 fi
	@$(OBJDUMP) -h -S -C $< > $@


# Create a symbol table from ELF output file.
%.sym: %.elf
	@if [ 'x${VERBOSE}' = x ];                            \
	 then                                                 \
	     echo "  SYM    ${@}";                             \
	 else                                                 \
	     echo $(NM) -n $< > $@; \
	 fi
	@$(NM) -n $< > $@


$(shell mkdir $(COMPILER)  2>/dev/null )

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif

