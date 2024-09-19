#include "game.h"
#include <iostream>
#include <algorithm>  
#include <utility>

Game::Game(Player p_1,Player p_2 , unsigned int max_turns, bool debugmode)
{
   
    players.push_back(p_1);
    players.push_back(p_2);

    DEBUGMODE = debugmode;
    MAX_TURNS = max_turns;
    // TODO Determine who goes first TODO
    if (DEBUGMODE){std::cout << "Dealing cards" << std::endl;};

    unsigned int player_count = 0; // Used as an offset in the init of helper decks
    // 13 cards to primary
    for(Player& player : players)
    {
        Deck deck;
        unsigned int offset;
        player_count == 0 ? offset = 0 : offset = 4;  // Used for dealing cards to helper decks

        // Shuffle decks
        if (DEBUGMODE){std::cout << "Shuffling decks" << std::endl;};
        deck.shuffle();

        for(unsigned int i=0; i < 13 ; i++)
        {
            move_card_to_deck(deck.cards,player.primary_deck);
        }
        if (DEBUGMODE){std::cout << "Number of cards in "<< player.name <<" primary deck: " << player.primary_deck.size() << std::endl;};
        for(unsigned int i=0; i < 4 ; i++)
        {
            table.helper_decks.push_back(std::vector<Card>());  // Add empty vector for new deck
            move_card_to_deck(deck.cards,table.helper_decks.at(i+offset));

        }
        if (DEBUGMODE){std::cout << "Number of cards in "<< player.name <<" helper deck: " << table.helper_decks.size() << std::endl;};

        // Rest to the secondary pile
        while(move_card_to_deck(deck.cards,player.secondary_deck));
       
        if (DEBUGMODE){std::cout << "Number of cards in "<< player.name <<" secondary deck: " << player.secondary_deck.size() << std::endl;};
        player_count += 1;
    }

    if (DEBUGMODE){
        printTable();
        std::cout << "Game is ready" << std::endl;
    }
}

void Game::start_game()
{
    // Player 1 starts
    int current_player_index = 1;
    Player* c_player = &players.at(!current_player_index);
    unsigned int turn_counter = 0;
    bool infinite_game = false;

    // Both Players are not out of the game
    while(!(players.at(0).hand_empty() || players.at(1).hand_empty()))
    {
        if (turn_counter > MAX_TURNS)
        {
            infinite_game = true;
            break;
        }

        // Player 1 starts
        c_player = &players.at(!current_player_index);
        current_player_index = !current_player_index;  // Invert current player index

        //std::cout<< "Current Player: " << c_player->name << std::endl;
        while(perform_checks(c_player)); // Perform checks and move cards until no move is possible
        if (DEBUGMODE){printNumberOfCards(c_player);};
        turn_counter += 1;
    }

    if (!infinite_game)
    {
        std::cout<< "End of game" << std::endl;
    std::cout << "Number of turns: " << turn_counter << std::endl;
    players.at(0).hand_empty() ? std::cout << "Winner: Player 1" << std::endl : std::cout << "Winner: Player 2" << std::endl ;
    }
    else{
        std::cout<< "Game could not be finised" << std::endl;
    }
}
bool Game::perform_checks(Player* c_player)
{
    bool moved = false;
    // Loop through main_decks
    //std::cout<< "Performing Main Deck checks" << std::endl;
    moved = check_main_deck();
    
    if(!moved){
        // check_primary
        //std::cout<< "Performing Primary Deck checks" << std::endl;
        moved = check_primary(c_player);
    }
    
    
    if(!moved){
       // check_secondary
        //std::cout<< "Performing Secondary Deck checks" << std::endl;
        moved = check_secondary(c_player); 
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
        if (c_player->secondary_deck.size() == 0)
        {
            c_player->reshuffle_throwaway_to_secondary();
        }
    }
    
    return moved;
}

bool Game::check_main_deck()
{
    for (auto& helper_deck : table.helper_decks){
        for (auto& main_deck : table.main_decks){
            if(helper_deck.back().value == 1)
            {
                if(!main_deck.empty())
                {
                    continue;
                }
                else{
                    // Can fill main deck from helper
                    std::cout<< "Moved " << cardColorToName(helper_deck.back().color) << " " << cardValueToName(helper_deck.back().value)  << " from helper deck to main deck."<< std::endl;
                    return move_card_to_deck(helper_deck,main_deck);
                }
            }
            else if (!main_deck.empty())
            {
                if (helper_deck.back().color == main_deck.back().color &&
                helper_deck.back().value == (main_deck.back().value + 1))
                {
                    // Can fill main deck from helper
                    std::cout<< "Moved " << cardColorToName(helper_deck.back().color) << " " << cardValueToName(helper_deck.back().value) 
                    << " from helper deck onto " << cardColorToName(main_deck.back().color) << " " << cardValueToName(main_deck.back().value) << " in main deck" << std::endl;
                    return move_card_to_deck(helper_deck,main_deck);
                }
            }
        }    
    }
    return false;
}

