#include "deck.h"
#include <iostream>

Deck::Deck() {
    for (unsigned int color = 0; color < 4; ++color) {
        for (unsigned int value = 1; value < 14; ++value) {
            cards.push_back(Card{value, color});
        }
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::shuffle(cards.begin(), cards.end(), eng);
}

std::string Deck::cardValueToName(unsigned int value){
    switch (value) {
        case 0: return "Error in cards management"; // Debug
        case 1: return "Ace";
        case 11: return "Jack";
        case 12: return "Queen";
        case 13: return "King";
        default: 
            return std::to_string(value);
    }
}

std::string Deck::cardColorToName(unsigned int color){
    switch (color) {
        case 0: return "Clubs";
        case 1: return "Diamonds";
        case 2: return "Spades";
        case 3: return "Hearts";
        default: return "Color_name_error, Invalid Card";
    }
}

void Deck::printDeck(){
    for (const auto& card : cards) {
        std::cout << cardColorToName(card.color) << " " << cardValueToName(card.value) << std::endl;
    }
}

