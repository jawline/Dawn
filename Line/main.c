char Data[1024];

int start(int argc, void* argv)
{
	unsigned char val = 0;
	unsigned int i = 0;

	for (i = 0; i < 1024; i++)
	{
		Data[i] = val;
		val++;
	}

	return 32;
}
