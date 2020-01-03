FILES_C_TEST = test.c red-black-tree.c
FILES_C = main.c red-black-tree.c

TARGET_TEST = test
TARGET = practica5

CFLAGS = -Wall -Werror -O -g
LFLAGS = -lm -lpthread

FILES_O_TEST = $(subst .c,.o,$(FILES_C_TEST))
FILES_O = $(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) Makefile
	gcc $(FILES_O) -o $(TARGET) $(LFLAGS) 

%.o: %.c Makefile
	gcc $(CFLAGS) -c $<

all: $(TARGET) 

clean:
	rm --force $(FILES_O) $(TARGET) 

$(TARGET_TEST): $(FILES_O_TEST) Makefile
	gcc $(FILES_O_TEST) -o $(TARGET_TEST) $(LFLAGS) 

test: $(TARGET_TEST) 

test_clean:
	rm --force $(FILES_O_TEST) $(TARGET_TEST) 