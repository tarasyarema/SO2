#pragma once

#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "config.h"

typedef struct file_name_
{
    char key[MAX_CHARS];     
    sem_t sem;
    int done;
} s_file_name; 

char *dbfnames_to_mmap(FILE *fp_db);
char *_dbfnames_to_mmap(FILE *fp_db);
void dbfnames_munmmap(char *mmap);
void _dbfnames_munmmap(char *mmap);
char *get_dbfname_from_mmap(char *mmap, int i);
char *_get_dbfname_from_mmap(char *mmap, int i);