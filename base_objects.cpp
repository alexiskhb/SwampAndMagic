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

bool BaseObject::is_alive() {
	return health > 0;
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
		map[row][col].insert(next(map[row][col].begin()), obj);
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
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				if (get_distance(r + i, c + j) == 0 && is_on_the_map(r + i, c + j) && is_penetrable(r + i, c + j)) {
					temp_way.push_back(IntIntPair(r + i, c + j));
					set_distance(r + i, c + j, d + 1);
				}
			}
		}
		
	}
	if (found) {
		r = target.first;
		c = target.second;
		d = get_distance(r, c);
		while (d > 0) {
			bool continue_ij = true;
			way.push_front(IntIntPair(r, c));
			for(int i = -1; i <= 1 && continue_ij; i++) {
				for(int j = -1; j <= 1 && continue_ij; j++) {
					if (i == 0 && j == 0) {
						continue;
					}
					if (get_distance(r + i, c + j) == d - 1) {
						r = r + i;
						c = c + j;
						--d;
						continue_ij = false;
					}	
				}
			}
		}
	}
	clear_distances();
	return way;
}

bool Map::is_penetrable(int arow, int acol) {
	// we know that impenetrable objects should be in the back of the list
	return map[arow][acol].back()->is_penetrable();
}

void Map::generate(int achance, int steps) {
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			map_stencil[i][j] = chance(achance, "");
		}
	}
	for(int i = 0; i < steps; i++) {
		gen_step();
	}
}

bool Map::gen_is_wall(int arow, int acol) {
	return map_stencil[arow][acol];
}

int Map::gen_alive_count(int row, int col) {
	int result = 0;
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			int nr = row + i, nc = col + j;
			if (nr == row && nc == col) {
				continue;
			}
			if (nr < 0 || nc < 0 || nr >= MAP_HEIGHT || nc >= MAP_WIDTH) {
				++result;
				continue;
			}
			if (map_stencil[nr][nc]) {
				++result;
			}			
		}
	}
	return result;
}

void Map::gen_step() {
	bool newmap[MAP_HEIGHT][MAP_WIDTH];
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			int alive_cnt = gen_alive_count(i, j);
			newmap[i][j] = map_stencil[i][j] ? (alive_cnt >= 4) : (alive_cnt > 4);
		}
	}
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			map_stencil[i][j] = newmap[i][j];
		}
	}
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