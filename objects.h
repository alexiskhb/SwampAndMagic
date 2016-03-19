#pragma once

#include "base_objects.h"
#include <string>
#include "colored_text.h"
#include "coords.h"

static const int 
	DMG_FLAME     = 3,
	DMG_CURSE     = 5,
	DMG_MAGIC     = 7,
	DMG_MEDKIT    = -20,
	TIME_SWAMP    = 15,
	TIME_MAGIC    = 5,
	TIME_CURSE    = 2,
	TIME_FLAME    = 5,
	TIME_INFTY    = -10000,
	TIME_MEDKIT   = TIME_INFTY,
	TIME_DRGNEST  = TIME_INFTY,
	TIME_GRVYARD  = TIME_INFTY,
	TIME_ZIGGURAT = TIME_INFTY;


class Object : public BaseObject {
public:
	Object(GCoord acoord);

	Object(GCoord acoord, GCoord dir);

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
	Wall(GCoord acoord);

	~Wall() override;

	char symbol() override;

	bool is_penetrable() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};





class Flame : public Object {
public:
	Flame(GCoord acoord);

	~Flame() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};





class Swamp : public Object {
public:
	Swamp(GCoord acoord);

	~Swamp() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};




// magic can heal knight
class Magic : public Object {
public:
	Magic(GCoord acoord);

	Magic(GCoord acoord, int timelife);

	Magic(GCoord acoord, int timelife, GCoord dir);

	~Magic() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};




class Curse : public Object {
public:
	Curse(GCoord acoord);

	Curse(GCoord acoord, int timelife);

	Curse(GCoord acoord, int timelife, GCoord dir);

	Curse(GCoord acoord, int timelife, int generation, GCoord dir);

	~Curse() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
private:
	int generation;
};




class Medkit : public Object {
public:
	Medkit(GCoord acoord);

	Medkit(GCoord acoord, int timelife);

	~Medkit() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};





class DragonNest : public Object {
public:
	DragonNest(GCoord acoord);

	~DragonNest() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
private:
	int frequency;
};


class Graveyard : public Object {
public:
	Graveyard(GCoord acoord);

	~Graveyard() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
private:
	int frequency;
};


class Ziggurat : public Object {
public:
	Ziggurat(GCoord acoord);

	~Ziggurat() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
private:
	int frequency;
};
