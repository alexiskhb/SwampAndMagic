#pragma once

#include <fstream>
#include "base_objects.h"
#include "objects.h"
#include "characters.h"


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

	void load_characteristics() {

	}

	
private:
	Config() {
	}
};
