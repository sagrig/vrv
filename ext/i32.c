#include <stdio.h>

#include "btree.h"
#include "core.h"
#include "i32.h"
#include "ins.h"
#include "help.h"

static void addi(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     *rd = (rs1 + imm);
}

static int8_t opi_pdec(struct core *c, uint32_t op)
{
     uint32_t      *rd  = GET_RD(c, op);
     uint32_t      *rs1 = GET_RS1(c, op);
     uint16_t       imm = ITYPE_IMM(op);
     uint8_t        f3  = ITYPE_F3(op);
     struct btn    *n   = NULL;
     struct opi_ex *o   = NULL;

     n = find_btn(&opi, f3);
     if (!n) {
	  printf("fail to find OP-IMM\n");
	  return -1;
     }
     o = CONT_OF(n, struct opi_ex, opi_btn);
     o->opi_exec(*rs1, imm, rd);

     return 0;
}

struct ins i32i[I32NUM] = {
     {
	  .i_btn.bn_val = 4,
	  .i_pdec       = opi_pdec,
     },
};

struct btr opi = {
     .br_dep = 3,
};

struct opi_ex opie[IT_F3_MAX] = {
     {
	  .opi_btn.bn_val = 0,
	  .opi_exec       = addi,
     },
};

void opi_init(void)
{
     uint8_t i = 0;

     for (; i < IT_F3_MAX; ++i)
	  add_btn(&opi, &opie[i].opi_btn);
}

void i32_init(void)
{
     uint8_t i = 0;

     opi_init();

     for (; i < I32NUM; ++i)
	  add_btn(&base, &i32i[i].i_btn);
}
