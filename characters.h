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

	~Character() override;

	void slash(std::list<CharacterPtr>& characters);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m);

	// returns True if character died
	virtual bool suffer(int dmg);

	virtual bool move(Map& m, std::list<CharacterPtr>& characters) = 0;

	// all characters are impenetrable
	bool is_penetrable() override;
};



class Knight : public Character {
public:
	Knight(int arow, int acol);

	Knight(int arow, int acol, int hp, int dmg);

	~Knight() override;

	char symbol() override;

	bool is_evil() override;

	void magic(std::list<ObjectPtr>& objects, char direction);

	bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;

	bool move(Map& m, std::list<CharacterPtr>& characters) override;

	bool suffer(int dmg) override;
private:
	char moved_on_attack;
};





class Princess : public Character {
public:
	Princess(int arow, int acol);

	Princess(int arow, int acol, int hp, int dmg);

	~Princess() override;

	char symbol() override;
	
	bool suffer(int dmg) override;

	bool move(Map& m, std::list<CharacterPtr>& characters) override;
};





class Monster : public Character {
public:
	Monster(int arow, int acol);

	Monster(int arow, int acol, int hp, int dmg);

	~Monster() override;

	bool move(Map& m, std::list<CharacterPtr>& characters) override;

	void refresh_way(Map& m, std::list<CharacterPtr>& characters);

	IntIntPairList shortest_way_to(BaseObjectPtr obj, Map& m);
protected:
	IntIntPairList way;
};





class Dragon : public Monster {
public:
	Dragon(int arow, int acol);

	Dragon(int arow, int acol, int hp, int dmg);

	~Dragon() override;

	void magic(std::list<ObjectPtr>& objects);

	bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;

	char symbol() override;

	bool suffer(int dmg) override;
};





class Zombie : public Monster {
public:
	Zombie(int arow, int acol);

	Zombie(int arow, int acol, int hp, int dmg);

	~Zombie() override;

	char symbol() override;

	bool suffer(int dmg) override;

	bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};
