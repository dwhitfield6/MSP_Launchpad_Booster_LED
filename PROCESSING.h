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
#ifndef PROCESSING_H
#define	PROCESSING_H

#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* Processing types
 *
 * This is the available types of data processing available.
 *                                                                            */
/******************************************************************************/
#define AVERAGE 				0
#define ABS_AVERAGE 			1
#define POS_AVERAGE 			2
#define NEG_AVERAGE 			3
#define RMS						4
#define WEIGHTED_AVERAGE 	 	5
#define WEIGHTED_ABS_AVERAGE 	6
#define WEIGHTED_POS_AVERAGE 	7
#define WEIGHTED_NEG_AVERAGE 	8
#define WEIGHTED_AVERAGE_2X		9
#define WEIGHTED_ABS_AVERAGE_2X 10
#define WEIGHTED_POS_AVERAGE_2X 11
#define WEIGHTED_NEG_AVERAGE_2X 12
#define WEIGHTED_AVERAGE_d5X		13
#define WEIGHTED_ABS_AVERAGE_d5X 	14
#define WEIGHTED_POS_AVERAGE_d5X 	15
#define WEIGHTED_NEG_AVERAGE_d5X 	16

/******************************************************************************/
/* DATA_BUFFER_SIZE
 *
 * This is the number of data samples of the processing buffer.
 *                                                                            */
/******************************************************************************/
#define DATA_BUFFER_SIZE 1000

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
extern short DataBuffer[2][DATA_BUFFER_SIZE];
extern short GlobalVolume;

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void Init_Processing(void);
void PRO_ClearProcessBuffer(void);
void PRO_AddToProcessBuffer(short data, unsigned char channel);
long PRO_ProcessData(unsigned char type, short window, unsigned char channel);

#endif	/* PROCESSING_H */
