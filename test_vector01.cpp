#include <iostream>
#include <vector>
using namespace std;


int main()
{
    vector<int> list;
    list.push_back(1);
    list.push_back(9);
    list.push_back(6);
    list.push_back(7);
    cout << "SIZE " << list.size() << endl;
    list.resize(0);
    cout << "SIZE " << list.size() << endl;
}
