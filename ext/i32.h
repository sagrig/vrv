#ifndef VRV_I32_H
#define VRV_I32_H

#include "ins.h"

#define I32NUM 1

struct opi_ex {
     struct btn   opi_btn;
     const itp_t  opi_exec;
};

extern struct btr    opi;
extern struct ins    i32i[I32NUM];
extern struct opi_ex opie[IT_F3_MAX];

#endif /* VRV_I32_H */
