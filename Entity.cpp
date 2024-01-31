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

void Player::addPoints(int p) {
    points += p;
}
void Player::setTreasure(bool t) {
    treasure = t;
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


Position Enemy::takeTurn(vector<Direction> validMoves, Position playerPos, bool hasTreasure) {
    vector<Direction> v = validMoves;
    Direction chosenDirection = Direction::None;

    //don't move if no valid directions
    if (validMoves.size() > 0) {

        //first choose a random directions
        chosenDirection = validMoves[rand() % validMoves.size()];

        //simple tracking AI
        int rowComp = pos.row - playerPos.row;
        int colComp = pos.col - playerPos.col;
        if (hasTreasure) {
            rowComp = -rowComp;
            colComp = -colComp;
        }
        if (pos.row == playerPos.row) {
            if (colComp > 0 && 
                (find(v.begin(), v.end(), Direction::Left) != v.end())) //only track if it is a valid option
                chosenDirection = Direction::Left;
            else if (
                (find(v.begin(), v.end(), Direction::Right) != v.end()))
                chosenDirection = Direction::Right;
        }
        if (pos.col == playerPos.col) {
            if (rowComp > 0 && 
                (find(v.begin(), v.end(), Direction::Up) != v.end()))
                chosenDirection = Direction::Up;
            else if (
                (find(v.begin(), v.end(), Direction::Down) != v.end()))
                chosenDirection = Direction::Down;
        }
    }
        
    
    switch (chosenDirection) {
        case Direction::Up:
            return Position{pos.row-1, pos.col};
        case Direction::Left:
            return Position{pos.row, pos.col-1};
        case Direction::Down:
            return Position{pos.row+1, pos.col};
        case Direction::Right:
            return Position{pos.row, pos.col+1};
        default:
            return Position{0, 0};
    }
}