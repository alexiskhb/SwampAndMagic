#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <ctime>


#define chance(a) (rand()%1000 + 1 <= 10*a)

// map is 4-connected area

static const char SYM_EMPTY    = ' ';
static const char SYM_WALL     = '#';
static const char SYM_KNIGHT   = 'K';
static const char SYM_PRINCESS = 'P';
static const char SYM_DRAGON   = 'D';
static const char SYM_ZOMBIE   = 'z';
static const char SYM_SWAMP    = '~';
static const char SYM_FLAME    = '*';
static const char SYM_MAGIC    = '%';

static const int MAP_HEIGHT  = 30; 
static const int MAP_WIDTH   = 60; 

static const int HP_KNIGHT   = 50; 
static const int HP_PRINCESS = 2;  
static const int HP_DRAGON   = 200;
static const int HP_ZOMBIE   = 20; 

static const int DMG_KN_SWORD = 7*100; 
static const int DMG_KN_MAGIC = 6*10; 
static const int DMG_PRINCESS = 0; 
static const int DMG_DRAGON   = 15;
static const int DMG_FIRE     = 4; 
static const int DMG_ZOMBIE   = 3;



class Map;



class Object {
public:
	Object();

	Object(int arow, int acol);

	virtual ~Object();

	// if two objects are NEAR but do not match
	bool operator%(const Object& obj);

	bool operator==(const Object& obj);

	int getrow();

	int getcol();

	int prevrow();

	int prevcol();

	void move_to_prev();

	virtual char symbol();

	virtual bool is_penetrable();

	virtual bool is_evil();
protected:
	int row = -1;
	int col = -1;
	// we need prev_coords to remove reference to object
	// from previous cell of map after move
	int prev_row = -1;
	int prev_col = -1;
	// for lifetime -1 means infinity
	int lifetime = -1;
	int damage = 0;
};



typedef std::shared_ptr<Object> ObjectPtr;



class Map {
public:
	ObjectPtr operator<<(ObjectPtr obj);

	std::list<ObjectPtr>* operator[](int index);

	bool is_penetrable(int arow, int acol);

	int get_height();

	int get_width();	
private:
	int height = MAP_HEIGHT;
	int width  = MAP_WIDTH;
	std::list<ObjectPtr> map[MAP_HEIGHT][MAP_WIDTH];
};

std::ostream& operator<<(std::ostream& display, Map& m);