#include <fstream>
#include <iostream>
#include "SparseVector.h"
#include "SparseMatrix.h"

using namespace std;

int main(){

	std::ofstream fs("output.txt");

	SparseVector v1("input.txt");
	SparseVector v2("input1.txt");
	SparseVector v3;

	double sum;

	//SparseVector vec({0.0, 0.0, 4.5, 0.0, 3.7});

	//cout << v1 << '\n';

	//v3 = -v1;

	//cout << v3 << '\n';

	//v3 = v2+v1;

	//cout << "sum: " << sum << endl;

	//cout << v3 << endl;

	//fs << "v3" << endl << v3 << endl;

	SparseMatrix m1("input1.txt");
	SparseMatrix m2("input2.txt");
	SparseMatrix m3;
	m3=m1-m2;
	cout << m3 <<endl;
	fs << m3 << endl;


	return 0;
}