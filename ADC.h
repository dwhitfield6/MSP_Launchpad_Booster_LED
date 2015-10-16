/******************************************************************************/
/* Change log                                                                 *
 *
 *
 *
 * Date         Revision    Comments
 * MM/DD/YY
 * --------     ---------   ----------------------------------------------------
 * 10/14/15     4.0_DW0a    Initial project make.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#ifndef ADC_H
#define	ADC_H

#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* ADC channels
 *
 * This is the place in the buffer that the different raw count channels are
 *  stored.
 *                                                                            */
/******************************************************************************/
#define AUDIO 		0
#define RAW			1
#define AUDIORAW	RAW
#define LOWPASS		2
#define AUDIOLOW	LOWPASS

/******************************************************************************/
/* reference voltage
 *
 * This is the reference voltage for the ADC.
 *                                                                            */
/******************************************************************************/
#define VOLTS_1v2 		12
#define VOLTS_3v3 		33

/******************************************************************************/
/* ADC_NUMBER_CHANNELS
 *
 * This is the number of channels that we use on the ADC.
 *                                                                            */
/******************************************************************************/
#define ADC_NUMBER_CHANNELS (LOWPASS + 1)

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
extern short ADC_Counts[ADC_NUMBER_CHANNELS];
extern volatile unsigned char ADC_Ready;
extern volatile unsigned char CurrentChannel;
extern unsigned char VREF;
extern short ADC_MIDPOINT_OFFSET;

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void Init_ADC(void);
short ADC_SetMidpointOffset(void);
unsigned char ADC_Interrupt(unsigned char state);
unsigned char ADC_Module(unsigned char state);
unsigned char ADC_EnableConversion(unsigned char state);
void ADC_Sample(unsigned char channel);
short ADC_SampleWait(unsigned char channel);
unsigned char ADC_Busy(void);
short ADC_Read(unsigned char channel);
unsigned char ADC_ChangeChannel(void);

#endif	/* ADC_H */
