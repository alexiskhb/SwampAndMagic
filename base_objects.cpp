#include "base_objects.h"
#include "objects.h"


using namespace std;


unsigned int cantor_pairing(const int a, const int b) {
	unsigned int A = a >= 0 ? 2*static_cast<unsigned int>(a) : -2*static_cast<unsigned int>(a) - 1;
	unsigned int B = b >= 0 ? 2*static_cast<unsigned int>(b) : -2*static_cast<unsigned int>(b) - 1;
	return ((A + B)*(A + B + 1))/2 + A;
}


bool chance(int a, std::string s) {
	a = 101 - a;
	int result = rand()%1000 + 1;
	if (s.size() > 0) {
		cout << "result of " << s << " is " << result/10 << ". need " << a << endl;
	}
	return result >= 10*a;
}

BaseObject::BaseObject() {
	fcolor = Colored(BG_WHITE, FG_BLACK).to_string();
}

BaseObject::BaseObject(int arow, int acol) : coord(arow, acol), prev_coord(arow, acol) {
	fcolor = Colored(BG_WHITE, FG_BLACK).to_string();	
}

BaseObject::~BaseObject() {

}

// if two objects are NEAR but do not match
bool BaseObject::operator%(const BaseObject& obj) {
	return 
		(abs(coord.row - obj.coord.row) <= 1 && abs(coord.col - obj.coord.col) <= 1) &&
		!(coord.row == obj.coord.row && coord.col == obj.coord.col);
}

bool BaseObject::operator==(const BaseObject& obj) {
	return coord.row == obj.coord.row && coord.col == obj.coord.col;
}

int BaseObject::getrow() {
	return coord.row;
}

int BaseObject::getcol() {
	return coord.col;
}

int BaseObject::prevrow() {
	return prev_coord.row;
}

int BaseObject::prevcol() {
	return prev_coord.col;
}

int BaseObject::hitpoints() {
	return health;
}

GCoord BaseObject::get_coord() {
	return GCoord(getrow(), getcol());
}

GCoord BaseObject::get_prev() {
	return GCoord(prevrow(), prevcol());
}

void BaseObject::move_to_prev() {
	coord.row = prev_coord.row;
	coord.col = prev_coord.col;
}

bool BaseObject::is_alive() {
	return health > 0;
}

char BaseObject::symbol() {
	return SYM_EMPTY;
}

std::string& BaseObject::color() {
	return fcolor;
}

bool BaseObject::is_penetrable() {
	return true;
}

bool BaseObject::is_evil() {
	return true;
}




Room::Room() {

}

Room::~Room() {

}




Map::Map(BaseList& arelief) : upper_left_corner(0, 0), bottom_rgt_corner(MAP_HEIGHT-1, MAP_WIDTH-1), relief(arelief) {
	create_room(0, 0);
}

Map::~Map() {
	static int k = 1;
	for(auto iter = world.begin(); iter != world.end(); ++iter) {
		if (*iter != nullptr) {
			delete *iter;
			cout << k++ << "-th room destroyed\n";
		}
	}
}

void Map::create_room(const int ax, const int ay) {
	if (is_room_exists[cantor_pairing(ax, ay)]) {
		return;
	}
	cout << ax << ' ' << ay << endl << "================\n";
	world[ax][ay] = new Room();
	generate(47, 6, ax, ay);
	is_room_exists[cantor_pairing(ax, ay)] = true;
}

void Map::move_the_frame(GCoord shift) {
	upper_left_corner += shift;
	bottom_rgt_corner += shift;
}

BaseObjectPtr Map::operator<<(BaseObjectPtr obj) {
	GCoord coord(obj->getrow(), obj->getcol());
	// cout << obj->getrow() << ' ' << obj->getcol() << endl;
	// cout << coord.parts.row << ' ' << coord.parts.col << endl;
	// cout << coord.parts.x << ' ' << coord.parts.y << endl;
	Room& room = *world[coord.parts.x][coord.parts.y];
	BaseList& cell = room.map[coord.parts.row][coord.parts.col];
	// impenetrable objects should be in the back of the list
	if (obj->is_penetrable()) {
		cell.insert(next(cell.begin()), obj);
	}
	else {
		cell.push_back(obj);
	}
	return obj;
}

