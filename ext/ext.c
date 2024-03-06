#include <stdio.h>

#include "ext.h"

void init_exts(void)
{
     ext_t **fn = &__start_ext;

     for (; fn < &__stop_ext; ++fn)
	  ((ext_t)*fn)();
}
