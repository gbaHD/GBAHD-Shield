# simple AVR Makefile
#
# written by michael cousins (http://github.com/mcous)
# released to the public domain

# Makefile
#
# targets:
#   all:    compiles the source code
#   test:   tests the isp connection to the mcu
#   flash:  writes compiled hex file to the mcu's flash memory
#   fuse:   writes the fuse bytes to the MCU
#   disasm: disassembles the code for debugging
#   clean:  removes all .hex, .elf, and .o files in the source code and library directories

# parameters (change this stuff accordingly)
# project name
PRJ = gbaHD-avr
# avr mcu
MCU = atmega168p
# mcu clock frequency
CLK = 8000000
# avr programmer (and port if necessary)
# e.g. PRG = usbtiny -or- PRG = arduino -P /dev/tty.usbmodem411
PRG = usbtiny
# fuse values for avr: low, high, and extended
# these values are from an Arduino Uno (ATMega328P)
# see http://www.engbedded.com/fusecalc/ for other MCUs and options


ifeq ($(MCU), atmega168p)
# atmega168:
# Fuse L: 0xc2 (8Mhz internal RC-Osz.)
# Fuse H: 0xdd (2.7V BOD)
# Fuse E: 0xfa (512 words bootloader)
AVRDUDE_MCU=m168p

LFU = 0xC2
HFU = 0xDD
EFU = 0xFA

CFLAGS += -D__AVR_ATmega168P__
endif

ifeq ($(MCU), atmega328p)
# atmega328p:
# Fuse L: 0xc2 (8Mhz internal RC-Osz.)
# Fuse H: 0xdc (512 words bootloader)
# Fuse E: 0xfd (2.7V BOD)
AVRDUDE_MCU=m328p

LFU = 0xFF
HFU = 0xDE
EFU = 0x05

CFLAGS += -D__AVR_ATmega328P__
endif

# program source files (not including external libraries)
SRC = ./src
# where to look for external libraries (consisting of .c files and .h files)
# e.g. EXT = ../../EyeToSee ../../YouSART
INC = ./inc


#################################################################################################
# \/ stuff nobody needs to worry about until such time that worrying about it is appropriate \/ #
#################################################################################################

# include path
INCLUDE := $(foreach dir, $(INC), -I$(dir))
# c flags
CFLAGS    += -Wall -Os -DF_CPU=$(CLK) -mmcu=$(MCU) $(INCLUDE)

# executables
AVRDUDE = avrdude -c $(PRG) -p $(AVRDUDE_MCU)
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size --format=avr --mcu=$(MCU)
CC      = avr-gcc

# generate list of objects
CFILES    = $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
OBJ       = $(CFILES:.c=.o)

# user targets
# compile all files
all: $(PRJ).hex $(PRJ).bin

# test programmer connectivity
test:
	$(AVRDUDE) -v

# flash program to mcu
flash: all
	$(AVRDUDE) -U flash:w:$(PRJ).hex:i

# write fuses to mcu
fuse:
	$(AVRDUDE) -U lfuse:w:$(LFU):m -U hfuse:w:$(HFU):m -U efuse:w:$(EFU):m

# generate disassembly files for debugging
disasm: $(PRJ).elf
	$(OBJDUMP) -d $(PRJ).elf

# remove compiled files
clean:
	-rm -f *.hex *.elf *.bin
	-$(foreach dir, $(SRC), rm -f $(dir)/*.o)

# other targets
# objects from c files
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# elf file
$(PRJ).elf: $(OBJ)
	$(CC) $(CFLAGS) -o $(PRJ).elf $(OBJ)

# hex file
$(PRJ).hex: $(PRJ).elf
	$(OBJCOPY) -j .text -j .data -O ihex $(PRJ).elf $(PRJ).hex
	$(SIZE) $(PRJ).elf
	
# hex file
$(PRJ).bin: $(PRJ).elf
	$(OBJCOPY) -j .text -j .data -O binary $(PRJ).elf $(PRJ).bin
	$(SIZE) $(PRJ).elf