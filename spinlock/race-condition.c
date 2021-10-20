#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "spinlock.h"

#define ITERATIONS 10000

int shared_global_counter = 0;
struct mutex counter_lock;

void *increment(void *param)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        acquire(&counter_lock);
        shared_global_counter++;
        release(&counter_lock);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Please provide the number of threads as an argument\n");
        exit(-1);
    }

    int thread_count = atoi(argv[1]);

    pthread_t thread_ids[thread_count];

    for (int i = 0; i < thread_count; i++)
    {
        pthread_create(&thread_ids[i], NULL, increment, NULL);
    }

    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(thread_ids[i], NULL);
    }

    printf("The value of our counter: %d\n", shared_global_counter);

    return 0;
}