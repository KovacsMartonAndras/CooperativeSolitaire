#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "deck.h"

// Forward declarations(?)
class Game;
class Player;

class Logger{
    public:
        Logger();
        void log(const std::string& logmessage);
        std::string log_filename; // File to keep logs in
        void set_game_instance(Game* game_instance);

        std::string log_current_player_rounds = "-1";
        std::string log_table_symbol = "-2";
        std::string empty_deck_symbol = "_";
        void log_table_state();
        void log_mains();
        void log_helpers();
        void log_player_stats(Player* player);
        
        void log_current_player_start(unsigned int player_index);
        void log_current_player_end();
        

        void log_player_hand(Player* player);
        std::string log_card(Card* card);
    private:
        Game* game;
        static int log_counter;
};

#endif