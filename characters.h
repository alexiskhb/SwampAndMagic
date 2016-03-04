#pragma once

#include "base_objects.h"
#include "objects.h"
#include <list>


class Character : public BaseObject {
public:
	Character(int arow, int acol);

	Character(int arow, int acol, int hp, int dmg);

	virtual ~Character();

	void slash(std::list<CharacterPtr>& characters, CharacterPtr self);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	int hitpoints();

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

	void magic(char direction);

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

	void magic();

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	virtual char symbol();

	virtual bool suffer(int dmg);

	virtual bool move(Map& m, std::list<CharacterPtr>& characters);
};





class Zombie : public Monster {
public:
	Zombie(int arow, int acol);

	Zombie(int arow, int acol, int hp, int dmg);

	virtual ~Zombie();

	virtual char symbol();

	virtual bool suffer(int dmg);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, CharacterPtr self);

	virtual bool move(Map& m, std::list<CharacterPtr>& characters);
};
