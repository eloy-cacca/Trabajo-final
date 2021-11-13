#include "DSP2833x_library.h"

/* Convert to and from floating point */
#define TOFIX(d, q) ((int32_t)( (d)*(double)((int32_t)1u<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)((int32)1u<<(q)) )

// Macros basicos de IQmath
#define FPMUL(a,b,q) ((int32)( ((int64)a) * (b) >> (q) ))
#define FPMUL64(a,b,q) ((int64)( ((int64)a) * (b) >> (q) ))
#define FPDIV(a,b,q) ((int32)( (((int64)a) << (q) ) / (b) ))

// IQMath
#define Q01 		((Uint32)1u<<1)
#define Q02 		((Uint32)1u<<2)
#define Q03 		((Uint32)1u<<3)
#define Q04 		((Uint32)1u<<4)
#define Q05 		((Uint32)1u<<5)
#define Q06 		((Uint32)1u<<6)
#define Q07 		((Uint32)1u<<7)
#define Q08 		((Uint32)1u<<8)
#define Q09 		((Uint32)1u<<9)
#define Q10 		((Uint32)1u<<10)
#define Q11 		((Uint32)1u<<11)
#define Q12 		((Uint32)1u<<12)
#define Q13 		((Uint32)1u<<13)
#define Q14 		((Uint32)1u<<14)
#define Q15 		((Uint32)1u<<15)
#define Q16 		((Uint32)1u<<16)
#define Q17 		((Uint32)1u<<17)
#define Q18 		((Uint32)1u<<18)
#define Q19 		((Uint32)1u<<19)
