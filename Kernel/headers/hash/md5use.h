#ifndef _MD5_USE_DEF_H_
#define _MD5_USE_DEF_H_
#include <hash/md5.h>

//Hash some data
void MDData(const char* m_Data, unsigned long len, char* Digest);

//Hash a string
void MDString (const char* Input, char* Digest);

//Compare two digests to see if they are equal (1 if true 0 if false)
unsigned char MDCompare(const char* One, const char* Two);

#endif //_MD5_USE_DEF_H_
