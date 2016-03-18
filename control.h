#pragma once

#include <string>
#include <curses.h>

static const char 
	CMD_UP      = 'w',
	CMD_DOWN    = 'x',
	CMD_LEFT    = 'a',
	CMD_RIGHT   = 'd',
	CMD_LUP     = 'q',
	CMD_LDOWN   = 'z',
	CMD_RUP     = 'e',
	CMD_RDOWN   = 'c',
	CMD_AROUND  = 's',
	CMD_NUP     = '8',
	CMD_NDOWN   = '2',
	CMD_NLEFT   = '4',
	CMD_NRIGHT  = '6',
	CMD_NLUP    = '7',
	CMD_NLDOWN  = '1',
	CMD_NRUP    = '9',
	CMD_NRDOWN  = '3',
	CMD_NAROUND = '5',
	CMD_ATTACK  = 'f',
	CMD_MAGIC   = 'r',
	CMD_NONE    = 'n',
	CMD_QUIT    = 'Q',
	CMD_REPLAY  = 'R',
	CMD_MAP     = 'M';


static struct {
public:
	void put_commands() {

	}
private:
	std::string commands;
} Control;