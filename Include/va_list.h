#ifndef _VA_LIST_DEF_H_
#define _VA_LIST_DEF_H_

//Define a va_list as a void *
typedef void * va_list;

#define __va_rounded_size(TYPE) \
   (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(AP, LASTARG) \
   (AP = ((char *)&(LASTARG) + __va_rounded_size(LASTARG)))

void va_end(va_list);
#define va_end(AP)

#define va_arg(AP, TYPE) \
   (AP += __va_rounded_size(TYPE), \
   *((TYPE *)(AP - __va_rounded_size(TYPE))))

#endif
