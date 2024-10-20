#include <iostream>
#include "game.h"

int main() {
    Player p_1("Player 1",0.0,0,1.0);
    Player p_2("Player 2",1.0,1.0,1.0);
    unsigned int nr_of_games = 1;
    unsigned int p_1_cnt = 0;
    unsigned int p_2_cnt = 0;
    unsigned int tie_cnt = 0;
    unsigned int unfinished_cnt = 0;
    int result;

    for (unsigned int game_cnt = 0; game_cnt < nr_of_games; game_cnt++)
    {
        Game game(p_1,p_2,true);
        result = game.start_game();
        if (result == -1)
        {
            unfinished_cnt += 1;
        }
        else if (result == 0)
        {
            tie_cnt += 1;
        }
        else if (result == 1)
        {
            p_1_cnt += 1;
        }
        else
        {
            p_2_cnt += 1;
        }
    }
    std::cout << "Player 1 wins: " << p_1_cnt  << ", "<< std::to_string(p_1_cnt/nr_of_games) <<std::endl;
    std::cout << "Player 2 wins: " << p_2_cnt  <<  ", "<< std::to_string(p_2_cnt/nr_of_games) << std::endl;
    std::cout << "Tie: " << tie_cnt  << std::endl;
    std::cout << "Unfinished games: " << unfinished_cnt  << std::endl;

    return 0;
}
