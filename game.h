#ifndef GAME_H
#define GAME_H

#include <vector>
#include <algorithm>
#include <random>
#include "deck.h" 

/*
Class that holds the state of the table
- player_1 is a vector of length 3 representing 
    - 0: Primary pile (Segédtalon)
    - 1: Secondary pile (Talon)
    - 2: Throwaway pile (Domb)
    (Same for player 2)
- helper_decks is a vector of length 8 representing 
    the 8 piles that are next to the main pile (Alakzat)
- main_decks is a vector of length 8 representing 
    the 8 main piles that need to be filled up
*/
class Table{
    public:
        std::vector<std::vector<Card>> player_1; 
        std::vector<std::vector<Card>> player_2; 
        std::vector<std::vector<Card>> helper_decks; 

        std::vector<std::vector<Card>> main_decks;

        // Constructor for the two helper decks on the sides and the main deck
        Table():player_1(3),player_2(3),helper_decks(8),main_decks(8){}
};

/*
Game class that initializes a game within its constructor,
implements the rules and simulates the game
*/
class Game {
public:
    Game();


    // Helper functions
    void printTable();
    void printHandsOfPlayer(unsigned int playerID);
    void printMain();
    void printHelper();

    std::string cardValueToName(unsigned int value);
    std::string cardColorToName(unsigned int color);

private:
    Table table;
};


#endif