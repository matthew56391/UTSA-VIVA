VIVA Final Project – RTC Time Zone Clock

PIC16F1829 | I2C RTC + I2C LCD

## Overview

This project is the final project for the VIVA (Microcontrollers I) course at UTSA. It implements a real-time clock using an external RTC module over I2C and displays the current time and date on an I2C LCD. A pushbutton input allows the user to cycle through major U.S. time zones in real time.

## Functionality

The system runs on a PIC16F1829 configured for a 4 MHz internal oscillator. Time data is read from a DS3231 or DS1307 real-time clock over I2C and displayed on a 16×2 LCD using a PCF8574 I2C backpack. A button connected to RB7 cycles through Pacific, Mountain, Central, and Eastern time zones by applying the appropriate offset to the RTC hour value. The formatted time and date are displayed on the LCD and also transmitted over UART for debugging and verification.

## Hardware

The project was implemented on the VIVA UTSA board using a PIC16F1829 microcontroller. An external DS3231 or DS1307 RTC module is connected via I2C, along with a 16×2 I2C LCD. A momentary pushbutton connected to RB7 is used for time zone selection.

## Hardware Demonstration

The images below show the final project implemented on the VIVA UTSA board.
They demonstrate system wiring, peripheral integration, and real-time clock
operation across multiple U.S. time zones using an external I2C RTC and I2C LCD.

### System Overview
![Board Overview](images/board_overview.jpg)

### Time Zone Demonstration
![Eastern Time](images/rtc_est.jpg)
![Central Time](images/rtc_cst.jpg)
![Mountain Time](images/rtc_mst.jpg)
![Pacific Time](images/rtc_pst.jpg)

## Repository Contents

This repository contains the main firmware source file for the final project along with the I2C and I2C LCD driver headers and source files used in the course. These drivers handle low-level communication with the RTC and LCD hardware.

## Development Notes

The firmware is based on the course-provided skeleton code and team development. My primary contributions focused on system integration, mechanical and enclosure design, validation on hardware, and code cleanup for documentation and public release. Unused experimental sections and inappropriate debug content were removed while preserving the original working functionality of the system.

## Disclaimer

This project was developed for academic purposes as part of coursework and is shared for reference and educational use.
