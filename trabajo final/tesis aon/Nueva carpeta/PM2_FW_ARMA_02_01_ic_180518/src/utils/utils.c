/*----------------------------------Source File------------------------------

    Company          : Semitech Semiconductor

    Module           : crc.c

    Original Project : SM2200

    Purpose          : Utility functions.

    Prefix           : UTILS

    Original Author  : R.O.

    Creation Date    : 2 September 2013

    Notes            :


    History

    Date
    dd/mm/yyyy    Author           Description
    ----------    ------           -----------



   Copyright (c) 2010-2013 Semitech Semiconductor Pty. Ltd.  All rights reserved.

   Semitech Semiconductor is supplying this software for use solely and
   exclusively with Semitech's PLC products. The software is owned by
   Semitech, and is protected under applicable copyright laws. You may not
   combine this software with GPL or similarly licensed software in order
   to form a larger program if that combination will induce a duty to
   publicly disclose this software.

   You may not remove this copyright notice from
   this software.

   You may not distribute, rent, sub-license or otherwise make available to
   others this software or documentation or copies thereof, except as expressly
   permitted in this License without prior written consent from Semitech.

   THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
   IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
   SOFTWARE. SEMITECH SHALL NOT, UNDER ANY CIRCUMSTANCES, BE LIABLE FOR
   SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

  ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                                 MODULE INCLUDES
-----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
/*---------------------------------------------------------------------------
                             LOCAL DEFINED CONSTANTS
-----------------------------------------------------------------------------*/
#define  MAX_STRING_LENGTH    (512u) /* No string should ever be larger than
                                      * this
                                      */
#define UI_32_MASK_16_BIT     (0x0000FFFFu)
/*---------------------------------------------------------------------------
                                  LOCAL MACROS
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                                LOCAL DATA TYPES
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                                  LOCAL TABLES
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                            LOCAL FUNCTION PROTOTYPES
-----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                                 GLOBAL VARIABLES
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                                  LOCAL VARIABLES
-----------------------------------------------------------------------------*/
static unsigned long int next = 1;

