#include <iostream>
#include "game.h"

int main() {
    Player p_1("Player 1",1.0,0,1.0);
    Player p_2("Player 2",1.0,0,1.0);
    unsigned int nr_of_games = 10;
    for (unsigned int game_cnt = 0; game_cnt < nr_of_games; game_cnt++)
    {
        Game game(p_1,p_2,true);
        game.start_game(); 
    }
    return 0;
}
