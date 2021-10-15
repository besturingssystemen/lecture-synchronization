#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define ITERATIONS 100000
#define THREAD_COUNT 2

int shared_global_counter = 0;

bool flag[THREAD_COUNT] = {false, false};
int turn;

void *increment(void *p_thread_index)
{
    int my_idx = *((int *)p_thread_index);
    int other_idx;

    if (my_idx == 0)
    {
        other_idx = 1;
    }
    else
    {
        other_idx = 0;
    }

    // We have 2 threads with idx 0 and 1.
    // In thread 0, my_idx == 0 and other_idx == 1
    // In thread 1, my_idx == 1 and other_idx == 0

    for (int i = 0; i < ITERATIONS; i++)
    {

        flag[my_idx] = true;
        asm volatile("mfence"
                     :
                     :
                     : "memory");
        turn = other_idx;

        while (flag[other_idx] == true && turn == other_idx)
        {
            // WAIT
        }
        // START CRITICAL SECTION
        shared_global_counter++;
        // END CRITICAL SECTION

        flag[my_idx] = false;
    }
}

int main()
{

    pthread_t thread_ids[THREAD_COUNT];
    int thread_idx[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        thread_idx[i] = i;
        pthread_create(&thread_ids[i], NULL, increment, thread_idx + i);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(thread_ids[i], NULL);
    }

    printf("The value of our counter: %d\n", shared_global_counter);

    return 0;
}