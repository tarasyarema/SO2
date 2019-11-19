#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define _GNU_SOURCE

// Global defines
#include "config.h"

#include "red-black-tree.h"
#include "process.c"
#include "utils.c"

int main(int argc, char **argv)
{
   // Default strings
   // (use small list)

   char path[MAX_CHARS] = "base_dades/";
   char list[MAX_CHARS] = "base_dades/llista_2.cfg";
   char dictionary[MAX_CHARS] = "words_small";

   rb_tree *tree;

   // Get file to process and it's path

   if (argc > 1)
   {
      list[0] = '\0';
      strcpy(list, argv[1]);

      strcpy(path, dirname(argv[1]));
      strcat(path, "/");
   }

   // Get dictionary file if given

   if (argc > 2)
      strcpy(dictionary, argv[2]);

   // Init the tree from a given dictionary

   tree = (rb_tree *)malloc(sizeof(rb_tree));
   init_tree(tree);

   if (init_tree_from_file(tree, dictionary) != 0)
   {
      fprintf(stderr,
              "Error on tree init.\nGiven dictionary: %s\nTree address: %p\n",
              dictionary, tree);
      exit(1);
   }

   // Process data

   if (process_list_files(tree, list, path) != 0)
   {
      fprintf(stderr, "Error processing %s\n", list);
      exit(1);
   }

   // Bash version:
   // sort -t "," -k2 -rn <data_file> | head -10

   if (print_tree_positive_n(tree, dictionary, 10) != 0)
   {
      fprintf(stderr, "Error");
      exit(1);
   }

   delete_tree(tree);
   free(tree);

   return 0;
}