/*---------------------------------------------------------------------------
                           GLOBAL FUNCTION DEFINITIONS
-----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
  Function   : UTILS_Memset
  Purpose    : This function will set an array of memory to a specific value.
  Parameters : destPtr - ptr to start of array
               value   - byte value to fill each byte of the array with
               dataLen - number of bytes in array to fill
  Returns    : None
  Notes      : IMPORTANT! - destPtr is not checked for NULL, this should be
                            done before this function is called.
-----------------------------------------------------------------------------*/
void UTILS_Memset (void *destPtr, unsigned char value, unsigned int dataLen)
{
   unsigned char *dataOutPtr;
   unsigned int  count;

   dataOutPtr = (unsigned char *)destPtr;
   for (count = 0; count < dataLen; count++)
   {
      dataOutPtr[count] = value;
   }
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Memcpy
  Purpose    : This function will copy data from one array to another.
  Parameters : destPtr - ptr to start of array where data should be written
               srcPtr  - ptr to start of array where data should be read
               dataLen - number of bytes to copy
  Returns    : None
  Notes      : IMPORTANT! - srcPtr and destPtr are not checked for NULL, this
                            should be done before this function is called.
-----------------------------------------------------------------------------*/
void UTILS_Memcpy (void *destPtr, const void *srcPtr, unsigned int dataLen)
{
   const unsigned char *dataIn8;
   unsigned char       *dataOut8;
   unsigned int        count;

   dataIn8 = srcPtr;
   dataOut8 = destPtr;
   for (count = 0; count < dataLen; count++)
   {
      dataOut8[count] = dataIn8[count];
   }
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Memcmp
  Purpose    : This function will compare data from one array with another,
               assuming data in LittleEndian format (comparing large numbers).
  Parameters : srcPtr  - ptr to start of 1st array
               destPtr - ptr to start of 2nd array
               dataLen - number of bytes to compare
  Returns    : Comparison value, 0 if arrays are equal for specified length
  Notes      : IMPORTANT! - srcPtr and destPtr are not checked for NULL, this
                            should be done before this function is called.
-----------------------------------------------------------------------------*/
int UTILS_Memcmp (const unsigned char *srcPtr, const unsigned char *destPtr, unsigned int dataLen)
{
   int diffVal;

   diffVal = 0;
   while ((diffVal == 0) && (dataLen != 0))
   {
      --dataLen;
      diffVal = srcPtr[dataLen] - destPtr[dataLen];
   } /* while dataLen not zero */

   return diffVal;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Strlen
  Purpose    : This function will return the length of a zero terminated string
  Parameters : strPtr - pointer to the ASCII string
  Returns    : length of the ASCII string
  Notes      : IMPORTANT! - strPtr is not checked for NULL, this should be
                            done before this function is called.
-----------------------------------------------------------------------------*/
unsigned int UTILS_Strlen (const unsigned char *strPtr)
{
   unsigned int retval = 0;

   /* Loop while looking for EOS character */
   while ((*(strPtr++) != 0) && (retval < MAX_STRING_LENGTH))
   {
      ++retval;
   }
   return retval;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_StrnCopy
  Purpose    : This function will copy a string from src to dst up to maxStrLen
               or when NULL terminator is reached.
               No Null terminator is appended after the copied string.
  Parameters : dstPtr    - ptr to copy string to
               srcPtr    - pointer to the ASCII string to copy
               maxStrLen - max length of string to copy
  Returns    : ptr to dst after copying
  Notes      : IMPORTANT! - ptrs is not checked for NULL, this should be
                            done before this function is called.
-----------------------------------------------------------------------------*/
unsigned char *UTILS_StrnCopy (unsigned char *dstPtr, const unsigned char  *srcPtr, unsigned int maxStrLen)
{
   while ((maxStrLen-- > 0) && (*srcPtr != '\0'))
   {
      *dstPtr++ = *srcPtr++;
   }

   return dstPtr;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Strcmp - with limits!
  Purpose    : This function will compare data from one array with another,
               assuming data in BigEndian format (or like StringCompare).
  Parameters : srcPtr  - ptr to start of 1st array
               destPtr - ptr to start of 2nd array
  Returns    : Comparison value, 0 if arrays are equal for specified length
  Notes      : IMPORTANT! - srcPtr and destPtr are not checked for NULL, this
                            should be done before this function is called.
-----------------------------------------------------------------------------*/
short UTILS_Strcmp (const unsigned char *srcPtr, const unsigned char *destPtr)
{
   short diffVal;
   unsigned int dataCount;
   unsigned short strLen1, strLen2;

   /* Compare the string lengths */
   strLen1 = (unsigned short)UTILS_Strlen(srcPtr);
   if (strLen1 > MAX_STRING_LENGTH)
   {
      return 1;
   }

   strLen2 = (unsigned short)UTILS_Strlen(destPtr);
   if (strLen2 > MAX_STRING_LENGTH)
   {
      return 1;
   }

   diffVal = strLen1 - strLen2;
   if (diffVal == 0)
   {  /* if strings are equal in length then compare the data */
      dataCount = 0;
      while ((diffVal == 0) && (dataCount < strLen1))
      {
         diffVal = srcPtr[dataCount] - destPtr[dataCount];
         ++dataCount;
      } /* while dataCount smaller than dataLen */
   }

   return diffVal;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Strncmp - with limits!
  Purpose    : This function will compare data from one array with another,
               assuming data in BigEndian format (or like StringCompare).
  Parameters : srcPtr  - ptr to start of 1st array
               destPtr - ptr to start of 2nd array
               dataLen - number of bytes to compare
  Returns    : Comparison value, 0 if arrays are equal for specified length
  Notes      : IMPORTANT! - srcPtr and destPtr are not checked for NULL, this
                            should be done before this function is called.
-----------------------------------------------------------------------------*/
short UTILS_Strncmp (const unsigned char *srcPtr,
                     const unsigned char *destPtr,
                     unsigned int        maxStringLen)
{
   short diffVal;
   unsigned int dataCount;
   unsigned short strLen1, strLen2;

   /* Compare the string lengths */
   strLen1 = (unsigned short)UTILS_Strlen(srcPtr);
   if (strLen1 > maxStringLen)
   {
      strLen1 =  maxStringLen & UI_32_MASK_16_BIT;
   }

   strLen2 = (unsigned short)UTILS_Strlen(destPtr);
   if (strLen2 > maxStringLen)
   {
      strLen2 = maxStringLen  & UI_32_MASK_16_BIT;
   }

   diffVal = strLen1 - strLen2;
   if (diffVal == 0)
   {  /* if strings are equal in length then compare the data */
      if (strLen1 < maxStringLen)
      {
         maxStringLen = strLen1;
      }
      dataCount = 0;
      while ((diffVal == 0) && (dataCount < maxStringLen))
      {
         diffVal = srcPtr[dataCount] - destPtr[dataCount];
         ++dataCount;
      } /* while dataCount smaller than dataLen */
   }

   return diffVal;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Strchr
  Purpose    : This function will find the first occurrence of a character
  Parameters : srcPtr  - ptr to start of 1st array
               chr     - what we are looking for
  Returns    : 0 if not found, else ptr to where found
  Notes      :
-----------------------------------------------------------------------------*/
char *UTILS_Strchr(char *srcPtr, char chr)
{
   if (srcPtr == ((void *) 0))
   {
      return ((void *) 0); /* Not a valid string */
   }

   if (UTILS_Strlen((unsigned char *)srcPtr) > MAX_STRING_LENGTH)
   {
      return ((void *) 0);
   }

    while (*srcPtr != chr)
    {
        if (!*srcPtr++)
        {
            return ((void *) 0);
        }
    }
    return (char *)srcPtr;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_strtok
  Purpose    : Breaks a string into different parameters that are sparated by
               a delimiter. The delimiter is replaced with a null terminator in the
               original string and the tokens are reference points into the string.
  Parameters : str   - the string to tokenise
                  tokens - an array that is to hold the references to all the tokens
                  within the string.
                  delimiter - the character that defines the spacing between tokens
  Returns    : the number of tokens found (a value of 1 means that the string wasn't broken up at all)
  Notes      :
-----------------------------------------------------------------------------*/
unsigned int  UTILS_Strtok(char *str, char *tokens[], char max_tokens, char delimiter)
{
   int count;
   char *tempPos;
   char *prevPos;
   bool foundNext = false;

   /*clear List*/
   for (count = 0; count < max_tokens; ++count)
   {
      tokens[count] = 0;
   }

   //tokens[0] = str;
   tempPos = str;
   prevPos = str;
   for (count = 0; count < max_tokens; ++count)
   {
      //save the reference to the token
      tokens[count] = tempPos;
      tempPos = UTILS_Strchr (tempPos, delimiter);

      //got to end of string so exit
      if(0 == tempPos)
      {
         //count is used to know how many tokens there are, so still need to update when exit in the case of a valid token,
         //as this means at end of string
         if((tokens[count][0] != 0))
         {
            count++;
         }
         break;
      }

      //detects multiple delimiters in a row

      if((tempPos == prevPos))
      {
         //alright to go negative for first time as when used as an index to tokens, will be >=0
         count--;

      }
      else
      {
         foundNext = true;
      }

      if (tempPos != 0)
      {
         if(foundNext)
         {
            tempPos[0] = 0;      /* replace the delimiter with a null character, so that it breaks up the original string */
            foundNext = false;
         }
         tempPos++; //the next character is guaranteed to exist, as at most it will be the end of the string
      }
      else
      {

      }

      prevPos = tempPos;
   }

   //adjustment in the case that the string is entirely delimiters
   if(count == 0)
   {
      tokens[0] = 0;
   }

   //either the loop is broken and the count has been adjusted to include the last token
   //or the loop has finished and there are the maximum number of tokens, so just need to return.
   return count;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Atoi
  Purpose    : This function return the interger value for a string
  Parameters : srcPtr  - ptr to string
  Returns    : false if not found and output is not valid; true if found and output is valid
  Notes      :
-----------------------------------------------------------------------------*/
bool UTILS_Atoi(char *srcPtr, int32_t *output)
{
   int res = 0;           /* Initialize result               */
   int sign = 1;          /* Initialize sign as positive     */
   unsigned char i = 0;    /* Initialize index of first digit */

   if (srcPtr == ((void *) 0))
   {
       return false; /* Not a valid string */
   }

   /* Must fit into a int */
   if (UTILS_Strlen((unsigned char *)srcPtr) > 0x11)
   {
      return false;
   }

   /* If number is negative, then update sign */
   if (srcPtr[0] == '-')
   {
       sign = -1;
       i++;  // Also update index of first digit
   }

   /* Iterate through all digits of input string and update result */
   for (; srcPtr[i] != '\0'; ++i)
   {
      if ((srcPtr[i] > '9') || (srcPtr[i] < '0'))
      {
         return false; /* not a valid digit */
      }
      res = res*10 + srcPtr[i] - '0';
    }

   /* Return result with sign */
   *output = sign*res;
   return true;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Atoi_hex
  Purpose    : This function returns the integer value for a string of hex digits
  Parameters : srcPtr  - ptr to string
  Returns    : false if not found and output is not valid; true if found and output is valid
  Notes      :
-----------------------------------------------------------------------------*/
bool UTILS_Atoi_hex(char *srcPtr, uint32_t *output)
{
   uint32_t res = 0;           /* Initialize result               */
   unsigned char i = 0;    /* Initialize index of first digit */

   if (srcPtr == ((void *) 0))
   {
      return false; /* Not a valid string */
   }

   /* Max hex digits for 32bit is 8 */
   if (UTILS_Strlen((unsigned char *)srcPtr) > 0x8)
   {
      return false;
   }


   /* Iterate through all digits of input string and update result */
   for (; srcPtr[i] != '\0'; ++i)
   {
      //process numeral digits first
      if ((srcPtr[i] <= '9') && (srcPtr[i] >= '0'))
      {
         res = res*16 + srcPtr[i] - '0';
      }
      else if((srcPtr[i] <= 'f') && (srcPtr[i] >= 'a'))
      {
         res = res*16 + srcPtr[i] - 'a' + 10;
      }
      else if((srcPtr[i] <= 'F') && (srcPtr[i] >= 'A'))
      {
         res = res*16 + srcPtr[i] - 'A' + 10;
      }
      else
      {
         //not a valid digit
         return false;
      }
   }

    /* Return result with sign */
   *output = res;
   return true;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Rand
  Purpose    : This function return a "random" value between 0 and 32767
  Parameters : None
  Returns    : the "random" value
  Notes      : RAND_MAX assumed to be 32767
-----------------------------------------------------------------------------*/
int UTILS_Rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

/*---------------------------------------------------------------------------
  Function   : UTILS_Srand
  Purpose    : This function sets the seed for UTILS_Rand
  Parameters : seed - The seed for the random function
  Returns    : None
  Notes      :
-----------------------------------------------------------------------------*/
void UTILS_Srand(unsigned int seed)
{
    next = seed;
}


/*---------------------------------------------------------------------------
  Function   : UTILS_Bitcpy
  Purpose    : This function will copy bits from one array to another.
               The bits will be shifted so that the startbit is in position 0
               of the new array.
  Parameters : destPtr  - ptr to start of array where data should be written
               srcPtr   - ptr to start of array where data should be read
               bitStart - The First bit to copy
               len      - The amount of bits to copy
  Returns    : The size of the new array.
  Notes      : IMPORTANT! - srcPtr and destPtr are not checked for NULL, this
                            should be done before this function is called.
-----------------------------------------------------------------------------*/
unsigned int UTILS_Bitcpy (void *destPtr, const void *srcPtr, unsigned int bitStart, unsigned int len )
{
   /* Amount of bytes to have*/
   const unsigned char *dataIn8 = srcPtr;
   unsigned char       *dataOut8 = destPtr;
   unsigned int        bytesCount = (len + 7)/ 8;
   unsigned int        count;
   unsigned int        byteOffset = bitStart / 8;
   unsigned char       bitOffsetMSB = bitStart % 8;
   unsigned char       bitOffsetLSB = 8 - bitOffsetMSB;

   /* Copy all of the bits */
   for (count = 0; count < bytesCount; ++count)
   {
      dataOut8[count] = (dataIn8[count + byteOffset] << bitOffsetMSB);
      dataOut8[count] |= (dataIn8[count + byteOffset + 1] >> bitOffsetLSB);
   }

   /*Mask the last few bits off */
   if ((len % 8) > 0)
   {
      dataOut8[count -1] &= (0xff << (8 - (len % 8)));
   }
   return bytesCount;
}

