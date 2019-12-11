#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "red-black-tree.h"

/**
 * 
 * Given a line, extract the words it contains and print them to screen
 * 
 */

void process_line_mmap(rb_tree *tree, char *line, int id)
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
                    if (i > 0 && i+1 < len_line)
                        if (isalpha(line[i-1]) && isalpha(line[i+1]))
                            continue;
                    
                    break;
                }

                if (line[i] == '-')
                {
                    if (i > 0 && i+1 < len_line)
                    {
                        if (isalpha(line[i-1]) && isalpha(line[i+1]))
                            break;
                    }
                    else if (i > 0)
                    {
                        if (isalpha(line[i-1]))
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
                sem_wait(&n_data->sem);
                n_data->num_times++;
                sem_post(&n_data->sem);
            }

            free(str);
        }

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i]))))
            i++;
    }

    free(paraula);
}

/**
 * 
 * Given a file, process every line to extract 
 * the words it contains and print them to screen
 * 
 */

int process_file_mmap(rb_tree *tree, const char *f_name, int id)
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
        process_line_mmap(tree, line, id);
        ++p;
    }

    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    if (DEBUG_TIME)
        fprintf(stderr, "C_%d INFO: Processed '%s' (%d lines) in %3.6lf s.\n", id, f_name, p, elapsed);

    if (line)
        free(line);

    fclose(fp);
    return 0;
}

/**
 * 
 * Given a file, process every line of it
 * containing a file and process.
 * 
 */

int process_list_files_mmap(char *ptr, const char *fl_name, const char *path)
{
    FILE *fp;
    rb_tree *tree;

    char file_name[MAX_CHARS];
    char *line = NULL;
    size_t len = (size_t)MAX_CHARS;

    clock_t start, end;
    double elapsed;

    unsigned long processed = 0, error = 0;

    fp = fopen(fl_name, "r");
    if (!fp)
        return 1;

    tree = (rb_tree *)malloc(sizeof(rb_tree));
    init_tree(tree);
    init_tree_mmap(tree, ptr);

    start = clock();

    while (getline(&line, &len, fp) != -1)
    {
        strtok(line, "\n");

        // Get rid of the first line as we
        // don't actually need the number fo lines

        if (atoi(line))
            continue;

        // Generate the correct file name with it's path
        // and append it to an empty string

        file_name[0] = '\0';

        strcat(file_name, path);
        strcat(file_name, line);

        // If we can't process a file, we continue.
        // When the list is large, we do not want to stop
        // the process if we encounter few wrong filenames.

        if (process_file_mmap(tree, file_name, 1) != 0)
        {
            if (DEBUG)
                fprintf(stderr, "WARN: Could not process file %s\n", file_name);
            
            ++error;
            file_name[0] = '\0';
            continue;
        }

        ++processed;
        file_name[0] = '\0';
    }

    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    if (DEBUG_TIME)
        fprintf(stderr, "INFO: Processed %lu files in %3.6lf s.\n", processed, elapsed);

    if (DEBUG)
        fprintf(stderr, "INFO: Encountered %lu errors (could not process a file).\n", error);

    if (line)
        free(line);

    fclose(fp);
    return 0;
}

void process_list_files_sem(char *db_ptr, char *tree_ptr, int id, int offset)
{
    rb_tree *tree;

    char *ptr;
    int i_file = offset, sem_value;
    s_file_name *file;

    tree = (rb_tree *)malloc(sizeof(rb_tree));
    init_tree(tree);
    init_tree_mmap(tree, tree_ptr);

    while ((ptr = _get_dbfname_from_mmap(db_ptr, i_file)) != NULL)
    {
        file = (s_file_name *)ptr;

        sem_getvalue(&file->sem, &sem_value);

        if (sem_value == 0)
        {
            ++i_file;
            continue;
        }

        sem_wait(&file->sem);

        if (file->done != 1)
        {
            if (process_file_mmap(tree, file->key, id) != 0)
            {
                fprintf(stderr, "C_%d WARN: Could not process file %s\n", id, file->key);
            }
            else
            {
                file->done = 1;
                if (DEBUG)
                    fprintf(stderr, "C_%d INFO: File %s marked as done (%d)\n", id, file->key, file->done);
            }
        }

        sem_post(&file->sem);

        ++i_file;
    }
}