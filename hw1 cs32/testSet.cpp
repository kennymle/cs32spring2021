#include "Set.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;



int main()
{

    
    //testing for Set, empty, size, insert,
    //erase, contains, and get functions
    Set a;
    assert(a.empty());
    assert(a.size() == 0);
    a.insert("tt");
    assert(!a.contains(""));
    a.insert("pickle");
    a.insert("sauce");
    assert(!a.contains(""));
    a.insert("");
    assert(a.contains(""));
    a.erase("tt");
    assert(a.size() == 3 && a.contains("sauce") && a.contains("pickle") && a.contains(""));
    string z;
    assert(a.get(0, z));
    assert(a.get(1, z) && z == "pickle");
    assert(a.get(2, z) && z == "");

    //test swap functions
    Set b;
    b.insert("gg");
    Set c;
    c.insert("wp");
    c.insert("ff");
    b.swap(c);
    assert(b.size() == 2 && b.contains("wp") && b.contains("ff") &&
        c.size() == 1 && c.contains("gg"));
    
    Set d;
    d.insert("ant");
    d.insert("45");
    assert(!d.contains("4"));
    assert(d.size() == 2 && d.contains("ant") && d.contains("45"));


    Set s;
    assert(s.empty());
    ItemType x = "arepa";
    assert(!s.get(42, x) && x == "arepa"); // x unchanged by get failure
    s.insert("chapati");
    assert(s.size() == 1);
    assert(s.get(0, x) && x == "chapati");

    /*          //for unsigned long
    Set s;
    assert(s.empty());
    ItemType x = 9876543;
    assert(!s.get(42, x) && x == 9876543); // x unchanged by get failure
    s.insert(123456789);
    assert(s.size() == 1);
    assert(s.get(0, x) && x == 123456789);

    Set a;
    assert(a.empty());
    assert(a.size() == 0);
    a.insert(123);
    assert(a.contains(123));
    a.insert(12);
    a.insert(1);
    assert(!a.contains(33));
    a.erase(12);
    assert(a.size() == 2 && a.contains(123) && a.contains(1));
    unsigned long z;
    assert(a.get(0, z));
    assert(a.get(1, z) && z == 1);

    //test swap functions
    Set b;
    b.insert(99);
    Set c;
    c.insert(88);
    c.insert(7);
    b.swap(c);
    assert(b.size() == 2 && b.contains(88) && b.contains(7) &&
        c.size() == 1 && c.contains(99));

    Set d;
    d.insert(76);
    d.insert(67);
    assert(!d.contains(5));
    assert(d.size() == 2 && d.contains(76) && d.contains(67));

    */
    cout << "Passed all tests" << endl;




}