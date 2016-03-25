#include "objects.h"
#include "characters.h"


using namespace std;



Object::Object(GCoord acoord) : BaseObject(acoord), direction(0, 0) {
	// log("obj");
}

Object::Object(GCoord acoord, GCoord dir) : BaseObject(acoord), direction(dir) {
	// log("obj");
}

Object::~Object() {
	// log("destroy obj");
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
// 	log("wall");
}

Wall::~Wall() {
	// log("destroy wall");
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
	log("flame");
	health = TIME_FLAME;
	damage = DMG_FLAME;
	fsymb = SYM_FLAME | A_BOLD | A_REVERSE | COLOR_PAIR(ID_FLAME);
}

Flame::~Flame() {
	log("destroy flame");	
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
	log("swamp");
	health = TIME_SWAMP;
	fsymb = SYM_SWAMP | A_BOLD | A_REVERSE | COLOR_PAIR(ID_SWAMP);
}

Swamp::~Swamp() {
	log("destroy swamp");
}

char Swamp::symbol() {
	return SYM_SWAMP;
}

void Swamp::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {

}





// magic can heal knight
Magic::Magic(GCoord acoord) : Object(acoord) {
	log("magic");
	health = TIME_MAGIC;
	damage = DMG_MAGIC;
	fsymb = SYM_MAGIC | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MAGIC) | A_BOLD;
}

Magic::Magic(GCoord acoord, int timelife) : Object(acoord) {
	log("magic");
	health = timelife;
	damage = DMG_MAGIC;
	fsymb = SYM_MAGIC | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MAGIC) | A_BOLD;
}

Magic::Magic(GCoord acoord, int timelife, GCoord dir) : Object(acoord, dir) {
	log("magic");
	health = timelife;
	damage = DMG_MAGIC;
	fsymb = SYM_MAGIC | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MAGIC) | A_BOLD;
}

Magic::~Magic() {
	log("destroy magic");
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
			health = 4*TIME_MAGIC;
			direction = GCoord(0, 0);
		}
	}
	if (!m.is_penetrable(coord)) {
		health = 0;
	}
}




Curse::Curse(GCoord acoord) : Object(acoord), generation(0)  {
	log("curse");
	health = TIME_CURSE;
	damage = DMG_CURSE;
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE);
}

Curse::Curse(GCoord acoord, int timelife) : Object(acoord), generation(0)  {
	log("curse");
	health = timelife;
	damage = DMG_CURSE;
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE);
}

Curse::Curse(GCoord acoord, int timelife, GCoord dir) : Object(acoord, dir), generation(0)  {
	log("curse");
	health = timelife;
	damage = DMG_CURSE;
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE) | A_BOLD;
}

Curse::Curse(GCoord acoord, int timelife, int gener, GCoord dir) : Object(acoord, dir), generation(gener) {
	log("curse");
	health = timelife;
	damage = DMG_CURSE;
	fsymb = SYM_CURSE | A_BOLD | A_REVERSE | COLOR_PAIR(ID_CURSE) | A_BOLD;
}

Curse::~Curse() {
	log("destroy curse");
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
	log("medkit");
	health = TIME_MEDKIT;
	damage = DMG_MEDKIT;
	fsymb = SYM_MEDKIT | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MEDKIT) | A_BOLD;
}

Medkit::Medkit(GCoord acoord, int timelife) : Object(acoord) {
	log("medkit");
	health = timelife;
	damage = DMG_MEDKIT;
	fsymb = SYM_MEDKIT | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MEDKIT) | A_BOLD;
}

Medkit::~Medkit() {
	log("destroy medkit");
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





Respawn::Respawn(GCoord acoord, int freq) : Object(acoord), frequency(freq) {
}

void Respawn::destroy() {
}



DragonNest::DragonNest(GCoord acoord) : Respawn(acoord, 25+rand()%15) {
	log("dragonnest");
	health = TIME_DRGNEST;
	fsymb = SYM_DRGNEST | A_BOLD | A_REVERSE | COLOR_PAIR(ID_DRGNEST) | A_BOLD;
}

DragonNest::~DragonNest() {
	log("destroy dragonnest");
}

char DragonNest::symbol() {
	return SYM_DRGNEST;
}

void DragonNest::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	if (turn%frequency == 0) {
		characters.push_back(make_shared<Dragon>(get_coord()));	
	}
}




Graveyard::Graveyard(GCoord acoord) : Respawn(acoord, 5+rand()%5) {
	log("graveyard");
	health = TIME_GRVYARD;
	fsymb = SYM_GRVYARD | A_BOLD | A_REVERSE | COLOR_PAIR(ID_GRVYARD) | A_BOLD;
}

Graveyard::~Graveyard() {
	log("destroy graveyard");
}

char Graveyard::symbol() {
	return SYM_GRVYARD;
}

void Graveyard::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	if (turn%frequency == 0) {
		characters.push_back(std::make_shared<Zombie>(get_coord()));
	}
}





Ziggurat::Ziggurat(GCoord acoord) : Respawn(acoord, 60+rand()%50) {
	log("ziggurat");
	health = TIME_ZIGGURAT;
	fsymb = SYM_ZIGGURAT | A_BOLD | A_REVERSE | COLOR_PAIR(ID_ZIGGURAT) | A_BOLD;
}

Ziggurat::~Ziggurat() {
	log("destroy ziggurat");
}

char Ziggurat::symbol() {
	return SYM_ZIGGURAT;
}

void Ziggurat::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	if (turn%frequency == 0) {
		characters.push_back(std::make_shared<Warlock>(get_coord()));
	}
}
