#include "game.h"
#include <iostream>
#include <algorithm>  
#include <utility>
#include "logger.h"



Game::Game(Player p_1,Player p_2 , bool log_on, unsigned int max_turns, bool debugmode)
{
    players.push_back(p_1);
    players.push_back(p_2);

    DEBUGMODE = debugmode;
    LOG_ON  = log_on;
    MAX_TURNS = max_turns;

    std::random_device rd;
    std::mt19937 gen(rd());

    // Defines the range as the starting and ending index of the list of players
    std::uniform_int_distribution<> distr(0, players.size()-1);

    // Generate a random index from the list
    starter_player_index = distr(gen);
    // Make the current player the starting one
    current_player_index = starter_player_index;

    //if (LOG_ON) {logger.log("Test Log");};

    unsigned int player_count = 0; // Used as an offset in the init of helper decks
    // 13 cards to primary
    for(Player& player : players)
    {
        Deck deck;
        unsigned int offset;
        player_count == 0 ? offset = 0 : offset = 4;  // Used for dealing cards to helper decks

        // Shuffle decks
        deck.shuffle();

        for(unsigned int i=0; i < 13 ; i++)
        {
            move_card_to_deck(deck.cards,player.primary_deck);
        }
        for(unsigned int i=0; i < 4 ; i++)
        {
            table.helper_decks.push_back(std::vector<Card>());  // Add empty vector for new deck
            move_card_to_deck(deck.cards,table.helper_decks.at(i+offset)); // Using offset to have one unified helper deck instead of two

        }

        // Rest to the secondary pile
        while(move_card_to_deck(deck.cards,player.secondary_deck));
        player_count += 1;
    }
}

void Game::start_game()
{
    logger.set_game_instance(this); // Workaround for circular dependency
    Player* c_player = &players.at(current_player_index);
    
    // Start game loggings
    if (LOG_ON)
    {
        logger.log_player_stats(&players.at(0));
        logger.log_player_stats(&players.at(1));
        logger.log(std::to_string(current_player_index));
        logger.log_table_state();
    }

    unsigned int turn_counter = 0;
    bool infinite_game = false;

    // Both Players are not out of the game
    while(!(players.at(0).hand_empty() || players.at(1).hand_empty()))
    {
        if (turn_counter >= MAX_TURNS)
        {
            infinite_game = true;
            break;
        }

        run_current_player(c_player);
        //Player hand empty, current player didn't start, game ended
        if(c_player->hand_empty() && current_player_index != starter_player_index){
            break;
        }
        // Player hand empty, current player started, give turn to other player for chance to finish
        if(c_player->hand_empty() && current_player_index == starter_player_index){
            current_player_index = !current_player_index;
            c_player = &players.at(current_player_index);
            run_current_player(c_player);
            break;
        }
        // Switch player
        current_player_index = !current_player_index;  // Invert current player index
        c_player = &players.at(current_player_index);
        turn_counter += 1;
    }

    if (!infinite_game)
    {
        //std::cout<< "End of game" << std::endl;
        //std::cout << "Number of turns: " << turn_counter << std::endl;

        if(players.at(0).hand_empty() && players.at(1).hand_empty()){
            std::cout << "Tie" << std::endl;
            if(LOG_ON)
            {
                logger.log("Tie");
            }
        } else{
            players.at(0).hand_empty() ? std::cout << "Winner: Player 1" << std::endl : std::cout << "Winner: Player 2" << std::endl ;
            if(LOG_ON)
            {
               players.at(0).hand_empty() ? logger.log("Winner: Player 1")  : logger.log("Winner: Player 2");
            }
        }

    }
    else{
        std::cout<< turn_counter << " Game could not be finished" << std::endl;
        if (LOG_ON)
        {
            logger.log(" Game could not be finished");
        }
    }
}

// Performs the moves of the player until it can perform valid ones
void Game::run_current_player(Player* c_player){
    if (LOG_ON)
    {
        logger.log_current_player_start(current_player_index);
    }
    
    while(perform_checks(c_player) && !c_player->hand_empty());
    if (LOG_ON)
    {
        logger.log_current_player_end();
    }
    
}

