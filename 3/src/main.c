/**
 *
 * SO2 - Practica 3 
 * @author Taras Yarema
 * @date 2019-11-27
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

// Config variables
#include "config.h"

// Red-Black tree structures and functions
#include "red-black-tree.h"

// Utility functions and
// file processers
#include "utils.c"
#include "process.c"
#include "read.c"
#include "write.c"


int main(int argc, char **argv)
{
    char str1[MAX_CHARS], str2[MAX_CHARS], path[MAX_CHARS];
    int opcio;
    
    clock_t start, end;
    double elapsed;

    rb_tree *tree = NULL;
    node_data *search_node = NULL;

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        // Some information about 
        // the current in-memory tree

        fprintf(stdout, "INFO: ");
        
        if (tree == NULL)
            fprintf(stdout, "Tree is empty\n");
        else
            fprintf(stdout, "Tree has %ld nodes\n", tree->size);
        
        opcio = menu();

        switch (opcio) {
            case 1:
                system("clear");
                
                printf("Fitxer de diccionari de paraules: ");
                fgets(str1, MAX_CHARS, stdin);
                str1[strlen(str1)-1]=0;

                printf("Fitxer de base de dades: ");
                fgets(str2, MAX_CHARS, stdin);
                str2[strlen(str2)-1]=0;

                // WE get the path of the DB file

                strcpy(path, dirname(str2));
                strcat(path, "/");
                fprintf(stdout, "%s - %s - %s\n", str1, str2, path);

                // Allocate memory for the tree
                // and init it from the given files.
                // Same process as the last assignment.

                tree = (rb_tree *)malloc(sizeof(rb_tree));
                init_tree(tree);

                if (init_tree_from_file(tree, str1) != 0)
                {
                    fprintf(stderr,
                            "Error on tree init.\nGiven dictionary: %s\nTree address: %p\n",
                            str1, tree);
                }

                if (process_list_files(tree, str2, path) != 0)
                {
                   fprintf(stderr, "Error processing %s\n", str2);
                }

                break;

            case 2:
                system("clear");
                
                printf("Nom de fitxer en que es desara l'arbre: ");
                fgets(str1, MAX_CHARS, stdin);
                str1[strlen(str1)-1]=0;

                fprintf(stdout, "Out file is: %s\n", str1);

                // If the tree is empty
                // we do not write...

                if (tree == NULL)
                {
                    fprintf(stderr, "Tree is empty\n");
                    break;
                }
                
                // Otherwise, we try to dump it 
                // with the given binary format

                if (write_tree_file(tree, str1) != 0) 
                    fprintf(stderr, "Could not write tree to file \"%s\"\n", str1);
                else
                    fprintf(stdout, "Wrote binary tree data to \"%s\"\n", str1);

                break;

            case 3:
                system("clear");
                
                printf("Nom del fitxer que conte l'arbre: ");
                fgets(str1, MAX_CHARS, stdin);
                str1[strlen(str1)-1]=0;

                // If the tree is empty,
                // we init it directly from the given file

                if (tree == NULL)
                {
                    tree = (rb_tree *)malloc(sizeof(rb_tree));
                    init_tree(tree);

                    if (read_tree_file(tree, str1) != 0)
                        fprintf(stderr, "Could not init tree from \"%s\"\n", str1);

                    break;
                }

                // Otherwise, we ask if the user
                // wants to delete the current tree and
                // replace it with the one in the given file

                fprintf(stdout, "There is a tree saved in memory, do you want to replace it? (y/n) ");
                fgets(str2, MAX_CHARS, stdin);
                str2[strlen(str2)-1] = 0;
                
                if (strcasecmp(str2, "y") != 0)
                    break;
                
                // Before initing the new tree
                // we deallocate the memory of the old one,
                // so we don't have memory leaks.

                fprintf(stderr, "Deleting old tree ... ");
                delete_tree(tree);
                free(tree);
                fprintf(stderr, "done.\n");


                fprintf(stderr, "Reading new tree from \"%s\" ... ", str1);
                
                tree = (rb_tree *)malloc(sizeof(rb_tree));
                init_tree(tree);

                read_tree_file(tree, str1);
                fprintf(stderr, "done.\n");

                break;

            case 4:
                system("clear");
                
                printf("Paraula a buscar o polsa enter per saber la paraula que apareix mes vegades: ");
                fgets(str1, MAX_CHARS, stdin);
                str1[strlen(str1)-1]=0;

                if (tree == NULL)
                {
                    fprintf(stderr, "Tree is empty\n");
                    break;
                }

                // If no word is given we search 
                // for the word with more occurences

                if (strlen(str1) == 0)
                {
                    if (tree->root == NIL || tree->root->data == NULL)
                    {
                        fprintf(stderr, "Root node is nil\n");
                        break;
                    }

                    search_node = tree->root->data;

                    fprintf(stdout, "Searching for the best node ... ");
                    
                    start = clock();
                    find_node_best(tree->root, &search_node);
                    end = clock();
                    fprintf(stdout, "done.\n");
                    
                    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
                    if (DEBUG_TIME)
                        fprintf(stderr, "Search time: %3.6lf s.\n", elapsed);

                    fprintf(stdout, "%s: %d\n", search_node->key, search_node->num_times);

                    break;
                }

                // Otherwise we try to find
                // the given word in the tree

                start = clock();
                search_node = find_node(tree, str1);
                end = clock();

                elapsed = (double)(end - start) / CLOCKS_PER_SEC;
                if (DEBUG_TIME)
                    fprintf(stderr, "Search time: %3.6lf s.\n", elapsed);
                
                if (search_node == NULL)
                    fprintf(stdout, "Word \"%s\" not found in the tree.\n", str1);
                else
                    fprintf(stdout, "%s: %d\n", search_node->key, search_node->num_times);

                break;

            case 5:
                system("clear");
                
                // We only deallocate when the 
                // tree is not empty, i.e. was
                // never initializaed

                if (tree != NULL)
                {
                    fprintf(stderr, "Freeing in-memory tree ...");

                    delete_tree(tree);
                    free(tree);

                    fprintf(stdout, " done.\n");

                    break;
                }
                
                fprintf(stdout, "Tree already null.\n");
                break;

            default:
                printf("Opcio no valida\n");

        }
    }
    while (opcio != 5);

    return 0;
}
