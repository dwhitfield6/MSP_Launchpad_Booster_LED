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
/* Contains functions for the analog to digital converter.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>

#include "ADC.h"
#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* ADC_MIDPOINT_OFFSET
 *
 * This is the adc counts when there is no audiop playing or the 3.5mm
 *  connector is unplugged.
 *                                                                            */
/******************************************************************************/
short ADC_MIDPOINT_OFFSET;

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
short ADC_Counts[ADC_NUMBER_CHANNELS];
volatile unsigned char ADC_Ready = FALSE;
volatile unsigned char CurrentChannel = AUDIO;
unsigned char VREF = VOLTS_3v3;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Init_ADC
 *
 * The function initializes the ADC module .
 *                                                                            */
/******************************************************************************/
void Init_ADC(void)
{
	/* pin as analog channel A3 */
	P1SEL1 |= Pin_Audio;
	P1SEL0 |= Pin_Audio;

	/* pin as analog channel A7 */
	P2SEL1 |= Pin_AudioLow;
	P2SEL0 |= Pin_AudioLow;

	/* pin as analog channel A10 */
	P4SEL1 |= Pin_AudioRaw;
	P4SEL0 |= Pin_AudioRaw;
	ADC_EnableConversion(OFF);
	ADC_Module(OFF);

	/* ADC12_B sample-and-hold time */
	ADC12CTL0 &= ~(ADC12SHT13 | ADC12SHT12 | ADC12SHT11 | ADC12SHT10);
	ADC12CTL0 |= (ADC12SHT13 | ADC12SHT11); // 1010b = 512 ADC12CLK cycles
	ADC12CTL0 &= ~(ADC12SHT03 | ADC12SHT02 | ADC12SHT01 | ADC12SHT00);
	ADC12CTL0 |= (ADC12SHT03 | ADC12SHT01); // 1010b = 512 ADC12CLK cycles

	/* ADC12_B predivider */
	ADC12CTL1 |= ADC12PDIV1;  // 10b = Predivide by 32
	ADC12CTL1 &= ~ADC12PDIV1; // 10b = Predivide by 32

	/* ADC12_B sample-and-hold source select */
	ADC12CTL1 &= ~(ADC12SHS2 | ADC12SHS1 | ADC12SHS0); // 000b = ADC12SC bit

	/* ADC12_B clock divider */
	ADC12CTL1 &= ~(ADC12DIV2 | ADC12DIV1 | ADC12DIV0); // 000b = /1

	/* ADC12_B sample-and-hold pulse-mode select */
	ADC12CTL1 |= ADC12SHP;  // 1b = SAMPCON signal is sourced from the sampling timer

	/* ADC12_B clock source select */
	ADC12CTL1 |= (ADC12SSEL1 | ADC12SSEL0);  //11b = SMCLK

	/* ADC12_B conversion sequence mode select */
	ADC12CTL1 &= ~(ADC12CONSEQ1 | ADC12CONSEQ0); // 00b = Single-channel, single-conversion

	/* ADC12_B resolution */
	ADC12CTL2 |= ADC12RES1;  // 10b = 12 bit (14 clock cycle conversion time)
	ADC12CTL2 &= ~ADC12RES0; // 10b = 12 bit (14 clock cycle conversion time)

	VREF = VOLTS_3v3; // set reference to the VDD voltage rail

	switch(VREF)
	{
	case VOLTS_1v2:
		/* Selects combinations of VR+ and VR- sources as well as the buffer selection */
		ADC12MCTL0 &= ~(ADC12VRSEL3 | ADC12VRSEL2 | ADC12VRSEL1 | ADC12VRSEL0); // 0011b = VR+ = VeREF+ buffered, VR- = AVSS
		ADC12MCTL0 |= (ADC12VRSEL1 | ADC12VRSEL0);  // 0011b = VR+ = VeREF+ buffered, VR- = AVSS

		/* Reference voltage level */
		CECTL2 &= ~CEREFL1; // 01b = 1.2 V is selected as shared reference voltage input
		CECTL2 |= CEREFL1;  // 01b = 1.2 V is selected as shared reference voltage input
		break;
	case VOLTS_3v3:
		/* Selects combinations of VR+ and VR- sources as well as the buffer selection */
		ADC12MCTL0 &= ~(ADC12VRSEL3 | ADC12VRSEL2 | ADC12VRSEL1 | ADC12VRSEL0); // 0011b = VR+ = VeREF+ buffered, VR- = AVSS
		ADC12MCTL0 |= (ADC12VRSEL1 | ADC12VRSEL0);  // 0011b = VR+ = VeREF+ buffered, VR- = AVSS

		/* Reference voltage level */
		CECTL2 &= ~CEREFL1; // 00b = Reference amplifier is disabled. No reference voltage is requested.
		CECTL2 &= ~CEREFL1;  // 00b = Reference amplifier is disabled. No reference voltage is requested.
		break;
	}

	ADC_Interrupt(ON);
	ADC_Module(ON);
	ADC_EnableConversion(ON);
}

/******************************************************************************/
/* ADC_SetMidpointOffset
 *
 * The function reads the midpoint offset which is the ADC counts when the
 *  audio connector is unplugged or the audio is muted.
 *
 * Input: N/A
 * Output: the ADC midpoint when audio is muted
 * Action: reads the midpoint offset
 *                                                                            */
