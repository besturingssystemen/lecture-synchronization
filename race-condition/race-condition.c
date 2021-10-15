#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 10000

int shared_global_counter = 0;

void *increment(void *param)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        shared_global_counter++;
    }
}

void *decrement(void *param)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        shared_global_counter--;
    }
}

int main()
{
    pthread_t increment_thread;
    pthread_t decrement_thread;

    pthread_create(&increment_thread, NULL, increment, NULL);
    pthread_create(&decrement_thread, NULL, decrement, NULL);

    pthread_join(increment_thread, NULL);
    pthread_join(decrement_thread, NULL);

    printf("The value of our counter: %d\n", shared_global_counter);

    return 0;
}