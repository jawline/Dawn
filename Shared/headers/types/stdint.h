#ifndef _INT_TYPES_H_
#define _INT_TYPES_H_

#if _ARCH_ == _ARCH_IA32_

typedef unsigned int   uint32_t;
typedef          int   sint32_t;

typedef unsigned short uint16_t;
typedef          short sint16_t;

typedef unsigned char  uint8_t;
typedef          char  sint8_t;

typedef unsigned long long int uint64_t;
typedef 	 long long int sint64_t;

#elif _ARCH_ == _ARCH_AMD64_
#error "Still designing AMD64 support"
#else
#error "Unsupported archectecture"
#endif

#endif
