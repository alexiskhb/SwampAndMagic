#include "objects.h"
#include "characters.h"

int 
	DMG_FLAME     = 3,
	DMG_CURSE     = 5,
	DMG_MAGIC     = 7,
	DMG_MEDKIT    = -20,
	TIME_SWAMP    = 15,
	TIME_MAGIC    = 5,
	TIME_CURSE    = 2,
	TIME_FLAME    = 5,
	TIME_INFTY    = -10000,
	TIME_MEDKIT   = TIME_INFTY,
	TIME_DRGNEST  = TIME_INFTY,
	TIME_GRVYARD  = TIME_INFTY,
	TIME_ZIGGURAT = TIME_INFTY,
	TIME_HOSPITAL = TIME_INFTY,
	LIM_MEDKIT    = 5;

using namespace std;

PairKeyMap Medkit::count;

Object::Object(GCoord acoord) : BaseObject(acoord), direction(0, 0) {
}

Object::Object(GCoord acoord, GCoord dir) : BaseObject(acoord), direction(dir) {
}

Object::~Object() {
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
			health = 4*TIME_MAGIC;
			direction = GCoord(0, 0);
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
	Coord crd1, crd2;
	if (abs(direction.row()) == abs(direction.col())) {
		crd1.row = 0;
		crd2.row = direction.row();
		crd1.col = direction.col();
		crd2.col = 0;
	}
	else {
		if (direction.row() == 0) {
			crd1.row = -1;
			crd2.row = 1;
			crd1.col = crd2.col = direction.col();
		}
		if (direction.col() == 0) {
			crd1.col = -1;
			crd2.col = 1;
			crd1.row = crd2.row = direction.row();
		}
	}
	if (generation < 3) {
		objects.push_front(make_shared<Curse>(
			get_coord() + GCoord(crd1.row, crd1.col), 
			3, 
			generation+1, 
			GCoord(crd1.row, crd1.col) 
			)
		);
		objects.push_front(make_shared<Curse>(
			get_coord() + GCoord(crd2.row, crd2.col),
			3, 
			generation+1, 
			GCoord(crd2.row, crd2.col)
			)
		);
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
	Medkit::count[IntIntPair(acoord.roomx(), acoord.roomy())] += 1;
}

Medkit::Medkit(GCoord acoord, int timelife) : Object(acoord) {
	health = timelife;
	damage = DMG_MEDKIT;
	fsymb = SYM_MEDKIT | A_BOLD | A_REVERSE | COLOR_PAIR(ID_MEDKIT) | A_BOLD;
	Medkit::count[IntIntPair(acoord.roomx(), acoord.roomy())] += 1;
}

Medkit::~Medkit() {
	Medkit::count[IntIntPair(initial_coord.roomx(), initial_coord.roomy())] -= 1;
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
	if (!m.is_penetrable(coord)) {
		health = 0;
	}
}





Spawn::Spawn(GCoord acoord, int freq) : Object(acoord), frequency(freq) {
}

void Spawn::destroy() {
}



DragonNest::DragonNest(GCoord acoord) : Spawn(acoord, 25+rand()%15) {
	health = TIME_DRGNEST;
	fsymb = SYM_DRGNEST | A_BOLD | A_REVERSE | COLOR_PAIR(ID_DRGNEST) | A_BOLD;
}

DragonNest::~DragonNest() {
}

char DragonNest::symbol() {
	return SYM_DRGNEST;
}

void DragonNest::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	if (turn%frequency == 0) {
		characters.push_back(make_shared<Dragon>(get_coord()));	
	}
}




Graveyard::Graveyard(GCoord acoord) : Spawn(acoord, 5+rand()%5) {
	health = TIME_GRVYARD;
	fsymb = SYM_GRVYARD | A_BOLD | A_REVERSE | COLOR_PAIR(ID_GRVYARD) | A_BOLD;
}

Graveyard::~Graveyard() {
}

char Graveyard::symbol() {
	return SYM_GRVYARD;
}

void Graveyard::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	if (turn%frequency == 0) {
		characters.push_back(std::make_shared<Zombie>(get_coord()));
	}
}





Ziggurat::Ziggurat(GCoord acoord) : Spawn(acoord, 60+rand()%50) {
	health = TIME_ZIGGURAT;
	fsymb = SYM_ZIGGURAT | A_BOLD | A_REVERSE | COLOR_PAIR(ID_ZIGGURAT) | A_BOLD;
}

Ziggurat::~Ziggurat() {
}

char Ziggurat::symbol() {
	return SYM_ZIGGURAT;
}

void Ziggurat::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	if (turn%frequency == 0) {
		characters.push_back(std::make_shared<Warlock>(get_coord()));
	}
}



Hospital::Hospital(GCoord acoord) : Spawn(acoord, 10) {
	health = TIME_HOSPITAL;
	fsymb = SYM_EMPTY | COLOR_PAIR(ID_EMPTY) | A_REVERSE | A_BOLD;
}

Hospital::~Hospital() {
}

char Hospital::symbol() {
	return SYM_EMPTY;
}

void Hospital::impact(list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) {
	static IntIntPair spawn_coord = make_pair(get_coord().roomx(), get_coord().roomy());
	if (turn%frequency == 0 && Medkit::count[spawn_coord] < LIM_MEDKIT) {
		objects.push_back(std::make_shared<Medkit>(m.get_rand_free_cell(coord)));
	}
}
