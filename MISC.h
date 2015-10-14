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
/* Files to Include                                                           */
/******************************************************************************/
#ifndef MISC_H
#define	MISC_H

#include <msp430.h>

#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define BIG     1
#define LITTLE  2
#define MIDDLE  3

/******************************************************************************/
/* Macro Functions                                                            */
/******************************************************************************/
#define NOP() __no_operation()
#define Nop() NOP()

/******************************************************************************/
/* Function prototypes                                                        */
/******************************************************************************/
void MSC_DelayUS(long US);
double MSC_ABS(double input);
double MSC_Round(double input);
void MSC_CleanBuffer(void* data, unsigned short bytes);
unsigned char IsLetter(unsigned char data);
unsigned char MSC_IsNumber(unsigned char data);
unsigned char MSC_IsCharacter(unsigned char data);
unsigned char MSC_IsAlphaNumeric(unsigned char data);
unsigned char MSC_IsAlphaNumericString(unsigned char* data);
void MSC_BufferFill(void* buffer, unsigned long data, unsigned char bits, unsigned short bytes);
unsigned short MSC_SizeOfString(unsigned char* string);
void MSC_StringCopy(unsigned char* from,unsigned char* to);
unsigned char MSC_StringMatch(void* This, void* That);
unsigned char MSC_BufferMatch(void* buffer1, void* buffer2, unsigned short bytes);
void MSC_BufferCopy(void* From, void* To, unsigned short bytes);
void MSC_LowercaseString(unsigned char* Input);
void MSC_LowercaseChar(unsigned char* Input);
unsigned short MSC_HEXtoBCD(unsigned short input);
unsigned short MSC_BCDtoHEX(unsigned short input);
unsigned long MSC_Endian(unsigned long number, unsigned char bits, unsigned char style);
#endif	/* MISC_H */
