#include "Game.h"
#include "Entity.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

SquareType getSquareTypeFromChar(char c) {
    switch (c) {
        case ' ':
            return SquareType::Dots;
        case 'X':
            return SquareType::Wall;
        default:
            return SquareType::Empty;
    }
}

//=============SQUARE FUNCTIONS================

//Square::Square(): type(SquareType::Empty), occupant(nullptr) {} 

Square::Square(Position pos_, SquareType type_): 
    pos(pos_), 
    type(type_), 
    occupant(nullptr) {}

Entity* Square::getOccupant() const {
    return occupant;
}

SquareType Square::getType() const {
    return type;
}

bool Square::isFillValid() {
    return  (type != SquareType::Wall) && 
            (occupant == nullptr);
}
bool Square::isMoveValid() {
    return  (type != SquareType::Wall) && 
            (occupant == nullptr || occupant->getType() != EntityType::Enemy);
}

void Square::changeType(SquareType newType) {
    type = newType;
}

void Square::changeOccupant(Entity* entity) {
    occupant = entity;
}

char Square::getChar() {
    if (occupant != nullptr && occupant->getType() == EntityType::Player)
        return 'P';
    if (occupant != nullptr && occupant->getType() == EntityType::Enemy)
        return 'E';

    switch (type) {
        case SquareType::Wall:
            return 'X';
        case SquareType::Dots:
            return '.';
        case SquareType::Treasure:
            return 'T';
        default: //for empty
            return ' ';
    }
}



//=============BOARD FUNCTIONS=================

Board::Board(string inputFile): inFile(inputFile) {
    parseBoard();
}

Square* Board::getSquare(Position pos) {
    return arr[pos.row][pos.col];
}

Position Board::getRandomEmptySpace() {
    Position pos;
    int tolerance = 1000;
    while (tolerance > 0) {
        pos = Position{rand() % 10, rand() % 10};
        if (getSquare(pos)->isFillValid())
            return pos;
        tolerance--;
    }
    throw runtime_error("Failed to find empty space in 1000 tries");
}

void Board::parseBoard() {
    ifstream file(inFile);
    if (!file.is_open()) {
        throw runtime_error("Failed to open " + inFile);
    }

    string line;
    int row = 0;
    int col = 0;
    while (getline(file, line)) {
        col = 0;
        for (char c : line) {
            arr[row][col] = new Square(Position{row, col}, getSquareTypeFromChar(c));
            col++;
        }
        row++;
    }
}

vector<Enemy*> Board::populateBoard(int enemyCount) {
    vector<Enemy*> enemies;
    for (int i = 0; i < enemyCount; i++) {
        Position newPos = getRandomEmptySpace();
        Enemy* newEnemy = new Enemy(newPos);
        getSquare(newPos)->changeOccupant(newEnemy);
        enemies.push_back(newEnemy);
        Position treasure = getRandomEmptySpace();
        getSquare(treasure)->changeType(SquareType::Treasure);
    }
    return enemies;
}

vector<Direction> Board::getValidMoves(Position pos) {
    vector<Direction> validDirections;

    //up
    if (pos.row > 0            && getSquare(Position{pos.row-1, pos.col  })->isMoveValid())
        validDirections.push_back(Direction::Up);
    //down
    if (pos.row < BOARD_SIZE-1 && getSquare(Position{pos.row+1, pos.col  })->isMoveValid())
        validDirections.push_back(Direction::Down);
    //left
    if (pos.col > 0            && getSquare(Position{pos.row  , pos.col-1})->isMoveValid())
        validDirections.push_back(Direction::Left);
    //right
    if (pos.col < BOARD_SIZE-1 && getSquare(Position{pos.row  , pos.col+1})->isMoveValid())
        validDirections.push_back(Direction::Right);

    return validDirections;
}

string Board::getValidMovesString(Position pos) {
    vector<Direction> validDirections = getValidMoves(pos);
    stringstream ss;

    ss << "Valid moves are: ";
    for (Direction direction : validDirections) {
        switch (direction) {
            case Direction::Up:
                ss << "Up";
                break;
            case Direction::Down:
                ss << "Down";
                break;
            case Direction::Left:
                ss << "Left";
                break;
            case Direction::Right:
                ss << "Right";
                break;
        }
        ss << " ";
    }
    ss << endl;
    return ss.str();
}

Square* Board::move(Entity* entity, Position pos) {
    Square* oldSquare = getSquare(entity->getPosition());
    Square* newSquare = getSquare(pos);

    oldSquare->changeOccupant(nullptr);
    newSquare->changeOccupant(entity);

    return newSquare;
}

string Board::printBoard() {
    stringstream ss;
    for (int row = 0; row < BOARD_SIZE; row++) {
        ss << BOARD_INDENT;
        for (int col = 0; col < BOARD_SIZE; col++) {
            ss << getSquare(Position{row, col})->getChar();
        }
        ss << endl;
    }
    return ss.str();
}


//=============GAME FUNCTIONS=================


//constructors
Game::Game() {
    setup("board.txt", 5);
}
Game::Game(string inputFile) {
    setup(inputFile, 5);
}
Game::Game(const int enemyCount) {
    setup("board.txt", enemyCount);
}
Game::Game(string inputFile, const int enemyCount) {
    setup(inputFile, enemyCount);
}


void Game::setup(string inputFile, const int enemyCount) {
    board = new Board(inputFile);

    player = new Player();
    board->getSquare(Position{0, 0})->changeOccupant(player);

    board->populateBoard(enemyCount);
}