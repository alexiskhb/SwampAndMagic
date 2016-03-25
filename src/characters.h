#pragma once

#include "base_objects.h"
#include <list>
#include <string>
#include <algorithm>
#include "colored_text.h"
#include "coords.h"

extern int 
	HP_KNIGHT    ,
	HP_PRINCESS  ,
	HP_DRAGON    ,
	HP_ZOMBIE    ,
	HP_WARLOCK   ,
	DMG_KN_SWORD ,
	DMG_PRINCESS ,
	DMG_DRAGON   ,
	DMG_ZOMBIE   ,
	DMG_WARLOCK  ;


static const std::string ENEMIES("zDW");


class Character : public BaseObject {
public:
	Character(GCoord acoord, int hp, int dmg);

	~Character() override;

	void slash(std::list<CharacterPtr>& characters);

	virtual bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m);

	virtual void suffer(int dmg);

	virtual bool move(Map& m, std::list<CharacterPtr>& characters) = 0;

	// all characters are impenetrable
	bool is_penetrable() override;
private:
	int max_health;
};



class Knight : public Character {
public:
	Knight(GCoord acoord);

	~Knight() override;

	char symbol() override;

	bool is_evil() override;

	void magic(std::list<ObjectPtr>& objects, char direction);

	bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;

	bool move(Map& m, std::list<CharacterPtr>& characters) override;
private:
	char get_command();
	char moved_on_attack;
};





class Princess : public Character {
public:
	Princess(GCoord acoord);

	~Princess() override;

	void destroy() override;

	char symbol() override;

	bool move(Map& m, std::list<CharacterPtr>& characters) override;
};





class Monster : public Character {
public:
	Monster(GCoord acoord, int hp, int dmg);

	~Monster() override;

	bool move(Map& m, std::list<CharacterPtr>& characters) override;

	void refresh_way(Map& m, std::list<CharacterPtr>& characters);

	IntIntPairList shortest_way_to(BaseObjectPtr obj, Map& m);

	chtype symb() override;
protected:
	IntIntPairList way;

	bool aggro = false;
};





class Dragon : public Monster {
public:
	Dragon(GCoord acoord);

	~Dragon() override;

	void magic(std::list<ObjectPtr>& objects);

	bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;

	char symbol() override;
};





class Zombie : public Monster {
public:
	Zombie(GCoord acoord);

	~Zombie() override;

	char symbol() override;

	bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};





class Warlock : public Monster {
public:
	Warlock(GCoord acoord);

	~Warlock() override;

	void magic(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects);

	bool move(Map& m, std::list<CharacterPtr>& characters) override;

	char symbol() override;

	bool attack(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;	
};
