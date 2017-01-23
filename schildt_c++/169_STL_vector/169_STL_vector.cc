#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<char> v;
    int i;

    for(i=0; i<10; i++) v.push_back('A' + i);
    for(i=0; i<10; i++) cout << v[i] << " ";
    cout << endl;

    vector<char>::iterator p = v.begin();
    while(p != v.end()){
        cout << *p << " ";
        p++;
    }

    return 0;
}
