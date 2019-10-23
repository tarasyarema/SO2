#include "extreu-paraules.c"

int main(int argc, char **argv)
{
    FILE *fp;
    char line[MAXCHAR];
    
    if (argc != 2) {
        printf("%s <file>\n", argv[0]);
        exit(1);
    }
    
    fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Could not open file\n");
        exit(1);
    }
    
    while (fgets(line, MAXCHAR, fp))
        process_line(line);
    
    fclose(fp);

    return 0;
}