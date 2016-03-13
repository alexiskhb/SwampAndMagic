#pragma once

#include <cmath>

static const int MAP_HEIGHT = 30;
static const int MAP_WIDTH  = 60;
static const int ST_ROOM = 0;
static const int ST_CELL = 0;


template <typename Type> 
int sgn(Type val) {
    return (Type(0) < val) - (val < Type(0));
}


struct Coord {
	Coord(const int ax, const int ay, const int arow, const int acol) : x(ax), y(ay), row(arow), col(acol) {

	}
	// Room coordinate
	int x, y;
	// Cell coordinate
	int row, col;
};

struct GlobalCoord {
	GlobalCoord(const int arow, const int acol) : row(arow), col(acol), components(ST_ROOM, ST_ROOM, ST_CELL, ST_CELL) {
		int absrow = abs(arow);
		int abscol = abs(acol);
		
		components.y = (absrow/MAP_HEIGHT + 1)*sgn(arow);
		components.x = (abscol/MAP_WIDTH  + 1)*sgn(acol);

		components.row = MAP_HEIGHT - absrow%MAP_HEIGHT;
		components.col = MAP_WIDTH  - abscol%MAP_WIDTH;

		if (arow >= 0) {
			components.y   -= 1;
			components.row -= 1;
		}
		if (acol >= 0) {
			components.x   -= 1;
			components.col -= 1;
		}
	}
	int row, col;
	Coord components;
};