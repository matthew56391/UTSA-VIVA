/*
 * File:   Lab7_F25.c
 * Author: Matthew Cuebas
 * Course: Microcontrollers I (F25) ? UTSA VIVA Board (PIC16F1829 @ 4 MHz INTOSC)
 *
 * Lab 7 ? UART + ADC + Internal Temperature Sensor
 * - Reads POT on AN11 (RB5)
 * - Reads Internal Temperature Sensor (ADC channel 0x75 / CHS=11101)
 * - Prints values over UART at 9600 baud
 * - (If required) compares Temp vs Pot to drive LEDs (active-LOW)
 */

// ======================= CONFIG =======================
// CONFIG1
#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
// CONFIG2
#pragma config WRT = OFF
#pragma config PLLEN = OFF
#pragma config STVREN = ON
#pragma config BORV = LO
#pragma config LVP = ON

#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000UL
#define BAUD   9600UL
#define FOSC   4000000UL
#define DIVIDER ((int)(FOSC/(16UL*BAUD)-1))  // 25 at 4 MHz & 9600
#define NINE_BITS 0
#define SPEED 0x4
#define RX_PIN TRISC5
#define TX_PIN TRISC4

// ---- Prototypes
void pinConfig(void);
void setup_comms(void);
void putch(char);

// ---- Globals
unsigned int Temp, FVval, Pot;
double Ctemp;

void main(void)
{
    pinConfig();
    setup_comms();

    // ----- FVR to ADC = 2.048 V -----
    FVRCONbits.FVREN  = 1;     // enable FVR
    FVRCONbits.ADFVR  = 0b10;  // 2.048 V to ADC
    FVRCONbits.CDAFVR = 0;     // DAC not used
    __delay_ms(5);             // settle

    // ----- Internal Temp Sensor enable -----
    TSEN  = 1;
    TSRNG = 0;                 // low range

    // ----- ADC: Vref+ = Vdd, Vref- = Vss, right-just, Fosc/4 -----
    ADCON1bits.ADPREF = 0b00;  // *** use Vdd as +Vref ***
    ADCON1bits.ADNREF = 0;     // Vss as -Vref
    ADCON1bits.ADFM   = 1;     // right-justified
    ADCON1bits.ADCS   = 0b011; // Fosc/4

    while (1)
    {
        // ---- Read Pot (AN11 / RB5) ----
        ADCON0 = 0x2D;               // CHS=1011 (AN11), ADON=1
        __delay_ms(10);
        ADGO = 1; while (ADGO);
        Pot = ((ADRESH << 8) | ADRESL);

        // ---- Read FVR channel ----
        ADCON0 = 0x7D;               // CHS=11111 (FVR), ADON=1
        __delay_ms(10);
        ADGO = 1; while (ADGO);
        FVval = ((ADRESH << 8) | ADRESL);

        // ---- Read Internal Temp channel ----
        ADCON0 = 0x75;               // CHS=11101 (Temp), ADON=1
        __delay_ms(10);
        ADGO = 1; while (ADGO);
        Temp = ((ADRESH << 8) | ADRESL);

        // ---- Temperature math (lab linearization) ----
        Ctemp = ((0.659 - (1.48 * (1 - ((float)Temp/1023.0)))) / 0.00132) - 40.0;
        double Ftemp = (Ctemp * 9.0 / 5.0) + 32.0;

        // ---- LED threshold: Temp vs Pot (ACTIVE-LOW) ----
        // Green = RA5, Blue = RA2 (OFF), Red = RC6
        RA2 = 1;                      // keep BLUE off for Part A
        if (Temp > Pot) {
            // hotter -> RED ON, GREEN OFF
            RC6 = 0;                  // Red ON   (active-LOW)
            RA5 = 1;                  // Green OFF
        } else {
            // cooler -> GREEN ON, RED OFF
            RA5 = 0;                  // Green ON (active-LOW)
            RC6 = 1;                  // Red OFF
        }

        // ---- Print ~2 lines/sec ----
        printf("IntT %u FixV %u Pot %u  F %0.2f\r\n", Temp, FVval, Pot, Ftemp);
        __delay_ms(500);
    }
}

// ======================= UART ============================
void setup_comms(void)
{
    RX_PIN = 1;
    TX_PIN = 1;
    SPBRG  = DIVIDER;
    RCSTA  = (NINE_BITS | 0x90);         // SPEN=1, CREN=1
    TXSTA  = (SPEED | NINE_BITS | 0x20); // BRGH=1, TXEN=1
    SYNC   = 0;
    BRGH   = 1;
}

void putch(char bVal)
{
    while(!TXIF);
    TXREG = bVal;
}

// ======================= PINS ============================
void pinConfig(void)
{
    // 4 MHz INTOSC
    OSCCON = 0x6A;

    // Map EUSART to RC4(TX)/RC5(RX)
    TXCKSEL = 1;   // APFCON: TX on RC4
    RXDTSEL = 1;   // APFCON: RX on RC5

    // LEDs (ACTIVE-LOW): RA5=Green, RA2=Blue, RC6=Red
    TRISA  = 0x10;     // RA4 input; others outputs
    ANSELA = 0x10;     // RA4 analog only; RA2/RA5 digital
    RA5 = 1;           // off
    RA2 = 1;           // off

    TRISC  = 0x30;     // RC4/RC5 for UART; others outputs
    ANSELC = 0x00;     // all digital on Port C
    TRISC6 = 0;        // RC6 as output
    RC6 = 1;           // Red OFF (active-LOW)

    // Pot on RB5 (AN11)
    TRISB  = 0xF0;     // RB4?RB7 inputs
    ANSELB = 0xF0;     // RB5 analog enabled (AN11)

    INTCON = 0;        // no interrupts
}

