#include "characters.h"
#include "objects.h"
#include <curses.h>

static const char* STR_MOVES  = "wasdqezcx"; 

using namespace std;

int dz_1[3] = {-1, 0, 1};
int dz_2[5] = {-2, -1, 0, 1, 2};
int dz_3[7] = {-3, -2, -1, 0, 1, 2, 3};


Character::Character(int arow, int acol) : BaseObject(arow, acol) {
	fcolor = Colored(BG_BLACK, FG_WHITE).to_string();
}

Character::Character(int arow, int acol, int hp, int dmg) : BaseObject(arow, acol) {
	damage = dmg;
	health = hp;
	fcolor = Colored(BG_BLACK, FG_WHITE).to_string();
}

Character::~Character() {

}

void Character::slash(list<CharacterPtr>& characters) {
	for(auto ch: characters) {
		if (!(*ch == *this) && *ch % *this) {
			ch->suffer(damage);
		}
	}
}

bool Character::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	return false;
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
	fcolor = Colored(BG_GREEN, FG_WHITE).to_string();
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

void Knight::magic(list<ObjectPtr>& objects, char direction) {
	int consts[5] = {0, 0, 0, 0, 0};
	int di_inc[5] = {1, 2, 3, 4, 5};
	int di_dec[5] = {-1, -2, -3, -4, -5};
	int dj_inc[5] = {1, 2, 3, 4, 5};
	int dj_dec[5] = {-1, -2, -3, -4, -5};
	int di_around[16] = {-2, -2, -2, -2, -2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1};
	int dj_around[16] = {-2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2};
	int* di = di_around;
	int* dj = dj_around;
	switch (direction) {
		case CMD_UP: {
			di = di_dec;
			dj = consts;
			break;	
		}
		case CMD_DOWN: {
			di = di_inc;
			dj = consts;
			break;
		}
		case CMD_LEFT: {
			di = consts;
			dj = dj_dec;
			break;
		}
		case CMD_RIGHT: {
			di = consts;
			dj = dj_inc;
			break;
		}
		case CMD_LUP: {
			di = di_dec;
			dj = dj_dec;
			break;	
		}
		case CMD_LDOWN: {
			di = di_inc;
			dj = dj_dec;
			break;
		}
		case CMD_RUP: {
			di = di_dec;
			dj = dj_inc;
			break;
		}
		case CMD_RDOWN: {
			di = di_inc;
			dj = dj_inc;
			break;
		}
		case CMD_AROUND: {
			di = di_around;
			dj = dj_around;
			for(unsigned int t = 0; t < 16; t++) {
				objects.push_back(ObjectPtr(new Magic(getrow() + di[t], getcol() + dj[t], 2)));
			}
			return;
		}

	}
	for(unsigned int t = 1; t < 5; t++) {
		objects.push_back(ObjectPtr(new Magic(getrow() + di[t], getcol() + dj[t], 2)));
	}
}

bool Knight::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	std::string moves(STR_MOVES);
	moved_on_attack = CMD_NONE;
	char action;
	// action = getch();
	cin >> action;
	// condition means player wants to move
	if (moves.find(action) != std::string::npos) {
		moved_on_attack = action;
		return false;
	}	
	switch (action) {
		case CMD_ATTACK: {
			slash(characters);
			return true;	
		}
		case CMD_MAGIC: {
			char direction;
			cin >> direction;
			magic(objects, direction);
			return true;
		}
		case CMD_QUIT: {
			exit(0);
		}
	}
	return true;
}

