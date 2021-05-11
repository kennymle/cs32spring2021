#include <iostream>
#include <random>
#include <utility>
#include <cstdlib>
//#include <stdlib.h>         //maybe to call exit?

#include "newSet.h"

Set::Set(int length)
{
	if (length < 0) {                    //0 is not negative
		std::cerr << "Invalid";
		exit(0);
	}
		m_size = 0;
		m_maxlength = length;
		m_set = new ItemType[m_maxlength];

}

Set::Set(const Set& other)   //copy constructor
{
	m_size = other.m_size;
	m_maxlength = other.m_maxlength;
	m_set = new ItemType[m_maxlength];     //not m_maxlength+1

	for (int i = 0; i < m_size; i++)
		m_set[i] = other.m_set[i];
}

Set& Set::operator=(const Set& rhs) {
	if (this != &rhs)              //copy&swap
	{
		Set temp(rhs);
		swap(temp);
	}
	return *this;
}

Set::~Set()             //destructor
{ 
	delete[] m_set;
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
	if (m_size == 0) {                  //for empty set, add the value. resource management :D
		m_set[0] = value;
		m_size++;
		return true;
	}
	if (m_size >= m_maxlength)     //change defaultmax to the m_maxlength that would be given
		return false;

	for (int i = 0; i < m_size; i++) {      //the value is already in the list. dont add
		if (m_set[i] == value) {
			return false;
		}
	}


	int position = 0;
	for (int j = 0; j < m_size; j++)
	{
		if (m_set[j] > value)
			position++;
		if (m_set[j] <= value)
			break;
	}

	for (int i = m_size - 1; i >= position; i--)
		m_set[i + 1] = m_set[i];


	m_set[position] = value;
	m_size++;
	return true;
}


bool Set::erase(const ItemType& value)
{
	for (int i = 0; i <= m_size; i++)
	{
		if (m_set[i] == value)
		{
			for (int k = i; k <= m_size; k++)
				m_set[k] = m_set[k + 1];
			m_size--;
			return true;
		}
	}
	return false;
}

bool Set::contains(const ItemType& value) const
{
	for (int i = 0; i < m_size; i++)
		if (m_set[i] == value)
			return true;
	return false;
}

bool Set::get(int i, ItemType& value) const        //assumes that the list is chronologically ordered in size from largest to smallest 
{                                                  //since this function gets called after set is filtered in "insert" function
	if (i >= 0 && i < m_size) {
		value = m_set[i];
		return true;
	}
	return false;
}

void Set::swap(Set& other)       //only calling data members
{
	int temp_size = other.m_size;
	other.m_size = this->m_size;
	this->m_size = temp_size;

	int temp = other.m_maxlength;
	other.m_maxlength = this->m_maxlength;
	this->m_maxlength = temp;

	ItemType* ptr = other.m_set;
	other.m_set = this->m_set;
	this->m_set = ptr;
}