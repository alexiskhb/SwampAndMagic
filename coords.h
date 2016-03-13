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
	Coord() : x(ST_ROOM), y(ST_ROOM), row(ST_CELL), col(ST_CELL) {
	}

	Coord(const int ax, const int ay, const int arow, const int acol) : x(ax), y(ay), row(arow), col(acol) {
	}
	// Room coordinate
	int x, y;
	// Cell coordinate
	int row, col;
};

struct GlobalCoord {
	GlobalCoord(const int arow, const int acol) : row(arow), col(acol) {
		int absrow = abs(arow);
		int abscol = abs(acol);
		
		components.y = (absrow/MAP_HEIGHT + 1)*sgn(arow);
		components.x = (abscol/MAP_WIDTH  + 1)*sgn(acol);

		components.y -= (arow >= 0 ? 1 : 0);
		components.x -= (acol >= 0 ? 1 : 0);

		components.row = arow >= 0 ? absrow%MAP_HEIGHT : MAP_HEIGHT - absrow%MAP_HEIGHT;
		components.col = acol >= 0 ? abscol%MAP_WIDTH  : MAP_WIDTH  - abscol%MAP_WIDTH;
	}

	GlobalCoord(const Coord c) : components(c) {
		// Translate cell's (0;0) to global coordinates
		int grow_st = c.y*MAP_HEIGHT;
		int gcol_st = c.x*MAP_WIDTH;

		row = grow_st + c.row;
		col = gcol_st + c.col;
	}

	int row, col;
	Coord components;
};