#pragma once

// #include <iostream>
#include <ostream>
#include <string>
#include <curses.h>


enum ObjectID {
	ID_KNIGHT   = 1,
	ID_PRINCESS = 2,
	ID_DRAGON   = 3,
	ID_ZOMBIE   = 4,
	ID_WARLOCK  = 5,
	ID_MAGIC    = 6,
	ID_FLAME    = 7,
	ID_SWAMP    = 8,
	ID_CURSE    = 9,
	ID_WALL     = 10,
	ID_EMPTY    = 11,
	ID_MEDKIT   = 12
};



enum FGColorCode {
	FG_BLACK      = 30, 
	FG_RED        = 31, 
	FG_GREEN      = 32, 
	FG_YELLOW     = 33, 
	FG_BLUE       = 34, 
	FG_MAGENTA    = 35, 
	FG_CYAN       = 36, 
	FG_LIGHT_GRAY = 37,
	FG_DEFAULT    = 39,
	FG_DARK_GRAY  = 90, 
	FG_B_RED      = 91, 
	FG_B_GREEN    = 92, 
	FG_B_YELLOW   = 93, 
	FG_B_BLUE     = 94, 
	FG_B_MAGENTA  = 95, 
	FG_B_CYAN     = 96, 
	FG_WHITE      = 97
};

enum BGColorCode {
	BG_BLACK      = 40,
	BG_RED        = 41, 
	BG_GREEN      = 42,
	BG_ORANGE     = 43,
	BG_BLUE       = 44, 
	BG_PINK       = 45,
	BG_AQUA       = 46,
	BG_GRAY       = 47,
	BG_DEFAULT    = 49,
	BG_B_GRAY     = 100,
	BG_B_RED      = 101,
	BG_B_GREEN    = 102,
	BG_YELLOW     = 103,
	BG_B_BLUE     = 104,
	BG_B_PINK     = 105,
	BG_B_AQUA     = 106,
	BG_WHITE      = 107
};

struct Colored {
	Colored();
	
	Colored(const FGColorCode acode);
	
	Colored(const BGColorCode acode);
	
	Colored(const FGColorCode afg_code, const BGColorCode abg_code);
	
	Colored(const BGColorCode abg_code, const FGColorCode afg_code);
	
	std::string to_string();
	
	FGColorCode fg_code;
	BGColorCode bg_code;
};

std::ostream& operator<<(std::ostream& os, Colored code);