#include "game.h"
#include <iostream>
#include <algorithm>  
#include <utility>

Game::Game(Player p_1,Player p_2)
{

    // Since these decks are not needed after dealing it's not added to game class
    Deck deck_a;
    Deck deck_b;
    players.push_back(p_1);
    players.push_back(p_2);

    // Shuffle decks
    std::cout << "Shuffling decks" << std::endl;
    deck_a.shuffle();
    deck_b.shuffle();
    deck_a.printDeck();
    deck_b.printDeck();

    // TODO Determine who goes first

     std::cout << "Dealing cards" << std::endl;
    // 13 cards to primary
    for( unsigned int i=0; i < 13 ; i++){
        table.player_1.at(0).push_back(deck_a.cards.back());
        table.player_2.at(0).push_back(deck_b.cards.back());
        deck_a.cards.pop_back();
        deck_b.cards.pop_back();
    }
    std::cout << "Number of cards in player 1 primary deck: " << table.player_1.at(0).size() << std::endl;
    std::cout << "Number of cards in player 2 primary deck: " << table.player_2.at(0).size() << std::endl;
    
    // 4 cards to the sides in the middle
    for(unsigned int i=0; i < 4 ; i++){
        table.helper_decks.at(i).push_back(deck_a.cards.back());
        table.helper_decks.at(i + 4).push_back(deck_b.cards.back());
        deck_a.cards.pop_back();
        deck_b.cards.pop_back();
    }

    // Rest to the secondary pile
    while(!deck_a.cards.empty())
    {
        table.player_1.at(1).push_back(deck_a.cards.back());
        table.player_2.at(1).push_back(deck_b.cards.back());
        
        deck_a.cards.pop_back();
        deck_b.cards.pop_back();
    }
    printTable();
    std::cout << "Game is ready" << std::endl;
}

void Game::start_game()
{
    // Player 1 starts
    int current_player_index = 1;
    Player c_player = players.at(!current_player_index);
    // Both a deck is not empty
    while(!(table.player_1_hand_empty() || table.player_2_hand_empty()))
    {
        // Player 1 starts
        c_player = players.at(!current_player_index);
        current_player_index = !current_player_index;
        //std::cout<< "Current Player: " << current_player_index << std::endl;
        while(perform_checks(current_player_index)); // Perform checks and move cards until no move is possible
    }
    printTable();

}
bool Game::perform_checks(int c_player_index)
{
    bool moved = false;
    // Loop through main_decks
    //std::cout<< "Performing Main Deck checks" << std::endl;
    moved = check_main_deck(c_player_index);
    
    if(!moved){
        // check_primary
        //std::cout<< "Performing Primary Deck checks" << std::endl;
        moved = check_primary(c_player_index);
    }
    
    if(!moved){
       // check_secondary
        //std::cout<< "Performing Secondary Deck checks" << std::endl;
        moved = check_secondary(c_player_index); 
    }
    
    // This is where checking if throw to opponents pile would be checked and the handle function from the player is called

    // No moves -> move secondary to throwaway pile
    if(!moved)
    {
        //std::cout<< "Moving secondary to throwaway Pile" << std::endl;
        if(c_player_index == 0)
        {
            if (table.player_1.at(1).size() == 1) // Reshuffle necessary
            {
                // Move last card to throwaway pile
                table.player_1.at(2).push_back(table.player_1.at(1).back());
                table.player_1.at(1).pop_back();

                table.player_1.at(1).reserve(table.player_1.at(2).size());

                // Move elements in reverse order from the source to the destination
                std::move(table.player_1.at(2).rbegin(), table.player_1.at(2).rend(), std::back_inserter(table.player_1.at(1)));

                // Clear the source vector (optional, since std::move leaves source in a valid but unspecified state)
                table.player_1.at(2).clear();
                //std::cout<< "Reshuffled throwawy to secondary" << std::endl;
            }
            else
            {
                // Move last card to throwaway pile
                //std::cout<< "Moving secondary to throwaway Pile" << std::endl;
                table.player_1.at(2).push_back(table.player_1.at(1).back());
                table.player_1.at(1).pop_back();
            }
        }
        else{
            if (table.player_2.at(1).size() == 1) // Reshuffle necessary
            {
                // Move last card to throwaway pile
                table.player_2.at(2).push_back(table.player_2.at(1).back());
                table.player_2.at(1).pop_back();

                table.player_2.at(1).reserve(table.player_2.at(2).size());

                // Move elements in reverse order from the source to the destination
                std::move(table.player_2.at(2).rbegin(), table.player_2.at(2).rend(), std::back_inserter(table.player_2.at(1)));

                // Clear the source vector (optional, since std::move leaves source in a valid but unspecified state)
                table.player_2.at(2).clear();
                //std::cout<< "Reshuffled throwawy to secondary" << std::endl;
            }
            else
            {
                // Move last card to throwaway pile
                //std::cout<< "Moving secondary to throwaway Pile" << std::endl;
                table.player_2.at(2).push_back(table.player_2.at(1).back());
                table.player_2.at(1).pop_back();
            }
        }
    }

    return moved;
}

