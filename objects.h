#pragma once

#include "base_objects.h"

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
