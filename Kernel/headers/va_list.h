#ifndef _VA_LIST_DEF_H_
#define _VA_LIST_DEF_H_

//Assume the stack width is the size of a integer.
#define	STACK_WIDTH	sizeof(int)

/* Round up object width so it's an even multiple of STACK_WIDTH.
Using & for division here, so STACK_WIDTH must be a power of 2. */
#define	TYPE_WIDTH(TYPE)				\
	((sizeof(TYPE) + STACK_WIDTH - 1) & ~(STACK_WIDTH - 1))

/* point the va_list pointer to LASTARG,
then advance beyond it to the first variable arg */
#define	va_start(PTR, LASTARG)				\
	PTR = (va_list)((char *)&(LASTARG) + TYPE_WIDTH(LASTARG))

#define va_end(PTR)	/* nothing */

/* Increment the va_list pointer, then return
(evaluate to, actually) the previous value of the pointer.
WHEEE! At last; a valid use for the C comma operator! */
#define va_arg(PTR, TYPE)	(			\
	(char *)(PTR) += TYPE_WIDTH(TYPE)		\
				,			\
	*((TYPE *)((char *)(PTR) - TYPE_WIDTH(TYPE)))	\
				)
/* Every other compiler/libc seems to be using 'void *', so...
(I _was_ using 'unsigned char *') */
typedef void *va_list;

#endif
