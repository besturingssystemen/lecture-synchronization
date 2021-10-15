#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ITERATIONS 10000

int shared_global_counter = 0;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Please provide the number of processes as an argument\n");
        exit(-1);
    }

    int process_count = atoi(argv[1]);

    // Fork PROCESS_COUNT processes
    int pid, i;
    for (i = 0; i < process_count - 1; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            break;
        }
    }

    for (int i = 0; i < ITERATIONS; i++)
    {
        shared_global_counter++;
    }

    printf("[%d] The value of our counter: %d\n", i, shared_global_counter);

    // Parent waits for all processes to exit (pid == 0 in all child processes)
    if (pid != 0)
    {
        for (int i = 0; i < process_count - 1; i++)
        {
            wait(NULL);
        }
    }

    return 0;
}