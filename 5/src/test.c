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

#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

// Config variables
#include "config.h"

// Red-Black tree structures and functions
#include "red-black-tree.h"

// Mmap functions
#include "tree-to-mmap.h"
#include "dbfnames-mmap.h"

#include "utils-mmap.c"
#include "process-mmap.c"


// Utility functions and
// file processers
#include "utils.c"
#include "process.c"
#include "read.c"
#include "write.c"

#define MAX_FORKS 2

int main(int argc, char **argv)
{
    FILE *db_file;
    char *dict_filename = "w", *db_filename = "test_data/l";

    rb_tree *tree = NULL;
    char *tree_mmap, *db_mmap;
    int db_files_n, forks = MAX_FORKS;

    if (argc > 1)
    {
        forks = atoi(argv[1]);
    }

    if (argc > 2)
    {
        db_filename = argv[2];
    }

    tree = (rb_tree *)malloc(sizeof(rb_tree));
    init_tree(tree);

    if (init_tree_from_file(tree, dict_filename) != 0) return 1;
    tree_mmap = serialize_node_data_to_mmap(tree);

    db_file = fopen(db_filename, "r");
    if (db_file == NULL) return 1;

    db_mmap = _dbfnames_to_mmap(db_file);

    fclose(db_file);

    db_files_n = *((int *)db_mmap);

    if (DEBUG)
        fprintf(stderr, "INFO: Number of files to process %d\n", db_files_n);
    
    // Multiprocess shit

    int pids[forks], n = forks;
    int status, pid;

    for (int i = 0; i < n; ++i)
    {
        if ((pids[i] = fork()) < 0)
        {
            perror("ERROR: Could not fork...");
            abort();
        }
        else if (pids[i] == 0)
        {
            pid = getpid();
            process_list_files_sem(db_mmap, tree_mmap, pid, 0);
            exit(0);
        }
    }

    // Wait for the processes to finish

    while (n > 0)
    {
        pid = wait(&status);

        if (DEBUG)
            fprintf(stderr, "INFO: Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
        
        --n;
    }

    deserialize_node_data_from_mmap(tree, tree_mmap);

    delete_tree(tree);
    free(tree);

    _dbfnames_munmmap(db_mmap);

    return 0;
}
