#include <stdio.h>
#include <string.h> // strtok() & strcpy()
#include <stdlib.h> // qsort()
#include <unistd.h> // optarg
#include "defs.h"


int _debug = DEBUG_OFF;


typedef int bool;
#define false 0
#define true  1


#define NUM_CARDS 5


struct Card
{
    int  _value;
    char _suit;
};
typedef struct Card Card;


void c_init(Card* card, char* input)
{
    switch (input[0])
    {
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            card->_value = input[0] - '0';
            break;
        case 'T':
            card->_value = 10;
            break;
        case 'J':
            card->_value = 11;
            break;
        case 'Q':
            card->_value = 12;
            break;
        case 'K':
            card->_value = 13;
            break;
        case 'A':
            card->_value = 1;
            break;
    }

    card->_suit = input[1];
    if (_debug >= DEBUG_COPIOUS)
        printf("Input: %s, Value: %d, Suit: %c\n",
               input, card->_value, card->_suit);
}


Card* c_copy(Card* left, Card* right)
{
    left->_value = right->_value;
    left->_suit = right->_suit;
    return left;
}


void c_print(Card* card)
{
    char output[3];

    switch (card->_value)
    {
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            output[0] = card->_value + '0';
            break;
        case 10:
            output[0] = 'T';
            break;
        case 11:
            output[0] = 'J';
            break;
        case 12:
            output[0] = 'Q';
            break;
        case 13:
            output[0] = 'K';
            break;
        case 1:
            output[0] = 'A';
            break;
    }

    output[1] = card->_suit;
    output[2] = '\0';
    printf("%s", output);
}


struct Hand
{
    Card _icards[NUM_CARDS]; // cards, as read from input
    Card _scards1[NUM_CARDS]; // sorted cards, where A has a value of 1
    Card _scards14[NUM_CARDS]; // sorted cards, where A has a value of 14
};
typedef struct Hand Hand;


Hand* h_copy(Hand* this, Hand* that)
{
    int i;
    for (i = 0; i < NUM_CARDS; i++)
    {
        c_copy(&this->_icards[i], &that->_icards[i]);
        c_copy(&this->_scards1[i], &that->_scards1[i]);
        c_copy(&this->_scards14[i], &that->_scards14[i]);
    }
    return this;
}


struct Pair
{
    int _first;
    int _second;
};
typedef struct Pair Pair;


int compare(const void* lhs, const void* rhs)
{
    int left = ((const Card*)lhs)->_value;
    int right = ((const Card*)rhs)->_value;
    return left - right;
}


void h_init(Hand* hand, char** token)
{
    // save the hand of 5 cards, as entered by input
    int i;
    for (i = 0; i < NUM_CARDS; i++)
    {
        c_init(&hand->_icards[i], *token);
        *token = strtok(0, " \n");
    }


    // the internal sorted hand of 5 cards
    for (i = 0; i < NUM_CARDS; i++)
        c_copy(&hand->_scards1[i], &hand->_icards[i]);
    qsort(&hand->_scards1, NUM_CARDS, sizeof(Card), compare);


    // another internal sorted hand, where Ace has a value of 14 instead of 1
    for (i = 0; i < NUM_CARDS; i++)
        c_copy(&hand->_scards14[i], &hand->_scards1[i]);
    if (hand->_scards14[0]._value == 1)
    {
        hand->_scards14[0]._value = 14;
        qsort(&hand->_scards14, NUM_CARDS, sizeof(Card), compare);
    }
}


void h_print(Hand* hand)
{
    int i;
    for (i = 0; i < NUM_CARDS; i++)
    {
        c_print(&hand->_icards[i]);
        printf(" ");
    }
}


struct Input
{
    Hand _black;
    Hand _white;
};
typedef struct Input Input;


void i_init(Input* input, char* line)
{
    char tmp[MAX_LINE];
    strcpy(tmp, line);
    char* token = strtok(tmp, " ");
    h_init(&input->_black, &token);
    h_init(&input->_white, &token);
}


void i_print(Input* input)
{
    printf("BLACK: ");
    h_print(&input->_black);
    printf("\n");
    printf("WHITE: ");
    h_print(&input->_white);
    printf("\n");
}


struct InputList
{
    Input _list[5000];
    int   _size;
};
typedef struct InputList InputList;



void il_free(InputList* list)
{
}


