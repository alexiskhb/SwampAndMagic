#include "base_objects.h"


using namespace std;



Object::Object() {

}

Object::Object(int arow, int acol) : row(arow), col(acol), prev_row(arow), prev_col(acol) {
	
}

Object::~Object() {

}

// if two objects are NEAR but do not match
bool Object::operator%(const Object& obj) {
	return 
		(abs(row - obj.row) == 1 && abs(col - obj.col) == 0) ||
		(abs(row - obj.row) == 0 && abs(col - obj.col) == 1);
}

bool Object::operator==(const Object& obj) {
	return row == obj.row && col == obj.col;
}

int Object::getrow() {
	return row;
}

int Object::getcol() {
	return col;
}

int Object::prevrow() {
	return prev_row;
}

int Object::prevcol() {
	return prev_col;
}

void Object::move_to_prev() {
	row = prev_row;
	col = prev_col;
}

char Object::symbol() {
	return SYM_EMPTY;
}	

bool Object::is_penetrable() {
	return true;
}

bool Object::is_evil() {
	return true;
}





ObjectPtr Map::operator<<(ObjectPtr obj) {
	int row = obj->getrow();
	int col = obj->getcol();
	map[row][col].push_back(obj);
	return obj;
}

ostream& operator<<(ostream& display, Map& m) {
	for(int i = 0; i < m.get_height(); i++) {
		for(int j = 0; j < m.get_width(); j++) {
			display << m[i][j].back()->symbol();
		}
		display << endl;
	}
	return display;
}

list<ObjectPtr>* Map::operator[](int index) {
	return map[index];
}

int Map::get_height() {
	return height;
}

int Map::get_width() {
	return width;
}	

bool Map::is_penetrable(int arow, int acol) {
	bool is_p = true;
	for(auto obj: map[arow][acol]) {
		is_p &= obj->is_penetrable();
	}
	return is_p;
}
