#ifndef VRV_CORE_H
#define VRV_CORE_H

#include <stdint.h>

#include "csr.h"

/* Machine XLEN (TODO: 64,128-bit support) */
#define MXLEN  (32)
#define MXL32  (1)

/* Privileges (TODO: U, S, H) */
#define M_PRV (3)

struct core {
     struct csr co_misa;

     uint8_t    co_priv:2;
};

struct core *init_core(void);

#endif /* VRV_CORE_H */
