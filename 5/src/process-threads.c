#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/syscall.h>

#include "config.h"
#include "red-black-tree.h"

void process_line_mmap(rb_tree *tree, char *line)
{
    node_data *n_data;

    int j, is_word, len_line, i = 0;
    char *paraula, *str;

    len_line = strlen(line);
    paraula = malloc(len_line * sizeof(char));

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i]))))
        i++;

    while (i < len_line)
    {
        j = 0;
        is_word = 1;

        for (;;)
        {
            if (isalpha(line[i]))
                paraula[j] = line[i];
            else
            {
                // The changes are here:
                // If the current character
                // is a "'" (apostrophe) or a "-" (slash).
                // Then we check if the character
                // before is alpha.

                if (line[i] == '\'')
                {
                    if (i > 0 && i + 1 < len_line)
                        if (isalpha(line[i - 1]) && isalpha(line[i + 1]))
                            continue;

                    break;
                }

                if (line[i] == '-')
                {
                    if (i > 0 && i + 1 < len_line)
                    {
                        if (isalpha(line[i - 1]) && isalpha(line[i + 1]))
                            break;
                    }
                    else if (i > 0)
                    {
                        if (isalpha(line[i - 1]))
                            break;
                    }
                }

                is_word = 0;
            }

            j++;
            i++;

            if ((i > len_line) || isspace(line[i]) || ispunct(line[i]))
                break;
        }

        // We update the found word in the
        // tree, incrementing it's num_time value

        if (is_word)
        {
            paraula[j] = 0;

            str = malloc((strlen(paraula) + 1) * sizeof(char));
            strcpy(str, paraula);

            n_data = find_node(tree, str);

            if (n_data != NULL)
            {
                pthread_mutex_lock(&n_data->mutex);
                n_data->num_times++;
                pthread_mutex_unlock(&n_data->mutex);
            }

            free(str);
        }

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i]))))
            i++;
    }

    free(paraula);
}

int process_file_mmap(rb_tree *tree, const char *f_name, pid_t id)
{
    FILE *fp;

    char *line = NULL;
    size_t len = (size_t)MAX_CHARS_LINE;
    int p = 0;

    clock_t start, end;
    double elapsed;

    fp = fopen(f_name, "r");
    if (!fp)
        return 1;

    start = clock();

    while (getline(&line, &len, fp) != -1)
    {
        process_line_mmap(tree, line);
        ++p;
    }

    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    if (DEBUG_TIME)
        fprintf(stderr, "T_%d INFO: Processed '%s' (%d lines) in %3.6lf s.\n", id, f_name, p, elapsed);

    if (line)
        free(line);

    fclose(fp);
    return 0;
}

void *process_list_files_mutex(void *args)
{
    arguments *data = args;
    pid_t thread_id = syscall(__NR_gettid); // gettid();

    for (int i = 0; i < data->num_files; i++)
    {
        if (pthread_mutex_trylock(&data->files[i].mutex) != 0)
            continue;

        if (data->files[i].read == 0)
        {
            if (process_file_mmap(data->tree, data->files[i].name, thread_id) != 0)
            {
                fprintf(stderr, "T_%d WARN: Could not process file %s\n", thread_id, data->files[i].name);
            }
            else 
            {
                data->files[i].read = 1;
            }

            if (DEBUG)
                fprintf(stderr, "INFO: (%d) %s\n", thread_id, data->files[i].name);
        }

        pthread_mutex_unlock(&data->files[i].mutex);
    }

    return (void *) 0;
}