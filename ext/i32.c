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
     printf("inputs:\n");
     printf("x1(r1) = +100\n");
     printf("imm    = -99 (0xf9d)\n\n");

     c->co_regs[1] = 100;
     for (i = 0; i < 8; ++i) {
	  printf("f3 = %u\n", i);
	  printf("expected:\n");
	  printf("x3(rd) = %u\n", exp_out[i]);
	  op &= 0xffff8fff;
	  op |= (i << 12);
	  core_work(c, op);
	  printf("actual:\n");
	  printf("x3(rd) = %u\n\n", c->co_regs[3]);
	  rc = rc && (exp_out[i] == c->co_regs[3]);
     }

     printf("LUI instruction:\n");
     printf("inputs:\n");
     printf("imm     = 0xf0f0f\n");
     printf("expected:\n");
     printf("x31(rd) = 0xf0f0f000\n");
     op = 0xf0f0ffb7;
     core_work(c, op);
     printf("actual:\n");
     printf("x31(rd) = 0x%08x\n\n", c->co_regs[31]);
     rc = rc && (0xf0f0f000 == c->co_regs[31]);

     printf("AUIPC instruction:\n");
     printf("inputs:\n");
     printf("pc      = 0x88888888\n");
     printf("imm     = 0xff\n");
     printf("expected:\n");
     printf("x17(rd) = 0x88987888\n");
     c->co_pc = 0x88888888;
     op       = 0xff897;
     core_work(c, op);
     printf("actual:\n");
     printf("x17(rd) = 0x%08x\n\n", c->co_regs[17]);
     rc = rc && (0x88987888 == c->co_regs[17]);

     printf("JALR instruction:\n");
     printf("inputs:\n");
     printf("pc      = 0x7f\n");
     printf("imm     = 0x800\n");
     printf("x1(rs1) = 0xff000\n");
     printf("expected:\n");
     printf("x27(rd) = 0x83\n");
     printf("pc      = 0xfe800\n");
     c->co_regs[1] = 0xff000;
     c->co_pc      = 0x7f;
     op            = 0x80008de7;
     core_work(c, op);
     printf("actual:\n");
     printf("x27(rd) = 0x%2x\n", c->co_regs[27]);
     printf("pc      = 0x%5x\n\n", c->co_pc);
     rc = rc && (0x83    == c->co_regs[27]);
     rc = rc && (0xfe800 == c->co_pc);

     printf("JAL instruction:\n");
     printf("inputs:\n");
     printf("pc      = 0xfadecafe\n");
     printf("imm     = 0xfff06000\n");
     printf("expected:\n");
     printf("x25(rd) = 0xfadecb02\n");
     printf("pc      = 0xfacf2afe\n");
     c->co_pc      = 0xfadecafe;
     op            = 0x80006cef;
     core_work(c, op);
     printf("actual:\n");
     printf("x25(rd) = 0x%8x\n", c->co_regs[25]);
     printf("pc      = 0x%8x\n", c->co_pc);
     rc = rc && (0xfadecb02 == c->co_regs[25]);
     rc = rc && (0xfacf2afe == c->co_pc);

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
     uint8_t rs1s = (rs1 >> (sizeof rs1 - 1));

     if (imm & ITIS_MASK) {
	  int32_t im32;
	  im32  = imm;
	  im32 |= ITNG_TO32;
	  *rd   = (!rs1s)?(0):((int32_t)rs1 < im32);
     }
     else {
	  *rd   = (!rs1s)?(rs1 < imm):(1);
     }
}

static void slli(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     const uint8_t sh = (imm & ISH32_MASK);
     *rd              = (rs1 << sh);
}

static void addi(const uint32_t rs1, const uint16_t imm, uint32_t *rd)
{
     *rd = (imm & ITIS_MASK)?
	  (rs1 + ((int32_t)imm | ITNG_TO32)):
	  (rs1 + imm);
}

static int8_t opi_pdec(struct core *c, uint32_t op)
{
     uint32_t      *rd  = NULL;
     uint32_t      *rs1 = NULL;
     uint16_t       imm = 0;
     uint8_t        f3  = 0;
     struct btn    *n   = NULL;
     struct opi_ex *o   = NULL;

     rd  = PULL_RD(c, op);
     f3  = PULL_ITF3(op);
     rs1 = PULL_RS1(c, op);
     imm = PULL_ITIM(op);

     n   = find_btn(&opi, f3);
     if (!n) {
	  printf("fail to find OP-IMM\n");
	  return -1;
     }
     o   = CONT_OF(n, struct opi_ex, opi_btn);
     o->opi_exec(*rs1, imm, rd);

     return 0;
}

static int8_t lui_pdec(struct core *c, uint32_t op)
{
     uint32_t *rd  = NULL;
     uint32_t  imm = 0;

     rd   = PULL_RD(c, op);
     imm  = PULL_UTIM(op);

     *rd ^= *rd;
     *rd |= (imm << (REGSIZE + OPSIZE));

     return 0;
}

static int8_t auipc_pdec(struct core *c, uint32_t op)
{
     uint32_t *rd  = NULL;
     uint32_t  imm = 0;

     rd   = PULL_RD(c, op);
     imm  = PULL_UTIM(op);

     *rd ^= *rd;
     *rd |= (imm << (REGSIZE + OPSIZE));
     *rd += c->co_pc;

     return 0;
}

static int8_t jalr_pdec(struct core *c, uint32_t op)
{
     uint32_t *rd  = NULL;
     uint32_t *rs1 = NULL;
     uint16_t  imm = 0;
     uint8_t    f3 = 0;

     rd  = PULL_RD(c, op);
     f3  = PULL_ITF3(op);
     rs1 = PULL_RS1(c, op);
     imm = PULL_ITIM(op);

     if (f3)
	  return -1;

     *rd      += (c->co_pc + 4);
     c->co_pc  = (imm & ITIS_MASK)?
	  (*rs1 + ((int32_t)imm | ITNG_TO32)):
	  (*rs1 + imm);
     c->co_pc &= ~1;

     return 0;
}

static int8_t jal_pdec(struct core *c, uint32_t op)
{
     uint32_t *rd  = NULL;
     uint32_t  uim = 0;
     uint32_t  jim = 0;

     rd  = PULL_RD(c, op);
     uim = PULL_UTIM(op);

     if (uim & UTIS_MASK)
	  jim |= JTNG_TO32;

     if (uim & UT20_MASK)
	  jim |= JT11_MASK;

     jim      |= ((uim & UT1912_MASK) << JT1912_OFFS);
     jim      |= ((uim & UT3021_MASK) >> JT1001_OFFS);

     *rd       = (c->co_pc + 4);
     c->co_pc += jim;

     return 0;
}

struct ins i32i[I32NUM] = {
     {
	  .i_btn.bn_val = 4,
	  .i_pdec       = opi_pdec,
     },
     {
	  .i_btn.bn_val = 5,
	  .i_pdec       = auipc_pdec,
     },
     {
	  .i_btn.bn_val = 13,
	  .i_pdec       = lui_pdec,
     },
     {
	  .i_btn.bn_val = 25,
	  .i_pdec       = jalr_pdec,
     },
     {
	  .i_btn.bn_val = 27,
	  .i_pdec       = jal_pdec,
     },
};

struct btr opi = {
     .br_dep = 3,
};

struct opi_ex opie[ITF3_MAX] = {
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

     for (; i < ITF3_MAX; ++i)
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
