#include <hash/md5use.h>
#include <debug/debug.h>

void MDData(const unsigned char* m_data, unsigned long len, char* digest) {
  MD5_CTX mdContext;
  memset(&mdContext, 0, sizeof(MD5_CTX));

  MD5Init (&mdContext);
  MD5Update (&mdContext, m_data, len);
  MD5Final (&mdContext);

  unsigned int iter = 0;

  for (iter = 0; iter < 16; iter++) {
	digest[iter] = mdContext.digest[iter];
  }

  return;
}

void MDString (const unsigned char* input, char* digest) {
  MDData(input, strlen(input), digest);
  return;
}

unsigned char MDCompare(const unsigned char* one, const unsigned char* two) {

	unsigned int iter = 0;

	for (iter = 0; iter < 16; iter++) {
	    DEBUG_PRINT("The two that don't match %i %x %x\n", iter, one[iter], two[iter]);
	    if (one[iter] != two[iter]) {
                return 0;
	    }
	}

	return 1;
}
