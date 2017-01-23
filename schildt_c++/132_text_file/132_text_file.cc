#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    char ch = 'a';
    int i = 0;
    float f;
    char str[80];

    ifstream in("test");
    if(!in)
    {
        cout << "Не удается открыть файл.\n";
        return 1;
    }

    in >> i;
    in >> f;
    in >> ch;
    in >> str;

    cout << i << " " << f << " " << ch << "\n";
    cout << str;

    in.close();
    return 0;
}
