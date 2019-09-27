#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int i, fd, num_int;
  char s[10];

  if (argc != 3) {
    printf("%s <file> <number_ints>\n", argv[0]);
    exit(1);
  }

  num_int = atoi(argv[2]);
  if (num_int <= 0) {
    printf("Not valid number of integers\n");
    exit(1);
  }
 
  fd = open(argv[1], 
        O_WRONLY | O_CREAT | O_TRUNC, 
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (fd < 0) {
    printf("Could not open file\n");
    exit(1);
  }

  int written_bytes = 0;

  for(i = 0; i < num_int; i++){
    written_bytes += write(fd, &i, sizeof(int));
  }

  off_t fsize;
  fsize = lseek(fd, 0, SEEK_END);
  printf("Size of file descriptor: %ld\n", fsize);

  printf("Written bytes: %d\n", written_bytes);
  printf("Check the file! %d integers have been written.\n", num_int);
  printf("Press Enter to close the file\n");
  fgets(s, 10, stdin);

  printf("Closing the file\n");

  close(fd);

  return 0;
}
