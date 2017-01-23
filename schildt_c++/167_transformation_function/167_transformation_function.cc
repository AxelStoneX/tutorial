#include <iostream>
using namespace std;

class three_d
{
    int x, y, z;
public:
    three_d(int a, int b, int c) { x = a; y = b; z = c; }
    three_d operator+ (three_d op2);
    friend ostream &operator<<(ostream &stream,
                               three_d &obj);
    operator int() { return x * y * z; }
};

ostream &operator<<(ostream &stream, three_d &obj)
{
    stream << obj.x << ", ";
    stream << obj.y << ", ";
    stream << obj.z << "\n";
    return stream;
}

three_d three_d::operator+(three_d op2)
{
    three_d temp(0, 0, 0);

    temp.x = x + op2.x;
    temp.y = y + op2.y;
    temp.z = z + op2.z;
    return temp;
}

int main()
{
    three_d a(1, 2, 3), b(2, 3, 4);
    cout << a << b;
    cout << b+100;
    cout << "\n";
    a = a + b;
    cout << a;
    return 0;
}
