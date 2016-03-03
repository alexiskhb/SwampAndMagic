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
	// impenetrable objects should be in the back of the list
	if (obj->is_penetrable()) {
		map[row][col].push_front(obj);
	}
	else {
		map[row][col].push_back(obj);
	}
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

void Map::clear_distances() {
	fill(distance, distance + width*height, 0);
}

IntIntPairList Map::shortest_way(IntIntPair from, IntIntPair to) {
	IntIntPairList way;
	IntIntPairList temp_way;
	int r = from.first;
	int c = from.second;
	int d = 0;
	bool found = false;
	IntIntPair target;
	temp_way.push_back(IntIntPair(r, c));
	while (temp_way.size() > 0) {
		r = temp_way.front().first;
		c = temp_way.front().second;
		if (abs(r - to.first) <= 1 && abs(c - to.second) <= 1) {
			found = true;
			target = IntIntPair(r, c);
			break;
		}
		d = get_distance(r, c);
		temp_way.pop_front();
		if (get_distance(r - 1, c) == 0 && is_on_the_map(r - 1, c) && is_penetrable(r - 1, c)) {
			temp_way.push_back(IntIntPair(r - 1, c));
			set_distance(r - 1, c, d + 1);
		}
		if (get_distance(r, c + 1) == 0 && is_on_the_map(r, c + 1) && is_penetrable(r, c + 1)) {
			temp_way.push_back(IntIntPair(r, c + 1));
			set_distance(r, c + 1, d + 1);
		}
		if (get_distance(r + 1, c) == 0 && is_on_the_map(r + 1, c) && is_penetrable(r + 1, c)) {
			temp_way.push_back(IntIntPair(r + 1, c));
			set_distance(r + 1, c, d + 1);
		}
		if (get_distance(r, c - 1) == 0 && is_on_the_map(r, c - 1) && is_penetrable(r, c - 1)) {
			temp_way.push_back(IntIntPair(r, c - 1));
			set_distance(r, c - 1, d + 1);
		}

		if (get_distance(r - 1, c - 1) == 0 && is_on_the_map(r - 1, c - 1) && is_penetrable(r - 1, c - 1)) {
			temp_way.push_back(IntIntPair(r - 1, c - 1));
			set_distance(r - 1, c - 1, d + 1);
		}
		if (get_distance(r - 1, c + 1) == 0 && is_on_the_map(r - 1, c + 1) && is_penetrable(r - 1, c + 1)) {
			temp_way.push_back(IntIntPair(r - 1, c + 1));
			set_distance(r - 1, c + 1, d + 1);
		}
		if (get_distance(r + 1, c + 1) == 0 && is_on_the_map(r + 1, c + 1) && is_penetrable(r + 1, c + 1)) {
			temp_way.push_back(IntIntPair(r + 1, c + 1));
			set_distance(r + 1, c + 1, d + 1);
		}
		if (get_distance(r + 1, c - 1) == 0 && is_on_the_map(r + 1, c - 1) && is_penetrable(r + 1, c - 1)) {
			temp_way.push_back(IntIntPair(r + 1, c - 1));
			set_distance(r + 1, c - 1, d + 1);
		}
	}
	if (found) {
		r = target.first;
		c = target.second;
		d = get_distance(r, c);
		while (d > 0) {
			way.push_front(IntIntPair(r, c));
			if (get_distance(r - 1, c) == d - 1) {
				r = r - 1;
				--d;
				continue;
			}
			if (get_distance(r, c + 1) == d - 1) {
				c = c + 1;
				--d;
				continue;
			}
			if (get_distance(r + 1, c) == d - 1) {
				r = r + 1;
				--d;
				continue;
			}
			if (get_distance(r, c - 1) == d - 1) {
				c = c - 1;
				--d;
				continue;
			}

			if (get_distance(r - 1, c - 1) == d - 1) {
				r = r - 1;
				c = c - 1;
				--d;
				continue;
			}
			if (get_distance(r - 1, c + 1) == d - 1) {
				r = r - 1;
				c = c + 1;
				--d;
				continue;
			}
			if (get_distance(r + 1, c + 1) == d - 1) {
				r = r + 1;
				c = c + 1;
				--d;
				continue;
			}
			if (get_distance(r + 1, c - 1) == d - 1) {
				r = r + 1;
				c = c - 1;
				--d;
				continue;
			}
			--d;
		}
	}
	clear_distances();
	return way;
}

bool Map::is_penetrable(int arow, int acol) {
	// we know that impenetrable objects should be in the back of the list
	return map[arow][acol].back()->is_penetrable();
}

inline void Map::set_distance(int arow, int acol, int value) {
	distance[arow*width + acol] = value;
}

inline int Map::get_distance(int arow, int acol) {
	return distance[arow*width + acol];
}

inline bool Map::is_on_the_map(int arow, int acol) {
	// TODO
	// remove this line to get an opportunity to throw off monsters
	return true;
	return arow >= 0 && acol >= 0 && arow < height && acol < width;
}