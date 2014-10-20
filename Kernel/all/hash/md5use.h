#ifndef _MD5_USE_DEF_H_
#define _MD5_USE_DEF_H_
#include <hash/md5.h>

/**
 * Hash some data
 */

void MDData(const unsigned char* data, unsigned long len, char* digest);

/**
 * Hash a string
 */

void MDString (const unsigned char* input, char* digest);

/**
 * Compare two digests to see if they are equal
 * returns 1 if they match and 0 otherwise
 */

unsigned char MDCompare(const unsigned char* one, const unsigned char* Two);

#endif //_MD5_USE_DEF_H_
