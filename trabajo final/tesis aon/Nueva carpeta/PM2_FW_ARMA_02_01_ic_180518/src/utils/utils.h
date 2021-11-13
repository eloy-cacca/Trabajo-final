// $Header
/// @file
/// @brief Utility functions
/// @details 
/// @author Riaan Oosthuizen
/// @version   0.2
/// @date      23 January 2014
/// @copyright
/// Copyright (c) 2010-2014 Semitech Semiconductor Pty. Ltd.  All rights reserved.
///
/// Semitech Semiconductor is supplying this software for use solely and
/// exclusively with Semitech's PLC products. The software is owned by
/// Semitech, and is protected under applicable copyright laws. You may not
/// combine this software with GPL or similarly licensed software in order
/// to form a larger program if that combination will induce a duty to
/// publicly disclose this software.
///
/// You may not remove this copyright notice from
/// this software.
///
/// You may not distribute, rent, sub-license or otherwise make available to
/// others this software or documentation or copies thereof, except as expressly
/// permitted in this License without prior written consent from Semitech.
///
/// THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
/// IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES
/// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS
/// SOFTWARE. SEMITECH SHALL NOT, UNDER ANY CIRCUMSTANCES, BE LIABLE FOR
/// SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
///

#ifndef UTILS_H_
#define UTILS_H_

/**************************** INCLUDES *****************************************/

#include <stdint.h>
#include <stdbool.h>

/**************************** MACROS *****************************************/

/* Unpack a 8 bit value to one byte (little endian) */
#define PUTBYTE(addr,byte)                                                    \
{                                                                             \
   *(addr + 0) = (byte) & 0xff;                                               \
}
/* Pack 1 byte into a 8 bit value (little endian) */
#define GETBYTE(addr,byte)                                                    \
{                                                                             \
   byte = *(addr + 0);                                                        \
}
/* Unpack a 16 bit value to two bytes (little endian) */
#define PUTWORD(addr,word)                                                    \
{                                                                             \
   *(addr + 0) = (word>> 0) & 0xff;                                           \
   *(addr + 1) = (word>> 8) & 0xff;                                           \
}
/* Pack 2 bytes into a 16 bit value (little endian) */
#define GETWORD(addr,word)                                                    \
{                                                                             \
   word  = *(addr + 1);                                                       \
   word <<= 8;                                                                \
   word |= *(addr + 0);                                                       \
}
/* Unpack a 32 bit value to four bytes (little endian) */
#define PUTDWORD(addr,word)                                                   \
{                                                                             \
   *(addr + 0) = (word>>  0) & 0xff;                                          \
   *(addr + 1) = (word>>  8) & 0xff;                                          \
   *(addr + 2) = (word>> 16) & 0xff;                                          \
   *(addr + 3) = (word>> 24) & 0xff;                                          \
}

/* Pack 4 bytes into a 32 bit value (little endian) */
#define GETDWORD(addr,word)                                                   \
{                                                                             \
   word  = *(addr + 3);                                                       \
   word <<= 8;                                                                \
   word |= *(addr + 2);                                                       \
   word <<= 8;                                                                \
   word |= *(addr + 1);                                                       \
   word <<= 8;                                                                \
   word |= *(addr + 0);                                                       \
}

/* Convert seconds to milliseconds */
#define SEC_TO_MILSEC(var)        ((var) * 1000)

/* Convert 0.seconds to milliseconds */
#define PSEC_TO_MILSEC(var)       ((var) * 100)

/* Convert 0.0x seconds to milliseconds */
#define TSEC_TO_MILSEC(var)       ((var) * 10)


#define TOKEN_PASTE(x,y)   x##y
#define TOKEN_PASTE3(x,y,z)   x##y##z


#define SET_FIELD(Reg,Mask,Val)   (Reg)=(((Reg)&(~(Mask)))|((Val)&(Mask)))

//can't make an inline function as don't know the type of the inputs
#define SET_FIELD_VAR(Reg,Mask,Val)                                           \
do{                                                                           \
   uint32_t cur = (Reg);                                                      \
   cur &= ~(Mask);                                                            \
   cur |= (Val) & (Mask);                                                     \
   (Reg) = cur;                                                               \
   }while(0);

//big to little endian conversion
#ifdef __XTENSA_EB__
   #define ADJ_E32(x)   ((((x)&0xff)<<24)|(((x)&0xff00)<<8)|                  \
                   (((x)&0xff0000)>>8)|(((x)&0xff000000)>>24))

   #define ADJ_E16(x)   ((((x)&0xff)<<8)|(((x)&0xff00)>>8))

   static inline void AdjustEndianLoad32 (uint32_t *pDest, const uint32_t *pSrc)
   {
      char *pD = (char *)pDest;
      const char *pS = (char *)pSrc;
      pD[0] = pS[3]; pD[1] = pS[2]; pD[2] = pS[1]; pD[3] = pS[0];
   }
#else
   #define ADJ_E32(x)   (x)
   #define ADJ_E16(x)   (x)
   #define AdjustEndianLoad32(x,y)   *(x)=*(y) 
#endif



/**************************** ENUMS, STRUCTS *********************************/


/**************************** TYPEDEF *****************************************/


/**************************** VARIABLES **************************************/


/**************************** FUNCTIONS **************************************/
/// @cond
  
  /* Functions equivalent to c library functions */
extern void           UTILS_Memset      (void                 *destPtr,
                                         unsigned char        value,
                                         unsigned int         dataLen);

extern void           UTILS_Memcpy      (void                 *destPtr,
                                         const void           *srcPtr,
                                         unsigned int         dataLen);

extern int            UTILS_Memcmp      (const unsigned char  *srcPtr,
                                         const unsigned char  *destPtr,
                                         unsigned int         dataLen);

unsigned int          UTILS_Obtain_Parm (char                 *str,
                                         char                 *tokens[],
                                         char                 max_tokens,
                                         char                 delimiter);

extern unsigned int   UTILS_Strlen      (const unsigned char  *strPtr);

extern unsigned char *UTILS_StrnCopy    (unsigned char        *destPtr,
                                         const unsigned char  *strPtr,
                                         unsigned int          maxStrLen);

extern short          UTILS_Strcmp      (const unsigned char  *srcPtr,
                                         const unsigned char  *destPtr);

extern short          UTILS_Strncmp     (const unsigned char  *srcPtr,
                                         const unsigned char  *destPtr,
                                         unsigned int         maxStringLen);

extern char          *UTILS_Strchr      (char                 *srcPtr,
                                         char                 chr);

unsigned int          UTILS_Strtok      (char                 *str,
                                         char                 *tokens[],
                                         char                 max_tokens,
                                         char                 delimiter);

extern bool           UTILS_Atoi        (char                 *srcPtr,
                                         int32_t              *output);

extern bool           UTILS_Atoi_hex    (char                 *srcPtr,
                                         uint32_t             *output);

extern int            UTILS_Rand        (void) ;

extern void           UTILS_Srand       (unsigned int         seed);

extern unsigned int   UTILS_Bitcpy      (void                 *destPtr,
                                         const void           *srcPtr,
                                         unsigned int         bitStart,
                                         unsigned int         len);

/// @endcond

#endif /* UTILS_H_ */
