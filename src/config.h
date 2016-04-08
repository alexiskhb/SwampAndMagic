#pragma once

#include <map>
#include <string>
#include <fstream>
#include "base_objects.h"
#include "objects.h"
#include "characters.h"

const int True  = 1;
const int False = 0;

class File {
public:
	File(const char* name) {
		handle.open(name, std::fstream::in | std::fstream::out);
	}

	~File() {
		if (handle.is_open()) {
			handle.close();
		}
	}

	void read() {
	}
private:
	std::fstream handle;
};


class Config {
public:
	static Config& instance() {
		static Config inst;
		return inst;
	}

	void load_configs() {

	}
	int show_minimap;
	int show_coordinates;
private:
	std::map<std::string, int*> variables; 
	Config() : show_minimap(True), show_coordinates(False) {
		variables[std::string("CharMaxLifeValue.Knight")]   = &HP_KNIGHT;
		variables[std::string("CharMaxLifeValue.Dragon")]   = &HP_DRAGON;
		variables[std::string("CharMaxLifeValue.Zombie")]   = &HP_ZOMBIE;
		variables[std::string("CharMaxLifeValue.Princess")] = &HP_PRINCESS;
		variables[std::string("CharMaxLifeValue.Warlock")]  = &HP_WARLOCK;
		
		variables[std::string("CharDamageValue.Knight")]   = &DMG_KN_SWORD;
		variables[std::string("CharDamageValue.Dragon")]   = &DMG_DRAGON;
		variables[std::string("CharDamageValue.Zombie")]   = &DMG_ZOMBIE;
		variables[std::string("CharDamageValue.Princess")] = &DMG_PRINCESS;
		variables[std::string("CharDamageValue.Warlock")]  = &DMG_WARLOCK;
		
		variables[std::string("ObjDamageValue.Flame")]  = &DMG_FLAME;
		variables[std::string("ObjDamageValue.Curse")]  = &DMG_CURSE;
		variables[std::string("ObjDamageValue.Magic")]  = &DMG_MAGIC;
		variables[std::string("ObjDamageValue.Medkit")] = &DMG_MEDKIT;
		
		variables[std::string("Game.ShowMinimap")]     = &show_minimap;
		variables[std::string("Game.ShowCoordinates")] = &show_coordinates;
	}
};
