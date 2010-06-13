char Data[1024];


#define DECL_SYSCALL0(fn) int syscall_##fn();
#define DECL_SYSCALL1(fn,p1) int syscall_##fn(p1);
#define DECL_SYSCALL2(fn,p1,p2) int syscall_##fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3) int syscall_##fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4) int syscall_##fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5) int syscall_##fn(p1,p2,p3,p4,p5);

#define DEFN_SYSCALL0(fn, num) \
	int syscall_##fn() \
	{ \
	 int a; \
	 asm volatile("int $127" : "=a" (a) : "0" (num)); \
	 return a; \
	}

#define DEFN_SYSCALL1(fn, num, P1) \
	int syscall_##fn(P1 p1) \
	{ \
	 int a; \
	 asm volatile("int $127" : "=a" (a) : "0" (num), "b" ((int)p1)); \
	 return a; \
	}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
	int syscall_##fn(P1 p1, P2 p2) \
	{ \
	 int a; \
	 asm volatile("int $127" : "=a" (a) : "0" (num), "b" ((int)p1), "c" ((int)p2)); \
	 return a; \
	}


DEFN_SYSCALL1(printf, 0, const char*);


int start(int argc, void* argv)
{
	unsigned char val = 0;
	unsigned int i = 0;

	for (i = 0; i < 1024; i++)
	{
		Data[i] = val;
		val++;
	}

	syscall_printf("Hello world! Line.x sending syscalls! :D\n");

	for (;;)
	{
	}

	return 32;
}
