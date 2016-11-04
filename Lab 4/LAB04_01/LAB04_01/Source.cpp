/**
LAB04_01
Description: Design a thread that will run this word_find() function concurrently.  
Use futures in your program to capture its return value, or the exception it throws when it is run!  
Return this to the main function.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <future>
#include <thread>
using namespace std;

struct DictionaryEntry {
	DictionaryEntry(string word, off_t offset) : word(word), offset(offset) {} // constructor
	string word; // word 
	off_t offset; // offset in file at which word was found
};

/**
	Finds the specified word in the file.
	@param word the words to find
	@return offset in file where word was found
*/
int word_find(string word)
{
	ifstream fin("Text.txt");
	vector<DictionaryEntry> dictionary;

	if (fin.fail())
		throw exception("Unable to open file");

	string w;
	while (true) {
		if (fin >> w) { // read word
			off_t pos = fin.tellg(); // query for the file offset after read
			pos -= w.length(); // subtract the word length to get the offset at the start of the word
			dictionary.push_back(DictionaryEntry(w, pos)); // put the information in a vector
		}
		else
			break;
	}
	fin.close();

	for (auto &ent : dictionary) {
		if (ent.word == word)
			return ent.offset;
	}
	throw invalid_argument("Unable to find word");
}


int main()
{
	promise<string> arg_prms;
	promise<int> index_prms;

	thread thr([&]() {
		auto arg_ftr = arg_prms.get_future();
		try {
			int s = word_find(arg_ftr.get());
			index_prms.set_value(s);
		}
		catch (...) {
			index_prms.set_exception(current_exception());
		}
	});

	cout << "Please enter a word to find: ";
	string word;
	cin >> word;

	arg_prms.set_value(word); // Set the to find

	try {
		//off_t pos = word_find(word);
		auto index_ftr = index_prms.get_future();
		int pos = index_ftr.get();
		cout << word << " found in offset " << pos << endl;

	}
	catch (exception &ex) {
		cout << "Exception: " << ex.what() << endl;
	}

	thr.join();
}