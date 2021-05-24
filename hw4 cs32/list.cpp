void listAll(string path, const MenuItem* m) // two-parameter overload
{
    if (m->name() != "")                    
        cout << path << m->name() << endl;
    if (m->menuItems() == nullptr||m->menuItems()->size() == 0 )
        return;
    if (m->name() != "")
        path += m->name() + "/";


    for (int i = 0; i < m->menuItems()->size(); i++) {
        MenuItem* pointer = m->menuItems()->at(i);
        listAll(path, pointer);
    }
}
