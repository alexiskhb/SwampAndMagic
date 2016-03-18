#pragma once

#include <cmath>

static const int MAP_HEIGHT = 30;
static const int MAP_WIDTH  = 96;
static const int ST_ROOM = 0;
static const int ST_CELL = 0;


template <typename Type> 
int sgn(Type val) {
    return (Type(0) <= val) - (val < Type(0));
}


struct Coord {
	Coord() : x(ST_ROOM), y(ST_ROOM), row(ST_CELL), col(ST_CELL) {
	}

	Coord(const int ax, const int ay, const int arow, const int acol) : x(ax), y(ay), row(arow), col(acol) {
	}
	// Room coordinates
	int x, y;
	// Cell coordinates
	int row, col;
};

struct GCoord {
	GCoord() : row(ST_CELL), col(ST_CELL) {

	}

	GCoord(const int arow, const int acol) : row(arow), col(acol) {
		recalculate(arow, acol);
	}

	GCoord(const Coord c) : parts(c) {
		// Translate cell's (0;0) to global coordinates
		int grow_st = c.y*MAP_HEIGHT;
		int gcol_st = c.x*MAP_WIDTH;

		row = grow_st + c.row;
		col = gcol_st + c.col;
	}

	void recalculate(const int arow, const int acol) {
		row = arow;
		col = acol;

		int absrow = abs(arow);
		int abscol = abs(acol);
		
		parts.y = ((absrow - (arow < 0 ? 1 : 0))/MAP_HEIGHT + 1)*sgn(arow);
		parts.x = ((abscol - (acol < 0 ? 1 : 0))/MAP_WIDTH  + 1)*sgn(acol);
		parts.y -= (arow >= 0 ? 1 : 0);
		parts.x -= (acol >= 0 ? 1 : 0);
	
		parts.row = arow >= 0 ? absrow%MAP_HEIGHT : MAP_HEIGHT - 1 - (absrow-1)%MAP_HEIGHT;
		parts.col = acol >= 0 ? abscol%MAP_WIDTH  : MAP_WIDTH  - 1 - (abscol-1)%MAP_WIDTH;
	}

	GCoord operator+(const GCoord& other) const {
		return GCoord(row + other.row, col + other.col);
	}

	GCoord& operator+=(const GCoord& other) {
		recalculate(row + other.row, col + other.col);
		return *this;
	}

	GCoord operator-(const GCoord& other) const {
		return GCoord(row - other.row, col - other.col);
	}

	// Global coordinates
	int row, col;
	Coord parts;
};