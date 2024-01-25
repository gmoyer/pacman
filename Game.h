#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include "Entity.h"
#include <string>

#define BOARD_SIZE 10

using namespace std;

// you may change this enum as you need
enum class SquareType { Wall, Dots, Treasure, Empty };
enum class Direction { Up, Down, Left, Right };

string getSquareTypeString(SquareType s); //gets the string of the SquareType s

class Square {
public:
	Square(SquareType type); //initializes a square
	Entity* getOccupant(); //gets whoever is currently on the square (player/enemy)
	SquareType getType(); //gets the type of the square
	void changeType(SquareType newType); //change the type of the square (ex. Dots -> Empty)
	void changeOccupant(Entity* entity); //change the occupant of the square
private:
	Entity* occupant;
	SquareType type; 
};

class Board {
public:
	Board(); //creates a board by parsing a file

	SquareType getSquareType(Position pos); //returns the type of the square at a position
	void setSquareType(Position pos, SquareType type); //change the type of the square at a position
	Entity* getSquareOccupant(Position pos); //returns the occupant of the entity at a position

	vector<Direction> getValidMoves(Position pos); //get the valid moves from a position
	string getValidMovesString(Position pos); //get the valid moves but as a string

	Square move(Entity* entity, Position pos); //move entity to pos, return the new square the entity is on. All moves MUST go through this function

	void printBoard(); //prints the entire board

private:
	Square arr[BOARD_SIZE][BOARD_SIZE];
};

class Game {
public:
	Game(); //creates a new game with 5 enemies
	Game(const int enemyCount); // creates a new game with a specified number of enemies

	Board* getBoard(); //returns the board object, useful for Entity constructors

	Player* getPlayer(); //returns the player, useful for enemies

	void takeTurn(); //turn logic, performs player move and then performs enemy moves

	void killEnemy(Enemy* enemy); //delete enemy, remove its address from enemies array

	void gameOver(bool win); //ends the game, displays win or loss screen based on bool

	~Game(); //clean up memory
private:
	Board* board;
	Player* player;
	vector<Enemy*> enemies;
	int turnCount;
};

#endif  // _GAME_H_
