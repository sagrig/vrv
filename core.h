#ifndef VRV_CORE_H
#define VRV_CORE_H

#include <stdint.h>

#include "csr.h"

/* Number of General-Purpose Registers */
#define GREGNUM (1 << GREGBIT)
#define GREGBIT (5)

/* Machine XLEN (TODO: 64,128-bit support) */
#define MXLEN   (32)

/* Privileges (TODO: U, S, H) */
#define M_PRV   (3)

struct core {
     struct csr  co_misa;

     uint32_t    co_regs[GREGNUM];
     uint32_t    co_pc;
     uint8_t     co_priv:2;
};

struct core *init_core(void);

#endif /* VRV_CORE_H */
