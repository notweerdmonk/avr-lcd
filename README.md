# avr-lcd

A modular, lightweight LCD library for AVR microcontrollers.

## Features

- **HD44780 Support**: Compatible with standard character LCD controllers
- **4-bit/8-bit Modes**: Flexible data bus configuration
- **Buffered Operation**: Non-blocking writes with automatic dirty tracking
- **Hardware Flexibility**: Multiple pin mapping modes (port addresses, relative/absolute pin numbers)
- **Runtime Configuration**: Dynamic pin assignment at runtime
- **Simulation Support**: Works with simavr for testing
- **Portable**: Port abstraction layer for different AVR variants

## Requirements

- avr-gcc toolchain
- avr-libc
- simavr (for simulation testing)
- avrdude (for flashing to hardware)
- Doxygen (for documentation generation)

## Quick Start

```c
#include <avr_lcd.h>

int main(void) {
    avr_lcd_set_pins(NULL);                             // Use macro-defined pins
    avr_lcd_reset();
    avr_lcd_setup();
    
    avr_lcd_put_string("Hello, World!");
    avr_lcd_set_cursor(1, 0);
    avr_lcd_put_string("AVR LCD v0.1");
    
#ifdef AVR_LCD_BUFFERED
    avr_lcd_display();
#endif
    
    while(1);
    return 0;
}
```

## Installation

There are two ways to use the avr-lcd library in your project:

### Option 1: Use Precompiled Library

Use the API headers and link to the precompiled static library:

1. Copy `include/`, `port/` directories to your project
2. Copy `lib/liblcd.a` (generated after building) to your project
3. Add include paths: `-Iinclude -Iport`
4. Link with: `-L. -llcd`

```bash
# Build the library first
make build-lib

# In your project:
avr-gcc -mmcu=atmega328p -Iinclude -Iport your_main.c -L. -llcd -o your_project.elf
```

This approach is simpler but you cannot customize the library features at compile time.

**Note:** To enable runtime hardware representation in the precompiled library, build with:

```bash
RUNTIMECONF=1 make build-lib
```

Then in your code:

```c
#define AVR_LCD_RUNTIME_HW_REPR_SEL
#include <avr_lcd.h>

struct hardware_repr hw = {
    // pin configuration
};
avr_lcd_set_pins(&hw);
avr_lcd_reset();
avr_lcd_setup();
```

### Option 2: Compile from Source

Copy the source files into your project and compile along with your code:

1. Copy `include/`, `src/`, and `port/` directories to your AVR project
2. Add `src/avr_lcd.c` to your build
3. Configure your build system to include the appropriate include paths
4. Define feature flags (like `AVR_LCD_BUFFERED`, `AVR_LCD_4BIT`, etc.) in your build

```bash
# In your project Makefile:
CFLAGS += -I$(LCD_DIR)/include -I$(LCD_DIR)/port -DAVR_LCD_BUFFERED
SRCS += $(LCD_DIR)/src/avr_lcd.c
```

This approach allows you to customize library features at compile time.

## Configuration

### Configuration Files

| File | Purpose |
|------|---------|
| `include/avr_avr_lcd_config.h` | LCD feature flags (bus mode, buffered mode, display dimensions) |
| `config/config.h` | Project compilation flags (simulation, debug, optimization) |

### Build Flags

Pass these as `make FLAG=value`:

| Flag | Default | Description |
|------|---------|-------------|
| `DEVICE` | atmega328p | Target AVR device |
| `CLOCK` | 16000000 | CPU clock frequency in Hz |
| `DEBUG` | (unset) | Enable debug build |
| `SIM` | (unset) | Compile for simulation |
| `SIMTEST` | (unset) | Compile for off-target testing |
| `OPTIM` | -Os | Compiler optimization level |

## Building

Build can be triggered from any subdirectory in the project. The Makefiles automatically discover the project root using relative paths.

### From Project Root

```bash
make              # Build library + tests
make build-lib    # Build library only
make build-tests  # Build tests only
make clean        # Clean all
```

### From Any Subdirectory

