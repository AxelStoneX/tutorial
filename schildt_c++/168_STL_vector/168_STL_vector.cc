#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> v;
    unsigned int i;

    cout << "Размер = " << v.size() << endl;
    for(i=0; i<10; i++) v.push_back(i);
    cout << "Новый размер = " << v.size() << endl;
    cout << "Текущее содержимое:\n";
    for(i=0; i<v.size(); i++) cout << v[i] << " ";
    cout << endl;

    for(i=0;i<10;i++) v.push_back(i+10);
    cout << "Новый размер = " << v.size() << endl;
    cout << "Текущее содержимое:\n";
    for(i=0; i<v.size();i++) cout << v[i] << " ";
    cout << endl;

    for(i=0; i<v.size(); i++) v[i] = v[i] + v[i];
    cout << "Содержимое удвоено:\n";
    for(i=0; i<v.size(); i++) cout << v[i] << " ";
    cout << endl;

    return 0;
}
