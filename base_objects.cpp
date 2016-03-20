#include "base_objects.h"
#include "objects.h"


using namespace std;

void log(const char* s) {
	return;
	fprintf(stderr, "%s\n", s);
}

int BaseObject::turn = 0;

bool Room::map_stencil[MAP_HEIGHT][MAP_WIDTH];
bool Room::newmap[MAP_HEIGHT][MAP_WIDTH];

unsigned int cantor_pairing(const int a, const int b) {
	unsigned int A = a >= 0 ? 2*static_cast<unsigned int>(a) : -2*static_cast<unsigned int>(a) - 1;
	unsigned int B = b >= 0 ? 2*static_cast<unsigned int>(b) : -2*static_cast<unsigned int>(b) - 1;
	return ((A + B)*(A + B + 1))/2 + A;
}

unsigned int cantor_pairing(GCoord acoord) {
	return cantor_pairing(acoord.row, acoord.col);
}


bool chance(int a) {
	a = 101 - a;
	int result = rand()%1000 + 1;
	return result >= 10*a;
}

BaseObject::BaseObject() {
	fsymb = SYM_EMPTY | COLOR_PAIR(ID_EMPTY) | A_REVERSE | A_BOLD;
}

BaseObject::BaseObject(GCoord acoord) : coord(acoord), prev_coord(acoord) {
	fsymb = SYM_EMPTY | COLOR_PAIR(ID_EMPTY) | A_REVERSE | A_BOLD;	
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

void BaseObject::set_prev(GCoord acoord) {
	prev_coord = acoord;
}

void BaseObject::moveto(GCoord acoord) {
	set_prev(coord);
	coord = acoord;
}

void BaseObject::move_to_prev() {
	coord = prev_coord;
}

void BaseObject::destroy() {
	health = 0;
}

bool BaseObject::is_alive() {
	return health > 0;
}

char BaseObject::symbol() {
	return SYM_EMPTY;
}

chtype BaseObject::symb() {
	return fsymb;
}

bool BaseObject::is_penetrable() {
	return true;
}

bool BaseObject::is_evil() {
	return true;
}




Room::Room(GCoord acoord) : coord(acoord) {
	log("room");
}

Room::~Room() {
	log("destroy room");
}

Room& Room::generate(BaseList& relief, Map& m, std::list<ObjectPtr>& objects, int steps) {
	srand(time(0));
	int achance = chance(70) ? 47+rand()%5 : 50+rand()%4;
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			Room::map_stencil[i][j] = chance(achance);
		}
	}

	for(int i = 0; i < steps; i++) {
		gen_step();
	}

	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			Coord c(coord.parts.x, coord.parts.y, i, j);
			relief.push_back(make_shared<Object>(GCoord(c)));
			m << relief.back();
		}
	}

	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			if (gen_is_wall(i, j)) {
				Coord c(coord.parts.x, coord.parts.y, i, j);
				relief.push_back(make_shared<Wall>(GCoord(c)));
				m << relief.back(); 
			}
		}
	}

	int row = rand()%MAP_HEIGHT;
	int col = rand()%MAP_WIDTH;
	GCoord crd = m.nearest_symb(GCoord(Coord(coord.parts.x, coord.parts.y, row, col)), string(1, SYM_EMPTY), MAP_HEIGHT/2)->get_coord();
	objects.push_back(make_shared<DragonNest>(crd));

	row = rand()%MAP_HEIGHT;
	col = rand()%MAP_WIDTH;
	crd = m.nearest_symb(GCoord(Coord(coord.parts.x, coord.parts.y, row, col)), string(1, SYM_EMPTY), MAP_HEIGHT/2)->get_coord();
	objects.push_back(make_shared<Graveyard>(crd));

	row = rand()%MAP_HEIGHT;
	col = rand()%MAP_WIDTH;
	crd = m.nearest_symb(GCoord(Coord(coord.parts.x, coord.parts.y, row, col)), string(1, SYM_EMPTY), MAP_HEIGHT/2)->get_coord();
	objects.push_back(make_shared<Ziggurat>(crd));
	return *this;
}

bool Room::gen_is_wall(int arow, int acol) {
	return Room::map_stencil[arow][acol];
}

