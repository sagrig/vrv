#include <stddef.h>

#include "core.h"

int main(void)
{
     struct core *main = NULL;

     main = init_core();
     if (!main)
	  goto err;

     return 0;
err:
     return -1;
}
