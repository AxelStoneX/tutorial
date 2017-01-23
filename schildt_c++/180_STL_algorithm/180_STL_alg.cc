#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

bool isvowel(char ch)
{
    ch = tolower(ch);
    if(ch == 'q' || ch == 'e' || ch == 'y' || ch == 'u'
                 || ch == 'o' || ch == 'j' || ch == 'i'
                 || ch == 'a'                           )
        return true;
    return false;
}

int main()
{
    char str[] = "STL-programming is a power!";
    vector<char> v;
    unsigned int i;

    for(i=0; str[i]; i++) v.push_back(str[i]);

    cout << "Последовательность: ";
    for(i=0; i<v.size(); i++) cout << v[i];
    cout << endl;

    int n;
    n = count(v.begin(), v.end(), 'm');
    cout << n << " симовла m\n";

    n = count_if(v.begin(), v.end(), isvowel);
    cout << n << " символов представляют гласные звуки.";

    return 0;
}
