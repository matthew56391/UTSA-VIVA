# Lab 8 – I2C LCD Interface (PCF8574 Backpack)

**PIC16F1829 | UTSA VIVA Board | 4 MHz INTOSC**  
**Interfaces:** I2C (MSSP) → 16×2 LCD via PCF8574 I2C backpack

## Overview
This lab demonstrates use of the PIC16F1829 I2C peripheral to control a 16×2
character LCD through a PCF8574 I2C backpack. The program initializes I2C in
master mode, initializes the LCD, and then performs a series of display and
command demonstrations including text output, ASCII character display, formatted
strings, and LCD shifting.

---

## Functionality
After initialization, the firmware runs the following LCD demonstrations:

1. **Hello World**
   - Clears the LCD and prints `"Hello World"` on line 2

2. **ASCII A–Z Display**
   - Prints `A` through `Z` sequentially with a delay
   - Wraps to line 2 after `P` so `Q–Z` remain visible on a 16×2 display

3. **Challenge: “Hello UTSA”**
   - Clears the display and prints `"Hello UTSA"`

4. **Challenge: Simple Math Output**
   - Displays a formatted expression: `100 + 50 = 150`

5. **LCD Shift Demonstration**
   - Displays two lines:
     - Line 1: `"Lab 8: I2C LCD"`
     - Line 2: `"Shift Demo"`
   - Continuously shifts the display right and left using LCD shift commands

---

## Hardware Configuration
- **Microcontroller:** PIC16F1829 (VIVA UTSA board)
- **Clock:** 4 MHz internal oscillator (INTOSC)
- **LCD:** 16×2 character LCD with **PCF8574 I2C backpack**
- **I2C Address:** `0x27` (common)  
  - Some backpacks use `0x3F` (update the define if needed)

---

## Firmware Notes

### I2C + LCD Drivers
This lab uses course-provided driver headers/source:
- `i2c.h` – I2C master initialization and transactions
- `i2c_LCD.h` – LCD over I2C helper functions (init, positioning, writes, commands)

### LCD Commands Used
- Clear display: `0x01`
- Set DDRAM position:
  - Line 1 start: `0x00`
  - Line 2 start: `0x40`
- Shift display:
  - Shift right: `0x1C`
  - Shift left:  `0x18`

---

## Files
- `Lab8_I2C_LCD.c` – Main firmware source file  
- `i2c.h`, `i2c_LCD.h` – Course-provided I2C and LCD driver interfaces

---

## Notes
- Demonstrations run sequentially once, then enter an infinite left/right shift loop
- Designed for hardware validation on the VIVA UTSA board using an external I2C LCD module

---

## Disclaimer
This lab was developed for academic coursework as part of **Microcontrollers I (VIVA)**
at the University of Texas at San Antonio and is shared for educational and reference purposes.

