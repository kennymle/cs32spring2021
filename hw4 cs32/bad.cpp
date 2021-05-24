void removeBad(list<Movie*>& li)
{
	for (list<Movie*>::iterator index = li.begin(); index != li.end(); index++) {
		if ((*index)->rating() < 55) {
			delete* index;
			index = li.erase(index);
			index--;
		}
	}
}