bool Knight::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	char action;
	if (moved_on_attack == CMD_NONE) {
		// action = getch();
		cin >> action;
	}
	else {
		action = moved_on_attack;
	}
	switch (action) {
		case CMD_UP: {
			--coord.row;
			return true;	
		}
		case CMD_DOWN: {
			++coord.row;
			return true;
		}
		case CMD_AROUND: {
			++coord.row;
			return true;
		}
		case CMD_LEFT: {
			--coord.col;
			return true;
		}
		case CMD_RIGHT: {
			++coord.col;
			return true;
		}
		case CMD_LUP: {
			--coord.row;
			--coord.col;
			return true;	
		}
		case CMD_LDOWN: {
			++coord.row;
			--coord.col;
			return true;
		}
		case CMD_RUP: {
			--coord.row;
			++coord.col;
			return true;
		}
		case CMD_RDOWN: {
			++coord.row;
			++coord.col;
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
	fcolor = Colored(BG_AQUA, FG_WHITE).to_string();
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

bool Princess::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	return false;
}





Monster::Monster(int arow, int acol) : Character(arow, acol) {
	fcolor = Colored(BG_RED, FG_WHITE).to_string();
}

Monster::Monster(int arow, int acol, int hp, int dmg) : Character(arow, acol, hp, dmg) {
	fcolor = Colored(BG_RED, FG_WHITE).to_string();
}

Monster::~Monster() {

}

bool Monster::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	if (chance(30, "")) {
		coord.row += dz_1[rand()%3];
		coord.col += dz_1[rand()%3];
	}
	else {
		if (way.size() > 0) {
			coord.row = way.front().first;
			coord.col = way.front().second;
			way.pop_front();
		}
	}
	return false;
}

void Monster::refresh_way(Map& m, std::list<CharacterPtr>& characters) {
	way = shortest_way_to(characters.front(), m);
}

IntIntPairList Monster::shortest_way_to(BaseObjectPtr obj, Map& m) {
	return m.shortest_way(
		IntIntPair(this->getrow(), this->getcol()), 
		IntIntPair(obj ->getrow(), obj ->getcol()));
}




Dragon::Dragon(int arow, int acol) : Monster(arow, acol, HP_DRAGON, DMG_DRAGON) {
	fcolor = Colored(BG_RED, FG_WHITE).to_string();
}

Dragon::Dragon(int arow, int acol, int hp, int dmg) : Monster(arow, acol, hp, dmg) {
	fcolor = Colored(BG_RED, FG_WHITE).to_string();
}

Dragon::~Dragon() {
	cout << "Dragon died\n";
}

void Dragon::magic(list<ObjectPtr>& objects) {
	int flames_cnt = rand()%30 + 1;
	for(int i = 0; i < flames_cnt; i++) {
		objects.push_back(ObjectPtr(new Flame( getrow() + (rand()%10-5), getcol() + (rand()%10-5) )));	
	}
}

bool Dragon::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	CharacterPtr knight = characters.front();
	refresh_way(m, characters);
	if (*this % *knight && chance(80, "")) {
		slash(characters);
		return true;
	}
	if (abs(knight->getrow() - getrow()) < 10 && abs(knight->getcol() - getcol()) < 10) {
		if (chance(45, "")) {
			magic(objects);
		}
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






Zombie::Zombie(int arow, int acol) : Monster(arow, acol, HP_ZOMBIE, DMG_ZOMBIE) {

}

Zombie::Zombie(int arow, int acol, int hp, int dmg) : Monster(arow, acol, hp, dmg) {
	fcolor = Colored(BG_ORANGE, FG_WHITE).to_string();
}

Zombie::~Zombie() {

}

char Zombie::symbol() {
	return SYM_ZOMBIE;
}

bool Zombie::suffer(int dmg) {
	return (health -= dmg) <= 0;
}

bool Zombie::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	CharacterPtr knight = characters.front();
	refresh_way(m, characters);
	if (*this % *knight) {
		slash(characters);
		return true;
	}
	// objects.push_back(ObjectPtr(new Swamp(getrow()), getcol())));
	return false;
}





Warlock::Warlock(int arow, int acol) : Monster(arow, acol, HP_WARLOCK, DMG_WARLOCK) {
	fcolor = Colored(BG_BLACK, FG_WHITE).to_string();
}

Warlock::Warlock(int arow, int acol, int hp, int dmg) : Monster(arow, acol, hp, dmg) {
	fcolor = Colored(BG_BLACK, FG_WHITE).to_string();
}

Warlock::~Warlock() {
	cout << "Warlock died\n";
}

char Warlock::symbol() {
	return SYM_WARLOCK;
}

bool Warlock::suffer(int dmg) {
	return (health -= dmg) <= 0;
}

bool Warlock::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	CharacterPtr knight = characters.front();
	refresh_way(m, characters);
	if (*this % *knight) {
		slash(characters);
		return true;
	}
	// objects.push_back(ObjectPtr(new Swamp(getrow(), getcol())));
	return false;
}
