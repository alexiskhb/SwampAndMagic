#pragma once

#include <vector>
#include <list>
#include <string>
#include <memory>
#include <ctime>
#include <algorithm>
#include <map>
#include <string>
#include "colored_text.h"
#include "coords.h"
#include "bilateral_array.h"
#include <curses.h>
#include <iostream>
#include <cstdio>

// map is 8-connected area

void log(const char* s);

static const char 
	SYM_EMPTY    = ' ',
	SYM_WALL     = '.',
	SYM_KNIGHT   = 'K',
	SYM_PRINCESS = 'P',
	SYM_DRAGON   = 'D',
	SYM_ZOMBIE   = 'z',
	SYM_WARLOCK  = 'W',
	SYM_SWAMP    = '~',
	SYM_FLAME    = '*',
	SYM_MAGIC    = '%',
	SYM_CURSE    = 'X',
	SYM_MEDKIT   = '+',
	SYM_DRGNEST  = '@',
	SYM_ZIGGURAT = '#',
	SYM_GRVYARD  = '~';




class Map;

class BaseObject;

class Object;

class Character;

class Room;


typedef std::shared_ptr<BaseObject> BaseObjectPtr;

typedef std::shared_ptr<Object> ObjectPtr;

typedef std::shared_ptr<Character> CharacterPtr;

typedef std::shared_ptr<Map> MapPtr;

typedef std::shared_ptr<Room> RoomPtr;

typedef std::shared_ptr<std::list<BaseObjectPtr>> ListBaseObjPtr;

typedef std::list<BaseObjectPtr> BaseList;

typedef std::pair<int,int> IntIntPair;

typedef std::list<IntIntPair> IntIntPairList;

typedef std::map<unsigned int, int> UIntIntMap;

typedef std::pair<unsigned int, int> UIntIntPair;

typedef std::map<unsigned int, bool> UIntBoolMap;

typedef std::pair<unsigned int, bool> UIntBoolPair;

typedef std::pair<BaseObjectPtr, chtype> GMapSpecPair;

typedef std::list<GMapSpecPair> GMapSpecialList;



extern unsigned int cantor_pairing(const int a, const int b);

extern bool chance(int a);



class BaseObject {
public:
	BaseObject();

	BaseObject(GCoord acoord);

	virtual ~BaseObject();

	// if two objects are NEAR but do not match
	bool operator%(const BaseObject& obj);

	bool operator==(const BaseObject& obj);

	int getrow();

	int getcol();

	int prevrow();

	int prevcol();

	int hitpoints();

	void move_to_prev();

	void set_prev(GCoord acoord);

	void moveto(GCoord acoord);

	GCoord get_coord();

	GCoord get_prev();

	chtype symb();

	virtual void destroy();

	virtual bool is_alive();

	virtual char symbol();

	virtual bool is_penetrable();

	virtual bool is_evil();

	static int turn;
protected:
	GCoord coord;
	// we need prev_coords to remove reference to object
	// from previous cell of map after move
	GCoord prev_coord;
	int health = 1;
	int damage = 0;
	chtype fsymb;
};



class Room {
public:
	Room(GCoord acoord);

	~Room();

	Room& generate(BaseList& relief, Map& m, std::list<ObjectPtr>& objects, int steps);

	GCoord get_coord();

	BaseList* operator[](int arow);
private:
	int  gen_alive_count(int arow, int acol);

	void gen_step();

	bool gen_is_wall(int arow, int acol);

	BaseList map[MAP_HEIGHT][MAP_WIDTH];

	static bool map_stencil[MAP_HEIGHT][MAP_WIDTH];

	static bool newmap[MAP_HEIGHT][MAP_WIDTH];

	GCoord coord;
};



class Map {
public:
	Map(BaseList& relief);

	~Map();

	BaseObjectPtr operator<<(BaseObjectPtr obj);

	bool is_penetrable(GCoord acoord);

	bool is_on_the_map(GCoord acoord);

	void create_room(const int ax, const int ay, std::list<ObjectPtr>& objects);

	void create_rooms(std::list<ObjectPtr>& objects);

	IntIntPairList shortest_way(IntIntPair from, IntIntPair to, int max_length);

	BaseObjectPtr nearest_symb(GCoord from, std::string targets, int max_length);

	BaseList& map(const int arow, const int acol);

	BaseList& map(GCoord acoord);

	BaseList& operator()(const int row, const int col);

	BaseList& operator()(GCoord acoord);

	void move_the_frame(GCoord shift);

	void display(int shift);

	void show_global_map(GMapSpecialList& gmsl, int drawrow, int drawcol);

	bool is_far(BaseObjectPtr obj1, BaseObjectPtr obj2, int distance);

	bool is_out_of_display(BaseObjectPtr obj);

	void remove(BaseObjectPtr obj);
private:
	void set_distance(GCoord acoord, int value);

	int  get_distance(GCoord acoord);

	BilateralArray2D<RoomPtr> world;
	UIntIntMap distance;
	UIntIntMap is_room_exists;
	int shortest_distance = MAP_WIDTH*MAP_WIDTH + MAP_HEIGHT*MAP_HEIGHT;
	GCoord upper_left_corner;
	GCoord bottom_rgt_corner;
	BaseList& relief;
};