bool Game::perform_checks(Player* c_player)
{
    bool moved = false;
    // Loop through main_decks
    moved = check_main_deck();
    
    if(!moved){
        // check_primary
        moved = check_deck(c_player, c_player->primary_deck, "primary");
    }
    
    
    if(!moved){
        // check_secondary
        moved = check_deck(c_player, c_player->secondary_deck, "secondary");
    }
    
    // This is where checking if throw to opponents pile would be checked and the handle function from the player is called

    if(!moved)
    {
        moved = check_opponents_piles(c_player);
    }
    // No moves -> move secondary to throwaway pile
    if(!moved)
    {
        move_card_to_deck(c_player->secondary_deck,c_player->throwaway_deck);
        if (c_player->secondary_deck.empty())
        {
            c_player->reshuffle_throwaway_to_secondary();
        }
    }
    if (moved && LOG_ON)
    {
        logger.log_table_state();
    }

    return moved;
}

bool Game::check_main_deck()
{
    for (auto& helper_deck : table.helper_decks){
        for (auto& main_deck : table.main_decks){
            if(helper_deck.empty())
            {
                continue; // Making sure the deck is accessible
            }
            if(helper_deck.back().value == 1) // TODO:Check back Here
            {
                if(!main_deck.empty())
                {
                    continue;
                }
                else{
                    // Can fill main deck from helper
                    return move_card_to_deck(helper_deck,main_deck);
                }
            }
            else if (!main_deck.empty())
            {
                if (helper_deck.back().color == main_deck.back().color &&
                helper_deck.back().value == (main_deck.back().value + 1))
                {
                    // Can fill main deck from helper
                    return move_card_to_deck(helper_deck,main_deck);
                }
            }
        }    
    }
    return false;
}


bool Game::check_deck(Player* c_player, std::vector<Card>& deck, std::string name) {
    if (deck.empty()) {
        return false;
    }

    Card* current_card = &deck.back();

    // Checking main decks for available moves
    for (auto& main_deck : table.main_decks) {
        if (current_card->value == 1) {
            if (!main_deck.empty()) {
                continue;
            } else {
                return move_card_to_deck(deck, main_deck);
            }
        } else if (!main_deck.empty()) {
            if (current_card->color == main_deck.back().color &&
                current_card->value == (main_deck.back().value + 1)) {
                return move_card_to_deck(deck, main_deck);
            }
        }
    }

    // Checking helper decks for available moves
    for (auto& helper_deck : table.helper_decks) {
        if (!helper_deck.empty()) {
            if ((current_card->color % 2 == 0 ^ helper_deck.back().color % 2 == 0) &&
                current_card->value == (helper_deck.back().value - 1)) {
                return move_card_to_deck(deck, helper_deck);
            }
        } else if (helper_deck.empty() && ((name == "primary") || ((name=="secondary") &&c_player->primary_deck.empty()))) {
            // Place is empty, and primary deck is also empty, simply put it out
            return move_card_to_deck(deck, helper_deck);
        }
    }

    return false;
}

bool Game::check_opponents_piles(Player* c_player)
{
    if(c_player->handle_throw_on_opponent()) // Overall throw rate
    {
        Player* other_player = get_other_player();
        bool moved = false;
        moved = check_throw_availability(c_player->primary_deck,other_player->primary_deck,c_player->get_decision(c_player->tendency_to_throw_on_primary));
        if(!moved)
        {
            moved = check_throw_availability(c_player->primary_deck,other_player->throwaway_deck,c_player->get_decision(c_player->tendency_to_throw_on_throwaway));
        }
        if(!moved)
        {
            moved = check_throw_availability(c_player->secondary_deck,other_player->primary_deck,c_player->get_decision(c_player->tendency_to_throw_on_primary));
        }
        if(!moved)
        {
            moved = check_throw_availability(c_player->secondary_deck,other_player->throwaway_deck,c_player->get_decision(c_player->tendency_to_throw_on_throwaway));
        }
        return moved;
    }
    return false; // No card is thrown

}

