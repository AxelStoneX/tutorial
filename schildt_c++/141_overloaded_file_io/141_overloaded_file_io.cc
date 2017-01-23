#include <iostream>
#include <fstream>
using namespace std;

class three_d
{
    int x, y, z;
public:
    three_d(int a, int b, int c) { x = a; y = b; z = c; }
    friend ostream &operator<< (ostream &stream, three_d obj);
};

ostream &operator<<(ostream &stream, three_d obj)
{
    stream << obj.x << ", ";
    stream << obj.y << ", ";
    stream << obj.z << "\n";
    return stream;
}

int main()
{
    three_d a(1, 2, 3), b(3, 4, 5), c(5, 6, 7);
    ofstream out("threed");

    if(!out)
    {
        cout << "Не удается открыть файл.";
        return 1;
    }

    out << a << b << c;
    out.close();
    return 0;
}
