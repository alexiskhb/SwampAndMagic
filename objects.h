#pragma once

#include "base_objects.h"

static const int DMG_FLAME = 4;
static const int DMG_MAGIC = 6;
static const int TIME_SWAMP = 10;
static const int TIME_MAGIC = 6;
static const int TIME_FLAME = 6;
static const int TIME_INFTY = -10000;


class Object : public BaseObject {
public:
	Object(int arow, int acol);

	virtual void destroy();

	virtual void turn();

	virtual bool is_penetrable();

	virtual bool is_alive();

	virtual void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects);
};



class Wall : public Object {
public:
	Wall(int arow, int acol);

	virtual ~Wall();

	virtual char symbol();

	virtual bool is_penetrable();

	virtual void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects);
};





class Flame : public Object {
public:
	Flame(int arow, int acol);

	virtual ~Flame();

	virtual char symbol();

	virtual void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects);
};





class Swamp : public Object {
public:
	Swamp(int arow, int acol);

	virtual ~Swamp();

	virtual char symbol();

	virtual void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects);
};




// magic can heal knight
class Magic : public Object {
public:
	Magic(int arow, int acol);

	Magic(int arow, int acol, int timelife);

	virtual ~Magic();

	virtual char symbol();

	virtual void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects);
};
