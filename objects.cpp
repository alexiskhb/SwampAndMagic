#include "objects.h"
#include "characters.h"


using namespace std;



Object::Object(int arow, int acol) : BaseObject(arow, acol) {
	
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

void Object::turn() {
	if (health > 0) {
		--health;
	}
}

void Object::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) {

}




Wall::Wall(int arow, int acol) : Object(arow, acol) {
	fcolor = Colored(BG_GRAY, FG_DARK_GRAY).to_string();
}

Wall::~Wall() {

}

char Wall::symbol() {
	return SYM_WALL;
}

bool Wall::is_penetrable() {
	return false;
}

void Wall::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) {

}





Flame::Flame(int arow, int acol) : Object(arow, acol) {
	health = TIME_FLAME;
	damage = DMG_FLAME;
	fcolor = Colored(BG_WHITE, FG_RED).to_string();
}

Flame::~Flame() {
	
}

char Flame::symbol() {
	return SYM_FLAME;
}

void Flame::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) {
	for(auto ch: characters) {
		if (*ch == *this && ch->symbol() != SYM_DRAGON) {
			ch->suffer(damage);
			cout << ch->symbol() << " ON FIRE\n";
		}
	}
}





Swamp::Swamp(int arow, int acol) : Object(arow, acol) {
	health = TIME_SWAMP;
	fcolor = Colored(BG_WHITE, FG_YELLOW).to_string();
}

Swamp::~Swamp() {

}

char Swamp::symbol() {
	return SYM_SWAMP;
}

void Swamp::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) {

}





// magic can heal knight
Magic::Magic(int arow, int acol) : Object(arow, acol) {
	health = TIME_MAGIC;
	damage = DMG_MAGIC;
	fcolor = Colored(BG_WHITE, FG_B_BLUE).to_string();
}

Magic::Magic(int arow, int acol, int timelife) : Object(arow, acol) {
	health = timelife;
	damage = DMG_MAGIC;
	fcolor = Colored(BG_WHITE, FG_B_BLUE).to_string();
}

Magic::~Magic() {

}

char Magic::symbol() {
	return SYM_MAGIC;
}

void Magic::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) {
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
}




Curse::Curse(int arow, int acol) : Object(arow, acol) {
	health = TIME_CURSE;
	damage = DMG_CURSE;
	fcolor = Colored(BG_WHITE, FG_B_BLUE).to_string();
}

Curse::Curse(int arow, int acol, int timelife) : Object(arow, acol) {
	health = timelife;
	damage = DMG_CURSE;
	fcolor = Colored(BG_WHITE, FG_B_BLUE).to_string();
}

Curse::~Curse() {

}

char Curse::symbol() {
	return SYM_CURSE;
}

void Curse::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) {
	for(auto ch: characters) {
		if (*ch == *this) {
			ch->suffer(ch->symbol() == SYM_KNIGHT ? -damage*1.5 : damage);
		}
	}
	for(auto obj: objects) {
		if (*obj == *this && obj->symbol() == SYM_FLAME) {
			obj->destroy();
			health = TIME_CURSE;
		}
	}
}