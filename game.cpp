#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <memory>


const char SYM_EMPTY    = ' ';
const char SYM_WALL     = '#';
const char SYM_KNIGHT   = 'K';
const char SYM_PRINCESS = 'P';
const char SYM_DRAGON   = 'D';
const char SYM_ZOMBIE   = 'z';
const char SYM_SWAMP    = '~';
const char SYM_FLAME    = '*';
const char SYM_MAGIC    = '%';

const char*STR_MOVES  = "wasd"; 
const char CMD_UP     = 'w';
const char CMD_DOWN   = 's';
const char CMD_LEFT   = 'a';
const char CMD_RIGHT  = 'd';
const char CMD_ATTACK = 'f';
const char CMD_MAGIC  = 'r';
const char CMD_NONE   = 'n';
const char CMD_QUIT   = 'Q';

const int MAP_HEIGHT  = 30; 
const int MAP_WIDTH   = 60; 

const int HP_KNIGHT   = 50; 
const int HP_PRINCESS = 2;  
const int HP_DRAGON   = 200;
const int HP_ZOMBIE   = 20; 

const int DMG_KN_SWORD = 7*10; 
const int DMG_KN_MAGIC = 6*10; 
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

	Object(int arow, int acol) : row(arow), col(acol), prev_row(arow), prev_col(acol) {
	}

	virtual ~Object() {

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

	int prevrow() {
		return prev_row;
	}

	int prevcol() {
		return prev_col;
	}

	void move_to_prev() {
		row = prev_row;
		col = prev_col;
	}

	virtual char symbol() {
		return SYM_EMPTY;
	}	

	virtual bool is_penetrable() {
		return true;
	}

	virtual bool is_evil() {
		return true;
	}

protected:
	int row = -1;
	int col = -1;
	// we need prev_coords to remove reference to object
	// from previous cell of map after move
	int prev_row = -1;
	int prev_col = -1;
	// for lifetime -1 means infinity
	int lifetime = -1;
	int damage = 0;
};

typedef std::shared_ptr<Object> ObjectPtr;

class Map {
public:
	ObjectPtr operator<<(ObjectPtr obj) {
		int row = obj->getrow();
		int col = obj->getcol();
		map[row][col].push_back(obj);
		return obj;
	}

	friend ostream& operator<<(ostream& display, Map& m) {
		for(int i = 0; i < MAP_HEIGHT; i++) {
			for(int j = 0; j < MAP_WIDTH; j++) {
				display << m.map[i][j].back()->symbol();
			}
			display << endl;
		}
		return display;
	}

	list<ObjectPtr>* operator[](int index) {
		return map[index];
	}

	bool is_penetrable(int arow, int acol) {
		bool is_p = true;
		for(auto obj: map[arow][acol]) {
			is_p &= obj->is_penetrable();
		}
		return is_p;
	}

private:
	list<ObjectPtr> map[MAP_HEIGHT][MAP_WIDTH];
};


class Wall : public Object {
public:
	Wall(int arow, int acol) : Object(arow, acol) {

	}

	virtual ~Wall() {

	}

	virtual char symbol() {
		return SYM_WALL;
	}

	virtual bool is_penetrable() {
		return false;
	}
};


class Flame : public Object {
public:
	Flame(int arow, int acol) : Object(arow, acol) {
		lifetime = 6;
	}

	virtual ~Flame() {

	}

	virtual char symbol() {
		return SYM_FLAME;
	}

	virtual bool is_penetrable(){
		return true;
	}
};


class Swamp : public Object {
public:
	Swamp(int arow, int acol) : Object(arow, acol) {

	}

	virtual ~Swamp() {

	}

	virtual char symbol() {
		return SYM_SWAMP;
	}

	virtual bool is_penetrable() {
		return true;
	}
};

// magic can heal knight
class Magic : public Object {
public:
	Magic(int arow, int acol) : Object(arow, acol) {
		lifetime = 5;
	}

	virtual ~Magic() {

	}

