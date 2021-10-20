#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include "spinlock.h"

#define ITERATIONS 100
#define FILENAME "tmpfile"

// From: https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c

void *create_shared_memory(size_t size)
{
    // Our memory buffer will be readable and writable:
    int protection = PROT_READ | PROT_WRITE;

    // The buffer will be shared (meaning other processes can access it), but
    // anonymous (meaning third-party processes cannot obtain an address for it),
    // so only this process and its children will be able to use it:
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    // The remaining parameters to `mmap()` are not important for this use case,
    // but the manpage for `mmap` explains their purpose.
    return mmap(NULL, size, protection, visibility, -1, 0);
}

struct mutex *create_shared_lock()
{
    struct mutex *mutex = create_shared_memory(sizeof(struct mutex));
    mutex->locked = 0;
    return mutex;
}

int read_int_from_file(int *out)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file)
    {
        fprintf(stderr, "Couldn't open tmpfile");
        return -1;
    };

    int read_val;
    if (fscanf(file, "%d", out) != 1)
    {
        return -1;
    };
    fclose(file);
    return 0;
}

int write_int_to_file(int number)
{
    FILE *file = fopen(FILENAME, "w+");
    if (!file)
    {
        fprintf(stderr, "Couldn't open tmpfile");
        return -1;
    };

    fprintf(file, "%d", number);
    fclose(file);
    return 0;
}

int increment_file_value()
{
    int value;
    if (read_int_from_file(&value) == 0)
    {
        write_int_to_file(value + 1);
        return 0;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Please provide the number of processes as an argument\n");
        exit(-1);
    }

    int process_count = atoi(argv[1]);

    int i, pid = -1;

    // Create file with value 0 written to it
    FILE *file = fopen(FILENAME, "w");
    fprintf(file, "0");
    fclose(file);

    struct mutex *lock = create_shared_lock();

    // Fork PROCESS_COUNT processes
    for (i = 0; i < process_count - 1; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            break;
        }
    }

    for (int j = 0; j < ITERATIONS; j++)
    {
        acquire(lock);
        // Increment the count value stored in FILENAME
        increment_file_value();
        release(lock);
    }

    printf("[%d] Done\n", i);

    // Parent waits for all processes to exit (pid == 0 in all child processes)
    if (pid != 0)
    {
        for (int i = 0; i < process_count - 1; i++)
        {
            wait(NULL);
        }
        int value;
        read_int_from_file(&value);
        printf("The value of our counter: %d\n", value);
    }

    return 0;
}