#pragma once

#include <vector>

template <typename Type>
class BilateralArray {
public:
	BilateralArray();

	void insert(int index, Type element);

	unsigned int size();

	bool includes(int index);
private:
	// zero is in positive 
	std::vector<Type> negative, positive;
	std::vector<bool> neg_used, pos_used;
};



template <typename Type>
class BilateralArray2D {
public:
	BilateralArray2D();

	void insert(int ax, int ay, Type element);
private:
	// ...-x <-|-|-|-|-|-0-|-|-|-|-|-|-> ...+x
	BilateralArray< BilateralArray<Type> > area;
};