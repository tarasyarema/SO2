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
 * Write the content of a Red Balck Tree to
 * a file in the correct format
 *
 */

int write_tree_file(rb_tree *tree, const char *file_name)
{
  FILE *f;
  
  int N = tree->size, magic = MAGIC_NUMBER;

  f = fopen(file_name, "w");

  if (f == NULL)
      return 1;

  fwrite(&magic, 1, sizeof(int), f);
  fwrite(&N, 1, sizeof(int), f);


  // Juicy part...
  // TODO

  /*
  for (int i = 0; i < N; i++)
  {
      int r_len = (rand() + 4) % 20, r_n = rand();
      str = rand_string(r_len);
      
      fprintf(stdout, "(%d) %s: %d\n", r_len, str, r_n);

      fwrite(&r_len, 1, sizeof(int), f);
      fwrite(str, r_len, sizeof(char), f);
      fwrite(&r_n, 1, sizeof(int), f);

      free(str);
  }
  */

  fclose(f);
  return 0;
}
