#include "newSet.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;



int main()
{
   
    Set a(1000);   // a can hold at most 1000 distinct items
    Set b(5);      // b can hold at most 5 distinct items
    Set c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
    //ItemType v[6] = {1,7,8,4,5,6};  
    ItemType v[6] = { "1","7","3","4","5","6" };
    // No failures inserting 5 distinct items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v[k]));

    // Failure if we try to insert a sixth distinct item into b
    assert(!b.insert(v[5]));

    // When two Sets' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(v[5]) && b.insert(v[5]));
    
    
    /*//extra tests
    Set d;
    ItemType g[10] = { 1,2,3,4,5,6,7,8,9,10 };
    assert(d.empty());
    assert(d.size() == 0);
    for (int k = 0; k < 9; k++)
        assert(!b.insert(v[5]));
    assert(d.insert(3));
    assert(!d.contains(44));
    assert(!d.contains(4));
    unsigned long s;
    assert(d.get(0, s));
    */

}