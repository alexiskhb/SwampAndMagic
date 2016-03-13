#pragma once

#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <limits>

#include <iostream>

template <class Type> class BilateralArray;
template <class Type> class BAIterator;
template <class Type> class BANode;


template <class Type> 
class BANode {
public:
	template <class TType> friend class BilateralArray;
	template <class TType> friend class BilateralArray2D;

	BANode() : value(Type()), index(0), is_used(false) {
	}

	BANode(const Type& elem, const int aindex) : value(elem), index(aindex), is_used(true) {
	}

	BANode(const BANode& other) : value(other.get_valuec()), index(other.indexc()), is_used(other.is_usedc()) {
	}

	BANode& operator=(const BANode& other) {
		value   = other.get_valuec();
		index   = other.indexc();
		is_used = other.is_usedc();
		return *this;
	}

	~BANode() {
	}

	bool is_usedc() const {
		return is_used;
	}

	int indexc() const {
		return index;
	}

	const Type& get_valuec() const {
		return value;
	}

	Type value;
private:
	int  index;
	bool is_used;
};





template <class Type>
class BilateralArray {
public:
	typedef std::vector<BANode<Type>> NodeVector;
	typedef NodeVector& NodeVectorRef;
	template <class TType> friend class BilateralArray2D;
	BilateralArray() {
		negative.push_back(BANode<Type>(Type(), 0));
	}

	NodeVectorRef semiaxis(const int index) {
		return index >= 0 ? positive : negative;
	}

	void update_minmax(const int index) {
		min_index = std::min(min_index, index);
		max_index = std::max(max_index, index);
	}

	void assign(const int index, Type value) {
		expand_for(index);
		semiaxis(index).at(abs(index)) = BANode<Type>(value, index);
	}

	unsigned int size() const {
		return positive.size() + (negative.size() > 1 ? negative.size() - 1 : 0);
	}

	bool includes(const int index) {
		return semiaxis(index).size() > abs(index);
	}

	Type& at(const int index) {
		return semiaxis(index).at(abs(index)).value;
	}

	Type& operator[](const int index) {
		expand_for(index);
		semiaxis(index).at(abs(index)).is_used = true;
		return semiaxis(index).at(abs(index)).value;
	}

	// Does nothing if value at index already available.
	// Returns reference to value at index, NOT this!
	Type& expand_for(const int index) {
		if (abs(index) >= semiaxis(index).size()) {
			semiaxis(index).resize(abs(index) + 1);
			auto t = semiaxis(index).begin();
			for(int i = 0, k = sgn(index); i < semiaxis(index).size(); t++, i++) {
				t->index = i*k; 
			}
		}
		semiaxis(index).at(abs(index)).is_used = true;
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
		update_minmax(positive.size());
		positive.push_back(BANode<Type>(value, positive.size() - 1));
	}

	void push_front(const Type&& value) {
		update_minmax(negative.size());
		negative.push_back(BANode<Type>(value, negative.size() - 1));
	}

	int biggest_index() const {
		return max_index;
	}

	class iterator : public std::iterator<std::forward_iterator_tag, BANode<Type>> {
	public:
		friend class BilateralArray;

		iterator(const iterator& other) : index(other.index), owner(other.owner) {
		}

		~iterator() {
		}

		bool is_dereferenceable() {
			return owner.is_used(index);
		}

		iterator& operator=(const iterator& other) {
			index = other.index;
			owner = other.owner;
			return *this;
		}

		bool operator==(const iterator& other) {
			return index == other.index;
		}

		bool operator!=(const iterator& other) {
			return index != other.index;
		}

		iterator& operator++() {
			do {
				++index;
			} while(!is_dereferenceable() && index <= owner.biggest_index());
			return *this;
		}

		Type& operator*() {
			return owner.at(index);
		}

		Type& operator->() {
			return owner.at(index);
		}
	private:
		iterator(int aindex, BilateralArray<Type>& aowner) : index(aindex), owner(aowner) {
		}

		int index;
		BilateralArray<Type>& owner;
	};

	iterator begin() {
		return iterator(min_index, *this);
	}

	iterator end() {
		return iterator(max_index+1, *this);	
	}
private:
	// Zero is in positive 
	NodeVector negative, positive;
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
	
	void assign(const int x, const int y, Type element) {
		expand_for(x, y) = element;
	}

	bool is_used(const int x, const int y) {
		return area.is_used(x) && area.at(x).is_used(y);
	}

	BilateralArray<Type>& operator[](const int index) {
		return area.expand_for(index);
	}

	Type& at(const int ax, const int ay) {
		return area.at(ax).at(ay);
	}

	class iterator : public std::iterator<std::forward_iterator_tag, BANode<Type>> {
	public:
		friend class BilateralArray2D;

		iterator(const iterator& other) : x(other.x), y(other.y), owner(other.owner) {
		}

		~iterator() {
		}

		bool is_dereferenceable() {
			return owner.is_used(x, y);
		}

		iterator& operator=(const iterator& other) {
			x = other.x;
			y = other.y;
			owner = other.owner;
			return *this;
		}

		bool operator==(const iterator& other) {
			return x == other.x && y == other.y;
		}

		bool operator!=(const iterator& other) {
			return x != other.x || y != other.y;
		}

		iterator& operator++() {
			bool at_x = true;
			do {
				if (x > owner.area.max_index) {
					x = owner.area.max_index;
					y = owner.area.at(x).max_index + 1;
					break;
				}
				if (!owner.area.is_used(x)) {
					at_x = false;
					++x;
					continue;
				}
				if (!at_x) {
					y = owner.area.at(x).min_index;
					at_x = true;
				}
				else {
					++y;
					if (y > owner.area.at(x).max_index) {
						++x;
						at_x = false;
					}
				}
			} while(!is_dereferenceable());
			return *this;
		}

		Type& operator*() {
			return owner.at(x, y);
		}

		Type& operator->() {
			return owner.at(x, y);
		}
	private:
		iterator(int ax, int ay, BilateralArray2D<Type>& aowner) : x(ax), y(ay), owner(aowner) {
		}

		int x, y;
		BilateralArray2D<Type>& owner;
	};

	iterator begin() {
		return iterator(area.min_index, area.at(area.min_index).min_index, *this);
	}

	iterator end() {
		return iterator(area.max_index, area.at(area.max_index).max_index + 1, *this);	
	}
private:
	// ...-x <-|-|-|-|-|-0-|-|-|-|-|-|-> ...+x
	BilateralArray< BilateralArray<Type> > area;
};
