#include "Set.h"

#ifndef AccountSet_h
#define AccountSet_h
#include <string>
using ItemType = unsigned long;

class AccountSet
{
public:
    AccountSet();  // Create an empty account set.

    bool add(unsigned long acctNum);
    // Add an account number to the AccountSet.  Return true if and
    // only if the account number was actually added.

    int size() const;
    // Return the number of account numbers in the AccountSet.

    void print() const;
    // Write to cout every account number in the AccountSet exactly
    // once, one per line.  Write no other text.

private:
    Set m_acctSet;   //acccountSet
    //int m_size;
    // m_set[DEFAULT_MAX_ITEMS];
}; 
#endif //AccountSet_h
