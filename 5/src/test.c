// General user libraries
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include <pthread.h>

// Config variables
#include "config.h"

// Red-Black tree structures and functions
#include "red-black-tree.h"

// Utility functions and
// file processers
#include "utils.c"
#include "process-threads.c"
#include "read.c"

pthread_t ntid[MAX_THREADS];

int main(int argc, char **argv)
{
    FILE *db_file;
    file *files = NULL;
    char *dict_filename = "data/w", *db_filename = "data/l";
    char line[MAX_CHARS];

    rb_tree *tree = NULL;
    node_data *node = NULL;

    int num_files, threads = MAX_THREADS;

    if (argc > 1)
        threads = atoi(argv[1]);

    if (argc > 2)
        db_filename = argv[2];

    tree = (rb_tree *)malloc(sizeof(rb_tree));
    init_tree(tree);

    if (init_tree_from_file(tree, dict_filename) != 0)
        return 1;

    db_file = fopen(db_filename, "r");
    if (db_file == NULL)
        return 1;

    fgets(line, MAX_CHARS, db_file);

    num_files = atoi(line);

    if (num_files <= 0)
        exit(1);

    files = (file *)malloc(num_files * sizeof(file));

    /* Read database file names */

    for (int i = 0; i < num_files; i++)
    {
        /* Read file name */

        fgets(line, MAX_CHARS, db_file);
        line[strlen(line) - 1] = 0;

        strcpy(files[i].name, line);
        files[i].mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
        files[i].read = 0;
    }

    fclose(db_file);

    arguments *args = malloc(sizeof(*args));

    args->tree = tree;
    args->files = files;
    args->num_files = num_files;

    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (pthread_create(&ntid[i], NULL, process_list_files_mutex, args) != 0)
        {
            free(args);
            exit(1);
        }
    }

    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (pthread_join(ntid[i], NULL) != 0) 
        {
            free(args);
            exit(1);
        }
    }

    delete_tree(tree);
    free(tree);
    free(files);
    free(args);

    return 0;
}