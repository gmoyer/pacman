#include <iostream>
#include "Game.h"
#include "Entity.h"

using namespace std;


int main() {
    //time_t t = time(NULL);
    srand(5);//time(NULL));
    Game* game = new Game();

    try{ 
        game->startGame();
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    } catch (...) {
        cout << "Something unknown happened" << endl;
    }
}