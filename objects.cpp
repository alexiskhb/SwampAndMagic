#include "objects.h"


using namespace std;



Object::Object(int arow, int acol) : BaseObject(arow, acol) {
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
	lifetime = 6;
}

Flame::~Flame() {

}

char Flame::symbol() {
	return SYM_FLAME;
}

bool Flame::is_penetrable(){
	return true;
}





Swamp::Swamp(int arow, int acol) : Object(arow, acol) {

}

Swamp::~Swamp() {

}

char Swamp::symbol() {
	return SYM_SWAMP;
}

bool Swamp::is_penetrable() {
	return true;
}





// magic can heal knight
Magic::Magic(int arow, int acol) : Object(arow, acol) {
	lifetime = 5;
}

Magic::~Magic() {

}

char Magic::symbol() {
	return SYM_MAGIC;
}

bool Magic::is_penetrable() {
	return true;
}