ostream& operator<<(ostream& display, Map& m) {
	m.create_room(m.upper_left_corner.parts.x, m.upper_left_corner.parts.y);
	m.create_room(m.bottom_rgt_corner.parts.x, m.upper_left_corner.parts.y);
	m.create_room(m.bottom_rgt_corner.parts.x, m.bottom_rgt_corner.parts.y);
	m.create_room(m.upper_left_corner.parts.x, m.bottom_rgt_corner.parts.y);
	GCoord cur_coord;
	for(int i = m.upper_left_corner.row; i <= m.bottom_rgt_corner.row; i++) {
		for(int j = m.upper_left_corner.col; j <= m.bottom_rgt_corner.col; j++) {
			cur_coord = GCoord(i, j);
			Room& room = *m.world[cur_coord.parts.x][cur_coord.parts.y];
			if (room.map[cur_coord.parts.row][cur_coord.parts.col].back()->color().size() > 10) {
				cout << endl << room.map[cur_coord.parts.row][cur_coord.parts.col].back()->color().size() << endl;
				cout << (room.map[cur_coord.parts.row][cur_coord.parts.col].size() == 0) << endl;
				exit(0);
			}
			display << room.map[cur_coord.parts.row][cur_coord.parts.col].back()->color();
			display << room.map[cur_coord.parts.row][cur_coord.parts.col].back()->symbol();
		}
		display << Colored() << endl;
	}
	return display;
}

int Map::get_height() {
	return height;
}

int Map::get_width() {
	return width;
}

void Map::clear_distances() {
	distance.clear();
}

IntIntPairList Map::shortest_way(IntIntPair from, IntIntPair to, int max_length) {
	IntIntPairList way;
	IntIntPairList temp_way;
	int r = from.first;
	int c = from.second;
	if ((r - to.first)*(r - to.first) + (c - to.second)*(c - to.second) > max_length*max_length) {
		return way;
	}
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
				if (is_on_the_map(r + i, c + j) && get_distance(r + i, c + j) == 0 && is_penetrable(r + i, c + j)) {
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

void Map::set_distance(int arow, int acol, int value) {
	distance[cantor_pairing(arow, acol)] = value;
}

int Map::get_distance(int arow, int acol) {
	return distance[cantor_pairing(arow, acol)];
}

BaseObjectPtr Map::nearest_symb(IntIntPair from, std::string targets, int max_length) {
	IntIntPairList deque;
	GCoord coord;
	int r = from.first;
	int c = from.second;
	int d = 0;
	bool found = false;
	IntIntPair target;
	deque.push_back(IntIntPair(r, c));
	while (deque.size() > 0) {
		r = deque.front().first;
		c = deque.front().second;
		char p = map(r, c).back()->symbol();
		for(unsigned int i = 0; i < targets.size() && !found; i++) {
			found |= targets.find(p) != std::string::npos;
		}
		if (found || d >= max_length) {
			target = IntIntPair(r, c);
			break;
		}
		d = get_distance(r, c);
		deque.pop_front();
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				if (is_on_the_map(r + i, c + j) && get_distance(r + i, c + j) == 0) {
					deque.push_back(IntIntPair(r + i, c + j));
					set_distance(r + i, c + j, d + 1);
				}
			}
		}
	}
	clear_distances();
	if (found) {
		return map(r, c).back();
	}
	else {
		return map(from.first, from.second).back();
	}
}

bool Map::is_penetrable(int arow, int acol) {
	// we know that impenetrable objects should be in the back of the list
	return map(arow, acol).back()->is_penetrable();
}

BaseList& Map::map(const int arow, const int acol) {
	GCoord coord(arow, acol);
	Room& room = *world[coord.parts.x][coord.parts.y];
	return room.map[coord.parts.row][coord.parts.col];
}

BaseList& Map::operator()(const int row, const int col) {
	return map(row, col);
}

void Map::generate(int achance, int steps, int ax, int ay) {
	srand(time(0));
	GCoord coord;
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			map_stencil[i][j] = chance(achance, "");
		}
	}
	for(int i = 0; i < steps; i++) {
		gen_step();
	}

	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			coord = GCoord(Coord(ax, ay, i, j));
			relief.push_back(ObjectPtr(new Object(coord.row, coord.col)));
			*this << relief.back();
		}
	}

	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			if (gen_is_wall(i, j)) {
				coord = GCoord(Coord(ax, ay, i, j));
				relief.push_back(BaseObjectPtr(new Wall(coord.row, coord.col)));
				*this << relief.back(); 
			}
		}
	}
}

bool Map::gen_is_wall(int arow, int acol) {
	return map_stencil[arow][acol];
}

int Map::gen_alive_count(int arow, int acol) {
	int result = 0;
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			int nr = arow + i, nc = acol + j;
			if (nr == arow && nc == acol) {
				continue;
			}
			if (nr < 0 || nc < 0 || nr >= MAP_HEIGHT || nc >= MAP_WIDTH) {
				// ++result;
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

bool Map::is_on_the_map(int arow, int acol) {
	GCoord coord(arow, acol);
	return is_room_exists[cantor_pairing(coord.parts.x, coord.parts.y)];
}