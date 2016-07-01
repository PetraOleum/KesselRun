CC=g++
LIBS=-lm -lSDL2 -lSDL2_gfx
CFLAGS=-Wall -Wextra -Werror -std=c++11 -Og
DEPS=kesselapp.h space.h
OBJ=main.o kesselapp.o space.o

all: kesselrun

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

kesselrun: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm -f *.o
	-rm -f kesselrun