bool Game::check_throw_availability(std::vector<Card>& source_deck,std::vector<Card>& target_deck, bool throw_flag)
{
    if((source_deck.empty() || target_deck.empty()))
    {
        return false;
    }

    if(source_deck.back().color == target_deck.back().color && 
    (source_deck.back().value == (target_deck.back().value - 1)||
    source_deck.back().value == (target_deck.back().value + 1)))
    {
        // Can throw primary card onto other player's primary deck, No corner case of ace card
        // on opponent's primary since it would have been already placed out to main deck
        if(throw_flag)
        {
            return move_card_to_deck(source_deck,target_deck);
        }
    }
    return false;
}

Player* Game::get_current_player()
{
    return &players.at(current_player_index);
}
Player* Game::get_other_player()
{
    return &players.at(!current_player_index);
}

Table* Game::get_table()
{
    return &table;
}

void Game::printTable(){
    printHandsOfPlayer();
    printMain();
    printHelper();
    
}
void Game::printNumberOfCards(Player* c_player)
{
    std::cout<< c_player->name << std::endl;
    std::cout<< "Number of cards in primary deck: " << c_player->primary_deck.size() << std::endl;
    std::cout<< "Number of cards in secondary deck: " << c_player->secondary_deck.size() << std::endl;
    std::cout<< "Number of cards in throwaway deck: " << c_player->throwaway_deck.size() << std::endl;
    unsigned int nr_cards = c_player->primary_deck.size() + c_player->secondary_deck.size() + c_player->throwaway_deck.size();
    std::cout<< "Number of cards at current player: " << nr_cards << std::endl;
}
void Game::printHandsOfPlayer()
{
    for (Player player : players)
    {
        std::cout << player.name << "--------------------" << std::endl;

        std::cout << player.name << " Primary" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (!player.primary_deck.empty())
        {
            std::cout << cardColorToName(player.primary_deck.back().color) << " " << cardValueToName(player.primary_deck.back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }
        
        std::cout << player.name << " Secondary" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (!player.secondary_deck.empty())
        {
        std::cout << cardColorToName(player.secondary_deck.back().color) << " " << cardValueToName(player.secondary_deck.back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }

        std::cout << player.name << " Throwaway" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (!player.throwaway_deck.empty())
        {
            std::cout << cardColorToName(player.throwaway_deck.back().color) << " " << cardValueToName(player.throwaway_deck.back().value) << std::endl;
        }
         else{
            std::cout << "Empty" << std::endl;
        }
    }
    
};
void Game::printMain(){
    std::cout << "State of Main Decks" << "--------------------" << std::endl;
    for (auto deck : table.main_decks){
        if(!deck.empty())
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
        case 1: return "Ace";
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
/*
Move top card of source deck to target deck
return true on a success
false on failure or invalid move
*/
bool Game::move_card_to_deck(std::vector<Card>& source_deck, std::vector<Card>& target_deck)
{
    if(source_deck.empty())
    {
        return false;
    }
    // Move the top card from source_deck to target_deck
    target_deck.push_back(std::move(source_deck.back()));
    source_deck.pop_back();
    return true;
}


bool Player::hand_empty()
{
    return primary_deck.empty() && secondary_deck.empty() && throwaway_deck.empty();
}
void Player::reshuffle_throwaway_to_secondary()
{
    secondary_deck.reserve(throwaway_deck.size());

    // Move elements in reverse order from the source to the destination
    std::move(throwaway_deck.rbegin(), throwaway_deck.rend(), std::back_inserter(secondary_deck));

    // Clear the source vector
    throwaway_deck.clear();
}


bool Player::get_decision(float probability) {
    if (probability == 1)
    {
        return true;
    }
    else if (probability == 0){
        return false;
    }

    std::random_device rd;                         
    std::mt19937 generator(rd());              
    std::uniform_real_distribution<float> dist(0.0f, 1.0f); // Create a distribution between 0 and 1

    // Generate a random float and compare it with the probability
    return dist(generator) < probability;
}

bool Player::handle_throw_on_opponent()
{
    return get_decision(tendency_to_throw); // Don't throw card
}
bool Player::handle_throw_on_opponent_primary()
{
    return get_decision(tendency_to_throw_on_primary); // Don't throw card
}
bool Player::handle_throw_on_opponent_throwaway()
{
    return get_decision(tendency_to_throw_on_throwaway); // Don't throw card
}