#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    char *p = "Всем привет!";

    ofstream out("test", ios::out | ios::binary);
    if(!out)
    {
        cout << "Не удается открыть файл.\n";
        return 1;
    }
    while(*p) out.put (*p++);

    out.close();

    return 0;
}
