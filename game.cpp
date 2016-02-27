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
#define CMD_ATTACK 'f'
#define CMD_MAGIC  'r'

// first knight's turn, 
// then zombies and dragon can attack him.
// in the way, player sees results of last turn and takes next.
// initialization is the first result of 0-th turn

using namespace std;

bool game_over = false;


class Map {
public:
	void display() {
	}
	Object map[22][22];
};


class Object {
public:
	Object() {
	}

	Object(int x_coord, int y_coord) : x(x_coord), y(y_coord) {
	}

	virtual char symbol() {
		return SYM_EMPTY;
	}

	virtual bool is_permeable();

protected:
	int x = -1;
	int y = -1;
	// for lifetime -1 means infinity
	int lifetime = -1;
};


class Wall : Object {
public:
	bool is_permeable() {
		return false;
	}

};


class Flame : Object {
public:
	bool is_permeable(){
		return true;
	}
};


class Swamp : Object {
public:
	char symbol() {
		return SYM_SWAMP;
	}

	bool is_permeable() {
		return true;
	}

};

// magic can heal
class Magic : Object {
public:
	Magic() {
		lifetime = 5;
	}

	Magic(int x_coord, int y_coord) : Object(x_coord, y_coord) {
		lifetime = 5;
	}

	char symbol() {
		return SYM_MAGIC;
	}

	bool is_permeable() {
		return true;
	}

};

class Character : Object {
public:
	Character() {
	}

	Character(int x_coord, int y_coord) : Object(x_coord, y_coord) {
	}

	Character(int x_coord, int y_coord, int hp, int dmg) : Object(x_coord, y_coord), health(hp), damage(dmg) {
	}

	virtual void move();
	virtual int  hitpoints();
	virtual char attack();
	// returns True if Character died
	virtual bool suffer(int dmg);

	bool is_permeable() {
		return false;
	}

protected:
	int health = 1;
	int damage = 0;
};


class Knight : Character {
public:
	char symbol() {
		return SYM_KNIGHT;
	}

	void move() {
		char action;
		cin >> action;
	}

};


class Princess : Character {
	char symbol() {
		return SYM_PRINCESS;
	}

};


class Monster : Character {

};


class Dragon : Monster {
	char symbol() {
		return SYM_DRAGON;
	}
};


class Zombie : Monster {
	char symbol() {
		return SYM_ZOMBIE;
	}
};


struct {
	vector<Character> characters;
	vector<Object>    objects;

	bool is_over() {
		return false;
	}

	void draw_objects() {

	}

	void draw_characters() {

	}

	void next_turn() {

	}

} Game;


int main(int argc, char** argv) {
 	while (!Game.is_over()) {
 		Game.next_turn();
 	}
}




