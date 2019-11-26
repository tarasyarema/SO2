#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_tree_file(const char *file_name, rb_tree *tree)
{
    FILE *f;
    int N, magic;
    int tmp = 0, v = 0;
    char buffer[MAX_CHARS], *str;

    node_data *n_data = NULL;

    f = fopen(file_name, "r");

    if (f == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", file_name);
        return 1;
    }
    
    fread(&magic, 1, sizeof(int), f);
    fread(&N, 1, sizeof(int), f);

    fprintf(stdout, "%d words\n", N);
    
    for (int i = 0; i < N; i++) 
    {
        fread(&tmp, 1, sizeof(int), f);
        str = malloc(tmp * sizeof(tmp));

        fread(str, tmp, sizeof(char), f);
        fread(&v, 1, sizeof(int), f);

        strncpy(buffer, str, tmp);

        fprintf(stderr, "%s: %d\n", str, v);

        n_data = find_node(tree, str);

        if (n_data == NULL)
        {
            n_data = malloc(sizeof(node_data));

            n_data->key = str;
            n_data->num_times = 0;

            insert_node(tree, n_data);
        }

        free(str);
        buffer[tmp] = 0;
    }

    fclose(f);

    return 0;
}
