#include<thread>
#include<iostream>
#include<string>

int main() {
	std::string resp;
	std::cout << "Threads: " << std::thread::hardware_concurrency() << std::endl;
	std::cin >> resp; //For some reason it was not keeping the cmd window open
	return 0;
}