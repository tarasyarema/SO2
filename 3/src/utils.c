#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
      printf("Could not open file: %s\n", f_name);
      return 1;
   }

   clock_t start, end;
   double elapsed;

   unsigned long processed = 0;

   start = clock();

   while (getline(&line, &len, fp) != -1)
   {
      // Get rid of the '\n' char
      // in the end of every line

      strtok(line, "\n");

      // The + 1 is for the null operator
      // strcpy adds to str.
      // (Thanks valgrind)

      str = malloc((strlen(line) + 1) * sizeof(char));
      strcpy(str, line);

      n_data = find_node(tree, str);

      if (n_data == NULL)
      {
         n_data = malloc(sizeof(node_data));

         n_data->key = str;
         n_data->num_times = 0;

         insert_node(tree, n_data);
         ++processed;
      }
   }

   end = clock();
   elapsed = (double)(end - start) / CLOCKS_PER_SEC;

   if (DEBUG_TIME)
      fprintf(stderr, "Processed %lu words in %3.6lf s.\n", processed, elapsed);

   if (line)
      free(line);

   fclose(fp);
   return 0;
}

/**
 * 
 * Function to print the nodes of a tree 
 * with num_times > 0.
 * 
 */

int print_tree_positive(rb_tree *tree, const char *f_name)
{
   FILE *fp;

   char *str, *line = NULL;
   size_t len = (size_t)MAX_CHARS;

   node_data *n_data;

   fp = fopen(f_name, "r");
   if (!fp)
   {
      fprintf(stderr, "Could not open file: %s\n", f_name);
      return 1;
   }

   if (PRINT_CSV == 0)
   {
      fprintf(stdout, " %16s | %8s \n", "key", "count");

      fprintf(stdout, "   ");
      for (int i = 0; i < 14 + 8 + 6; i++)
         fprintf(stdout, "-");
      fprintf(stdout, "\n");
   }

   while (getline(&line, &len, fp) != -1)
   {
      strtok(line, "\n");

      str = malloc((strlen(line) + 1) * sizeof(char));
      strcpy(str, line);

      n_data = find_node(tree, str);

      if (n_data != NULL && n_data->num_times > 0)
      {
         if (PRINT_CSV)
            fprintf(stdout, "%s,%d\n", n_data->key, n_data->num_times);
         else
            fprintf(stdout, " %16s | %8d\n", n_data->key, n_data->num_times);
      }

      free(str);
   }

   if (line)
      free(line);

   fclose(fp);
   return 0;
}

/**
 * 
 * Function to print the n nodes of a tree 
 * with greater num_times > 0.
 * 
 */

int print_tree_positive_n(rb_tree *tree, const char *f_name, int n)
{
   FILE *fp;

   char *str, *line = NULL;
   size_t len = (size_t)MAX_CHARS;

   node_data *n_data;

   // Ladder variables

   char top_key[n][MAX_CHARS];
   int *top_num, min = 0;

   // Temporal variables

   int tmp, tmp2;
   char tmp_k[MAX_CHARS], tmp_k2[MAX_CHARS];

   fp = fopen(f_name, "r");
   if (!fp)
   {
      fprintf(stderr, "Could not open file: %s\n", f_name);
      return 1;
   }

   // Init the ladder variables to defaults

   top_num = malloc(n * sizeof(int));
   for (int i = 0; i < n; i++)
      top_num[i] = min;

   for (int i = 0; i < n; i++)
      top_key[i][0] = '\0';

   while (getline(&line, &len, fp) != -1)
   {
      strtok(line, "\n");

      str = malloc((strlen(line) + 1) * sizeof(char));
      strcpy(str, line);

      n_data = find_node(tree, str);

      if (n_data != NULL && n_data->num_times > min)
      {
         for (int i = 0; i < n; i++)
         {
            if (top_num[i] < n_data->num_times)
            {
               tmp = top_num[i];
               top_num[i] = n_data->num_times;

               tmp_k[0] = '\0';
               strcpy(tmp_k, top_key[i]);
               strcpy(top_key[i], n_data->key);

               for (int j = i + 1; j < n; j++)
               {
                  tmp2 = top_num[j];
                  top_num[j] = tmp;
                  tmp = tmp2;

                  tmp_k2[0] = '\0';
                  strcpy(tmp_k2, top_key[j]);
                  strcpy(top_key[j], tmp_k);
                  strcpy(tmp_k, tmp_k2);
               }

               if (i == n - 1)
                  min = n_data->num_times;

               break;
            }
         }
      }

      free(str);
   }

   if (PRINT_CSV)
   {
      for (int i = 0; i < n; i++)
         fprintf(stdout, "%s,%d\n", top_key[i], top_num[i]);
   }
   else
   {
      fprintf(stdout, " %3s | %10s | %8s \n", "#", "key", "count");

      fprintf(stdout, " ");
      for (int i = 0; i < 3 + 10 + 8 + 6; i++)
         fprintf(stdout, "-");
      fprintf(stdout, "\n");

      for (int i = 0; i < n; i++)
         fprintf(stdout, " %3d | %10s | %8d \n", i + 1, top_key[i], top_num[i]);
   }

   if (line)
      free(line);

   free(top_num);
   fclose(fp);
   return 0;
}