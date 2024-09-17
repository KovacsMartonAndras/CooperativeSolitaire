#include "game.h"
#include <iostream>
#include <algorithm>  
#include <utility>

Game::Game(Player p_1,Player p_2)
{
   
    players.push_back(p_1);
    players.push_back(p_2);

    // TODO Determine who goes first
    std::cout << "Dealing cards" << std::endl;

    unsigned int player_count = 0; // Used as an offset in the init of helper decks
    // 13 cards to primary
    for(Player& player : players)
    {
        Deck deck;
        unsigned int offset;
        player_count == 0 ? offset = 0 : offset = 4;
        // Shuffle decks
        std::cout << "Shuffling decks" << std::endl;
        deck.shuffle();

        for(unsigned int i=0; i < 13 ; i++)
        {
            player.primary_deck.push_back(deck.cards.back());
            deck.cards.pop_back();
        }
        std::cout << "Number of cards in "<< player.name <<" primary deck: " << player.primary_deck.size() << std::endl;
        for(unsigned int i=0; i < 4 ; i++)
        {
            table.helper_decks.push_back(std::vector<Card>());  // Add empty vector for new deck
            table.helper_decks.at(i+offset).push_back(deck.cards.back());
            deck.cards.pop_back();
        }
        std::cout << "Number of cards in "<< player.name <<" helper deck: " << table.helper_decks.size() << std::endl;

        // Rest to the secondary pile
        while(!deck.cards.empty())
        {
            player.secondary_deck.push_back(deck.cards.back());
            deck.cards.pop_back();
        }
        std::cout << "Number of cards in "<< player.name <<" secondary deck: " << player.secondary_deck.size() << std::endl;
        player_count += 1;
    }

    printTable();
    std::cout << "Game is ready" << std::endl;
}

void Game::start_game()
{
    // Player 1 starts
    int current_player_index = 1;
    Player* c_player = &players.at(!current_player_index);

    // Both Players are not out of the game
    while(!(players.at(0).hand_empty() || players.at(1).hand_empty()))
    {
        // Player 1 starts
        c_player = &players.at(!current_player_index);
        current_player_index = !current_player_index;  // Invert current player index
        //std::cout<< "Current Player: " << current_player_index << std::endl;
        while(perform_checks(c_player)); // Perform checks and move cards until no move is possible
    }
    std::cout<< "End of game" << std::endl;

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

    // No moves -> move secondary to throwaway pile
    if(!moved)
    {
        c_player->move_card_to_throwaway();
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
                    main_deck.push_back(helper_deck.back());
                    helper_deck.pop_back();  // Remove moved card
                    return true;
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
                    main_deck.push_back(helper_deck.back());
                    helper_deck.pop_back();  // Remove moved card
                    return true;
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
                main_deck.push_back(*current_card);
                c_player->primary_deck.pop_back(); // Remove moved card
                return true;
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
            main_deck.push_back(*current_card);
            c_player->primary_deck.pop_back(); // Remove moved card
            return true;
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
                helper_deck.push_back(*current_card);
                c_player->primary_deck.pop_back(); // Remove moved card
                return true;
            }
        }
        else
        {
            //Place is empty, simply put it out
            helper_deck.push_back(*current_card);
            c_player->primary_deck.pop_back(); // Remove moved card
            return true;
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
                main_deck.push_back(*current_card);
                c_player->secondary_deck.pop_back(); // Remove moved card
                return true;
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
            main_deck.push_back(*current_card);
            c_player->secondary_deck.pop_back(); // Remove moved card
            return true;
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
            helper_deck.push_back(*current_card);
            c_player->secondary_deck.pop_back(); // Remove card from secondary pile
            return true;
        }
        }
        else if (helper_deck.empty() && c_player->primary_deck.empty())
        {
            //Place is empty and primary deck is also empty, simply put it out
            helper_deck.push_back(*current_card);
            c_player->secondary_deck.pop_back(); // Remove moved card
        }
        
        else{
            std::cout<< "error in placement logic" << std::endl;
        }
    }    
    return false;
}


void Game::printTable(){
    printHandsOfPlayer();
    printMain();
    printHelper();
    
};

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

void Player::move_card_to_throwaway()
{
    throwaway_deck.push_back(secondary_deck.back());
    secondary_deck.pop_back();
}

int Player::handle_throw_on_opponent(Table table)
{
    return 0; // Don't throw card
}