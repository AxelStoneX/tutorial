#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    char ch = 'a';

    if(argc != 2)
    {
        cout << "Применение: имя_программы <имя_файла>\n";
        return 1;
    }

    ifstream in(argv[1], ios::in | ios::binary);
    if(!in)
    {
        cout << "Не удается открыть файл.\n";
        return 1;
    }

    while(in)
    {
        in.get(ch);
        if(in) cout << ch;
    }

    in.close();

    return 0;
}
