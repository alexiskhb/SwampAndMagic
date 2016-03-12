#include "bilateral_array.h"
#include <cmath>

using namespace std;

template <typename Type>
BilateralArray<Type>::BilateralArray() {

}


template <typename Type>
void BilateralArray<Type>::insert(const int index, Type element) {
	expand_for(index);
	vector<Type>& semiaxis = index >= 0 ? positive : negative;
	semiaxis.at(abs(index)) = element;
}

template <typename Type>
unsigned int BilateralArray<Type>::size() {
	return positive.size() + (negative.size() > 1 ? negative.size() - 1 : 0);
}

template <typename Type>
bool BilateralArray<Type>::includes(const int index) {
	vector<Type>& semiaxis = index >= 0 ? positive : negative;
	return semiaxis.size() > abs(index);
}

template <typename Type>
Type& BilateralArray<Type>::operator[](const int index) {
	expand_for(index);
	vector<Type>& semiaxis = index >= 0 ? positive : negative;
	return semiaxis.at(abs(index));
}

template <typename Type>
void BilateralArray<Type>::expand_for(const int index) {
	vector<Type>& semiaxis = index >= 0 ? positive : negative;
	index = abs(index);
	if (index >= semiaxis.size()) {
		semiaxis.resize(index + 1);
	}
}




template <typename Type>
BilateralArray2D<Type>::BilateralArray2D() {

}

template <typename Type>
void BilateralArray2D<Type>::insert(int x, int y, Type element) {
	area.expand_for(x);
	area[x].expand_for(y);
	area[x][y] = element;
}