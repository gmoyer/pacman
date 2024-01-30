#include <string>
#include "Entity.h"
#include "Game.h"

using namespace std;



//==============Entity=========================











//==================Player=====================
Player::Player() {
    pos = Position{0, 0};
    treasure = false;
    lives = 3;
    points = 0;
}





//================Enemy========================
Enemy::Enemy(Position pos_) {
    pos = pos_;
}