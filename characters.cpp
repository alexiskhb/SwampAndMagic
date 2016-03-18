#include "characters.h"
#include "objects.h"
#include <curses.h>
#include "control.h"

static const char* STR_MOVES  = "wasdqezcx"; 

using namespace std;

int dz_1[3] = {-1, 0, 1};
int dz_2[5] = {-2, -1, 0, 1, 2};
int dz_3[7] = {-3, -2, -1, 0, 1, 2, 3};


Character::Character(GCoord acoord) : BaseObject(acoord) {
	fcolor = Colored(BG_BLACK, FG_BLACK).to_string();
}

Character::Character(GCoord acoord, int hp, int dmg) : BaseObject(acoord) {
	damage = dmg;
	health = hp;
	fcolor = Colored(BG_BLACK, FG_BLACK).to_string();
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

bool Character::has_plans() {
	return false;
}

// all characters are impenetrable
bool Character::is_penetrable() {
	return false;
}





Knight::Knight(GCoord acoord) : Character(acoord) {

}

Knight::Knight(GCoord acoord, int hp, int dmg) : Character(acoord, hp, dmg) {
	fcolor = Colored(BG_GREEN, FG_WHITE).to_string();
	fsymb = SYM_KNIGHT | A_BOLD | COLOR_PAIR(ID_KNIGHT);
}

Knight::~Knight() {

}

char Knight::symbol() {
	return SYM_KNIGHT;
}

bool Knight::is_evil() {
	return false;
}

void Knight::magic(list<ObjectPtr>& objects, char direction) {
	static int consts[5] = {0, 0, 0, 0, 0};
	static int di_inc[5] = {1, 2, 3, 4, 5};
	static int di_dec[5] = {-1, -2, -3, -4, -5};
	static int dj_inc[5] = {1, 2, 3, 4, 5};
	static int dj_dec[5] = {-1, -2, -3, -4, -5};
	static int di_around[16] = {-2, -2, -2, -2, -2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1};
	static int dj_around[16] = {-2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2};
	int* di = di_around;
	int* dj = dj_around;
	switch (direction) {
		case CMD_NUP: 
		case CMD_UP: 
			di = di_dec;
			dj = consts;
			break;	
		
		case CMD_NDOWN: 
		case CMD_DOWN: 
			di = di_inc;
			dj = consts;
			break;
		
		case CMD_NLEFT: 
		case CMD_LEFT: 
			di = consts;
			dj = dj_dec;
			break;
		
		case CMD_NRIGHT: 
		case CMD_RIGHT: 
			di = consts;
			dj = dj_inc;
			break;
		
		case CMD_NLUP: 
		case CMD_LUP: 
			di = di_dec;
			dj = dj_dec;
			break;	
		
		case CMD_NLDOWN: 
		case CMD_LDOWN: 
			di = di_inc;
			dj = dj_dec;
			break;
		
		case CMD_NRUP: 
		case CMD_RUP: 
			di = di_dec;
			dj = dj_inc;
			break;
		
		case CMD_NRDOWN: 
		case CMD_RDOWN: 
			di = di_inc;
			dj = dj_inc;
			break;
		
		case CMD_NAROUND: 
		case CMD_AROUND: 
			di = di_around;
			dj = dj_around;
			for(unsigned int t = 0; t < 16; t++) {
				objects.push_back(make_shared<Magic>(GCoord(getrow() + di[t], getcol() + dj[t]), 10, GCoord(sgn0(di[t]), sgn0(dj[t]))));
			}
			return;
	}
	for(unsigned int t = 0; t < 5; t++) {
		objects.push_back(make_shared<Magic>(GCoord(getrow() + di[t], getcol() + dj[t]), 10, GCoord(sgn0(di[t]), sgn0(dj[t]))));
	}
}

bool Knight::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	static std::string moves(STR_MOVES);
	moved_on_attack = CMD_NONE;
	char action = get_command();
	// condition means player wants to move
	if (moves.find(action) != std::string::npos) {
		moved_on_attack = action;
		return false;
	}	
	switch (action) {
		case CMD_ATTACK: {
			slash(characters);
			prev_coord = coord;
			return true;	
		}
		case CMD_MAGIC: {
			char direction = get_command();
			magic(objects, direction);
			prev_coord = coord;
			return true;
		}
		case CMD_QUIT: {
			health = 0;
		}
	}
	return true;
}

bool Knight::has_plans() {
	return actions.size() > 0;
}

char Knight::get_command() {
	return getch();
}

bool Knight::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	char action;
	if (moved_on_attack == CMD_NONE) {
		action = get_command();
	}
	else {
		action = moved_on_attack;
	}
	switch (action) { 
		case CMD_NUP: 
		case CMD_UP:
			--coord.row;
			return true;	
		
		case CMD_NDOWN: 
		case CMD_DOWN: 
			++coord.row;
			return true;
		
		case CMD_NAROUND: 
		case CMD_AROUND: 
			++coord.row;
			return true;
		
		case CMD_NLEFT: 
		case CMD_LEFT: 
			--coord.col;
			return true;
		
		case CMD_NRIGHT: 
		case CMD_RIGHT: 
			++coord.col;
			return true;
		
		case CMD_NLUP: 
		case CMD_LUP: 
			--coord.row;
			--coord.col;
			return true;	
		
		case CMD_NLDOWN: 
		case CMD_LDOWN: 
			++coord.row;
			--coord.col;
			return true;
		
		case CMD_NRUP: 
		case CMD_RUP: 
			--coord.row;
			++coord.col;
			return true;
		
		case CMD_NRDOWN: 
		case CMD_RDOWN: 
			++coord.row;
			++coord.col;
			return true;	
	}
	return false;
}

