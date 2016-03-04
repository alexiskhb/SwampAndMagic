#pragma once

#include "base_objects.h"


class Object : public BaseObject {
public:
	Object(int arow, int acol);

	virtual bool is_penetrable();

	virtual bool is_alive();
};



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
};





class Swamp : public Object {
public:
	Swamp(int arow, int acol);

	virtual ~Swamp();

	virtual char symbol();
};




// magic can heal knight
class Magic : public Object {
public:
	Magic(int arow, int acol);

	virtual ~Magic();

	virtual char symbol();
};
