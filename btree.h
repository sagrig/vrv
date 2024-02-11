#ifndef VRV_BTREE_H
#define VRV_BTREE_H

#include <stdint.h>

/**
 * Binary Tree Root:
 * @le:  pointer to left node
 * @ri:  pointer to right node
 * @dep: depth of the tree that mustn't be exceeded
 */
struct btr {
     struct btn    *br_le;
     struct btn    *br_ri;
     const uint8_t  br_dep;
};

/**
 * Binary Tree Node
 * @le:  pointer to left node
 * @ri:  pointer to right node
 * @val: node value used as key
 */
struct btn {
     struct btn    *bn_le;
     struct btn    *bn_ri;
     const uint8_t  bn_val;
};

void        add_btn(struct btr *, struct btn *);
struct btn *find_btn(const struct btr *, const uint8_t);

#endif /* VRV_BTREE_H */
