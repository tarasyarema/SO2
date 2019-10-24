#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR      1024
#define MAGIC_NUMBER 0x01234567

int main()
{
    FILE *f;
    int N, magic;
    int tmp = 0, v = 0;
    char buffer[MAXCHAR], *str;

    f = fopen("test", "r");

    if (f == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }
    
    fread(&magic, 1, sizeof(int), f);
    fread(&N, 1, sizeof(int), f);

    fprintf(stdout, "#%d\n", N);
    
    for (int i = 0; i < N; i++) 
    {
        fread(&tmp, 1, sizeof(int), f);
        str = malloc(tmp * sizeof(tmp));

        fread(str, tmp, sizeof(char), f);
        fread(&v, 1, sizeof(int), f);

        strncpy(buffer, str, tmp);
        free(str);
        buffer[tmp] = 0;

        fprintf(stdout, "(%d) %s: %d\n", tmp, buffer, v);
    }

    fclose(f);

    return 0;
}
