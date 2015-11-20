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
/* Contains functions for data processing.
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>
#include <math.h>

#include "MISC.h"
#include "PROCESSING.h"
#include "SYSTEM.h"
#include "USER.h"

/******************************************************************************/
/* ADC_MIDPOINT_OFFSET
 *
 * This is the adc counts when there is no audiop playing or the 3.5mm
 *  connector is unplugged.
 *                                                                            */
/******************************************************************************/
short ADC_MIDPOINT_OFFSET;
short GlobalVolume = 150;

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
#pragma SET_DATA_SECTION(".fram_vars")
short DataBuffer[2][DATA_BUFFER_SIZE];
#pragma SET_DATA_SECTION()

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* Init_Processing
 *
 * The function initializes the processing buffer.
 *                                                                            */
/******************************************************************************/
void Init_Processing(void)
{
	PRO_ClearProcessBuffer();
}

/******************************************************************************/
/* PRO_ClearProcessBuffer
 *
 * The function clears the processing buffer.
 *
 * Input: N/A
 * Output: N/A
 * Action: sets every byte in the processing buffer to 0
 *                                                                            */
/******************************************************************************/
void PRO_ClearProcessBuffer(void)
{
	MSC_CleanBuffer(DataBuffer,(DATA_BUFFER_SIZE * 2));
}

/******************************************************************************/
/* PRO_AddToProcessBuffer
 *
 * The function adds a data sample to the global databuffer and moves all data
 *  1 to the right.
 *
 * Input: data
 * Output: N/A
 * Action: adds a data sample to the global databuffer and moves all data
 *  1 to the right
 *                                                                            */
/******************************************************************************/
void PRO_AddToProcessBuffer(short data, unsigned char channel)
{
	short i;

	for(i=(DATA_BUFFER_SIZE -2);i >= 0;i--)
	{
		/* move everything to the right 1 */
		DataBuffer[channel][i+1] = DataBuffer[channel][i];
	}
	DataBuffer[channel][0] = data;
}

/******************************************************************************/
/* PRO_AddToProcessBuffer
 *
 * The function adds a data sample to the global databuffer and moves all data
 *  1 to the right.
 *
 * Input: data
 * Output: N/A
 * Action: adds a data sample to the global databuffer and moves all data
 *  1 to the right
 *                                                                            */
