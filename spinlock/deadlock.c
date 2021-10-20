
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "spinlock.h"

struct mutex lock_1;
struct mutex lock_2;

void *thread_a_func(void *param)
{
    acquire(&lock_1);
    printf("[A] Got lock 1\n");

    sleep(1);
    acquire(&lock_2);
    printf("[A] Got lock 2\n");
    release(&lock_1);
    release(&lock_2);
}

void *thread_b_func(void *param)
{
    acquire(&lock_2);
    printf("[B] Got lock 2\n");

    sleep(1);
    acquire(&lock_1);
    printf("[B] Got lock 1\n");
    release(&lock_1);
    release(&lock_2);
}

int main(int argc, char *argv[])
{
    pthread_t thread_a;
    pthread_t thread_b;

    pthread_create(&thread_a, NULL, thread_a_func, NULL);
    pthread_create(&thread_b, NULL, thread_b_func, NULL);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    printf("We will never get here\n");
    return 0;
}