#include <iostream>
using namespace std;

class three_d
{
    int x, y, z;
public:
    three_d(int a, int b, int c) { x = a; y = b; z = c; }
    friend ostream &operator<<(ostream &stream,
                               three_d obj     );
    friend istream &operator>>(istream &stream,
                               three_d &obj    );
};

ostream &operator<<(ostream &stream, three_d obj)
{
    stream << obj.x << ", ";
    stream << obj.y << ", ";
    stream << obj.z << "\n";
    return stream;
}

istream &operator>>(istream &stream, three_d &obj)
{
    cout << "Введите координаты X,Y и Z: \n";
    stream >> obj.x >> obj.y >> obj.z;
    return stream;
}

int main()
{
    three_d a(1, 2, 3);
    cout << a;
    cin >> a;
    cout << a;
    return 0;
}
