#include "game_objects.h"

#define chance(a) (rand()%1000 + 1 <= 10*a)

static const char*STR_MOVES  = "wasd"; 
static const char CMD_UP     = 'w';
static const char CMD_DOWN   = 's';
static const char CMD_LEFT   = 'a';
static const char CMD_RIGHT  = 'd';
static const char CMD_ATTACK = 'f';
static const char CMD_MAGIC  = 'r';
static const char CMD_NONE   = 'n';
static const char CMD_QUIT   = 'Q';

using namespace std;





Object::Object() {

}

Object::Object(int arow, int acol) : row(arow), col(acol), prev_row(arow), prev_col(acol) {
	
}

Object::~Object() {

}

// if two objects are NEAR but do not match
bool Object::operator%(const Object& obj) {
	return 
		(abs(row - obj.row) == 1 && abs(col - obj.col) == 0) ||
		(abs(row - obj.row) == 0 && abs(col - obj.col) == 1);
}

bool Object::operator==(const Object& obj) {
	return row == obj.row && col == obj.col;
}

int Object::getrow() {
	return row;
}

int Object::getcol() {
	return col;
}

int Object::prevrow() {
	return prev_row;
}

int Object::prevcol() {
	return prev_col;
}

void Object::move_to_prev() {
	row = prev_row;
	col = prev_col;
}

char Object::symbol() {
	return SYM_EMPTY;
}	

bool Object::is_penetrable() {
	return true;
}

bool Object::is_evil() {
	return true;
}





ObjectPtr Map::operator<<(ObjectPtr obj) {
	int row = obj->getrow();
	int col = obj->getcol();
	map[row][col].push_back(obj);
	return obj;
}

ostream& operator<<(ostream& display, Map& m) {
	for(int i = 0; i < m.get_height(); i++) {
		for(int j = 0; j < m.get_width(); j++) {
			display << m[i][j].back()->symbol();
		}
		display << endl;
	}
	return display;
}

list<ObjectPtr>* Map::operator[](int index) {
	return map[index];
}

int Map::get_height() {
	return height;
}

int Map::get_width() {
	return width;
}	

bool Map::is_penetrable(int arow, int acol) {
	bool is_p = true;
	for(auto obj: map[arow][acol]) {
		is_p &= obj->is_penetrable();
	}
	return is_p;
}





Wall::Wall(int arow, int acol) : Object(arow, acol) {

}

Wall::~Wall() {

}

char Wall::symbol() {
	return SYM_WALL;
}

bool Wall::is_penetrable() {
	return false;
}





Flame::Flame(int arow, int acol) : Object(arow, acol) {
	lifetime = 6;
}

Flame::~Flame() {

}

char Flame::symbol() {
	return SYM_FLAME;
}

bool Flame::is_penetrable(){
	return true;
}





Swamp::Swamp(int arow, int acol) : Object(arow, acol) {

}

Swamp::~Swamp() {

}

char Swamp::symbol() {
	return SYM_SWAMP;
}

bool Swamp::is_penetrable() {
	return true;
}





// magic can heal knight
Magic::Magic(int arow, int acol) : Object(arow, acol) {
	lifetime = 5;
}

Magic::~Magic() {

}

char Magic::symbol() {
	return SYM_MAGIC;
}

bool Magic::is_penetrable() {
	return true;
}





Character::Character(int arow, int acol) : Object(arow, acol) {
}

Character::Character(int arow, int acol, int hp, int dmg) : Object(arow, acol), health(hp) {
	damage = dmg;
}

Character::~Character() {

}

void Character::slash(list<CharacterPtr>& characters, CharacterPtr self) {
	for(auto ch: characters) {
		if (ch != self && *ch % *self) {
			ch->suffer(damage);
		}
	}
}

bool Character::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, CharacterPtr self) {
	return true;
}

int Character::hitpoints() {
	return health;
}

// returns True if character died
bool Character::suffer(int dmg) {
	return (health -= dmg) <= 0;
}

// all characters are impenetrable
bool Character::is_penetrable() {
	return false;
}





Knight::Knight(int arow, int acol) : Character(arow, acol) {

}

Knight::Knight(int arow, int acol, int hp, int dmg) : Character(arow, acol, hp, dmg) {

}

Knight::~Knight() {
	cout << ">> Knight died\n";
}

char Knight::symbol() {
	return SYM_KNIGHT;
}

bool Knight::is_evil() {
	return false;
}

void Knight::magic(char direction) {

}

bool Knight::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, CharacterPtr self) {
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
		case CMD_QUIT: {
			exit(0);
		}
	}
	return true;
}

bool Knight::move() {
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

bool Knight::suffer(int dmg) {
	return (health -= dmg) <= 0;
}





Princess::Princess(int arow, int acol) : Character(arow, acol) {

}

Princess::Princess(int arow, int acol, int hp, int dmg) : Character(arow, acol, hp, dmg) {

}

Princess::~Princess() {
	cout << ">> Princess died\n";
}

char Princess::symbol() {
	return SYM_PRINCESS;
}

bool Princess::suffer(int dmg) {
	return (health -= dmg) <= 0;
}

bool Princess::move() {
	prev_row = row;
	prev_col = col;
	return false;
}





Monster::Monster(int arow, int acol) : Character(arow, acol) {

}

Monster::Monster(int arow, int acol, int hp, int dmg) : Character(arow, acol, hp, dmg) {

}

Monster::~Monster() {

}





Dragon::Dragon(int arow, int acol) : Monster(arow, acol) {

}

Dragon::Dragon(int arow, int acol, int hp, int dmg) : Monster(arow, acol, hp, dmg) {

}

Dragon::~Dragon() {
	cout << "Dragon died\n";
}

void Dragon::magic() {

}

bool Dragon::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, CharacterPtr self) {
	CharacterPtr knight = characters.front();
	if (*self % *knight) {
		slash(characters, self);
		return true;
	}
	if (abs(knight->getrow() - row) < 10 && abs(knight->getcol() - col) < 10) {
		magic();
		return chance(40);
	}
	return false;
}

char Dragon::symbol() {
	return SYM_DRAGON;
}

bool Dragon::suffer(int dmg) {
	return (health -= dmg) <= 0;
}

bool Dragon::move() {
	prev_row = row;
	prev_col = col;
	return true;
}





Zombie::Zombie(int arow, int acol) : Monster(arow, acol) {

}

Zombie::Zombie(int arow, int acol, int hp, int dmg) : Monster(arow, acol, hp, dmg) {

}

Zombie::~Zombie() {
	cout << "Zombie died\n";
}

char Zombie::symbol() {
	return SYM_ZOMBIE;
}

bool Zombie::suffer(int dmg) {
	return (health -= dmg) <= 0;
}

bool Zombie::move() {
	prev_row = row;
	prev_col = col;
	return true;
}
