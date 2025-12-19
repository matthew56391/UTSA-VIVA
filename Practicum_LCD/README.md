# Practicum_LCD (PIC16F1829 + SSD1306)

Microcontroller practicum project for the UTSA VIVA board using a PIC16F1829.
Uses I2C to drive an SSD1306 OLED and reads the onboard potentiometer via ADC (AN11/RB5).

## What it does
1. Displays the UTSA splash screen for ~2 seconds
2. Displays the student name for ~2 seconds
3. Continuously displays the potentiometer reading as `Pot=xxxx` on the OLED

## Hardware / Setup
- Board: UTSA VIVA (PIC16F1829)
- OLED: SSD1306 (I2C)
- Potentiometer: AN11 (RB5)

## Files
- `PRACTICUM.c` — main application (OLED + ADC logic)
- `i2c.h` — I2C interface used by the project
- `SSD1306.h` — SSD1306 OLED interface used by the project

## Build notes
Designed for MPLAB X + XC8 with the provided course/VIVA support files.
