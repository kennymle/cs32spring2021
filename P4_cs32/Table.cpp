//Table.cpp contains the implementation of the Table class and any additional helper functions
//For this project, you must implement the tree or hash table data structure yourself.

#include "Table.h"
#include <string>
#include <vector>
#include <functional>
#include <cstdlib>
using namespace std;
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Implementation of additional helper functions//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>
// StringParser.h

// Explanatory comment to come
//
//    Example of what it does:
//	StringParser parser("  abc  'def ghi'  'Don''t cry'  ''  jk ");
//	string s;
//	assert(parser.getNextField(s)  &&  s == "abc");
//	assert(parser.getNextField(s)  &&  s == "def ghi");
//	assert(parser.getNextField(s)  &&  s == "Don't cry");
//	assert(parser.getNextField(s)  &&  s == "");
//	assert(parser.getNextField(s)  &&  s == "jk");
//	assert(!parser.getNextField(s));

class StringParser
{
public:
	StringParser(std::string text = "")
	{
		setString(text);
	}

	void setString(std::string text)
	{
		m_text = text;
		m_start = 0;
	}

	bool getNextField(std::string& fieldText) //bool getNextField(std::string& field)
	{
		m_start = m_text.find_first_not_of(" \t\r\n", m_start);
		if (m_start == std::string::npos)
		{
			m_start = m_text.size();
			fieldText = "";
			return false;
		}
		if (m_text[m_start] != '\'')
		{
			size_t end = m_text.find_first_of(" \t\r\n", m_start + 1);
			fieldText = m_text.substr(m_start, end - m_start);
			m_start = end;
			return true;
		}
		fieldText = "";
		for (;;)
		{
			m_start++;
			size_t end = m_text.find('\'', m_start);
			fieldText += m_text.substr(m_start, end - m_start);
			m_start = (end != std::string::npos ? end + 1 : m_text.size());
			if (m_start == m_text.size() || m_text[m_start] != '\'')
				break;
			fieldText += '\'';
		}
		return true;
	}
private:
	std::string m_text;
	size_t m_start;
};

