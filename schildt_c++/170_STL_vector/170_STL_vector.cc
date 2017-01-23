#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<char> v;
    unsigned int i;

    for(i=0; i<10; i++) v.push_back('A' + i);

    cout << "Размер = " << v.size() << endl;
    cout << "Исходное содержимое вектора:\n";
    for(i=0; i<v.size(); i++) cout << v[i] << " ";
    cout << endl << endl;

    vector<char>::iterator p = v.begin();
    p += 2;
    v.insert(p, 10, 'X');
    cout << "Размер вектора после вставки = " << v.size()
                                              << endl;
    cout << "Содержимое вектора после вставки:\n";
    for(i=0; i<v.size(); i++) cout << v[i] << " ";
    cout << endl << endl;

    p = v.begin();
    p += 2;
    v.erase(p, p+10);

    cout << "Размер вектора после удаления символов = "
         << v.size() << endl;
    cout << "Содержимое вектора после удаления символов:\n";
    for(i=0; i<v.size(); i++) cout << v[i] << " ";
    cout << endl;

    return 0;
}