```bash
make -C src              # Build library
cd src && make           # Or use cd

make -C tests            # Build all tests
cd tests && make         # Or use cd

make -C tests/target     # Build target firmware
cd tests/target && make  # Or use cd
```

### Running Simulation

The project uses simavr for firmware simulation:

```bash
make SIM=1              # Compile for simulation
make sim                # Run with simavr
make sim-gdb            # Run with simavr in GDB mode
```

### Flashing to Hardware

```bash
make flash              # Build and flash to AVR
make fuse               # Set AVR fuses
```

### Other Build Targets

```bash
make clean              # Clean all build artifacts
make cpp                # Preprocess only (generates .i files)
make disasm             # Generate disassembly
make disasmall          # Generate full disassembly
make size               # Show memory usage
make help               # Show available targets
```

### Generating Documentation

```bash
doxygen                 # Generate HTML and LaTeX documentation
```

Output is generated in `docs/html/` (HTML) and `docs/latex/` (LaTeX).

## Setting Feature and Compilation Flags

There are three ways to enable features and control compilation:

### 1. Via config/config.h

Uncomment the desired definitions in `config/config.h`:

```c
/* Enable runtime UART configuration (call uart_setup with config struct) */
// #define AVR_LCD_RUNTIME_CONFIG

/* Enable use of UART in I/O streams (stdin/stdout/stderr) */
// #define AVR_LCD_UART_IOSTREAM

/* Enable UART input pattern match */
// #define AVR_LCD_UART_MATCH

/* Use strncmp for pattern matching */
// #define AVR_LCD_STRNCMP_MATCH

/* Emit a trigger signal that can be used by logic analyser to start capture */
// #define AVR_LCD_EMIT_TRIGGER

/* SIM denotes that source code will compiled for simulation */
// #define AVR_LCD_SIMULATION

/* Denotes that source code will be compiled for off-target testing */
// #define AVR_LCD_SIMTEST

/* Demo mode with serial communication program */
// #define AVR_LCD_DEMO

/* Enable debug build with debugging information and symbols */
// #define AVR_LCD_DEBUG

/* Preserve compilation intermediaries */
// #define AVR_LCD_SAVE_TEMPS

/* Set the compiler optimization level (-O0, -O1, -O2, -O3, -Os, etc.) */
// #define AVR_LCD_OPTIM -Os
```

### 2. Via Command Line

Pass flags to make:

```bash
SIM=1 make              # Compile for simulation
SIMTEST=1 make          # Compile for off-target testing
DEBUG=1 make            # Debug build
SAVETEMPS=1 make        # Preserve intermediate files
OPTIM=-O2 make          # Set optimization level
```

### 3. Via Code

Define flags directly in your source code before including avr_lcd.h:

```c
#define AVR_LCD_BUFFERED
#define AVR_LCD_CURSOR_ON
#include <avr_lcd.h>
```

## Feature Flags

Feature flags are defined in `include/avr_avr_lcd_config.h`. All feature flags are prefixed with `AVR_` to avoid conflicts with user code.

### Display Configuration

| Flag | Default | Description |
|------|---------|-------------|
| `AVR_LCD_TYPE` | hd44780 | LCD controller type |
| `AVR_LCD_ROWS` | 2 | Number of display rows |
| `AVR_LCD_COLS` | 16 | Number of display columns |

### Bus Mode

| Flag | Description |
|------|-------------|
| `AVR_LCD_4BIT` | Use 4-bit data bus (saves 4 I/O pins) |

If `AVR_LCD_4BIT` is not defined, 8-bit mode is used.

### Buffered Mode

| Flag | Default | Description |
|------|---------|-------------|
| `AVR_LCD_BUFFERED` | defined | Enable buffered operation |

When enabled, character writes go to a RAM buffer. Call `avr_lcd_display()` to flush the buffer to the LCD. Benefits:
- Faster character writes (no blocking LCD wait)
- Automatic dirty character tracking

### Hardware Representation Modes

Choose ONE mode that matches your wiring:

