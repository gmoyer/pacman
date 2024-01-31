#include <iostream>
#include "Game.h"
#include "Entity.h"

using namespace std;


int main() {
    //srand (time(NULL));
    Game* game = new Game();

    try{ 
        game->startGame();
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    }
}