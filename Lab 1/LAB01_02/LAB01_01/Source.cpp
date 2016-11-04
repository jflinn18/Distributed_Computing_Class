#include <iostream>
#include <random>
#include <vector> 
#include <ppl.h>
using namespace std;
using namespace concurrency;

const int DIM_SIZE = 1000000;
const int THREAD_COUNT = 10;

/**
	Vector addition
	@param a	vector a
	@param b	vector b
	@param result	vector (a+b)
*/
void vector_add(const vector<int> &a, const vector<int> &b, vector<int> &result) {
	result.resize(a.size());  // ensure that result has sufficient elements allocated
	/*for (int i = 0; i < a.size(); i++) {
		result[i] = a[i] + b[i];
	}*/

	parallel_for(0,10, 
		[&a, &b, &result](int tid) {
		int env_size = DIM_SIZE / THREAD_COUNT;
		for (int j = 0; j < env_size; j++)
			result[env_size*tid + j] = a[env_size*tid + j] + b[env_size*tid + j];
	});
}

/**
	Vector subtraction
	@param a	vector a
	@param b	vector b
	@param result	vector (a-b)
*/
void vector_subtract(const vector<int> &a, const vector<int> &b, vector<int> &result) {
	result.resize(a.size());  // ensure that result has sufficient elements allocated
	/*for (int i = 0; i < a.size(); i++) {
		result[i] = a[i] - b[i];
	}*/

	parallel_for(0, 10, [&a, &b, &result](int tid) {
		int env_size = DIM_SIZE / THREAD_COUNT;
		for (int j = 0; j < env_size; j++)
			result[env_size*tid + j] = a[env_size*tid + j] - b[env_size*tid + j];
	});

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