#ifndef BRINE_H
#define BRINE_H
#define DEFAULT_ATTRIBUTES __attribute__((visibility("default")))

typedef struct brine_thread brine_thread;

//void brine_write_thread(void *sys_gs, void *teb);
//brine_thread *brine_read_thread();

typedef struct brine_api {
    void (*thread_write)(brine_thread *);
    int (*thread_read)(brine_thread **);
    int (*set_teb_gs)();
    int (*set_sys_gs)();
} brine_api;

int set_sys_gs();
int set_teb_gs();

#endif
