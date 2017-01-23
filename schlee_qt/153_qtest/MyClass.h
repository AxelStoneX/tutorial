#ifndef MYCLASS_H
#define MYCLASS_H

// ===========================================================================
class MyClass
{
public:
    int min(int n1, int n2)
    {
        return n1 < n2 ? n1 : n2;
    }

    int max(int n1, int n2)
    {
        return n1 > n2 ? n1 : n2;
    }
};

#endif // MYCLASS_H
