#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{
    char ch = 'a';

    if(argc!=3)
    {
        cout << "Применение: имя_программы <имя_файла> <стартовая позиция> \n";
        return 1;
    }

    ifstream in(argv[1], ios::in | ios::binary);
    if(!in)
    {
        cout << "Не удается открыть файл.\n";
        return 1;
    }

    in.seekg(atoi(argv[2]), ios::beg);

    while(in.get(ch))
        cout << ch;

    return 0;
}
