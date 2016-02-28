.PHONY: clean all

NAME=game

CC=g++

CFLAGS=-c -Wall -std=c++0x

LIBS=

SRC=$(NAME).cpp $(NAME)_objects.cpp

OBJ=$(SRC:.cpp=.o)

all: $(NAME)	

game: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(NAME)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm -rf *.o