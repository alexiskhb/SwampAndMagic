#pragma once

#include "base_objects.h"
#include <string>
#include "colored_text.h"
#include "coords.h"

static const int 
	DMG_FLAME  = 7,
	DMG_CURSE  = 3,
	DMG_MAGIC  = 7,
	DMG_MEDKIT = -20,
	TIME_SWAMP   = 15,
	TIME_MAGIC   = 5,
	TIME_CURSE   = 2,
	TIME_FLAME   = 5,
	TIME_INFTY   = -10000,
	TIME_MEDKIT  = TIME_INFTY,
	TIME_DRGNEST = TIME_INFTY,
	TIME_GRVYARD = TIME_INFTY;


class Object : public BaseObject {
public:
	Object(int arow, int acol);

	Object(int arow, int acol, GCoord dir);

	~Object() override;
	
	virtual void destroy();

	virtual void make_turn();

	virtual void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m);

	bool is_penetrable() override;

	bool is_alive() override;
protected:
	GCoord direction;
};





class Wall : public Object {
public:
	Wall(int arow, int acol);

	~Wall() override;

	char symbol() override;

	bool is_penetrable() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};





class Flame : public Object {
public:
	Flame(int arow, int acol);

	~Flame() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};





class Swamp : public Object {
public:
	Swamp(int arow, int acol);

	~Swamp() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};




// magic can heal knight
class Magic : public Object {
public:
	Magic(int arow, int acol);

	Magic(int arow, int acol, int timelife);

	Magic(int arow, int acol, int timelife, GCoord dir);

	~Magic() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};




class Curse : public Object {
public:
	Curse(int arow, int acol);

	Curse(int arow, int acol, int timelife);

	Curse(int arow, int acol, int timelife, GCoord dir);

	Curse(int arow, int acol, int timelife, int generation, GCoord dir);

	~Curse() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
private:
	int generation;
};




class Medkit : public Object {
public:
	Medkit(int arow, int acol);

	Medkit(int arow, int acol, int timelife);

	~Medkit() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};





class DragonNest : public Object {
	DragonNest(int arow, int acol);

	~DragonNest() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
private:
	int frequency;
};


class Graveyard : public Object {
	Graveyard(int arow, int acol);

	~Graveyard() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
private:
	int frequency;
};


