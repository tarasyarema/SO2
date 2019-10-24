#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXCHAR      100
#define MAGIC_NUMBER 0x01234567

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

int main()
{
    FILE *f;
    
    char *str;
    int N = 13, magic = MAGIC_NUMBER;

    f = fopen("test", "w");

    if (f == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    srand(time(NULL));

    fwrite(&magic, 1, sizeof(int), f);
    fwrite(&N, 1, sizeof(int), f);

    fprintf(stdout, "#%d\n", N);

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

    fclose(f);

    return 0;
}
