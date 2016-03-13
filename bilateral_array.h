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
		std::vector<Type>& semiaxis      = index >= 0 ? positive : negative;
		std::vector<Type>& semiaxis_used = index >= 0 ? pos_used : neg_used;
		semiaxis.at(abs(index)) = element;
		semiaxis_used.at(abs(index)) = true;
	}

	unsigned int size() {
		return positive.size() + (negative.size() > 1 ? negative.size() - 1 : 0);
	}

	bool includes(const int index) {
		std::vector<Type>& semiaxis = index >= 0 ? positive : negative;
		return semiaxis.size() > abs(index);
	}

	Type& operator[](const int index) {
		expand_for(index);
		std::vector<Type>& semiaxis = index >= 0 ? positive : negative;
		return semiaxis.at(abs(index));
	}

	// Does nothing if element at index already available.
	// Returns reference to element at index, NOT this!
	Type& expand_for(const int index) {
		std::vector<Type>& semiaxis      = index >= 0 ? positive : negative;
		std::vector<bool>& semiaxis_used = index >= 0 ? pos_used : neg_used;
		if (abs(index) >= semiaxis.size()) {
			semiaxis.resize(abs(index) + 1);
			semiaxis_used.resize(abs(index) + 1);
		}
		return semiaxis.at(abs(index));
	}

	// Does the same as expand_for(), but returns *this.
	BilateralArray<Type>& expanded(const int index) {
		expand_for(index);
		return *this;
	}

	bool is_used(const int index) {
		expand_for(index);
		std::vector<bool>& semiaxis_used = index >= 0 ? pos_used : neg_used;
		return semiaxis_used.at(abs(index));
	}
private:
	// Zero is in positive 
	std::vector<Type> negative, positive;
	std::vector<bool> neg_used, pos_used;
};



template <typename Type>
class BilateralArray2D {
public:
	BilateralArray2D() {

	}

	Type& expand_for(const int x, const int y) {
		return area.expand_for(x).expand_for(y);
	}

	BilateralArray2D<Type>& expanded(const int x, const int y) {
		expand_for(x, y);
		return *this;
	}
	
	void insert(const int x, const int y, Type element) {
		expand_for(x, y) = element;
	}

	bool is_used(const int x, const int y) {
		return area.expand_for(x).is_used(y);
	}

	BilateralArray<Type>& operator[](const int index) {
		return area.expand_for(index);
	}
private:
	// ...-x <-|-|-|-|-|-0-|-|-|-|-|-|-> ...+x
	BilateralArray< BilateralArray<Type> > area;
};