#include "settings.h"

/* Convert to and from floating point */
#define TOFIX(d, q) ((int32_t)( (d)*(double)((int32_t)1u<<(q)) ))
#define TOFLT(a, q) ( (double)(a) / (double)((int32_t)1u<<(q)) )

// Macros basicos de IQmath
#define FPMUL16(a,b,q) ((int16_t)( ((int32_t)a) * (b) >> (q) ))
#define FPMUL32(a,b,q) ((int32_t)( ((int64_t)a) * (b) >> (q) ))
#define FPMUL64(a,b,q) ((int64_t)( ((int64_t)a) * (b) >> (q) ))

#define FPDIV(a,b,q)   ((uint32_t)( (((int64_t)a) << (q) ) / (b) ))

// IQMath
#define Q01 		((uint32_t)(1u<<1))
#define Q02 		((uint32_t)(1u<<2))
#define Q03 		((uint32_t)(1u<<3))
#define Q04 		((uint32_t)(1u<<4))
#define Q05 		((uint32_t)(1u<<5))
#define Q06 		((uint32_t)(1u<<6))
#define Q07 		((uint32_t)(1u<<7))
#define Q08 		((uint32_t)(1u<<8))
#define Q09 		((uint32_t)(1u<<9))
#define Q10 		((uint32_t)(1u<<10))
#define Q11 		((uint32_t)(1u<<11))
#define Q12 		((uint32_t)(1u<<12))
#define Q13 		((uint32_t)(1u<<13))
#define Q14 		((uint32_t)(1u<<14))
#define Q15 		((uint32_t)(1u<<15))
#define Q16 		((uint32_t)(1u<<16))
#define Q17 		((uint32_t)(1u<<17))
#define Q18 		((uint32_t)(1u<<18))
#define Q19 		((uint32_t)(1u<<19))