	virtual char symbol() {
		return SYM_MAGIC;
	}

	virtual bool is_penetrable() {
		return true;
	}
};

class Character;
typedef std::shared_ptr<Character> CharacterPtr;

class Character : public Object {
public:
	Character(int arow, int acol) : Object(arow, acol) {
	}

	Character(int arow, int acol, int hp, int dmg) : Object(arow, acol), health(hp) {
		damage = dmg;
	}

	virtual ~Character() {

	}

	void slash(list<CharacterPtr>& characters, CharacterPtr self) {
		for(auto ch: characters) {
			if (ch != self && *ch%*self) {
				ch->suffer(damage);
			}
		}
	}

	virtual bool attack(list<CharacterPtr>& characters, CharacterPtr self) {
		return true;
	}

	virtual int hitpoints() {
		return health;
	}

	// returns True if character died
	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

	virtual bool move()=0;

	// all characters are impenetrable
	virtual bool is_penetrable() {
		return false;
	}

protected:
	int health = 1;
};


class Knight : public Character {
public:
	Knight(int arow, int acol) : Character(arow, acol) {

	}

	Knight(int arow, int acol, int hp, int dmg) : Character(arow, acol, hp, dmg) {

	}

	virtual ~Knight() {

	}

	virtual char symbol() {
		return SYM_KNIGHT;
	}

	virtual bool is_evil() {
		return false;
	}

	void magic(char direction) {

	}

	virtual bool attack(list<CharacterPtr>& characters, CharacterPtr self) {
		std::string moves(STR_MOVES);
		moved_on_attack = CMD_NONE;
		char action;
		cin >> action;
		// condition means player wants to move
		if (moves.find(action) != std::string::npos) {
			moved_on_attack = action;
			return false;
		}	
		switch (action) {
			case CMD_ATTACK: {
				slash(characters, self);
				return true;	
			}
			case CMD_MAGIC: {
				char direction;
				cin >> direction;
				magic(direction);
				return true;
			}
		}
		return true;
	}

	virtual bool move() {
		prev_row = row;
		prev_col = col;
		char action;
		if (moved_on_attack == CMD_NONE) {
			cin >> action;
		}
		else {
			action = moved_on_attack;
		}
		switch (action) {
			case CMD_UP: {
				--row;
				return true;	
			}
			case CMD_DOWN: {
				++row;
				return true;
			}
			case CMD_LEFT: {
				--col;
				return true;
			}
			case CMD_RIGHT: {
				++col;
				return true;
			}
		}
		return false;
	}

	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}
private:
	char moved_on_attack;
};


class Princess : public Character {
public:
	Princess(int arow, int acol) : Character(arow, acol) {

	}

	Princess(int arow, int acol, int hp, int dmg) : Character(arow, acol, hp, dmg) {

	}

	virtual ~Princess() {

	}

	virtual char symbol() {
		return SYM_PRINCESS;
	}
	
	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

	virtual bool move() {
		prev_row = row;
		prev_col = col;
		return false;
	}
};


class Monster : public Character {
public:
	Monster(int arow, int acol) : Character(arow, acol) {

	}

	Monster(int arow, int acol, int hp, int dmg) : Character(arow, acol, hp, dmg) {

	}

	virtual ~Monster() {

	}
};


class Dragon : public Monster {
public:
	Dragon(int arow, int acol) : Monster(arow, acol) {

	}

	Dragon(int arow, int acol, int hp, int dmg) : Monster(arow, acol, hp, dmg) {

	}

	virtual ~Dragon() {

	}

	virtual char symbol() {
		return SYM_DRAGON;
	}

	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

	virtual bool move() {
		prev_row = row;
		prev_col = col;
		return true;
	}
};


class Zombie : public Monster {
public:
	Zombie(int arow, int acol) : Monster(arow, acol) {

	}

	Zombie(int arow, int acol, int hp, int dmg) : Monster(arow, acol, hp, dmg) {

	}