| Flag | Description |
|------|-------------|
| `AVR_LCD_RUNTIME_HW_REPR_SEL` | Runtime pin configuration via `avr_lcd_set_pins()` |
| `AVR_LCD_USE_PORT_ADDR` | Use direct port addresses |
| `AVR_LCD_USE_RELATIVE_PIN_NUMBERS` | Use port numbers (PORTB=1, PORTC=2, etc.) |
| `AVR_LCD_USE_ABSOLUTE_PIN_NUMBERS` | Use Arduino-style pin numbers (0-21) |
| `AVR_LCD_USE_NONCONTIGUOUS_DATA_PINS` | Data pins can be non-contiguous |
| `AVR_LCD_USE_SEPARATE_PORTS` | Control signals on different ports |

### Display Options

| Flag | Description |
|------|-------------|
| `AVR_LCD_DISPLAY_OFF` | Start with display off |
| `AVR_LCD_CURSOR_ON` | Enable cursor display |
| `AVR_LCD_BLINK_ON` | Enable cursor blink |
| `AVR_LCD_ENTRY_DECR` | Decrement cursor on write |
| `AVR_LCD_ENTRY_SHIFT` | Shift display with cursor |

### Hardware Wiring

When not using runtime configuration, define port assignments:

```c
#define AVR_LCD_CTL_PORT B                              // Control port
#define AVR_LCD_DATA_PORT D                             // Data port
```

## Compilation Flags

Compilation flags are defined in `config/config.h`. These control build behavior:

| Flag | Description |
|------|-------------|
| `AVR_LCD_RUNTIME_CONFIG` | Enable runtime UART configuration |
| `AVR_LCD_UART_IOSTREAM` | Enable UART in I/O streams |
| `AVR_LCD_UART_MATCH` | Enable UART input pattern match |
| `AVR_LCD_SIMULATION` | Compile for simulation |
| `AVR_LCD_SIMTEST` | Compile for off-target testing |
| `AVR_LCD_DEMO` | Demo mode |
| `AVR_LCD_DEBUG` | Enable debug build |
| `AVR_LCD_SAVE_TEMPS` | Preserve compilation intermediaries |
| `AVR_LCD_OPTIM` | Compiler optimization level |

## API Reference

### Initialization

```c
void avr_avr_lcd_set_pins(struct hardware_repr *p);     // Set pin configuration
void avr_avr_lcd_reset(void);                           // Reset LCD controller
void avr_avr_lcd_setup(void);                           // Initialize LCD (no runtime config)
void avr_avr_lcd_setup(uint8_t entry_mode,              // Initialize LCD (runtime config)
               uint8_t display,
               uint8_t cursor_shift,
               uint8_t function);
```

### Display Control

```c
void avr_avr_lcd_clear(void);                           // Clear display
void avr_avr_lcd_clear_till(uint8_t n);                 // Clear n characters from cursor
void avr_avr_lcd_display(void);                         // Flush buffer to LCD (buffered mode)
```

### Cursor Control

```c
struct cursor avr_avr_lcd_get_cursor(void);             // Get cursor position
void avr_avr_lcd_set_cursor(uint8_t row, uint8_t col);  // Set cursor position
```

### Output Functions

```c
void avr_avr_lcd_put_char(char c);                      // Write single character
void avr_avr_lcd_put_string(char *str);                 // Write string
void avr_avr_lcd_put_pgm_string(PGM_P str);             // Write PROGMEM string
void avr_avr_lcd_put_uint(unsigned int u);              // Write unsigned integer
void avr_avr_lcd_put_int(int n);                        // Write signed integer
void avr_avr_lcd_put_float(float f, uint8_t m);         // Write float with m decimal places
```

### Backlight Control

```c
/**
 * PWM callback function type for backlight control
 * @param value PWM value (0-255)
 */
typedef void (*avr_avr_lcd_pwm_set_value_hook)(uint8_t value);

/**
 * @brief Register PWM callback for backlight control
 * @param hook Function pointer to PWM setter callback
 * @return Previous callback function pointer
 */
avr_avr_lcd_pwm_set_value_hook avr_avr_lcd_set_pwm_set_value_hook(avr_avr_lcd_pwm_set_value_hook hook);

/**
 * @brief Set backlight brightness
 * @param value Brightness value (0-255)
 */
void avr_lcd_set_brightness(uint8_t value);
```

