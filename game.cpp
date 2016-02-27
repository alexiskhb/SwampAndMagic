#include <iostream>
#include <vector>
#include <list>

#define cint const int

const char SYM_EMPTY    = ' ';
const char SYM_WALL     = '#';
const char SYM_KNIGHT   = 'K';
const char SYM_PRINCESS = 'P';
const char SYM_DRAGON   = 'D';
const char SYM_ZOMBIE   = 'z';
const char SYM_SWAMP    = '~';
const char SYM_FLAME    = '*';
const char SYM_MAGIC    = '%';

const char CMD_QUIT   = 'Q';
const char CMD_UP     = 'w';
const char CMD_DOWN   = 's';
const char CMD_LEFT   = 'a';
const char CMD_RIGHT  = 'd';
const char CMD_ATTACK = 'f';
const char CMD_MAGIC  = 'r';

const int MAP_SIZE    = 30; 

const int HP_KNIGHT   = 50; 
const int HP_PRINCESS = 2;  
const int HP_DRAGON   = 200;
const int HP_ZOMBIE   = 20; 

const int DMG_KN_SWORD = 7; 
const int DMG_KN_MAGIC = 6; 
const int DMG_PRINCESS = 0; 
const int DMG_DRAGON   = 10;
const int DMG_FIRE     = 4; 
const int DMG_ZOMBIE   = 3; 


// player sees results of latest turn and takes next.
// initialization is the first result of 0-th turn

// map is 4-connected area

using namespace std;


class Map;


class Object {
public:
	Object() {
	}

	Object(int arow, int acol) : row(arow), col(acol) {
	}

	// if two objects are NEAR but do not match
	bool operator%(const Object& obj) {
		return 
			(abs(row - obj.row) == 1 && abs(col - obj.col) == 0) ||
			(abs(row - obj.row) == 0 && abs(col - obj.col) == 1);
	}
	
	bool operator==(const Object& obj) {
		return row == obj.row && col == obj.col;
	}

	int getrow() {
		return row;
	}

	int getcol() {
		return col;
	}

	virtual char symbol() {
		return SYM_EMPTY;
	}	

	virtual bool is_permeable() {
		return true;
	}

	virtual bool is_enemy() {
		return true;
	}

protected:
	int row = -1;
	int col = -1;
	// for lifetime -1 means infinity
	int lifetime = -1;
	int damage = 0;
};


class Map {
public:
	Object* operator<<(Object* obj) {
		int row = obj->getrow();
		int col = obj->getcol();
		map[row][col].push_back(obj);
		return obj;
	}

	friend ostream& operator<<(ostream& display, Map& m) {
		for(int i = 0; i < MAP_SIZE; i++) {
			for(int j = 0; j < MAP_SIZE; j++) {
				display << m.map[i][j].back()->symbol();
			}
			display << endl;
		}
		return display;
	}

private:
	int width  = MAP_SIZE;
	int height = MAP_SIZE;
	list<Object*> map[MAP_SIZE][MAP_SIZE];
};


class Wall : public Object {
public:
	Wall(int arow, int acol) : Object(arow, acol) {

	}

	virtual char symbol() {
		return SYM_WALL;
	}

	virtual bool is_permeable() {
		return false;
	}
};


class Flame : public Object {
public:
	Flame(int arow, int acol) : Object(arow, acol) {
		lifetime = 6;
	}

	virtual char symbol() {
		return SYM_FLAME;
	}

	virtual bool is_permeable(){
		return true;
	}
};


class Swamp : public Object {
public:
	Swamp(int arow, int acol) : Object(arow, acol) {

	}

	virtual char symbol() {
		return SYM_SWAMP;
	}

	virtual bool is_permeable() {
		return true;
	}
};

// magic can heal knight
class Magic : public Object {
public:
	Magic(int arow, int acol) : Object(arow, acol) {
		lifetime = 5;
	}

	virtual char symbol() {
		return SYM_MAGIC;
	}

	virtual bool is_permeable() {
		return true;
	}
};


class Character : public Object {
public:
	Character(int arow, int acol) : Object(arow, acol) {
	}

	Character(int arow, int acol, int hp, int dmg) : Object(arow, acol), health(hp) {
		damage = dmg;
	}

	// returns True if character died
	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

	virtual void move() {

	}

	// all characters are impenetrable
	virtual bool is_permeable() {
		return false;
	}

protected:
	int health = 1;
};


class Knight : public Character {
public:
	Knight(int arow, int acol) : Character(arow, acol) {

	}

	virtual char symbol() {
		return SYM_KNIGHT;
	}

	virtual void move() {
		char action;
		cin >> action;
	}

	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

};


class Princess : public Character {
public:
	Princess(int arow, int acol) : Character(arow, acol) {

	}

	virtual char symbol() {
		return SYM_PRINCESS;
	}
	
	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

	virtual void move() {

	}
};


class Monster : public Character {
public:
	Monster(int arow, int acol) : Character(arow, acol) {

	}
};


class Dragon : public Monster {
public:
	Dragon(int arow, int acol) : Monster(arow, acol) {

	}

	virtual char symbol() {
		return SYM_DRAGON;
	}

	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

	virtual void move() {

	}
};


class Zombie : public Monster {
public:
	Zombie(int arow, int acol) : Monster(arow, acol) {

	}

	virtual char symbol() {
		return SYM_ZOMBIE;
	}

	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

	virtual void move() {

	}
};


struct {
	list<Character*> characters;
	list<Object*> objects;
	list<Object*> empties;
	Map map;

	bool is_over() {
		return false;
	}

	void draw_objects() {

	}

	void draw_characters() {

	}

	void next_turn() {

	}

	void render() {
		cout << map << endl;
	}

	void init() {
		for(int i = 0; i < MAP_SIZE; i++) {
			for(int j = 0; j < MAP_SIZE; j++) {
				empties.push_back(new Object(i, j));
				map << empties.back();
			}
		}
		for(int i = 0; i < MAP_SIZE; i++) {
			objects.push_back(new Wall(0, i));
			map << objects.back();
			objects.push_back(new Wall(MAP_SIZE-1, i));
			map << objects.back();
			objects.push_back(new Wall(i, 0));
			map << objects.back();
			objects.push_back(new Wall(i, MAP_SIZE-1));
			map << objects.back();
		}
		characters.push_back(new Knight(MAP_SIZE-2, 1));
		map << characters.back();
		characters.push_back(new Princess(1, MAP_SIZE-2));
		map << characters.back();
		characters.push_back(new Dragon(4, MAP_SIZE-4));
		map << characters.back();
	}
} Game;


int main(int argc, char** argv) {
	Game.init();
	Game.render();
 	while (Game.is_over()) {
 		Game.next_turn();
 	}
}




