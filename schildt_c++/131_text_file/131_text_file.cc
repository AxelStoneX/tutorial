#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ofstream out("test2");
    if(!out)
    {
        cout << "Не удается открыть файл.\n";
        return 1;
    }

    out << 10 << " " << 123.23 << "\n";
    out << "Это короткий текстовый файл.";

    out.close();

    return 0;
}
