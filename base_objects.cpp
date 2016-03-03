#include "base_objects.h"


using namespace std;

bool chance(int a, std::string s) {
	a = 101 - a;
	int result = rand()%1000 + 1;
	if (s.size() > 0) {
		cout << "result of " << s << " is " << result/10 << ". need " << a << endl;
	}
	return result >= 10*a;
}

BaseObject::BaseObject() {

}

BaseObject::BaseObject(int arow, int acol) : row(arow), col(acol), prev_row(arow), prev_col(acol) {
	
}

BaseObject::~BaseObject() {

}

// if two objects are NEAR but do not match
bool BaseObject::operator%(const BaseObject& obj) {
	return 
		(abs(row - obj.row) == 1 && abs(col - obj.col) == 0) ||
		(abs(row - obj.row) == 0 && abs(col - obj.col) == 1);
}

bool BaseObject::operator==(const BaseObject& obj) {
	return row == obj.row && col == obj.col;
}

int BaseObject::getrow() {
	return row;
}

int BaseObject::getcol() {
	return col;
}

int BaseObject::prevrow() {
	return prev_row;
}

int BaseObject::prevcol() {
	return prev_col;
}

void BaseObject::move_to_prev() {
	row = prev_row;
	col = prev_col;
}

char BaseObject::symbol() {
	return SYM_EMPTY;
}	

bool BaseObject::is_penetrable() {
	return true;
}

bool BaseObject::is_evil() {
	return true;
}





BaseObjectPtr Map::operator<<(BaseObjectPtr obj) {
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

list<BaseObjectPtr>* Map::operator[](int index) {
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
