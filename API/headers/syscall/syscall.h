#ifndef _API_SYSTEM_CALL_DEFINITIONS_
#define _API_SYSTEM_CALL_DEFINITIONS_
#include <types/memory.h>

#define DECL_SYSCALL0(fn) MEM_LOC syscall_##fn();
#define DECL_SYSCALL1(fn,p1) MEM_LOC syscall_##fn(p1);
#define DECL_SYSCALL2(fn,p1,p2) MEM_LOC syscall_##fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3) MEM_LOC syscall_##fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4) MEM_LOC syscall_##fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5) MEM_LOC syscall_##fn(p1,p2,p3,p4,p5);

#define DEFN_SYSCALL0(fn, num) \
	MEM_LOC syscall_##fn() \
	{ \
	 MEM_LOC a; \
	 asm volatile("int $127" : "=a" (a) : "0" (num)); \
	 return a; \
	}

#define DEFN_SYSCALL1(fn, num, P1) \
	MEM_LOC syscall_##fn(P1 p1) \
	{ \
	 MEM_LOC a; \
	 asm volatile("int $127" : "=a" (a) : "0" (num), "b" ((int)p1)); \
	 return a; \
	}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
	MEM_LOC syscall_##fn(P1 p1, P2 p2) \
	{ \
	 MEM_LOC a; \
	 asm volatile("int $127" : "=a" (a) : "0" (num), "b" ((int)p1), "c" ((int)p2)); \
	 return a; \
	}


#endif //_SYSTEM_CALL_DEF_H_
