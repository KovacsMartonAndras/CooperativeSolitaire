#ifndef GAME_H
#define GAME_H

#include <vector>
#include <algorithm>
#include <random>
#include "deck.h" 

/*
Class that holds the state of the table
- helper_decks is a vector of length 8 representing 
    the 8 piles that are next to the main pile (Alakzat)
- main_decks is a vector of length 8 representing 
    the 8 main piles that need to be filled up
*/
class Table{
    public:
        std::vector<std::vector<Card>> main_decks;
        std::vector<std::vector<Card>> helper_decks; 
        Table():main_decks(8){}

}; // Move table class to seperate file

class Player {
public:
    std::string name;
    std::vector<Card> primary_deck; 
    std::vector<Card> secondary_deck;
    std::vector<Card> throwaway_deck;

    // Between [0,1], probality to throw on pile
    float tendency_to_throw; // Overall
    float tendency_to_throw_on_primary; 
    float tendency_to_throw_on_throwaway;

    // Constructor
    Player(const std::string& playerName,float overall_t=0,float primary_t=0,float throwaway_t=0) : 
    name(playerName), tendency_to_throw(overall_t),tendency_to_throw_on_primary(primary_t),
    tendency_to_throw_on_throwaway(throwaway_t) {
    }
    // Decision making functions
    bool handle_throw_on_opponent();
    bool handle_throw_on_opponent_primary();
    bool handle_throw_on_opponent_throwaway();
    // Helper functions
    bool hand_empty();
    bool get_decision(float probability);
    void reshuffle_throwaway_to_secondary();

};

/*
Game class that initializes a game within its constructor,
implements the rules and simulates the game
*/
class Game {
public:
    Game(Player player_1, Player player_2, unsigned int max_turns = 500, bool debug_mode = false);
    bool DEBUGMODE;
    unsigned int MAX_TURNS;
    unsigned int current_player_index;
    unsigned int starter_player_index;

    void start_game();

    //Simulation functions
    bool perform_checks(Player* c_player);
    bool check_main_deck();  // Non dependent on current player
    bool check_deck(Player* c_player, std::vector<Card>& deck, std::string name);
    bool check_opponents_piles(Player* c_player);
    bool check_throw_availability(std::vector<Card>& source_deck,std::vector<Card>& target_deck, bool throw_flag);

    //Card management
    bool move_card_to_deck(std::vector<Card>& source_deck, std::vector<Card>& target_deck);

    // Helper functions
    Player* get_current_player();
    Player* get_other_player();


    //Prints
    void printTable();
    void printNumberOfCards(Player* c_player);
    void printHandsOfPlayer();
    void printMain();
    void printHelper();

    std::string cardValueToName(unsigned int value);
    std::string cardColorToName(unsigned int color);

private:
    Table table;
    std::vector<Player> players;
};


#endif