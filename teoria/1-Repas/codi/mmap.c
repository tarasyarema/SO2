#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
  struct stat st;

  int i, fd, len;
  char *file_memory;
  
  stat(argv[1], &st);
  len = st.st_size;
  
  fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
  
  file_memory = mmap(0, len, PROT_READ | PROT_WRITE, 
		     MAP_SHARED, fd, 0);
  
  close(fd);
  
  for(i = 0; i < len; i++)
    if (file_memory[i] == 'e')
      file_memory[i] = 'o';
    
  munmap(file_memory, len);
}
