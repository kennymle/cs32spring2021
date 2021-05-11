#include "AccountSet.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;



int main()
{
    //test accountset, add, size, print

    AccountSet a;     // Create an empty account set.
    
    assert(a.size() == 0);
    a.add(123);
    a.add(435);
    a.add(432);
    assert(!a.add(123));
    assert(a.size() == 3);
    a.print();

	cout << "Test Passed";
}