	virtual ~Zombie() {

	}

	virtual char symbol() {
		return SYM_ZOMBIE;
	}

	virtual bool suffer(int dmg) {
		return (health -= dmg) <= 0;
	}

	virtual bool move() {
		prev_row = row;
		prev_col = col;
		return true;
	}
};

typedef std::map<CharacterPtr, bool> CharacterBoolMap;
typedef std::pair<CharacterPtr, bool> CharacterBoolPair;

// struct Game
struct {
	// CHARACTERS
	// [0] : player(knight)
	// [1] : princess
	// [2] : dragon
	// [3+]: zombies and other(if any)
	list<CharacterPtr> characters;
	list<ObjectPtr> objects;
	list<ObjectPtr> empties;
	Map map;

	bool is_over() {
		return false;
	}

	void refresh_characters_objects() {
		for(auto obj: objects) {
			map[obj->prevrow()][obj->prevcol()].remove(obj);
			map << obj;	
		}
		for(auto ch: characters) {
			map[ch->prevrow()][ch->prevcol()].remove(ch);
			map << ch;
		}
		for(auto ch: characters) {
			if (ch->hitpoints() <= 0) {
				map[ch->getrow()][ch->getcol()].remove(ch);
			}
		}
		characters.remove_if([](CharacterPtr ch) { 
			return ch->hitpoints() <= 0;
		});

	}

	void next_turn() {
		CharacterBoolMap did_attack;
		for(auto ch: characters) {
			did_attack.insert(CharacterBoolPair(ch, ch->attack(characters, ch)));
		}
		for(auto ch: characters) {
			if (!did_attack[ch]) {
				ch->move();
				// changes will be accepted after calling refresh_characters_objects()
				// so we can cancel try to move on wall here. prev_coords now match with actual.
				// there is no check for case when two or more characters going to
				// step on the same cell, I know
				if (!map.is_penetrable(ch->getrow(), ch->getcol())) {
					ch->move_to_prev();
				}
			}
		}

		refresh_characters_objects();
	}

	inline void render() {
		cout << map;
		cout << "HP: " << characters.front()->hitpoints() << "\n"; 
		cout << "enemies cnt: " << characters.size()-2 << "\n\n";
	}

	void generate_level() {
		objects.push_back(ObjectPtr(new Wall(MAP_HEIGHT/2, MAP_WIDTH/2)));
		map << objects.back();
	}

	void init() {
		for(int i = 0; i < MAP_HEIGHT; i++) {
			for(int j = 0; j < MAP_WIDTH; j++) {
				empties.push_back(ObjectPtr(new Object(i, j)));
				map << empties.back();
			}
		}
		for(int i = 0; i < MAP_HEIGHT; i++) {
			objects.push_back(ObjectPtr(new Wall(i, 0)));
			map << objects.back();
			objects.push_back(ObjectPtr(new Wall(i, MAP_WIDTH-1)));
			map << objects.back();
		}
		for(int j = 0; j < MAP_WIDTH; j++) {
			objects.push_back(ObjectPtr(new Wall(0, j)));
			map << objects.back();
			objects.push_back(ObjectPtr(new Wall(MAP_HEIGHT-1, j)));
			map << objects.back();
		}
		generate_level();

		characters.push_back(CharacterPtr(new Knight(MAP_HEIGHT-2, 1, HP_KNIGHT, DMG_KN_SWORD)));
		map << characters.back();
		characters.push_back(CharacterPtr(new Princess(1, MAP_WIDTH-2, HP_PRINCESS, DMG_PRINCESS)));
		map << characters.back();
		characters.push_back(CharacterPtr(new Dragon(4, MAP_WIDTH-4, HP_DRAGON, DMG_DRAGON)));
		map << characters.back();
	}
} Game;


int main(int argc, char** argv) {
	Game.init();
	Game.render();
 	while (!Game.is_over()) {
 		Game.next_turn();
 		Game.render();
 	}
 	return 0;
}
