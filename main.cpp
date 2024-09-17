#include <iostream>
#include "game.h"

int main() {
    Player p_1;
    Player p_2;
    Game game(p_1,p_2);
    game.start_game();

    return 0;
}
