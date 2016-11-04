#include <iostream>
#include <thread>
#include <ppl.h>
#include <mutex>
#include <condition_variable>
using namespace std;
using namespace concurrency;

mutex m;

#define AMOUNT 20
class BankAccount {
private:
	int amount;
	condition_variable cv;
public:
	BankAccount(int amount) : amount(amount) {}
	/**
	* Deposits money into account
	*/
	void Deposit(int dollar) {
		unique_lock<mutex> lk(m);
		amount += dollar;
		if (amount >= AMOUNT)
			cv.notify_one();
		lk.unlock();


	}
	/**
	* Withdraws money from account
	* If account has less money than the amount requested,
	* it does not perform the withdrawal.
	*/
	bool Withdraw(int dollar) {
		unique_lock<mutex> lk(m);
		if (amount < AMOUNT) {
			cv.wait(lk, [this]() {
				return this->amount >= AMOUNT;
			});
		}
		if (amount >= dollar) {
			amount -= dollar;
			lk.unlock();
			return true;
		}
		else {
			return false;
		}
	}
	int Amount() {
		return amount;
	}
};
int main()
{
	BankAccount myAccount(0); // Open an account with an initial balance of $0
	parallel_for(0, 1000, [&](int tid) {
		if (tid % 2 == 0) { // every other thread will withdraw
			myAccount.Withdraw(AMOUNT); // Withdraw some amount
		}
		else {
			myAccount.Deposit(AMOUNT); // Deposit some amount
		}
	});
	cout << "My account has $" << myAccount.Amount() << endl;
}