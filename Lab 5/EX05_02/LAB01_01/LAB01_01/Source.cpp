#include <iostream>
#include <random>
#include <vector> 
#include <omp.h>
using namespace std;

const int DIM_SIZE = 1000000;

#define NUM_THREADS 8
/**
	Vector addition
	@param a	vector a
	@param b	vector b
	@param result	vector (a+b)
*/
void vector_add(const vector<int> &a, const vector<int> &b, vector<int> &result) {

	omp_set_num_threads(NUM_THREADS);
	result.resize(a.size());  // ensure that result has sufficient elements allocated

#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		int nth = omp_get_num_threads();
		for (int i = ID; i < a.size(); i+=nth) { // Round Robin partitioning
			result[i] = a[i] + b[i];
		}
	}
}

/**
	Vector subtraction
	@param a	vector a
	@param b	vector b
	@param result	vector (a-b)
*/
void vector_subtract(const vector<int> &a, const vector<int> &b, vector<int> &result) {
	omp_set_num_threads(NUM_THREADS);
	result.resize(a.size());  // ensure that result has sufficient elements allocated
	
#pragma omp parallel
	{
		int ID = omp_get_thread_num();
		int nth = omp_get_num_threads();
		for (int i = ID; i < a.size(); i += nth) { // Round Robin partitioning
			result[i] = a[i] - b[i];
		}
	}
}

int main()
{
	vector<int> A, B, C1, C2;

	for (int i = 0; i < DIM_SIZE; i++) // Initialize vectors A and B
	{
		A.push_back(i + 1);
		B.push_back(i);
	}

	vector_add(A, B, C1); // C1 = A + B

	bool wrong = false;
	for (int i = 0; i < DIM_SIZE; i++) { // verify C1
		if (C1[i] != 2 * i + 1) {
			wrong = true;
		}
	}
	if (wrong)
		cout << "Sorry your vector_add() is incorrect\n";
	else
		cout << "vector_add() appears to be okay\n";

	vector_subtract(A, B, C2); // C2 = A - B

	wrong = false;
	for (int i = 0; i < DIM_SIZE; i++) { // verify C2
		if (C2[i] != 1)
			wrong = true;
	}
	if (wrong)
		cout << "Sorry your vector_subtract() is incorrect\n";
	else
		cout << "vector_subtract() appears to be okay\n";
}