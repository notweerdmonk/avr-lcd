# AGENTS.md - Development Guidelines for avr-lcd

This file provides guidance for AI agents working on this AVR HD44780 LCD driver project.

## Project Overview

AVR embedded C library for driving HD44780-compatible character LCD displays. Uses a Makefile-based build system with simavr simulation and hardware flashing support.

## Build Commands

### Full Build
```bash
make              # Build library and tests
make build-lib    # Build library only (lib/lcd.a)
make build-tests  # Build test targets
make clean        # Clean all build artifacts
```

### Single Test Target
```bash
cd tests/target && make           # Build specific test
cd tests/target && make clean && make  # Rebuild after changes
```

### Simulation (simavr)
```bash
make SIM=1              # Compile for simulation
make sim                # Run with simavr
make sim-gdb            # Run with simavr in GDB mode
```

### Hardware Flashing
```bash
make flash              # Build and flash to AVR
make fuse               # Set AVR fuses
```

### Code Analysis
```bash
make cpp                # Preprocess only (.i files)
make disasm             # Generate disassembly
make disasmall          # Full disassembly
make size               # Show memory usage
```

### Build Flags
| Flag | Default | Description |
|------|---------|-------------|
| `DEVICE` | atmega328p | Target AVR device |
| `CLOCK` | 16000000 | CPU frequency (Hz) |
| `DEBUG` | - | Enable debug build |
| `SIM` | - | Compile for simulation |
| `SIMTEST` | - | Compile for off-target testing |
| `RUNTIMECONF` | - | Enable runtime pin config |
| `OPTIM` | -Os | Optimization level |

## Code Style Guidelines

### File Organization
- Headers: `include/`, `config/`, `port/` directories
- Source: `src/` directory
- Tests: `tests/target/` directory
- Guards: `#ifndef _FILENAME_H_ / #define _FILENAME_H_ / #endif`

### Naming Conventions
- Public API functions: snake_case with `avr_lcd_` prefix (e.g., `avr_lcd_put_string()`)
- Private/hardware functions: snake_case with `lcd_`, `hd44780_`, or `port_` prefix
- Types: structs without suffix, typedefs with `_t` suffix, enums lowercase
- Macros: UPPERCASE with `AVR_LCD_` prefix for public feature flags
- Variables: snake_case, use `static` for module state

### Formatting
- 2 spaces for indentation (no tabs)
- 80-100 column limit
- Opening brace on same line: `if (condition) {`
- Use C-style `/* */` for block comments
- Use `//` for feature flags (easier to uncomment)

### Include Order
1. Standard C/AVR headers (`<avr/io.h>`, `<util/delay.h>`, `<avr/interrupt.h>`)
2. Project config (`<config.h>`)
3. Public module headers (`<avr_lcd.h>`, `<hd44780.h>`)
4. Private/hardware headers (`<port.h>`, `<avr_io.h>`)

**Always define `F_CPU` before including AVR headers.**

### Function Documentation (Doxygen)
All public API functions must have Javadoc-style comments:
```c
/**
 * @brief Short description of function
 * @param param_name Description of parameter
 * @return Description of return value
 */
void example_function(uint8_t param);
```

Use these Doxygen tags:
- `@brief` - Brief one-line description
- `@param` - Parameter description
- `@return` - Return value description
- `@name` - Grouping related functions
- `@defgroup` - Define documentation group

### Error Handling
- Return status booleans: `bool avr_lcd_ready()`
- Validate input parameters at function entry
- Use compile-time config to reduce runtime checks
- Use `NULL` pointer checks where appropriate

### Memory Considerations (AVR-specific)
- Use `PROGMEM` for constant data in flash memory
- Use `pgm_read_byte()` for reading from flash
- Minimize RAM usage with static allocation
- Use bitfields for boolean flags
- Prefer `uint8_t` over `int` for AVR

## Port Macros Usage

The `port_mega328p.h` header provides macros for hardware abstraction. Use these instead of naked register access:

```c
// Timer macros
PORT_TIMER2_FAST_PWM_1();
PORT_TIMER2_SET_CS(32);
PORT_TIMER2_SET_OCR(A, value);

// Pin macros
OUTPUT_PIN_NUMBER(15);    // Set pin as output (absolute pin number)
SET_PIN_NUMBER(15);        // Set pin high
CLR_PIN_NUMBER(15);       // Clear pin low
```

Available port macro categories:
- Port/pin manipulation: `OUTPUT_PIN_NUMBER`, `SET_PIN_NUMBER`, `CLR_PIN_NUMBER`
- Timer/Counter: `PORT_TIMER0_*`, `PORT_TIMER1_*`, `PORT_TIMER2_*`
- ADC: `PORT_ADC_*`
- USART: `PORT_USART_*`
- Sleep modes: `PORT_SLEEP_MODE_*`

## Configuration

### Feature Flags (include/avr_lcd_config.h)
| Flag | Description |
|------|-------------|
| `AVR_LCD_4BIT` | Use 4-bit data bus mode |
| `AVR_LCD_BUFFERED` | Enable buffered operation |
| `AVR_LCD_RUNTIME_HW_REPR_SEL` | Runtime pin configuration |

### Project Flags (config/config.h)
| Flag | Description |
|------|-------------|
| `AVR_LCD_SIMULATION` | Compile for simavr simulation |
| `AVR_LCD_SIMTEST` | Compile for off-target testing |
| `AVR_LCD_DEBUG` | Enable debug symbols |

## Testing

### Test Location
All tests are in `tests/target/`. Build and run tests:
```bash
cd tests/target && make          # Build test
make sim                         # Run with simavr
make sim-gdb                    # Run with GDB
```

### Simulation with VCD
```bash
make SIM=1             # Compile with VCD trace
make sim               # Run simulation
# View trace in gtkwave
```

## Important Notes

- Use `LCD_PREFIX(x)` macro for hardware abstraction layer
- Build system expects `INCLUDE_DIRS` to include: `config`, `include`, `port`
- Follow existing code patterns for consistency
- Public API functions MUST use `avr_lcd_` prefix
- Feature flags MUST use `AVR_LCD_` prefix
- Always verify build after changes: `make clean && make`
- Generate documentation after changes: `doxygen`
