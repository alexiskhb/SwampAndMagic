#pragma once

#include "base_objects.h"
#include <string>
#include "colored_text.h"
#include "coords.h"
#include <map>

extern int 
	DMG_FLAME    ,
	DMG_CURSE    ,
	DMG_MAGIC    ,
	DMG_MEDKIT   ,
	TIME_SWAMP   ,
	TIME_MAGIC   ,
	TIME_CURSE   ,
	TIME_FLAME   ,
	TIME_INFTY   ,
	TIME_MEDKIT  ,
	TIME_DRGNEST ,
	TIME_GRVYARD ,
	TIME_ZIGGURAT,
	TIME_HOSPITAL,
	LIM_MEDKIT   ;


class Object : public BaseObject {
public:
	Object(GCoord acoord);

	Object(GCoord acoord, GCoord dir);

	~Object() override;

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

	static PairKeyMap count;
};




class Spawn : public Object {
public:
	Spawn(GCoord acoord, int freq);

	void destroy() override;
protected:
	int frequency;
};


class DragonNest : public Spawn {
public:
	DragonNest(GCoord acoord);

	~DragonNest() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};


class Graveyard : public Spawn {
public:
	Graveyard(GCoord acoord);

	~Graveyard() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};


class Ziggurat : public Spawn {
public:
	Ziggurat(GCoord acoord);

	~Ziggurat() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};


class Hospital : public Spawn {
public:
	Hospital(GCoord acoord);

	~Hospital() override;

	char symbol() override;

	void impact(std::list<CharacterPtr>& characters, std::list<ObjectPtr>& objects, Map& m) override;
};
