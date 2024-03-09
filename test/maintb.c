#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ext.h"
#include "tb.h"

struct tbh tbh = {0};

static bool run_tbs(void)
{
     struct tb *t  = NULL;
     bool       rc = true;

     LIST_FOREACH(t, &tbh, tb_list) {
	  rc = rc && t->tb_fn();
     }
     return rc;
}

int main(void)
{
     bool rc = true;

     LIST_INIT(&tbh);
     init_exts();

     rc = run_tbs();
     if (rc) {
	  printf("Test bench is successful.\n");
	  exit(EXIT_SUCCESS);
     } else {
	  printf("Test bench failed.\n");
	  exit(EXIT_FAILURE);
     }
}
