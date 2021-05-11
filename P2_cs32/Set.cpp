#include <iostream>
#include <random>
#include <utility>
#include <cstdlib>

#include "Set.h"

Set::Set()
{
	m_size = 0;
	//head = nullptr;
	//tail = nullptr;
	head = new Node;
	head->next = head;
	head->prev = head; //circular doublylinkedlist
}

Set::~Set()
{
	for (int size = 0; size < m_size; size++) {
		Node* tobedeleted = head->next; //pointer to next node
		head->next = tobedeleted->next;
		tobedeleted->next->prev = head;
		delete tobedeleted;
	}
	//after loop, only first node remains
	delete head;

	/* //noncircular doublylinkedlist
	Node* tobedeleted = head;
	while (tobedeleted != nullptr) {
		Node* n = tobedeleted->next;
		delete tobedeleted;
		tobedeleted = n;
	}*/
}

Set::Set(const Set& other)
{
	//same as default constructor
	m_size = 0;
	head = new Node;
	head->next = head;
	head->prev = head;

	//copy over the existing nodes until it circles back to first node of list
	for (Node* temp = other.head->next; temp != other.head; temp = temp->next)
		this->insert(temp->data);


	/* //noncircular doublylinkedlist
	m_size = other.m_size;
	if (other.head == nullptr)
		head = tail = nullptr;
	else {
		Node* newguy = new Node;
		newguy->data = other.head->data;
		head = newguy;            //sets head to copy of other head
		Node* tempother = other.head->next;
		Node* temp = head;
		temp->prev = nullptr; //for first node


		while (tempother != nullptr) {
			temp->next = new Node;                      //create new node
			temp->next->data = tempother->data;         //copy the value to the new node
			//temp->next->prev = temp;               //does same as following 3 lines
			Node* newguy = temp;
			temp = temp->next;                          //increment
			temp->prev = newguy;                        //sets prev. to newguy
			tempother = tempother->next;
		}
	}
	*/
}

Set& Set::operator=(const Set& rhs)
{
	if (this != &rhs)
	{
		Set temp(rhs);
		swap(temp);
	}
	return* this;
}

bool Set::empty() const
{

	if (m_size == 0)
		return true;
	else
		return false;
}

int Set::size() const
{
	return m_size;
}

bool Set::insert(const ItemType& value)
{


	//to find postiion to insert before
	Node* temp = head->next;
	for (; temp != head && temp->data > value; temp = temp->next){}

	if (temp == head || temp->data != value) {
		Node* newguy = new Node;
		newguy->data = value;

		newguy->next = temp;
		newguy->prev = temp->prev;    //brings in new node into LL /w pointers
		temp->prev->next = newguy;    //fix next/prev pointers to point to new node
		temp->prev = newguy;
		m_size++;
		return true;
	}
	else return false;
	/*  //for noncircular DLL
	//for 0 element LL
	if (m_size == 0) {
		head = new Node;
		tail = head; 
		head->next = nullptr;
		head->prev = nullptr;
		head->data = value;
		m_size++;
		return true;
	}

	//iterates thru linkedlist. if a data in LL = value, return false
	for (Node* temp = head; temp != nullptr; temp = temp->next) {
		if (temp->data == value)
			return false;
	}

	//elements from largest to smallest
	Node* temp = head;
	for (; temp != nullptr && temp->data > value; temp = temp->next)

		if (temp->next == nullptr) {       //if its to add element to end
			for (; temp->next != nullptr && temp->data > value; temp = temp->next) {}
			Node* newguy = new Node;
			newguy->data = value;
			newguy->next = nullptr;
			newguy->prev = temp;
			temp->next = newguy;
			m_size++;
			return true;
		}

	//while(true)	{
	Node* newguy = new Node;
	newguy->data = value;
	newguy->next = temp;
	if (temp->prev == nullptr) {  //for inserting at very beginning
		temp->prev = newguy;
		newguy->prev = nullptr;
		m_size++;
		return true;
	}
	newguy->prev = temp->prev;    //brings in new node into LL

	temp->prev->next = newguy;    //fix next/prev pointers to point to new node
	temp->prev = newguy;

	m_size++;
	return true;
	//}

	*/
}

bool Set::erase(const ItemType& value)
{
	Node* temp = head->next;
	for (; temp != head && temp->data > value; temp = temp->next) {}    //elements from largest to smallest

		if (temp->data != value || temp==head)  //can't find value
			return false;

	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	delete temp;
	m_size--;
	return true;
}

bool Set::contains(const ItemType& value) const
{
	Node* temp = head->next;
	for (; temp != head; temp = temp->next) {    //elements from largest to smallest
		if (temp->data == value)
			return true;
	}
	return false;
}

bool Set::get(int i, ItemType& value) const        //assumes that the list is chronologically ordered in size from largest to smallest 
{                                                  //since this function gets called after set is filtered in "insert" function
	if (i < 0 || i >= m_size)
		return false;

	Node* temp = head->next;
	while (i > 0) {          //advance temp pointer depending on given int i
		temp = temp->next;
		i--;
	}
	value = temp->data;
	return true;

}

void Set::swap(Set& other)       //only calling data members
{
	int temp_size = other.m_size;
	other.m_size = this->m_size;
	this->m_size = temp_size;

	Node* temp = other.head;
	other.head = this->head;
	head = temp;                //swap pointers


}

void unite(const Set& s1, const Set& s2, Set& result)
{
	Set temp;
	ItemType element;

	for (int i = 0; i < s1.size(); i++) {
		s1.get(i, element);
		temp.insert(element);
	}
	for (int i = 0; i < s2.size(); i++) {
		s2.get(i, element);
		temp.insert(element);
	}

	result.swap(temp);  //swap temp to result pointers
}

void difference(const Set& s1, const Set& s2, Set& result)
{
	Set temp;
	ItemType element;
	//Set diff;

	if (s1.size() >= s2.size()) {
		for (int i = 0; i < s1.size(); i++) {
			s1.get(i, element);
			temp.insert(element);
		}
		for (int i = 0; i < s2.size(); i++) {
			s2.get(i, element);
			if (!temp.insert(element)) {
				temp.erase(element);
			}
		}
	}

	if (s1.size() <= s2.size()) {
		for (int i = 0; i < s2.size(); i++) {
			s2.get(i, element);
			temp.insert(element);
		}
		for (int i = 0; i < s1.size(); i++) {
			s1.get(i, element);
			if (!temp.insert(element)) {
				temp.erase(element);
			}
		}
	}

	result.swap(temp);  //swap temp to result pointers
}