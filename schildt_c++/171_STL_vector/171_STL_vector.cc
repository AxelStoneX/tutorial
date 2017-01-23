#include <iostream>
#include <vector>
using namespace std;

class three_d
{
    int x, y, z;
public:
    three_d() { x = y = z = 0; }
    three_d(int a, int b, int c) { x = a; y = b; z = c; }
    three_d &operator+(int a)
    {
        x +=a; y += a; z += a;
        return *this;
    }

    friend ostream &operator<< (ostream &stream, three_d obj);
    friend bool operator< (three_d a, three_d b);
    friend bool operator== (three_d a, three_d b);

};

ostream &operator<<(ostream &stream, three_d obj)
{
    stream << obj.x << ", ";
    stream << obj.y << ", ";
    stream << obj.z << "\n";
    return stream;
}

bool operator<(three_d a, three_d b)
{
    return (a.x + a.y + a.z) < (b.x + b.y + b.z);
}

bool operator==(three_d a, three_d b)
{
    return (a.x + a.y + a.z) == (b.x + b.y + b.z);
}

int main()
{
    vector<three_d> v;
    unsigned int i;

    for(i=0; i<10; i++)
        v.push_back(three_d(i, i+2, i+3));
    for(i=0; i<v.size(); i++)
        cout << v[i];
    cout << endl;

    for(i=0; i<v.size(); i++)
        v[i] = v[i] + 10;
    for(i=0; i<v.size(); i++)
        cout << v[i];

    return 0;
}
