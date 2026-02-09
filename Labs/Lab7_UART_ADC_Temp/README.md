Lab 7 – UART, ADC, and Internal Temperature Sensor

PIC16F1829 | UTSA VIVA Board | 4 MHz INTOSC

Overview

This lab demonstrates simultaneous use of multiple on-chip peripherals on the PIC16F1829, including the ADC, internal temperature sensor, Fixed Voltage Reference (FVR), and EUSART (UART). Analog measurements are read and processed in real time, then transmitted over UART for monitoring and verification.

The lab integrates analog sensing, serial communication, and basic decision logic using LEDs.

Functionality

The firmware performs the following operations in a continuous loop:

Reads a potentiometer connected to AN11 (RB5) using the ADC

Reads the internal temperature sensor via the ADC temperature channel

Reads the Fixed Voltage Reference (2.048 V) as an ADC input

Converts internal temperature readings to degrees Fahrenheit using the lab-provided linearization equation

Transmits ADC and temperature values over UART at 9600 baud

Compares temperature against potentiometer value to drive LEDs (active-LOW)

UART output is printed approximately twice per second for real-time observation.

Hardware Configuration

Microcontroller: PIC16F1829

Clock: 4 MHz internal oscillator (INTOSC)

UART:

TX → RC4

RX → RC5

Baud rate: 9600

ADC Inputs:

Potentiometer: AN11 (RB5)

Internal temperature sensor (ADC channel 0x75)

Fixed Voltage Reference (FVR, 2.048 V)

LED Outputs (active-LOW):

Green → RA5

Blue → RA2 (unused in Part A)

Red → RC6

ADC Configuration

Vref+ = VDD

Vref− = VSS

Right-justified result

ADC clock = FOSC / 4

Internal temperature sensor enabled in low range

Fixed Voltage Reference enabled at 2.048 V

Temperature Processing

The internal temperature sensor reading is converted to Celsius using the lab-specified linearization formula and then converted to Fahrenheit:

Celsius → calculated from ADC reading

Fahrenheit → (C × 9/5) + 32

Temperature and ADC values are transmitted via UART for debugging and validation.

LED Logic

If temperature ADC value > potentiometer ADC value

Red LED ON

Green LED OFF

Otherwise

Green LED ON

Red LED OFF

(LEDs are wired active-LOW.)

Files

Lab7_UART_ADC_Temp.c – Main firmware source file

Notes

Firmware uses polling (no interrupts)

Based on UTSA VIVA course skeleton code

Code cleaned and documented for clarity and public reference

Disclaimer

This lab was developed for academic coursework as part of Microcontrollers I (VIVA) at the University of Texas at San Antonio. It is shared for educational and reference purposes.
