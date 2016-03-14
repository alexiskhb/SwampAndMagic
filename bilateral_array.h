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
	}

	int size() const {
		return static_cast<int>(axis.size());
	}

	Type& at(const int index) {
		return axis.at(stou(index)).value;
	}

	Type& operator[](const int index) {
		expand_for(index);
		axis.at(stou(index)).is_used = true;
		return axis.at(stou(index)).value;
	}

	// Does nothing if value at index already available.
	// Returns reference to value at index, NOT this!
	Type& expand_for(const int index) {
		if (stou(index) >= size()) {
			axis.resize(stou(index)+1);
			for(int i = prev_size; i < size(); i++) {
				axis.at(i).index = utos(i); 
			}
			prev_size = size();
		}
		return axis.at(stou(index)).value;
	}

	// Does the same as expand_for(), but returns *this.
	BilateralArray<Type>& expanded(const int index) {
		expand_for(index);
		return *this;
	}

	bool is_used(const int index) {
		return axis.size() > stou(index) && axis.at(stou(index)).is_used;
	}

	class iterator : public std::iterator<std::forward_iterator_tag, BANode<Type>> {
	public:
		friend class BilateralArray;

		iterator(const iterator& other) : uindex(other.uindex), owner(other.owner) {
		}

		~iterator() {
		}

		bool is_deref() {
			return owner.is_used(utos(uindex));
		}

		iterator& operator=(const iterator& other) {
			uindex = other.uindex;
			owner  = other.owner;
			return *this;
		}

		bool operator==(const iterator& other) {
			return uindex == other.uindex && owner == other.owner;
		}

		bool operator!=(const iterator& other) {
			return uindex != other.uindex || owner != other.owner;
		}

		iterator& operator++() {
			do {
				++uindex;
			} while(!is_deref() && uindex < owner.size());
			return *this;
		}

		Type& operator*() {
			return owner.at(owner.utos(uindex));
		}

		Type& operator->() {
			return owner.at(owner.utos(uindex));
		}
	private:
		iterator(int auindex, BilateralArray<Type>& aowner) : uindex(auindex), owner(aowner) {
		}

		int uindex;
		BilateralArray<Type>& owner;
	};

	iterator begin() {
		iterator newit = iterator(0, *this);
		if (!newit.is_deref()) {
			++newit;
		}
		return newit;
	}

	iterator end() {
		return iterator(size(), *this);
	}
private:
	// signed to unsigned
	inline int stou(const int signed_index) {
		return signed_index >= 0 ? 2*signed_index : 2*abs(signed_index)-1;
	}

	// unsigned to signed
	inline int utos(const int uindex) {
		return ((uindex+1)/2) * (uindex%2 == 0 ? 1 : -1);
	}

	NodeVector axis;
	int prev_size = 0;
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

	bool is_used(const int x, const int y) {
		return area.is_used(x) && area.at(x).is_used(y);
	}

	BilateralArray<Type>& operator[](const int index) {
		BilateralArray<Type>& result = area.expand_for(index);
		area.axis.at(area.stou(index)).is_used = true;
		return result;
	}

	Type& at(const int ax, const int ay) {
		return area.at(ax).at(ay);
	}

	class iterator : public std::iterator<std::forward_iterator_tag, BANode<Type>> {
	public:
		friend class BilateralArray2D;

		iterator(const iterator& other) : ux(other.ux), uy(other.uy), owner(other.owner) {
		}

		~iterator() {
		}

		bool is_deref() {
			return owner.is_used(utos(ux), utos(uy));
		}

		iterator& operator=(const iterator& other) {
			ux = other.ux;
			uy = other.uy;
			owner = other.owner;
			return *this;
		}

		bool operator==(const iterator& other) {
			return ux == other.ux && uy == other.uy;
		}

		bool operator!=(const iterator& other) {
			return ux != other.ux || uy != other.uy;
		}

		iterator& operator++() {
			bool at_x = true;
			do {
				if (ux >= owner.area.size()) {
					ux = owner.area.size() > 0 ? owner.area.size()-1 : 0;
					uy = owner.area.size() > 0 ? owner.area.at(owner.area.utos(ux)).size() : 0;
					break;
				}
				if (!owner.area.is_used(owner.area.utos(ux))) {
					at_x = false;
					++ux;
					continue;
				}
				if (!at_x) {
					uy = 0;
					at_x = true;
				}
				else {
					++uy;
					if (uy >= owner.area.at(owner.area.utos(ux)).size()) {
						++ux;
						at_x = false;
					}
				}
			} while(!is_deref());
			return *this;
		}

		Type& operator*() {
			return owner.at(owner.area.utos(ux), owner.area.utos(uy));
		}

		Type& operator->() {
			return owner.at(owner.area.utos(ux), owner.area.utos(uy));
		}
	private:
		iterator(int ax, int ay, BilateralArray2D<Type>& aowner) : ux(ax), uy(ay), owner(aowner) {
		}

		int utos(const int index) {
			return owner.area.utos(index);
		}

		int stou(const int index) {
			return owner.area.stou(index);
		}

		int ux, uy;
		BilateralArray2D<Type>& owner;
	};

	iterator begin() {
		iterator newit = iterator(0, 0, *this);
		if (!newit.is_deref()) {
			++newit;
		}
		return newit;
	}

	iterator end() {
		return iterator(
			area.size() > 0 ? area.size()-1 : 0, 
			area.size() > 0 ? area.at(area.utos(area.size()-1)).size() : 0, 
			*this);	
	}
private:
	// ...-x <-|-|-|-|-|-0-|-|-|-|-|-|-> ...+x
	BilateralArray< BilateralArray<Type> > area;
};
