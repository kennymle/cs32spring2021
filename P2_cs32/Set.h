#ifndef Set_h
#define Set_h
#include <string>
using ItemType = std::string;
//using ItemType = unsigned long;

class Set
{
public:
    Set();
    ~Set();                      //destructor
    Set(const Set& other);  //copy constructor
    Set& operator=(const Set& rhs);        //assignment operator
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
private:
    int m_size;
    struct Node
    {
        ItemType data;
        Node* next;
        Node* prev;
    };
    Node* head;
   //Node* tail;
};

void unite(const Set& s1, const Set& s2, Set& result);
void difference(const Set& s1, const Set& s2, Set& result);
#endif //Set_h
