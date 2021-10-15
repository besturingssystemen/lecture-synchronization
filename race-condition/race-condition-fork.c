#include <stdio.h>
#include <unistd.h>

#define ITERATIONS 10000

int shared_global_counter = 0;

int main()
{
    int pid = fork();

    if (pid == 0)
    {
        for (int i = 0; i < ITERATIONS; i++)
        {
            shared_global_counter++;
        }
    }
    else
    {
        for (int i = 0; i < ITERATIONS; i++)
        {
            shared_global_counter--;
        }
    }

    printf("The value of our counter: %d\n", shared_global_counter);
    return 0;
}