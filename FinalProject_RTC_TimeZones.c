/*
 * File:   FinalProject_RTC_TimeZones.c
 * Author: Matthew Cuebas
 *
 * VIVA Final Project (PIC16F1829 @ 4MHz)
 * DS3231/DS1307 RTC over I2C + I2C LCD
 * RB7 button cycles US time zones (P/M/C/E)
 */


// PIC16F1829 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include  "i2c.h"
#include  "i2c_LCD.h"

#define I2C_SLAVE_HI 0x27
#define I2C_SLAVE_LO 0x20
#define Clock_Address 0x68
#define I2C_WRITE 0
#define I2C_READ 1
#define BAUD 9600      
#define FOSC 4000000L
#define DIVIDER ((int)(FOSC/(16UL * BAUD) -1))
#define NINE_BITS 0
#define SPEED 0x4
	#define RX_PIN TRISC5
	#define TX_PIN TRISC4
#define _XTAL_FREQ 4000000.0    /*for 4mhz*/
#define pi 3.141592/180.0		/*sets pi*/
/* Serial initialization */
void init_comms(void);
void I2C_LCD_Command(unsigned char,unsigned char);
void I2C_LCD_SWrite(unsigned char,unsigned char *, char);
void I2C_LCD_Init(unsigned char);
void I2C_LCD_Pos(unsigned char,unsigned char);
unsigned char I2C_LCD_Busy(unsigned char);
void init_a2d(void);
void putch(char);
int getch(void);
unsigned char getche(void);
char * ftoa (float, int *);
/*  ********************************************************************* */
void main(void) {
    TRISB = 0x80;
    ANSELB = 0x80;
    
unsigned char Treg, Tdata, Sout[16],Sout2[16];
unsigned char * Sptr;
	Sptr = Sout;  // This is a buffer for the Sprintf command to use
/* Sample code to set up the A2D module */
	init_a2d();

    int zone = 2;
    
unsigned char cvar, channel, LCD_pos, slaveAddress, Temp1;
unsigned char sec, min, hour, day, month, year;
int count, ioF,i,j, dF, Temp, mag;
unsigned long int i2, i3, j2, j3, jDelay;
float	xvar, yvar, ans;
	FVRCON |= 0x30;
	i= 0;
	ans= 25 ;
	xvar = 3;
	count=0xFFFF;
	cvar = '0' ;
	count = (int)xvar;
   /* Setup 16F1825  4MHz clock and Serial 232 out */

	OSCCON  = 0x68; /* b6..4 = 1101 = 4MHz */
	APFCON0	= 0x00;  /*makes RC4 & 5 TX & RX for USART (Allows ICSP) also selects T1G=RA4*/
	ANSELA   = 0x00;    /* all ADC pins to digital I/O */
	ANSELC	= 0x80;
    TRISC = 0x03;      /* set as output all but I2C pins 0 & 1  */
    TRISA = 0x10;	//make RA4 input for ping echo; note: RA2 is pulse
	init_comms();
    LATA = 0x00;   /* zero out */
    LATC = 0x00;   /* zero out */
    //RA2=0;   /* zero RA2  */
//  ****************************set up Timer  1 for Echo pulse**************************** 
	T1CON = 0x01;  // SET FOR Fosc/4 AND 1:1 PreScaler And turn on T1
	T1GCON = 0xC0;	// Set for gate enable single pulse mode, then set bit 3 when go | 0x08
	TMR1H = 0 ;
	TMR1L = 0 ;
// Print out start up message on UART
	printf("\033U\033X");    /*Clears display and positons on to line Leftmost */
	printf("Start of Code");
	__delay_ms(1000.0);

/*  ********************************I2C**************************** */
/*  Note the I2C write to LCD uses the 8 bits of the PCF8574 chip to control the LCD  */
/*  Connected High 4 bits are High 4 data for LCD (use in 4 bit mode)  the other 4 are     */
/*  bit3=turn on /off bk light  bit 2= E line writes on Hi 2 Lo transition, reads Lo to Hi */
/*  bit 2=Read write set  to 0 for write   bit 0=RS  command=0 Data=1      */
		for (jDelay =0; jDelay< 1000; jDelay++);	  //needs delay on power up

		i2c_Init();				// Start I2C as Master 100KH
		LCD_pos = 0;
		slaveAddress = I2C_SLAVE_HI;
		I2C_LCD_Init(slaveAddress);
		i2c_Start();
	    I2C_LCD_Command(slaveAddress, 0x01);		//Clear display
		I2C_LCD_Pos(slaveAddress,0x40);
		sprintf(Sout,"Hello World  %02x ",LCD_pos);
		I2C_LCD_SWrite(slaveAddress,Sout, 16 );
		for (jDelay =0; jDelay< 1000; jDelay++);
	    I2C_LCD_Command(slaveAddress, 0x01);		//Clear display
		for (jDelay =0; jDelay< 1000; jDelay++);
        
   int doomsdayCounter = 0;     
   while(1) {      /* infinite loop */
				/*channel = 0x03;     75 for Temp,  19 for AN6 on RC2 pin 8 */
       RCSTA = (NINE_BITS|0x00);	
       if(!RB7) {
           zone = (zone+1) % 4;
           __delay_ms(100);
           doomsdayCounter++;
        } else {
           //doomsdayCounter = 0;
        }
       __delay_ms(5);
       __delay_ms(5);
       	RCSTA = (NINE_BITS|0x90);

       
	ADFM=1;		//Right justify
	ADCON0  = 0x19;	// apply the new channel (0D an3 pin3 / 75 temp/ 1D an7 pin 7)
	for (jDelay =0; jDelay< 10; jDelay++);
	ADGO  = 1;	// initiate conversion on the selected channel
	while(ADGO)continue;
	Temp = ((ADRESH<<8)+(ADRESL));   /*((ADRESH<<8)+(ADRESL))  */   

// **************** Flash led
      //LATA |= 0x20; 
	for (jDelay =0; jDelay< 1000; jDelay++);
      //LATA &= 0xDF; 
	for (jDelay =0; jDelay< 1000; jDelay++);
	
		slaveAddress = 0x68;
		i2c_Start();      					// send Start
		i2c_Address(slaveAddress, I2C_WRITE);	// Send  slave address with write operation 
		i2c_Write(0x00);					//write to the register pointer 00
		i2c_Restart();						// Change modes to read
		i2c_Address(slaveAddress, I2C_READ);	// Send slave address with read operation
		sec = i2c_Read(1);				// get the seconds BCD digits must mask off hi bit
		min = i2c_Read(1);
		hour = i2c_Read(1);
		day = i2c_Read(1);
		day = i2c_Read(1);
		month = i2c_Read(1);
		year = i2c_Read(0);
		i2c_Stop();
        char timeCode;
        switch (zone){
            case 0:
                timeCode = 'P';
                break;
            case 1:
                timeCode = 'M';
                break;
            case 2:
                timeCode = 'C';
                break;
            default:
                timeCode = 'E';
                break;
        }
        
        
		printf("\033U\033X");    /*Clears display and positons on to line Leftmost */
		printf("%02x:%02x:%02x %cST",(hour+zone)%24,min,sec, timeCode);
		printf("\033L");    /* positons on to bottom line Leftmost */
        
        if(hour+zone > 23) {
            day ++;
        }
        
		printf("%02x-%02x-20%02x ",month,day,year);
        
        

		sprintf(Sout ,"%02x:%02x:%02x %cST   ",(hour+zone)%24,min,sec, timeCode);
		slaveAddress = I2C_SLAVE_HI;
		I2C_LCD_Pos(slaveAddress,0x00);
		I2C_LCD_SWrite(slaveAddress,Sout, 15 ); 
		I2C_LCD_Pos(slaveAddress,0x40);
		sprintf(Sout ,"%02x-%02x-20%2x      ",month,day,year);
		I2C_LCD_SWrite(slaveAddress,Sout, 15 );
		for (jDelay =0; jDelay< 1000; jDelay++);
        }

void init_comms(){
	RX_PIN = 1;	
	TX_PIN = 1;		  
	SPBRG = DIVIDER;     	
	RCSTA = (NINE_BITS|0x90);	
	TXSTA = (SPEED|NINE_BITS|0x20);
}

void putch(char byte) 
{
	/* output one byte */
	while(!TXIF)	/* set when register is empty */
		continue;
	TXREG = byte;
}

int getch() {
	/* retrieve one byte */
	while(!RCIF)	/* set when register is not empty */
		continue;
	return RCREG;	
}

unsigned char getche(void)
{
	unsigned char c;
	putch(c = getch());
	return c;
}

/* Sample code to set up the A2D module */
void init_a2d(void){
	ADCON0=0x75;	//  Temp chanel CHS=11101 and GO/Done =0, ADON=1 (
	ADCON1=0x40;	// select Fosc/4 select Right Justify, Vrefs= Vss and Vdd
	ADON=1;		// turn on the A2D conversion module
}
