#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <ppl.h>

using namespace std;
using namespace concurrency;

//#define DATA_SIZE	int(1e8)
#define DATA_SIZE int(1e8)

random_device rd;
default_random_engine engine;
uniform_int_distribution<> distribution(0, DATA_SIZE); // Generate a uniform real distribution between 0, 1e8

vector<int>::iterator beg;
/**
* Thread-safe C++11 pseudo-random number generator
* @return		returns a random value between 0 and 1e8
*/
int cs273_rand() {
	return distribution(engine);
}

/**
*	Find the target value within the vector range described by the start and end iterators
*
*	@param start iterator pointing to the start of the vector
*	@param end iterator pointing to the end of the vector
*	@parame target target value to find in the vector
*	@return the index position if the target is found, -1 otherwise
*/
template <typename T>
int find(T start, T end, int target) {
	for (T it = start; it != end; ++it)
		if (*it == target)
			return (it - start); // return the index
	return -1;
}

template <typename T>
int  pfind(T start, T end, int target) {
	if ((end - start) < 10) {
		for (T it = start; it != end; ++it)
			if (*it == target)
				return (it - start); // return the index
		return -1;
	}
	else {
		T middle = start + ((end - start) / 2);

		int r1, r2;
		parallel_invoke([&r1, &start, &middle, &target]() {
			r1 = pfind(start, middle, target);
		}, [&r2, &middle, &end, &target]() {
			r2 = pfind(middle, end, target);
		});

		//int r1 = pfind(start, middle, int target);
		//int r2 = pfind(middle, end, int target);

		/*if (r1 != -1) {
			//int i = (start - beg) + r1;
			//return i;    //This is returning a vector<int>::iterator
			return r1;
		}
		else if (r2 != -1) {
			//int i = (middle - beg) + r2;
			//return i;   // So is this
			return (middle - r2) + start;
		}
		else
			return -1;*/
		if (r1 != -1)
			return r1;
		else if (r2 != -1)
			return ((middle + r2) - start);
		else
			return -1;
	}
}

int main()
{
	cout << "Initializing haystack ... please wait ...\n";
	vector<int> haystack(DATA_SIZE);
	for (int i = 0; i < DATA_SIZE; i++)
		haystack[i] = cs273_rand();

	int needle;
	do {
		cout << "What number do you want to search for ?[0," << DATA_SIZE << "] ";
		if (cin >> needle && needle >= 0 && needle <= DATA_SIZE)
			break;
	} while (true); // repeat until a valid number is found

	cout << "Searching!\n";
	auto start_time = chrono::steady_clock::now();

	beg = haystack.begin();
	int index = pfind(haystack.begin(), haystack.end(), needle);

	auto end_time = chrono::steady_clock::now();
	auto sec = chrono::duration_cast<chrono::seconds>(end_time - start_time);

	//int new_index = find(haystack.begin(), haystack.end(), needle);

	/*if (index == new_index)
		cout << "The pIndex is correct" << endl;
	else
		cout << "Index: " << new_index << endl;*/

	cout << "Time taken: " << sec.count() << " seconds\n";

	if (index == -1)
		cout << "Did not find " << needle << endl;
	else
	{
		cout << "Found " << needle << " in index " << index << endl;
		// verify that the found index location is correct!
		if (needle == haystack[index])
			cout << "Index is correct!\n";
		else
			cout << "Sorry the index is incorrect!\n";
	}
}