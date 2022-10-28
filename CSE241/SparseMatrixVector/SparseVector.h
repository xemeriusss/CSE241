#ifndef _SPARSEVECTOR_H
#define _SPARSEVECTOR_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class SparseVector
{
public:
	SparseVector();
	SparseVector(std::string inFile);
	SparseVector(std::vector<double> newData);
	SparseVector(std::istream& inStream); //new2
	SparseVector operator+(const SparseVector &rhs)const;
	SparseVector operator-(const SparseVector &rhs)const;
	SparseVector operator-() const;
	SparseVector& operator=(const SparseVector& rhs);
	friend std::ostream& operator<<(std::ostream& os, const SparseVector& rhs);
	friend double DotProduct(const SparseVector& lfs,const SparseVector& rhs);
	static int ErrorCheck(const std::istream& inStream);
	SparseVector add(const SparseVector& rhs) const;
	SparseVector sub(const SparseVector& rhs) const;
	int size() const;

private:
	std::vector<double> data;
};

#endif