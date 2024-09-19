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

};

class Player {
public:
    std::string name;
    std::vector<Card> primary_deck; 
    std::vector<Card> secondary_deck;
    std::vector<Card> throwaway_deck;

    float tendency_to_throw;
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
    void reshuffle_throwaway_to_secondary();

};

/*
Game class that initializes a game within its constructor,
implements the rules and simulates the game
*/
class Game {
public:
    Game(Player player_1, Player player_2, unsigned int max_turns = 200, bool debug_mode = false);
    bool DEBUGMODE;
    unsigned int MAX_TURNS;
    // 
    void start_game();
    bool perform_checks(Player* c_player);
    bool check_main_deck();  // Non dependent on current player
    bool check_primary(Player* c_player);
    bool check_secondary(Player* c_player);
    bool check_opponents_piles(Player* c_player);
    bool move_card_to_deck(std::vector<Card>& source_deck, std::vector<Card>& target_deck);

    // Helper functions
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