int Room::gen_alive_count(int arow, int acol) {
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
			if (Room::map_stencil[nr][nc]) {
				++result;
			}			
		}
	}
	return result;
}

void Room::gen_step() {
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			int alive_cnt = gen_alive_count(i, j);
			Room::newmap[i][j] = Room::map_stencil[i][j] ? (alive_cnt >= 4) : (alive_cnt > 4);
		}
	}
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			Room::map_stencil[i][j] = Room::newmap[i][j];
		}
	}
}

GCoord Room::get_coord() {
	return coord;
}

BaseList* Room::operator[](int arow) {
	return map[arow];
}




Map::Map(BaseList& arelief) : upper_left_corner(0, 0), bottom_rgt_corner(MAP_HEIGHT-1, MAP_WIDTH-1), relief(arelief) {

}

Map::~Map() {

}

void Map::create_room(const int ax, const int ay, std::list<ObjectPtr>& objects) {
	if (is_room_exists[cantor_pairing(ax, ay)]) {
		return;
	}
	world[ax][ay] = std::make_shared<Room>(GCoord(Coord(ax, ay, 0, 0)));
	world[ax][ay]->generate(relief, *this, objects, 6);
	is_room_exists[cantor_pairing(ax, ay)] = true;
}

void Map::move_the_frame(GCoord shift) {
	upper_left_corner += shift;
	bottom_rgt_corner += shift;
}

BaseObjectPtr Map::operator<<(BaseObjectPtr obj) {
	GCoord coord(obj->getrow(), obj->getcol());
	Room& room = *world[coord.parts.x][coord.parts.y];
	BaseList& cell = room[coord.parts.row][coord.parts.col];
	// non-penetrable objects should be in the back of the list
	if (obj->is_penetrable()) {
		cell.insert(next(cell.begin()), obj);
	}
	else {
		cell.push_back(obj);
	}
	return obj;
}

void Map::create_rooms(std::list<ObjectPtr>& objects) {
	create_room(upper_left_corner.parts.x, upper_left_corner.parts.y, objects);
	create_room(bottom_rgt_corner.parts.x, upper_left_corner.parts.y, objects);
	create_room(bottom_rgt_corner.parts.x, bottom_rgt_corner.parts.y, objects);
	create_room(upper_left_corner.parts.x, bottom_rgt_corner.parts.y, objects);
}

void Map::display(const int shift) {
	static GCoord coords[MAP_HEIGHT][MAP_WIDTH];
	for(int i = upper_left_corner.row, ii = 0; i <= bottom_rgt_corner.row; i++, ii++) {
		for(int j = upper_left_corner.col, jj = 0; j <= bottom_rgt_corner.col; j++, jj++) {
			coords[ii][jj] = GCoord(i, j);
		}
	}
	GCoord cur_coord;
	for(int i = 0; i < MAP_HEIGHT; i++) {
		for(int j = 0; j < MAP_WIDTH; j++) {
			cur_coord = coords[i][j];
			move(i + shift, j + shift);
			Room& room = *world[cur_coord.parts.x][cur_coord.parts.y];
			addch(room[cur_coord.parts.row][cur_coord.parts.col].back()->symb());
		}
	}
	refresh();
}

IntIntPairList Map::shortest_way(IntIntPair from, IntIntPair to, int max_length) {
	IntIntPairList way;
	int r = from.first;
	int c = from.second;
	if ((r - to.first)*(r - to.first) + (c - to.second)*(c - to.second) > max_length*max_length) {
		return way;
	}
	IntIntPairList temp_way;
	GCoord crd;
	int d = 0;
	bool found = false;
	IntIntPair target;
	temp_way.push_back(IntIntPair(r, c));
	while (temp_way.size() > 0) {
		r = temp_way.front().first;
		c = temp_way.front().second;
		if ((abs(r - to.first) <= 1 && abs(c - to.second) <= 1) || d >= max_length) {
			found = true;
			target = IntIntPair(r, c);
			break;
		}
		d = get_distance(GCoord(r, c));
		temp_way.pop_front();
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				crd = GCoord(r + i, c + j);
				if (is_on_the_map(crd) && get_distance(crd) == 0 && is_penetrable(crd)) {
					temp_way.push_back(IntIntPair(r + i, c + j));
					set_distance(crd, d + 1);
				}
			}
		}
	}
	if (found) {
		r = target.first;
		c = target.second;
		d = get_distance(GCoord(r, c));
		// Without bound it sometimes get into infinite loop, hz
		int bound = 0;
		while (d > 0 && bound < 300) {
			++bound;
			bool continue_ij = true;
			way.push_front(IntIntPair(r, c));
			for(int i = -1; i <= 1 && continue_ij; i++) {
				for(int j = -1; j <= 1 && continue_ij; j++) {
					if (i == 0 && j == 0) {
						continue;
					}
					if (get_distance(GCoord(r + i, c + j)) == d - 1) {
						r = r + i;
						c = c + j;
						--d;
						continue_ij = false;
					}	
				}
			}
		}
	}
	distance.clear();
	return way;
}

