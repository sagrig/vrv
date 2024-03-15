#ifndef VRV_INS_H
#define VRV_INS_H

#include <stdint.h>
#include "btree.h"
#include "core.h"

/* Bit-field constants */
/* ******************************** */

/* General const */
#define REGSIZE     (5)
#define OPSIZE      (7)
#define BASEOP_MASK (0x1F)
#define ISBASE_MASK (0x3)  /* TODO: move to C extension header */
#define REG_MASK    (0x1F)

/* I-Type const */
#define ITIM_SIZE   (12)
#define ITF3_SIZE   (3)
#define ITF3_MAX    (8)
#define ITIM_MASK   (0xFFF)
#define ITIS_MASK   (0x800)
#define ITF3_MASK   (0x7)
#define ITNG_TO32   (0xFFFFF000)

/* U-Type const */
#define UTIM_SIZE   (20)
#define UTIM_MASK   (0xFFFFF)
#define UTIS_MASK   (0x80000)
#define UT3021_MASK (0x7FE00)
#define UT20_MASK   (0x80)
#define UT1912_MASK (0xFF)

/* J-Type const */
#define JTIS_MASK   (0x100000)
#define JT11_MASK   (0x800)
#define JT1912_OFFS (12)
#define JT1001_OFFS (8)
#define JTNG_TO32   (0xFFF00000)

/* ******************************** */
/* Bit-field macros */

/* General macro */
#define PULL_REG(c, op) ({				\
	       uint8_t r    = ((op) & REG_MASK);	\
	       op         >>= REGSIZE;			\
	       &(c)->co_regs[r];			\
	  })
#define PULL_RS2(c, op)				\
     PULL_REG(c, op)
#define PULL_RS1(c, op)				\
     PULL_REG(c, op)
#define PULL_RD(c, op)				\
     PULL_REG(c, op)

/* I-Type macro */
#define PULL_ITIM(op) ({				\
	       uint16_t im   = ((op) & ITIM_MASK);	\
	       (op)        >>= ITIM_SIZE;		\
	       im;					\
	  })
#define PULL_ITF3(op) ({				\
	       uint8_t f3    = ((op) & ITF3_MASK);	\
	       (op)        >>= ITF3_SIZE;		\
	       f3;					\
	  })

/* U-Type macro */
#define PULL_UTIM(op) ({				\
	       uint32_t im   = ((op) & UTIM_MASK);	\
	       (op)        >>= UTIM_SIZE;		\
	       im;					\
	  })
/* ******************************** */

extern struct btr base;

typedef void   (*itp_t)(const uint32_t, const uint16_t, uint32_t *);
typedef int8_t (*ins_t)(struct core *, uint32_t);

struct ins {
     struct btn    i_btn;
     const ins_t   i_pdec;
};

#endif /* VRV_INS_H */
