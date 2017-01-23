#include <iostream>
#include <string>
using namespace std;

int main()
{
    unsigned int i;
    string s1 = "Класс string облегчает обработку строк.";
    string s2;

    i = s1.find("string");
    if(i != string::npos)
    {
        cout << "Совпадение обнаружено в позиции " << i << endl;
        cout << "Остаток строки таков: ";
        s2.assign(s1, i, s1.size());
        cout << s2;
    }

    return 0;
}
