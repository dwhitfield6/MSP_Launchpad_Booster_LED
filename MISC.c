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
/* Contains Miscellaneous Functions
 *                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <msp430.h>
#include <float.h>

#include "MISC.h"
#include "USER.h"
#include "SYSTEM.h"

/******************************************************************************/
/* Inline Functions                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

/******************************************************************************/
/* MSC_DelayUS
 * Input the number in microseconds to be delayed.
 *
 * The function waists loops for the entered number of cycles.
 * The actual delay can be skewed when interrupts are enabled.
 *
 * Input: microseconds
 * Output: N/A
 * Action: waits the number of microseconds.
 *                                                                            */
/******************************************************************************/
void MSC_DelayUS(long US)
{
    long i;

    for(i=US; i>0; i--)
    {
        NOP();
    }
}

/******************************************************************************/
/* MSC_ABS
 *
 * The function returns the absolute value of the number.
 *
 * Input: number
 * Output: absolute value of number
 * Action: calculates the absolute vaule of the input
 *                                                                            */
/******************************************************************************/
double MSC_ABS(double input)
{
	if(input < DBL_MIN)
	{
		input *= -1.0;
	}
	return input;
}

/******************************************************************************/
/* MSC_Round
 *
 * The function rounds a number to the nearest number.
 *
 * Input: number
 * Output: rounded number
 * Action: rounds the input number to the nearest whole number.
 *                                                                            */
/******************************************************************************/
double MSC_Round(double input)
{
    long temp = (long)(input + 0.5);

    return (double) temp;
}

/******************************************************************************/
/* MSC_CleanBuffer
 *
 * This function sets an amount of data in the array as 0.
 *
 * Input: pointer to an array or data
 * Output: N/A
 * Action: sets the array data to all zeros
 *                                                                            */
/******************************************************************************/
void MSC_CleanBuffer(void* data, unsigned short bytes)
{
    unsigned short i=0;
    for(i=0; i<bytes;i++)
    {
        *(unsigned char*)data= 0;
        (unsigned char*) data++;
    }
}


/******************************************************************************/
/* IsLetter
 *
 * The function returns true if the data is a Letter.
 *
 * Input: character
 * Output: TRUE or FALSE
 * Action: returns TRUE if the byte is an ASCII letter
 *                                                                            */
