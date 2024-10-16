#include "logger.h"
#include <iostream>

// Logging library
#include <fstream>
#include <string>

//Time
#include <iomanip>
#include <ctime>

#include "game.h"

std::string get_time()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H-%M-%S");
    auto str = oss.str();

    return str;
}
void Logger::log(const std::string& logmessage) {
    std::ofstream logFile(log_filename, std::ios_base::app); 
    if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        return;
    }
    logFile << logmessage << std::endl;
    logFile.close();
}

Logger::Logger()
    
{
    log_filename = "Game_Log_" + get_time() + ".txt";
}

void Logger::set_game_instance(Game* game_instance)
{
    game = game_instance;
}

void Logger::log_table_state()
{
    log(log_table_symbol);
    //Log players
    log_player_hand(game->get_current_player());
    log_player_hand(game->get_other_player());
    log_mains();
    log_helpers();
    log(log_table_symbol);
}
void Logger::log_mains()
{
    std::ostringstream oss;
    unsigned int iter_counter = 0;
    // Hard coded 8 for iteration since main number of main decks does not change

    oss << "(" ;
    for (auto& main_deck : game->get_table()->main_decks)
    {
        if(main_deck.empty())
        {
            oss << empty_deck_symbol;
        }
        else{
            oss << log_card(&(main_deck.back()));
        }
        if (iter_counter < 7)
        {
            oss << ";";
        }
        iter_counter += 1;
    }
    oss << ")";

    auto str = oss.str();
    log(str);
}
void Logger::log_helpers()
{
    std::ostringstream oss;
    unsigned int iter_counter = 0;
    // Hard coded 8 for iteration since main number of main decks does not change

    oss << "(" ;
    for (auto& helper_deck : game->get_table()->helper_decks)
    {
        if(helper_deck.empty())
        {
            oss << empty_deck_symbol;
        }
        else{
            oss << log_card(&(helper_deck.back()));
        }
        if (iter_counter < 7)
        {
            oss << ";";
        }
        iter_counter += 1;
    }
    oss << ")";

    auto str = oss.str();
    log(str);
}
void Logger::log_player_hand(Player* player)
{
    std::ostringstream oss;
    // Primary deck
    if(player->primary_deck.empty())
    {
        oss << "(" << empty_deck_symbol <<";";
    }
    else{
        oss << "(" << log_card(&(player->primary_deck.back())) <<";";
    }
    // Secondary deck
    if(player->secondary_deck.empty())
    {
        oss << empty_deck_symbol << ";";
    }
    else{
        oss << log_card(&(player->secondary_deck.back())) << ";";
    }
    if(player->throwaway_deck.empty()){
        oss << empty_deck_symbol <<")";
    }
    else{
        oss << log_card(&(player->throwaway_deck.back())) << ")";
    }

    auto str = oss.str();
    log(str);
}
std::string Logger::log_card(Card* card)
{   
    if (card == nullptr)
    {
        return "";
    }
    else
    {
        std::cout << game->cardColorToName(card->color);
        std::ostringstream oss;
        oss << "(" << card->color <<";"<< card->value << ")";
        auto str = oss.str();
        return str;
    }
    
}
void Logger::log_player_stats(Player* player)
{
    std::ostringstream oss;
    oss << "(" << player->tendency_to_throw << ";" << player->tendency_to_throw_on_primary << ";" << player->tendency_to_throw_on_throwaway << ")";
    auto str = oss.str();
    log(str);
}
void Logger::log_current_player_start(unsigned int player_index)
{
    std::ostringstream oss;
    oss << log_current_player_rounds << "," << std::to_string(player_index);
    auto str = oss.str();
    log(str); 
}
void Logger::log_current_player_end()
{
    std::ostringstream oss;
    oss << log_current_player_rounds;
    auto str = oss.str();
    log(str);
}