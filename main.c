/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/21/15     4.0_DW0b    Changed version to continue developement.
 * 10/14/15     4.0_DW0a    Initial project make (branched from
 * 							  "MSP_Launchpad_MSP430FR5969_Test".
 * 							Added base periperal functionality for ADC,
 * 							  Timers, UART, and drivers for TLC5940.
 * 							Added audio sampling to make a VU meter.
 * 							Added variable space to FRAM to increase
 * 							  processing buffer size.
 * 							Added processing algorithms.
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
#include "PROCESSING.h"
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
	unsigned short window;

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    /* Initialize */
    SYS_ConfigureOscillator();
    Init_App();
    Init_System();
    UART_PrintBanner();

    /* Flash red and green leds */
    LED_DisplayShow();

    /* Initialize variables */
    ADC_SetMidpointOffset(AUDIORAW);

    while(1)
    {
    	window = ProcessingWindow;
    	temp = ADC_SampleWait(AUDIORAW) - ADC_Midpoint_Offset[AUDIORAW] - ADC_LEEWAY;
    	PRO_AddToProcessBuffer(temp);
    	temp = (short) PRO_ProcessData(WEIGHTED_POS_AVERAGE_d5X, window);
    	TLC_SetLEDsLinear(temp, 250, FadeDirection);
    }
	return 0;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
