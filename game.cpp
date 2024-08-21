#include "game.h"
#include <iostream>

Game::Game()
{
    // Since these decks are not needed after dealing it's not added to game class
    Deck deck_a;
    Deck deck_b;

    // Shuffle decks
    std::cout << "Shuffling decks" << std::endl;
    deck_a.shuffle();
    deck_b.shuffle();

    // TODO Determine who goes first

     std::cout << "Dealing cards" << std::endl;
    // 12 cards to dome
    for( unsigned int i=0; i < 12 ; i++){
        table.player_1.at(0).push_back(deck_a.cards.back());
        table.player_2.at(0).push_back(deck_b.cards.back());
        deck_a.cards.pop_back();
        deck_b.cards.pop_back();
    }

    // 4 cards to the sides in the middle
    for(unsigned int i=0; i < 4 ; i++){
        table.helper_decks.at(i).push_back(deck_a.cards.back());
        table.helper_decks.at(i + 4).push_back(deck_b.cards.back());
        deck_a.cards.pop_back();
        deck_b.cards.pop_back();
    }

    // Rest to the secondary pile
    while(deck_a.cards.size() != 0)
    {
        table.player_1.at(1).push_back(deck_a.cards.back());
        table.player_2.at(1).push_back(deck_b.cards.back());
        
        deck_a.cards.pop_back();
        deck_b.cards.pop_back();
    }

    std::cout << "Game is ready" << std::endl;
}


void Game::printTable(){
    printHandsOfPlayer(1);
    printHandsOfPlayer(2);
    printMain();
    printHelper();
    
};

void Game::printHandsOfPlayer(unsigned int playerID)
{
    // Check if player id is valid
    if (playerID != 1 && playerID != 2)
    {
        return;
    }
     
    if (playerID == 1)
    {
        std::cout << "Player 1" << "--------------------" << std::endl;

        std::cout << "Player 1 Primary" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (table.player_1.at(0).size() != 0)
        {
            std::cout << cardColorToName(table.player_1.at(0).back().color) << " " << cardValueToName(table.player_1.at(0).back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }
        
        std::cout << "Player 1 Secondary" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (table.player_1.at(1).size() != 0)
        {
        std::cout << cardColorToName(table.player_1.at(1).back().color) << " " << cardValueToName(table.player_1.at(1).back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }

        std::cout << "Player 1 Throwaway" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (table.player_1.at(2).size() != 0)
        {
            std::cout << cardColorToName(table.player_1.at(2).back().color) << " " << cardValueToName(table.player_1.at(2).back().value) << std::endl;
        }
         else{
            std::cout << "Empty" << std::endl;
        }
        
    }
    else{
        std::cout << "Player 2" << "--------------------" << std::endl;

        std::cout << "Player 2 Primary" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (table.player_2.at(0).size() != 0)
        {
            std::cout << cardColorToName(table.player_2.at(0).back().color) << " " << cardValueToName(table.player_2.at(0).back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }
        
        std::cout << "Player 2 Secondary" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (table.player_2.at(1).size() != 0)
        {
        std::cout << cardColorToName(table.player_2.at(1).back().color) << " " << cardValueToName(table.player_2.at(1).back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }

        std::cout << "Player 2 Throwaway" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (table.player_2.at(2).size() != 0)
        {
            std::cout << cardColorToName(table.player_2.at(2).back().color) << " " << cardValueToName(table.player_2.at(2).back().value) << std::endl;
        }
         else{
            std::cout << "Empty" << std::endl;
        }
    }
};

void Game::printMain(){
    std::cout << "State of Main Decks" << "--------------------" << std::endl;


    for (auto deck : table.main_decks){
        if(deck.size() != 0)
        {
            std::cout << cardColorToName(deck.back().color) << " " << cardValueToName(deck.back().value) << std::endl;
        }
    }
}

void Game::printHelper(){
    std::cout << "State of Helper Decks" << "--------------------" << std::endl;

    for (auto deck : table.helper_decks){
    std::cout << cardColorToName(deck.back().color) << " " << cardValueToName(deck.back().value) << std::endl;
        
    }
}

std::string Game::cardValueToName(unsigned int value){
    switch (value) {
        case 0: return "Ace";
        case 11: return "Jack";
        case 12: return "Queen";
        case 13: return "King";
        default: return std::to_string(value);
    }
}

std::string Game::cardColorToName(unsigned int color){
    switch (color) {
        case 0: return "Clubs";
        case 1: return "Diamonds";
        case 2: return "Spades";
        case 3: return "Hearts";
        default: return std::to_string(color);
    }
}