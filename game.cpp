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


class Object {
public:
	Object() {
	}

	Object(int x_coord, int y_coord) : x(x_coord), y(y_coord) {
	}

	bool near(const Object& obj) {
		return 
			(abs(x - obj.x) == 1 && abs(y - obj.y) == 0) ||
			(abs(x - obj.x) == 0 && abs(y - obj.y) == 1);
	}
	
	bool operator==(const Object& obj) {
		return x == obj.x && y == obj.y;
	}

	virtual char symbol() {
		return SYM_EMPTY;
	}	

	virtual bool is_permeable() {
		return false;
	}

protected:
	int x = -1;
	int y = -1;
	// for lifetime -1 means infinity
	int lifetime = -1;
};


class Map {
public:
	void display() {
	}
	Object map[22][22];
};


class Wall : public Object {
public:
	Wall() {

	}

	Wall(int x_coord, int y_coord) : Object(x_coord, y_coord) {

	}

	char symbol() {
		return SYM_WALL;
	}

	bool is_permeable() {
		return false;
	}

};


class Flame : public Object {
public:
	char symbol() {
		return SYM_FLAME;
	}

	bool is_permeable(){
		return true;
	}
};


class Swamp : public Object {
public:
	char symbol() {
		return SYM_SWAMP;
	}

	bool is_permeable() {
		return true;
	}
};

// magic can heal
class Magic : public Object {
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

class Character : public Object {
public:
	Character() {
	}

	Character(int x_coord, int y_coord) : Object(x_coord, y_coord) {
	}

	Character(int x_coord, int y_coord, int hp, int dmg) : Object(x_coord, y_coord), health(hp), damage(dmg) {
	}

	// virtual int  hitpoints();
	// virtual char attack();

	// returns True if Character died
	// virtual bool suffer(int dmg);

	bool is_permeable() {
		return false;
	}

protected:
	int health = 1;
	int damage = 0;
};


class Knight : public Character {
public:
	Knight(int x_coord, int y_coord) : Character(x_coord, y_coord) {

	}

	char symbol() {
		return SYM_KNIGHT;
	}

	void move() {
		char action;
		cin >> action;
	}

};


class Princess : public Character {
public:
	Princess(int x_coord, int y_coord) : Character(x_coord, y_coord) {

	}

	char symbol() {
		return SYM_PRINCESS;
	}

};


class Monster : public Character {
public:
	Monster(int x_coord, int y_coord) : Character(x_coord, y_coord) {

	}
};


class Dragon : public Monster {
public:
	Dragon(int x_coord, int y_coord) : Monster(x_coord, y_coord) {

	}

	char symbol() {
		return SYM_DRAGON;
	}
};


class Zombie : public Monster {
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


char symb(Object& o) {
	return o.symbol();
}


int main(int argc, char** argv) {
	Knight k(1, 2);
	Wall w(1, 3);
	Dragon d(1, 2);
	// Object* ok = &k;
	Object* ow = &w;
	Object* od = &d;
	cout << symb(k) << ' ' << ow->symbol() << ' ' << od->symbol() << endl;
	// Character c = static_cast<Character>(k);
	// Object o1 = static_cast<Object>(k);
	// Object o2 = static_cast<Object>(w);

	cout << (k == w) << endl;
	cout << (w == d) << endl;
	cout << (d == k) << endl;

 	while (!Game.is_over()) {
 		Game.next_turn();
 	}
}