bool Knight::suffer(int dmg) {
	return (health -= dmg) <= 0;
}





Princess::Princess(GCoord acoord) : Character(acoord) {
	fcolor = Colored(BG_AQUA, FG_WHITE).to_string();
	fsymb = SYM_PRINCESS | A_BOLD | COLOR_PAIR(ID_PRINCESS);
}

Princess::Princess(GCoord acoord, int hp, int dmg) : Character(acoord, hp, dmg) {
	fcolor = Colored(BG_AQUA, FG_WHITE).to_string();
	fsymb = SYM_PRINCESS | A_BOLD | COLOR_PAIR(ID_PRINCESS);
}

Princess::~Princess() {
	// cout << ">> Princess died\n";
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





Monster::Monster(GCoord acoord) : Character(acoord) {
}

Monster::Monster(GCoord acoord, int hp, int dmg) : Character(acoord, hp, dmg) {
}

Monster::~Monster() {

}

bool Monster::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	if (chance(30)) {
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
		IntIntPair(obj ->getrow(), obj ->getcol()), 2*(std::min(MAP_HEIGHT, MAP_WIDTH)/3));
}




Dragon::Dragon(GCoord acoord) : Monster(acoord, HP_DRAGON, DMG_DRAGON) {
	fcolor = Colored(BG_RED, FG_WHITE).to_string();
	fsymb = SYM_DRAGON | A_BOLD | COLOR_PAIR(ID_DRAGON);
}

Dragon::Dragon(GCoord acoord, int hp, int dmg) : Monster(acoord, hp, dmg) {
	fcolor = Colored(BG_RED, FG_WHITE).to_string();
	fsymb = SYM_DRAGON | A_BOLD | COLOR_PAIR(ID_DRAGON);
}

Dragon::~Dragon() {
	// cout << "Dragon died\n";
}

void Dragon::magic(list<ObjectPtr>& objects) {
	int flames_cnt = rand()%30 + 1;
	for(int i = 0; i < flames_cnt; i++) {
		objects.push_back(make_shared<Flame>(GCoord(getrow() + (rand()%10-5), getcol() + (rand()%10-5))));	
	}
}

bool Dragon::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	CharacterPtr knight = characters.front();
	refresh_way(m, characters);
	if (*this % *knight && chance(80)) {
		slash(characters);
		return true;
	}
	if (abs(knight->getrow() - getrow()) < 10 && abs(knight->getcol() - getcol()) < 10) {
		if (chance(45)) {
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






Zombie::Zombie(GCoord acoord) : Monster(acoord, HP_ZOMBIE, DMG_ZOMBIE) {
	fcolor = Colored(BG_ORANGE, FG_WHITE).to_string();
	fsymb = SYM_ZOMBIE | A_BOLD | COLOR_PAIR(ID_ZOMBIE);
}

Zombie::Zombie(GCoord acoord, int hp, int dmg) : Monster(acoord, hp, dmg) {
	fcolor = Colored(BG_ORANGE, FG_WHITE).to_string();
	fsymb = SYM_ZOMBIE | A_BOLD | COLOR_PAIR(ID_ZOMBIE);
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





Warlock::Warlock(GCoord acoord) : Monster(acoord, HP_WARLOCK, DMG_WARLOCK) {
	fcolor = Colored(BG_BLACK, FG_WHITE).to_string();
	fsymb = SYM_WARLOCK | A_BOLD | COLOR_PAIR(ID_WARLOCK);
}

Warlock::Warlock(GCoord acoord, int hp, int dmg) : Monster(acoord, hp, dmg) {
	fcolor = Colored(BG_BLACK, FG_WHITE).to_string();
	fsymb = SYM_WARLOCK | A_BOLD | COLOR_PAIR(ID_WARLOCK);
}

Warlock::~Warlock() {
	// cout << "Warlock died\n";
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
	if (chance(30)) {
		magic(characters, objects);
	}
	if (*this % *knight) {
		slash(characters);
		return true;
	}
	return false;
}

bool Warlock::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	if (chance(20)) {
		coord.row += dz_2[rand()%5];
		coord.col += dz_2[rand()%5];
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

void Warlock::magic(list<CharacterPtr>& characters, list<ObjectPtr>& objects) {
	CharacterPtr kn = characters.front();
	GCoord dc = kn->get_coord() - coord;
	objects.push_back(make_shared<Curse>(
		GCoord(getrow() + sgn0(dc.row), getcol() + sgn0(dc.col)), 
		2, GCoord(sgn0(dc.row), sgn0(dc.col))));
}