**Example Usage:**

```c
// Define your PWM setter function (e.g., for Timer1 OC1A)
void my_pwm_set_value(uint8_t value) {
    OCR1A = value;  // Set PWM compare register
}

// Register the callback before using brightness
int main(void) {
    avr_avr_lcd_set_pwm_set_value_hook(my_pwm_set_value);
    
    avr_avr_lcd_set_pins(NULL);
    avr_avr_lcd_reset();
    avr_avr_lcd_setup();
    
    // Set brightness (0-255)
    avr_avr_lcd_set_brightness(128);                    // 50% brightness
    
    avr_avr_lcd_put_string("Hello!");
    
    while(1);
    return 0;
}
```

### Utility

```c
bool avr_avr_lcd_ready(void);                           // Check if LCD is ready
```

### Character Constants

```c
#define LCD_CLEAR_LINE()                                // Clear current line
```

## Wiring Diagram

### Typical 16x2 LCD in 4-bit Mode

```
                    ATmega328P
                   +-----------+
                   |        PB0|<-- D4
                   |        PB1|<-- D5
                   |        PB2|<-- D6
                   |        PB3|<-- D7
                   |           |
                   |        PB4|<-- RS (Register Select)
                   |        PB5|<-- EN (Enable)
                   |        PB6|<-- RW (Read/Write, optional)
                   |        PB7|<-- BL (Backlight PWM, optional)
                   +-----------+
                        |
                        v
              +-------------------+
              |    HD44780 LCD    |
              |                   |
              |  VSS -- GND       |
              |  VDD -- +5V       |
              |  V0  -- Contrast  |
              |  RS  -- PB4       |
              |  RW  -- GND/PB6   |
              |  EN  -- PB5       |
              |  D0-D3 -- NC      |
              |  D4  -- PB0       |
              |  D5  -- PB1       |
              |  D6  -- PB2       |
              |  D7  -- PB3       |
              |  A   -- +5V/PB7   |
              |  K   -- GND       |
              +-------------------+
```

**Note:** PB7 is shown as optional backlight PWM. Connect to a PWM-capable pin for brightness control. Use the callback registration API to enable software-controlled brightness.

## Example Usage

### Basic Setup (Macro Configuration)

```c
#include <avr_lcd.h>

int main(void) {
    avr_lcd_set_pins(NULL);                             // Use macro-defined pins
    avr_lcd_reset();
    avr_lcd_setup();
    
    avr_lcd_put_string("Hello, World!");
    avr_lcd_set_cursor(1, 0);
    avr_lcd_put_string("AVR LCD v0.1");
    
#ifdef AVR_LCD_BUFFERED
    avr_lcd_display();
#endif
    
    while(1);
    return 0;
}
```

### Runtime Pin Configuration

```c
#include <avr_lcd.h>

int main(void) {
    avr_lcd_set_pins(&(struct hardware_repr) {
        .mode = ABSOLUTE_PIN_NUMBERS | BUS_4BIT,
        .ctl.rs = 8,
        .ctl.en = 9,
        .ctl.rw = PIN_NC,
        .ctl.bl = 10,
        .data.pins.d4 = 4,
        .data.pins.d5 = 5,
        .data.pins.d6 = 6,
        .data.pins.d7 = 7,
    });
    
    avr_lcd_reset();
    avr_lcd_setup(
        0,
        LCD_CMD_DISPLAY_CONTROL | LCD_CMD_DISPLAY_ON | LCD_CMD_CURSOR_ON,
        0,
        0
    );
    
    avr_lcd_put_string("Runtime config");
    
#ifdef AVR_LCD_BUFFERED
    avr_lcd_display();
#endif
    
    while(1);
    return 0;
}
```

### Displaying Sensor Data

