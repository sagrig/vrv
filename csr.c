#include "core.h"
#include "csr.h"

void init_misa(struct csr *misa)
{
     misa->csr_prv  = M_PRV;
     misa->csr_val |= (MXL32 << MISA_MXL);
}
