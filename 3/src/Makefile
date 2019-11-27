FILES_C = main.c red-black-tree.c
TARGET = practica3

CFLAGS = -Wall -Werror -O -g
LFLAGS = -lm

FILES_O = $(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) Makefile
	gcc $(FILES_O) -o $(TARGET) $(LFLAGS) 

%.o: %.c Makefile
	gcc $(CFLAGS) -c $<

all: $(TARGET) 

clean:
	rm $(FILES_O) $(TARGET) 