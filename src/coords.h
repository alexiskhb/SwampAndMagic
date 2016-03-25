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

template <typename Type> 
int sgn0(Type val) {
    return (Type(0) < val) - (val < Type(0));
}


struct Coord {
	Coord() : x(ST_ROOM), y(ST_ROOM), row(ST_CELL), col(ST_CELL) {
	}

	Coord(const Coord& other) : x(other.x), y(other.y), row(other.row), col(other.col) {
	}

	Coord(const int a, const int b) : x(a), y(b), row(a), col(b) {
	}

	Coord(const int ax, const int ay, const int arow, const int acol) : x(ax), y(ay), row(arow), col(acol) {
	}

	~Coord() {
	}

	Coord& operator=(const Coord& other) {
		x   = other.x;
		y   = other.y;
		row = other.row;
		col = other.col;
		return *this;
	}
	// Room coordinates
	int x, y;
	// Cell coordinates
	int row, col;
};

struct GCoord {
	GCoord() : frow(ST_CELL), fcol(ST_CELL) {

	}

	GCoord(const GCoord& other) :
		frow(other.frow),
		fcol(other.fcol),
		parts(other.parts) {
	}

	GCoord(const int afrow, const int afcol) : frow(afrow), fcol(afcol) {
		recalculate(afrow, afcol);
	}

	GCoord(const Coord c) : parts(c) {
		// Translate cell's (0;0) to global coordinates
		int gfrow_st = c.y*MAP_HEIGHT;
		int gfcol_st = c.x*MAP_WIDTH;

		frow = gfrow_st + c.row;
		fcol = gfcol_st + c.col;
	}

	~GCoord() {
	}

	void recalculate(const int afrow, const int afcol) {
		frow = afrow;
		fcol = afcol;

		int absfrow = abs(afrow);
		int absfcol = abs(afcol);
		
		parts.y = ((absfrow - (afrow < 0 ? 1 : 0))/MAP_HEIGHT + 1)*sgn(afrow);
		parts.x = ((absfcol - (afcol < 0 ? 1 : 0))/MAP_WIDTH  + 1)*sgn(afcol);
		parts.y -= (afrow >= 0 ? 1 : 0);
		parts.x -= (afcol >= 0 ? 1 : 0);
	
		parts.row = afrow >= 0 ? absfrow%MAP_HEIGHT : MAP_HEIGHT - 1 - (absfrow-1)%MAP_HEIGHT;
		parts.col = afcol >= 0 ? absfcol%MAP_WIDTH  : MAP_WIDTH  - 1 - (absfcol-1)%MAP_WIDTH;
	}

	GCoord operator+(const GCoord& other) const {
		return GCoord(frow + other.frow, fcol + other.fcol);
	}

	GCoord& operator+=(const GCoord& other) {
		recalculate(frow + other.frow, fcol + other.fcol);
		return *this;
	}

	GCoord operator-(const GCoord& other) const {
		return GCoord(frow - other.frow, fcol - other.fcol);
	}

	GCoord& operator=(const GCoord& other) {
		frow = other.frow;
		fcol = other.fcol;
		parts = other.parts;
		return *this;
	}

	int row() const {
		return frow;
	}

	int col() const {
		return fcol;
	}

	int cellrow() const {
		return parts.row;
	}

	int cellcol() const {
		return parts.col;
	}

	int roomx() const {
		return parts.x;
	}

	int roomy() const {
		return parts.y;
	}
private:
	int frow, fcol;
	Coord parts;
};
