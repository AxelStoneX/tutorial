#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<char> v, v2;
    unsigned int i;

    for(i=0; i<10; i++) v.push_back('A' + i);

    cout << "Исходное содержимое вектора:\n";
    for(i=0; i<v.size(); i++) cout << v[i] << " ";
    cout << endl << endl;

    char str[] = "-STL -- это сила!-";
    for(i=0; str[i]; i++) v2.push_back(str[i]);

    vector<char>::iterator p = v.begin() + 5;
    vector<char>::iterator p2start = v2.begin();
    vector<char>::iterator p2end = v2.end();

    v.insert(p, p2start, p2end);

    cout << "Содержимое вектора v после вставки:\n";
    for(i=0; i<v.size(); i++) cout << v[i] << " ";

    return 0;
}
