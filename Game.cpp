#include "Game.h"
#include "Entity.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <algorithm>

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
bool Square::isMoveValid(bool isPlayer) {
    if (isPlayer)
        return  (type != SquareType::Wall);
    else
        return  (type != SquareType::Wall) && 
            (occupant == nullptr || occupant->getType() == EntityType::Player); //enemies can move to player
    
}

void Square::changeType(SquareType newType) {
    type = newType;
}

void Square::changeOccupant(Entity* entity) {
    occupant = entity;
}

string Square::getStr() {
    if (occupant != nullptr && occupant->getType() == EntityType::Player) {
        if (static_cast<Player*>(occupant)->hasTreasure())
            return "O"; //like the player has their mouth open
        else
            return "P";
    }
    if (occupant != nullptr && occupant->getType() == EntityType::Enemy)
        return "E";

    switch (type) {
        case SquareType::Wall:
            return "X";
        case SquareType::Dots:
            return "-";
        case SquareType::Treasure:
            return "T";
        default: //for empty
            return " ";
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
        pos = Position{rand() % BOARD_SIZE, rand() % BOARD_SIZE};
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

vector<Direction> Board::getValidMoves(Position pos, bool isPlayer) {
    vector<Direction> validDirections;

    //up
    if (pos.row > 0            && getSquare(Position{pos.row-1, pos.col  })->isMoveValid(isPlayer))
        validDirections.push_back(Direction::Up);
    //down
    if (pos.row < BOARD_SIZE-1 && getSquare(Position{pos.row+1, pos.col  })->isMoveValid(isPlayer))
        validDirections.push_back(Direction::Down);
    //left
    if (pos.col > 0            && getSquare(Position{pos.row  , pos.col-1})->isMoveValid(isPlayer))
        validDirections.push_back(Direction::Left);
    //right
    if (pos.col < BOARD_SIZE-1 && getSquare(Position{pos.row  , pos.col+1})->isMoveValid(isPlayer))
        validDirections.push_back(Direction::Right);

    return validDirections;
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
            ss << getSquare(Position{row, col})->getStr() << ' ';
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
    turnCount = 0;
    board = new Board(inputFile);
    statusDisplay = "";

    player = new Player();
    board->getSquare(Position{0, 0})->changeOccupant(player);

    enemies = board->populateBoard(enemyCount);
}

void Game::startGame() {
    
    while (player->getLives() != 0 && enemies.size() != 0) {
        turnCount++;
        cout << printState(statusDisplay);
        statusDisplay = "";
        playerTurn();
        cout << "a" << endl;
        enemiesTurn();
        cout << "b" << endl;
        //check if there is a collision
        Entity* occupant = board->getSquare(player->getPosition())->getOccupant();
        if (occupant != nullptr && occupant->getType() == EntityType::Enemy)
            playerCollide(static_cast<Enemy*>(occupant));
        cout << "c" << endl;
    }

    if (player->getLives() == 0) { //lose
        cout << printState("You lose!");
    } else { //win
        cout << printState("You win!");
    }
}

string Game::validMovesString(vector<Direction> validDirections) {
    stringstream ss;

    ss << "Valid moves are: ";
    for (size_t i = 0; i < validDirections.size(); i++) {
        switch (validDirections[i]) {
            case Direction::Up:
                ss << "Up (w)";
                break;
            case Direction::Down:
                ss << "Down (s)";
                break;
            case Direction::Left:
                ss << "Left (a)";
                break;
            case Direction::Right:
                ss << "Right (d)";
                break;
        }
        if (i != validDirections.size() - 1)
            ss << ", ";
    }
    return ss.str();
}

string Game::printState(string custom) {
    stringstream ss;
    ss << endl << endl << endl << endl << endl << endl;
    ss << endl << endl << endl << custom << endl << endl; //seperate space
    ss << "==================== Turn " << turnCount << " ====================" << endl;
    ss << board->printBoard() << endl;

    ss << "Points: " << player->getPoints() << endl;

    Position oldPos = player->getPosition();
    vector<Direction> playerValidMoves = board->getValidMoves(oldPos, true);

    ss << validMovesString(playerValidMoves) << endl;

    return ss.str();
}

void Game::playerTurn() {
    Position oldPos = player->getPosition();
    vector<Direction> playerValidMoves = board->getValidMoves(oldPos, true);
    Position newPos;
    int tolerance = 100;
    while (tolerance > 0) {
        newPos = player->takeTurn(playerValidMoves);
        if (newPos == oldPos) { //player did not choose valid move
            cout << printState("Please choose a valid move!");
        } else {
            break;
        }
        tolerance--;
    }
    

    //display new turn after player input
    
    Entity* newOccupant = board->getSquare(newPos)->getOccupant();

    if (newOccupant != nullptr && newOccupant->getType() == EntityType::Enemy)
            playerCollide(static_cast<Enemy*>(newOccupant));

    //manage board state
    switch (board->getSquare(newPos)->getType()) {
        case SquareType::Dots:
            player->addPoints(1);
            board->getSquare(newPos)->changeType(SquareType::Empty);
            break;
        case SquareType::Treasure:
            if (!player->hasTreasure()) {
                player->setTreasure(true);
                player->addPoints(100);
                statusDisplay = "You collected a treasure! Now eat an enemy";
                board->getSquare(newPos)->changeType(SquareType::Empty);
            }
            break;
        default:
            break;
    }

    moveEntity(player, newPos);
}

void Game::enemiesTurn() {
    for (Enemy* enemy : enemies) {
        Position oldPos = enemy->getPosition();
        vector<Direction> validMoves = board->getValidMoves(oldPos, false);
        Position newPos = enemy->takeTurn(validMoves, player->getPosition());
        moveEntity(enemy, newPos);
    }
}

void Game::moveEntity(Entity* entity, Position newPos) {
    Position oldPos = entity->getPosition();

    board->getSquare(oldPos)->changeOccupant(nullptr);
    entity->setPosition(newPos);
    board->getSquare(newPos)->changeOccupant(entity);
}

void Game::playerCollide(Enemy* enemy) {

    if (player->hasTreasure()) { //eat enemy

        auto it = find(enemies.begin(), enemies.end(), enemy);
        enemies.erase(it);

        board->getSquare(player->getPosition())->changeOccupant(player);

        player->setTreasure(false);

        statusDisplay = "You ate an enemy!";
        cout << "3" << endl;
    } else { //take damage
        player->takeDamage();
        statusDisplay = "You took damage! You are now at " + to_string(player->getLives()) + " lives.";
        Position newPos = board->getRandomEmptySpace();
        moveEntity(player, newPos);
    }
}