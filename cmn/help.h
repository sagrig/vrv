#ifndef VRV_HELP_H
#define VRV_HELP_H

#include <stddef.h>

#define CONT_OF(ptr, type, memb) ({			\
	       char *mptr = (char *)(ptr);		\
	       ((type *)(mptr - offsetof(type, memb))); \
	  })

#endif /* VRV_HELP_H */
