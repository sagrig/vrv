#ifndef VRV_CSR_H
#define VRV_CSR_H

#include <stdint.h>

#include "core.h"

/* misa CSR bit field*/
#define MISA_MXL (MXLEN - 2)
#define MXL32    (1)

/*
 * CSR register
 * @csr_val: value stored in register
 * @csr_prv: min privilege required to access register
 * TODO:
 * WARL, WPRI, WLRL regions
 *
 */
struct csr {
     uint32_t  csr_val;
     uint8_t   csr_prv:2;
};

void init_misa(struct csr *);

#endif /* VRV_CSR_H */