bool stringToDouble(string s, double& d)
{
	char* end;
	d = std::strtof(s.c_str(), &end);
	return end == s.c_str() + s.size() && !s.empty();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////Implementation of the Table class//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
Table::Table(std::string keyColumn, const std::vector<std::string>& columns) 
:m_key(keyColumn), keynumber(0)
{
	for (int i = 0; i < columns.size(); i++)
		m_columns.push_back(columns.at(i));
		

	for (int i = 0; i < m_columns.size(); i++)	//if "keyColumn" appears anywhere in "columns"
		if (m_key == m_columns.at(i))				//store that index into int keynumber
			keynumber = i;

	m_hash.resize(buckets);		//initializing m_hash with appropriate size
}

Table::~Table() {}

bool Table::good() const {
	vector<string> copy;	//string to store previous column name
	for (int i = 0; i < m_columns.size(); i++)		//creating a copy of the column names (fields)
		copy.push_back(m_columns.at(i));
	for (int i = 0; i < m_columns.size(); i++) {	//checks if there are duplicate column names
		int counter = 0;	
		for (int j = 0; j < copy.size(); j++)
			if (copy.at(j) == m_columns.at(i)) 
				counter++;
		if (counter > 1)							//if counter > 1, there is a duplicate column name
			return false;							//and return false
	}

	for (int i = 0; i < m_columns.size(); i++)		
		if (m_columns.at(i) == "")					//if there is an empty string column name, return false
			return false;

	for (int i = 0; i < m_columns.size(); i++)	//if "keyColumn" appears anywhere in "columns"
		if (m_key == m_columns[i])				//return true, else return false
			return true;
	return false;
}

bool Table::insert(const std::string& recordString) {
	if (!good())
		return false;
	int size = 0;
	StringParser copy(recordString);
	string s;
	vector<string> record;
	while (copy.getNextField(s)) {
		record.push_back(s);		//store each field of the recordString into a vector
		size++;
	}
	if (size != m_columns.size())	//If the string has the incorrect number of fields for the table
		return false;					//return false
	else {
		Node NodeAtBucket(record);		//put the record vector into a node
		unsigned int index = std::hash<string>().operator()(record.at(keynumber)) % (buckets);	//create 0 to #ofcols indices mapping 
		m_hash.at(index).push_back(NodeAtBucket);	//pushback the record at corresponding index calculated
	}
	return true;
}

void Table::find(std::string key, std::vector<std::vector<std::string>>& records) const {
	records.clear();
	if (!good())
		return;
	unsigned int index = std::hash<string>().operator()(key) % (buckets);
	unsigned int hashsize = m_hash.at(index).size();	//size of bucket at the index of the "key"
	if (hashsize == 0)
		return;

	//going through list at the given index
	int recordindex = 0;
	for (auto record = m_hash.at(index).begin(); record != m_hash.at(index).end(); record++) {
		if (record->m_recordString.at(keynumber) == key) {		//pushback record into the 2d vector
			records.push_back(vector<string>());
			for (int i = 0; i < m_columns.size(); i++)
				records[recordindex].push_back(record->m_recordString.at(i));
			recordindex++;
		}		
	}
	return;
}

int Table::select(std::string query, std::vector<std::vector<std::string>>& records) const {
	records.clear();
	if (!good())
		return -1;	//not good table

	int size = 0;
	StringParser copy(query);	//should have three arguments (column, operator, value)
	string s;
	string queryterm;		//operator
	vector<string> record;
	string value;
	double numericvalue = 0;
	bool validcolumn = false;
	bool isnumber = false;				//keep track if value is number or not
	int keyindex;


	//whileloop and if statement to check for valid query
	while(copy.getNextField(s) && size < 3) {
		if (size == 0) {
			for (int i = 0; i < m_columns.size(); i++) {
				if (s == m_columns[i]) {
					keyindex = i;				//keep track of what column the query is at
					validcolumn = true;
				}
			}
			if (!validcolumn)
				return -1;		//checks if first parameter of queryterm is equal to any of the column names
		}
		if (size == 1)
			queryterm = s;	//store <, <=, >, >=, !=, ==, =, LT, LE, GT, GE, NE, or EQ
		if (size == 2) {
			isnumber = stringToDouble(s, numericvalue); //store numeric value
			value = s;									//store (string) value
		}
		size++;
	}
	if (size != 3)		//return -1 (false) if query did not have three arguments (i.e. not valid query)
		return -1;

	//check for valid operator
	if (queryterm != "<" && queryterm != "<=" && queryterm != ">" && queryterm != ">=" && queryterm != "!=" && queryterm != "==" && queryterm != "=" &&
		queryterm != "LT" && queryterm != "lt" && queryterm != "Lt" && queryterm != "lT" && queryterm != "LE" && queryterm != "le" && queryterm != "Le" &&
		queryterm != "lE" && queryterm != "GT" && queryterm != "gt" && queryterm != "Gt" && queryterm != "gT" && queryterm != "GE" && queryterm != "ge" &&
		queryterm != "Ge" && queryterm != "gE" && queryterm != "NE" && queryterm != "ne" && queryterm != "Ne" && queryterm != "nE" && queryterm != "EQ" &&
		queryterm != "eq" && queryterm != "Eq" && queryterm != "eQ")
		return -1;


	//stores records with equal value if operator is equal
	if (queryterm == "==" || queryterm == "=" || queryterm == "EQ" || queryterm == "eq" || queryterm == "Eq" || queryterm == "eQ") {
		find(value, records);

		//integers that count the number of correct and incorrect comparisons of record value with value
		int incorrectcomparisons = 0;		//if string compared with number, or number with string (for numerical operation)

		//iterating through all buckets of the hashtable
		int recordindex = 0;
		for (int v = 0; v < buckets; v++) {
			unsigned int hashsize = m_hash.at(v).size();	//size of bucket at the index of the "key"
			//iterate through list at the given index if there are records in that bucket
			if (hashsize != 0) {
				for (auto record = m_hash.at(v).begin(); record != m_hash.at(v).end(); record++) {
					double numkey = 0;
					if (stringToDouble(record->m_recordString.at(keyindex), numkey) && isnumber) {				//if they are number comparison
						if (numkey == numericvalue) {		//pushback record into the 2d vector if record value is less than numericvalue
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else if ((!stringToDouble(record->m_recordString.at(keyindex), numkey) && !isnumber) || (queryterm == "==" || queryterm == "=")) {		//if they are string comparison
						if (record->m_recordString.at(keyindex) == value) {		//pushback record into the 2d vector if record value is less than value
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else incorrectcomparisons++;	//increment counter for invalid comparisons [comparing (number with string) or (string with number)]
				}
			}
		}
		//For a query term specifying a field involved in a numerical comparison, return number of records that are not in proper form
		if (incorrectcomparisons > 0) {
			if (isnumber && (queryterm == "EQ" || queryterm == "eq" || queryterm == "Eq" || queryterm == "eQ"))		//if valid query term, return -1
				return incorrectcomparisons;
			records.clear();		//else if invalid query term, clear records and then return -1
			return -1;
		}
		return 0;
	}


	//stores records with less than values if operator is less than
	if (queryterm == "<" || queryterm == "lT" || queryterm == "LT" || queryterm == "lt" || queryterm == "Lt"){

		//integers that count the number of correct and incorrect comparisons of record value with value
		int incorrectcomparisons = 0;		//if string compared with number, or number with string (for numerical operation)

		//iterating through all buckets of the hashtable
		int recordindex = 0;
		for (int v = 0; v < buckets; v++) {
			unsigned int hashsize = m_hash.at(v).size();	//size of bucket at the index of the "key"
			//iterate through list at the given index if there are records in that bucket
			if (hashsize != 0) {
				for (auto record = m_hash.at(v).begin(); record != m_hash.at(v).end(); record++) {
					double numkey = 0;
					if (stringToDouble(record->m_recordString.at(keyindex), numkey) && isnumber) {				//if they are number comparison
						if (numkey < numericvalue) {		//pushback record into the 2d vector if record value is less than numericvalue
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else if ((!stringToDouble(record->m_recordString.at(keyindex), numkey) && !isnumber) || (queryterm == "<")) {		//if they are string comparison
						if (record->m_recordString.at(keyindex) < value) {		//pushback record into the 2d vector if record value is less than value
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else incorrectcomparisons++;	//increment counter for invalid comparisons [comparing (number with string) or (string with number)]
				}
			}
		}
		//For a query term specifying a field involved in a numerical comparison, return number of records that are not in proper form
		if (incorrectcomparisons > 0) {
			if (isnumber && (queryterm == "lT" || queryterm == "LT" || queryterm == "lt" || queryterm == "Lt"))		//if valid query term, return -1
				return incorrectcomparisons;
			records.clear();		//else if invalid query term, clear records and then return -1
			return -1;
		}
		return 0;
	}


	//stores records with less than and equal values if operator is less than or equal to
	if (queryterm == "<=" || queryterm == "LE" || queryterm == "le" || queryterm == "Le" || queryterm == "lE") {

		//integers that count the number of correct and incorrect comparisons of record value with value
		int incorrectcomparisons = 0;		//if string compared with number, or number with string (for numerical operation)

		//iterating through all buckets of the hashtable
		int recordindex = 0;
		for (int v = 0; v < buckets; v++) {
			unsigned int hashsize = m_hash.at(v).size();	//size of bucket at the index of the "key"
			//iterate through list at the given index if there are records in that bucket
			if (hashsize != 0) {
				for (auto record = m_hash.at(v).begin(); record != m_hash.at(v).end(); record++) {
					double numkey = 0;
					if (stringToDouble(record->m_recordString.at(keyindex), numkey) && isnumber) {				//if they are number comparison
						if (numkey <= numericvalue) {		//pushback record into the 2d vector if record value is less than numericvalue
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else if ((!stringToDouble(record->m_recordString.at(keyindex), numkey) && !isnumber) || (queryterm == "<=")) {		//if they are string comparison
						if (record->m_recordString.at(keyindex) <= value) {		//pushback record into the 2d vector if record value is less than value
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else incorrectcomparisons++;	//increment counter for invalid comparisons [comparing (number with string) or (string with number)]
				}
			}
		}
		//For a query term specifying a field involved in a numerical comparison, return number of records that are not in proper form
		if (incorrectcomparisons > 0) {
			if (isnumber && (queryterm == "LE" || queryterm == "le" || queryterm == "Le" || queryterm == "lE"))		//if valid query term, return -1
				return incorrectcomparisons;
			records.clear();		//else if invalid query term, clear records and then return -1
			return -1;
		}
		return 0;
	}


	//stores records with greater than values if operator is greater than
	if (queryterm == ">" || queryterm == "gT" || queryterm == "GT" || queryterm == "gt" || queryterm == "Gt") {
		
		//integers that count the number of correct and incorrect comparisons of record value with value
		int incorrectcomparisons = 0;		//if string compared with number, or number with string (for numerical operation)

		//iterating through all buckets of the hashtable
		int recordindex = 0;
		for (int v = 0; v < buckets; v++) {
			unsigned int hashsize = m_hash.at(v).size();	//size of bucket at the index of the "key"
			//iterate through list at the given index if there are records in that bucket
			if (hashsize != 0) {
				for (auto record = m_hash.at(v).begin(); record != m_hash.at(v).end(); record++) {
					double numkey = 0;
					if (stringToDouble(record->m_recordString.at(keyindex), numkey) && isnumber) {				//if they are number comparison
						if (numkey > numericvalue) {		//pushback record into the 2d vector if record value is greater than numericvalue
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else if ((!stringToDouble(record->m_recordString.at(keyindex), numkey) && !isnumber) || (queryterm == ">")) {		//if they are string comparison
						if (record->m_recordString.at(keyindex) > value) {		//pushback record into the 2d vector if record value is greater than value
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else incorrectcomparisons++;	//increment counter for invalid comparisons [comparing (number with string) or (string with number)]
				}
			}
		}
		//For a query term specifying a field involved in a numerical comparison, return number of records that are not in proper form
		if (incorrectcomparisons > 0) {
			if (isnumber && (queryterm == "gT" || queryterm == "GT" || queryterm == "gt" || queryterm == "Gt"))		//if valid query term, return -1
				return incorrectcomparisons;
			records.clear();		//else if invalid query term, clear records and then return -1
			return -1;
		}
		return 0;
	}


	//stores records with greater than and equal values if operator is greater than or equal to
	if (queryterm == ">=" || queryterm == "GE" || queryterm == "ge" || queryterm == "Ge" || queryterm == "gE"){

		//integers that count the number of correct and incorrect comparisons of record value with value
		int incorrectcomparisons = 0;		//if string compared with number, or number with string (for numerical operation)

		//iterating through all buckets of the hashtable
		int recordindex = 0;
		for (int v = 0; v < buckets; v++) {
			unsigned int hashsize = m_hash.at(v).size();	//size of bucket at the index of the "key"
			//iterate through list at the given index if there are records in that bucket
			if (hashsize != 0) {
				for (auto record = m_hash.at(v).begin(); record != m_hash.at(v).end(); record++) {
					double numkey = 0;
					if (stringToDouble(record->m_recordString.at(keyindex), numkey) && isnumber) {				//if they are number comparison
						if (numkey >= numericvalue) {		//pushback record into the 2d vector if record value is greater than numericvalue
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else if ((!stringToDouble(record->m_recordString.at(keyindex), numkey) && !isnumber) || (queryterm == ">=")) {		//if they are string comparison
						if (record->m_recordString.at(keyindex) >= value) {		//pushback record into the 2d vector if record value is greater than value
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else incorrectcomparisons++;	//increment counter for invalid comparisons [comparing (number with string) or (string with number)]
				}
			}
		}
		//For a query term specifying a field involved in a numerical comparison, return number of records that are not in proper form
		if (incorrectcomparisons > 0) {
			if (isnumber && (queryterm == "GE" || queryterm == "ge" || queryterm == "Ge" || queryterm == "gE"))		//if valid query term, return -1
				return incorrectcomparisons;
			records.clear();		//else if invalid query term, clear records and then return -1
			return -1;
		}
		return 0;
	}

	//stores records not equal to values if operator is not equal to
	if (queryterm == "!=" || queryterm == "NE" || queryterm == "ne" || queryterm == "Ne" || queryterm == "nE") {

		//integers that count the number of correct and incorrect comparisons of record value with value
		int incorrectcomparisons = 0;		//if string compared with number, or number with string (for numerical operation)

		//iterating through all buckets of the hashtable
		int recordindex = 0;
		for (int v = 0; v < buckets; v++) {
			unsigned int hashsize = m_hash.at(v).size();	//size of bucket at the index of the "key"
			//iterate through list at the given index if there are records in that bucket
			if (hashsize != 0) {
				for (auto record = m_hash.at(v).begin(); record != m_hash.at(v).end(); record++) {
					double numkey = 0;
					if (stringToDouble(record->m_recordString.at(keyindex), numkey) && isnumber) {				//if they are number comparison
						if (numkey != numericvalue) {		//pushback record into the 2d vector if record value is not equal to numericvalue
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else if ((!stringToDouble(record->m_recordString.at(keyindex), numkey) && !isnumber) || (queryterm == "!=")) {		//if they are string comparison
						if (record->m_recordString.at(keyindex) != value) {		//pushback record into the 2d vector if record value is not equal to value
							records.push_back(vector<string>());
							for (int i = 0; i < m_columns.size(); i++)
								records[recordindex].push_back(record->m_recordString.at(i));
							recordindex++;
						}
					}
					else incorrectcomparisons++;	//increment counter for invalid comparisons [comparing (number with string) or (string with number)]
				}
			}
		}
		//For a query term specifying a field involved in a numerical comparison, return number of records that are not in proper form
		if (incorrectcomparisons > 0) {
			if (isnumber && (queryterm == "NE" || queryterm == "ne" || queryterm == "Ne" || queryterm == "nE"))		//if valid query term, return -1
				return incorrectcomparisons;
			records.clear();		//else if invalid query term, clear records and then return -1
			return -1;
		}
		return 0;
	}
	return -1; //query term is poorly formed
}














