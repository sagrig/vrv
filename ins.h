#ifndef VRV_INS_H
#define VRV_INS_H

#include <stdint.h>
#include "btree.h"
#include "core.h"

/* Macros for masking instruction bit-field regions */
/* ******************************** */
/* Mask values are determined considering base-op shifting (7) */

/* Constant-region masks */
#define RS2REG_MASK (0x3E000)
#define RS1REG_MASK (0x1F00)
#define RDREG_MASK  (0x1F)

/* I-Type Masks */
#define IT_MASK_IMM (0x1FFE000)
#define IT_MASK_F3  (0xE0)

/* This mask is applied after un-shifting base bits (2) */
#define BASEOP_MASK (0x1F)

/* This mask is applied on un-shifted ins */
#define ISBASE_MASK (0x3)
/* ******************************** */

/* Shift constants to acquire instruction bit-field region value */
/* ******************************** */
/* Shift values are determined considering base-op shifting (7) */

/* Constant-region shift */
#define RS2REG_SH   (13)
#define RS1REG_SH   (8)
#define RDREG_SH    (0)

/* I-Type Shifts */
#define IT_SH_IMM   (13)
#define IT_SH_F3    (5)

/* ******************************** */
#define IT_F3_MAX   (8)

#define GET_REG(c, op, R) ({					\
	       uint8_t r = ((op & R##REG_MASK) >> R##REG_SH);	\
	       &c->co_regs[r];					\
	  })
#define GET_RS2(c, op) \
     GET_REG(c, op, RS2)
#define GET_RS1(c, op) \
     GET_REG(c, op, RS1)
#define GET_RD(c, op) \
     GET_REG(c, op, RD)

#define ITYPE_X(op, R) \
     ((op & IT_MASK_##R) >> IT_SH_##R)
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
