.PHONY: clean all

NAME=game

CC=g++

CFLAGS=-c -Wall -std=c++11
DBGFLAGS= -g -O3

LIBS=

SRC=$(NAME).cpp base_objects.cpp objects.cpp characters.cpp

OBJ=$(SRC:.cpp=.o)

all: $(NAME)

game: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(NAME)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm -rf *.o