#include "characters.h"
#include "objects.h"
#include <curses.h>
#include "control.h"


using namespace std;

int dz_1[3] = {-1, 0, 1};
int dz_2[5] = {-2, -1, 0, 1, 2};
int dz_3[7] = {-3, -2, -1, 0, 1, 2, 3};


Character::Character(GCoord acoord, int hp, int dmg) : BaseObject(acoord) {
	damage = dmg;
	max_health = health = hp;
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

void Character::suffer(int dmg) {
	health = std::min(health - dmg, max_health);
}

// all characters are impenetrable
bool Character::is_penetrable() {
	return false;
}





Knight::Knight(GCoord acoord) : Character(acoord, HP_KNIGHT, DMG_KN_SWORD) {
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
		break;
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
	// This condition means there is one of WASDQEZXC in moves
	if (moves.find(action) != std::string::npos) {
		moved_on_attack = action;
		return false;
	}	
	switch (action) {
		break;
		case CMD_ATTACK: {
			slash(characters);
			prev_coord = coord;
			return true;	
		}
		break;
		case CMD_MAGIC: {
			char direction = get_command();
			magic(objects, direction);
			prev_coord = coord;
			return true;
		}
		break;
		case CMD_QUIT: {
			health = 0;
		}
	}
	return true;
}

char Knight::get_command() {
	return Control::instance().get_command();
}

bool Knight::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	char action;
	GCoord delta;
	if (moved_on_attack == CMD_NONE) {
		action = get_command();
	}
	else {
		action = moved_on_attack;
	}
	switch (action) { 
		break;
		case CMD_NUP: 
		case CMD_UP:
			coord += GCoord(-1, 0);
			return true;	
		
		break;
		case CMD_NDOWN: 
		case CMD_DOWN:
			coord += GCoord(1, 0);
			return true;
		
		break;
		case CMD_NAROUND: 
		case CMD_AROUND:
			coord += GCoord(1, 0);
			return true;
		
		break;
		case CMD_NLEFT: 
		case CMD_LEFT: 
			coord += GCoord(0, -1);
			return true;
		
		break;
		case CMD_NRIGHT: 
		case CMD_RIGHT:
			coord += GCoord(0, 1);
			return true;
		
		break;
		case CMD_NLUP: 
		case CMD_LUP:
			coord += GCoord(-1, -1);
			return true;	
		
		break;
		case CMD_NLDOWN: 
		case CMD_LDOWN:
			coord += GCoord(1, -1);
			return true;
		
		break;
		case CMD_NRUP: 
		case CMD_RUP: 
			coord += GCoord(-1, 1);
			return true;
		
		break;
		case CMD_NRDOWN: 
		case CMD_RDOWN: 
			coord += GCoord(1, 1);
			return true;	
	}
	return false;
}




Princess::Princess(GCoord acoord) : Character(acoord, HP_PRINCESS, DMG_PRINCESS) {
	fsymb = SYM_PRINCESS | A_BOLD | COLOR_PAIR(ID_PRINCESS);
}

Princess::~Princess() {
}

char Princess::symbol() {
	return SYM_PRINCESS;
}

bool Princess::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	return false;
}

void Princess::destroy() {

}





Monster::Monster(GCoord acoord, int hp, int dmg) : Character(acoord, hp, dmg) {
}

Monster::~Monster() {
}

bool Monster::move(Map& m, std::list<CharacterPtr>& characters) {
	prev_coord = coord;
	if (way.size() > 0) {
		if (chance(80)) {
			coord = GCoord(way.front().first, way.front().second);
			way.pop_front();
			return false;
		}
	}
	coord += GCoord(dz_1[rand()%3], dz_1[rand()%3]);
	return false;
}

void Monster::refresh_way(Map& m, std::list<CharacterPtr>& characters) {
	way = shortest_way_to(characters.front(), m);
}

IntIntPairList Monster::shortest_way_to(BaseObjectPtr obj, Map& m) {
	return m.shortest_way(
		IntIntPair(this->getrow(), this->getcol()), 
		IntIntPair(obj ->getrow(), obj ->getcol()), std::min(MAP_HEIGHT, MAP_WIDTH)/2);
}

chtype Monster::symb() {
	return fsymb | (aggro ? A_BOLD : 0);
}



Dragon::Dragon(GCoord acoord) : Monster(acoord, HP_DRAGON, DMG_DRAGON) {
	fsymb = SYM_DRAGON | COLOR_PAIR(ID_DRAGON);
}

Dragon::~Dragon() {
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
	aggro = way.size() > 0 || *this % *knight;
	if (*this % *knight && chance(80)) {
		slash(characters);
		return true;
	}
	if (abs(knight->getrow() - getrow()) < 10 && abs(knight->getcol() - getcol()) < 10) {
		if (chance(45)) {
			magic(objects);
		}
	}
	return chance(15);
}

char Dragon::symbol() {
	return SYM_DRAGON;
}




Zombie::Zombie(GCoord acoord) : Monster(acoord, HP_ZOMBIE, DMG_ZOMBIE) {
	fsymb = SYM_ZOMBIE | COLOR_PAIR(ID_ZOMBIE);
}

Zombie::~Zombie() {
}

char Zombie::symbol() {
	return SYM_ZOMBIE;
}

bool Zombie::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	CharacterPtr knight = characters.front();
	refresh_way(m, characters);
	aggro = way.size() > 0 || *this % *knight;
	if (*this % *knight) {
		slash(characters);
		return true;
	}
	// objects.push_back(ObjectPtr(new Swamp(getrow()), getcol())));
	return false;
}





Warlock::Warlock(GCoord acoord) : Monster(acoord, HP_WARLOCK, DMG_WARLOCK) {
	fsymb = SYM_WARLOCK | COLOR_PAIR(ID_WARLOCK);
}

Warlock::~Warlock() {
}

char Warlock::symbol() {
	return SYM_WARLOCK;
}

bool Warlock::attack(list<CharacterPtr>& characters, list<ObjectPtr>& objects, Map& m) {
	CharacterPtr knight = characters.front();
	refresh_way(m, characters);
	aggro = way.size() > 0 || *this % *knight;
	if (chance(30)) {
		magic(characters, objects);
		return true;
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
		coord += GCoord(dz_2[rand()%5], dz_2[rand()%5]);
	}
	else {
		if (way.size() > 0) {
			coord = GCoord(way.front().first, way.front().second);
			way.pop_front();
		}
	}
	return false;
}

void Warlock::magic(list<CharacterPtr>& characters, list<ObjectPtr>& objects) {
	CharacterPtr kn = characters.front();
	GCoord dc = kn->get_coord() - coord;
	objects.push_back(make_shared<Curse>(
		GCoord(
			getrow() + sgn0(dc.row()), 
			getcol() + sgn0(dc.col())), 
		2, 
		GCoord(sgn0(dc.row()), sgn0(dc.col()))
		)
	);
}
