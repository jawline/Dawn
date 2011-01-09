/* MD5.H - header file for MD5C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.
These notices must be retained in any copies of any part of this
documentation and/or software.
*/

#ifndef _MD5_HASH_ALG_
#define _MD5_HASH_ALG_


#include <types/stdint.h>

/* MD5 context. */
typedef struct {
  uint32_t i[2];                   /* number of _bits_ handled mod 2^64 */
  uint32_t buf[4];                                    /* scratch buffer */
  uint8_t in[64];                              /* input buffer */
  uint8_t digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

void MD5Init (MD5_CTX * Context);
void MD5Update (MD5_CTX *, const unsigned char *, unsigned int);
void MD5Final (MD5_CTX *);

#endif //_MD5_HASH_ALG_
