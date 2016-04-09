#pragma once

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include "base_objects.h"
#include "objects.h"
#include "characters.h"
#include <iostream>

const int True  = 1;
const int False = 0;
const int STREAMSIZE = 256;
const char* CONFIG_FILENAME = "config.sam";

class ConfigFile : public std::fstream {
public:
	virtual ~ConfigFile() {
		if (is_open()) {
			close();
		}
	}

	// Returns true if not end of file. 
	// In other case pair is not valid
	bool get_option(std::pair<std::string, std::string>& opt) {
		line[0] = '#';
		while (std::string(line).size() == 0 || line[0] == '#') {
			if(!getline(line , 2*STREAMSIZE)) {
				break;
			}
		}
		std::stringstream ss;
		ss << line;
		ss.getline(key  , STREAMSIZE, '=' );
		ss.getline(value, STREAMSIZE, '\n');
		opt.first  = std::string(key);
		opt.second = std::string(value);
		return !eof();
	}
private:
	char line[2*STREAMSIZE];
	char key[STREAMSIZE];
	char value[STREAMSIZE];
};


class BadConfigException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Config file not found";
	}
};

class BadOptionException : public std::exception {
public:
	BadOptionException(std::string s) : bad_string(s) {		
	}

	virtual const char* what() const throw() {
		return ("Config: invalid key: " + bad_string).c_str();
	}

	std::string bad_string;
};

class Config {
public:
	static Config& instance() {
		static Config inst;
		return inst;
	}

	void load_configs() {
		std::pair<std::string, std::string> opt;
		ConfigFile file;
		file.open(CONFIG_FILENAME);
		if (!file.is_open()) {
			throw BadConfigException();
		}
		// ConfigFile file;
		while (file.get_option(opt)) {
			if (variables.count(opt.first) == 0) {
				throw BadOptionException(opt.first);
			}
			int value;
			try {
				value = std::stoi(opt.second);
			}
			catch (std::invalid_argument) {
				throw BadOptionException(opt.second + " at " + opt.first);
			}
			*variables[opt.first] = value;
			std::cout << opt.first << " = " << value << std::endl;
		}
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
		
		variables[std::string("CharDamageValue.Knight")]    = &DMG_KN_SWORD;
		variables[std::string("CharDamageValue.Dragon")]    = &DMG_DRAGON;
		variables[std::string("CharDamageValue.Zombie")]    = &DMG_ZOMBIE;
		variables[std::string("CharDamageValue.Princess")]  = &DMG_PRINCESS;
		variables[std::string("CharDamageValue.Warlock")]   = &DMG_WARLOCK;
		
		variables[std::string("ObjDamageValue.Flame")]      = &DMG_FLAME;
		variables[std::string("ObjDamageValue.Curse")]      = &DMG_CURSE;
		variables[std::string("ObjDamageValue.Magic")]      = &DMG_MAGIC;
		variables[std::string("ObjDamageValue.Medkit")]     = &DMG_MEDKIT;
		
		variables[std::string("Game.ShowMinimap")]          = &show_minimap;
		variables[std::string("Game.ShowCoordinates")]      = &show_coordinates;
	}
};
