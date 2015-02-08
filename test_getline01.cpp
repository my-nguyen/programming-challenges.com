#include <iostream>
#include <fstream>
using namespace std;


int main()
{
    ifstream file("tmp");
    char line[80];
    file.getline(line, 80);
    cout << "read: \"" << line << "\"" << endl;
    file.close();
}
