#include "config.h"
#include "red-black-tree.h"
#include "dbfnames-mmap.h"


void init_tree_mmap(rb_tree *tree, char *pointer)
{
    node_data *node;

    char *tmp_ptr = pointer;
    int total = *((int *)tmp_ptr);

    tmp_ptr += sizeof(int);

    for (int i=0; i < total / sizeof(node_data); i++)
    {
        node = (node_data *)tmp_ptr;

        insert_node(tree, node);
        tmp_ptr += sizeof(node_data);
    }
}

// Linear time search in an mmaped array

void search_node_mmap(char *pointer, const char *str)
{
    node_data *node;
    char *tmp_ptr = pointer;
    int total = *((int *)tmp_ptr);

    tmp_ptr += sizeof(int);
    
    for (int i=0; i < total / sizeof(node_data); i++)
    {
        node = (node_data *)tmp_ptr;

        if (strcasecmp(node->key, str) == 0)
        {
            printf("%d: %p -> (%s, %d)\n", i + 1, node, node->key, node->num_times);
            break;
        }

        tmp_ptr += sizeof(node_data);
    }
}

void update_node_mmap(char *pointer, const char *str)
{
    node_data *node;
    char *tmp_ptr = pointer;
    int total = *((int *)tmp_ptr);
    
    tmp_ptr += sizeof(int);
    
    for (int i=0; i < total / sizeof(node_data); i++)
    {
        node = (node_data *)tmp_ptr;

        if (strcasecmp(node->key, str) == 0)
        {
            node->num_times += 1;
            break;
        }

        tmp_ptr += sizeof(node_data);
    }
}

// Print the mmaped tree

void print_tree_mmap(char *pointer)
{
    node_data *node;
    char *tmp_ptr = pointer;
    int total = *((int *)tmp_ptr) / sizeof(node_data);
    int p = 0;

    tmp_ptr += sizeof(int);
    
    for (int i=0; i < total; i++)
    {
        node = (node_data *)tmp_ptr;
        
        printf("%d: (%p, %p) -> (%s, %d)\n", i + 1, tmp_ptr, node, node->key, node->num_times);
        
        tmp_ptr += sizeof(node_data);
        ++p;
    }
}

// Print the mmaped db

void print_db_mmap(char *pointer)
{
    s_file_name *file;
    char *tmp_ptr = pointer;

    int total = *((int *)tmp_ptr);
    tmp_ptr += sizeof(int);
    
    for (int i=0; i < total; i++)
    {
        file = (s_file_name *)tmp_ptr;
        
        printf("%d: (%p) %s\n", i + 1, tmp_ptr, file->key);
        
        tmp_ptr += sizeof(s_file_name);
    }
}