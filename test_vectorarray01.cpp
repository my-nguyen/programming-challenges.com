// test declaration and usage of a vector of array of 4 characters
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
using namespace std;


int main()
{
    vector<string> _instructions;
    ifstream file("1.6.6.tmp");
    char line[80];
    file.getline(line, 80);
    int count = atoi(line);

    int i = -1;
    while (file.getline(line, 80))
    {
        // blank line means the beginning of a new set (case) of input
        if (*line == '\0')
            i++;
        else
            // fetch the 3-digit instruction
            _instructions.push_back(string(line, 3));
    }
    file.close();


    cout << "OUTPUT" << endl;
    for (int i = 0; i < _instructions.size(); i++)
        cout << _instructions[i] << endl;
}
