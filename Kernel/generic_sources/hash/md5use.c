#include <hash/md5use.h>

void MDData(const char* m_Data, unsigned long len, char* Digest)
{
  MD5_CTX mdContext;
  memset(&mdContext, 0, sizeof(MD5_CTX));

  MD5Init (&mdContext);
  MD5Update (&mdContext, m_Data, len);
  MD5Final (&mdContext);

  unsigned int iter = 0;
  for (iter = 0; iter < 16; iter++)
	Digest[iter] = mdContext.digest[iter];

  return;
}

void MDString (const char* Input, char* Digest)
{
  MDData(Input, strlen(Input), Digest);
  return;
}

unsigned char MDCompare(const char* One, const char* Two)
{

	unsigned int iter = 0;
	for (iter = 0; iter < 16; iter++)
	{
		if (One[iter] != Two[iter]) return 0;
	}

	return 1;
}