/******************************************************************************/
short ADC_SetMidpointOffset(void)
{
	ADC_MIDPOINT_OFFSET = ADC_SampleWait(AUDIO);
}

/******************************************************************************/
/* ADC_Interrupt
 *
 * The function controls the ADC ADC12MEM0 interrupt.
 *
 * Input: on or off
 * Output: previous status of module interrupt
 * Action: controls the ADC ADC12MEM0 interrupt
 *                                                                            */
/******************************************************************************/
unsigned char ADC_Interrupt(unsigned char state)
{
	unsigned char status = FALSE;

	if(ADC12IER0 & ADC12IE0)
	{
		status = TRUE;
	}

	if(state)
	{
		ADC12IER0 |= ADC12IE0; // ADC12IFG0 bit Interrupt enabled
	}
	else
	{
		ADC12IER0 &= ~ADC12IE0; // ADC12IFG0 bit Interrupt disabled
	}
	return status;
}

/******************************************************************************/
/* ADC_Module
 *
 * The function controls the ADC module.
 *
 * Input: on or off
 * Output: previous status of module
 * Action: controls the ADC module
 *                                                                            */
/******************************************************************************/
unsigned char ADC_Module(unsigned char state)
{
	unsigned char status = FALSE;
	unsigned char StatusConversion;

	StatusConversion = ADC_EnableConversion(OFF); // save conversion status
	if(ADC12CTL0 & ADC12ON)
	{
		status = TRUE;
	}

	if(state)
	{
		ADC12CTL0 |= ADC12ON; // ADC12_B on
	}
	else
	{
		ADC12CTL0 &= ~ADC12ON; // ADC12_B off
	}

	if(StatusConversion)
	{
		ADC_EnableConversion(ON);
	}
	return status;
}

/******************************************************************************/
/* ADC_EnableConversion
 *
 * The function controls the ADC module conversion status.
 *
 * Input: on or off
 * Output: previous status of module
 * Action: controls the ADC module
 *                                                                            */
/******************************************************************************/
unsigned char ADC_EnableConversion(unsigned char state)
{
	unsigned char status = FALSE;

	if(ADC12CTL0 & ADC12ENC)
	{
		status = TRUE;
	}

	if(state)
	{
		ADC12CTL0 |= ADC12ENC; // ADC12_B enabled
	}
	else
	{
		ADC12CTL0 &= ~ADC12ENC; // ADC12_B disabled
	}
	return status;
}

/******************************************************************************/
/* ADC_Sample
 *
 * The function starts a sample and conversion.
 *
 * Input: channel (AUDIO, RAW, or LOWPASS)
 * Output: N/A
 * Action: starts sample and conversion
 *                                                                            */
/******************************************************************************/
void ADC_Sample(unsigned char channel)
{
	unsigned char status = ADC_EnableConversion(OFF);
	ADC_Ready = FALSE;
	ADC12MCTL0 &= ~(0x001F);
	switch (channel)
	{
	case AUDIO:
		ADC12MCTL0 |= Channel_Audio;
		break;
	case RAW:
			ADC12MCTL0 |= Channel_AudioRaw;
			break;
	default:
			ADC12MCTL0 |= Channel_AudioLow;
			break;
	}
	if(status)
	{
		ADC_EnableConversion(ON);
	}
	ADC12CTL0 |= ADC12SC;
}

/******************************************************************************/
/* ADC_SampleWait
 *
 * The function starts a sample and conversion then waits for the result.
 *
 * Input: channel (AUDIO, RAW, or LOWPASS)
 * Output: ADC channel raw counts.
 * Action: starts sample and conversion then waits for the result
 *                                                                            */
/******************************************************************************/
short ADC_SampleWait(unsigned char channel)
{
	ADC_Sample(channel);
	while(!ADC_Ready);
	return ADC_Counts[channel];
}
/******************************************************************************/
/* ADC_Busy
 *
 * The function indicates if a sequence, sample, or conversion is active.
 *
 * Input: N/A
 * Output: status of busy
 * Action: indicates if a sequence, sample, or conversion is active
 *                                                                            */
/******************************************************************************/
unsigned char ADC_Busy(void)
{
	if(ADC12CTL1 & ADC12BUSY)
	{
		return TRUE;
	}
	return FALSE;
}

/******************************************************************************/
/* ADC_Read
 *
 * The function reads the ADC raw counts and aves it to the global ADC_Counts[].
 *
 * Input: channel (AUDIO, RAW, or LOWPASS)
 * Output: ADC counts
 * Action: reads the adc and sets the value to the global
 *                                                                            */
/******************************************************************************/
short ADC_Read(unsigned char channel)
{
	short data = ADC12MEM0;
	ADC_Counts[channel] = data;
	return data;
}

/******************************************************************************/
/* ADC_ChangeChannel
 *
 * The function increments the channel to get ready to take the next sample.
 *
 * Input: N/A
 * Output: the current channel
 * Action: increments the channel and output the current channel
 *                                                                            */
/******************************************************************************/
unsigned char ADC_ChangeChannel(void)
{
	unsigned char channel = CurrentChannel;
	CurrentChannel++;
	if(CurrentChannel > LOWPASS)
	{
		CurrentChannel = AUDIO;
	}
	ADC_Counts[channel] = ADC12MEM0;
	return channel;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
