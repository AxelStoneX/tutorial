#include <iostream>
using namespace std;

class three_d
{
public:
    int x, y, z;
    three_d(int a, int b, int c) { x = a; y = b; z = c; }
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
    cout << a << b << c;
    return 0;
}
