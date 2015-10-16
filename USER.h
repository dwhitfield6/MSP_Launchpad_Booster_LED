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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef USER_H
#define	USER_H

#include <msp430.h>

#include "SYSTEM.h"

/******************************************************************************/
/* Firmware Version                                                           */
/******************************************************************************/
#define CPU_VERSION    "4"
#define CPU_REVISION   "0"
#define CPU_ALPHA      ""
#define CPU_BRANCH     "DW0a"

/******************************************************************************/
/* PCB board
 *
 * This code is used with boards:
 * 1. MSP430FR5969_LAUNCHPAD      (Debug/Release MSP430FR5969 Launchpad)      */
/******************************************************************************/
#define MSP430FR5969_LAUNCHPAD

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define ON 1
#define OFF 0
#define TRUE 1
#define FALSE 0
#define OUTPUT 1
#define INPUT 0
#define PASS 1
#define FAIL 0
#define YES 1
#define NO 0

/******************************************************************************/
/* Pin Defines                                                                */
/******************************************************************************/

/************* 32768 crystal *************/
/* Connected to the LFXIN */
#define Port_LF_Crystal1	P4DIR // PJ.4
#define Pin_LF_Crystal1	0x10  // PJ.4

/* Connected to the LFXOUT */
#define Port_LF_Crystal2	P4DIR // PJ.5
#define Pin_LF_Crystal2	0x20  // PJ.5

/************* Green LED *************/
/* Connected to the green LED indicator */
#define Port_GreenLED	P1DIR // P1.0
#define Pin_GreenLED	0x01  // P1.0

/************* Red LED *************/
/* Connected to the Red LED indicator */
#define Port_RedLED	P4DIR // P4.6
#define Pin_RedLED	0x40  // P4.6

/************* Button 1 (SW 1) *************/
/* Connected to pushbutton switch S1 */
#define Port_Button1	P4DIR // P4.5
#define Pin_Button1		0x20  // P4.5

/************* Button 2 (SW 2) *************/
/* Connected to pushbutton switch S2 */
#define Port_Button2	P1DIR // P1.1
#define Pin_Button2		0x02  // P1.1

/************* TLC5940 *************/
/* Connected to BLANK pin on TLC5940 */
#define Port_Blank		P4DIR // P4.3
#define Pin_Blank		0x08  // P4.3

/* Connected to SCLK pin on TLC5940 */
#define Port_SCLK		P2DIR // P2.2
#define Pin_SCLK		0x04  // P2.2

/* Connected to XLAT pin on TLC5940 */
#define Port_XLAT		P1DIR // P1.2
#define Pin_XLAT		0x04  // P1.2

/* Connected to XERR pin on TLC5940 */
#define Port_XERR		P1DIR // P1.5
#define Pin_XERR		0x20  // P1.5

/* Connected to SIN pin on TLC5940 */
#define Port_SIN_MOSI	P1DIR // P1.6
#define Pin_SIN_MOSI	0x40  // P1.6

/* Connected to SOUT pin on TLC5940 */
#define Port_SOUT_MISO	P1DIR // P1.7
#define Pin_SOUT_MISO	0x80  // P1.7

/* Connected to GSCLK pin on TLC5940 */
#define Port_PWM_CLK	P3DIR // P3.4
#define Pin_PWM_CLK		0x10  // P3.4


/************* UART *************/
/* Connected to UART transmit (TX) */
#define Port_TX		P2DIR // P2.5
#define Pin_TX		0x20  // P2.5

/* Connected to UART receive (RX) */
#define Port_RX		P2DIR // P2.6
#define Pin_RX		0x40  // P2.6

/************* Analog audio *************/
/* Connected to OP amp output */
#define Port_Audio		P1DIR 	// P1.3 used as A3
#define Pin_Audio		0x08  	// P1.3 used as A3
#define Channel_Audio	3  	  	// P1.3 used as A3

/* Connected to output of low pass filter */
#define Port_AudioLow		P2DIR 	// P2.4 used as A7
#define Pin_AudioLow		0x10  	// P2.4 used as A7
#define Channel_AudioLow	7  		// P2.4 used as A7

/* Connected to the left channel of the stereo jack */
#define Port_AudioRaw		P4DIR 	// P4.2 used as A10
#define Pin_AudioRaw		0x04  	// P4.2 used as A10
#define Channel_AudioRaw	10  	// P4.2 used as A10

/******************************************************************************/
/* Version variables                                                          */
/******************************************************************************/
extern const unsigned char Version[];
extern const unsigned char Revision[];
extern const unsigned char Alpha[];
extern const unsigned char Branch_Version[];

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void Init_App(void);
void Init_System (void);

#endif	/* USER_H */
