#include "objects.h"
#include "characters.h"


using namespace std;



Object::Object(int arow, int acol) : BaseObject(arow, acol), direction(0, 0) {
}

Object::Object(int arow, int acol, GCoord dir) : BaseObject(arow, acol), direction(dir) {
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




Wall::Wall(int arow, int acol) : Object(arow, acol) {
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





Flame::Flame(int arow, int acol) : Object(arow, acol) {
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





Swamp::Swamp(int arow, int acol) : Object(arow, acol) {
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
Magic::Magic(int arow, int acol) : Object(arow, acol) {
	health = TIME_MAGIC;
	damage = DMG_MAGIC;
	fsymb = SYM_MAGIC | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MAGIC) | A_BOLD;
}

Magic::Magic(int arow, int acol, int timelife) : Object(arow, acol) {
	health = timelife;
	damage = DMG_MAGIC;
	fsymb = SYM_MAGIC | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MAGIC) | A_BOLD;
}

Magic::Magic(int arow, int acol, int timelife, GCoord dir) : Object(arow, acol, dir) {
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
			health = TIME_MAGIC;
		}
	}
	if (!m.is_penetrable(coord)) {
		health = 0;
	}
}




Curse::Curse(int arow, int acol) : Object(arow, acol) {
	health = TIME_CURSE;
	damage = DMG_CURSE;
	fcolor = Colored(BG_WHITE, FG_B_BLUE).to_string();
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE);
}

Curse::Curse(int arow, int acol, int timelife) : Object(arow, acol) {
	health = timelife;
	damage = DMG_CURSE;
	fcolor = Colored(BG_WHITE, FG_B_BLUE).to_string();
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE);
}

Curse::~Curse() {

}

char Curse::symbol() {
	return SYM_CURSE;
}

void Curse::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	for(auto ch: characters) {
		if (*ch == *this) {
			ch->suffer(ch->symbol() == SYM_KNIGHT ? -damage*1.5 : damage);
		}
	}
	if (!m.is_penetrable(coord)) {
		health = 0;
	}
}




Medkit::Medkit(int arow, int acol) : Object(arow, acol) {
	health = TIME_MEDKIT;
	damage = DMG_MEDKIT;
	fsymb = SYM_MEDKIT | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MEDKIT) | A_BOLD;
}

Medkit::Medkit(int arow, int acol, int timelife) : Object(arow, acol) {
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





DragonNest::DragonNest(int arow, int acol) : Object(arow, acol), frequency(5+rand()%5) {
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

}




Graveyard::Graveyard(int arow, int acol) : Object(arow, acol), frequency(5+rand()%5) {
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

}