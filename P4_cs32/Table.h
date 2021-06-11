//Table.h contains the declaration of the Table class with appropriate include guard

#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED

#include <string>
#include <vector>
#include <list>
#include <cstdlib>
using namespace std;

const int buckets = 997;	//number of buckets  (prime number)

class Table
{
public:
	//Construct an empty Table whose column names are the elements of the vector second parameter. The first parameter is the name of the key field.
	Table(std::string keyColumn, const std::vector<std::string>& columns);
	
	//destructor
	~Table();
	
	//Return true if the table was successfully constructed, and false otherwise.
	bool good() const;			

	//If the string has the correct number of fields for the table, insert a record with those fields into the table and return true;
	//otherwise, leave the table unchangedand return false.
	bool insert(const std::string& recordString);

	//Replace the value in the second parameter with a vector containing as many elements as there are records in the table whose key field is equal to the value of the first parameter.
	//Each of those elements is one of the matching records, represented as a vector whose elements are the fields of that record.
	//The records placed in the vector don't have to be in any particular order. 
	void find(std::string key, std::vector<std::vector<std::string>>& records) const;

	//Replace the value in the second parameter with a vector containing
	//as many elements as there are records in the table that satisfy the query (first parameter)
	int select(std::string query, std::vector<std::vector<std::string>>& records) const;

	// We prevent a Table object from being copied or assigned by
	// making the copy constructor and assignment operator unavailable.
	Table(const Table&) = delete;
	Table& operator=(const Table&) = delete;
private:
	string m_key;
	int keynumber;						//column number of key
	vector<string> m_columns; //fields
	struct Node {
		Node(vector<string> s) {
			for (int i = 0; i < s.size(); i++)
				m_recordString.push_back(s.at(i));
		}
		vector<string> m_recordString;
	};
	vector<list<Node>> m_hash;			//vector containing lists of nodes(records)
};
#endif //TABLE_INCLUDED