```c
#include <avr_lcd.h>

void display_temperature(float temp) {
    avr_lcd_set_cursor(0, 0);
    avr_lcd_put_string("Temp: ");
    avr_lcd_put_float(temp, 1);                         // 1 decimal place
    avr_lcd_put_string(" C");
    
#ifdef AVR_LCD_BUFFERED
    avr_lcd_display();
#endif
}
```

### Backlight Brightness Control

```c
#include <avr_lcd.h>
#include <avr_io.h>
#include <util/delay.h>

// PWM callback for Timer2 (PB7/OC2A)
void set_backlight_pwm(uint8_t value) {
    PORT_TIMER2_SET_OCR(A, value);                      // Set PWM duty cycle
}

int main(void) {
    // Configure Timer2 for Fast PWM on OC2A (PB7)
    // WGM = 0b011 (Fast PWM, 8-bit)
    // COM2A = 0b10 (Clear OC2A on compare match)
    PORT_TIMER2_FAST_PWM_1();
    PORT_TIMER2_CLEAR_OC2A();
    
    // Clock select /32 (prescaler)
    PORT_TIMER2_SET_CS(32);
    
    // Enable OC2A output (PB7) - using absolute pin number 15
    OUTPUT_PIN_NUMBER(15);
    
    // Register backlight callback
    avr_avr_lcd_set_pwm_set_value_hook(set_backlight_pwm);
    
    avr_lcd_set_pins(NULL);
    avr_lcd_reset();
    avr_lcd_setup();
    
    // Set brightness levels
    avr_lcd_set_brightness(255);                        // Full brightness
    
    avr_lcd_put_string("Max brightness");
    _delay_ms(2000);
    
    avr_lcd_clear();
    avr_lcd_set_brightness(64);                         // ~25% brightness
    
    avr_lcd_put_string("Dimmed");
    
    while(1);
    return 0;
}
```

Alternatively, to toggle PB7 manually (without hardware PWM):

```c
#include <avr_lcd.h>
#include <avr_io.h>
#include <util/delay.h>

static volatile uint8_t brightness = 255;

// Timer2 overflow interrupt for software PWM
ISR(TIMER2_OVF_vect) {
    static uint8_t counter = 0;
    counter++;
    if (counter >= 255) {
        counter = 0;
    }
    if (counter < brightness) {
        SET_PIN_NUMBER(15);                             // LED on (PB7 = pin 15)
    } else {
        CLR_PIN_NUMBER(15);                             // LED off
    }
}

void set_backlight_pwm(uint8_t value) {
    brightness = value;
}

int main(void) {
    // Configure Timer2 for software PWM
    PORT_TIMER2_SET_CS(32);
    PORT_TIMER2_ENABLE_OVERFLOW_INTERRUPT();
    
    // Enable PB7 output (absolute pin 15)
    OUTPUT_PIN_NUMBER(15);
    
    // Register backlight callback
    avr_avr_lcd_set_pwm_set_value_hook(set_backlight_pwm);
    
    avr_lcd_set_pins(NULL);
    avr_lcd_reset();
    avr_lcd_setup();
    
    // Set brightness
    avr_lcd_set_brightness(128);                        // 50% brightness
    
    avr_lcd_put_string("50% brightness");
    
    while(1);
    return 0;
}
```

## Memory Usage

Typical memory usage with default configuration on ATmega328P:

| Configuration | Program | Data |
|---------------|---------|------|
| Buffered, 4-bit | ~1200 bytes | ~96 bytes |
| Unbuffered, 4-bit | ~982 bytes | ~32 bytes |

## Supported MCUs

- ATmega328P (tested)
- ATmega32 (partial support)

Adding support for additional MCUs requires creating a port file in `port/<mcu>/`.

## Simulation

