#include "bilateral_array.h"
#include <cmath>

using namespace std;

template <typename Type>
BilateralArray<Type>::BilateralArray() {

}


template <typename Type>
void BilateralArray<Type>::insert(int index, Type element) {
	vector<Type>& semiaxis = index >= 0 ? positive : negative;
	index = abs(index);
	if (index > semiaxis.size()) {
		semiaxis.resize(index);
		semiaxis.push_back(element);
	}
	else {
		semiaxis.at(index) = element;
	}
}

template <typename Type>
unsigned int BilateralArray<Type>::size() {
	return positive.size() + (negative.size() > 1 ? negative.size() - 1 : 0);
}

template <typename Type>
bool BilateralArray<Type>::includes(int index) {
	vector<Type>& semiaxis = index >= 0 ? positive : negative;
	return semiaxis.size() > abs(index);
}




template <typename Type>
BilateralArray2D<Type>::BilateralArray2D() {

}

template <typename Type>
void BilateralArray2D<Type>::insert(int x, int y, Type element) {
	if (abs(x) > area.size()) {

	}
}