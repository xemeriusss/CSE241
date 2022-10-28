#include "SparseVector.h"


// Default constructor
SparseVector::SparseVector(){

}

// Constructor
// Reads from file and parse the index with data in proper way.
// Makes a vector of datas.
SparseVector::SparseVector(std::string inFile){

	int a; // index of data
	double b; // data
	char ch; // ":" sign
	int index=0;
	std::vector<int> int_vec; // holds indexes
	std::vector<double> double_vec; // holds datas

	// Create the file stream object.
	std::ifstream vec_file;

	// Open the file and associate it with the object.
	vec_file.open(inFile); 

	// Check is the opened
	if(vec_file.is_open()){

		if(ErrorCheck(vec_file)){
			exit(1);
		} 

		// Reads until the end of txt.
		while(vec_file >> a >> ch >> b){

			int_vec.push_back(a);
			double_vec.push_back(b);
		}

		data.resize(a , 0.0); // Fill the vector with zero.

		for(int i=0 ; i<(data.size()) ; i++){

			if(int_vec[index] == i+1){ // If indexes encounter in the loop, fills the data vector.

				data[i] = double_vec[index];

				++index;
			}
		}
	}

	vec_file.close();
}

// Constructor
// Reads from stream and parse the index with data in proper way.
// For SparseMatrix
SparseVector::SparseVector(std::istream& inStream){

	int a; // index of data
	double b; // data
	char ch; // ":" sign
	int index=0;
	std::vector<int> int_vec; // holds indexes
	std::vector<double> double_vec; // holds datas

		if(ErrorCheck(inStream)){
			exit(1);
		} 

		// Reads until the end of txt.
		while(inStream >> a >> ch >> b){

			int_vec.push_back(a);
			double_vec.push_back(b);
		}

		data.resize(a , 0.0); // Fill the vector with zero.

		for(int i=0 ; i<(data.size()) ; i++){

			if(int_vec[index] == i+1){ // If indexes encounter in the loop, fills the data vector.

				data[i] = double_vec[index];

				++index;
			}
		}
}

// Constructor
// Makes a new object for returns.
SparseVector::SparseVector(std::vector<double> newData){

	this->data = newData;
}

// operator+
// Adds two SparseVectors
SparseVector SparseVector::operator+(const SparseVector &rhs)const{

	if(data.size() != rhs.data.size()){
		exit(1);
	}

	std::vector<double> sum;
	int size;
	size = data.size();

	sum.resize(size);

	for(int i=0 ; i<data.size() ; i++){

		sum[i] = data[i] + rhs.data[i];
	}

	return SparseVector(sum);
}

// add Function
// For adding vectors in SparseMatrix.
// It finds the bigger vector and returns the sum in bigger size vector.
SparseVector SparseVector::add(const SparseVector &rhs) const{

	int length;

	// Assigns the length with bigger vector.
	if(data.size() > rhs.data.size())
		length = data.size();
	
	else
		length = rhs.data.size();
	
	// Makes a vector with bigger size.
	std::vector<double> res(length);

	for(int i=0; i < length; i++){

		double val1, val2;

		// If smaller vector finishes, fill with 0 until the bigger vector size.
		if(i >= data.size()){
			val1 = 0.0;
		}
		else{
			val1 = data[i];
		}

		if(i >= rhs.data.size()){
			val2 = 0.0;
		} 
		else{
			val2 = rhs.data[i];
		}

		res[i] = val1 + val2;
	}

	return SparseVector(res);
}

// operator-
// Extracts two SparseVectors
SparseVector SparseVector::operator-(const SparseVector &rhs)const{

	if(data.size() != rhs.data.size()){
		exit(1);
	}

	std::vector<double> subtraction;
	int size;
	size = data.size();

	subtraction.resize(size);

	for(int i=0 ; i<data.size() ; i++){

		subtraction[i] = data[i] - rhs.data[i];
	} 

	return SparseVector(subtraction);
}

// sub Function
// For subtracting vectors in SparseMatrix.
// It finds the bigger vector and returns the subtraction in bigger size vector.
SparseVector SparseVector::sub(const SparseVector &rhs)const{

	int length;

	if(data.size() > rhs.data.size()){
		length = data.size();
	}
	else{
		length = rhs.data.size();
	}

	std::vector<double> res(length);

	for(int i=0; i < length; i++){

		double val1, val2;

		if(i >= data.size()){
			val1 = 0.0;
		}
		else{
			val1 = data[i];
		}

		if (i >= rhs.data.size()){
			val2 = 0.0;
		} 
		else{
			val2 = rhs.data[i];
		}

		res[i] = val1 - val2;
	}

	return SparseVector(res);
}

// operator-
// Negates elements of a SparseVector
SparseVector SparseVector::operator-()const{

	std::vector<double> new_data(data.size());

	for(int i=0 ; i<data.size() ; i++){

		new_data[i] = -data[i];
	}

	return SparseVector(new_data);
}

// operator=
// Assigns one SparseVector to another 
SparseVector &SparseVector::operator=(const SparseVector& rhs){

	this->data = rhs.data; //copy assignment

	return *this;
}

// operator<<
// Sends contents of a SparseVector in proper format to a std::ostream object.
std::ostream& operator<<(std::ostream& os, const SparseVector& rhs) {
	int count = 0;

	for(int i = 0; i < rhs.data.size(); i++){
			if(rhs.data[i] == 0.0){
				continue;
			}

			if(count != 0){
				os << ' ';
			}
			os << (i+1) << ':' << rhs.data[i];
			++count;
		}

		return os;
}

// Calculates the dot product of two SparseVectors
double DotProduct(const SparseVector& lfs,const SparseVector& rhs){

	if(lfs.data.size() != rhs.data.size()){
		exit(1);
	}

	double product=0;

	for(int i=0 ; i<rhs.data.size() ; i++){

		product = product + (lfs.data[i]*rhs.data[i]);
	}

	return product;
}

int SparseVector::ErrorCheck(const std::istream& inStream){

	int stop = 0;

	if (inStream.eof())
		stop = 0;
	
	if (inStream.fail()) 
		stop = 1;
	
	if (inStream.bad()) 
		stop = 1;
	
	return stop;
}

int SparseVector::size() const{
	return data.size();
}
