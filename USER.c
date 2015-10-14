/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/04/15     4.0_DW0a    Initial project make.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Contains Functions for PIC initialization
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "BUTTON.h"
#include "LED.h"
#include "SPI.h"
#include "SYSTEM.h"
#include "TIMERS.h"
#include "TLC5940.h"
#include "UART.h"
#include "USER.h"


/******************************************************************************/
/* Version variables                                                          */
/******************************************************************************/
const unsigned char Version[]        = CPU_VERSION;
const unsigned char Revision[]       = CPU_REVISION;
const unsigned char Alpha[]          = CPU_ALPHA;
const unsigned char Branch_Version[] = CPU_BRANCH;

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/******************************************************************************/
/* Inline Functions															  */
/******************************************************************************/

/******************************************************************************/
/* Functions																  */
/******************************************************************************/

/******************************************************************************/
/* Init_App
 *
 * The function initializes the application. It sets the pin directions and
 *  initial values.
 *                                                                            */
/******************************************************************************/
void Init_App(void)
{
	TLC_Blank(ON);
	TLC_XLAT(OFF);

	/* initialize all ports as output */
	P1DIR = 0xFF;
	P2DIR = 0xFF;
	P3DIR = 0xFF;
	P4DIR = 0xFF;
	PJDIR = 0xFF;

	/*
	 * Disable the GPIO power-on default high-impedance mode
	 * to activate previously configured port settings
	 */
    PM5CTL0 &= ~LOCKLPM5;

    /*~~~~~~~~~~~~~ Low power crystal ~~~~~~~~~~~~~~~~~*/
    /* set function to LFXIN crystal mode */
    PJSEL0 |= Pin_LF_Crystal1;
    PJSEL1 &= ~Pin_LF_Crystal1;

    /*~~~~~~~~~~~~~ LEDs ~~~~~~~~~~~~~~~~~*/
    /* Green LED */
    Port_GreenLED |= Pin_GreenLED; 	// set to output

    /* Red LED */
    Port_RedLED |= Pin_RedLED; 		// set to output

    /*~~~~~~~~~~~~~ Pushbuttons ~~~~~~~~~~~~~~~~~*/
    /* Button 1 */
    Port_Button1 &= ~Pin_Button1;	// set to input

    /* Button 2 */
    Port_Button2 &= ~Pin_Button2; 	// set to input

    /*~~~~~~~~~~~~~ TLC5940 LED driver ~~~~~~~~~~~~~~~~~*/
    /* Blank */
    Port_Blank |= Pin_Blank;  // set to output

    /* SCLK */
    Port_SCLK |= Pin_SCLK;	// set to output

    /* XLAT */
    Port_XLAT |= Pin_XLAT;	// set to output

    /* XERR */
    Port_XERR &= ~Pin_XERR;	// set to input

    /* SIN (MOSI) */
    Port_SIN_MOSI |= Pin_SIN_MOSI;	// set to output

    /* SOUT (MISO) */
    Port_SOUT_MISO &= ~Pin_SOUT_MISO;  // set to input

    /* GSCLK (PWM_CLK) */
    Port_PWM_CLK |= Pin_PWM_CLK;  // set to output

    /*~~~~~~~~~~~~~ UART ~~~~~~~~~~~~~~~~~*/
    /* TX */
    Port_TX |= Pin_TX;	// set to output

    /* RX */
    Port_RX &= ~Pin_RX;	// set to input
}

/******************************************************************************/
/* Init_System
 *
 * The function initializes the modules.
 *                                                                            */
/******************************************************************************/
void Init_System(void)
{

	Init_Buttons();
	Init_Timers();
	Init_SPI();
	Init_TLC5940();
	Init_UART();

	__enable_interrupt();   // enable global interrupts
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
