#pragma once

#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <limits>

template <class Type> class BilateralArray;
template <class Type> class BAIterator;
template <class Type> class BANode;


template <class Type> 
class BANode {
public:
	BANode() : value(Type()), index(0), is_used(false) {
	}

	BANode(const Type& elem, const int aindex) : value(elem), index(aindex), is_used(true) {
	}

	~BANode() {
	}

	Type value;
	int  index;
	bool is_used;
};





template <class Type>
class BilateralArray {
public:
	BilateralArray() {
		negative.push_back(BANode<Type>(Type(), 0));
	}

	std::vector<BANode<Type>>& semiaxis(const int index) {
		return index >= 0 ? positive : negative;
	}

	void update_minmax(const int index) {
		min_index = std::min(min_index, index);
		max_index = std::max(max_index, index);
	}

	void insert(const int index, Type value) {
		expand_for(index);
		semiaxis(index).at(abs(index)) = BANode<Type>(value, index);
	}

	unsigned int size() const {
		return positive.size() + (negative.size() > 1 ? negative.size() - 1 : 0);
	}

	bool includes(const int index) const {
		return semiaxis(index).size() > abs(index);
	}

	Type& at(const int index) {
		expand_for(index);
		return semiaxis(index).at(abs(index)).value;
	}

	Type& operator[](const int index) {
		return at(index);
	}

	// Does nothing if value at index already available.
	// Returns reference to value at index, NOT this!
	Type& expand_for(const int index) {
		if (abs(index) >= semiaxis(index).size()) {
			semiaxis(index).resize(abs(index) + 1);
		}
		update_minmax(index);
		return semiaxis(index).at(abs(index)).value;
	}

	// Does the same as expand_for(), but returns *this.
	BilateralArray<Type>& expanded(const int index) {
		expand_for(index);
		return *this;
	}

	bool is_used(const int index) {
		return includes(index) && semiaxis(index).at(abs(index)).is_used;
	}

	void push_back(const Type&& value) {
		update_minmax(positive.size() - 1);
		positive.push_back(BANode<Type>(value, positive.size() - 1));
	}

	void push_front(const Type&& value) {
		update_minmax(negative.size() - 1);
		negative.push_back(BANode<Type>(value, negative.size() - 1));
	}

	int biggest_index() {
		return max_index;
	}

	
private:
	// Zero is in positive 
	std::vector<BANode<Type>> negative, positive;
	int min_index = std::numeric_limits<int>::max();
	int max_index = std::numeric_limits<int>::min();
};




template <class Type>
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
