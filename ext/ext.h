#ifndef VRV_EXT_H
#define VRV_EXT_H

/* Section names for extension initialisation */
#define __ext    __attribute__((__section__("ext")))

/* Prevent errors of unused static init functions */
#define __used   __attribute__((__used__))

typedef void (*ext_t)(void);

extern ext_t *__start_ext;
extern ext_t *__stop_ext;

#define ext_init(fn)				\
     static ext_t __ext_init_##fn __used __ext = fn

void init_exts(void);

#endif /* VRV_EXT_H */
