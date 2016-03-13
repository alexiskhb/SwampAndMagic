#pragma once

#include "base_objects.h"
#include <string>
#include "colored_text.h"
#include "coords.h"

static const int DMG_FLAME  = 7;
static const int DMG_CURSE  = 15;
static const int DMG_MAGIC  = 8;
static const int DMG_MEDKIT = -20;
static const int TIME_SWAMP  = 15;
static const int TIME_MAGIC  = 5;
static const int TIME_CURSE  = 2;
static const int TIME_FLAME  = 4;
static const int TIME_INFTY  = -10000;
static const int TIME_MEDKIT = TIME_INFTY;


class Object : public BaseObject {
public:
	Object(int arow, int acol);

	~Object() override;
	
	virtual void destroy();

	virtual void turn();

	virtual void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects);

	bool is_penetrable() override;

	bool is_alive() override;
};





class Wall : public Object {
public:
	Wall(int arow, int acol);

	~Wall() override;

	char symbol() override;

	bool is_penetrable() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) override;
};





class Flame : public Object {
public:
	Flame(int arow, int acol);

	~Flame() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) override;
};





class Swamp : public Object {
public:
	Swamp(int arow, int acol);

	~Swamp() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) override;
};




// magic can heal knight
class Magic : public Object {
public:
	Magic(int arow, int acol);

	Magic(int arow, int acol, int timelife);

	~Magic() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) override;
};




class Curse : public Object {
public:
	Curse(int arow, int acol);

	Curse(int arow, int acol, int timelife);

	~Curse() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) override;
};




class Medkit : public Object {
public:
	Medkit(int arow, int acol);

	Medkit(int arow, int acol, int timelife);

	~Medkit() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects) override;
};





class DragonNest {

};


class Graveyard {

};


