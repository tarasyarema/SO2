#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_tree_file(rb_tree *tree, const char *file_name)
{
    FILE *f;

    int N, magic;
    int tmp = 0, v = 0;
    
    char *str;

    node_data *n_data;

    f = fopen(file_name, "r");

    if (f == NULL)
        return 1;
    
    fread(&magic, 1, sizeof(int), f);

    if (magic != MAGIC_NUMBER)
        return 2;

    fread(&N, 1, sizeof(int), f);
    
    for (int i = 0; i < N; i++) 
    {
        fread(&tmp, 1, sizeof(int), f);
        
        str = malloc((tmp + 1) * sizeof(tmp));
        fread(str, tmp, sizeof(char), f);
        str[tmp] = 0;
        
        fread(&v, 1, sizeof(int), f);

        n_data = find_node(tree, str);

        if (n_data == NULL)
        {
            n_data = malloc(sizeof(node_data));

            n_data->key = str;
            n_data->num_times = v;

            insert_node(tree, n_data);
        }
    }

    fclose(f);

    return 0;
}
