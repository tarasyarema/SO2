#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "red-black-tree.h"

// Debug print a string

void debug_str(char *p)
{
    fprintf(stderr, "%p: (%ld) %s\n", p, strlen(p), p);
}

/**
 * 
 * Given a line, extract the words it contains and print them to screen
 * 
 */

void process_line(rb_tree *tree, char *line)
{
    int j, is_word, len_line, i = 0;
    char *paraula, *str;

    node_data *n_data;

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
                n_data->num_times++;

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

int process_file(rb_tree *tree, const char *f_name)
{
    FILE *fp;

    char *line = NULL;
    size_t len = (size_t)MAX_CHARS_LINE;

    clock_t start, end;
    double elapsed;

    fp = fopen(f_name, "r");
    if (!fp)
        return 1;

    start = clock();

    while (getline(&line, &len, fp) != -1)
    {
        process_line(tree, line);
    }

    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    if (DEBUG_TIME)
        fprintf(stderr, "Processed '%s' in %3.6lf s.\n", f_name, elapsed);

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

int process_list_files(rb_tree *tree, const char *fl_name, const char *path)
{
    FILE *fp;

    char file_name[MAX_CHARS];
    char *line = NULL;
    size_t len = (size_t)MAX_CHARS;

    clock_t start, end;
    double elapsed;

    unsigned long processed = 0, error = 0;

    fp = fopen(fl_name, "r");
    if (!fp)
        return 1;

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

        if (process_file(tree, file_name) != 0)
        {
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
        fprintf(stderr, "Processed %lu files in %3.6lf s.\n", processed, elapsed);

    if (DEBUG)
        fprintf(stderr, "Encountered %lu errors (could not process a file).\n", error);

    if (line)
        free(line);

    fclose(fp);
    return 0;
}