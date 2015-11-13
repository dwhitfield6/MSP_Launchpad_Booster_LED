/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/21/15     4.0_DW0b    Added low pass filter IC functionality.
 * 							Fixed some comment errors.
 * 							Changed version to continue developement.
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
unsigned short test[16] = {0,2048,0,0,0,0,0,0,0,0,0,0,5000,0,0,0};

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
    ADC_SetMidpointOffset(AUDIOLOW);

    while(1)
    {
    	window = ProcessingWindow;
    	temp = ADC_SampleWait(AUDIOLOW) - ADC_Midpoint_Offset[AUDIOLOW] - ADC_LEEWAY;
    	PRO_AddToProcessBuffer(temp);
    	temp = (short) PRO_ProcessData(WEIGHTED_POS_AVERAGE_d5X, window);
    	TLC_SetLEDsLinear(temp, 250, FadeDirection);
    	//TLC_SetLEDs(test);
    }
	return 0;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