BaseObjectPtr Map::nearest_symb(GCoord from, std::string targets, int max_length) {
	IntIntPairList deque;
	int r = from.row;
	int c = from.col;
	int d = 0;
	GCoord crd;
	bool found = false;
	IntIntPair target;
	deque.push_back(IntIntPair(r, c));
	while (deque.size() > 0) {
		log("nearest");
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
		d = get_distance(GCoord(r, c));
		deque.pop_front();
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				crd = GCoord(r + i, c + j);
				if (is_on_the_map(crd) && get_distance(crd) == 0) {
					deque.push_back(IntIntPair(r + i, c + j));
					set_distance(crd, d + 1);
				}
			}
		}
	}
	distance.clear();
	if (found) {
		return map(r, c).back();
	}
	else {
		return map(from.row, from.col).back();
	}
}

bool Map::is_penetrable(GCoord acoord) {
	// we know that impenetrable objects should be in the back of the list
	return map(acoord.row, acoord.col).back()->is_penetrable();
}

bool Map::is_far(BaseObjectPtr obj1, BaseObjectPtr obj2, int distance) {
	return 
		abs(obj1->get_coord().parts.x - obj2->get_coord().parts.x) >= distance ||
		abs(obj1->get_coord().parts.y - obj2->get_coord().parts.y) >= distance;
}

bool Map::is_out_of_display(BaseObjectPtr obj) {
	return
		obj->get_coord().row < upper_left_corner.row ||
		obj->get_coord().col < upper_left_corner.col ||
		obj->get_coord().row > bottom_rgt_corner.row ||
		obj->get_coord().col > bottom_rgt_corner.col;
}

BaseList& Map::map(GCoord acoord) {
	Room& room = *world[acoord.parts.x][acoord.parts.y];
	return room[acoord.parts.row][acoord.parts.col];
}

BaseList& Map::map(const int arow, const int acol) {
	return map(GCoord(arow, acol));
}

BaseList& Map::operator()(const int row, const int col) {
	return map(row, col);
}

BaseList& Map::operator()(GCoord acoord) {
	return map(acoord);
}

bool Map::is_on_the_map(GCoord acoord) {
	return is_room_exists[cantor_pairing(acoord.parts.x, acoord.parts.y)];
}

void Map::show_global_map(GMapSpecialList& glob_map_spec, int drawrow, int drawcol) {
	int most_left = world.most_left_used();
	int most_up   = world.most_up_used();
	int x, y;
	chtype ch = SYM_WALL | COLOR_PAIR(ID_WALL);
	for(auto room = world.begin(); room != world.end(); ++room) {
		x = room->get_coord().parts.x;
		y = room->get_coord().parts.y;
		move(drawrow + y - most_up, drawcol + x - most_left);
		addch(ch);
	}
	for(auto p: glob_map_spec) {
		x = p.first->get_coord().parts.x;
		y = p.first->get_coord().parts.y;
		ch = p.second;
		move(drawrow + y - most_up, drawcol + x - most_left);
		addch(ch);
	}
	refresh();
}

void Map::set_distance(GCoord acoord, int value) {
	distance[cantor_pairing(acoord)] = value;
}

int Map::get_distance(GCoord acoord) {
	return distance[cantor_pairing(acoord)];
}

void Map::remove(BaseObjectPtr obj) {
	(*this)(obj->get_prev()).remove(obj);
	(*this)(obj->get_coord()).remove(obj);
}