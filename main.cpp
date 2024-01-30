#include <iostream>
#include "Game.h"
#include "Entity.h"

using namespace std;


int main() {
    //srand (time(NULL));
    Board board = Board();

    cout << "populating board" << endl;
    board.populateBoard(5);
    cout << "populated board" << endl;

    cout << board.printBoard() << endl;
}