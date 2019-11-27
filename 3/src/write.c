/*
 *
 * Helper function to generate random
 * strings, so I can test the write function
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

/*
 *
 * DFS Implementation
 * Helper fucntion to write the content 
 * of a Red Balck Tree to
 * a file in the correct format
 *
 */

void write_node(node *n, FILE *f)
{
  int str_len = strlen(n->data->key);

  // Write the node data to 
  // the given file

  fwrite(&str_len, 1, sizeof(int), f);
  fwrite(n->data->key, str_len, sizeof(char), f);
  fwrite(&(n->data->num_times), 1, sizeof(int), f);

  if (n->right != NIL)
    write_node(n->right, f);
  
  if (n->left != NIL)
    write_node(n->left, f);
}


int write_tree_file(rb_tree *tree, const char *file_name)
{
  FILE *f;
  
  int N = tree->num_nodes, magic = MAGIC_NUMBER;

  f = fopen(file_name, "w");

  if (f == NULL)
      return 1;

  fwrite(&magic, 1, sizeof(int), f);
  fwrite(&N, 1, sizeof(int), f);

  if (tree->root != NIL)
    write_node(tree->root, f);

  fclose(f);
  return 0;
}
