#include <stdatomic.h>
#include <stdio.h>
#include "spinlock.h"

void acquire(struct mutex *lock)
{
    while (atomic_exchange(&(lock->locked), 1))
    {
        // Busy waiting
    }
}

void release(struct mutex *lock)
{
    lock->locked = 0;
}
