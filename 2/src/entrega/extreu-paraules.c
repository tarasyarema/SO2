#include "red-black-tree.h"

void debug_str(char *p)
{
   fprintf(stderr, "%p: (%ld) %s\n", p, strlen(p), p);
}

/**
 * 
 * Given a line, extract the words it contains and print them to screen
 * 
 */

void process_line(rb_tree *tree, char *line)
{
    int j, is_word, len_line, i = 0;
    char *paraula, *str;

    node_data *n_data;

    len_line = strlen(line);
    paraula = malloc(len_line * sizeof(char));

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i]))))
        i++;

    while (i < len_line)
    {
        j = 0;
        is_word = 1;

        do
        {
            if (isalpha(line[i]))
                paraula[j] = line[i];
            else
                is_word = 0;

            j++;
            i++;

        } while ((i < len_line) && (!isspace(line[i])) && (!ispunct(line[i])));

        if (is_word)
        {
            paraula[j] = 0;

            str = malloc((strlen(paraula) + 1) * sizeof(char));
            strcpy(str, paraula);
            
            n_data = find_node(tree, str);

            if (n_data != NULL)
            {
                n_data->num_times++;
            }

            free(str);
        }

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i]))))
            i++;
    }

    free(paraula);
}
