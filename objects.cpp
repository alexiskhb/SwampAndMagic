#include "objects.h"


using namespace std;


static const int TIME_SWAMP = 5;
static const int TIME_MAGIC = 5;
static const int TIME_FLAME = 6;
static const int TIME_INFTY = -10000;

Object::Object(int arow, int acol) : BaseObject(arow, acol) {
}

bool Object::is_penetrable() {
	return true;
}

bool Object::is_alive() {
	return health > 0 || health == TIME_INFTY;
}




Wall::Wall(int arow, int acol) : Object(arow, acol) {

}

Wall::~Wall() {

}

char Wall::symbol() {
	return SYM_WALL;
}

bool Wall::is_penetrable() {
	return false;
}





Flame::Flame(int arow, int acol) : Object(arow, acol) {
	health = TIME_FLAME;
}

Flame::~Flame() {

}

char Flame::symbol() {
	return SYM_FLAME;
}





Swamp::Swamp(int arow, int acol) : Object(arow, acol) {
	health = TIME_SWAMP;

}

Swamp::~Swamp() {

}

char Swamp::symbol() {
	return SYM_SWAMP;
}





// magic can heal knight
Magic::Magic(int arow, int acol) : Object(arow, acol) {
	health = TIME_MAGIC;
}

Magic::~Magic() {

}

char Magic::symbol() {
	return SYM_MAGIC;
}
