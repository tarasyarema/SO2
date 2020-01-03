#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <errno.h>

#include "config.h"
#include "red-black-tree.h"

/**
 * 
 *  Menu
 * 
 */

int menu() 
{
    char str[5];
    int opcio;

    printf(" Menu\n");
    printf(" 1. Creacio de l'arbre\n");
    printf(" 2. Emmagatzemar arbre a disc\n");
    printf(" 3. Llegir arbre de disc\n");
    printf(" 4. Consultar informacio de l'arbre\n");
    printf(" 5. Sortir\n");
    printf(" 6. Borrar l'arbre en memoria\n\n");
    printf(" Escull opcio\n> ");

    fgets(str, 5, stdin);
    opcio = atoi(str); 

    return opcio;
}

/**
 *
 * DFS implementation
 * Recursive algorithm to find the node with 
 * most occurences in a Red-Black tree
 *
 */

void find_node_best(node *n, node_data **best)
{
   if (n->data->num_times > (*best)->num_times)
      *best = n->data; 

   if (n->right != NIL)
   {
      find_node_best(n->right, best);
   }
   
   if (n->left != NIL) 
   {
      find_node_best(n->left, best);
   }
}

/**
 * 
 * Function to init a tree, read a dictionary 
 * from a file and populate the tree.
 * 
 */

int init_tree_from_file(rb_tree *tree, const char *f_name)
{
   FILE *fp;

   char *str, *line = NULL;
   
   size_t len = (size_t)MAX_CHARS;

   node_data *n_data;

   fp = fopen(f_name, "r");
   if (!fp)
   {
      fprintf(stderr, "ERROR: Could not open file: %s\n", f_name);
      return 1;
   }

   clock_t start, end;
   double elapsed;

   unsigned long processed = 0;

   start = clock();

   while (getline(&line, &len, fp) != -1)
   {
      strtok(line, "\n");

      str = malloc((strlen(line) + 1) * sizeof(char));
      strcpy(str, line);

      n_data = find_node(tree, str);

      if (n_data == NULL)
      {
         n_data = malloc(sizeof(node_data));

         n_data->key = str;
         n_data->num_times = 0;
         n_data->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

         insert_node(tree, n_data);
         ++processed;
      }
   }

   end = clock();
   elapsed = (double)(end - start) / CLOCKS_PER_SEC;

   if (DEBUG_TIME)
      fprintf(stderr, "INFO: Processed %lu words in %3.6lf s.\n", processed, elapsed);

   if (line)
      free(line);

   fclose(fp);
   return 0;
}
