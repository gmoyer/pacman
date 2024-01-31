#include <string>
#include "Entity.h"
#include "Game.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <conio.h> //for windows systems
#include <cstdio>

using namespace std;



//==============Entity=========================


void Entity::setPosition(Position p) {
    pos = p;
}




//==================Player=====================
Player::Player() {
    pos = Position{0, 0};
    treasure = false;
    lives = 3;
    points = 0;
}

bool Player::takeDamage() {
    lives -= 1;
    return lives == 0;
}

Position Player::takeTurn(vector<Direction> validDirections) {
    char input;

    vector<Direction> v = validDirections;

    input = _getch(); //windows systems only
    switch (input) {
        case 'w':
            if (find(v.begin(), v.end(), Direction::Up) != v.end())
                return Position{pos.row-1, pos.col};
            break;
        case 'a':
            if (find(v.begin(), v.end(), Direction::Left) != v.end())
                return Position{pos.row, pos.col-1};
            break;
        case 's':
            if (find(v.begin(), v.end(), Direction::Down) != v.end())
                return Position{pos.row+1, pos.col};
            break;
        case 'd':
            if (find(v.begin(), v.end(), Direction::Right) != v.end())
                return Position{pos.row, pos.col+1};
            break;
        default:
            break;
    }
    return pos; //no move chosen
}

//================Enemy========================
Enemy::Enemy(Position pos_) {
    pos = pos_;
}