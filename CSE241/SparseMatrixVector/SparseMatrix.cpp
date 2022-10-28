#include "SparseMatrix.h"
#include "SparseVector.h"
#include <sstream>

// Default constructor
SparseMatrix::SparseMatrix(){

}

// Constructor
// Reads the matrix from file and sends the vectors for parsing to SparseVector
// Makes a matrix
SparseMatrix::SparseMatrix(std::string inFile){

	// Create the file stream object.
	std::ifstream matrix_file;

	matrix_file.open(inFile);

	if(!matrix_file.is_open()){
		exit(1);
	}

	if(SparseVector::ErrorCheck(matrix_file)){
		exit(1);
	}

	std::string curLine;
	std::vector<SparseVector> sparsevector_vec;

	// Reads until the end of txt.
	while(std::getline(matrix_file, curLine)){

		std::istringstream curLineStream(curLine); // istringstream for operating on vector string

		int curIndex;
		curLineStream >> curIndex; // Reads row number and eliminates it from the stream

		while(sparsevector_vec.size() < curIndex - 1){
			sparsevector_vec.push_back(SparseVector()); // Adds Deafult SparseVector vector to matrix
		}

		sparsevector_vec.push_back(SparseVector(curLineStream)); // Adds SparseVector vector to matrix
	}

	data = sparsevector_vec; // Assigns the final matrix to private data
}

// Constructor
// Makes a new object for returns.
SparseMatrix::SparseMatrix(std::vector<SparseVector> newData){

	this->data = newData;
}

SparseMatrix &SparseMatrix::operator=(const SparseMatrix& rhs){

	this->data = rhs.data; //copy assignment

	return *this;
}

SparseMatrix SparseMatrix::operator+(const SparseMatrix &rhs)const{

	if(data.size() != rhs.data.size()){
		exit(1);
	}

	int rows = data.size();

	// Makes a vector size of data
	std::vector<SparseVector> sum(rows); 

	for(int i=0 ; i<rows ; i++){
		sum[i] = data[i].add(rhs.data[i]); // It makes the sum inside add function.
	}

	return SparseMatrix(sum);
}

SparseMatrix SparseMatrix::operator-(const SparseMatrix &rhs)const{

	if(data.size() != rhs.data.size()){
		exit(1);
	}

	int rows = data.size();

	// Makes a vector size of data
	std::vector<SparseVector> sub(rows); // It makes the subtraction inside sub function.

	for(int i=0 ; i<rows ; i++){
		sub[i] = data[i].sub(rhs.data[i]);
	}

	return SparseMatrix(sub);
}

SparseMatrix SparseMatrix::operator-()const{

	std::vector<SparseVector> result;

	for (const SparseVector &vec : data){
		result.push_back(-vec);
	}

	return SparseMatrix(result);
}

std::ostream& operator<<(std::ostream& os,const SparseMatrix& rhs){

	int length = rhs.data.size();
	int printed_count = 0;

	for (int i = 0; i < length; ++i){

		if (rhs.data[i].size() == 0){
			continue;
		}

		if(printed_count > 0){ // endl can be done if any row is printed.
			os << std::endl;
		}

		os << (i + 1) << ' ' << rhs.data[i];
		++printed_count;
	}
	return os;
}

SparseMatrix SparseMatrix::operator*(const SparseMatrix &rhs)const{

	return SparseMatrix();
}

SparseMatrix SparseMatrix::transpose(){

	return SparseMatrix();
}
