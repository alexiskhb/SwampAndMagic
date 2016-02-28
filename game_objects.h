#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <ctime>

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



class Wall : public Object {
public:
	Wall(int arow, int acol);

	virtual ~Wall();

	virtual char symbol();

	virtual bool is_penetrable();
};





class Flame : public Object {
public:
	Flame(int arow, int acol);

	virtual ~Flame();

	virtual char symbol();

	virtual bool is_penetrable();
};





class Swamp : public Object {
public:
	Swamp(int arow, int acol);

	virtual ~Swamp();

	virtual char symbol();

	virtual bool is_penetrable();
};




// magic can heal knight
class Magic : public Object {
public:
	Magic(int arow, int acol);

	virtual ~Magic();

	virtual char symbol();

	virtual bool is_penetrable();
};





class Character;

typedef std::shared_ptr<Character> CharacterPtr;





class Character : public Object {
public:
	Character(int arow, int acol);

	Character(int arow, int acol, int hp, int dmg);

	virtual ~Character();

	void slash(std::list<CharacterPtr>& characters, CharacterPtr self);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	virtual int hitpoints();

	// returns True if character died
	virtual bool suffer(int dmg);

	virtual bool move() = 0;

	// all characters are impenetrable
	virtual bool is_penetrable();
protected:
	int health = 1;
};





class Knight : public Character {
public:
	Knight(int arow, int acol);

	Knight(int arow, int acol, int hp, int dmg);

	virtual ~Knight();

	virtual char symbol();

	virtual bool is_evil();

	void magic(char direction);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	virtual bool move();

	virtual bool suffer(int dmg);
private:
	char moved_on_attack;
};





class Princess : public Character {
public:
	Princess(int arow, int acol);

	Princess(int arow, int acol, int hp, int dmg);

	virtual ~Princess();

	virtual char symbol();
	
	virtual bool suffer(int dmg);

	virtual bool move();
};





class Monster : public Character {
public:
	Monster(int arow, int acol);

	Monster(int arow, int acol, int hp, int dmg);

	virtual ~Monster();
};





class Dragon : public Monster {
public:
	Dragon(int arow, int acol);

	Dragon(int arow, int acol, int hp, int dmg);

	virtual ~Dragon();

	void magic();

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	virtual char symbol();

	virtual bool suffer(int dmg);

	virtual bool move();
};





class Zombie : public Monster {
public:
	Zombie(int arow, int acol);

	Zombie(int arow, int acol, int hp, int dmg);

	virtual ~Zombie();

	virtual char symbol();

	virtual bool suffer(int dmg);

	virtual bool move();
};