void p_init(Pair* pair, int input, int output)
{
    pair->_first = input;
    pair->_second = output;
}


Pair* p_copy(Pair* this, Pair* that)
{
    this->_first = that->_first;
    this->_second = that->_second;
    return this;
}


struct Rank
{
    Hand _hand;
    Pair _pair;
};
typedef struct Rank Rank;


void r_init(Rank* rank, Hand* hand, Pair* pair)
{
    h_copy(&rank->_hand, hand);
    p_copy(&rank->_pair, pair);
}


// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called after is_straight_flush(), is_four_of_a_kind(),
//     is_full_house(), is_flush(), is_straight(), is_three_of_a_kind(),
//     is_two_pairs() and is_pair() have been called
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first records the value of the highest card in
//     _second records nothing
bool is_high_card(Hand* hand, Pair* pair)
{
    pair->_first = hand->_scards14[NUM_CARDS-1]._value;
    return true;
}


// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called after is_straight_flush(), is_four_of_a_kind(),
//     is_full_house(), is_flush(), is_straight(), is_three_of_a_kind(), and
//     is_two_pairs() have been called
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first acts as input, so that this function must look for some pair
//         other than the pair given in input. So if a pair is already found,
//         by is_two_pairs(), for example, then this function must look for
//         a different pair. If no pair has ever been found, then input has
//         a value of 0.
//     _second acts as output, to record the value of the pair found here.
bool is_pair(Hand* hand, Pair* pair)
{
    int i;
    for (i = 0; i < 4; i++)
        if (hand->_scards14[i]._value == hand->_scards14[i+1]._value &&
            hand->_scards14[i]._value != pair->_first)
        {
            pair->_second = hand->_scards14[i]._value;
            return true;
        }
    return false;
}


// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called after is_straight_flush(), is_four_of_a_kind(),
//     is_full_house(), is_flush(), is_straight() and is_three_of_a_kind() have
//     been called
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first records the value of the first pair found
//     _second records the value of the second pair found
bool is_two_pairs(Hand* hand, Pair* pair)
{
    // find the first pair
    Pair tmp;
    p_init(&tmp, 0, 0);
    if (is_pair(hand, &tmp))
    {
        // if found, use the value of the first pair as input to look for
        // a second pair, so is_pair will know to look for a different pair 
        p_init(pair, tmp._second, 0);
        return is_pair(hand, pair);
    }
    else
        return false;
}


// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called after is_straight_flush(), is_four_of_a_kind(),
//     is_full_house(), is_flush(), and is_straight() have been called
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first records the value of the three of a kind found
//     _second records nothing
bool is_three_of_a_kind(Hand* hand, Pair* pair)
{
    int i;
    for (i = 0; i < 3; i++)
        if (hand->_scards1[i]._value == hand->_scards1[i+2]._value)
        {
            pair->_first = hand->_scards1[i]._value;
            return true;
        }
    return false;
}


// NOTE: An ACE can be either a 1 or a 14 for purpose of a straight
// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called after is_straight_flush(), is_four_of_a_kind(),
//     is_full_house() and is_flush() have been called
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first records the value of the highest card in a straight
//     _second records nothing
bool is_straight(Hand* hand, Pair* pair)
{
    int i;
    for (i = 0; i < NUM_CARDS-1; i++)
        if (hand->_scards1[i+1]._value - hand->_scards1[i]._value != 1)
        {
            int j;
            for (j = 0; j < NUM_CARDS-1; j++)
               if (hand->_scards14[j+1]._value - hand->_scards14[j]._value != 1)
                    return false;

            pair->_first = hand->_scards14[NUM_CARDS-1]._value;
            return true;
        }

    pair->_first = hand->_scards1[NUM_CARDS-1]._value;
    return true;
}


// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called after is_straight_flush(), is_four_of_a_kind(),
//     and is_full_house() have been called
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first records the value of the highest card in a flush
//     _second records nothing
bool is_flush(Hand* hand, Pair* pair)
{
    int i;
    for (i = 1; i < NUM_CARDS; i++)
        if (hand->_scards14[i]._suit != hand->_scards14[0]._suit)
            return false;

    if (hand->_scards1[NUM_CARDS-1]._value == 5)
        pair->_first = 5;
    else
        pair->_first = hand->_scards14[NUM_CARDS-1]._value;
    return true;
}


// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called after is_straight_flush() and is_four_of_a_kind()
//     have been called
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first records the value of the three of a kind
//     _second records the value of the pair
bool is_full_house(Hand* hand, Pair* pair)
{
    if (is_three_of_a_kind(hand, pair))
        return is_pair(hand, pair);
    else
        return false;
}


// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called after is_straight_flush() has been called
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first records the value of the four of a kind
//     _second records nothing
bool is_four_of_a_kind(Hand* hand, Pair* pair)
{
    int i;
    for (i = 0; i < 2; i++)
        if (hand->_scards14[i]._value == hand->_scards14[i+3]._value)
        {
            pair->_first = hand->_scards14[i]._value;
            return true;
        }
    return false;
}


// Assumptions:
// (1) Hand::_scards is sorted and A in Hand::_scards has a value of 14
// (2) This function is called before all other is_() functions
// Arguments:
// (1) hand is the current hand
// (2) pair has 2 integers: _first and _second
//     _first records the value of the highest card in the straight
//     _second records nothing
bool is_straight_flush(Hand* hand, Pair* pair)
{
    return is_straight(hand, pair) && is_flush(hand, pair);
}


bool input(char* input_file, InputList* input)
{
    int success;
    FILE* file = fopen(input_file, "r");
    if (file == NULL)
    {
        printf("Cannot open file %s\n", input_file);
        success = false;
    }
    else
    {
        char line[MAX_LINE];
        int i;
        for (i = 0; fgets(line, MAX_LINE, file); i++)
        {
            // read 10 card values
            Input* current = &input->_list[i];
            i_init(current, line);
            if (_debug >= DEBUG_LIGHT)
                i_print(current);
        }


        input->_size = i;
        fclose(file);
        success = true;
    }

    return success;
}


struct IFunctions
{
    bool (*_list[9])(Hand*, Pair*);
    int _size;
    char _names[9][20];
};
typedef struct IFunctions IFunctions;


void f_init(IFunctions* functions)
{
    if (functions->_list[0] == 0)
    {
        functions->_list[8] = &is_straight_flush;
        functions->_list[7] = &is_four_of_a_kind;
        functions->_list[6] = &is_full_house;
        functions->_list[5] = &is_flush;
        functions->_list[4] = &is_straight;
        functions->_list[3] = &is_three_of_a_kind;
        functions->_list[2] = &is_two_pairs;
        functions->_list[1] = &is_pair;
        functions->_list[0] = &is_high_card;

        functions->_size = 9;

        strcpy(functions->_names[8], "straight flush");
        strcpy(functions->_names[7], "four of a kind");
        strcpy(functions->_names[6], "full house");
        strcpy(functions->_names[5], "flush");
        strcpy(functions->_names[4], "straight");
        strcpy(functions->_names[3], "three of a kind");
        strcpy(functions->_names[2], "two pairs");
        strcpy(functions->_names[1], "pair");
        strcpy(functions->_names[0], "high card");
    }
}


// This function determines the rank of a hand; the rank is represented by
// an integer, the highest being a straight flush with a value of 8, down to
// just high card with a value of 0. In addition, the function records and
// returns some relevant information about the hand, as follows:
//     straight flush: the integer value of the highest card
//     four of a kind: the integer value of the four of a kind
//     full house: the integer values of the trips and the pair
//     flush: the integer value of the highest card
//     straight: the integer value of the highest card
//     three of a kind: the integer value of the trips
//     two pairs: the integer values of each of the pairs
//     one pair: the integer value of the pair
//     high card: the integer value of the highest card
int h_find_rank(Hand* hand, Pair* data)
{
    static IFunctions functions;
    f_init(&functions);


    int i;
    for (i = functions._size-1; i >= 0; i--)
        if (functions._list[i](hand, data))
        {
            if (_debug >= DEBUG_LIGHT)
            {
                h_print(hand);
                printf("%s\n", functions._names[i]);
            }
            return i;
        }
}


int compare_high_card(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    int i;
    for (i = NUM_CARDS-1; i >= 0; i--)
        if (bhand->_scards14[i]._value != whand->_scards14[i]._value)
            return bhand->_scards14[i]._value - whand->_scards14[i]._value;
    return 0;
}


int compare_pair(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    return bpair->_first - wpair->_first;
}


int compare_two_pairs(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    if (bpair->_first != wpair->_first)
        return bpair->_first - wpair->_first;
    else
        return bpair->_second - wpair->_second;
}


