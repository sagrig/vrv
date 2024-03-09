#ifndef VRV_TB_H
#define VRV_TB_H 1

#include <sys/queue.h>
#include <stdbool.h>

typedef bool (*tbf_t)(void);

struct tb {
     LIST_ENTRY(tb)   tb_list;
     tbf_t            tb_fn;
};

LIST_HEAD(tbh, tb);
extern struct tbh tbh;

inline void tb_reg(struct tb *new)
{
     LIST_INSERT_HEAD(&tbh, new, tb_list);
}

#endif /* VRV_TB_H */
