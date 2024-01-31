#include <iostream>
#include "Game.h"
#include "Entity.h"

using namespace std;


int main() {
    //srand (time(NULL));
    Game* game = new Game();

    game->startGame();
}