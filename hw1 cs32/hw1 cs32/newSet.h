#ifndef newSet_h
#define newSet_h
#include <string>

const int DEFAULT_MAX_ITEMS = 150;

//using ItemType = unsigned long;      //typedef unsigned long ItemType; 
using ItemType = std::string;          //typedef std::string ItemType;

class Set
{
public:
    Set(int length = DEFAULT_MAX_ITEMS);         // Create a set with specify length.

    Set(const Set& other);            //copy constructor: assing obj b to a. b no purpose yet
                                      //copy a stuff over to b

    Set& operator=(const Set& rhs);       //b has purpose already but overwriting

    ~Set();                             //Destructor

    bool empty() const;  // Return true if the set is empty, otherwise false.

    int size() const;    // Return the number of items in the set.

    bool insert(const ItemType& value);
    // Insert value into the set if it is not already present.  Return
    // true if the value is actually inserted.  

    bool erase(const ItemType& value);
    // Remove the value from the set if it is present.  Return true if the
    // value was removed; otherwise, leave the set unchanged and
    // return false.

    bool contains(const ItemType& value) const;
    // Return true if the value is in the set, otherwise false.

    bool get(int i, ItemType& value) const;
    // If 0 <= i < size(), copy into value the item in the set that is
    // strictly less than exactly i items in the set and return true.
    // Otherwise, leave value unchanged and return false.

    void swap(Set& other);

private:
    int m_size;
    ItemType* m_set;       //dynamically allocated array	//string[] m_set = new string[];
    int m_maxlength;
};
#endif // newSet_h 
