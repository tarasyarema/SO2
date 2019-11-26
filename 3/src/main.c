/**
 *
 * Practica 3 
 *
 */

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include "config.h"
#include "utils.c"
#include "process.c"
#include "red-black-tree.h"
#include "read.c"

// #include "write.c"


/**
 * 
 *  Menu
 * 
 */

int menu() 
{
    char str[5];
    int opcio;

    printf("\n\nMenu\n\n");
    printf(" 1 - Creacio de l'arbre\n");
    printf(" 2 - Emmagatzemar arbre a disc\n");
    printf(" 3 - Llegir arbre de disc\n");
    printf(" 4 - Consultar informacio de l'arbre\n");
    printf(" 5 - Sortir\n\n");
    printf("   Escull opcio: ");

    fgets(str, 5, stdin);
    opcio = atoi(str); 

    return opcio;
}

/**
 * 
 *  Main procedure
 *
 */

int main(int argc, char **argv)
{
    char str1[MAX_CHARS], str2[MAX_CHARS], path[MAX_CHARS];
    int opcio;
    
    rb_tree *tree = NULL;
    node_data *search_node = NULL;

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case 1:
                system("clear");
                
                printf("Fitxer de diccionari de paraules: ");
                fgets(str1, MAX_CHARS, stdin);
                str1[strlen(str1)-1]=0;

                printf("Fitxer de base de dades: ");
                fgets(str2, MAX_CHARS, stdin);
                str2[strlen(str2)-1]=0;

                strcpy(path, dirname(str2));
                strcat(path, "/");
                fprintf(stdout, "%s - %s - %s\n", str1, str2, path);

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
                fprintf(stderr, "Size of the tree is: %ld bytes\n", sizeof(tree));

                if (tree == NULL)
                {
                    fprintf(stderr, "Tree is empty\n");
                    break;
                }

                break;

            case 3:
                system("clear");
                
                printf("Nom del fitxer que conte l'arbre: ");
                fgets(str1, MAX_CHARS, stdin);
                str1[strlen(str1)-1]=0;

                if (tree == NULL)
                {
                    tree = (rb_tree *)malloc(sizeof(rb_tree));
                    init_tree(tree);

                    read_tree_file(str1, tree);
                }
                else
                {
                    fprintf(stdout, "There is a tree saved in memory, do you want to replace it? (y/n) ");
                    fgets(str2, MAX_CHARS, stdin);
                    str2[strlen(str2)-1] = 0;
                    
                    if (strcasecmp(str2, "y") != 0)
                        break;
                    
                    fprintf(stderr, "Deleting old tree ... ");
                    delete_tree(tree);
                    free(tree);
                    fprintf(stderr, "done.\n");

                    fprintf(stderr, "Reading new tree from \"%s\" ... ", str2);

                    tree = (rb_tree *)malloc(sizeof(rb_tree));
                    init_tree(tree);
                    
                    read_tree_file(str2, tree);
                    fprintf(stderr, "done.\n");
                }


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

                if (strlen(str1) == 0)
                {
                    if (tree->root == NIL || tree->root->data == NULL)
                    {
                        fprintf(stderr, "Root node is nil\n");
                        break;
                    }

                    search_node = tree->root->data;

                    fprintf(stdout, "Searching for the best node ... ");
                    find_node_best(tree->root, &search_node);
                    fprintf(stdout, "done.\n");

                    fprintf(stdout, "%s: %d\n", search_node->key, search_node->num_times);

                    break;
                }

                search_node = find_node(tree, str1);
                if (search_node == NULL)
                {
                    fprintf(stdout, "Word \"%s\" not found in the tree.\n", str1);
                }
                else
                {
                    fprintf(stdout, "%s: %d\n", search_node->key, search_node->num_times);
                }

                break;

            case 5:
                system("clear");
                
                if (tree != NULL)
                {
                    fprintf(stderr, "Freeing in-memory tree ...");
                    delete_tree(tree);
                    free(tree);
                    fprintf(stdout, " done.\n");
                }
                else
                {
                    fprintf(stdout, "Tree already null.\n");
                }

                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);

    return 0;
}

