#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    char str[] = "This is very simple test.";
    vector<char> v, v2(30);
    unsigned int i;

    for(i=0; str[i]; i++) v.push_back(str[i]);

    cout << "Входная последовательность: ";
    for(i=0; i<v.size(); i++) cout << v[i];
    cout << endl;

    remove_copy(v.begin(), v.end(), v2.begin(), 'i');

    cout << "После удаления букв i: ";
    for(i=0; v2[i]; i++) cout << v2[i];
    cout << endl << endl;

    cout << "Входная последовательность: ";
    for(i=0; i<v.size(); i++) cout << v[i];
    cout << endl;

    replace_copy(v.begin(), v.end(), v2.begin(), 'i', 'I');

    cout << "После замены букв i буквами I: ";
    for(i=0; v2[i]; i++) cout << v2[i];
    cout << endl << endl;
}
