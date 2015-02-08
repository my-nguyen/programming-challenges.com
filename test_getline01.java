// test to see whether ifstream::getline() retains or discards the EOL char
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ifstream file("1.6.6.in");
    char line[80];
    while (file.getline(line, 80))
        cout << line << endl;
    return 0;
}
