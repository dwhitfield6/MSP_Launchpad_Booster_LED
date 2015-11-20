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
 * 							Increased clock frequency to 16MHz.
 * 							Created beat detecting algorithm.
 * 							Fixed several bugs in system frequency calculation.
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
	short tempsample = 0;
	short tempaverageBig = 0;
	short tempaverageSmall = 0;
	short temp;
	unsigned short volume;

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
    	MSC_Test0Toggle(); // toggle so we know the sampling rate
    	volume = GlobalVolume; // update small processing window
    	tempsample = ADC_SampleWait(AUDIORAW) - ADC_Midpoint_Offset[AUDIORAW] - ADC_LEEWAY; // sample ADC

    	/* add to sample to  averagers */
    	PRO_AddToProcessBuffer(tempsample, 0);
    	PRO_AddToProcessBuffer(tempsample, 1);
    	tempaverageBig = (short) PRO_ProcessData(POS_AVERAGE, 20, 0);
    	tempaverageSmall = (short) PRO_ProcessData(POS_AVERAGE, 1, 1);

    	temp = (short) MSC_ABSL((long)tempaverageSmall - (long)tempaverageBig);

    	TLC_SetLEDsLinear(temp, volume, FadeDirection);
    }
	return 0;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