int compare_three_of_a_kind(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    if (bpair->_first != wpair->_first)
        return bpair->_first - wpair->_first;
    else
        return bpair->_second - wpair->_second;
}


// This function works like a C compare function, in that, if the first
// object is smaller than the second object, then a negative value is returned
// If the first object is bigger, then a positive is returned. And when the
// two objects equal, then zero is returned

// Must do 2 things:
// (1) revise logic to take advantage of the values in the pair returned
//     from the h_find_rank() so that we don't have to reinvent the wheel
// (2) pass Pair as an argument, together with Hand, rather than compacting
//     the 2 fields Hand and Pair into a new struct (Rank, that is)
int compare_straight(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    return bpair->_first - wpair->_first;
/*
    // When A is given the value of 1 in _scards1, T-J-Q-K-A will be
    // represented as A-T-J-Q-K, and _scards1[0]._value is 1, so that
    // A-T-J-Q-K is deemed smaller than 2-3-4-5-6 or any other straight.
    // Therefore, must compare the 2 straights when A is assigned the value
    // of 14 as well.
    if (bpair->_scards1[0]._value < wpair->_scards1[0]._value)
        return bpair->_scards14[0]._value < wpair->_scards14[0]._value;

    // likewise, when A has the value of 1, then 2-3-4-5-6 will be deemed
    // greater than A-T-J-Q-K. Therefore, must compare the 2 straights when
    // A has the value of 14
    else if (bpair->_scards1[0]._value > wpair->_scards1[0]._value)
        return bpair->_scards14[0]._value > wpair->_scards14[0]._value;
    else
        return 0;
*/
}


int compare_flush(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    return bpair->_first - wpair->_first;
}


int compare_full_house(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    if (bpair->_first != wpair->_first)
        return bpair->_first - wpair->_first;
    else
        return bpair->_second - wpair->_second;
}


int compare_four_of_a_kind(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    return bpair->_first - wpair->_first;
}


int compare_straight_flush(Hand* bhand, Pair* bpair, Hand* whand, Pair* wpair)
{
    return compare_straight(bhand, bpair, whand, wpair);
}


int h_compare_hands(Hand* bhand, Pair* bdata,
                    Hand* whand, Pair* wdata, int rank)
{
    int (*functions[9])(Hand*, Pair*, Hand*, Pair*);
    functions[8] = &compare_straight_flush;
    functions[7] = &compare_four_of_a_kind;
    functions[6] = &compare_full_house;
    functions[5] = &compare_flush;
    functions[4] = &compare_straight;
    functions[3] = &compare_three_of_a_kind;
    functions[2] = &compare_two_pairs;
    functions[1] = &compare_pair;
    functions[0] = &compare_high_card;

    return functions[rank](bhand, bdata, whand, wdata);
}


int h_compare(Hand* bhand, Hand* whand)
{
    Pair bpair;
    p_init(&bpair, 0, 0);
    Pair wpair;
    p_init(&wpair, 0, 0);


    int brank = h_find_rank(bhand, &bpair);
    int wrank = h_find_rank(whand, &wpair);
    if (brank > wrank)
        printf("Black wins.\n");
    else if (brank < wrank)
        printf("White wins.\n");
    else
    {
/*
        brank = h_compare_hands(bhand, &bpair, whand, &wpair, brank);
        if (brank > wrank)
            printf("Black wins.\n");
        else if (brank < wrank)
            printf("White wins.\n");
        else
*/
            printf("Tie.\n");
    }
}


void output(InputList* input)
{
    int i;
    for (i = 0; i < input->_size; i++)
        h_compare(&input->_list[i]._black, &input->_list[i]._white);

    il_free(input);
}


void main(int argc, char** argv)
{
    char opt;
    char* input_file = NULL;

    while ((opt = getopt(argc, argv, "i:d:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            input_file = optarg;
            break;
        case 'd':
            _debug = atoi(optarg);
            break;
        default:
            printf("Usage: %s -i input_file [-d 0|1|2]\n", argv[0]);
        }
    }

    if (input_file == NULL || _debug < DEBUG_OFF || _debug > DEBUG_COPIOUS)
        printf("Usage: %s -i input_file [-d 0|1|2]\n", argv[0]);
    else
    {
        InputList cases;
        if (input(input_file, &cases) != 0)
            output(&cases);
    }
}
