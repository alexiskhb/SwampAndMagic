#include "objects.h"
#include "characters.h"


using namespace std;



Object::Object(GCoord acoord) : BaseObject(acoord), direction(0, 0) {
}

Object::Object(GCoord acoord, GCoord dir) : BaseObject(acoord), direction(dir) {
}

Object::~Object() {

}

void Object::destroy() {
	health = 0;
}

bool Object::is_penetrable() {
	return true;
}

bool Object::is_alive() {
	return health > 0 || health == TIME_INFTY;
}

void Object::make_turn() {
	if (health > 0 && health != TIME_INFTY) {
		--health;
	}
	moveto(coord+direction);
}

void Object::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {

}




Wall::Wall(GCoord acoord) : Object(acoord) {
	fsymb = SYM_WALL | COLOR_PAIR(ID_WALL);
}

Wall::~Wall() {

}

char Wall::symbol() {
	return SYM_WALL;
}

bool Wall::is_penetrable() {
	return false;
}

void Wall::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {

}





Flame::Flame(GCoord acoord) : Object(acoord) {
	health = TIME_FLAME;
	damage = DMG_FLAME;
	fsymb = SYM_FLAME | A_BOLD | A_REVERSE | COLOR_PAIR(ID_FLAME);
}

Flame::~Flame() {
	
}

char Flame::symbol() {
	return SYM_FLAME;
}

void Flame::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	for(auto ch: characters) {
		if (*ch == *this && ch->symbol() != SYM_DRAGON) {
			ch->suffer(damage);
			// cout << ch->symbol() << " ON FIRE\n";
		}
	}
}





Swamp::Swamp(GCoord acoord) : Object(acoord) {
	health = TIME_SWAMP;
	fsymb = SYM_SWAMP | A_BOLD | A_REVERSE | COLOR_PAIR(ID_SWAMP);
}

Swamp::~Swamp() {

}

char Swamp::symbol() {
	return SYM_SWAMP;
}

void Swamp::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {

}





// magic can heal knight
Magic::Magic(GCoord acoord) : Object(acoord) {
	health = TIME_MAGIC;
	damage = DMG_MAGIC;
	fsymb = SYM_MAGIC | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MAGIC) | A_BOLD;
}

Magic::Magic(GCoord acoord, int timelife) : Object(acoord) {
	health = timelife;
	damage = DMG_MAGIC;
	fsymb = SYM_MAGIC | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MAGIC) | A_BOLD;
}

Magic::Magic(GCoord acoord, int timelife, GCoord dir) : Object(acoord, dir) {
	health = timelife;
	damage = DMG_MAGIC;
	fsymb = SYM_MAGIC | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MAGIC) | A_BOLD;
}

Magic::~Magic() {

}

char Magic::symbol() {
	return SYM_MAGIC;
}

void Magic::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	for(auto ch: characters) {
		if (*ch == *this) {
			ch->suffer(ch->symbol() == SYM_KNIGHT ? -damage*1.5 : damage);
		}
	}
	for(auto obj: objects) {
		if (*obj == *this && obj->symbol() == SYM_FLAME) {
			obj->destroy();
			health = 3*TIME_MAGIC;
		}
	}
	if (!m.is_penetrable(coord)) {
		health = 0;
	}
}




Curse::Curse(GCoord acoord) : Object(acoord), generation(0)  {
	health = TIME_CURSE;
	damage = DMG_CURSE;
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE);
}

Curse::Curse(GCoord acoord, int timelife) : Object(acoord), generation(0)  {
	health = timelife;
	damage = DMG_CURSE;
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE);
}

Curse::Curse(GCoord acoord, int timelife, GCoord dir) : Object(acoord, dir), generation(0)  {
	health = timelife;
	damage = DMG_CURSE;
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE) | A_BOLD;
}

Curse::Curse(GCoord acoord, int timelife, int gener, GCoord dir) : Object(acoord, dir), generation(gener) {
	health = timelife;
	damage = DMG_CURSE;
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE) | A_BOLD;
}

Curse::~Curse() {

}

char Curse::symbol() {
	return SYM_CURSE;
}

void Curse::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	static int di[4] = {-1, -1, 1, 1};
	static int dj[4] = {-1, 1, 1, -1};
	if (generation < 3) {
		for(int t = 0; t < 4; t++) {
			objects.push_front(make_shared<Curse>(GCoord(getrow() + di[t], getcol() + dj[t]), 2, generation+1, GCoord(sgn0(di[t]), sgn0(dj[t]))));
		}
	}
	for(auto ch: characters) {
		if (*ch == *this) {
			ch->suffer(ch->symbol() == SYM_WARLOCK ? 0 : damage);
		}
	}
	if (!m.is_penetrable(coord)) {
		health = 0;
	}
}




Medkit::Medkit(GCoord acoord) : Object(acoord) {
	health = TIME_MEDKIT;
	damage = DMG_MEDKIT;
	fsymb = SYM_MEDKIT | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MEDKIT) | A_BOLD;
}

Medkit::Medkit(GCoord acoord, int timelife) : Object(acoord) {
	health = timelife;
	damage = DMG_MEDKIT;
	fsymb = SYM_MEDKIT | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MEDKIT) | A_BOLD;
}

Medkit::~Medkit() {

}

char Medkit::symbol() {
	return SYM_MEDKIT;
}

void Medkit::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	for(auto ch: characters) {
		if (*ch == *this) {
			ch->suffer(damage);
			health = 0;
			break;
		}
	}
}





DragonNest::DragonNest(GCoord acoord) : Object(acoord), frequency(5+rand()%5) {
	health = TIME_DRGNEST;
	fsymb = SYM_DRGNEST | A_BOLD | A_REVERSE | COLOR_PAIR(ID_DRGNEST) | A_BOLD;
}

DragonNest::~DragonNest() {

}

char DragonNest::symbol() {
	return SYM_DRGNEST;
}

void DragonNest::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	if (!(turn%frequency == 0)) {
		return;
	}
	std::make_shared<Dragon>(get_coord());
}




Graveyard::Graveyard(GCoord acoord) : Object(acoord), frequency(5+rand()%5) {
	health = TIME_GRVYARD;
	fsymb = SYM_GRVYARD | A_BOLD | A_REVERSE | COLOR_PAIR(ID_GRVYARD) | A_BOLD;
}

Graveyard::~Graveyard() {

}

char Graveyard::symbol() {
	return SYM_GRVYARD;
}

void Graveyard::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	if (!(turn%frequency == 0)) {
		return;
	}
	characters.push_back(std::make_shared<Zombie>(get_coord()));
}