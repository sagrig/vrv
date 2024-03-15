#ifndef VRV_I32_H
#define VRV_I32_H

#include "ins.h"

#define I32NUM (5)

#define ISH32_MASK (0x1F)
#define IOP_MASK   (0x400)

struct opi_ex {
     struct btn   opi_btn;
     const itp_t  opi_exec;
};

#ifdef VRV_DEBUG
#include "tb.h"

bool i32_tbfn(void);

struct tb i32_tb = {
     .tb_fn = i32_tbfn,
};

inline void i32_tb_reg(void)
{
     tb_reg(&i32_tb);
}
#else
inline void i32_tb_reg(void)
{
}
#endif /* VRV_DEBUG */

extern struct btr    opi;
extern struct ins    i32i[I32NUM];
extern struct opi_ex opie[ITF3_MAX];

#endif /* VRV_I32_H */
