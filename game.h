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
        bool player_1_hand_empty();
        bool player_2_hand_empty();
};

class Player {
public:
    // Decision making functions
    int handle_throw_on_opponent(Table table);

};

/*
Game class that initializes a game within its constructor,
implements the rules and simulates the game
*/
class Game {
public:
    Game(Player player_1, Player player_2);

    // 
    void start_game();
    bool perform_checks(int c_player_index);
    bool check_main_deck(int c_player_index);
    bool check_primary(int c_player_index);
    bool check_secondary(int c_player_index);

    // Helper functions
    void printTable();
    void printHandsOfPlayer(unsigned int playerID);
    void printMain();
    void printHelper();

    std::string cardValueToName(unsigned int value);
    std::string cardColorToName(unsigned int color);

private:
    Table table;
    std::vector<Player> players;
};


#endif