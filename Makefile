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
OBJECTS    = main.o
LIBS       = lcd.o
SOURCES    = main.c
FUSES      = -U lfuse:w:0x64:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m


######################################################################
######################################################################

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude -v $(PROGRAMMER) -p $(DEVICE)

LIB_DIR = src

TEST_DIR =
TESTS =

TESTS := $(TEST_DIR)/$(TESTS)

INCLUDE = config port include
INCLUDE_FLAGS := $(foreach i, $(INCLUDE), -I$(CURDIR)/$i)
export

ifneq ($(LIB_DIR),)
LIBS := $(LIB_DIR)/$(LIBS)
endif

OPTIMIZATION = -Os

DEBUG_OPTIONS = -save-temps=obj -fstack-usage

ifneq ($(DEBUG),)
DEBUG_OPTIONS += -g
endif

CFLAGS += -fshort-enums -Wall 
CFLAGS += -DF_CPU=$(CLOCK) $(DEBUG_OPTIONS) $(OPTIMIZATION)

LD_FLAGS = -Wl, -Map=main.map

# dependancy lists
#ifneq ($(MAKECMDGOALS),clean)
#ifneq ($(strip $(C_DEPS)),)
#-include $(C_DEPS)
#endif
#endif

COMPILE = avr-gcc -mmcu=$(DEVICE) $(CFLAGS) $(INCLUDE_FLAGS) -c -MD -MP -MF "$(@:%.o=%.d)"

# symbolic targets:
all: main.hex
	avr-size -C --mcu=$(DEVICE) main.elf

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

tests: $(TESTS)

$(TESTS): $(TEST_DIR)

$(TEST_DIR):
	@echo Entering $@...
	$(MAKE) -C $@

flash: all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

#install: flash fuse
install: flash

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

ifneq ($(LIB_DIR),)
clean_libs:
	$(MAKE) -C $(LIB_DIR) clean
else
clean_libs:
endif

ifneq ($(TEST_DIR),)
clean_tests:
	$(MAKE) -C $(TEST_DIR) clean
else
clean_tests:
endif

clean: clean_libs clean_tests
	rm -f *.d *.s *.i *.su main.hex main.elf main.lst $(OBJECTS)

# file targets:
$(OBJECTS): $(SOURCES)

$(LIBS): $(LIB_DIR)

$(LIB_DIR):
	$(MAKE) -C $@

.PHONY: $(LIBS) $(LIB_DIR) $(TESTS) $(TEST_DIR)

main.elf: $(OBJECTS) $(LIBS)
	avr-gcc -mmcu=$(DEVICE) $(CFLAGS) $(INCLUDE_FLAGS) -Wl,-Map=main.map -o $@ $(OBJECTS) $(LIBS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf > main.lst

disasmall:	main.elf
	avr-objdump -D main.elf > main.lst

cpp:
	$(COMPILE) -E main.c

%.lst: %.c
	$(COMPILE) -c -g -Wa,-adhln $< > $@

asm_listing:
	$(COMPILE) -S $<
