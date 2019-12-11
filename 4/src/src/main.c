/**
 *
 * SO2 - Practica 4
 * @author Taras Yarema
 * @date 2019-12-11
 * @email tarasyaremabcn@gmail.com
 *
 */

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

#define MAX_FORKS 4

int main(int argc, char **argv)
{
    FILE *db_file;

    char *tree_mmap, *db_mmap;
    int db_files_n, forks = MAX_FORKS;

    int pids[forks], n;
    int status, pid;

    char str1[MAX_CHARS], str2[MAX_CHARS], path[MAX_CHARS];
    int opcio, return_code;
    char started = 0;

    clock_t start, end;
    double elapsed;

    rb_tree *tree = NULL;
    node_data *search_node = NULL;

    if (argc != 1)
        fprintf(stderr, "ERROR: Opcions de la linia de comandes ignorades\n");

    do
    {
        // Some information about
        // the current in-memory tree

        fprintf(stdout, "\n Processes we will use: %d\n", forks);

        if (started != 0)
        {
            if (tree == NULL)
                fprintf(stdout, "\n Tree (%p): empty\n\n", tree);
            else
                fprintf(stdout, "\n Tree (%p): %ld nodes and %ld bytes\n\n", tree, tree->num_nodes, tree->size);
        }
        else
        {
            started = 1;
        }

        opcio = menu();

        switch (opcio)
        {
        case 1:
            system("clear");

            printf(" Fitxer de diccionari de paraules\n> ");
            fgets(str1, MAX_CHARS, stdin);
            str1[strlen(str1) - 1] = 0;

            printf(" Fitxer de base de dades\n> ");
            fgets(str2, MAX_CHARS, stdin);
            str2[strlen(str2) - 1] = 0;

            // We get the path of the DB file

            strcpy(path, dirname(str2));
            strcat(path, "/");

            // We deallocate the tree if
            // there's one allocated already

            if (tree != NULL)
            {
                delete_tree(tree);
                free(tree);
            }

            // Allocate memory for the tree
            // and init it from the given files.
            // Same process as the last assignment.

            tree = (rb_tree *)malloc(sizeof(rb_tree));
            init_tree(tree);

            if (init_tree_from_file(tree, str1) != 0)
                return 1;
            
            tree_mmap = serialize_node_data_to_mmap(tree);

            db_file = fopen(str2, "r");

            if (db_file == NULL)
                return 1;

            db_mmap = _dbfnames_to_mmap(db_file);

            fclose(db_file);

            db_files_n = *((int *)db_mmap);

            if (DEBUG)
                fprintf(stderr, "INFO: Number of files to process %d\n", db_files_n);

            // Multiprocess shit

            n = forks;

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
            _dbfnames_munmmap(db_mmap);

            break;

        case 2:
            system("clear");

            printf(" Nom de fitxer en que es desara l'arbre\n> ");
            fgets(str1, MAX_CHARS, stdin);
            str1[strlen(str1) - 1] = 0;

            // If the tree is empty
            // we do not write...

            if (tree == NULL)
            {
                fprintf(stderr, "ERROR: Tree is empty\n");
                break;
            }

            // Otherwise, we try to dump it
            // with the given binary format

            if (write_tree_file(tree, str1) != 0)
                fprintf(stderr, "ERROR: Could not write tree to file %s\n", str1);
            else
                fprintf(stderr, "INFO: Wrote binary tree data to %s\n", str1);

            break;

        case 3:
            system("clear");

            printf(" Nom del fitxer que conte l'arbre\n> ");
            fgets(str1, MAX_CHARS, stdin);
            str1[strlen(str1) - 1] = 0;

            // If the tree is empty,
            // we init it directly from the given file

            if (tree == NULL)
            {
                tree = (rb_tree *)malloc(sizeof(rb_tree));
                init_tree(tree);

                if (read_tree_file(tree, str1) != 0)
                    fprintf(stderr, "ERROR: Could not init tree from %s\n", str1);

                break;
            }

            // Otherwise, we ask if the user
            // wants to delete the current tree and
            // replace it with the one in the given file

            fprintf(stdout, " There is a tree saved in memory, do you want to replace it? (y/n)\n> ");
            fgets(str2, MAX_CHARS, stdin);
            str2[strlen(str2) - 1] = 0;

            if (strcasecmp(str2, "y") != 0)
                break;

            // Before initing the new tree
            // we deallocate the memory of the old one,
            // so we don't have memory leaks.

            fprintf(stderr, "INFO: Deleting old tree ... ");
            delete_tree(tree);
            free(tree);
            fprintf(stderr, "done.\n");

            fprintf(stderr, "INFO: Reading new tree from %s ... ", str1);

            tree = (rb_tree *)malloc(sizeof(rb_tree));
            init_tree(tree);

            return_code = read_tree_file(tree, str1);

            if (return_code == 1)
                fprintf(stderr, "\nERROR: Could not read from file %s.\n", str1);
            else if (return_code == 2)
                fprintf(stderr, "\nERROR: Magic number not equal\n");
            else
                fprintf(stderr, "done.\n");

            break;

        case 4:
            system("clear");

            fprintf(stdout, " Paraula a buscar o polsa enter per saber la paraula que apareix mes vegades\n> ");
            fgets(str1, MAX_CHARS, stdin);
            str1[strlen(str1) - 1] = 0;

            if (tree == NULL)
            {
                fprintf(stderr, "ERROR: Tree is empty\n");
                break;
            }

            // If no word is given we search
            // for the word with more occurences

            if (strlen(str1) == 0)
            {
                if (tree->root == NIL || tree->root->data == NULL)
                {
                    fprintf(stderr, "ERROR: Root node is nil\n");
                    break;
                }

                search_node = tree->root->data;

                fprintf(stderr, "INFO: Searching for the best node ... ");

                start = clock();
                find_node_best(tree->root, &search_node);
                end = clock();

                fprintf(stderr, "done.\n");

                elapsed = (double)(end - start) / CLOCKS_PER_SEC;

                if (DEBUG_TIME)
                    fprintf(stderr, "INFO: Search time: %3.6lf s.\n", elapsed);

                fprintf(stdout, "\n key:       %s\n", search_node->key);
                fprintf(stdout, " num_times: %d\n", search_node->num_times);

                break;
            }

            // Otherwise we try to find
            // the given word in the tree

            start = clock();
            search_node = find_node(tree, str1);
            end = clock();

            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
            if (DEBUG_TIME)
                fprintf(stderr, "INFO: Search time: %3.6lf s.\n", elapsed);

            if (search_node == NULL)
            {
                fprintf(stdout, "\n Word '%s' not found in the tree.\n", str1);
            }
            else
            {
                fprintf(stdout, "\n key:       %s\n", search_node->key);
                fprintf(stdout, " num_times: %d\n", search_node->num_times);
            }

            break;

        case 5:
            system("clear");

            // We only deallocate when the
            // tree is not empty, i.e. was
            // never initializaed

            if (tree != NULL)
            {
                fprintf(stderr, "INFO: Freeing in-memory tree ...");

                delete_tree(tree);
                free(tree);

                fprintf(stderr, " done.\n");
                break;
            }

            fprintf(stderr, "INFO: Tree already null.\n");
            break;

        case 6:
            system("clear");

            if (tree != NULL)
            {
                fprintf(stderr, "INFO: Freeing in-memory tree ...");

                delete_tree(tree);
                free(tree);
                tree = NULL;
                fprintf(stderr, " done.\n");

                break;
            }

            fprintf(stderr, "INFO: Tree already null.");
            break;

        default:
            fprintf(stderr, "ERROR: Opcio no valida\n");
        }
    } while (opcio != 5);

    return 0;
}
