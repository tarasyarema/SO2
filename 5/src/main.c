/**
 *
 * SO2 - Practica 5
 * @author Taras Yarema
 * @date 2020-01-03
 * @email tarasyaremabcn@gmail.com
 *
 */

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

#include "config.h"
#include "red-black-tree.h"
#include "process-threads.c"

#include "utils.c"
#include "read.c"
#include "write.c"

pthread_t ntid[MAX_THREADS];

int main(int argc, char **argv)
{
    FILE *db_file;

    int num_files, n_threads;

    char str1[MAX_CHARS], str2[MAX_CHARS], path[MAX_CHARS], line[MAX_CHARS];
    int opcio, return_code;
    char started = 0;

    clock_t start, end;
    double elapsed;

    rb_tree *tree = NULL;
    node_data *search_node = NULL;
    file *files = NULL;

    if (argc != 1)
        fprintf(stderr, "ERROR: Opcions de la linia de comandes ignorades\n");

    do
    {
        // Some information about
        // the current in-memory tree

        fprintf(stdout, "\n MAX_THREADS: %d\n", MAX_THREADS);

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

            db_file = fopen(str2, "r");

            if (db_file == NULL)
                return 1;

            fgets(line, MAX_CHARS, db_file);

            num_files = atoi(line);

            if (num_files <= 0)
            {
                fprintf(stderr, "ERROR: Number of files is %d <= 0\n", num_files);
                return 1;
            }

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

            if (DEBUG)
                fprintf(stderr, "INFO: Number of files to process %d\n", num_files);

            // Multi-thread shit

            arguments *args = malloc(sizeof(*args));

            args->tree = tree;
            args->files = files;
            args->num_files = num_files;

            // Use the min. of the 
            // number of files and the MAX_THREADS constant

            n_threads = num_files > MAX_THREADS ? MAX_THREADS : num_files;
            fprintf(stdout, " Threads we will use: %d\n", n_threads);

            for (int i = 0; i < n_threads; i++)
            {
                if (pthread_create(&ntid[i], NULL, process_list_files_mutex, args) != 0)
                {
                    free(args);
                    exit(1);
                }
            }

            for (int i = 0; i < n_threads; i++)
            {
                if (pthread_join(ntid[i], NULL) != 0) 
                {
                    free(args);
                    exit(1);
                }
            }

            free(files);
            free(args);

            break;

        case 2:
            system("clear");

            fprintf(stdout, " Nom de fitxer en que es desara l'arbre\n> ");
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

            fprintf(stdout, " Nom del fitxer que conte l'arbre\n> ");
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
