#include <iostream>
using namespace std;

const int SIZE = 100;

template <class QType>
class queue {
    QType q[SIZE];
    int sloc, rloc;
public:
    queue() { sloc = rloc = 0; }
    void qput(QType i);
    QType qget();
};

template <class QType>
void queue<QType>::qput(QType i)
{
    if(sloc==SIZE) {
        cout << "Очередь заполнена.\n";
        return;
    }
    sloc++;
    q[sloc] = i;
}

template <class QType>
QType queue<QType>::qget()
{
    if(rloc == sloc) {
        cout << "Очередь пуста.\n";
        return 0;
    }
    rloc++;
    return q[rloc];
}

int main()
{
    queue<int> a, b;

    a.qput(10);
    b.qput(19);
    a.qput(20);
    b.qput(1);

    cout << a.qget() << " ";
    cout << a.qget() << " ";
    cout << b.qget() << " ";
    cout << b.qget() << "\n";

    queue<double> c, d;

    c.qput(10.12);
    d.qput(19.99);
    c.qput(-20.0);
    d.qput(0.986);

    cout << c.qget() << " ";
    cout << c.qget() << " ";
    cout << d.qget() << " ";
    cout << d.qget() << "\n";
    return 0;
}