/******************************************************************************/
long PRO_ProcessData(unsigned char type, short window, unsigned char channel)
{
	short i;
	unsigned long temp;
	unsigned long denominator;

	if(window >= DATA_BUFFER_SIZE)
	{
		window = DATA_BUFFER_SIZE;
	}
	else if(window == 0)
	{
		window = 1;
	}

	switch (type)
	{
	case AVERAGE:
		/* calculates the average of the data in the window */
		temp = 0;
		for(i=0; i<window;i++)
		{
			temp += DataBuffer[channel][i];
		}
		temp /= window;
		break;
	case ABS_AVERAGE:
		/* calculates the average of the absolute value of the data in the window */
		temp = 0;
		for(i=0; i<window;i++)
		{
			temp += MSC_ABSL(DataBuffer[channel][i]);
		}
		temp /= window;
		break;
	case POS_AVERAGE:
		/* calculates the average of the positive values of the data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			if(DataBuffer[channel][i] >= 0)
			{
				temp += DataBuffer[channel][i];
				denominator++;
			}
		}
		if(denominator)
		{
			temp /= denominator;
		}
		break;
	case NEG_AVERAGE:
		/* calculates the average of the negative values of the data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			if(DataBuffer[channel][i] <= 0)
			{
				temp += DataBuffer[channel][i];
				denominator++;
			}
		}
		if(denominator)
		{
			temp /= denominator;
		}
		break;
	case RMS:
		/* calculates the average of the absolute value of the data in the window */
		temp = 0;
		for(i=0; i<window;i++)
		{
			temp += DataBuffer[channel][i] * DataBuffer[channel][i]; // x^2
		}
		temp /= window;
		temp = (long) sqrt((double)temp);
		break;
	case WEIGHTED_AVERAGE:
		/* calculates the weighted average of the data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			temp += DataBuffer[channel][i] * (window - i);
			denominator += (window - i);
		}
		temp /= denominator;
		break;
	case WEIGHTED_ABS_AVERAGE:
		/* calculates the weighted average of the absolute value of the data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			temp += MSC_ABSL(DataBuffer[channel][i] * (window - i));
			denominator += (window - i);
		}
		temp /= denominator;
		break;
	case WEIGHTED_POS_AVERAGE:
		/* calculates the weighted average of the positive data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			if(DataBuffer[channel][i] >= 0)
			{
				temp += (DataBuffer[channel][i] * (window - i));
				denominator += (window - i);
			}
		}
		if(denominator)
		{
			temp /= denominator;
		}
		break;
	case WEIGHTED_NEG_AVERAGE:
		/* calculates the weighted average of the negative data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			if(DataBuffer[channel][i] <= 0)
			{
				temp += (DataBuffer[channel][i] * (window - i));
				denominator += (window - i);
			}
		}
		if(denominator)
		{
			temp /= denominator;
		}
		break;
	case WEIGHTED_AVERAGE_2X:
		/* calculates the weighted double average of the data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			temp += DataBuffer[channel][i] * (window - i) * (window - i);
			denominator += (window - i) * (window - i);
		}
		temp /= denominator;
		break;
	case WEIGHTED_ABS_AVERAGE_2X:
		/* calculates the weighted double average of the absolute value of the data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			temp += MSC_ABSL(DataBuffer[channel][i] * (window - i) * (window - i));
			denominator += (window - i) * (window - i);
		}
		temp /= denominator;
		break;
	case WEIGHTED_POS_AVERAGE_2X:
		/* calculates the weighted double average of the positive data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			if(DataBuffer[channel][i] >= 0)
			{
				temp += (DataBuffer[channel][i] * (window - i) * (window - i));
				denominator += (window - i) * (window - i);
			}
		}
		if(denominator)
		{
			temp /= denominator;
		}
		break;
	case WEIGHTED_NEG_AVERAGE_2X:
		/* calculates the weighted double average of the negative data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			if(DataBuffer[channel][i] <= 0)
			{
				temp += (DataBuffer[channel][i] * (window - i) * (window - i));
				denominator += (window - i) * (window - i);
			}
		}
		if(denominator)
		{
			temp /= denominator;
		}
		break;
	case WEIGHTED_AVERAGE_d5X:
		/* calculates the weighted half average of the data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			temp += DataBuffer[channel][i] * ((window - i) / 2);
			denominator += (window - i) / 2;
		}
		temp /= denominator;
		break;
	case WEIGHTED_ABS_AVERAGE_d5X:
		/* calculates the weighted half average of the absolute value of the data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			temp += MSC_ABSL(DataBuffer[channel][i] * ((window - i) / 2));
			denominator += (window - i) / 2;
		}
		temp /= denominator;
		break;
	case WEIGHTED_POS_AVERAGE_d5X:
		/* calculates the weighted half average of the positive data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			if(DataBuffer[channel][i] >= 0)
			{
				temp += (DataBuffer[channel][i] * ((window - i) / 2));
				denominator += (window - i) / 2;
			}
		}
		if(denominator)
		{
			temp /= denominator;
		}
		break;
	case WEIGHTED_NEG_AVERAGE_d5X:
		/* calculates the weighted half average of the negative data in the window */
		temp = 0;
		denominator = 0;
		for(i=0; i<window;i++)
		{
			if(DataBuffer[channel][i] <= 0)
			{
				temp += (DataBuffer[channel][i] * ((window - i) / 2));
				denominator += (window - i) / 2;
			}
		}
		if(denominator)
		{
			temp /= denominator;
		}
		break;
	}
	return temp;
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
