#include <stdio.h>

#include "btree.h"
#include "core.h"
#include "ext.h"
#include "help.h"
#include "i32.h"
#include "ins.h"

#ifdef VRV_DEBUG
static const uint32_t exp_out[8] = {
     1,
     2147483648,
     0,
     1,
     4089,
     0,
     4093,
     4,
};

bool i32_tbfn(void)
{
     struct core *c  = NULL;
     uint32_t     op = 0xf9d08193;
     uint8_t      i  = 0;
     bool         rc = true;

     c = init_core();
     if (!c) {
	  printf("fail to alloc core!\n");
	  return false;
     }

     printf("RV32I test bench.\n");
     printf("OP-IMM instructions:\n");
     printf("Inputs:\n");
     printf("x1(r1): +100\n");
     printf("imm:    -99 (0xf9d)\n\n");

     c->co_regs[1] = 100;
     for (i = 0; i < 8; ++i) {
	  printf("f3: = %u\n", i);
	  printf("expected: x3(rd) = %u\n", exp_out[i]);
	  op &= 0xffff8fff;
	  op |= (i << 12);
	  core_work(c, op);
	  printf("actual:   x3(rd) = %u\n\n", c->co_regs[3]);
	  rc = rc && (exp_out[i] == c->co_regs[3]);
     }
     return rc;
}
#endif /* VRV_DEBUG */

static void andi(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     *rd = (rs1 & imm);
}

static void ori(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     *rd = (rs1 | imm);
}

static void xori(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     *rd = (rs1 ^ imm);
}

static void srai(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     const int32_t srs1 = rs1;
     const uint8_t sh   = (imm & ISH32_MASK);
     *rd                = (int32_t)(srs1 >> sh);
}

static void srli(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     const uint8_t sh = (imm & ISH32_MASK);
     *rd              = (rs1 >> sh);
}

static void srxi(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     if (imm & IOP_MASK)
	  srai(rs1, imm, rd);
     else
	  srli(rs1, imm, rd);
}

static void sltiu(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     *rd = (rs1 < imm);
}

static void slti(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     *rd = ((int32_t)rs1 < (int32_t)imm);
}

static void slli(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     const uint8_t sh = (imm & ISH32_MASK);
     *rd              = (rs1 << sh);
}

static void addi(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     *rd = (imm & 0x800)?
	  (rs1 - ((imm - 1) ^ 0xfff)):
	  (rs1 + imm);
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
     {
	  .opi_btn.bn_val = 1,
	  .opi_exec       = slli,
     },
     {
	  .opi_btn.bn_val = 2,
	  .opi_exec       = slti,
     },
     {
	  .opi_btn.bn_val = 3,
	  .opi_exec       = sltiu,
     },
     {
	  .opi_btn.bn_val = 4,
	  .opi_exec       = xori,
     },
     {
	  .opi_btn.bn_val = 5,
	  .opi_exec       = srxi,
     },
     {
	  .opi_btn.bn_val = 6,
	  .opi_exec       = ori,
     },
     {
	  .opi_btn.bn_val = 7,
	  .opi_exec       = andi,
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

     i32_tb_reg();
}
ext_init(i32_init);
