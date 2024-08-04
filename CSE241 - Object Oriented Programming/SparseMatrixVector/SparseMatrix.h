#ifndef _SPARSEMATRIX_H
#define _SPARSEMATRIX_H

#include "SparseVector.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class SparseMatrix
{
public:
	SparseMatrix();
	SparseMatrix(std::string inFile);
	SparseMatrix(std::vector<SparseVector> newData);
	SparseMatrix operator+(const SparseMatrix& rhs) const;
    SparseMatrix operator-(const SparseMatrix& rhs) const;
    SparseMatrix operator-() const;
    SparseMatrix& operator=(const SparseMatrix& rhs);
    friend std::ostream& operator<<(std::ostream& os,const SparseMatrix& rhs);

    // I couldn't do operator* and transpose. They are returning default constructor.
    SparseMatrix operator*(const SparseMatrix& rhs) const;
    SparseMatrix transpose();


private:
	std::vector<SparseVector> data;
};

#endif