#pragma once

#include "base_objects.h"
#include <list>
#include <string>
#include "colored_text.h"


static const int HP_KNIGHT   = 70*100; 
static const int HP_PRINCESS = 2;  
static const int HP_DRAGON   = 200;
static const int HP_ZOMBIE   = 20; 


static const int DMG_KN_SWORD = 8; 
static const int DMG_PRINCESS = 0; 
static const int DMG_DRAGON   = 15;
static const int DMG_ZOMBIE   = 2;


static const char CMD_UP     = 'w';
static const char CMD_DOWN   = 'x';
static const char CMD_LEFT   = 'a';
static const char CMD_RIGHT  = 'd';
static const char CMD_LUP    = 'q';
static const char CMD_LDOWN  = 'z';
static const char CMD_RUP    = 'e';
static const char CMD_RDOWN  = 'c';
static const char CMD_AROUND = 's';
static const char CMD_ATTACK = 'r';
static const char CMD_MAGIC  = 'f';
static const char CMD_NONE   = 'n';
static const char CMD_QUIT   = 'Q';


class Character : public BaseObject {
public:
	Character(int arow, int acol);

	Character(int arow, int acol, int hp, int dmg);

	virtual ~Character();

	void slash(std::list<CharacterPtr>& characters, CharacterPtr self);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	// returns True if character died
	virtual bool suffer(int dmg);

	virtual bool move(Map& m, std::list<CharacterPtr>& characters) = 0;

	// all characters are impenetrable
	virtual bool is_penetrable();
};



class Knight : public Character {
public:
	Knight(int arow, int acol);

	Knight(int arow, int acol, int hp, int dmg);

	virtual ~Knight();

	virtual char symbol();

	virtual bool is_evil();

	void magic(std::list<ObjectPtr>& objects, char direction);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	virtual bool move(Map& m, std::list<CharacterPtr>& characters);

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

	virtual bool move(Map& m, std::list<CharacterPtr>& characters);
};





class Monster : public Character {
public:
	Monster(int arow, int acol);

	Monster(int arow, int acol, int hp, int dmg);

	virtual bool move(Map& m, std::list<CharacterPtr>& characters);

	IntIntPairList shortest_way_to(BaseObjectPtr obj, Map& m);

	virtual ~Monster();
protected:
	IntIntPairList way;
};





class Dragon : public Monster {
public:
	Dragon(int arow, int acol);

	Dragon(int arow, int acol, int hp, int dmg);

	virtual ~Dragon();

	void magic(std::list<ObjectPtr>& objects);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	virtual char symbol();

	virtual bool suffer(int dmg);
};





class Zombie : public Monster {
public:
	Zombie(int arow, int acol);

	Zombie(int arow, int acol, int hp, int dmg);

	virtual ~Zombie();

	virtual char symbol();

	virtual bool suffer(int dmg);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);
};
