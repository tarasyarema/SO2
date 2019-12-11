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

#define MAX_FORKS 10

typedef struct shared_mem {
    sem_t s;
    double elapsed;
} total_elapsed;

total_elapsed *t_elapsed;

int main(int argc, char **argv)
{
    FILE *db_file;
    char *target = "the", *dict_filename = "w", *db_filename = "hardcore";

    rb_tree *tree = NULL;
    char *tree_mmap, *db_mmap;
    int db_files_n, forks;

    clock_t start, end;
    double elapsed;

    tree = (rb_tree *)malloc(sizeof(rb_tree));
    init_tree(tree);

    if (init_tree_from_file(tree, dict_filename) != 0) return 1;
    tree_mmap = serialize_node_data_to_mmap(tree);

    db_file = fopen(db_filename, "r");
    if (db_file == NULL) return 1;

    db_mmap = _dbfnames_to_mmap(db_file);
    
    fclose(db_file);

    db_files_n = *((int *)db_mmap);
    forks = db_files_n < MAX_FORKS ? db_files_n : MAX_FORKS;

    // Multiprocess shit

    int pids[forks], n = forks;
    int status, pid;

    t_elapsed = mmap(NULL, sizeof(total_elapsed), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
    t_elapsed->elapsed = 0.0;
    sem_init(&t_elapsed->s, 1, 1);

    for (int i = 0; i < n; ++i) 
    {
        if ((pids[i] = fork()) < 0) 
        {
            perror("ERROR: Could not fork...");
            abort();
        } 
        else if (pids[i] == 0) 
        {
            // clock_t f_start = clock();

            pid = getpid();
            process_list_files_sem(db_mmap, tree_mmap, pid, i);
            
            // clock_t f_end = clock();
            // sem_wait(&t_elapsed->s);
            // t_elapsed->elapsed += (double)(f_end - f_start) / CLOCKS_PER_SEC;
            // sem_post(&t_elapsed->s);

            exit(0);
        }
    }

    // Wait for the processes to finish

    while (n > 0) 
    {
        pid = wait(&status);
        // fprintf(stderr, "INFO: Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
        --n;
    }
    
    if (DEBUG_TIME)
        fprintf(stderr, "INFO: Elapsed %4.8lf s.\n", t_elapsed->elapsed);
    
    munmap(t_elapsed, sizeof(total_elapsed));

    fprintf(stdout, "Child processes: %d\n", forks);

    start = clock();
    _find_node(tree, target);
    end = clock();

    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tree search:   %3.6lf s.\n", elapsed);

    deserialize_node_data_from_mmap(tree, tree_mmap);

    delete_tree(tree);
    free(tree);

    _dbfnames_munmmap(db_mmap);

    return 0;
}