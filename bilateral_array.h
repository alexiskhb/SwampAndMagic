#pragma once

#include <vector>
#include <cmath>

template <typename Type>
class BilateralArray {
public:
	BilateralArray() {

	}

	void insert(const int index, Type element) {
		expand_for(index);
		std::vector<Type>& semiaxis = index >= 0 ? positive : negative;
		semiaxis.at(abs(index)) = element;
	}

	unsigned int size() {
		return positive.size() + (negative.size() > 1 ? negative.size() - 1 : 0);
	}

	bool includes(const int index) {
		std::vector<Type>& semiaxis = index >= 0 ? positive : negative;
		return semiaxis.size() > abs(index);
	}

	void expand_for(const int index) {
		std::vector<Type>& semiaxis      = index >= 0 ? positive : negative;
		std::vector<Type>& semiaxis_used = index >= 0 ? pos_used : neg_used;
		index = abs(index);
		if (index >= semiaxis.size()) {
			semiaxis.resize(abs(index) + 1);
			semiaxis_used.resize(abs(index) + 1);
		}
	}

	bool is_used(const int index) {
		expand_for(index);
		std::vector<Type>& semiaxis = index >= 0 ? pos_used : neg_used;
		return semiaxis.at(abs(index));
	}

	Type& operator[](const int index) {
		expand_for(index);
		std::vector<Type>& semiaxis = index >= 0 ? positive : negative;
		return semiaxis.at(abs(index));
	}
private:
	// zero is in positive 
	std::vector<Type> negative, positive;
	std::vector<bool> neg_used, pos_used;
};



template <typename Type>
class BilateralArray2D {
public:
	BilateralArray2D() {

	}
	
	void insert(int x, int y, Type element) {
		area.expand_for(x);
		area[x].expand_for(y);
		area[x][y] = element;
	}

	BilateralArray<Type>& operator[](int index) {
		area.expand_for(index);
		return area[index];
	}
private:
	// ...-x <-|-|-|-|-|-0-|-|-|-|-|-|-> ...+x
	BilateralArray< BilateralArray<Type> > area;
};