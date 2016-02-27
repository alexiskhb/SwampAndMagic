#include <iostream>
#include <vector>

#define SYM_EMPTY    '.'
#define SYM_WALL     '#'
#define SYM_KNIGHT   'K'
#define SYM_PRINCESS 'P'
#define SYM_DRAGON   'D'
#define SYM_ZOMBIE   'z'
#define SYM_SWAMP    '~'
#define SYM_FLAME    '*'
#define SYM_MAGIC    '%'

#define CMD_QUIT   'Q'
#define CMD_UP     'w'
#define CMD_DOWN   's'
#define CMD_LEFT   'a'
#define CMD_RIGHT  'd'
#define CMD_ATTACK 'r'
#define CMD_FBALL  'f'
#define CMD_RANDOM 'r'

// first knight's turn, 
// then zombies and dragon can attack him.
// in the way, player sees results of last turn and takes next.
// initialization is the first result of 0-th turn

using namespace std;

bool game_over = false;


class Object {
public:
	Object() {
	}

	Object(int x_coord, int y_coord) : x(x_coord), y(y_coord) {
	}

	virtual char symbol() {
		return EMPTY;
	}

protected:
	int x = -1;
	int y = -1;
	// for lifetime -1 means infinity
	int lifetime = -1;
};


class Character : Object {
public:
	Character() : Object() {
	}

	Character(int x_coord, int y_coord) : Object(x_coord, y_coord) {
	}

	virtual void move();
	virtual int  hitpoints();
	virtual char attack();
	// returns True if Character died
	virtual bool suffer(int dmg);
protected:
	int health = 1;
	int damage = 0;
};


class Knight : Character {
	char symbol() {
		return KNIGHT;
	}

private:

};


class Princess : Character {
	char symbol() {
		return PRINCESS;
	}

};


class Monster : Character {

};


class Dragon : Monster {
	char symbol() {
		return DRAGON;
	}

};


class Zombie : Monster {
	char symbol() {
		return ZOMBIE;
	}
};


struct Map {
	void display() {

	}
	Object map[22][22];

};


int main(int argc, char** argv) {
	char turn;
 	while (!game_over) {
 		cin >> turn;
 		if (turn == QUIT) {
 			break;
 		}
 	}
}