The project supports simulation using [simavr](https://github.com/buserror/simavr).

### Running Simulation

```bash
# Build with SIM flag
SIM=1 make -C tests/target

# Run simulation
make -C tests/target sim

# Or run with GDB debug
make -C tests/target sim-gdb
```

### Off-Target Testing

For testing without actual hardware, use SIMTEST to compile for off-target testing:

```bash
make SIMTEST=1          # Compile for off-target testing
```

This allows running the target firmware test program in a simulated environment using simavr's HD44780 board.

#### Using HD44780 Board

The simavr HD44780 board emulates a 16x2 LCD. It loads the target firmware and emulates the LCD display.

**Prerequisites:**
- Build simavr from source
- Note the path to the compiled charlcd ELF file

**Running off-target tests:**

1. Build the target firmware with SIMTEST flag:
```bash
SIMTEST=1 make -C tests/target
```

2. Run HD44780 board with the target firmware:
```bash
/path/to/simavr/examples/board_hd44780/<simavr_build_target>/charlcd.elf ./tests/target/main.elf
```

**Note:** The path to charlcd.elf can vary depending on the host platform where simavr was built. It uses the `obj-$(shell $(CC) -dumpmachine)` format, e.g., `obj-x86_64-linux-gnu` on Debian Linux.

The simavr board will load the firmware and emulate a 16x2 LCD. The OpenGL UI displays the LCD content.

### VCD Trace Generation

When compiled with `SIM=1`, the target firmware can generate VCD (Value Change Dump) files that can be viewed in GTKWave. The traces include:
- PORTB signals
- Data lines (D4-D7)
- RS (Register Select)
- EN (Enable)
- RW (Read/Write)

### Viewing Simulation Results

Open the generated VCD file in GTKWave:

```bash
gtkwave simulation/avr_lcd_trace.vcd &
```

## Architecture

```
+------------------+
|    User Code     |
+--------+---------+
         |
         v
+--------+---------+
|    avr_lcd.h     |  <-- Public API
+--------+---------+
         |
         v
+--------+---------+
|    avr_lcd.c     |  <-- LCD module (buffered mode, cursor, etc.)
+--------+---------+
         |
         v
+--------+---------+
|   hd44780.c      |  <-- HD44780 controller driver
+--------+---------+
         |
         v
+--------+---------+
|  port/mega328p/  |  <-- MCU-specific port definitions
+------------------+
```

## Directory Structure

```
avr-lcd/
`-- AGENTS.md                   # Development guidelines for AI agents
|-- bashrc                      # Additional bash configuration for project
|-- config/
|   `-- config.h                # Project compilation flags
|-- docs/
|   |-- html/                   # Generated HTML documentation
|   `-- latex/                  # Generated LaTeX documentation
|-- Doxyfile                    # Doxygen configuration
|-- include/
|   `-- avr_ascii.h             # ASCII conversion utilities
|   |-- avr_io.h                # I/O utilities
|   |-- avr_avr_lcd_config.h    # LCD feature flags
|   |-- avr_lcd.h               # Public API header
|   |-- avr_utility.h           # General utilities
|   |-- hd44780_cmds.h          # HD44780 command definitions
|   `-- hd44780.h               # HD44780 driver header
|-- lib/                        # Built library output (created upon build)
|-- LICENSE
|-- mainpage.h
|-- Makefile
|-- Makefile.common
|-- port/
|   |-- mega328p/
|   |   `-- port_mega328p.h     # ATmega328P specific definitions
|   `-- port.h                  # Port abstraction
|-- README.md
|-- simulation/                 # VCD files from simulations
|-- src/
|   |-- avr_lcd.c               # LCD module implementation
|   |-- hd44780.c               # HD44780 driver implementation
|   `-- Makefile
`-- tests/
    |-- Makefile
    `-- target/
        |-- main.c              # Test target for simulation
        `-- Makefile
```

## Requirements

- avr-gcc toolchain
- avr-libc
- simavr (for simulation testing)
- avrdude (for flashing to hardware)
- Doxygen (for documentation generation)

## License

MIT License - see LICENSE file for details.

## Contributing

Contributions are welcome. Please ensure code follows the existing style:
- 2-space indentation
- snake_case for functions and variables
- UPPERCASE for macros
- Feature flags prefixed with `AVR_LCD_`
- Public API functions prefixed with `avr_lcd_`
- Use Javadoc-style comments (`/** ... */`) for documentation
- See `AGENTS.md` for development guidelines

---

Documentation updated with [OpenCode](https://opencode.ai) (minimax-m2.5-free model).
