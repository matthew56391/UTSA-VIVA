/*
 * File:   Lab8_I2C_LCD.c
 * Author: Matthew Cuebas
 *
 * Micro 1 Lab 8 (F25) ? VIVA UTSA Board (PIC16F1829 @ 4 MHz INTOSC)
 * I2C LCD (PCF8574 backpack)
 *
 * Behavior:
 * 1) Display "Hello World"
 * 2) Display ASCII A?Z (wrap to line 2 after 'P')
 * 3) Display a simple math expression
 * 4) Demonstrate LCD shifting left/right
 */

#include <xc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "i2c.h"
#include "i2c_LCD.h"

// ================= CONFIG BITS =================
// CONFIG1
#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
// CONFIG2
#pragma config WRT = OFF
#pragma config PLLEN = OFF
#pragma config STVREN = ON
#pragma config BORV = LO
#pragma config LVP = ON

// ================= DEFINES =================
#define I2C_SLAVE 0x27          // Change to 0x3F if your backpack uses that address
#define _XTAL_FREQ 4000000UL    // Must be integer for __delay_ms

// ================= PROTOTYPES (from lab headers) =================
void I2C_LCD_Command(unsigned char, unsigned char);
void I2C_LCD_SWrite(unsigned char, unsigned char *, char);
void I2C_LCD_Init(unsigned char);
void I2C_LCD_Pos(unsigned char, unsigned char);
unsigned char I2C_LCD_Busy(unsigned char);

void main(void)
{
    // Buffer (ensure long enough for our longest line + '\0')
    char Sout[21];
    unsigned char *Sptr = (unsigned char *)Sout;
    (void)Sptr; // silence unused warning if not used directly

    // ================= CLOCK =================
    // Keep lab's original 4 MHz setting
    // (This matches your starter: OSCCON = 0x68; comment: 4 MHz)
    OSCCON = 0x68;

    // ================= I2C + LCD INIT =================
    i2c_Init();                  // I2C Master @ 100kHz per lab driver
    I2C_LCD_Init(I2C_SLAVE);     // Initialize LCD via PCF8574 backpack

    // ===== Stage 1: Hello World (bottom line) =====
    I2C_LCD_Command(I2C_SLAVE, 0x01);     // Clear display
    __delay_ms(2);
    I2C_LCD_Pos(I2C_SLAVE, 0x40);         // Line 2, column 0
    sprintf(Sout, "Hello World");         // 11 chars
    I2C_LCD_SWrite(I2C_SLAVE, (unsigned char*)Sout, (char)strlen(Sout));
    __delay_ms(1000);

    // ===== Stage 2: ASCII crawl A..Z (wrap after P) =====
    I2C_LCD_Command(I2C_SLAVE, 0x01);
    __delay_ms(2);
    I2C_LCD_Pos(I2C_SLAVE, 0x00);         // Line 1 start (DDRAM 0x00)

    for (int i = 0; i < 26; i++) {
        char ch = 'A' + i;
        Sout[0] = ch;
        // write exactly 1 char each step
        I2C_LCD_SWrite(I2C_SLAVE, (unsigned char*)Sout, 1);
        __delay_ms(200);

        // after 'P' (i==15), jump to line 2 so Q?Z are visible
        if (i == 15) {
            I2C_LCD_Pos(I2C_SLAVE, 0x40); // Line 2 start (DDRAM 0x40)
        }
    }
    __delay_ms(800);

    // ===== Stage 3: Challenge 1 ("Hello UTSA") =====
    I2C_LCD_Command(I2C_SLAVE, 0x01);
    __delay_ms(2);
    sprintf(Sout, "Hello UTSA");       // 13 chars
    I2C_LCD_SWrite(I2C_SLAVE, (unsigned char*)Sout, (char)strlen(Sout));
    __delay_ms(1500);

    // ===== Stage 4: Challenge 2 (100 + 50 = 150) =====
    {
        int a = 100, b = 50, sum = a + b; // 150
        I2C_LCD_Command(I2C_SLAVE, 0x01);
        __delay_ms(2);
        sprintf(Sout, "%d + %d = %d", a, b, sum); // "100 + 50 = 150"
        I2C_LCD_SWrite(I2C_SLAVE, (unsigned char*)Sout, (char)strlen(Sout));
        __delay_ms(2000);
    }

    // ===== Stage 5: Challenge 3 (scroll demo) =====
    I2C_LCD_Command(I2C_SLAVE, 0x01);
    __delay_ms(2);

    // Top line
    I2C_LCD_Pos(I2C_SLAVE, 0x00);
    sprintf(Sout, "Lab 8: I2C LCD");
    I2C_LCD_SWrite(I2C_SLAVE, (unsigned char*)Sout, (char)strlen(Sout));

    // Bottom line
    I2C_LCD_Pos(I2C_SLAVE, 0x40);
    sprintf(Sout, "Shift Demo");
    I2C_LCD_SWrite(I2C_SLAVE, (unsigned char*)Sout, (char)strlen(Sout));

    // Smooth back-and-forth scroll; a tiny longer delay improves readability
    while (1) {
        for (int i = 0; i < 15; i++) {
            I2C_LCD_Command(I2C_SLAVE, 0x1C);  // shift right
            __delay_ms(250);
        }
        for (int i = 0; i < 15; i++) {
            I2C_LCD_Command(I2C_SLAVE, 0x18);  // shift left
            __delay_ms(250);
        }
    }
}
