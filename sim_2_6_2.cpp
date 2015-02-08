#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>  // RAND_MAX
#include <unistd.h> // optarg
#include "defs.h"
using namespace std;


#define NUM_VALUES 13
#define NUM_SUITS  4
#define NUM_CARDS  5


int _debug = DEBUG_OFF;


struct Card
{
    int  _value;
    char _suit;


    Card();
    void set(int val, char sut);
    char value();
    void print();
};


Card::Card()
{
    _value = 0;
    _suit = '-';
}


void Card::set(int val, char sut)
{
    _value = val;
    _suit = sut;
}


char Card::value()
{
    switch (_value)
    {
        case 1:
            return 'A';
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            return '0' + _value;
        case 10:
            return 'T';
        case 11:
            return 'J';
        case 12:
            return 'Q';
        case 13:
            return 'K';
        default:
            return '-';
    }
}


void Card::print()
{
    cout << value() << _suit;
}


struct Suit
{
    char _list[NUM_SUITS];
    int  _size;


    Suit();
    void print();
    bool search(char key);
    void remove(char elem);
};


Suit::Suit()
{
    _list[0] = 'C';
    _list[1] = 'D';
    _list[2] = 'H';
    _list[3] = 'S';
    _size = NUM_SUITS;
}


void Suit::print()
{
    for (int i = 0; i < _size; i++)
        cout << _list[i] << " ";
    cout << endl;
}


bool Suit::search(char key)
{
    for (int i = 0; i < _size; i++)
        if (_list[i] == key)
            return true;
    return false;
}


void Suit::remove(char elem)
{
    for (int i = 0; i < _size; i++)
        if (_list[i] == elem)
        {
            for (int j = i; j < _size-1; j++)
                _list[j] = _list[j+1];
            _size--;
            break;
        }
}


struct Hand
{
    Card _cards[NUM_CARDS];


    Hand();
    void print();
    bool search(int value, char suit);
};


Hand::Hand()
{
    Suit suit;
    for (int i = 0; i < NUM_CARDS; i++)
    {
        int value = rand() % NUM_VALUES + 1;
        while (true)
        {
            char siut = suit._list[rand() % suit._size];
            if (search(value, siut))
                suit.remove(siut);
            else
            {
                _cards[i].set(value, siut);
                break;
            }
        }
    }
}


void Hand::print()
{
    for (int i = 0; i < NUM_CARDS; i++)
    {
        _cards[i].print();
        cout << " ";
    }
    cout << endl;
}


bool Hand::search(int value, char suit)
{
    for (int i = 0; i < NUM_CARDS; i++)
        if (_cards[i]._value == value && _cards[i]._suit == suit)
            return true;

    return false;
}


int main(int argc, char** argv)
{
    int num_cases = 0;
    char* output_file = NULL;
    char opt;
    while ((opt = getopt(argc, argv, "c:o:d:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            num_cases = atoi(optarg);
            break;
        case 'o':
            output_file = optarg;
            break;
        case 'd':
            _debug = atoi(optarg);
            break;
        default:
            cout << "Usage: " << argv[0]
                 << " -c num_cases -o output_file [-d 0|1|2]" << endl;
        }
    }

    if (num_cases == 0 || output_file == 0 ||
        _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        cout << "Usage: " << argv[0]
             << " -c num_cases -o output_file [-d 0|1|2]" << endl;
    else
    {
        srand(time(NULL));
        ofstream file(output_file);


        for (int i = 0; i < num_cases; i++)
        {
            Hand black;
            Hand white;


            for (int j = 0; j < NUM_CARDS; j++)
            {
                file << black._cards[j].value() << black._cards[j]._suit << " ";
                if (_debug >= DEBUG_LIGHT)
                    cout << black._cards[j].value()
                         << black._cards[j]._suit << " ";
            }
            for (int j = 0; j < NUM_CARDS; j++)
            {
                file << white._cards[j].value() << white._cards[j]._suit << " ";
                if (_debug >= DEBUG_LIGHT)
                    cout << white._cards[j].value()
                         << white._cards[j]._suit << " ";
            }
            file << endl;
            if (_debug >= DEBUG_LIGHT)
                cout << endl;
        }

        file.close();
    }

    return 0;
}
