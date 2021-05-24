void removeOdds(vector<int>& v)
{
	for (vector<int>::iterator index = v.begin(); index != v.end(); index++) {
		if (*index % 2 != 0) {
			index = v.erase(index);
			index--;
		}
	}
}