/******************************************************************************/
unsigned char IsLetter(unsigned char data)
{
    if((data >= 65 && data <= 90) || (data >= 97 && data <= 122))
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* MSC_IsNumber
 *
 * The function returns true if the data is a number.
 *
 * Input: character
 * Output: TRUE or FALSE
 * Action: returns TRUE if the byte is an ASCII Number
 *                                                                            */
/******************************************************************************/
unsigned char MSC_IsNumber(unsigned char data)
{
    if(data >= '0' && data <= '9')
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* MSC_IsCharacter
 *
 * The function returns true if the data is a printable character.
 *
 * Input: pointer to an array or data
 * Output: N/A
 * Action: sets the array data to all zeros
 *                                                                            */
/******************************************************************************/
unsigned char MSC_IsCharacter(unsigned char data)
{
    if(data >= 32 && data <= 126)
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* MSC_IsAlphaNumeric
 *
 * The function returns true if the data is either a letter or a number or
 *  a space.
 *
 * Input: character
 * Output: TRUE or FALSE
 * Action: returns TRUE if the byte is an ASCII Number or letter
 *                                                                            */
/******************************************************************************/
unsigned char MSC_IsAlphaNumeric(unsigned char data)
{
    if(IsLetter(data) || MSC_IsNumber(data))
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/* MSC_IsAlphaNumericString
 *
 * The function returns true if the string contains only letters and numbers
 *  and spaces.
 *
 * Input: pointer to ta string
 * Output: TRUE or FALSE
 * Action: returns TRUE if the string is all ASCII Numbers or letters
 *                                                                            */
/******************************************************************************/
unsigned char MSC_IsAlphaNumericString(unsigned char* data)
{
    while(*data != 0)
    {
        if(!MSC_IsAlphaNumeric(*data))
        {
            return FALSE;
        }
        data++;
    }
    return TRUE;
}

/******************************************************************************/
/* MSC_BufferFill
 *
 * This function fills a buffer with dummy data.
 *
 * Input: pointer to an array or data, data to fill the buffer with, the number
 *  of bits of the data type (8, 16, or 32), number of bytes
 * Output: N/A
 * Action: fills a buffer with the data
 *                                                                            */
/******************************************************************************/
void MSC_BufferFill(void* buffer, unsigned long data, unsigned char bits, unsigned short bytes)
{
    unsigned short i;

    if(bits == 8)
    {
        for(i=0;i<bytes;i++)
        {
            *(unsigned char*)buffer = (unsigned char) data;
            (unsigned char*)buffer++;
        }
    }
    else if(bits == 16)
    {
        for(i=0;i<bytes;i+=2)
        {
            *(unsigned char*)buffer = (unsigned char) (data >> 8);
            (unsigned char*)buffer++;
            *(unsigned char*)buffer = (unsigned char) data;
            (unsigned char*)buffer++;
        }
    }
    else
    {
        for(i=0;i<bytes;i+=4)
        {
            *(unsigned char*)buffer = (unsigned char) (data >> 24);
            (unsigned char*)buffer++;
            *(unsigned char*)buffer = (unsigned char) (data >> 16);
            (unsigned char*)buffer++;
            *(unsigned char*)buffer = (unsigned char) (data >> 8);
            (unsigned char*)buffer++;
            *(unsigned char*)buffer = (unsigned char) data;
            (unsigned char*)buffer++;
        }
    }
}

/******************************************************************************/
/* MSC_SizeOfString
 *
 * This function returns the size of the string including the ending null
 *  character.
 *
 * Input: pointer to an  string
 * Output: the number of bytes/characters in the string including the NULL
 * Action: returns the size of the sting in bytes.
 *                                                                            */
/******************************************************************************/
unsigned short MSC_SizeOfString(unsigned char* string)
{
    unsigned short i = 1;

    while(*string != 0)
    {
        i++;
        string++;
    }
    return i;
}

/******************************************************************************/
/* MSC_StringCopy
 *
 * This function copies a string "from" to "to".
 *
 * Input: pointer to an array or data, pointer to an array or data
 * Output: N/A
 * Action: copies an array from one place to another
 *                                                                            */
/******************************************************************************/
void MSC_StringCopy(unsigned char* from,unsigned char* to)
{
    while(*from != 0)
    {
        *to = *from;
        from++;
        to++;
    }
    *to = *from;
}

/******************************************************************************/
/* MSC_StringMatch
 *
 * This function returns TRUE if the array 'This' matches the array 'That'.
 *
 * Input: pointer to an array or data, pointer to an array or data
 * Output: TRUE or FALSE
 * Action: returns true if the strings match
 *                                                                            */
/******************************************************************************/
unsigned char MSC_StringMatch(void* This, void* That)
{
    while(*(unsigned char*)This != 0)
    {
       if(*(unsigned char*)This != *(unsigned char*)That || *(unsigned char*)That == 0)
       {
           return FALSE;
       }
       (unsigned char*)This++;
       (unsigned char*)That++;
    }
    if(*(unsigned char*)That == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
/* MSC_BufferMatch
 *
 * This function checks to see if two buffers are equal.
 *
 * Input: pointer to an array or data, pointer to an array or data, number
 *  of bytes
 * Output: TRUE or FALSE
 * Action: returns true if the buffers match
 *                                                                            */
/******************************************************************************/
unsigned char MSC_BufferMatch(void* buffer1, void* buffer2, unsigned short bytes)
{
    unsigned short i;

    for(i=0;i<bytes;i++)
    {
        if(*(unsigned char*)buffer1 != *(unsigned char*)buffer2)
        {
            return FAIL;
        }
        (unsigned char*)buffer1++;
        (unsigned char*)buffer2++;
    }
    return PASS;
}

/******************************************************************************/
/* MSC_BufferCopy
 *
 * This function copies 'from' buffer to 'To' buffer.
 *
 * Input: pointer to an array or data, pointer to an array or data, number
 *  of bytes to copy
 * Output: N/A
 * Action: copies 'from' buffer to 'To' buffer.
 *                                                                            */
/******************************************************************************/
void MSC_BufferCopy(void* From, void* To, unsigned short bytes)
{
    unsigned short i;

    for(i=0;i<bytes;i++)
    {
        *(unsigned char*)To = *(unsigned char*)From;
        (unsigned char*)To++;
        (unsigned char*)From++;
    }
}

/******************************************************************************/
/* MSC_LowercaseString
 *
 * This function turns all characters to in the string to lowercase.
 *
 * Input: pointer to an array or data
 * Output: N/A
 * Action: turns all characters to in the string to lowercase.
 *                                                                            */
/******************************************************************************/
void MSC_LowercaseString(unsigned char* Input)
{
    while((*Input != 0))
    {
        if((*Input >= 65) && (*Input <= 90 ))
        {
            *Input += 32;
        }
        Input++;
    }
}

/******************************************************************************/
/* MSC_LowercaseChar
 *
 * This function turns the character to lowercase.
 *
 * Input: character
 * Output: N/A
 * Action: turns the character to lowercase.
 *                                                                            */
/******************************************************************************/
void MSC_LowercaseChar(unsigned char* Input)
{
    if((*Input >= 65) && (*Input <= 90 ))
    {
        *Input += 32;
    }
}

/******************************************************************************/
/* MSC_HEXtoBCD
 *
 * This function converts from Hex to BCD.
 *
 * Input: an integer
 * Output: the integer in BCD format
 * Action: converts from Hex to BCD.
 *                                                                            */
/******************************************************************************/
unsigned short MSC_HEXtoBCD(unsigned short input)
{
    unsigned short temp0;
    unsigned short temp1;
    unsigned short temp2;
    unsigned short temp3;
    unsigned short Value = input;

    temp3 = Value/1000;
    Value -= (temp3 * 1000);
    temp2 = Value / 100;
    Value -= (temp2 * 100);
    temp1 = Value / 10;
    Value -= (temp1 * 10);
    temp0 = Value;

    return (temp0 + (temp1 << 4) + (temp2 << 8) + (temp3 << 12));
}

/******************************************************************************/
/* MSC_BCDtoHEX
 *
 * This function converts from BCD to Hex.
 *
 * Input:  an integer in BCD format
 * Output: the integer in HEX
 * Action: converts from BCD to Hex.
 *                                                                            */
/******************************************************************************/
unsigned short MSC_BCDtoHEX(unsigned short input)
{
    unsigned short temp0;
    unsigned short temp1;
    unsigned short temp2;
    unsigned short temp3;
    unsigned short Value = input;

    temp3 = Value >> 12;
    Value -= (temp3 << 12);
    temp2 = Value >> 8;
    Value -= (temp2 << 8);
    temp1 = Value >> 4;
    Value -= (temp1 << 4);
    temp0 = Value;

    return (temp0 + (temp1 * 10) + (temp2 * 100) + (temp3 * 1000));
}

/******************************************************************************/
/* MSC_Endian
 *
 * This function converts a number from big endian to little endian or
 * vice versa.
 *
 * Input: number to convert, the number of bits (8, 16, or 32), style
 *  (BIG, MIDDLE, LITTLE)
 * Output: the coverted data
 * Action: converts a number from big endian to little endian or
 * vice versa.
 *                                                                            */
/******************************************************************************/
unsigned long MSC_Endian(unsigned long number, unsigned char bits, unsigned char style)
{
    unsigned long temp1, temp2,temp3,temp4;

    if(bits == 16)
    {
        temp1 = (number & 0x000000FF);
        temp2 = ((number & 0x0000FF00) >> 8);
        return (temp2 | (temp1 << 8));
    }
    else
    {
        temp1 = (number & 0x000000FF);
        temp2 = ((number & 0x0000FF00) >> 8);
        temp3 = ((number & 0x00FF0000) >> 16);
        temp4 = ((number & 0xFF000000) >> 24);
        if(style == LITTLE || style == BIG)
        {

            return (temp4 | (temp3 << 8) | (temp2 << 16) | (temp1 << 24));
        }
        else
        {
            /* Middle endian */
            return (temp2 | (temp1 << 8) | (temp4 << 16) | (temp3 << 24));
        }
    }
}

/*-----------------------------------------------------------------------------/
 End of File
/-----------------------------------------------------------------------------*/
