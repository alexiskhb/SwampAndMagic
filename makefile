.PHONY: clean all

NAME=game

CC=g++

CFLAGS=-c -Wall -std=c++11

LIBS=

SRC=$(NAME).cpp

OBJ=$(SRC:.cpp=.o)

all: $(NAME)	

game: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(NAME)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm -rf *.o