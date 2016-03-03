#include "characters.h"

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

int dz_1[2] = {1, -1};


Character::Character(int arow, int acol) : BaseObject(arow, acol) {
}

Character::Character(int arow, int acol, int hp, int dmg) : BaseObject(arow, acol), health(hp) {
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
		return false;
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
	if (chance(100, "dragon")) {
		row += dz_1[rand()%2];
		col += dz_1[rand()%2];
	}
	return false;
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
