#include "characters.h"
#include "objects.h"

static const char* STR_MOVES  = "wasdqezcx"; 

using namespace std;

int dz_1[3] = {-1, 0, 1};
int dz_3[7] = {-3, -2, -1, 0, 1, 2, 3};


Character::Character(int arow, int acol) : BaseObject(arow, acol) {
}

Character::Character(int arow, int acol, int hp, int dmg) : BaseObject(arow, acol) {
	damage = dmg;
	health = hp;
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
	return false;
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

void Knight::magic(list<ObjectPtr>& objects, char direction) {
	int consts[5] = {0, 0, 0, 0, 0};
	int di_inc[5] = {1, 2, 3, 4, 5};
	int di_dec[5] = {-1, -2, -3, -4, -5};
	int dj_inc[5] = {1, 2, 3, 4, 5};
	int dj_dec[5] = {-1, -2, -3, -4, -5};
	int di_around[12] = {-2, -2, -2, -1, 0, 1, 2, 2, 2, 1, 0, -1};
	int dj_around[12] = {-1, 0, 1, 2, 2, 2, 1, 0, -1, -2, -2, -2};
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
			for(unsigned int t = 0; t < 12; t++) {
				objects.push_back(ObjectPtr(new Magic(row + di[t], col + dj[t], 2)));
			}
			return;
		}

	}
	for(unsigned int t = 1; t < 5; t++) {
		objects.push_back(ObjectPtr(new Magic(row + di[t], col + dj[t], 2)));
	}
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
		case CMD_AROUND: {
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
		case CMD_LUP: {
			--row;
			--col;
			return true;	
		}
		case CMD_LDOWN: {
			++row;
			--col;
			return true;
		}
		case CMD_RUP: {
			--row;
			++col;
			return true;
		}
		case CMD_RDOWN: {
			++row;
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

bool Princess::move(Map& m, std::list<CharacterPtr>& characters) {
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

bool Monster::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_row = row;
	prev_col = col;
	if (chance(50, "")) {
		row += dz_1[rand()%3];
		col += dz_1[rand()%3];
	}
	else {
		way = shortest_way_to(characters.front(), m);
		if (way.size() > 0) {
			row = way.front().first;
			col = way.front().second;
			way.pop_front();
		}
	}
	return false;
}

IntIntPairList Monster::shortest_way_to(BaseObjectPtr obj, Map& m) {
	return m.shortest_way(
		IntIntPair(this->getrow(), this->getcol()), 
		IntIntPair(obj ->getrow(), obj ->getcol()));
}




Dragon::Dragon(int arow, int acol) : Monster(arow, acol) {

}

Dragon::Dragon(int arow, int acol, int hp, int dmg) : Monster(arow, acol, hp, dmg) {

}

Dragon::~Dragon() {
	cout << "Dragon died\n";
}

void Dragon::magic(list<ObjectPtr>& objects) {
	objects.push_back(ObjectPtr(new Flame(row - 1, col - 1)));
	objects.push_back(ObjectPtr(new Flame(row - 1, col + 1)));
	objects.push_back(ObjectPtr(new Flame(row + 1, col + 1)));
	objects.push_back(ObjectPtr(new Flame(row + 1, col - 1)));
}

bool Dragon::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, CharacterPtr self) {
	CharacterPtr knight = characters.front();
	if (*self % *knight) {
		slash(characters, self);
		return true;
	}
	if (abs(knight->getrow() - row) < 10 && abs(knight->getcol() - col) < 10) {
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

bool Zombie::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, CharacterPtr self) {
	CharacterPtr knight = characters.front();
	if (*self % *knight) {
		slash(characters, self);
		return true;
	}
	objects.push_back(ObjectPtr(new Swamp(this->row, this->col)));
	return false;
}
