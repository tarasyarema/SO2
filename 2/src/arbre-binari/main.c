/**
 *
 * Main file 
 * 
 * This file is an example that uses the red-black-tree.c functions.
 *
 * Lluis Garrido, July 2019.
 * Update: Taras Yarema, October 2019.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "red-black-tree.h"

#define DEFAULT_VALUE 100
#define STR_LEN 3
#define DEBUG 0

// Helper function to generate random keys
char *rand_string(size_t);

/**
 *
 *  Main function. This function is an example that shows
 *  how the binary tree works.
 *
 */

int main(int argc, char **argv)
{
  int a, maxnum;
  unsigned long long int ct = 0;
  char *str;

  rb_tree *tree;
  node_data *n_data;

  if (argc != 2)
    maxnum = DEFAULT_VALUE;
  else
    maxnum = atoi(argv[1]);

  if (DEBUG)
    printf("Test with red-black-tree\n");

  srand(time(NULL));

  tree = (rb_tree *)malloc(sizeof(rb_tree));
  init_tree(tree);

  for (ct = 0; ct < maxnum; ct++)
  {
    str = rand_string(STR_LEN);
    n_data = find_node(tree, str);

    if (n_data != NULL)
    {
      n_data->num_times++;
      free(str);

      if (DEBUG)
        printf("%s:  %d\n", str, n_data->num_times);
    }
    else
    {
      n_data = malloc(sizeof(node_data));

      n_data->key = str;
      n_data->num_times = 1;

      insert_node(tree, n_data);

      if (DEBUG)
        printf("%s:  CREATED\n", str);
    }
  }

  if (DEBUG)
    printf("\n\n");

  /** 
   * 
   * As we generated the keys randomly and did not store them,
   * we must just guess some and see if the random key is in
   * the tree...
   *
   */

  for (a = 0; a < maxnum; a++)
  {
    str = rand_string(STR_LEN);
    n_data = find_node(tree, str);

    if (n_data)
    {
      if (DEBUG)
        printf("%s:  %d\n", n_data->key, n_data->num_times);

      ct += n_data->num_times;
    }
    else
    {
      if (DEBUG)
        printf("%s:  0\n", str);
    }

    free(str);
  }

  printf("Accessed the tree %lld times.\n", ct);

  delete_tree(tree);
  free(tree);

  return 0;
}

/** 
 * 
 * Function that returns a dynamically
 * generated random string of size len - 1
 * 
 */

char *rand_string(size_t len)
{
  static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *str = NULL;

  if (len)
  {
    str = malloc(sizeof(char) * (len + 1));

    if (str)
    {
      for (int n = 0; n < len; n++)
      {
        int key = rand() % (int)(sizeof(charset) - 1);
        str[n] = charset[key];
      }

      str[len] = 0;
    }
  }

  return str;
}