bool Game::check_main_deck(int c_player_index)
{
    for (auto& helper_deck : table.helper_decks){
        for (auto& main_deck : table.main_decks){
            if (!main_deck.empty())
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
            else if(helper_deck.back().value == 1 && main_deck.empty())
            {
                // Can fill main deck from helper
                std::cout<< "Moved " << cardColorToName(helper_deck.back().color) << " " << cardValueToName(helper_deck.back().value)  << " from helper deck to main deck."<< std::endl;
                main_deck.push_back(helper_deck.back());
                helper_deck.pop_back();  // Remove moved card
                return true;
            }
        }    
    }
    return false;
}

bool Game::check_primary(int c_player_index){
   
    if((c_player_index == 0 && table.player_1.at(0).empty()) || (c_player_index == 1 && table.player_2.at(0).empty()))
    {
        return false;
    }

    Card* current_card = nullptr;
    if(c_player_index == 0)
    {
        current_card = &table.player_1.at(0).back();
    }
    else if(c_player_index == 1)
    {
        current_card = &table.player_2.at(0).back();
    }

    if(current_card == nullptr)
    {
        std::cout<<"Invalid pointer encountered";
        return false;
    }
    //std::cout<<"Current Card: " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) << std::endl;
    // Checking Main deck for available moves
    for (auto& main_deck : table.main_decks){
            if (!main_deck.empty())
            {
            if (current_card->color == main_deck.back().color &&
            current_card->value == (main_deck.back().value + 1))
            {
                // Can fill main deck from helper
                std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) 
                << " from primary deck onto " << cardColorToName(main_deck.back().color) << " " << cardValueToName(main_deck.back().value) << " in main deck"<< std::endl;
                main_deck.push_back(*current_card);
                c_player_index == 0 ? table.player_1.at(0).pop_back(): table.player_2.at(0).pop_back(); // Remove moved card

                return true;
            }
            }
            else
            {
                if(current_card->value == 1) // Ace is current card 
                {
                    std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) << " from primary deck to main deck"<< std::endl;
                    main_deck.push_back(*current_card);
                    c_player_index == 0 ? table.player_1.at(0).pop_back(): table.player_2.at(0).pop_back(); // Remove moved card
                    return true;
                }
            }
        }    

    // Checking helper decks for available moves
    for (auto& helper_deck : table.helper_decks){
                if (!helper_deck.empty())
                {
                if ((current_card->color % 2 == 0 ^ helper_deck.back().color % 2 == 0) &&
                current_card->value == (helper_deck.back().value - 1))
                {
                    // Can fill helper_deck from primary
                    std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) 
                    << " onto " << cardColorToName(helper_deck.back().color) << " " << cardValueToName(helper_deck.back().value) << " from primary deck to helper deck"<< std::endl;
                    helper_deck.push_back(*current_card);
                    c_player_index == 0 ? table.player_1.at(0).pop_back(): table.player_2.at(0).pop_back(); // Remove moved card
                    return true;
                }
                }
            }    
    return false;
}
bool Game::check_secondary(int c_player_index)
{

    if((c_player_index == 0 && table.player_1.at(1).empty()) || (c_player_index == 1 && table.player_2.at(1).empty()))
    {
        return false;
    }
    Card* current_card = nullptr;
    if(c_player_index == 0)
    {
        current_card = &table.player_1.at(1).back();
    }
    else if(c_player_index == 1)
    {
        current_card = &table.player_2.at(1).back();
    }

    for (auto& main_deck : table.main_decks){
            if (!main_deck.empty())
            {
            if (current_card->color == main_deck.back().color &&
            current_card->value == (main_deck.back().value + 1))
            {
                // Can fill main deck from helper
                std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) 
                << " from primary deck onto " << cardColorToName(main_deck.back().color) << " " << cardValueToName(main_deck.back().value) << " in main deck"<< std::endl;
                main_deck.push_back(*current_card);
                c_player_index == 0 ? table.player_1.at(1).pop_back(): table.player_2.at(1).pop_back(); // Remove moved card

                return true;
            }
            }
            else
            {
                if(current_card->value == 1) // Ace is current card 
                {
                    std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) << " from secondary deck to main deck"<< std::endl;
                    main_deck.push_back(*current_card);
                    c_player_index == 0 ? table.player_1.at(1).pop_back(): table.player_2.at(1).pop_back(); // Remove moved card
                    return true;
                }
            }
        }    

    // Checking helper decks for available moves
    for (auto& helper_deck : table.helper_decks){
                if (!helper_deck.empty())
                {
                if ((current_card->color % 2 == 0 ^ helper_deck.back().color % 2 == 0) &&
                current_card->value == (helper_deck.back().value - 1))
                {
                    // Can fill helper_deck from secondary
                    std::cout<< "Moved " << cardColorToName(current_card->color) << " " << cardValueToName(current_card->value) 
                    << " onto " << cardColorToName(helper_deck.back().color) << " " << cardValueToName(helper_deck.back().value) << " from secondary deck to helper deck"<< std::endl;
                    helper_deck.push_back(*current_card);
                    c_player_index == 0 ? table.player_1.at(1).pop_back(): table.player_2.at(1).pop_back(); // Remove moved card
                    return true;
                }
                }
            }    
    return false;
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
        if (!table.player_1.at(0).empty())
        {
            std::cout << cardColorToName(table.player_1.at(0).back().color) << " " << cardValueToName(table.player_1.at(0).back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }
        
        std::cout << "Player 1 Secondary" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (!table.player_1.at(1).empty())
        {
        std::cout << cardColorToName(table.player_1.at(1).back().color) << " " << cardValueToName(table.player_1.at(1).back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }

        std::cout << "Player 1 Throwaway" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (!table.player_1.at(2).empty())
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
        if (!table.player_2.at(0).empty())
        {
            std::cout << cardColorToName(table.player_2.at(0).back().color) << " " << cardValueToName(table.player_2.at(0).back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }
        
        std::cout << "Player 2 Secondary" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (!table.player_2.at(1).empty())
        {
        std::cout << cardColorToName(table.player_2.at(1).back().color) << " " << cardValueToName(table.player_2.at(1).back().value) << std::endl;
        }
        else{
            std::cout << "Empty" << std::endl;
        }

        std::cout << "Player 2 Throwaway" << "-*-*-*-*-*-*-*-*-*" << std::endl;
        if (!table.player_2.at(2).empty())
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

bool Table::player_1_hand_empty()
{
    return player_1.at(0).empty() && player_1.at(1).empty() && player_1.at(2).empty();
}
bool Table::player_2_hand_empty()
{
    return player_2.at(0).empty() && player_2.at(1).empty() && player_2.at(2).empty();
}
int Player::handle_throw_on_opponent(Table table)
{
    return 0; // Don't throw card
}