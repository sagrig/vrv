#ifndef HELP_VRV_H
#define HELP_VRV_H

#include <stddef.h>

#define CONT_OF(ptr, type, memb) ({			\
	       char *mptr = (char *)(ptr);		\
	       ((type *)(mptr - offsetof(type, memb))); \
	  })

#endif /* HELP_VRV_H */
