#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "ins.h"
#include "help.h"

static struct ins *pipe_idec(uint32_t *op)
{
     struct ins *insn = NULL;
     struct btn *node = NULL;

     if (*op & ISBASE_MASK) {
	  *op >>= 2;
	  node  = find_btn(&base, (*op & BASEOP_MASK));
	  if (!node) {
	       printf("node find fail\n");
	       return NULL;
	  }
	  insn  = CONT_OF(node, struct ins, i_btn);
	  *op >>= 5;
     } else
	  printf("Not implemented\n");
     return insn;
}

/**
 * Calls parameter decoding function that
 * must call the respective exec function.
 */
static inline void pipe_pdec_exec(struct ins *i, struct core *c, const uint32_t op)
{
     i->i_pdec(c, op);
}

void core_work(struct core *c, uint32_t op)
{
     struct ins *insn = NULL;

     /* TODO: while (true): read & pass instruction via pipeline */
     /* TODO: fetch (need a fetchable mem region) */
     insn = pipe_idec(&op);
     if (!insn) {
	  printf("idec fail\n");
	  /* add BUG()? */
	  return;
     }
     pipe_pdec_exec(insn, c, op);
}

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
