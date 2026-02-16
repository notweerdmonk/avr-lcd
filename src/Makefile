# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = atmega328p
CLOCK      = 16000000
PROGRAMMER = -c arduino -P /dev/ttyACM*
OBJECTS    = lcd.o
SOURCES    = lcd.c


######################################################################
######################################################################

# Tune the lines below only if you know what you are doing:

INCLUDE_FLAGS := $(INCLUDE_FLAGS)
COMPILE = avr-gcc -g -save-temps -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(INCLUDE_FLAGS)
COMPILE_SAVE_TEMPS = avr-gcc -save-temps -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all: $(OBJECTS)

.c.o: 
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

clean:
	rm lcd.i lcd.s -f $(OBJECTS)

# file targets:
$(OBJECTS): $(SOURCES)

# Targets for code debugging and analysis:
disasm:	lcd.o
	avr-objdump -d lcd.o > lcd.lst

disasmall: lcd.o
	avr-objdump -D lcd.o > lcd.lst

cpp:
	$(COMPILE) -E lcd.c

%.lst: %.c
	$(COMPILE) -c -g -Wa,-adhln $< > $@

asm_listing:
	$(COMPILE) -S $<
