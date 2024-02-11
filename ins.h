#ifndef VRV_INS_H
#define VRV_INS_H

#include <stdint.h>
#include "btree.h"
#include "core.h"

#define RS2REG_MASK (0x1F00000)
#define RS1REG_MASK (0xF8000)
#define RDREG_MASK  (0xF80)
#define BASEOP_MASK (0x3)

#define IT_MASK_IMM (0xFFF00000)
#define IT_MASK_F3  (0x7000)

#define IT_F3_MAX   (8)

#define GET_REG(c, op, R) ({	     \
     uint8_t r = (op & R##REG_MASK); \
     &c->co_regs[r];		     \
})
#define GET_RS2(c, op) \
     GET_REG(c, op, RS2)
#define GET_RS1(c, op) \
     GET_REG(c, op, RS1)
#define GET_RD(c, op) \
     GET_REG(c, op, RD)

#define ITYPE_X(op, R) \
     (op & IT_MASK_##R)
#define ITYPE_IMM(op) \
     ITYPE_X(op, IMM)
#define ITYPE_F3(op) \
     ITYPE_X(op, F3)

extern struct btr base;

typedef void   (*itp_t)(const uint32_t, const uint16_t, uint32_t *);

typedef int8_t (*ins_t)(struct core *, uint32_t);

struct ins {
     struct btn    i_btn;
     const ins_t   i_pdec;
};

#endif /* VRV_INS_H */
