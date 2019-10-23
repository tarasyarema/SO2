#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "red-black-tree.h"
#define MAXVALUE 20

char *rand_string(size_t size)
{
   const char charset[] = "abcdefghijklmnopqrstuvwxyz";
   char *str = malloc(size * sizeof(char));

   if (size)
   {
      --size;
      for (size_t n = 0; n < size; n++)
      {
         int key = rand() % (int)(sizeof charset - 1);
         str[n] = charset[key];
      }

      str[size] = '\0';
   }

   return str;
}

int main()
{
   int r_len;
   rb_tree *tree;
   node_data *n_data;

   srand(time(NULL));
   tree = (rb_tree *)malloc(sizeof(rb_tree));

   init_tree(tree);

   n_data = malloc(sizeof(node_data));
   r_len = rand() % MAXVALUE + 1;
   n_data->key = rand_string(r_len);

   printf("%s\n", n_data->key);

   free(n_data->key);
   free(n_data);

   delete_tree(tree);
   free(tree);

   return 0;
}