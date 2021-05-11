#include <iostream>
#include <random>
#include <utility>
#include <cstdlib>
#include "AccountSet.h"
#include "Set.h"
using namespace std;

AccountSet::AccountSet()  // Create an empty account set.
{}


bool AccountSet::add(unsigned long acctNum)
{
	if (m_acctSet.insert(acctNum))
		return true;
	else return false;
}

int AccountSet::size() const
{
	return m_acctSet.size();
}


void AccountSet::print() const
{
	ItemType x;
	for (int i = 0; i < m_acctSet.size(); i++) {
		m_acctSet.get(i, x);
		cout << x << endl;
	}

}