/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/12/15     4.0_DW0a    Initial project make.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef TLC5940_H
#define	TLC5940_H

#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* NUMBER_LEDS
 *
 * This is the number of LED channels in the system (TLC5940)
 *                                                                            */
/******************************************************************************/
#define NUMBER_LEDS 	16

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define FORWARD 	0
#define BACKWARD 	1

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
extern unsigned short LEDs[16];
extern unsigned char FadeDirection;

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void Init_TLC5940(void);
void TLC_GSCLK(unsigned char state);
void TLC_UpdateLEDs(void);
void TLC_SetLEDsLinear(short value, unsigned char direction);
void TLC_SetLEDs(unsigned short* temp);
void TLC_Blank(unsigned char state);
void TLC_Blank(unsigned char state);
void TLC_XLAT(unsigned char state);
unsigned char TLC_XERRInterrupt(unsigned char state);

#endif	/* TLC5940_H */
