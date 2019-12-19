#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global;

int main(void)
{
  int mida;
  char str[10];

  int local, *vector;

  global = 0;
  local  = 0;
  vector = malloc(1000);

  printf("PID del proces: %d\n", getpid());
  printf("Funcio main: %lx\n", &main);
  printf("Variable global: %lx\n", &global);
  printf("Variable local: %lx\n", &local);
  printf("Variable vector: %lx\n", &vector);
  printf("Variable vector apunta a: %lx\n", vector);

  return 0;
}
