#include <stdlib.h>
#include <string.h>

#include "core.h"

struct core *init_core(void)
{
     struct core *c = NULL;

     c = malloc(sizeof *c);
     if (!c)
	  goto err;
     memset(c, 0, sizeof *c);

     c->co_priv = M_PRV;
     init_misa(&c->co_misa);

     return c;
err:
     return NULL;
}
