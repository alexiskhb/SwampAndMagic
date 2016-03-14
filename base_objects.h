#pragma once

#include <iostream>
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

// map is 8-connected area

static const char SYM_EMPTY    = ' ';
static const char SYM_WALL     = '.';
static const char SYM_KNIGHT   = 'K';
static const char SYM_PRINCESS = 'P';
static const char SYM_DRAGON   = 'D';
static const char SYM_ZOMBIE   = 'z';
static const char SYM_WARLOCK  = 'W';
static const char SYM_SWAMP    = '~';
static const char SYM_FLAME    = '*';
static const char SYM_MAGIC    = '%';
static const char SYM_CURSE    = 'X';
static const char SYM_MEDKIT   = '+';




class Map;

class BaseObject;

class Object;

class Character;


typedef std::shared_ptr<BaseObject> BaseObjectPtr;

typedef std::shared_ptr<Object> ObjectPtr;

typedef std::shared_ptr<Character> CharacterPtr;

typedef std::shared_ptr<std::list<BaseObjectPtr>> ListBaseObjPtr;

typedef std::list<BaseObjectPtr> BaseList;

typedef std::pair<int,int> IntIntPair;

typedef std::list<IntIntPair> IntIntPairList;

typedef std::map<unsigned int, int> UIntIntMap;

typedef std::pair<unsigned int, int> UIntIntPair;

typedef std::map<unsigned int, bool> UIntBoolMap;

typedef std::pair<unsigned int, bool> UIntBoolPair;



extern unsigned int cantor_pairing(const int a, const int b);

extern bool chance(int a, std::string s);



class BaseObject {
public:
	BaseObject();

	BaseObject(int arow, int acol);

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

	GCoord get_coord();

	GCoord get_prev();

	std::string& color();

	virtual bool is_alive();

	virtual char symbol();

	virtual bool is_penetrable();

	virtual bool is_evil();
protected:
	std::string fcolor;
	GCoord coord;
	// we need prev_coords to remove reference to object
	// from previous cell of map after move
	GCoord prev_coord;
	int health = 1;
	int damage = 0;
};



class Room {
public:
	Room();

	~Room();
	
	BaseList map[MAP_HEIGHT][MAP_WIDTH];	
};



class Map {
public:
	Map(BaseList& relief);

	~Map();

	BaseObjectPtr operator<<(BaseObjectPtr obj);

	// std::list<BaseObjectPtr>* operator[](int index);

	bool is_penetrable(int arow, int acol);

	int get_height();

	int get_width();

	void generate(int achance, int steps, int ax, int ay);

	bool gen_is_wall(int arow, int acol);

	bool is_on_the_map(int arow, int acol);

	void create_room(const int ax, const int ay);

	void create_rooms();

	IntIntPairList shortest_way(IntIntPair from, IntIntPair to, int max_length);

	BaseObjectPtr nearest_symb(IntIntPair from, std::string targets, int max_length);

	BaseList& map(const int arow, const int acol);

	friend std::ostream& operator<<(std::ostream& display, Map& m);

	BaseList& operator()(const int row, const int col);

	void move_the_frame(GCoord shift);
private:
	int  gen_alive_count(int arow, int acol);

	void gen_step();

	void set_distance(int arow, int acol, int value);

	int  get_distance(int arow, int acol);

	BilateralArray2D<Room*> world;
	int height = MAP_HEIGHT;
	int width  = MAP_WIDTH;
	UIntIntMap distance;
	UIntIntMap is_room_exists;
	bool map_stencil[MAP_HEIGHT][MAP_WIDTH];
	int shortest_distance = MAP_WIDTH*MAP_WIDTH + MAP_HEIGHT*MAP_HEIGHT;
	GCoord upper_left_corner;
	GCoord bottom_rgt_corner;
	BaseList& relief;
};