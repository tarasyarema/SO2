# This is the makefile that generates the executable

# Files to compile
FILES_C = main.c red-black-tree.c
_FILES_C = test.c red-black-tree.c

# Exectuable to generate
TARGET = main
_TARGET = test

# Compilation options
CFLAGS = -Wall -Werror -O -g

# Linker options 
LFLAGS = -lm

# There is no need to change the instructions below this
# line. Change if you really know what you are doing.

FILES_O = $(subst .c,.o,$(FILES_C))
_FILES_O = $(subst .c,.o,$(_FILES_C))

$(TARGET): $(FILES_O) Makefile
	gcc $(FILES_O) -o $(TARGET) $(LFLAGS) 

%.o: %.c Makefile
	gcc $(CFLAGS) -c $<

all: $(TARGET) 


$(_TARGET): $(_FILES_O) Makefile
	gcc $(_FILES_O) -o $(_TARGET) $(LFLAGS) 

%.o: %.c Makefile
	gcc $(CFLAGS) -c $<

_all: $(_TARGET) 

clean:
	rm $(_FILES_O) $(_TARGET) $(_FILES_O) $(_TARGET) 