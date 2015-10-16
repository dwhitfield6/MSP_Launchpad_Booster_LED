/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/14/15     4.0_DW0a    Initial project make (branched from
 * 							  "MSP_Launchpad_MSP430FR5969_Test".
 * 							Added base periperal functionality for ADC,
 * 							  Timers, UART, and drivers for TLC5940.
 * 							Added audio sampling to make a VU meter.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Contains main function.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "ADC.h"
#include "LED.h"
#include "MISC.h"
#include "SYSTEM.h"
#include "TLC5940.h"
#include "UART.h"
#include "USER.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Global Variable                                                            */
/******************************************************************************/

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int main (void)
{
	short temp = 0;
	unsigned char direction;

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    /* Initialize */
    SYS_ConfigureOscillator();
    Init_App();
    Init_System();
    UART_PrintBanner();

    /* Flash red and green leds */
    LED_DisplayShow();

    /* Initialize variables */
    direction = FadeDirection;
    ADC_SetMidpointOffset();

    while(1)
    {
    	temp = (ADC_SampleWait(AUDIO) - (ADC_MIDPOINT_OFFSET + 50));
    	if(temp >= 0)
    	{
    		TLC_SetLEDsLinear(temp, 300, direction);
    	}
    }
	return 0;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
