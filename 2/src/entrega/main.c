#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// Global defines
#include "config.h"

#include "red-black-tree.h"
#include "extreu-paraules.c"

int main(void)
{
   FILE *fp;
   const char *dictionary = "data/words_big";
   const char *test_file = "data/test";

   char *str, *line = NULL;
   size_t len = (size_t)MAX_CHARS;

   rb_tree *tree;
   node_data *n_data;

   fp = fopen(dictionary, "r");
   if (!fp)
   {
      printf("Could not open file: %s\n", dictionary);
      exit(1);
   }

   // Init the tree, read the dictionary
   // and populate

   tree = (rb_tree *)malloc(sizeof(rb_tree));
   init_tree(tree);

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
      }
   }

   fclose(fp);

   if (line)
      free(line);

   fp = fopen(test_file, "r");
   if (!fp)
   {
      printf("Could not open file: %s\n", test_file);
      exit(1);
   }

   line = NULL;

   while (getline(&line, &len, fp) != -1)
   {
      process_line(tree, line);
   }

   fclose(fp);

   if (line)
      free(line);

   fp = fopen(dictionary, "r");
   if (!fp)
   {
      printf("Could not open file: %s\n", test_file);
      exit(1);
   }

   line = NULL;

   while (getline(&line, &len, fp) != -1)
   {
      strtok(line, "\n");

      str = malloc((strlen(line) + 1) * sizeof(char));
      strcpy(str, line);

      n_data = find_node(tree, str);

      if (n_data != NULL && n_data->num_times > 0)
         printf("%s: %d\n", n_data->key, n_data->num_times);

      free(str);
   }

   fclose(fp);

   if (line)
      free(line);

   delete_tree(tree);
   free(tree);

   return 0;
}