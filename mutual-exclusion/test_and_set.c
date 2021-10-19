#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#define ITERATIONS 10000

atomic_flag in_critical_section;

int shared_global_counter = 0;

void *increment(void *param)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        while (atomic_flag_test_and_set(&in_critical_section) == 1)
        {
            // Busy waiting
        }
        shared_global_counter++;
        atomic_flag_clear(&in_critical_section);
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