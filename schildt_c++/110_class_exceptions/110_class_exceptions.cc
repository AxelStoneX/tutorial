#include <iostream>
#include <cstring>
using namespace std;

class MyException
{
public:
    char str_what[80];
    MyException() { *str_what = 0; }
    MyException(char *s)
    {
        strcpy(str_what, s);
    }
};

int main()
{
    int a, b;
    try
    {
        cout << "Введите числитель и знаменатель: \n";
        cin >> a >> b;
        if(!b)
            throw MyException("Делить на нуль нельзя!");
        else
            cout << "Частное равно " << a/b << "\n";
    }
    catch (MyException e)
    {
        cout << e.str_what << "\n";
    }

    return 0;
}
