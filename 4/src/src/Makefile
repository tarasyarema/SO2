FILES_C = test.c red-black-tree.c dbfnames-mmap.c tree-to-mmap.c
TARGET = test

CFLAGS = -Wall -Werror -O -g
LFLAGS = -lm -pthread

FILES_O = $(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) Makefile
	gcc $(FILES_O) -o $(TARGET) $(LFLAGS) 

%.o: %.c Makefile
	gcc $(CFLAGS) -c $<

all: $(TARGET) 

clean:
	rm $(FILES_O) $(TARGET) 
