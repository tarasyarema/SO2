#pragma once

#include "red-black-tree.h"

#define MAGIC_NUMBER 0x01234567
#define MAX_CHARS 100
#define MAX_CHARS_LINE 1024
#define MAX_THREADS 500
#define DEBUG 1
#define DEBUG_TIME 0
#define DEBUG_TREE 0

typedef struct _file
{
    char name[MAX_CHARS];
    pthread_mutex_t mutex;
    int read;
} file;

typedef struct _args
{
    rb_tree *tree;
    file *files;
    int num_files;
} arguments;

