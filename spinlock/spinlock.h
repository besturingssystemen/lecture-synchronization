#ifndef _SPINLOCK
#define _SPINLOCK

#include <stdatomic.h>

struct mutex
{
    atomic_int locked;
};

void acquire(struct mutex *lock);
void release(struct mutex *lock);

#endif
