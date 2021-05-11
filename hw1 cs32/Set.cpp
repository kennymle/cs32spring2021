#include <iostream>
#include <random>
#include <utility>
#include <cstdlib>

#include "Set.h"

Set::Set()
{
	m_size = 0;
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
	if (m_size >= DEFAULT_MAX_ITEMS)
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
	if (this->m_size <= other.m_size) {
		for (int i = 0; i < this->m_size; i++) {
			ItemType temp = other.m_set[i];
			other.m_set[i] = this->m_set[i];
			this->m_set[i] = temp;
		}
		for(int j = this->m_size; j <= other.m_size; j++)
			 this->m_set[j] = other.m_set[j];

	}
	else {
		for (int i = 0; i < other.m_size; i++) {
			ItemType temp = this->m_set[i];
			this->m_set[i] = other.m_set[i];
			other.m_set[i] = temp;
		}
		for (int j = other.m_size; j <= this->m_size; j++)
			other.m_set[j] = this->m_set[j];
	}

		int temp_size = other.m_size;
		other.m_size = this->m_size;
		this->m_size = temp_size;

}