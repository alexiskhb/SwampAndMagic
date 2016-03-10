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

extern bool chance(int a, std::string s);

// map is 8-connected area

static const char SYM_EMPTY    = ' ';
static const char SYM_WALL     = '.';
static const char SYM_KNIGHT   = 'K';
static const char SYM_PRINCESS = 'P';
static const char SYM_DRAGON   = 'D';
static const char SYM_ZOMBIE   = 'z';
static const char SYM_SWAMP    = '~';
static const char SYM_FLAME    = '*';
static const char SYM_MAGIC    = '%';

static const int MAP_HEIGHT = 30;
static const int MAP_WIDTH  = 60;

static std::map<char, std::string> names;

class Map;
class BaseObject;
class Object;
class Character;

typedef std::shared_ptr<BaseObject> BaseObjectPtr;
typedef std::shared_ptr<Object> ObjectPtr;
typedef std::shared_ptr<Character> CharacterPtr;
typedef std::pair<int,int> IntIntPair;
typedef std::list<IntIntPair> IntIntPairList;


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

	void move_to_prev();

	virtual bool is_alive();

	virtual char symbol();

	virtual std::string& color();

	virtual bool is_penetrable();

	virtual bool is_evil();

	std::string fcolor;
protected:
	int row = 0;
	int col = 0;
	// we need prev_coords to remove reference to object
	// from previous cell of map after move
	int prev_row = 0;
	int prev_col = 0;
	int health = 1;
	int damage = 0;
};



class Map {
public:
	BaseObjectPtr operator<<(BaseObjectPtr obj);

	std::list<BaseObjectPtr>* operator[](int index);

	bool is_penetrable(int arow, int acol);

	int get_height();

	int get_width();

	void generate(int achance, int steps);

	bool gen_is_wall(int arow, int acol);

	bool is_on_the_map(int arow, int acol);

	IntIntPairList shortest_way(IntIntPair from, IntIntPair to);
private:
	int  gen_alive_count(int row, int col);

	void gen_step();

	void clear_distances();

	inline void set_distance(int arow, int acol, int value);

	inline int  get_distance(int arow, int acol);

	int height = MAP_HEIGHT;
	int width  = MAP_WIDTH;
	std::list<BaseObjectPtr> map[MAP_HEIGHT][MAP_WIDTH];
	int distance[MAP_HEIGHT * MAP_WIDTH];
	bool map_stencil[MAP_HEIGHT][MAP_WIDTH];
};

std::ostream& operator<<(std::ostream& display, Map& m);
