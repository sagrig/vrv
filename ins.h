#ifndef VRV_INS_H
#define VRV_INS_H

#include <stdint.h>
#include "btree.h"

extern struct btr base;

typedef int8_t (*ins_t)(uint32_t);

struct ins {
     struct btn    i_btn;
     const ins_t   i_hand;
     const uint8_t i_opc;
};

#endif /* VRV_INS_H */