bool Game::check_primary(Player* c_player){
   
    if(c_player->primary_deck.empty())
    {
        return false;
    }

    Card* current_card = &c_player->primary_deck.back();
   
    //std::cout<<"Current Card: " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) << std::endl;
    // Checking Main deck for available moves
    for (auto& main_deck : table.main_decks)
    {
        if(current_card->value == 1)
        {
            if(!main_deck.empty())
            {
                continue;
            }
            else{
                std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) << " from primary deck to main deck"<< std::endl;
                return move_card_to_deck(c_player->primary_deck,main_deck);
            }
        }
        else if (!main_deck.empty())
        {
            if (current_card->color == main_deck.back().color &&
            current_card->value == (main_deck.back().value + 1))
            {
            // Can fill main deck from helper
            std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) 
            << " from primary deck onto " << cardColorToName(main_deck.back().color) << " " << cardValueToName(main_deck.back().value) << " in main deck"<< std::endl;
            return move_card_to_deck(c_player->primary_deck,main_deck);
            }
        }
    }    

    // Checking helper decks for available moves
    for (auto& helper_deck : table.helper_decks)
    {
        if (!helper_deck.empty())
        {
            if ((current_card->color % 2 == 0 ^ helper_deck.back().color % 2 == 0) &&
            current_card->value == (helper_deck.back().value - 1))
            {
                // Can fill helper_deck from primary
                std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) 
                << " onto " << cardColorToName(helper_deck.back().color) << " " << cardValueToName(helper_deck.back().value) << " from primary deck to helper deck"<< std::endl;
                return move_card_to_deck(c_player->primary_deck,helper_deck);
            }
        }
        else
        {
            return move_card_to_deck(c_player->primary_deck,helper_deck);
        }
    }    
    return false;
}
bool Game::check_secondary(Player* c_player)
{

    if(c_player->secondary_deck.empty())
    {
        return false;
    }

    Card* current_card = &c_player->secondary_deck.back();
    
    for (auto& main_deck : table.main_decks)
    {
        if(current_card->value == 1)
        {
            if(!main_deck.empty())
            {
                continue;
            }
            else{
                std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) << " from secondary deck to main deck"<< std::endl;
                return move_card_to_deck(c_player->secondary_deck,main_deck);
            }
        }
        else if (!main_deck.empty())
        {
            if (current_card->color == main_deck.back().color &&
            current_card->value == (main_deck.back().value + 1))
            {
            // Can fill main deck from helper
            std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) 
            << " from secondary deck onto " << cardColorToName(main_deck.back().color) << " " << cardValueToName(main_deck.back().value) << " in main deck"<< std::endl;
             return move_card_to_deck(c_player->secondary_deck, main_deck);
            }
        }
    }    

    // Checking helper decks for available moves
    for (auto& helper_deck : table.helper_decks)
    {
        if (!helper_deck.empty())
        {
        if ((current_card->color % 2 == 0 ^ helper_deck.back().color % 2 == 0) &&
        current_card->value == (helper_deck.back().value - 1))
        {
            // Can fill helper_deck from secondary
            std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) 
            << " onto " << cardColorToName(helper_deck.back().color) << " " << cardValueToName(helper_deck.back().value) << " from secondary deck to helper deck"<< std::endl;
            return move_card_to_deck(c_player->secondary_deck,helper_deck);
        }
        }
        else if (helper_deck.empty() && c_player->primary_deck.empty())
        {
            //Place is empty and primary deck is also empty, simply put it out
             return move_card_to_deck(c_player->secondary_deck,helper_deck);
        }
    }    
    return false;
}
bool Game::check_opponents_piles(Player* c_player)
{
    //Check primary availability
    if(c_player->handle_throw_on_opponent())
    {
        if(c_player->primary_deck.empty())
        {
        return false;
        }
        //TODO...
    }
    return false; // No card is thrown

}

void Game::printTable(){
    printHandsOfPlayer();
    printMain();
    printHelper();
    
};

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

    //std::cout<< "Reshuffled throwaway to secondary" << std::endl;
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


bool get_decision(float probability) {
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