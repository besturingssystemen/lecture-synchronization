#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ITERATIONS 100
#define FILENAME "tmpfile"

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

    // Fork PROCESS_COUNT processes
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
        // Increment the count value stored in FILENAME
        increment_file_value();
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
        printf("End value: %d\n", value);
    }

    return 0;
}