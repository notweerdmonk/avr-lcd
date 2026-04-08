export AVR_LCD_DEP_LIBS_MODULES := avr-portable avr-utils

ifeq ($(strip $(AVR_LCD_ROOT_DIR)),)
AVR_LCD_ROOT_DIR := $(PROJECT_ROOT)
endif

ifeq ($(strip $(AVR_LCD_LIB_DIR)),)
AVR_LCD_LIB_DIR := $(LIB_DIR)
endif

export AVR_LCD_INCLUDE_DIRS = \
$(foreach d,config include port,$(AVR_LCD_ROOT_DIR)/$(d)) \
$(foreach d, $(AVR_LCD_DEP_LIBS_MODULES), $(AVR_LCD_LIB_DIR)/$(d)/include) \
$(foreach d, $(AVR_LCD_DEP_LIBS_MODULES), $(AVR_LCD_LIB_DIR)/$(d)/port)

# This variable is defined at the top-level (project root level) Makefile to
# include the required upper directories containing header files.
# It needs to be defined on the command line to run make from any lower
# directory.
INCLUDE_DIRS += $(foreach i, $(AVR_LCD_INCLUDE_DIRS), $(realpath $(i)))
INCLUDE_HEADERS = $(foreach dir, $(abspath $(INCLUDE_DIRS)), $(wildcard $(dir)/*.h))
INCLUDE += $(foreach i, $(INCLUDE_DIRS), -I$(realpath $(i)))

ifeq ($(strip $(BUILD_LIB)),)
override CFLAGS += $(INCLUDE)
LDFLAGS += -L$(AVR_LCD_LIB_DIR)/lib
LDLIBS += -llcd
endif
