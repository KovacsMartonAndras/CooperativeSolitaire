#ifndef DECK_H
#define DECK_H

#include <vector>
#include <algorithm>
#include <random> 

struct Card {
    unsigned int value;
    unsigned int color;
};

class Deck {
public:
    Deck();
    void shuffle();
    void printDeck();

    std::string cardValueToName(unsigned int value);
    std::string cardColorToName(unsigned int color);
    std::vector<Card> cards;
};

#endif
