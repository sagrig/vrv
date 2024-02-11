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

     if (*op & BASEOP_MASK) {
	  node  = find_btn(&base, *op);
	  if (!node) {
	       printf("node find fail\n");
	       return NULL;
	  }
	  insn  = CONT_OF(node, struct ins, i_btn);
	  *op >>= 2;
     } else
	  printf("Not implemented\n");
     return insn;
}

static inline void pipe_pdec(struct ins *i, struct core *c, const uint32_t op)
{
     i->i_pdec(c, op);
}

void core_work(struct core *c)
{
     struct ins *insn = NULL;
     uint32_t op = 0;

     while (1) {
	  /* TODO: fetch (need a fetchable mem region) */
	  insn = pipe_idec(&op);
	  if (!insn) {
	       printf("idec fail\n");
	       continue;
	  }
	  pipe_pdec(insn, c, op);
     }
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
