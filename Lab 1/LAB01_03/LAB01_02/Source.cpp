#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
using namespace std;

random_device rd;	// Used to produce a random seed
default_random_engine engine(rd()); // Use the defualt random number generator engine
std::uniform_real_distribution<> distribution(0, 1); // Generate a uniform real distribution between 0, 1

/**
Thread-safe C++11 pseudo-random number generator
@return		returns a random value between 0 and 1
*/
double cs273_rand() {
	return distribution(engine);
}

/**
Checks if coordinate (x,y) is in circle of radius 1
@param x x-dimension coordinate
@param y y-dimension coordinate
@return true if coordinate is in circle
*/
bool WithinCircle(double x, double y) {
	if (x*x + y*y < 1)
		return true;
	else
		return false;
}

int main()
{
	// inCircleCount and totalPointsCount will be required for your solution
	vector <int> inCircleCount(10, 0); 
	vector <int> totalPointsCount(10, 0);

	int inCircle = 0; // counter for points following within circle
	int totalPoints = 0; // counter for total number of pointers generated

	auto start_time = chrono::steady_clock::now(); // start timer

	vector<thread> pool;

	for (int i = 0; i < 10; i++) 
		pool.push_back(thread([&inCircleCount, &totalPointsCount](int tid) {
		for (int j = 0; j < 1e8; j++) {
			if (WithinCircle(cs273_rand(), cs273_rand()))
				inCircleCount[tid]++;
			totalPointsCount[tid]++;
		}
	}, i));

	for (int i = 0; i < 10; i++)
		pool[i].join();


	/*for (int i = 0; i < 1e8; i++) {
		double x = cs273_rand();
		double y = cs273_rand();
		if (WithinCircle(x, y))
			inCircle++;
		totalPoints++;
	}*/


	for (int x : inCircleCount)
		inCircle += x;
	for (int x : totalPointsCount)
		totalPoints += x;


	double pi = 4.0 * inCircle / totalPoints; // Estimating PI!



	cout << "Approximate value of pi is " << pi << endl;

	auto end_time = chrono::steady_clock::now();  // end timer
	auto sec = chrono::duration_cast<chrono::seconds>(end_time - start_time); // convert duration to seconds
	cout << "Time taken: " << sec.count() << " seconds\n";
}