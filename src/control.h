#pragma once

#include <string>
#include <curses.h>

static const char 
	CMD_UP       = 'w',
	CMD_DOWN     = 'x',
	CMD_LEFT     = 'a',
	CMD_RIGHT    = 'd',
	CMD_LUP      = 'q',
	CMD_LDOWN    = 'z',
	CMD_RUP      = 'e',
	CMD_RDOWN    = 'c',
	CMD_AROUND   = 's',
	CMD_NUP      = '8',
	CMD_NDOWN    = '2',
	CMD_NLEFT    = '4',
	CMD_NRIGHT   = '6',
	CMD_NLUP     = '7',
	CMD_NLDOWN   = '1',
	CMD_NRUP     = '9',
	CMD_NRDOWN   = '3',
	CMD_NAROUND  = '5',
	CMD_ATTACK   = 'f',
	CMD_MAGIC    = 'r',
	CMD_NONE     = 'n',
	CMD_QUIT     = 'Q',
	CMD_REPLAY   = 'R',
	CMD_MAP      = 'M',
	CMD_COORD    = 'C',
	CMD_SETTINGS = '.';

static const char* STR_MOVES  = "wasdqezcx";

enum Mode {
	M_GAME     = 1,
	M_MAP      = 2,
	M_SETTINGS = 3
};


class Control {
public:
	static Control& instance() {
		static Control inst;
		return inst;
	}

	char get_command() {
		if (commands.size() > 0) {
			char result = commands.front();
			commands.erase(0, 1);
			return result;
		}
		else {
			return CMD_QUIT;
		}
	}

	Mode put_command() {
		char c = getch();
		switch (c) {
			break;
			case CMD_MAP: {
				return M_MAP;
			}
			break;
			case CMD_MAGIC: {
				commands.push_back(CMD_MAGIC);
				put_second(CMD_MAGIC);
				return M_GAME;
			}
			break;
			case CMD_SETTINGS: {
				put_second(CMD_SETTINGS);
				return M_SETTINGS;
			}
		}
		commands.push_back(c);
		return M_GAME;
	}

	void clear() {
		commands.clear();
	}
private:
	Control() {
	}

	void put_second(char first) {
		char c = getch();
		switch (first) {
			break;
			case CMD_MAGIC: {
				if (std::string(STR_MOVES).find(c) == std::string::npos) {
					c = CMD_AROUND;
				}
			}
		}
		commands.push_back(c);
	}

	std::string commands;
};
