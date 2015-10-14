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
/* Contains functions for LEDs.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "LED.h"
#include "USER.h"
#include "SYSTEM.h"
#include "MISC.h"

/******************************************************************************/
/* Inline Functions                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* LED_GreenToggle
 *
 * The function controls the green LED on the launchpad.
 *
 * Input: N/A
 * Output: N/A
 * Action: toggles the green LED
 *                                                                            */
/******************************************************************************/
void LED_GreenToggle(void)
{
	P1OUT ^= Pin_GreenLED;
}

/******************************************************************************/
/* LED_RedToggle
 *
 * The function controls the red LED on the launchpad.
 *
 * Input: N/A
 * Output: N/A
 * Action: toggles the red LED
 *                                                                            */
/******************************************************************************/
void LED_RedToggle(void)
{
	P4OUT ^= Pin_RedLED;
}

/******************************************************************************/
/* LED_Green
 *
 * The function controls the green LED on the launchpad.
 *
 * Input: state (ON or OFF)
 * Output: the previous state of the green LED
 * Action: controls the green LED
 *                                                                            */
/******************************************************************************/
unsigned char LED_Green(unsigned char state)
{
	unsigned char status = FALSE;

	if(P1IN & Pin_GreenLED)
	{
		status = TRUE;
	}

    if(state)
    {
    	P1OUT |= Pin_GreenLED;	// turn on the LED
    }
    else
    {
		P1OUT &= ~Pin_GreenLED;	// turn off the LED
    }
    return status;
}

/******************************************************************************/
/* LED_Red
 *
 * The function controls the red LED on the launchpad.
 *
 * Input: state (ON or OFF)
 * Output: the previous state of the red LED
 * Action: controls the red LED
 *                                                                            */
/******************************************************************************/
unsigned char LED_Red(unsigned char state)
{
	unsigned char status = FALSE;

	if(P4IN & Pin_RedLED)
	{
		status = TRUE;
	}

    if(state)
    {
    	P4OUT |= Pin_RedLED;	// turn on the LED
    }
    else
    {
		P4OUT &= ~Pin_RedLED;	// turn off the LED
    }
    return status;
}

/******************************************************************************/
/* LED_DisplayShow
 *
 * The function blinks the LEDS as a startup show.
 *
 * Input: N/A
 * Output: N/A
 * Action: Flashes the LEDs to indicate start-up.
 *                                                                            */
/******************************************************************************/
void LED_DisplayShow(void)
{
	unsigned char i;

	LED_Green(ON);
	LED_Red(OFF);

	for(i=0;i<10;i++)
	{
		LED_GreenToggle();
		LED_RedToggle();
		MSC_DelayUS(20000);
	}
	LED_Green(OFF);
	LED_Red(OFF);
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
